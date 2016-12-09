//
// Created by Angel Zaprianov on 11.10.2016 г..
//
#include <Arduino.h>

#ifndef SHUTDOWN_SAVE_STATE
#define SHUTDOWN_SAVE_STATE HIGH
#endif
#ifndef SHUTDOWN_SAVE_BUTTON
#define SHUTDOWN_SAVE_BUTTON 8
#endif

#ifndef SHUTDOWN_SAVE_LOOPS
#define SHUTDOWN_SAVE_LOOPS 1200
#endif

#ifndef SHUTDOWN_LOW_VALUE
#define SHUTDOWN_LOW_VALUE 500
#endif

#ifndef ARDUINOMID_SHUTDOWN_H
#define ARDUINOMID_SHUTDOWN_H


class ShutDw {


    IntAmp *_amp;

    EepRom *_eep;

    CarSens *_car;

    WhlSens *_whl;

private :
    uint8_t pinCtrl, pinDtct, pinTone;

    int indexWait = 0;
    int entryDisplay = 0;
    int alreadySaved = 0;
    int alreadyShutdown = 0;
    bool entryUsbDetected = false;
    bool isShutdownActive = false;
    bool isShutdownInactive = false;
    int detectorValue = 1000;
    int handlerCursorMenu;

    void resolveUsbActive(int _detectorValue, int _detectUsbAct);

    void displaySaved(LiquidCrystal *lcd);

    void displayCancel(LiquidCrystal *lcd);

public:
    static constexpr int MENU_SHUTDOWN = 99;

    ShutDw(EepRom *eepRom, IntAmp *ampInt, CarSens *carSens, WhlSens *whlSens);

    void setup(int pinControl, int pinDetect, int pinToAlarm);

    void listener();

    void lcdDisplay(LiquidCrystal *lcd);

    bool isUsbActive();

    void cursor(int &cursorMenu);

};


/***********************************************************************************************
 *                                                                                             *
 *                                   CPP part of file                                          *
 *                                                                                             *
 ***********************************************************************************************/


/**
 * Constructor of shutdown
  * @param eepRom
  * @param ampInt
  * @param carSens
  * @param whlSens
  */
ShutDw::ShutDw(EepRom *eepRom, IntAmp *ampInt, CarSens *carSens, WhlSens *whlSens) {
    _eep = eepRom;
    _amp = ampInt;
    _car = carSens;
    _whl = whlSens;
}

/**
 * Setup shutdown class
 */
void ShutDw::setup(int pinControl, int pinDetect, int pinToAlarm) {

    pinCtrl = uint8_t(pinControl);
    pinDtct = uint8_t(pinDetect);
    pinTone = uint8_t(pinToAlarm);


    //
    // Control save shutdown
    pinMode(pinCtrl, OUTPUT);
    analogWrite(pinCtrl, 255);
    //
    // Detect ignition key
    pinMode(pinDtct, INPUT);

}

/**
 * When mid is started and loop si lower number resolve as USB active
 */
void ShutDw::resolveUsbActive(int _detectorValue, int _detectUsbAct) {
    if (_detectorValue < 500 && _detectUsbAct < 50) {
        entryUsbDetected = true;
    }
}
/**
 *  Is mid started from USB power supply
 */
bool ShutDw::isUsbActive() {

    if (entryUsbDetected) {
        return true;
    }

    if (detectorValue < 500 && _amp->getLoopIndex() < 50) {
        entryUsbDetected = true;
        return true;
    }
}

/**
 * Handler the cursor menu
 */
void ShutDw::cursor(int &cursorMenu) {

    //
    // Switch menu if cannot shutdown ...
    if (alreadyShutdown == 1) {
        cursorMenu = 1;
        alreadyShutdown = 2;

    }

    if (isShutdownActive && alreadyShutdown != 2) {
        cursorMenu = MENU_SHUTDOWN;
    }

}

/**
 * Listen shutdown and change menu to shutdown
 */
void ShutDw::listener() {
    //
    // Get voltage from pin
    detectorValue = analogRead(pinDtct);


//    if (_amp->isMid()) {
//        Serial.print("Detected car voltage is: ");
//        Serial.println(detectorValue);
//    }

    if (detectorValue < SHUTDOWN_LOW_VALUE && alreadyShutdown != 2) {
        //
        // get data again
        detectorValue = analogRead(pinDtct);

        //
        // Is shutdown mode .... not noise
        if (_amp->isMax() && detectorValue < SHUTDOWN_LOW_VALUE) {
            isShutdownActive = true;
        }
        //
        // Check for some data changed,  but in case save button is pressed ... shutdown save trigger ...
        if (_amp->isMax() && detectorValue < SHUTDOWN_LOW_VALUE && !_car->isRunEng()) {
            //
            // Fixes digital pot
            _whl->shutdownMode();
            //
            // Wait dig pot
            delay(100);
            //
            // Skip save and shutdown
            digitalWrite(pinCtrl, LOW);
        }
        //
        // Reverse value for next check
        detectorValue = !detectorValue;
    }


}

/**
 * Display shutdown menu
 */
void ShutDw::lcdDisplay(LiquidCrystal *lcd) {


    char sec[2];

    //
    // Show message before straiting procedure
    if (entryDisplay == 0) {
        lcd->clear();
        lcd->setCursor(0, 0);
        lcd->print("Hit \"S<\" to ");
        lcd->setCursor(0, 2);
        lcd->print("skip saving data");
        tone(pinTone, 3000, 500);
        delay(3400);
        lcd->clear();
        lcd->setCursor(1, 0);
        lcd->print("Shutting  down");
        tone(pinTone, 2000, 500);
        delay(1000);
        entryDisplay = 1;
    }


    //
    // Catch seconds from loop
    if (_amp->isSec()) {

        lcd->setCursor(0, 0);
        lcd->print("Waiting ");
        //
        // Convert data to human format
        sprintf(sec, "%02d", ((indexWait - SHUTDOWN_SAVE_LOOPS) / 200) * -1);

        lcd->print(sec);
        lcd->print(" sec.  ");
        lcd->setCursor(1, 2);
        lcd->print(" for ESC saving");
    }

    //
    // Listen press button
    if (digitalRead(SHUTDOWN_SAVE_BUTTON) == SHUTDOWN_SAVE_STATE) {
        delay(5);
        if (digitalRead(SHUTDOWN_SAVE_BUTTON) == SHUTDOWN_SAVE_STATE && alreadySaved == 0) {
            displayCancel(lcd);
        }
    }
    //
    // Shutdown without wait ...
    if (indexWait >= SHUTDOWN_SAVE_LOOPS && alreadyShutdown == 0) {
        //
        // Save current data and shutdown
        _eep->saveCurrentData();
        //
        // Show on lcdDisplay
        displaySaved(lcd);
    }
    //
    // Count loops
    indexWait++;

}

/**
 * Cancel state
 */
void ShutDw::displayCancel(LiquidCrystal *lcd) {
    lcd->clear();
    lcd->setCursor(1, 0);
    lcd->print(" Data cancel :(");
    lcd->setCursor(1, 2);
    lcd->print(" Bye bye ...");
    //
    // Shutdown the system
    tone(pinTone, 800, 500);
    _whl->shutdownMode();
    delay(2000);
    analogWrite(pinCtrl, LOW);
    //
    // Mark mid as shutdown
    alreadyShutdown = 1;
}

/**
 * Save state
 */
void ShutDw::displaySaved(LiquidCrystal *lcd) {
    //
    // Mark saved
    alreadySaved = 1;
    lcd->clear();
    lcd->setCursor(1, 0);
    lcd->print(" Data saved :)");
    lcd->setCursor(1, 2);
    lcd->print(" Bye bye ...");
    //
    // Shutdown the system
    tone(pinTone, 1000, 50);
    delay(50);
    tone(pinTone, 1500, 50);
    delay(50);
    tone(pinTone, 1500, 50);
    delay(50);
    tone(pinTone, 2000, 50);
    delay(50);
    tone(pinTone, 3000, 50);
    _whl->shutdownMode();
    delay(2000);
    digitalWrite(pinCtrl, LOW);
    //
    // Mark mid as shutdown
    alreadyShutdown = 1;
}

#endif //ARDUINOMID_SHUTDOWN_H

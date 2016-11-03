//
// Created by Angel Zaprianov on 11.10.2016 г..
//
#include <Arduino.h>

#ifndef SHUTDOWN_SAVE_STATE
#define SHUTDOWN_SAVE_STATE HIGH
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


    TimeAmp *_amp;

    EepRom *_eep;

    CarSens *_car;

private :
    uint8_t pinCtrl, pinDtct, pinSaveCancel, pinTone;

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

    void displaySaved();

    void displayCancel();

public:
    static constexpr int MENU_SHUTDOWN = 99;

    ShutDw(EepRom *eepRom, TimeAmp *ampInt, CarSens *carSens);

    void setup(int pinControl, int pinDetect, int pintPressSave, int pinToAlarm);

    void listener();

    void display();

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
 */
ShutDw::ShutDw(EepRom *eepRom, TimeAmp *ampInt, CarSens *carSens) {
    _eep = eepRom;
    _amp = ampInt;
    _car = carSens;
}

/**
 * Setup shutdown class
 */
void ShutDw::setup(int pinControl, int pinDetect, int pintPressSave, int pinToAlarm) {

    pinCtrl = uint8_t(pinControl);
    pinDtct = uint8_t(pinDetect);
    pinSaveCancel = uint8_t(pintPressSave);
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
void  ShutDw::resolveUsbActive(int _detectorValue, int _detectUsbAct) {
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
        lcd.clear();
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
    //
    // Detect usb
    resolveUsbActive(detectorValue, _amp->getLoopIndex());
    //
    // Is shutdown mode ....
    if (detectorValue < SHUTDOWN_LOW_VALUE && alreadyShutdown != 2) {
        isShutdownActive = true;
    }
    //
    // Check for some data changed,  but in case save button is pressed ... shutdown save trigger ...
    if (/*detectorValue < SHUTDOWN_LOW_VALUE && !_car->isRunDst() && digitalRead(pinSaveCancel) != SHUTDOWN_SAVE_STATE ||*/
        detectorValue < SHUTDOWN_LOW_VALUE && !_car->isRunEng() /*&& digitalRead(pinSaveCancel) != SHUTDOWN_SAVE_STATE*/) {
        //
        // Skip shutdown menu when vehicle is not moved or engine is off
        isShutdownActive = false;
        alreadyShutdown = 2;
        isShutdownInactive = true;
        digitalWrite(pinCtrl, LOW);
    }
}

/**
 * Display shutdown menu
 */
void ShutDw::display() {

    if (isShutdownInactive) {
        return;
    }
    char sec[2];

    //
    // Show message before straiting procedure
    if (entryDisplay == 0) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Hit \"S\" to skip");
        lcd.setCursor(0, 2);
        lcd.print("and unsave data");
        tone(pinTone, 3000, 500);
        delay(2000);
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Shutting  down");
        tone(pinTone, 2000, 500);
        delay(1000);
        entryDisplay = 1;
    }


    //
    // Catch seconds from loop
    if (_amp->isSec()) {

        lcd.setCursor(0, 0);
        lcd.print("Waiting ");
        //
        // Convert data to human format
        sprintf(sec, "%02d", ((indexWait - SHUTDOWN_SAVE_LOOPS) / 200) * -1);

        lcd.print(sec);
        lcd.print(" sec.  ");
        lcd.setCursor(1, 2);
        lcd.print("to canceling...");
    }

    //
    // Listen press button
    if (digitalRead(pinSaveCancel) == SHUTDOWN_SAVE_STATE) {
        delay(10);
        if (digitalRead(pinSaveCancel) == SHUTDOWN_SAVE_STATE && alreadySaved == 0) {
            displayCancel();
        }
    }
    //
    // Shutdown without wait ...
    if (indexWait >= SHUTDOWN_SAVE_LOOPS && alreadyShutdown == 0) {
        //
        // Save current data and shutdown
        _eep->saveCurrentData();
        //
        // Show on display
        displaySaved();
    }
    //
    // Count loops
    indexWait++;

}

/**
 * Cancel state
 */
void ShutDw::displayCancel() {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print(" Data cancel :(");
    lcd.setCursor(1, 2);
    lcd.print(" Bye bye ...");
    //
    // Shutdown the system
    tone(pinTone, 800, 500);
    delay(2000);
    analogWrite(pinCtrl, LOW);
    //
    // Mark mid as shutdown
    alreadyShutdown = 1;
}

/**
 * Save state
 */
void ShutDw::displaySaved() {
    //
    // Mark saved
    alreadySaved = 1;
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print(" Data saved :)");
    lcd.setCursor(1, 2);
    lcd.print(" Bye bye ...");
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
    delay(2000);
    digitalWrite(pinCtrl, LOW);
    //
    // Mark mid as shutdown
    alreadyShutdown = 1;
}

#endif //ARDUINOMID_SHUTDOWN_H

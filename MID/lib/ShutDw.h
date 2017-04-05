//
// Created by Angel Zaprianov on 11.10.2016 г..
//
#include <Arduino.h>
#include <avr/pgmspace.h>

#if SCREEN == 162 || !defined(SCREEN)
#import <LiquidCrystal.h>
#else

#endif

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

const char ShutDw__msgString_01[] PROGMEM = "Goodbye ...     ";
const char ShutDw__msgString_02[] PROGMEM = "until next time!";
const char ShutDw__msgString_03[] PROGMEM = "Hit \"S<\" to   ";
const char ShutDw__msgString_04[] PROGMEM = "skip saving TRIP";
const char ShutDw__msgString_05[] PROGMEM = "Shutting  down  ";
const char ShutDw__msgString_06[] PROGMEM = "Waiting ";
const char ShutDw__msgString_07[] PROGMEM = " sec.  ";
const char ShutDw__msgString_08[] PROGMEM = " for ESC saving ";
const char ShutDw__msgString_09[] PROGMEM = "Trip data cancel";
const char ShutDw__msgString_10[] PROGMEM = "Erasing trip ...";
const char ShutDw__msgString_11[] PROGMEM = " Trip saved :)  ";
const char ShutDw__msgString_12[] PROGMEM = " Bye bye ...    ";

const char* const  ShutDw_table[] PROGMEM =
        {
                ShutDw__msgString_01,
                ShutDw__msgString_02,
                ShutDw__msgString_03,
                ShutDw__msgString_04,
                ShutDw__msgString_05,
                ShutDw__msgString_06,
                ShutDw__msgString_07,
                ShutDw__msgString_08,
                ShutDw__msgString_09,
                ShutDw__msgString_10,
                ShutDw__msgString_11,
                ShutDw__msgString_12,
        };

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
    bool isShutdownActive = false;
    int detectorValue = 1000;

#if SCREEN == 162 || !defined(SCREEN)
    void displaySaved(LiquidCrystal *lcd);

    void displayCancel(LiquidCrystal *lcd);
#else

#endif
    void melodySave(void);

    char buffer[74];
    char *getMsg(int index) {
        strcpy_P(buffer, (char *)pgm_read_word(&(ShutDw_table[index])));
        return  buffer;
    }


public:
    static constexpr uint8_t MENU_SHUTDOWN = 99;

    ShutDw(EepRom *eepRom, IntAmp *ampInt, CarSens *carSens, WhlSens *whlSens);

    void setup(int pinControl, int pinDetect, int pinToAlarm);

    void listener();
#if SCREEN == 162 || !defined(SCREEN)
    void lcd16x2(LiquidCrystal *lcd);
#else

#endif
    void cursor();

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
 * Handler the cursor menu
 */
void ShutDw::cursor() {

    //
    // Switch menu if cannot shutdown ...
    if (alreadyShutdown == 1) {
        MidCursorMenu = 1;
        alreadyShutdown = 2;

    }

    if (isShutdownActive && alreadyShutdown != 2) {
        MidCursorMenu = MENU_SHUTDOWN;
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
 * Save melody play
 */
void ShutDw::melodySave() {
    tone(pinTone, 1000, 50);
    delay(50);
    tone(pinTone, 1500, 50);
    delay(50);
    tone(pinTone, 1500, 50);
    delay(50);
    tone(pinTone, 2000, 50);
    delay(50);
    tone(pinTone, 3000, 50);
}

#if SCREEN == 162 || !defined(SCREEN)
/**
 * Display shutdown menu
 */
void ShutDw::lcd16x2(LiquidCrystal *lcd) {



    //
    // Basic information save
    if (_car->getDst() < SHUTDOWN_SAVE_TRIP) {
        tone(pinTone, 1000, 50);
        delay(50);
        tone(pinTone, 1200, 50);
        delay(50);

        lcd->clear();
        lcd->setCursor(0, 0);
        lcd->print(getMsg(0));
        lcd->setCursor(0, 1);
        lcd->print(getMsg(1));
        melodySave();
        _eep->saveCurrentData();
        _whl->shutdownMode();
        delay(2200);
        digitalWrite(pinCtrl, LOW);
        return;
    }

    char sec[2];
    //
    // Show message before straiting procedure
    if (entryDisplay == 0) {
        lcd->clear();
        lcd->setCursor(0, 0);
        lcd->print(getMsg(2));
        lcd->setCursor(0, 1);
        lcd->print(getMsg(3));
        tone(pinTone, 800, 200);
        delay(200);
        tone(pinTone, 800, 200);
        delay(200);
        tone(pinTone, 3000, 1000);
        delay(3600);
        lcd->clear();
        lcd->setCursor(1, 0);
        lcd->print(getMsg(4));
        tone(pinTone, 2000, 600);
        delay(1000);
        entryDisplay = 1;
    }


    //
    // Catch seconds from loop
    if (_amp->isSec()) {

        lcd->setCursor(0, 0);
        lcd->print(getMsg(5));
        //
        // Convert data to human format
        sprintf(sec, "%02d", ((indexWait - SHUTDOWN_SAVE_LOOPS) / 200) * -1);

        lcd->print(sec);
        lcd->print(getMsg(6));
        lcd->setCursor(1, 2);
        lcd->print(getMsg(7));
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
        _eep->saveTripData();
        //
        // Show on playEntry
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
    lcd->print(getMsg(8));
    lcd->setCursor(1, 2);
    lcd->print(getMsg(9));

    //
    // Erase trip data
    _eep->clearTripData();
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
    lcd->print(getMsg(10));
    lcd->setCursor(1, 2);
    lcd->print(getMsg(11));
    //
    // Shutdown the system
    melodySave();
    _whl->shutdownMode();
    delay(2000);
    digitalWrite(pinCtrl, LOW);
    //
    // Mark mid as shutdown
    alreadyShutdown = 1;
}

#endif

#endif //ARDUINOMID_SHUTDOWN_H

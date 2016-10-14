//
// Created by Angel Zaprianov on 11.10.2016 г..
//
#include <Arduino.h>

#ifndef SHUTDOWN_SAVE_STATE
#define SHUTDOWN_SAVE_STATE HIGH
#endif

#ifndef SHUTDOWN_SAVE_LOOPS
#define SHUTDOWN_SAVE_LOOPS 1000
#endif

#ifndef ARDUINOMID_SHUTDOWN_H
#define ARDUINOMID_SHUTDOWN_H


class MidShutdown {

private :
    uint8_t pinCtrl, pinDtct, pinSave, pinTone;

    int indexWait = 0;
    int entryDisplay = 0;
    int alreadySaved = 0;
    int alreadyShutdown = 0;
    bool entryUsbDetected = false;
    bool idShutdownActive = false;
    int handlerCursorMenu;

    void resolveUsbActive(int _detectorValue, int _detectUsbAct);

public:
    static constexpr int MENU_SHUTDOWN = 99;

    MidShutdown(int pinControl, int pinDetect, int pintPressSave, int pinToAlarm);

    void setup();

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
MidShutdown::MidShutdown(int pinControl, int pinDetect, int pintPressSave, int pinToAlarm) {
    pinCtrl = uint8_t(pinControl);
    pinDtct = uint8_t(pinDetect);
    pinSave = uint8_t(pintPressSave);
    pinTone = uint8_t(pinToAlarm);
}

/**
 * Setup shutdown class
 */
void MidShutdown::setup() {
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
void  MidShutdown::resolveUsbActive(int _detectorValue, int _detectUsbAct) {
    if (_detectorValue < 500 && _detectUsbAct < 50) {
        entryUsbDetected = true;
    }
}
/**
 *  Is mid started from USB power supply
 */
bool MidShutdown::isUsbActive() {

    if (entryUsbDetected) {
        return true;
    }

    if (detectorValue < 500 && detectUsbAct < 50) {
        entryUsbDetected = true;
        return true;
    }
}

/**
 * Handler the cursor menu
 */
void MidShutdown::cursor(int &cursorMenu) {

    //
    // Switch menu if cannot shutdown ...
    if (alreadyShutdown == 1) {
        cursorMenu = 1;
        alreadyShutdown = 2;
        lcd.clear();
    }

    if (idShutdownActive && alreadyShutdown != 2) {
        cursorMenu = MENU_SHUTDOWN;
    }

}

/**
 * Listen shutdown and change menu to shutdown
 */
void MidShutdown::listener() {
    //
    // Get voltage from pin
    int detectorValue = analogRead(pinDtct);
    //
    // Detect usb
    resolveUsbActive(detectorValue, ampInt.getLoopIndex());
    //
    // Is shutdown mode ....
    if (detectorValue < 500 && alreadyShutdown != 2) {
        idShutdownActive = true;
    }
}

/**
 * Display shutdown menu
 */
void MidShutdown::display() {

    char sec[2];

    //
    // Show message before straiting procedure
    if (entryDisplay == 0) {
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Shutting  down");
        tone(pinTone, 2000, 500);
        delay(2000);
        entryDisplay = 1;
    }


    //
    // Catch seconds from loop
    if (ampInt.isSec()) {

        lcd.setCursor(0, 0);
        lcd.print("Waiting ");
        //
        // Convert data to human format
        sprintf(sec, "%02d", ((indexWait - SHUTDOWN_SAVE_LOOPS) / 100) * -1);

        lcd.print(sec);
        lcd.print(" sec.  ");
        lcd.setCursor(1, 2);
        lcd.print("Press 0 to save");
    }

    //
    // Listen press button
    if (digitalRead(pinSave) == SHUTDOWN_SAVE_STATE) {
        delay(10);
        if (digitalRead(pinSave) == SHUTDOWN_SAVE_STATE && alreadySaved == 0) {

            //
            // Save current data and shutdown
            eepRom.saveCurrentData();

            Serial.println("Data saved!");
            //
            // Mark saved
            alreadySaved = 1;
            lcd.clear();
            lcd.setCursor(1, 0);
            lcd.print(" Data saved :)");
            lcd.setCursor(1, 2);
            lcd.print(" Bay bay ...");
            //
            // Shutdown the system
            tone(pinTone, 2000, 500);
            delay(2000);
            digitalWrite(pinCtrl, LOW);
            //
            // Mark mid as shutdown
            alreadyShutdown = 1;
        }
    }

    //
    // Shutdown without wait ...
    if (indexWait >= SHUTDOWN_SAVE_LOOPS && alreadyShutdown == 0) {
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Bay bay ...!");
        tone(pinTone, 800, 500);
        delay(2000);
        analogWrite(pinCtrl, LOW);
        //
        // Mark mid as shutdown
        alreadyShutdown = 1;
    }
    indexWait++;

}

#endif //ARDUINOMID_SHUTDOWN_H

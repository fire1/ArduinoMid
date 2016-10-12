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


public:
    static constexpr int MENU_SHUTDOWN = 99;

    MidShutdown(int pinControl, int pinDetect, int pintPressSave, int pinToAlarm);

    void setup();

    void listener(int &cursorMenu);

    void display();

};


/***********************************************************************************************
 *                                                                                             *
 *                                   CPP part of file                                          *
 *                                                                                             *
 ***********************************************************************************************/

MidShutdown::MidShutdown(int pinControl, int pinDetect, int pintPressSave, int pinToAlarm) {
    pinCtrl = uint8_t(pinControl);
    pinDtct = uint8_t(pinDetect);
    pinSave = uint8_t(pintPressSave);
    pinTone = uint8_t(pinToAlarm);
}

void MidShutdown::setup() {
    //
    // Control save shutdown
    pinMode(pinCtrl, OUTPUT);
    analogWrite(pinCtrl, 255);
    //
    // Detect ignition key
    pinMode(pinDtct, INPUT);

}


void MidShutdown::listener(int &cursorMenu) {

    int detectorValue = analogRead(pinDtct);

    if (ampInt.isSec()) {
        Serial.print("Detector is \t");
        Serial.println(detectorValue);
    }

    if (detectorValue < 500 && alreadyShutdown != 2) {
        cursorMenu = MENU_SHUTDOWN;
        tone(pinTone, 4000, 10);
    }
    if (alreadyShutdown == 1) {
        cursorMenu = 1;
        alreadyShutdown = 2;
        lcd.clear();
    }
}


void MidShutdown::display() {

    char sec[2];

    if (entryDisplay == 0) {
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Shutting  down!");
        delay(2000);
        entryDisplay = 1;
    }


    if (ampInt.isSec()) {

        lcd.setCursor(0, 0);
        lcd.print("Waiting ");

        sprintf(sec, "%02d", ((indexWait - SHUTDOWN_SAVE_LOOPS) / 100) * -1);

        lcd.print(sec);
        lcd.print(" sec.  ");

        lcd.setCursor(1, 2);
        lcd.print("Press 0 to save");
        tone(pinTone, 2000, 100);
        delay(100);
    }


    if (digitalRead(pinSave) == SHUTDOWN_SAVE_STATE) {
        delayMicroseconds(150);
        if (digitalRead(pinSave) == SHUTDOWN_SAVE_STATE && alreadySaved == 0) {
            tone(pinTone, 2000, 100);
            //
            // Save current data and shutdown
            eepRom.saveCurrentData();

            Serial.println("Data saved!");

            alreadySaved = 1;
            lcd.clear();
            lcd.setCursor(1, 0);
            lcd.print(" Data saved :)");
            lcd.setCursor(1, 2);
            lcd.print(" Bay bay ...");
            delay(2000);
            digitalWrite(pinCtrl, LOW);
        } else {
            tone(pinTone, 800, 100);
            delay(2000);
            digitalWrite(pinCtrl, LOW);
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
        alreadyShutdown = 1;
    }
    indexWait++;

}

#endif //ARDUINOMID_SHUTDOWN_H

//
// Created by Angel Zaprianov on 11.10.2016 г..
//
#include <Arduino.h>

#ifndef SHUTDOWN_SAVE_STATE
#define SHUTDOWN_SAVE_STATE HIGH
#endif

#ifndef SHUTDOWN_SAVE_LOOPS
#define SHUTDOWN_SAVE_LOOPS 500
#endif

#ifndef ARDUINOMID_SHUTDOWN_H
#define ARDUINOMID_SHUTDOWN_H


class MidShutdown {

private :
    uint8_t pinCtrl, pinDtct, pinSave, pinTone;

    int indexWait = 0;
    int entryDisplay = 0;
    int alreadySaved = 0;


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
    if (analogRead(pinDtct) < 500) {
        cursorMenu = MENU_SHUTDOWN;
        tone(pinTone, 4000, 500);
    }
}


void MidShutdown::display() {

    char sec[2];

    if (entryDisplay == 0) {
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Shutting  down!");
        delay(400);
        entryDisplay = 1;
    }


    if (amp.isSec()) {

        lcd.setCursor(0, 0);
        lcd.print("Waiting ");

        sprintf(sec, "%02d", (indexWait / 100));

        lcd.print(sec);
        lcd.print(" sec.  ");

        lcd.setCursor(1, 2);
        lcd.print("Press 0 to save");
        tone(pinTone, 2000, 500);
    }


    if (digitalRead(pinSave) == SHUTDOWN_SAVE_STATE) {
        delay(5);
        if (digitalRead(pinSave) == SHUTDOWN_SAVE_STATE && alreadySaved == 0) {
            tone(pinTone, 4000, 500);
            //
            // Save current data and shutdown
//            rom.saveCurrentData();
            alreadySaved = 1;
            delay(500);
            digitalWrite(pinCtrl, LOW);
        }else{
            tone(pinTone, 800, 500);
            delay(500);
            digitalWrite(pinCtrl, LOW);
        }
    }

    //
    // Shutdown without wait ...
    if (indexWait >= SHUTDOWN_SAVE_LOOPS) {
        tone(pinTone, 800, 500);
        delay(500);
        digitalWrite(pinCtrl, LOW);
    }

}

#endif //ARDUINOMID_SHUTDOWN_H

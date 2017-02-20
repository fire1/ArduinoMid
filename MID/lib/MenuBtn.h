//
// Created by Angel Zaprianov on 14.2.2017 г..
//

#ifndef ARDUINO_MID_MENU_BTN_H
#define ARDUINO_MID_MENU_BTN_H

#include "IntAmp.h"
#include "WhlSens.h"
#include "CarSens.h"
#include "EepRom.h"

#ifndef AWAITING
#define AWAITING 2000
#endif

class MenuBtn {

    IntAmp *amp;
    CarSens *car;
    EepRom *eep;
    WhlSens *whl;
    CarState *stt;

private:
    uint8_t btnUp, btnDw, pinTn;
    uint8_t lastButtonPushed = LOW; //TODO @deprecated [changed to MenuBtn::STATE]
    int isMainNavigationStatus = 0;
    boolean _isEditOption = false;
    boolean isEnterSub = false;
    boolean isNavigationActive = true;
    boolean playSecondTone = false;
    unsigned long entryTimeDownState = 0;


    void captureUp(void);

    void captureDw(void);

    void captureHold(void);

    boolean shortcut(void);

public:
    MenuBtn(IntAmp *_amp, CarSens *_car, EepRom *_eep, WhlSens *_whl, CarState *_stt);

    void listener(void);

    void setup(uint8_t buttonPinUp, uint8_t buttonPinDw, uint8_t pinTones);


    uint8_t getPinUp(void);

    uint8_t getPinDw(void);

    uint8_t getLastBtn(void);

    uint8_t getPinTn(void);

    static uint8_t STATE;

    IntAmp *passAmp(void);

    CarSens *passCar(void);

    EepRom *passEep(void);

    WhlSens *passWhl(void);

    CarState *passStt(void);

};

uint8_t MenuBtn::STATE = LOW;


MenuBtn::MenuBtn(IntAmp *_amp, CarSens *_car, EepRom *_eep, WhlSens *_whl, CarState *_stt) {
    amp = _amp;
    car = _car;
    eep = _eep;
    whl = _whl;
    stt = _stt;

}

uint8_t MenuBtn::getPinUp(void) {
    return btnUp;
}

uint8_t MenuBtn::getPinDw(void) {
    return btnDw;
}

uint8_t MenuBtn::getPinTn(void) {
    return pinTn;
}

uint8_t MenuBtn::getLastBtn() {
    return lastButtonPushed;
}

IntAmp *MenuBtn::passAmp(void) {
    return amp;
}

CarSens *MenuBtn::passCar(void) {
    return car;
}

EepRom *MenuBtn::passEep(void) {
    return eep;
}

WhlSens *MenuBtn::passWhl(void) {
    return whl;
}

CarState *MenuBtn::passStt(void) {
    return stt;
}

void MenuBtn::setup(uint8_t buttonPinUp, uint8_t buttonPinDw, uint8_t pinTones) {
    btnUp = buttonPinUp;
    btnDw = buttonPinDw;
    pinTn = pinTones;
    //
    // Pin button mode
    pinMode(pinTn, INPUT);
    pinMode(btnDw, INPUT);
    //
    // Default state value
    this->STATE = LOW;
}

void MenuBtn::listener() {

    //
    // Delete last loop state record
    lastButtonPushed = LOW;
    MenuBtn::STATE = LOW;
    //
    // Checks is navigation active (default = true)
    if (isNavigationActive) {
        //
        // Detect up state button
        captureUp();
        //
        // Detect down state button
        captureDw();
        //
        // Detect Hold button state
        captureHold();
    }

}

void MenuBtn::captureUp(void) {
    if (!digitalRead(btnUp) == HIGH) {

        if (amp->isLow() && !digitalRead(btnUp) == HIGH) {
            lastButtonPushed = btnUp;
            MenuBtn::STATE = btnUp;
        }
    }
}

void MenuBtn::captureDw(void) {
    if (entryTimeDownState + AWAITING > millis() && isEnterSub && !digitalRead(btnDw) == LOW) {
        //
        //
        tone(pinTn, 700, 20);
        delay(20);
        tone(pinTn, 700, 20);
        isEnterSub = false;
        //
        // Perform button is released action
        lastButtonPushed = btnDw;
        MenuBtn::STATE = btnDw;
        //
        // Reset entry down state
        entryTimeDownState = 0;
    }
}

void MenuBtn::captureHold(void) {
    //
    // Detect EDIT state button
    if (!digitalRead(btnDw) == HIGH) {
        //
        // Controlling start of press state
        if (entryTimeDownState == 0) {
            whl->disable();
            entryTimeDownState = millis();
        }


        if (amp->isMin() && !digitalRead(btnDw) == HIGH) {
            isEnterSub = true;
        }

        //
        // Hold
        if (entryTimeDownState + AWAITING < millis() && !digitalRead(btnDw) == HIGH) {
            //
            // If is still high state [pressed]
            if (!digitalRead(btnDw) == HIGH) {
                //
                // Reset entry down state
                entryTimeDownState = 0;
                //
                // Cut the method if shortcut is executed
                if (shortcut()) return;
                //
                // Check for subMenu if not got inner level entry
                if (_isEditOption == 0) {
                    //
                    // Enter inner level menu
                    _isEditOption = 1;
                    tone(pinTn, 400, 100);
                    //
                    // Exit inner level menu
                } else if (_isEditOption == 1) {
                    _isEditOption = 0;
                    tone(pinTn, 400, 50);
                    playSecondTone = 1;
                }
            } else {

            }
        }

    } else { // <- deprecated
        entryTimeDownState = 0;
        whl->enable(); // unlock radio
    }

    if (amp->isSec() && playSecondTone) {
        tone(pinTn, 800, 50);
        playSecondTone = 0;
    }
}


boolean MenuBtn::shortcut(void) {

    /*********** [SHORTCUTS] *********** *********** *********** *********** START ***********/
    // Steering button is pressed
    if (whl->getCurrentState() == whl->STR_BTN_ATT) {
        tone(pinTn, 1000, 10);
        delay(10);
        tone(pinTn, 1000, 10);
        delay(10);

        tone(pinTn, 2500, 10);
        delay(20);
        tone(pinTn, 2500, 10);
        eep->saveZeroingData();
        delay(20);

        return true;
    }
    //
    // Change Speed alarm Up
    if (whl->getCurrentState() == WhlSens::STR_BTN_SKU) {
        car->speedingAlarmsUp();
        tone(pinTn, 800, 50);
        delay(50);
        tone(pinTn, 2000, 80);
        delay(80);

        return true;
    }
    //
    // Change Speed alarm Down
    if (whl->getCurrentState() == WhlSens::STR_BTN_SKD) {
        car->speedingAlarmsDw();
        tone(pinTn, 2000, 50);
        delay(50);
        tone(pinTn, 800, 80);
        delay(80);

        return true;
    }
    /*********** [SHORTCUTS] *********** *********** *********** *********** END   ***********/

    return false;
}

#endif //ARDUINOMID_BUTTONS_H

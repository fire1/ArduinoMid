//
// Created by Angel Zaprianov on 14.2.2017 г..
//

#ifndef ARDUINO_MID_MENU_BTN_H
#define ARDUINO_MID_MENU_BTN_H

#include "IntAmp.h"
#include "WhlSens.h"
#include "CarSens.h"
#include "EepRom.h"

#ifndef AWAITING_HOLD_BTN
#define AWAITING_HOLD_BTN 2000
#endif

class MenuBtn {

    IntAmp *amp;
    CarSens *car;
    EepRom *eep;
    WhlSens *whl;
    CarState *stt;

private:
    uint8_t btnUp, btnDw, pinTn;
    uint8_t lastButtonPushed = 0; //TODO @deprecated [changed to MenuBtn::STATE]



    boolean isNavigationActive = true;
    boolean playSecondTone = false;
    boolean activateSteering = false;


    //
    // Down Button
    bool entryDownState = false;
    unsigned long holdTimeHandler = 0;

    void captureUp(void);

    void captureDw(void);

    void captureHl(void);

    void shortcut(void);

public:
    MenuBtn(IntAmp *_amp, CarSens *_car, EepRom *_eep, WhlSens *_whl, CarState *_stt);

    void listener(void);

    void setup(uint8_t buttonPinUp, uint8_t buttonPinDw, uint8_t pinTones);

    void clearLastButton() {
        lastButtonPushed = 0;
    }

    uint8_t getPinUp(void);

    uint8_t getPinDw(void);

    uint8_t getLastBtn(void);

    uint8_t getPinTn(void);

    boolean isUp() {
        return lastButtonPushed == btnUp;
    }


    boolean isDw() {
        return lastButtonPushed == btnDw;
    }



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
    lastButtonPushed = 0;
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
        captureHl();
        // TODO  make this hold as additional option
        // and other other waiting press after hold to activate shortcuts
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

    //
    // Single press button
    if (!digitalRead(btnDw) == HIGH && !entryDownState) {
        //
        // Clear noise
        if (amp->isLow() && !digitalRead(btnDw) == HIGH) {
            tone(TONE_ADT_PIN, 700, 20);
            entryDownState = true;
            playSecondTone = true;
            holdTimeHandler = millis();
            lastButtonPushed = btnDw;
            MenuBtn::STATE = btnDw;
            whl->disable();
        }

    } else if (!digitalRead(btnDw) == LOW && entryDownState) {
        entryDownState = false;
        whl->enable();
        lastButtonPushed = btnUp;
        MenuBtn::STATE = btnUp;
    }

    if (amp->isBig() && playSecondTone) {
        tone(pinTn, 1100, 50);
        playSecondTone = false;
    }

}


void MenuBtn::captureHl(void) {
    //
    // Hold button detection
    if ((AWAITING_HOLD_BTN + holdTimeHandler) < millis() && (!digitalRead(btnDw)) == HIGH && entryDownState) {
        if (amp->isLow() && !digitalRead(btnDw) == HIGH) {
            //
            // Cut the method if shortcut is executed
            shortcut();
            holdTimeHandler = 0;
            entryDownState = false;
            activateSteering = true;
            whl->disable();
        }
    }

    //
    // Reactivate steering wheel buttons
    if (amp->isSecond() && activateSteering) {
        activateSteering = false;
        whl->enable();
        tone(pinTn, 1300, 100);
    }

}


void MenuBtn::shortcut(void) {

    /*********** [SHORTCUTS] *********** *********** *********** *********** START ***********/
    // Steering zero
    // Steering button is pressed
    if (whl->getCurrentState() == WhlSens::STR_BTN_ATT) {
        tone(TONE_ADT_PIN, 1000, 10);
        delay(10);
        tone(TONE_ADT_PIN, 1000, 10);
        delay(10);

        tone(TONE_ADT_PIN, 2500, 10);
        delay(20);
        tone(TONE_ADT_PIN, 2500, 10);
        eep->saveZeroingData();
        delay(20);
        lastButtonPushed = 0;
    }
    // Steering SEEK UP
    // Change Speed alarm Up
    if (whl->getCurrentState() == WhlSens::STR_BTN_SKU) {
        car->speedingAlarmsUp();
        tone(TONE_ADT_PIN, 800, 50);
        delay(50);
        tone(TONE_ADT_PIN, 2000, 80);
        delay(80);
        lastButtonPushed = 0;
    }
    // Steering SEEK DOWN
    // Change Speed alarm Down
    if (whl->getCurrentState() == WhlSens::STR_BTN_SKD) {
        car->speedingAlarmsDw();
        tone(TONE_ADT_PIN, 2000, 50);
        delay(50);
        tone(TONE_ADT_PIN, 800, 80);
        delay(80);
        lastButtonPushed = 0;
    }
    /*********** [SHORTCUTS] *********** *********** *********** *********** END   ***********/


}

#endif //ARDUINOMID_BUTTONS_H

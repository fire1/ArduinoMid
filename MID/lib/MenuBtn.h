//
// Created by Angel Zaprianov on 14.2.2017 г..
//

#ifndef ARDUINO_MID_MENU_BTN_H
#define ARDUINO_MID_MENU_BTN_H

#include "AmpTime.h"
#include "WhlSens.h"
#include "CarSens.h"
#include "EepRom.h"
#include "CarState.h"


#define BUTTONS_DEBUG

#ifndef AWAITING_HOLD_BTN
#define AWAITING_HOLD_BTN 2000
#endif


class MenuBtn {

    AmpTime *amp;
    CarSens *car;
    EepRom *eep;
    WhlSens *whl;
    CarState *stt;

private:
    unsigned long lastUsed = 0;
    uint8_t btnUp, btnDw, btnBk, pinTn;
    uint8_t lastButtonPushed = 0;
    const uint8_t btnMn = 200;

    boolean isNavigationActive = true;
    boolean playSecondTone = false;
    boolean activateSteering = false;
    boolean isHoldState = false;

    //
    // Down Button
    bool entryDownState = false;
    bool deblouncingState = false;
    unsigned long holdTimeHandler = 0;

    void captureUp(void);

    void captureDw(void);

    void captureHl(void);

    void shortcut(void);

public:
    MenuBtn(AmpTime &_amp, CarSens &_car, EepRom &_eep, WhlSens &_whl, CarState &_stt) :
            amp(&_amp), car(&_car), eep(&_eep), whl(&_whl), stt(&_stt) {
    }

    void listener(void);

    void setup(uint8_t buttonPinUp, uint8_t buttonPinDw, uint8_t brakePedal, uint8_t pinTones);

    inline void clearLastButton() {
        lastButtonPushed = 0;
    }


    /**
     * Activate or disable navigation
     */
    inline void setNavigationState(boolean state) {
        isNavigationActive = state;
    }

    inline boolean getNavigationState() {
        return isNavigationActive;
    }

    inline uint8_t MenuBtn::getPinUp(void) {
        return btnUp;
    }

    inline uint8_t MenuBtn::getPinDw(void) {
        return btnDw;
    }

    inline uint8_t MenuBtn::getPinTn(void) {
        return pinTn;
    }

    inline uint8_t MenuBtn::getLastBtn() {
        return lastButtonPushed;
    }

    inline boolean isMn() {
        return (!isNavigationActive) ? false : lastButtonPushed == btnMn;
    }

    inline boolean isUp() {
        return (!isNavigationActive) ? false : lastButtonPushed == btnUp;
    }

    inline boolean isDw() {
        return (!isNavigationActive) ? false : lastButtonPushed == btnDw;
    }

    inline boolean isHl() {
        return isHoldState;
    }

    inline boolean isOk() {
        return (isNavigationActive) ? false : lastButtonPushed == btnUp;
    }

    inline boolean isNo() {
        return (isNavigationActive) ? false : lastButtonPushed == btnDw;
    }

    inline boolean isBk() {
        return digitalRead(btnBk) == HIGH && car->getVss() == 0;
    }

    inline AmpTime *passAmp(void) {
        return amp;
    }

    inline CarSens *passCar(void) {
        return car;
    }

    inline EepRom *passEep(void) {
        return eep;
    }

    inline WhlSens *passWhl(void) {
        return whl;
    }

    inline CarState *passStt(void) {
        return stt;
    }

    boolean lastUseDebounce() {
        if (millis() - lastUsed > 450) {
#if defined(BUTTONS_DEBUG) || defined(GLOBAL_SENS_DEBUG)
            if (amp->isSecond()) {
                Serial.println("Debounce is true");
            }

#endif
            return true;
        } else {

#if defined(BUTTONS_DEBUG) || defined(GLOBAL_SENS_DEBUG)
            if (amp->isSecond()) {
                Serial.println("Debounce is false");
            }
        }
        return false;
#endif
    }


};


void MenuBtn::setup(uint8_t buttonPinUp, uint8_t buttonPinDw, uint8_t brakePedal, uint8_t pinTones) {
    btnUp = buttonPinUp;
    btnDw = buttonPinDw;
    btnBk = brakePedal;
    pinTn = pinTones;
    //
    // Pin button mode
    pinMode(pinTn, INPUT);
    pinMode(btnUp, INPUT);
    pinMode(btnDw, INPUT);
    pinMode(btnBk, INPUT);

    //
    // Turn on  Internal pull up resistor
    digitalWrite(btnUp, HIGH);
    digitalWrite(btnDw, HIGH);
    digitalWrite(btnBk, LOW);

}


void MenuBtn::listener() {
    //
    // Delete hold state
    isHoldState = false;
    //
    // Delete last loop state record

    if (lastUseDebounce()) {
        //
        // Detect up state button
        captureUp();
        //
        // Detect down state button
        captureDw();

        // TODO  make this hold as additional option
        // and other other waiting press after hold to activate shortcuts
    }
    //
    // Detect Hold button state
    captureHl();

#if defined(BUTTONS_DEBUG) || defined(GLOBAL_SENS_DEBUG)
    if (amp->isSecond()) {
        Serial.print(F("Last button is :"));
        Serial.println(lastButtonPushed);
        Serial.print(F("Navigation state :"));
        Serial.println(isNavigationActive);

    }
#endif

}


void MenuBtn::captureUp(void) {
    if (!digitalRead(btnUp) == HIGH) {
        if (amp->isLow() && !digitalRead(btnUp) == HIGH) {
            lastUsed = millis();
            if (!digitalRead(btnDw) == HIGH) {
                lastButtonPushed = btnMn;
                tone(TONE_ADT_PIN, 2500, 15);
            } else {
                lastButtonPushed = btnUp;
            }
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
            lastUsed = millis();
            tone(TONE_ADT_PIN, 700, 20);
            entryDownState = true;
            playSecondTone = true;
            holdTimeHandler = millis();
            lastButtonPushed = btnDw;
            whl->disable();
        }

    } else if (!digitalRead(btnDw) == LOW && entryDownState) {
        entryDownState = false;
        whl->enable();
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
            isHoldState = true;
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
        //
        // Preventing navigation when is deactivated
        if (isNavigationActive) {
            lastButtonPushed = btnUp;
        }
        tone(pinTn, 1300, 100);
    }

}


void MenuBtn::shortcut(void) {

    /*********** [SHORTCUTS] *********** *********** *********** *********** START ***********/
    // Steering zero
    // Steering button is pressed
    if (whl->getCurrentState() == WhlSens::STR_BTN_ATT || isBk()) {
        tone(TONE_ADT_PIN, 1000, 10);
        delay(10);
        tone(TONE_ADT_PIN, 1000, 10);
        delay(10);

        tone(TONE_ADT_PIN, 2500, 10);
        delay(20);
        tone(TONE_ADT_PIN, 2500, 10);
        eep->saveResetData();
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

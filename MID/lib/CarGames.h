//
// Created by Angel Zaprianov on 29.12.2016 г..
//

#ifndef ARDUINO_MID_CAR_GAME_H
#define ARDUINO_MID_CAR_GAME_H

#include "IntAmp.h"
#include "Menu16x2.h"

struct GamesBest {
    float drag;
    float t100;
};

/**
 * Car games
 */
class CarGames {

    IntAmp *_amp;
    CarSens *_car;
    Menu16x2 *menu;


private:

    uint8_t btnUp, btnDw;

    GamesBest bests;

    float stopwatchResult = 0;

    float from0To100Result = 0;

    boolean isTo100BeginState = false;

    unsigned long startStopwatch = 0, stopStopwatch = 0;

    unsigned long start0To100 = 0;

public:
    CarGames(IntAmp *intAmp, CarSens *carSens, Menu16x2 *midMenu);

    void setup(uint8_t buttonUp, uint8_t buttonDw) {
        btnUp = buttonUp;
        btnDw = buttonDw;
    }

    GamesBest getBestResults(void);

    void listenAStopwatch(void);

    float getStopwatchLive(void);

    float getStopwatch(void);

    void listen0to100(void);

    float get0to100Live();

    float get0to100();

    boolean isBegin0to100();


};

/**
 * Class constructor
 * @param amp
 */
CarGames::CarGames(IntAmp *intAmp, CarSens *carSens, Menu16x2 *midMenu) {
    _amp = intAmp;
    _car = carSens;
    menu = midMenu;
}


/**
 * Gets car games best results
 * @return
 */
GamesBest CarGames::getBestResults(void) {
    return bests;
}

/**
 * listen stopwatch timers
 */
void CarGames::listenAStopwatch(void) {

    //
    // Handle controllers button  to activate / deactivate stopwatch future
    if (menu->isNavigationActive() && menu->isEditOption()) {
        menu->setNavigation(false);
        tone(TONE_ADT_PIN, 2000, 100);
    } else if (menu->isNavigationActive() && menu->isEditOption()) {
        menu->setNavigation(false);
        tone(TONE_ADT_PIN, 200, 100);
    }


    //
    // Handle stopwatch time
    if (!menu->isNavigationActive() && !digitalRead(btnUp) == HIGH && startStopwatch == 0) {
        if (_amp->isMin() && !digitalRead(btnUp) == HIGH) { // debounce
            startStopwatch = millis();
        }
    } else if (!menu->isNavigationActive() && !digitalRead(btnUp) == HIGH && startStopwatch > 0) {
        if (_amp->isMin() && !digitalRead(btnUp) == HIGH) { // debounce
            stopStopwatch = millis();
        }
    }
    //
    // Calculate stopwatch final result
    if (!stopwatchResult && startStopwatch > 0 && stopStopwatch > 0) {
        stopwatchResult = (stopStopwatch - startStopwatch) / MILLIS_PER_SC;
    }

}

/**
 * Gets live seconds for stopwatch
 * @return
 */
float CarGames::getStopwatchLive() {
    return (millis() - startStopwatch) / 1000;
}

/**
 * Gets stop watch value
 * @return float
 */
float CarGames::getStopwatch(void) {
    return stopwatchResult;
}

/**
 * Listen from 0 to 100 game
 */
void CarGames::listen0to100() {

    //
    // Reset data
    if (menu->isEditOption()) {
        isTo100BeginState = false;
        start0To100 = 0;
        from0To100Result = 0; // TODO compare with BEST result and save it!!!
        menu->setEditOption(false); // Do not perform any changes over menu ...
    }


    if (!isTo100BeginState && _car->getVss() == 0) {
        isTo100BeginState = true;
    }

    if (isTo100BeginState && _car->getVss() > 0 && start0To100 == 0) {
        start0To100 = millis();
    }

    if (isTo100BeginState && _car->getVss() >= 100 && from0To100Result == 0) {
        from0To100Result = (millis() - start0To100) / MILLIS_PER_SC;
    }

}

/**
 * Gets is game begin
 * @return
 */
boolean CarGames::isBegin0to100() {
    return isTo100BeginState;
}

/**
 * Gets current time for 0 to 100 game
 * @return float
 */
float CarGames::get0to100Live() {
    return (millis() - start0To100) / 1000;
}

/**
 * Gets result of 0 to 100 game
 * @return
 */
float CarGames::get0to100() {
    return from0To100Result;
}


#endif //ARDUINOMID_CARGAME_H

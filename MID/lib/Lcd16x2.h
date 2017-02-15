//
// Created by Angel Zaprianov on 27.6.2016 г..
//

#ifndef ARDUINOMID_LCD_16x2_H
#define ARDUINOMID_LCD_16x2_H

#include <Arduino.h>
#include "../conf.h"
#include <LiquidCrystal.h>
//
// add Calculators
#include "MainFunc.h"
#include "Menu16x2.h"
#include "ShutDw.h"
#include "CarState.h"
#include "CarGames.h"
#include "graphics/LcdChar.h"

//
//
/****************************************************************
 * Display temperature sensor
 */
class Lcd16x2 {
    IntAmp *amp;
    LiquidCrystal *lcd;
    CarSens *car;
    EepRom *eep;
    CarState *stt;
    CarGames *gms;
    WhlSens *whl;
    Menu16x2 *mmn;
    ShutDw *sdw;
    GamesBest gameResults;


private:
    boolean outTempLowController = true;
    boolean displayAlertActive = false;
public:
    Lcd16x2(LiquidCrystal *_lcd, MenuBtn *_btn, Menu16x2 *_mmn, CarGames *_gms, ShutDw *_sdw);

    void intro(void);

    void begin(void);

    void draw(int cursorMenu);

protected:
    void displayOutTmp();

    void displayInsTmp();

    void displayTotalCons();

    void displayTotalDst();

    void displayAlert();

    void displayEngRPM();

    void displayCarKMH();

    void displayEngTmp();

    void displayCarECU();

    void displayTrip();

    void displayTotalTrip();

    void displayTest();

    void displayAverage();

    void displayConsumption();

    void displayFuelTanks();

    void displayCarState();

    void displayCarGames();

    void displayCarGameWatch();

    void displayCarGameDrag();

    void displayCarGameT100();
};

/**
 * Constructor
 */
Lcd16x2::Lcd16x2(LiquidCrystal *_lcd, MenuBtn *_btn, Menu16x2 *_mmn, CarGames *_gms, ShutDw *_sdw) {
    lcd = _lcd;
    amp = _btn->passAmp();
    car = _btn->passCar();
    eep = _btn->passEep();
    stt = _btn->passStt();
    whl = _btn->passWhl();
    mmn = _mmn;
    sdw = _sdw;
}

/**
 * Welcome screen ...
 */
void Lcd16x2::intro(void) {
    lcd->setCursor(0, 0);
    lcd->print("    ASTRA       ");
    //
    // Test tone
    tone(TONE_ADT_PIN, 400, 20);
    delay(10);
    lcd->setCursor(0, 1);
    lcd->print("   ");
    lcd->print(" Bertnone    ");
    delay(1500);
    lcd->clear();
}


void Lcd16x2::displayOutTmp(void) {

    char tmpTemp[3];

    float value = car->getTmpOut();

    if (amp->isSec()) {
        //
        // Preformat ...
        displayFloat(value, tmpTemp);
        lcd->setCursor(9, 0);
        lcd->print("^");
        lcd->print(tmpTemp);
        lcd->write((uint8_t) 1);
        lcd->print(" ");
    }

    if (value < 3 && amp->isMin() && outTempLowController) {
        lcd->setCursor(1, 1);
        lcd->print("*ICE*");
    }

    if (value < 3 && amp->is10Seconds() && outTempLowController) {
        outTempLowController = false;
    }
}

/**
 * Inside temperature
 */
void Lcd16x2::displayInsTmp() {

    char tmpTemp[3];

    float value = car->getTmpIns();

    if (amp->isSec()) {
        //
        // Preformat ...
        displayFloat(value, tmpTemp);
        lcd->setCursor(9, 1);
        lcd->write((uint8_t) 5);
        lcd->print(tmpTemp);
        lcd->write((uint8_t) 1);
        lcd->print(" ");
    }
}

/**
 * Total consumption
 */
void Lcd16x2::displayTotalCons() {
    char tmp[3];
    float value = 0;

    SavedData data = eep->getData();

    if (amp->isSec()) {

        if (car->getFuelState() == 0) {
            value = data.fuel_def + car->getDefFuelCns();
        }
        if (car->getFuelState() == 1) {
            value = data.fuel_adt + car->getAdtFuelCns();
        }


        if (amp->isBig()) {
            lcd->print("    ");
        }
        //
        // Preformat ...
        displayFloat(value, tmp);
        lcd->setCursor(0, 0);
        lcd->print(" ");
        lcd->print(tmp);
        lcd->write((uint8_t) 4);
    }
}

/**
 * Total distance
 */
void Lcd16x2::displayTotalDst() {
    char tmp[3];

    SavedData data = eep->getData();

    float value = data.distance + car->getDst();


    if (amp->isSecond()) {
        //
        // Preformat ...
        displayFloat(value, tmp);
        lcd->setCursor(0, 1);
        if (value < 100) {
            lcd->print(" ");
        }
        lcd->print(tmp);
        lcd->write((uint8_t) 2);
        lcd->print(" ");
    }
}


void Lcd16x2::displayAlert(void) {

    if (!displayAlertActive && stt->isAlert() && amp->is10Seconds() || displayAlertActive && amp->isMin()) {
        lcd->setCursor(0, 1);
        lcd->print("  ");
        lcd->write((uint8_t) 3);
        lcd->print("    ");
        displayAlertActive = true;
    }

    if (displayAlertActive && amp->is5Seconds()) {
        displayAlertActive = false;
    }
}


/****************************************************************
 * Display engine RPMs
 */
void Lcd16x2::displayEngRPM() {
    char rpmDisplay[4];

    if (amp->isMid()) {
        //
        // Gets RPM

        lcd->setCursor(0, 1);
        lcd->print("RPm:");
        //
        // Handle RPM screen print
        sprintf(rpmDisplay, "%04d", car->getRpm());
        lcd->print(rpmDisplay);
    }


}

/****************************************************************
 * Display  KMh
 */
void Lcd16x2::displayCarKMH() {
    char vssDisplay[3];

    if (amp->isMid()) {
        lcd->setCursor(0, 0);
        lcd->print("KMh:");
        //
        // Handle VSS screen print
        sprintf(vssDisplay, "%03d", car->getVss());
        lcd->print(vssDisplay);
    }
}

/****************************************************************
 * Display  ENG Temp
 */
void Lcd16x2::displayEngTmp() {

    char tmpDisplay[3];

    if (amp->isSec()) {

        lcd->setCursor(9, 1);
        lcd->print("ENg:");
        //
        // Handle Dst screen print
        sprintf(tmpDisplay, "%02d", car->getEngTmp());
        lcd->print(tmpDisplay);
        lcd->write((uint8_t) 1);
    }
}

/****************************************************************
 * Display  ECU
 */
void Lcd16x2::displayCarECU() {
    char ecuDisplay[2];

    if (amp->isMid()) {

        lcd->setCursor(9, 0);
        lcd->print("ECu:");
        //
        // Handle ECU screen print
        sprintf(ecuDisplay, "%02d", car->getEcu());
        lcd->print(ecuDisplay);
        lcd->print(" ");
    }

}

/****************************************************************
 *  Travel distance
 */
void Lcd16x2::displayTrip() {
    //
    // Handle Distance screen

    if (amp->isSec()) {
        lcd->setCursor(0, 0);
        lcd->print(" Current Trip");
        //
        // Display travel time
        lcd->setCursor(0, 1);
        lcd->print(" ");
        lcd->print(car->getHTm());
        lcd->print("h");
        //
        // Display travel distance

        char dspDist[4];
        displayFloat(car->getDst(), dspDist);

        lcd->print(" ");
        lcd->setCursor(9, 1);
        lcd->print(dspDist);
        lcd->write((uint8_t) 2);
        lcd->print(" ");

    }
}

void Lcd16x2::displayTotalTrip() {
    //
    // Handle Distance screen

    if (amp->isSec()) {
        lcd->setCursor(0, 0);
        lcd->print("Total Distance");
        //
        // Display travel time
        lcd->setCursor(0, 1);
        lcd->print(" ");
        lcd->print(eep->getWorkDistance());
        lcd->write((uint8_t) 2);
        lcd->print(" ");
    }
}

void Lcd16x2::displayConsumption() {

    if (amp->isMax()) {

        lcd->setCursor(0, 0);
        lcd->print(" Consumption");


        lcd->setCursor(1, 1);

        lcd->write((uint8_t) 5);
        lcd->write((uint8_t) 6);
        lcd->print("  ");

        lcd->setCursor(9, 1);
        lcd->write((uint8_t) 7);
        lcd->write((uint8_t) 8);
        lcd->print("   ");


    }

    int dspInst[2];

    if (amp->isMid()) {

        separateFloat(car->getIfcAvr(), dspInst);

        float valueConsFuel = 0;


        if (car->getFuelState() == 0) {
            valueConsFuel = car->getDefFuelCns();
        }
        if (car->getFuelState() == 1) {
            valueConsFuel = car->getAdtFuelCns();
        }

        lcd->setCursor(1, 1);
        lcd->print((char) 5);
        lcd->print((char) 6);
        lcd->print(dspInst[0]);
        lcd->print("L ");

        lcd->setCursor(8, 1);
        lcd->write((uint8_t) 7);
        lcd->write((uint8_t) 8);
        lcd->print(valueConsFuel);
        lcd->print("L ");
    }
}


void Lcd16x2::displayFuelTanks() {
    char dspLpg[2];
    char dspBnz[2];

    if (amp->isSec()) {

        sprintf(dspLpg, "%02d", car->getTnkLpgPer());
        sprintf(dspBnz, "%02d", car->getTnkBnzPer());


        lcd->setCursor(0, 0);
        lcd->print(" Fuel Tanks");


        lcd->setCursor(0, 1);
        lcd->print("Bnz:");
        lcd->print(dspBnz);
        lcd->print("%");
        lcd->print(" Lpg:");
        lcd->print(dspLpg);
        lcd->print("% ");
    }
}


/****************************************************************
 * Average
 */
void Lcd16x2::displayAverage() {

    if (amp->isMid()) {
        lcd->setCursor(0, 0);
        lcd->print("Average");

        lcd->setCursor(0, 1);
        lcd->print(" ");
        lcd->print(car->getAvrRpm());
        lcd->print("rpm");

        lcd->setCursor(8, 1);
        lcd->print(" ");
        lcd->print(car->getAvrVss());
        lcd->print("kmh");
    }
}

/****************************************************************
 * Testing menu
 */
void Lcd16x2::displayTest() {

    char display[4];

    if (amp->isMid()) {
        lcd->setCursor(0, 0);
        lcd->print("VDS: ");
//        lcd->print((char) 127);

        lcd->print(car->getVdsDump());
//        lcd->print((char) 126);


        lcd->setCursor(0, 1);
        lcd->print("Whl Btns: ");

        lcd->print(whl->getAnalogReadButtons() / 100);

        if (whl->isDisable()) {
            lcd->print((char) 222);
        } else {
            lcd->print(" ");
        }
        lcd->print(whl->getCurrentState());
//

    }
}


/****************************************************************
 * State menu
 */
void Lcd16x2::displayCarState() {


    if (Menu16x2::cursorMenu == 4) {
        if (amp->isSecond() && !amp->is4Seconds()) {

            lcd->setCursor(0, 1);
            if (!stt->isAlert()) {
                lcd->print("no warnings  :) ");
            } else {
                lcd->print("finds warning ");
                lcd->write((uint8_t) 3);
                lcd->print("  ");
            }
        } else if (amp->is4Seconds()) {
            lcd->setCursor(0, 1);
            lcd->print("use >R to switch");
        }
    }

    if (amp->isMid()) {
        //
        // Shows header menu title
        lcd->setCursor(0, 0);
        lcd->print("Servicing  ");
        //
        // Continue with info
        lcd->setCursor(0, 1);
        if (Menu16x2::cursorMenu == 41) {
            if (stt->getLiveBrk()) lcd->print("CHECK brake wear");
            else lcd->print("Brake wear OK   ");
        }
        if (Menu16x2::cursorMenu == 42) {
            if (stt->getLiveCnt()) lcd->print("Low coolant !");
            else lcd->print("Coolant is OK   ");
        }
        if (Menu16x2::cursorMenu == 43) {
            if (stt->getLiveWin()) lcd->print("Low window wash");
            else lcd->print("Window washer OK");
        }
        if (Menu16x2::cursorMenu == 44) {
            if (stt->getLiveOil()) lcd->print("Low oil level !");
            else lcd->print("Oil level is OK ");
        }
    }

    if (Menu16x2::cursorMenu == 45) {
        if (amp->isMid()) {
            if (stt->getLiveVol()) {
                lcd->print("Voltage ");
                lcd->write((uint8_t) 3);
                lcd->print(" ");
                lcd->print(stt->getVoltage());
                lcd->print("V    ");
                lcd->setCursor(11, 13);
            } else lcd->print("Voltage is OK   ");
        }
    }


}

/****************************************************************
 * Games menu
 */
void Lcd16x2::displayCarGames(void) {
    gameResults = gms->getBestResults();
}

void Lcd16x2::displayCarGameWatch(void) {

    if (amp->isSec()) {
        lcd->setCursor(0, 1);
        lcd->print("Stopwatch");
    }
    //
    // Gets stopwatchResult result
    float result = gms->getStopwatch();
    //
    // Show user manual
    if (mmn->isNavigationActive()) {

        if (amp->isToggle() && amp->isMid()) {
            lcd->setCursor(0, 1);
            lcd->print("Hold >R to enter");

        }

        if (!amp->isToggle() && amp->isMid()) {
            lcd->setCursor(0, 1);
            lcd->print("Hit >S start/end");
        }
    } else {
        if (amp->isMid()) {
            char tmp[3];
            float value = gms->getStopwatchLive();

            //
            // Clear screen char-s at the row
            lcd->setCursor(0, 1);
            lcd->print("       |L       ");

            //
            // Live timing
            displayFloat(value, tmp);
            lcd->setCursor(1, 1);
            lcd->print(tmp);

            //
            // Last result
            value = result;
            displayFloat(value, tmp);
            lcd->setCursor(11, 1);
            lcd->print(tmp);

        }
    }

}

/**
 * Display drag race
 */
void Lcd16x2::displayCarGameDrag(void) {

    if (amp->isToggle() && amp->isMid()) {

    }

    if (!amp->isToggle() && amp->isMid()) {

    }
}

/**
 * Display game 0 to 100
 */
void Lcd16x2::displayCarGameT100() {

    float result = gms->get0to100();

    bool begin = gms->isBegin0to100();

    if (amp->isMid()) {
        char tmp[3];
        float value = gms->get0to100Live();

        if (!begin) {
            lcd->setCursor(0, 1);
            lcd->print("Stop vehicle !  ");
        } else if (car->getVss() == 0) {
            lcd->setCursor(0, 1);
            lcd->print("Ready to start..");
        } else if (car->getVss() > 0) {
            lcd->setCursor(0, 1);
            lcd->print("       |L       ");

            displayFloat(value, tmp);
            lcd->setCursor(1, 1);
            lcd->print(tmp);

            value = result;
            displayFloat(value, tmp);
            lcd->setCursor(11, 1);
            lcd->print(tmp);

        }
    }

}

void Lcd16x2::begin(void) {
    lcd->noDisplay();
    //
    // Initializes the interface to the LCD screen
    lcd->begin(16, 2);
    // and clear it ...
    lcd->clear();
    //
    // Adding custom characters to LCD
    setLcdBaseChar(lcd);
    lcd->display();
    //
    // Show welcome from car
}

void Lcd16x2::draw(int cursorMenu) {
    switch (cursorMenu) {
        case Menu16x2::MENU_ENTER:
            mmn->playEntry(lcd);
            break;
            //
            // Main / first menu
        case 1:
            displayTotalCons();
            displayTotalDst();
            displayAlert();
            displayOutTmp();
            displayInsTmp();
            break;
            //
            // Dashboard
        case 11:
            displayEngRPM();
            displayCarKMH();
            displayCarECU();
            displayEngTmp();
            break;

        case 12:
            displayTest();
            break;

            //
            // Travel menu
        case 2:
            displayTrip();
            break;
        case 21:
            displayAverage();
            break;
        case 22:
            displayTotalTrip();
            break;

            //
            // Fuel menu
        case 3:
            displayConsumption();
            break;
        case 32:
            displayFuelTanks();
            break;
        case 4:
        case 41:
        case 42:
        case 43:
        case 44:
        case 45:
            displayCarState();
            break;
            //
            // Games menu
        case 5:
            displayCarGames();
            break;
        case 51:
            gms->listenAStopwatch();
            displayCarGameWatch();
            break;
        case 52:
            displayCarGameDrag();
            break;
        case 53:
            gms->listen0to100();
            displayCarGameT100();
            break;

        case ShutDw::MENU_SHUTDOWN:
            sdw->lcd16x2(lcd);
            break;
    }
}

#endif //ARDUINOMID_READINNTERTEMP_H

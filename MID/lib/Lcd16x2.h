//
// Created by Angel Zaprianov on 27.6.2016 г..
//

#ifndef ARDUINO_MID_LCD_16x2_H
#define ARDUINO_MID_LCD_16x2_H

#include <Arduino.h>
#include "../conf.h"

#include <LiquidCrystal.h>
#include "MainFunc.h"
#include "Menu16x2.h"
#include "MenuBase.h"
#include "ShutDw.h"
#include "CarState.h"

//#include "CarGames.h"
#include "graphics/LcdChar.h"



//
//
/****************************************************************
 * Display
 */
class Lcd16x2 : virtual public LcdMenuInterface {
    IntAmp *amp;
    LiquidCrystal *lcd;
    CarSens *car;
    EepRom *eep;
    CarState *stt;
    WhlSens *whl;
    MenuBase *mbs;
    ShutDw *sdw;


protected:
    boolean outTempLowController = true;
    boolean displayAlertActive = false;

    char displayChar_2[2];
    char displayChar_3[3];
    char displayChar_4[4];

public:

    Lcd16x2(LiquidCrystal *_lcd, MenuBtn *_btn, MenuBase *_mbs, ShutDw *_sdw) {
        lcd = _lcd;
        amp = _btn->passAmp();
        car = _btn->passCar();
        eep = _btn->passEep();
        stt = _btn->passStt();
        whl = _btn->passWhl();
        mbs = _mbs;
        sdw = _sdw;
    }

    void intro(void);

    void begin(void);

    void draw(uint8_t cursor);

protected:
    void displayOutTmp();

    void displayInsTmp();

    void displayTotalCns();

    void displayTotalDst();

    void displayCarAlert();

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
// TODO Lowering code for sRAM exhausting
//    void displayCarGames();
//
//    void displayCarGameWatch();
//
//    void displayCarGameDrag();
//
//    void displayCarGameT100();
};


/**
 * Welcome screen ...
 */
void Lcd16x2::intro(void) {
    lcd->setCursor(0, 0);
    lcd->print(F("    ASTRA       "));
    //
    // Test tone
    tone(TONE_ADT_PIN, 400, 20);
    delay(10);
    lcd->setCursor(0, 1);
    lcd->print(F("   "));
    lcd->print(F(" Bertnone    "));
    delay(1500);
    lcd->clear();
}

/****************************************************************
 * Main screen begin ...
 */

void Lcd16x2::displayOutTmp(void) {

    if (amp->isSec()) {
        //
        // Preformat ...
        displayFloat(car->getTmpOut(), displayChar_3);
        lcd->setCursor(9, 0);
        lcd->print("^");
        lcd->print(displayChar_3);
        lcd->write((uint8_t) 1);
        lcd->print(" ");
    }

    if (car->getTmpOut() < 1 && amp->isMin() && outTempLowController) {
        lcd->setCursor(1, 1);
        tone(TONE_ADT_PIN, 800, 20);
        lcd->print(F("[ICE]"));
    }
    //
    // Check memory usage every 10 seconds
    if (amp->is10Seconds()) {
        if (getFreeRam() < 1000) {
            lcd->setCursor(1, 1);
            tone(TONE_ADT_PIN, 800, 20);
            lcd->print(F("sRAM!"));
        }
    }

    if (car->getTmpOut() < 1 && amp->is10Seconds() && outTempLowController) {
        outTempLowController = false;
    }
}

/**
 * Inside temperature
 */
void Lcd16x2::displayInsTmp() {

    if (amp->isSec()) {
        //
        // Preformat ...
        displayFloat(car->getTmpIns(), displayChar_3);
        lcd->setCursor(9, 1);
        lcd->write((uint8_t) 5);
        lcd->print(displayChar_3);
        lcd->write((uint8_t) 1);
        lcd->print(" ");
    }
}

/**
 * Total consumption
 */
void Lcd16x2::displayTotalCns() {

    float value = 0;

    SavedData data = eep->getData();

    if (amp->isSec()) {
        //
        // Switching between LPG / BNZ
        if (car->getFuelState() == 0) { // BNZ [default]
            value = data.fuel_def + car->getDefFuelCns();
        }
        if (car->getFuelState() == 1) { // LPG [additional]
            value = data.fuel_adt + car->getAdtFuelCns();
        }

        //
        // Reset screen place
        if (amp->isBig()) {
            lcd->print(F("    "));
        }
        //
        // Preformat ...
        displayFloat(value, displayChar_3);
        lcd->setCursor(0, 0);
        lcd->print(" ");
        lcd->print(displayChar_3);
        lcd->write((uint8_t) 4);
    }
}

/**
 * Total distance
 */
void Lcd16x2::displayTotalDst() {


    SavedData data = eep->getData();

    float value = data.dist_trv + car->getDst();


    if (amp->isSecond()) {
        //
        // Preformat ...
        displayFloat(value, displayChar_3);
        lcd->setCursor(0, 1);
        if (value < 100) {
            lcd->print(" ");
        }
        lcd->print(displayChar_3);
        lcd->write((uint8_t) 2);
        lcd->print(" ");
    }
}

/**
 * Display warnings if some alert is detected
 */
void Lcd16x2::displayCarAlert(void) {

    if (!displayAlertActive && stt->isAlert() && amp->is10Seconds() || displayAlertActive && amp->isMin()) {
        lcd->setCursor(0, 1);
        lcd->print(F("  "));
        lcd->write((uint8_t) 3);
        lcd->print(F("    "));
        displayAlertActive = true;
//        tone(TONE_ADT_PIN, 1200, 60);
    }

    if (displayAlertActive && amp->is5Seconds()) {
        displayAlertActive = false;
    }
}


/****************************************************************
 * Display engine RPMs
 */
void Lcd16x2::displayEngRPM() {


    if (amp->isMid()) {
        //
        // Gets RPM

        lcd->setCursor(0, 1);
        lcd->print(F("RPm:"));
        //
        // Handle RPM screen print
        sprintf(displayChar_4, "%04d", car->getRpm());
        lcd->print(displayChar_4);
    }


}

/****************************************************************
 * Display  KMh
 */
void Lcd16x2::displayCarKMH() {

    if (amp->isMid()) {
        lcd->setCursor(0, 0);
        lcd->print(F("KMh:"));
        //
        // Handle VSS screen print
        sprintf(displayChar_3, "%03d", car->getVss());
        lcd->print(displayChar_3);
    }
}

/****************************************************************
 * Display  ENG Temp
 */
void Lcd16x2::displayEngTmp() {


    if (amp->isSec()) {

        lcd->setCursor(9, 1);
        lcd->print(F("ENg:"));
        //
        // Handle Dst screen print
        sprintf(displayChar_3, "%02d", car->getEngTmp());
        lcd->print(displayChar_3);
        lcd->write((uint8_t) 1);
    }
}

/****************************************************************
 * Display  ECU
 */
void Lcd16x2::displayCarECU() {

    if (amp->isSec()) {

        lcd->setCursor(9, 0);
        lcd->print("ECu:");
        //
        // Handle ECU screen print
        sprintf(displayChar_2, "%02d", car->getEcu());
        lcd->print(displayChar_2);
        lcd->print(" ");
    }

}

/****************************************************************
 *  Travel distance
 */
void Lcd16x2::displayTrip() {
    //
    // Handle Distance screen
    SavedData saved = eep->getData();

    if (amp->isSec()) {
        lcd->setCursor(0, 0);
        lcd->print(F(" Current Trip"));
        //
        // Display travel time
        lcd->setCursor(0, 1);
        lcd->print(" ");
        lcd->print(car->getHTm(saved.time_trp));
        lcd->print("h");
        //
        // Display travel distance

        displayFloat(car->getDst() + saved.dist_trp, displayChar_4);

        lcd->print(" ");
        lcd->setCursor(9, 1);
        lcd->print(displayChar_4);
        lcd->write((uint8_t) 2);
        lcd->print(" ");

    }

}

void Lcd16x2::displayTotalTrip() {
    //
    // Handle Distance screen

    if (amp->isSec()) {
        lcd->setCursor(0, 0);
        lcd->print(F("Total Distance  "));
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
        lcd->print(F(" Consumption"));


        lcd->setCursor(1, 1);

        lcd->write((uint8_t) 5);
        lcd->write((uint8_t) 6);
        lcd->print(F("  "));

        lcd->setCursor(9, 1);
        lcd->write((uint8_t) 7);
        lcd->write((uint8_t) 8);
        lcd->print(F("   "));


    }

    int dspInst[2];

    if (amp->isSec()) {

        separateFloat(car->getIfcAvr(), dspInst);

        float valueConsFuel = 0;


        if (car->getFuelState() == 0) {
            valueConsFuel = car->getDefFuelCns();
        }
        if (car->getFuelState() == 1) {
            valueConsFuel = car->getAdtFuelCns();
        }

        const char *liters = "L ";

        lcd->setCursor(1, 1);
        lcd->print((char) 5);
        lcd->print((char) 6);
        lcd->print(dspInst[0]);
        lcd->print(liters);

        lcd->setCursor(8, 1);
        lcd->write((uint8_t) 7);
        lcd->write((uint8_t) 8);
        lcd->print(valueConsFuel);
        lcd->print(liters);
    }
}


void Lcd16x2::displayFuelTanks() {

    if (amp->isSec()) {

        lcd->setCursor(0, 0);
        lcd->print(F(" Fuel Tanks"));
        lcd->setCursor(0, 1);
        lcd->print("Bnz:");
        sprintf(displayChar_2, "%02d", car->getTnkBnzPer());
        lcd->print(displayChar_2);
        lcd->print("%");
        lcd->print(" Lpg:");
        sprintf(displayChar_2, "%02d", car->getTnkLpgPer());
        lcd->print(displayChar_2);
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
        lcd->print(F("VDS: "));
//        lcd->print((char) 127);

        lcd->print(car->getVdsDump());
//        lcd->print((char) 126);


        lcd->setCursor(0, 1);
        lcd->print(F("Whl Btns: "));

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


    if (MidCursorMenu == 4) {
        if (amp->isSecond() && !amp->is4Seconds()) {

            lcd->setCursor(0, 1);
            if (!stt->isAlert()) {
                lcd->print(F("no warnings  :) "));
            } else {
                lcd->print(F("finds warning "));
                lcd->write((uint8_t) 3);
                lcd->print("  ");
            }
        } else if (amp->is4Seconds()) {
            lcd->setCursor(0, 1);
            lcd->print(F("use >R to switch"));
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
        if (MidCursorMenu == 41) {
            if (stt->getLiveBrk()) lcd->print(F("CHECK brake wear"));
            else lcd->print(F("Brake wear OK   "));
        }
        if (MidCursorMenu == 42) {
            if (stt->getLiveCnt()) lcd->print(F("Low coolant !"));
            else lcd->print(F("Coolant is OK   "));
        }
        if (MidCursorMenu == 43) {
            if (stt->getLiveWin()) lcd->print("Low window wash");
            else lcd->print(F("Window washer OK"));
        }
        if (MidCursorMenu == 44) {
            if (stt->getLiveOil()) lcd->print(F("Low oil level !"));
            else lcd->print(F("Oil level is OK "));
        }
    }

    if (MidCursorMenu == 45) {
        if (amp->isMid()) {
            if (stt->getLiveVol()) {
                lcd->print(F("Voltage "));
                lcd->write((uint8_t) 3);
                lcd->print(" ");
                lcd->print(stt->getVoltage());
                lcd->print("V    ");
                lcd->setCursor(11, 13);
            } else lcd->print(F("Voltage is OK   "));
        }
    }


}
//
// TODO Lowering code for sRAM exhausting
/****************************************************************
 * Games menu
 *
void Lcd16x2::displayCarGames(void) {
//    gameResults = gms->getBestResults();
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

/*
 * Display drag race
 *
void Lcd16x2::displayCarGameDrag(void) {

    if (amp->isToggle() && amp->isMid()) {

    }

    if (!amp->isToggle() && amp->isMid()) {

    }
}

/**
 * Display game 0 to 100
 *
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
*/
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

void Lcd16x2::draw(uint8_t cursor) {

    if (amp->isSecond()) {
        Serial.print("Drawing: ");
        Serial.println(MidCursorMenu);
    }


    switch (MidCursorMenu) {
        default:
        case MENU_ENTRY:
            mbs->startEntry();
            lcd->clear();
            lcd->setCursor(0, 0);
            lcd->print("~ ");
            tone(TONE_ADT_PIN, 2800, 16);
            delay(100);
            lcd->print(MenuBase::usedMenu);
            lcd->setCursor(16, 0);
            delay(300);  //delay to allow message reading
            lcd->setCursor(0, 0);
            car->clearBaseData();
            mbs->finishEntry();
            lcd->clear();

            if (amp->isSecond()) {
                Serial.print("Drawing entry: ");
                Serial.println(cursor);
            }

            break;
            //
            // Main / first menu
        case 1:
            displayTotalCns();
            displayTotalDst();
            displayCarAlert();
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
/*
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
            break;*/

        case ShutDw::MENU_SHUTDOWN:
            sdw->lcd16x2(lcd);
            break;
    }
}

#endif //ARDUINOMID_READINNTERTEMP_H

//
// Created by Angel Zaprianov on 27.6.2016 г..
//

#ifndef ARDUINO_MID_LCD_16x2_H
#define ARDUINO_MID_LCD_16x2_H

#include <Arduino.h>
#include "../../conf.h"

#include <LiquidCrystal.h>
#include "../MainFunc.h"
#include "Menu16x2.h"
#include "../MenuBase.h"
#include "../ShutDw.h"
#include "../CarState.h"

//#include "CarGames.h"
#include "../graphics/LcdChar.h"



//
//
/****************************************************************
 * Display
 */
class Lcd16x2 : virtual public LcdMenuInterface {

    LiquidCrystal *lcd;
    AmpTime *amp;
    CarSens *car;
    EepRom *eep;
    CarState *stt;
    WhlSens *whl;
    MenuBase *mbs;
    ShutDw *sdw;
    MenuBtn *btn;


protected:
    //
    // Additional messaging controlled vars
    boolean displayTemperatureLowActive = true;
    boolean displayAlertMessagingActive = false;
    //
    // Defining content generate container variables
//    char displayChar_2[3];
//    char displayChar_3[4];
//    char displayChar_4[5];


public:

    Lcd16x2(LiquidCrystal &_lcd, MenuBtn &_btn, MenuBase &_mbs, ShutDw &_sdw) :
            lcd(&_lcd), btn(&_btn), mbs(&_mbs), amp(_btn.passAmp()), car(_btn.passCar()), eep(_btn.passEep()),
            whl(_btn.passWhl()), stt(_btn.passStt()), sdw(&_sdw) {}

/**
 * Welcome screen ...
 */
    void intro(void) {
        lcd->setCursor(0, 0);
        lcd->print(F("    ASTRA       "));
        //
        // Test tone
        tone(TONE_ADT_PIN, 400, 20);
        delay(10);
        lcd->setCursor(0, 1);
        lcd->print(F("   "));
        lcd->print(F(" Bertone    "));
        delay(1500);
        lcd->clear();
    }

    void begin(void) {
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

    void draw(void);


/****************************************************************
 * SHUTDOWN METHODS
 */
/**
 * Draws shutdown begin for trip save
 */
    void drawShutdownBegin() {
        lcd->clear();
        lcd->setCursor(0, 0);
        lcd->print(F("Hit \"S<\" to   "));
        lcd->setCursor(0, 1);
        lcd->print(F("skip saving TRIP"));
    }

/**
 * Draws shutdown begin for trip save
 */
    void drawShutdownShort() {
        lcd->clear();
        lcd->setCursor(0, 0);
        lcd->print(F("Goodbye ...     "));
        lcd->setCursor(0, 1);
        lcd->print(F("until next time!"));
    }

/**
 * Draws countdown time for saving trip
 */
    void drawShutdownCount(char sec[2]) {
        lcd->setCursor(0, 0);
        lcd->print(F("Waiting "));
        lcd->print(sec);
        lcd->print(F(" sec.  "));
        lcd->setCursor(1, 2);
        lcd->print(F(" for ESC saving "));
    }

/**
 *
 */
    void draWShutdownTripSave() {
        lcd->clear();
        lcd->setCursor(1, 0);
        lcd->print(F(" Trip saved :)  "));
        lcd->setCursor(1, 2);
        lcd->print(F(" Bye bye ...    "));
    }

/**
 *
 */
    void draWShutdownTripSkip() {
        lcd->clear();
        lcd->setCursor(1, 0);
        lcd->print(F("Trip data cancel"));
        lcd->setCursor(1, 1);
        lcd->print(F("Erasing trip ..."));
    }


protected:
/****************************************************************
 * Main screen begin ...
 */

    void displayOutTmp(void) {

        if (amp->isBig()) {
            //
            // Preformat ...
            displayFloat(car->getTmpOut(), displayChar_3);
            lcd->setCursor(9, 0);
            lcd->print("^");
            lcd->print(displayChar_3);
            lcd->write((uint8_t) 1);
            lcd->print(" ");
        }

        if (car->getTmpOut() < 1 && amp->isMin() && displayTemperatureLowActive) {
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

        if (car->getTmpOut() < 1 && amp->is10Seconds() && displayTemperatureLowActive) {
            displayTemperatureLowActive = false;
        }
    }

/**
 * Inside temperature
 */
    void displayInsTmp() {

        if (amp->isBig()) {
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

/*    float getConsumedFuel() {
        //
        // Load saved data
        SavedData data = eep->getData();
        //
        // Switching between LPG / BNZ
        if (car->getFuelState() == 0) { // BNZ [default]
            return data.fuel_def + car->getDefFuelCns();
        }
        if (car->getFuelState() == 1) { // LPG [additional]
            return data.fuel_adt + car->getAdtFuelCns();
        }
    }*/

/**
 * Total consumption
 */
    void displayTotalCns() {

        if (amp->is2Seconds()) {
            //
            // Reset screen place
            if (amp->isBig()) {
                lcd->print(F("    "));
            }

            lcd->setCursor(0, 0);


            if (amp->is4Seconds()) {
                //
                // 4 seconds to display average consumption per 100km

                float value = ((eep->getData().dist_trv + car->getDst()) / eep->getConsumedFuel());
                displayFloat(value, displayChar_3);

                lcd->print(displayChar_3);
                lcd->print("L/");
                lcd->write((uint8_t) 3);
                lcd->write((uint8_t) 2);
//            lcd->print(" ");
            } else {
                //
                // Consumed fuel
                lcd->print("        ");
                lcd->setCursor(1, 0);
                displayFloat(eep->getConsumedFuel(), displayChar_3);
                lcd->print(displayChar_3);
                lcd->write((uint8_t) 4);
                lcd->print("  ");
            }
        }
    }

/**
 * Total distance
 */
    void displayTotalDst() {
        //
        //
        if (amp->isSecond()) {
            SavedData data = eep->getData();
            float value = data.dist_trv + car->getDst();
            //
            // Preformat ...
            lcd->setCursor(0, 1);
            if (value < 100) {
                lcd->print(" ");
            }
            displayFloat(value, displayChar_3);
            lcd->print(displayChar_3);
            lcd->write((uint8_t) 2);
            lcd->print(" ");
        }
    }

/**
 * Display warnings if some alert is detected
 */
    void displayCarAlert(void) {

        if (!displayAlertMessagingActive && stt->isAlert() && amp->is10Seconds() ||
            displayAlertMessagingActive && amp->isMin()) {
            lcd->setCursor(0, 1);
            lcd->print(F("  "));
//        lcd->write((uint8_t) 3);
            lcd->write("!");
            lcd->print(F("    "));
            displayAlertMessagingActive = true;
//        tone(TONE_ADT_PIN, 1200, 60);
        }

        if (displayAlertMessagingActive && amp->is5Seconds()) {
            displayAlertMessagingActive = false;
        }
    }


/****************************************************************
 * Display engine RPMs
 */
    void displayEngRPM() {


        if (amp->isBig()) {
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
    void displayCarKMH() {

        if (amp->isBig()) {
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
    void displayEngTmp() {


        if (amp->isBig()) {
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
    void displayCarECU() {

        if (amp->isBig()) {

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
    void displayTrip() {
        //
        // Handle Distance screen
        if (amp->isBig()) {
            SavedData saved = eep->getData();
            lcd->setCursor(0, 0);
            lcd->print(F(" Current Trip"));
            //
            // Display travel time
            lcd->setCursor(0, 1);
            lcd->print(F(" "));
            /* 11 = len of clock time + 1 char for \0*/
            char dspTime[6] = "00:00";
            unsigned long tmSec;
            int tmMin, tmHrs;

            tmSec = millis() / 1000;
            tmMin = int(tmSec / 60);
            tmHrs = tmMin / 60;


            sprintf(dspTime, "%02d:%02d", tmHrs, tmMin);
            lcd->print(dspTime /*car->getHTm(*//*saved.time_trp*//*)*/);
            lcd->print(F("h"));
            //
            // Display travel distance
            displayFloat(car->getDst()/* + saved.dist_trp*/, displayChar_4);

            lcd->print(F(" "));
            lcd->setCursor(9, 1);
            lcd->print(displayChar_4);
//            lcd->write((uint8_t) 2);
            lcd->print(F(" "));

        }

    }

    void displayTotalTrip() {
        //
        // Handle Distance screen

        if (amp->isBig()) {
            lcd->setCursor(0, 0);
            lcd->print(F("Total Distance  "));
            //
            // Display travel time
            lcd->setCursor(0, 1);
            lcd->print(F(" "));
            lcd->print(eep->getWorkDistance());
            lcd->write((uint8_t) 2);
            lcd->print(F(" "));
        }
    }

    void displayConsumption() {

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

        if (amp->isBig()) {

            separateFloat(car->getIfcAvr(), dspInst);

            float valueConsFuel = 0;


            if (car->getFuelState() == 0) {
                valueConsFuel = car->getDefFuelCns();
            }
            if (car->getFuelState() == 1) {
                valueConsFuel = car->getAdtFuelCns();
            }

            const __FlashStringHelper *liters = F("L ");

            lcd->setCursor(1, 1);
            lcd->write((uint8_t) 5);
            lcd->write((uint8_t) 6);
            lcd->print(dspInst[0]);
            lcd->print(liters);

            lcd->setCursor(8, 1);
            lcd->write((uint8_t) 7);
            lcd->write((uint8_t) 8);
            lcd->print(valueConsFuel);
            lcd->print(liters);
        }
    }


    void displayFuelTanks() {

        if (amp->isBig()) {

            lcd->setCursor(0, 0);
            lcd->print(F(" Fuel Tanks"));
            lcd->setCursor(0, 1);
            lcd->print(F("Bnz:"));
            sprintf(displayChar_2, "%02d", car->getTnkBnzPer());
            lcd->print(displayChar_2);
            lcd->print(F("%"));
            lcd->print(F(" Lpg:"));
            sprintf(displayChar_2, "%02d", car->getTnkLpgPer());
            lcd->print(displayChar_2);
            lcd->print(F("% "));
        }
    }


/****************************************************************
 * Average
 */
    void displayAverage() {

        if (amp->isBig()) {
            lcd->setCursor(0, 0);
            lcd->print(F("Average"));

            lcd->setCursor(0, 1);
            lcd->print(F(" "));
            lcd->print(car->getAvrRpm());
            lcd->print(F("rpm"));

            lcd->setCursor(8, 1);
            lcd->print(F(" "));
            lcd->print(car->getAvrVss());
            lcd->print(F("kmh"));
        }
    }

/****************************************************************
 * Testing menu
 */
    void displayTest() {

        char display[4];

        if (amp->isBig()) {
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
                lcd->print(F(" "));
            }
            lcd->print(whl->getCurrentState());
//

        }
    }

    void displayResetFuel() {
        lcd->setCursor(0, 0);
        if (amp->isSecond()) {
            if (amp->is2Seconds()) {
                lcd->print(F("Press S< here to"));
            } else {
                lcd->print(F("Reset fuel & km "));
            }
        }

        if (amp->isSecond()) {
            lcd->setCursor(0, 1);
            if (amp->is2Seconds()) {
                lcd->print(F("WAIT ...."));
//                btn->setNavigationState(1);
            } else {
//                btn->setNavigationState(0);
                lcd->print(F("Now!  "));
                tone(TONE_ADT_PIN, 1500, 10);
                if (btn->isUp()) {
//            eep->saveResetData();
                    Serial.println(F("Zero saved"));
                    tone(TONE_ADT_PIN, 2500, 50);
                }
            }

        }


    }

    void displayAboutInfo() {
        lcd->setCursor(0, 0);
        if (amp->isSecond()) {
            lcd->print(F("ArduinoMid      "));
            lcd->setCursor(0, 1);
            lcd->print(MID_VERSION);
        }
    }

/****************************************************************
 * State menu
 */
    void displayCarState() {


        if (MidCursorMenu == 4) {
            if (amp->isSecond() && !amp->is4Seconds()) {

                lcd->setCursor(0, 1);
                if (!stt->isAlert()) {
                    lcd->print(F("no warnings  :) "));
                } else {
                    lcd->print(F("finds warning "));
//                lcd->write((uint8_t) 3);
                    lcd->print("!");
                    lcd->print("  ");
                }
            } else if (amp->is4Seconds()) {
                lcd->setCursor(0, 1);
                lcd->print(F("use >R to switch"));
            }
        }

        if (amp->isBig()) {
            //
            // Shows header menu title
            lcd->setCursor(0, 0);
            lcd->print(F("Servicing  "));
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
            if (MidCursorMenu == 46) {
                lcd->print(F("Range: "));
                lcd->print(eep->getWorkDistance());
                lcd->write((uint8_t) 2);
                lcd->print(" ");
            }
        }

        if (MidCursorMenu == 45) {
            if (amp->isBig()) {
                if (stt->getLiveVol()) {
                    lcd->print(F("Voltage "));
//                lcd->write((uint8_t) 3);
                    lcd->print(F("!"));
                    lcd->print(F(" "));
                    lcd->print(stt->getVoltage());
                    lcd->print(F("V    "));
                    lcd->setCursor(11, 13);
                } else lcd->print(F("Voltage is OK   "));
            }
        }


    }
// TODO Lowering code for sRAM exhausting
//    void displayCarGames();
//
//    void displayCarGameWatch();
//
//    void displayCarGameDrag();
//
//    void displayCarGameT100();
};





//
// TODO Lowering code for sRAM exhausting
/****************************************************************
 * Games menu
 *
void displayCarGames(void) {
//    gameResults = gms->getBestResults();
}

void displayCarGameWatch(void) {

    if (amp->isBig()) {
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
void displayCarGameDrag(void) {

    if (amp->isToggle() && amp->isMid()) {

    }

    if (!amp->isToggle() && amp->isMid()) {

    }
}

/**
 * Display game 0 to 100
 *
void displayCarGameT100() {

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


void Lcd16x2::draw(void) {
/*

    if (amp->isSecond()) {
        Serial.print("Drawing: ");
        Serial.println(MidCursorMenu);
    }
*/


    switch (MidCursorMenu) {
        default:
        case MENU_ENTRY:
            mbs->startEntry();
            lcd->clear();
            lcd->setCursor(0, 0);
            lcd->print("~ ");
            tone(TONE_ADT_PIN, 2800, 16);
            delay(100);
            lcd->print(usedMenu.used);
            lcd->setCursor(16, 0);
            delay(300);  //delay to allow message reading
            lcd->setCursor(0, 0);
            car->clearBuffer();
            mbs->finishEntry();
            lcd->clear();
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
            displayCarKMH();
            displayEngRPM();
            displayCarECU();
            displayEngTmp();
            break;
            //
            // Reset fuel menu
        case 12:
            displayResetFuel();
            break;
            //
            // Test
        case 13:
            displayTest();
            break;
            //
            // About
        case 14:
            displayAboutInfo();
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
        case 46:
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
            sdw->menu(this);


            break;
    }
}

#endif //ARDUINOMID_READINNTERTEMP_H

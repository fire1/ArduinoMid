//
// Created by Angel Zaprianov on 27.6.2016 г..
//

#ifndef ARDUINOMID_LCD_DISPLAY_16x2_H
#define ARDUINOMID_LCD_DISPLAY_16x2_H

#include <Arduino.h>
#include <LiquidCrystal.h>
//
// add Calculators
#include "MainFunc.h"
#include "MidMenu.h"
#include "ShutDw.h"
#include "CarState.h"
#include "CarGames.h"
#include "graphics/LcdChar.h"

//
//
/****************************************************************
 * Display temperature sensor
 */

boolean outTempLowController = true;

void displayOutTmp() {

    char tmpTemp[3];

    float value = carSens.getTmpOut();

    if (ampInt.isSec()) {
        //
        // Preformat ...
        displayFloat(value, tmpTemp);
        lcd.setCursor(9, 0);
        lcd.print("^");
        lcd.print(tmpTemp);
        lcd.write((uint8_t) 1);
        lcd.print(" ");
    }

    if (value < 3 && ampInt.isMin() && outTempLowController) {
        lcd.setCursor(1, 1);
        lcd.write((uint8_t) 9);
        lcd.print("ICE");
        lcd.write((uint8_t) 9);
    }

    if (value < 3 && ampInt.is10Seconds() && outTempLowController) {
        outTempLowController = false;
    }
}

/**
 * Inside temperature
 */
void displayInsTmp() {

    char tmpTemp[3];

    float value = carSens.getTmpIns();

    if (ampInt.isSec()) {
        //
        // Preformat ...
        displayFloat(value, tmpTemp);
        lcd.setCursor(9, 1);
        lcd.write((uint8_t) 5);
        lcd.print(tmpTemp);
        lcd.write((uint8_t) 1);
        lcd.print(" ");
    }
}

/**
 * Total consumption
 */
void displayTotalCons() {
    char tmp[3];
    float value = 0;

    SavedData data = eepRom.getData();

    if (ampInt.isSec()) {

        if (carSens.getFuelState() == 0) {
            value = data.fuel_def + carSens.getDefFuelCns();
        }
        if (carSens.getFuelState() == 1) {
            value = data.fuel_adt + carSens.getAdtFuelCns();
        }


        if (ampInt.isBig()) {
            lcd.print("    ");
        }
        //
        // Preformat ...
        displayFloat(value, tmp);
        lcd.setCursor(0, 0);
        lcd.print(" ");
        lcd.print(tmp);
        lcd.write((uint8_t) 4);
    }
}

/**
 * Total distance
 */
void displayTotalDst() {
    char tmp[3];

    SavedData data = eepRom.getData();

    float value = data.distance + carSens.getDst();


    if (ampInt.isSecond()) {
        //
        // Preformat ...
        displayFloat(value, tmp);
        lcd.setCursor(0, 1);
        if (value < 100) {
            lcd.print(" ");
        }
        lcd.print(tmp);
        lcd.write((uint8_t) 2);
        lcd.print(" ");
    }
}

boolean displayAlertActive = false;

void displayAlert() {
    if (!displayAlertActive && carStat.isAlert() && ampInt.is10Seconds() || displayAlertActive && ampInt.isMin()) {
        lcd.setCursor(0, 1);
        lcd.print("  ");
        lcd.write((uint8_t) 3);
        lcd.print("    ");
        displayAlertActive = true;
    }

    if (displayAlertActive && ampInt.is5Seconds()) {
        displayAlertActive = false;
    }
}


/****************************************************************
 * Display engine RPMs
 */
void displayEngRPM() {
    char rpmDisplay[4];

    if (ampInt.isMid()) {
        //
        // Gets RPM

        lcd.setCursor(0, 1);
        lcd.print("RPm:");
        //
        // Handle RPM screen print
        sprintf(rpmDisplay, "%04d", carSens.getRpm());
        lcd.print(rpmDisplay);
    }


}

/****************************************************************
 * Display  KMh
 */
void displayCarKMH() {
    char vssDisplay[3];

    if (ampInt.isMid()) {
        lcd.setCursor(0, 0);
        lcd.print("KMh:");
        //
        // Handle VSS screen print
        sprintf(vssDisplay, "%03d", carSens.getVss());
        lcd.print(vssDisplay);
    }
}

/****************************************************************
 * Display  ENG Temp
 */
void displayEngTmp() {

    char tmpDisplay[3];

    if (ampInt.isSec()) {

        lcd.setCursor(9, 1);
        lcd.print("ENg:");
        //
        // Handle Dst screen print
        sprintf(tmpDisplay, "%02d", carSens.getEngTmp());
        lcd.print(tmpDisplay);
        lcd.write((uint8_t) 1);
    }
}

/****************************************************************
 * Display  ECU
 */
void displayCarECU() {
    char ecuDisplay[2];

    if (ampInt.isMid()) {

        lcd.setCursor(9, 0);
        lcd.print("ECu:");
        //
        // Handle ECU screen print
        sprintf(ecuDisplay, "%02d", carSens.getEcu());
        lcd.print(ecuDisplay);
        lcd.print(" ");
    }

}

/****************************************************************
 *  Travel distance
 */
void displayTrip() {
    //
    // Handle Distance screen

    if (ampInt.isSec()) {
        lcd.setCursor(0, 0);
        lcd.print(" Current Trip");
        //
        // Display travel time
        lcd.setCursor(0, 1);
        lcd.print(" ");
        lcd.print(carSens.getHTm());
        lcd.print("h");
        //
        // Display travel distance

        char dspDist[4];
        displayFloat(carSens.getDst(), dspDist);

        lcd.print(" ");
        lcd.setCursor(9, 1);
        lcd.print(dspDist);
        lcd.write((uint8_t) 2);
        lcd.print(" ");

    }
}

void displayTotalTrip() {
    //
    // Handle Distance screen

    if (ampInt.isSec()) {
        lcd.setCursor(0, 0);
        lcd.print("Total Distance");
        //
        // Display travel time
        lcd.setCursor(0, 1);
        lcd.print(" ");
        lcd.print(eepRom.getWorkDistance());
        lcd.write((uint8_t) 2);
        lcd.print(" ");
    }
}

void displayConsumption() {

    if (ampInt.isMax()) {

        lcd.setCursor(0, 0);
        lcd.print(" Consumption");


        lcd.setCursor(1, 1);

        lcd.write((uint8_t) 5);
        lcd.write((uint8_t) 6);
        lcd.print("  ");

        lcd.setCursor(9, 1);
        lcd.write((uint8_t) 7);
        lcd.write((uint8_t) 8);
        lcd.print("   ");


    }

    int dspInst[2];

    if (ampInt.isMid()) {

        separateFloat(carSens.getIfcAvr(), dspInst);

        float valueConsFuel = 0;


        if (carSens.getFuelState() == 0) {
            valueConsFuel = carSens.getDefFuelCns();
        }
        if (carSens.getFuelState() == 1) {
            valueConsFuel = carSens.getAdtFuelCns();
        }

        lcd.setCursor(1, 1);
        lcd.print((char) 5);
        lcd.print((char) 6);
        lcd.print(dspInst[0]);
        lcd.print("L ");

        lcd.setCursor(8, 1);
        lcd.write((uint8_t) 7);
        lcd.write((uint8_t) 8);
        lcd.print(valueConsFuel);
        lcd.print("L ");
    }
}


void displayFuelTanks() {
    char dspLpg[2];
    char dspBnz[2];

    if (ampInt.isSec()) {

        sprintf(dspLpg, "%02d", carSens.getTnkLpgPer());
        sprintf(dspBnz, "%02d", carSens.getTnkBnzPer());


        lcd.setCursor(0, 0);
        lcd.print(" Fuel Tanks");


        lcd.setCursor(0, 1);
        lcd.print("Bnz:");
        lcd.print(dspBnz);
        lcd.print("%");
        lcd.print(" Lpg:");
        lcd.print(dspLpg);
        lcd.print("% ");
    }
}


/****************************************************************
 * Average
 */
void displayAverage() {

    if (ampInt.isMid()) {
        lcd.setCursor(0, 0);
        lcd.print("Average");

        lcd.setCursor(0, 1);
        lcd.print(" ");
        lcd.print(carSens.getAvrRpm());
        lcd.print("rpm");

        lcd.setCursor(8, 1);
        lcd.print(" ");
        lcd.print(carSens.getAvrVss());
        lcd.print("kmh");
    }
}

/****************************************************************
 * Testing menu
 */
void displayTest() {

    char display[4];

    if (ampInt.isMid()) {
        lcd.setCursor(0, 0);
        lcd.print("VDS: ");
//        lcd.print((char) 127);

        lcd.print(carSens.getVdsDump());
//        lcd.print((char) 126);


        lcd.setCursor(0, 1);
        lcd.print("Whl Btns: ");

        lcd.print(whlSens.getAnalogReadButtons() / 100);

        if (whlSens.isDisable()) {
            lcd.print((char) 222);
        } else {
            lcd.print(" ");
        }
        lcd.print(whlSens.getCurrentState());
//

    }
}


/****************************************************************
 * State menu
 */
void displayCarState() {


    if (MidMenu::cursorMenu == 4) {
        if (ampInt.isSecond() && !ampInt.is4Seconds()) {

            lcd.setCursor(0, 1);
            if (!carStat.isAlert()) {
                lcd.print("no warnings  :) ");
            } else {
                lcd.print("finds warning ");
                lcd.write((uint8_t) 3);
                lcd.print("  ");
            }
        } else if (ampInt.is4Seconds()) {
            lcd.setCursor(0, 1);
            lcd.print("use >R to switch");
        }
    }

    if (ampInt.isMid()) {
        //
        // Shows header menu title
        lcd.setCursor(0, 0);
        lcd.print("Servicing  ");
        //
        // Continue with info
        lcd.setCursor(0, 1);
        if (MidMenu::cursorMenu == 41) {
            if (carStat.getLiveBrk()) lcd.print("CHECK brake wear");
            else lcd.print("Brake wear OK   ");
        }
        if (MidMenu::cursorMenu == 42) {
            if (carStat.getLiveCnt()) lcd.print("Low coolant !");
            else lcd.print("Coolant is OK   ");
        }
        if (MidMenu::cursorMenu == 43) {
            if (carStat.getLiveWin()) lcd.print("Low window wash");
            else lcd.print("Window washer OK");
        }
        if (MidMenu::cursorMenu == 44) {
            if (carStat.getLiveOil()) lcd.print("Low oil level !");
            else lcd.print("Oil level is OK ");
        }
    }

    if (MidMenu::cursorMenu == 45) {
        if (ampInt.isMid()) {
            if (carStat.getLiveVol()) {
                lcd.print("Voltage ");
                lcd.write((uint8_t) 3);
                lcd.print(" ");
                lcd.print(carStat.getVoltage());
                lcd.print("V    ");
                lcd.setCursor(11, 13);
            } else lcd.print("Voltage is OK   ");
        }
    }


}

/****************************************************************
 * Games menu
 */
void displayCarGames(void) {
    GamesBest results = carGames.getBestResults();


}

void displayCarGameWatch(void) {

    if (ampInt.isSec()) {
        lcd.setCursor(0, 1);
        lcd.print("Stopwatch");
    }
    //
    // Gets stopwatchResult result
    float result = carGames.getStopwatch();
    //
    // Show user manual
    if (midMenu.isNavigationActive()) {

        if (ampInt.isToggle() && ampInt.isMid()) {
            lcd.setCursor(0, 1);
            lcd.print("Hold >R to enter");

        }

        if (!ampInt.isToggle() && ampInt.isMid()) {
            lcd.setCursor(0, 1);
            lcd.print("Hit >S start/end");
        }
    } else {
        if (ampInt.isMid()) {
            char tmp[3];
            float value = carGames.getStopwatchLive();

            //
            // Clear screen char-s at the row
            lcd.setCursor(0, 1);
            lcd.print("       |L       ");

            //
            // Live timing
            displayFloat(value, tmp);
            lcd.setCursor(1, 1);
            lcd.print(tmp);

            //
            // Last result
            value = result;
            displayFloat(value, tmp);
            lcd.setCursor(11, 1);
            lcd.print(tmp);

        }
    }

}

/**
 * Display drag race
 */
void displayCarGameDrag(void) {

    if (ampInt.isToggle() && ampInt.isMid()) {

    }

    if (!ampInt.isToggle() && ampInt.isMid()) {

    }
}

/**
 * Display game 0 to 100
 */
void displayCarGameT100() {

    float result = carGames.get0to100();

    bool begin = carGames.isBegin0to100();

    if (ampInt.isMid()) {
        char tmp[3];
        float value = carGames.get0to100Live();

        if (!begin) {
            lcd.setCursor(0, 1);
            lcd.print("Stop vehicle !  ");
        } else if (carSens.getVss() == 0) {
            lcd.setCursor(0, 1);
            lcd.print("Ready to start..");
        } else if (carSens.getVss() > 0) {
            lcd.setCursor(0, 1);
            lcd.print("       |L       ");

            displayFloat(value, tmp);
            lcd.setCursor(1, 1);
            lcd.print(tmp);

            value = result;
            displayFloat(value, tmp);
            lcd.setCursor(11, 1);
            lcd.print(tmp);

        }
    }

}

#endif //ARDUINOMID_READINNTERTEMP_H

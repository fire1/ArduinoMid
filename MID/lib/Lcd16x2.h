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

//
//
/****************************************************************
 * Display temperature sensor
 */
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
        lcd.setCursor(9, 2);
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


    if (ampInt.isSec()) {

        if (carSens.getFuelState() == 0) {
            value = TTL_CNS_DEF + carSens.getDefFuelCns();
        }
        if (carSens.getFuelState() == 1) {
            value = TTL_CNS_ADT + carSens.getAdtFuelCns();
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
    float value = TTL_TTD + carSens.getDst();


    if (ampInt.isSec()) {
        //
        // Preformat ...
        displayFloat(value, tmp);
        lcd.setCursor(0, 2);
        lcd.print(" ");
        lcd.print(tmp);
        lcd.write("km");
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

        lcd.setCursor(0, 2);
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

        lcd.setCursor(9, 2);
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
        lcd.setCursor(0, 2);
        lcd.print(" ");
        lcd.print(carSens.getHTm());
        lcd.print("h");
        //
        // Display travel distance

        char dspDist[4];
        displayFloat(carSens.getDst(), dspDist);

        lcd.print(" ");
        lcd.setCursor(9, 2);
        lcd.print(dspDist);
        lcd.print("km");

    }
}


void displayConsumption() {

    if (ampInt.isMax()) {

        lcd.setCursor(0, 0);
        lcd.print(" Consumption");


        lcd.setCursor(1, 2);

        lcd.write((uint8_t) 5);
        lcd.write((uint8_t) 6);
        lcd.print("  ");

        lcd.setCursor(9, 2);
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

        lcd.setCursor(1, 2);
        lcd.print((char) 5);
        lcd.print((char) 6);
        lcd.print(dspInst[0]);
        lcd.print("L ");

        lcd.setCursor(8, 2);
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


        lcd.setCursor(0, 2);
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

        lcd.setCursor(0, 2);
        lcd.print(" ");
        lcd.print(carSens.getAvrRpm());
        lcd.print("rpm");

        lcd.setCursor(8, 2);
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
        lcd.print("Lpg Tank: ");

        sprintf(display, "%04d", carSens.getTnkLpg());

        lcd.print(display);

        lcd.setCursor(0, 2);
        lcd.print("Wheel Btn: ");

        lcd.print(whlSens.getAnalogReadButtons() / 100);

        if (whlSens.isDisable()) {
            lcd.print("'");
        } else {
            lcd.print(" ");
        }
        lcd.print(whlSens.getCurrentState());
//

    }
}

bool dspStateDig = false;

/****************************************************************
 * State menu
 */
void diaplyCarState() {

    Diagnostic dig = carStat.getResult();
    if (ampInt.isMax()) {
        lcd.setCursor(0, 0);
        if (!dspStateDig) {
            dspStateDig = true;
            lcd.print("Diagnostic State");
        } else {
            dspStateDig = false;
            lcd.print(" Ol Ct Bk Wn Vl ");
        }
    }

    if (ampInt.isMid()) {
        lcd.setCursor(0, 2);
        if (dig.oil) lcd.print("X"); else lcd.write((uint8_t) 3);
        lcd.print(" | ");
        if (dig.cnt) lcd.print("X"); else lcd.write((uint8_t) 3);
        lcd.print(" | ");
        if (dig.brk) lcd.print("X"); else lcd.write((uint8_t) 3);
        lcd.print(" | ");
        if (dig.win) lcd.print("X"); else lcd.write((uint8_t) 3);
        lcd.print(" ~");
        if (dig.vol) lcd.print("X"); else lcd.write((uint8_t) 3);

    }

    if (ampInt.isMax()) {
        lcd.setCursor(0, 0);
        lcd.print(" Ol Ct Bk Wn Vl ");
    }
}


#endif //ARDUINOMID_READINNTERTEMP_H

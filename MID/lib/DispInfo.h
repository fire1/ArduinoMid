//
// Created by Angel Zaprianov on 27.6.2016 г..
//

#ifndef ARDUINOMID_READINNTERTEMP_H
#define ARDUINOMID_READINNTERTEMP_H

#include <Arduino.h>
#include <LiquidCrystal.h>
//
// add Calculators
#include "trait/AvrgCalc.h"
#include "trait/DistCalc.h"
#include "trait/ConsCalc.h"

#include "MainFunc.h"


//
//
/****************************************************************
 * Display temperature sensor
 */
void displayOutTmp() {

    char tmpTemp[3];

    float value = getTmpOut();

    if (ampInt.isSec()) {
        //
        // Preformat ...
        displayFloat(value, tmpTemp);
        lcd.setCursor(9, 0);
        lcd.print("^");
        lcd.print(tmpTemp);
        lcd.write((uint8_t) 1);
    }
}

void displayInsTmp() {

    char tmpTemp[3];

    float value = getTmpIns();

    if (ampInt.isSec()) {
        //
        // Preformat ...
        displayFloat(value, tmpTemp);
        lcd.setCursor(9, 2);
        lcd.write((uint8_t) 5);
        lcd.print(tmpTemp);
        lcd.write((uint8_t) 1);
    }
}

void displayTotalCons() {
    char tmp[3];

    float value = getTotalCons();


    if (ampInt.isSec()) {

        if (ampInt.isBig()) {
            lcd.print("    ");
        }

        //
        // Preformat ...
        displayFloat(value, tmp);
        lcd.setCursor(1, 0);
        lcd.print(tmp);
        lcd.write((uint8_t) 4);
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
        lcd.print("RPM:");
        //
        // Handle RPM screen print
        sprintf(rpmDisplay, "%04d", carSens.getRpm());
        lcd.print(rpmDisplay);
    }


}

/****************************************************************
 * Display engine KMh
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

void displayCarDST() {

    char tmpDisplay[3];

    if (ampInt.isMid()) {

        lcd.setCursor(9, 2);
        lcd.print("D:");
        //
        // Handle Dst screen print
        displayFloat(carSens.getDst(), tmpDisplay);
        lcd.print(tmpDisplay);
    }
}

/****************************************************************
 * Display engine KMh
 */
void displayCarECU() {
    char ecuDisplay[2];

    if (ampInt.isMid()) {

        lcd.setCursor(10, 0);
        lcd.print("ECU:");
        //
        // Handle ECU screen print
        sprintf(ecuDisplay, "%02d", carSens.getEcu());
        lcd.print(ecuDisplay);
    }

}

/****************************************************************
 *  Travel distance
 */
void displayDistance() {
    //
    // Handle Distance screen

    int long tmSec;

    int tmMin, tmHrs;
    tmSec = getTravelTime();

    tmMin = tmSec / 60;
    tmHrs = tmMin / 60;

    char dspTime[5];
    sprintf(dspTime, "%02d:%02d", tmHrs, tmMin);

    if (ampInt.isMid()) {
        lcd.setCursor(0, 0);
        lcd.print(" Current Trip");
        //
        // Display travel time
        lcd.setCursor(0, 2);
        lcd.print(" ");
        lcd.print(dspTime);
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

/****************************************************************
 * Consumptions
 */
void displayConsumption() {

    if (ampInt.isMid()) {

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

    char dspInst[3];

    //
    // Handle Distance screen

    displayFloat(getInstCons(), dspInst);

    lcd.setCursor(1, 2);
    lcd.print((char) 5);
    lcd.print((char) 6);
    lcd.print(dspInst);

//    char dspTotal[3];
//    displayFloat(getTripCons(), dspTotal);

    lcd.setCursor(9, 2);
    lcd.write((uint8_t) 7);
    lcd.write((uint8_t) 8);
    lcd.print(getTripCons());
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
        lcd.print(getAverageRpm());
        lcd.print("rpm");

        lcd.setCursor(8, 2);
        lcd.print(" ");
        lcd.print(getAverageVss());
        lcd.print("kmh");
    }
}

void displayTest() {

    char display[4];

    if (ampInt.isMid()) {
        lcd.setCursor(0, 0);
        lcd.print("DST ");

        sprintf(display, "%04d", sensStr.getAnalogReadButtons());

        lcd.print(display);

//        lcd.setCursor(0, 2);
//        lcd.print(" ");
//        lcd.print(getAverageRpm());
//        lcd.print("rpm");
//
//        lcd.setCursor(8, 2);
//        lcd.print(" ");
//        lcd.print(getAverageVss());
//        lcd.print("kmh");
    }
}


#endif //ARDUINOMID_READINNTERTEMP_H

//
// Created by Angel Zaprianov on 27.6.2016 г..
//

#ifndef ARDUINOMID_READINNTERTEMP_H
#define ARDUINOMID_READINNTERTEMP_H

#include <Arduino.h>
#include <LiquidCrystal.h>
//
// add Calculators
#include "trait/DistCalc.h"
#include "trait/ConsCalc.h"
#include "trait/AvrgCalc.h"
#include "MainFunc.h"

//
//
/****************************************************************
 * Display temperature sensor
 */
void displayOutTmp() {
    //
    // Inner vars
    char tmpDisplay[3];
    int sens = getTmpOut();
    //
    // Read pin value
    if (isSensorReadMid()) {

        lcd.setCursor(10, 2);
        sprintf(tmpDisplay, "%3d", sens);
        lcd.print(tmpDisplay);
        lcd.print((char) 1);
    }

}

/****************************************************************
 * Display engine RPMs
 */
void displayEngRPM() {
    char rpmDisplay[4];
    //
    // Gets RPM
    int rpmSnsCount = getRpmSens();

    lcd.setCursor(0, 2);
    lcd.print("RPM:");

    if (isSensorReadMid()) {
        lcd.print("      ");
        lcd.setCursor(0, 2);
        lcd.print("RPM:");
    }
    //
    // Handle RPM screen print
    sprintf(rpmDisplay, "%04d", rpmSnsCount);
    lcd.print(rpmDisplay);

}

/****************************************************************
 * Display engine KMh
 */
void displayCarKMH() {
    char vssDisplay[3];

    lcd.setCursor(0, 0);
    lcd.print("KMh:");

    if (isSensorReadMid()) {
        lcd.print("    ");
        lcd.setCursor(0, 0);
        lcd.print("KMh:");
    }

    //
    // Handle VSS screen print
    sprintf(vssDisplay, "%03d", getVssSens());
    lcd.print(vssDisplay);

}

/****************************************************************
 * Display engine KMh
 */
void displayCarECU() {
    char ecuDisplay[2];

    lcd.setCursor(10, 0);
    lcd.print("ECU:");

    if (isSensorReadMid()) {
        lcd.print("   ");
        lcd.setCursor(10, 0);
        lcd.print("ECU:");
    }
    //
    // Handle ECU screen print
    sprintf(ecuDisplay, "%02d", getEcuSens());
    lcd.print(ecuDisplay);
}

/****************************************************************
 *  Travel distance
 */
void displayDistance() {
    //
    // Handle Distance screen
    int tmpDistance = getTravelDistance();
    int tmpTime = getTravelTime();// 100

    int a = tmpDistance;
    int b = tmpDistance % 10;

    if (isSensorReadLow()) {
        //
        // Display travel distance
        lcd.setCursor(0, 0);
        lcd.print("Dist:");
        lcd.setCursor(6, 0);
        lcd.print(tmpDistance);
//        lcd.print(".");
//        lcd.print(b);
        //
        // Display travel time
        lcd.setCursor(0, 2);
        lcd.print("Time:");
        lcd.setCursor(6, 2);
        lcd.print(tmpTime);
    }
}

/****************************************************************
 * Consumptions
 */
void displayConsumption() {

    if (isSensorReadMid()) {

        lcd.setCursor(0, 0);
        lcd.print(" Consumption");
        lcd.setCursor(1, 2);

        lcd.print((char)5);
        lcd.print((char)6);
        lcd.print("  ");

        lcd.setCursor(8, 2);
        lcd.print((char)7);
        lcd.print((char)8);
        lcd.print("  ");

    }

    //
    // Handle Distance screen
    lcd.setCursor(1, 2);

    lcd.print((char)5);
    lcd.print((char)6);
    lcd.print(getInstCons());

    lcd.setCursor(8, 2);
    lcd.print((char)7);
    lcd.print((char)8);
    lcd.print(getUsedFuel());
}

/****************************************************************
 * Average
 */
void displayAverage() {


    if (isSensorReadLow()) {
        lcd.setCursor(0, 0);
        lcd.print(" ");
        lcd.print((char)9);
        lcd.print((char)10);
        lcd.print(getAverageVss());
        lcd.print("kmh");

        lcd.setCursor(0, 2);
        lcd.print(" RPM:");
        lcd.print(getAverageRpm());
        lcd.print("rpm");
    }
}

#endif //ARDUINOMID_READINNTERTEMP_H

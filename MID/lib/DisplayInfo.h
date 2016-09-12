//
// Created by Angel Zaprianov on 27.6.2016 г..
//

#ifndef ARDUINOMID_READINNTERTEMP_H
#define ARDUINOMID_READINNTERTEMP_H


#include <Arduino.h>
#include <LiquidCrystal.h>



/**
 * Display temperature sensor
 */
void readInnerTemp() {
    //
    // Inner vars
    int pinReadValue;
    int temperatureC;
    //
    // Read pin value
    pinReadValue = analogRead(sensorTempPin_1);
    //
    // Check is output time is allowed
//    if (isSensorReadAllow()) {
//
//        //temperatureC = (pinReadValue * 100.0) / 1024.0;
//        //temperatureC =  (200*pin_value)>>8;
//        temperatureC = (225 * pinReadValue) >> 8;
//        // converting that reading to voltage, for 3.3v arduino use 3.3
//        int voltage = pinReadValue * 5.0;
//        //voltage /= 1024.0;
//
//        // now print out the temperature
//        // int temperatureC = (voltage - 0.5) * 100 ;
//
//        lcd.setCursor(8, 1);
//        if (temperatureC > 0) {
//            lcd.print("  ");
//
//        }
//        lcd.print(temperatureC);
//        lcd.print((char) 1);
//    }

}

char rpmDisplay[4];

/**
 * Display engine RPMs
 */
void displayEngRPM() {

    //
    // Gets RPM
    int rpmSnsCount = getRpmSens();

    lcd.setCursor(0, 2);
    lcd.print("RPM:");

//    if (rpmSnsCount < 100) {
//        lcd.print(0);
//        lcd.print(0);
//    }
//    if (rpmSnsCount < 1000 ) {
//        lcd.print(0);
//    }

    if (isSensorReadAllow()) {
        lcd.print("      ");
        lcd.setCursor(0, 2);
        lcd.print("RPM:");
    }
    //
    // Handle screen display
    int rpmH = rpmSnsCount / 1000;
    int rpmH2 = rpmSnsCount / 100;
    int rpm2s = rpmH2 - (rpmH * 10);
    int rpm3s = (rpmSnsCount / 10) - (rpmH2 * 10);

    rpmDisplay[0] = '0' + rpmH;
    rpmDisplay[1] = '0' + rpm2s;
    rpmDisplay[2] = '0' + rpm3s;
    rpmDisplay[3] = '0' + (rpmSnsCount % 10);

//    if (rpmSnsCount < 600) {
//        rpmDisplay[0] = '0';
//        rpmDisplay[1] = '0';
//        rpmDisplay[2] = '0';
//        rpmDisplay[3] = '0';
//    }

    lcd.print(rpmDisplay);

}

/**
 * Display engine KMh
 */
void displayCarKMH() {
    int kmhSnsCount = 0;

    //
    // Gets RPM
    kmhSnsCount = getDigitalSpeedKmh();

    lcd.setCursor(0, 0);
    lcd.print("KMh:");
    //
    // Handle screen display
    if (kmhSnsCount < 10) {
        lcd.print("  ");
    }
    lcd.print(kmhSnsCount);
}

/**
 * Display engine KMh
 */
void displayCarECU() {
    int counterEcu = 0;

    //
    // Gets RPM
    counterEcu = getDigitalEngineEcu();

    lcd.setCursor(8, 0);
    lcd.print("ECU:");
    //
    // Handle screen display
    if (counterEcu < 10) {
        lcd.print("  ");
    }
    lcd.print(counterEcu);
}


#endif //ARDUINOMID_READINNTERTEMP_H

//
// Created by Angel Zaprianov on 27.6.2016 г..
//

#ifndef ARDUINOMID_READINNTERTEMP_H
#define ARDUINOMID_READINNTERTEMP_H


#include <Arduino.h>
#include <LiquidCrystal.h>


//
// Read Tachometer
#include "sensors/Tachometer.h"
//
// Read SpeedHub
#include "sensors/SpeedSens.h"
//
//
#include "sensors/ReadEcu.h"

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
    int rpmSnsCount = 0;

    //
    // Gets RPM
    rpmSnsCount = getDigitalTachometerRpm();
//    rpmSnsCount = getDigitalTachometerRpm2();

    lcd.setCursor(0, 2);
    lcd.print("RPM:");
    //
    // Handle screen display
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

    int rpmh = rpmSnsCount / 1000;

    int rpmh2 = rpmSnsCount / 100;

    int rpm2s = rpmh2 - (rpmh * 10);

    int rpm3s = (rpmSnsCount / 10) - (rpmh2 * 10);

//    if (rpmSnsCount < 700) {
//        rpmDisplay[0] = '0';
//        rpmDisplay[1] = '0';
//        rpmDisplay[2] = '0';
//        rpmDisplay[3] = '0';
//    }
    lcd.print(rpmSnsCount);
//
//    Serial.print("RPM \t \t");
//    Serial.print(rpmSnsCount);
//    Serial.print("\n\n");
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

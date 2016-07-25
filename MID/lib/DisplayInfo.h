//
// Created by Angel Zaprianov on 27.6.2016 г..
//

#ifndef ARDUINOMID_READINNTERTEMP_H
#define ARDUINOMID_READINNTERTEMP_H

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
    if (isSensorReadAllow()) {

        //temperatureC = (pinReadValue * 100.0) / 1024.0;
        //temperatureC =  (200*pin_value)>>8;
        temperatureC = (225 * pinReadValue) >> 8;
        // converting that reading to voltage, for 3.3v arduino use 3.3
        int voltage = pinReadValue * 5.0;
        //voltage /= 1024.0;

        // now print out the temperature
        // int temperatureC = (voltage - 0.5) * 100 ;

        lcd.setCursor(8, 2);
        if (temperatureC > 0) {
            lcd.print("  ");

        }
        lcd.print(temperatureC);
        lcd.print((char) 1);
    }

}
/**
 * Display engine RPMs
 */
void displayEngRPM() {
    unsigned int rpmSnsCount = 0;

    //
    // Gets RPM
    rpmSnsCount = getDigitalTachometerRpm();

    lcd.setCursor(0, 2);
    lcd.print("RPM:");
    //
    // Handle screen display
    if (rpmSnsCount == 0) {
        lcd.print(000);
    } else if (rpmSnsCount < 1000 && rpmSnsCount >= 1) {
        lcd.print(0);
    }
    lcd.print(rpmSnsCount, DEC);
}
/**
 * Display engine KMh
 */
void displayCarKMH() {
    unsigned int kmhSnsCount = 0;

    //
    // Gets RPM
    kmhSnsCount = getDigitalSpeedKmh();

    lcd.setCursor(0,0);
    lcd.print("KMh:");
    //
    // Handle screen display
     if (kmhSnsCount < 1000 ) {
        lcd.print(0);
    }
    lcd.print(kmhSnsCount, DEC);
}




#endif //ARDUINOMID_READINNTERTEMP_H

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



/**
 * Display engine RPMs
 */
void displayEngRPM() {
    char rpmDisplay[4];
    //
    // Gets RPM
    int rpmSnsCount = getRpmSens();

    lcd.setCursor(0, 2);
    lcd.print("RPM:");

    if (isSensorReadAllow()) {
        lcd.print("      ");
        lcd.setCursor(0, 2);
        lcd.print("RPM:");
    }
    //
    // Handle screen display
//    int rpm1 = rpmSnsCount / 1000;
//    int rpm2 = rpmSnsCount / 100;
//    int rpm3 = rpm2 - (rpm1 * 10);
//    int rpm4 = (rpmSnsCount / 10) - (rpm2 * 10);
//
//    rpmDisplay[0] = '0' + rpm1;
//    rpmDisplay[1] = '0' + rpm3;
//    rpmDisplay[2] = '0' + rpm4;
//    rpmDisplay[3] = '0' + (rpmSnsCount % 10);

  //
  // Handle RPM screen print
  sprintf(rpmDisplay,"%04d", rpmSnsCount);
  lcd.print(rpmDisplay);

}


/**
 * Display engine KMh
 */
void displayCarKMH() {
    char vssDisplay[3];
    //
    // Gets KmH
    int vssSnsCount = getVssSens();

    lcd.setCursor(0, 0);
    lcd.print("KMh:");

  if (isSensorReadAllow()) {
      lcd.print("    ");
      lcd.setCursor(0, 0);
      lcd.print("KMh:");
    }

    //
    // Handle VSS screen print
    sprintf(vssDisplay,"%03d", vssSnsCount);
    lcd.print(vssDisplay);

}

/**
 * Display engine KMh
 */
void displayCarECU() {
    char ecuDisplay[2];

    //
    // Gets RPM
    int counterEcu = getEcuSens();

    lcd.setCursor(8, 0);
    lcd.print("ECU:");


    if (isSensorReadAllow()) {
        lcd.print("   ");
        lcd.setCursor(8, 0);
        lcd.print("ECU:");
    }
    //
    // Handle ECU screen print
    sprintf(ecuDisplay,"%02d", counterEcu);
    lcd.print(ecuDisplay);
}


#endif //ARDUINOMID_READINNTERTEMP_H

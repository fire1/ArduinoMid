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
//
//
/****************************************************************
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
//    if (isSensorReadMid()) {
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

    lcd.setCursor(8, 0);
    lcd.print("ECU:");

    if (isSensorReadMid()) {
        lcd.print("   ");
        lcd.setCursor(8, 0);
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
	  int tmpTime = getTravelTime() / 1000;

	  int a = tmpDistance / 10;
	  int b = tmpDistance % 10;

	  String printDistance = String(a + "." + b);

    if (isSensorReadLow()) {
		//
		// Display travel distance
		lcd.setCursor(0, 0);
		lcd.print("Dist:");
		lcd.setCursor(6, 0);
        lcd.print(printDistance);
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

    char LitersIns[2];
    char LitersAvr[2];

    if (isSensorReadMid()) {
        lcd.setCursor(0, 2);
        lcd.print((char) 4);
        lcd.print(" Ins:");
        lcd.print("  ");

        lcd.setCursor(8, 2);
        lcd.print("Avr:");
        lcd.print("  ");

    }
    sprintf(LitersIns, "%02d", 00);
    sprintf(LitersAvr, "%02d", 00);

    //
    // Handle Distance screen

    lcd.setCursor(0, 2);
    lcd.print((char) 4);
    lcd.print(" Ins:");
    lcd.print(LitersIns);

    lcd.setCursor(8, 2);
    lcd.print("Avr:");
    lcd.print(LitersAvr);
}
/****************************************************************
 * Average
 */
void displayAverage(){

  String averageSpeed = String(getAverageVss() + "kmh");
  String averageEngine = String(getAverageRpm() + "rpm");

  if (isSensorReadLow()) {
      lcd.setCursor(0, 0);
      lcd.print(" SPD:");
      lcd.print(averageSpeed);

      lcd.setCursor(0, 2);
      lcd.print(" RPM");
      lcd.print(averageEngine);
    }
}

#endif //ARDUINOMID_READINNTERTEMP_H

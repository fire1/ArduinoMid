//
// Created by Angel Zaprianov on 27.6.2016 г..
//

#ifndef ARDUINOMID_READINNTERTEMP_H
#define ARDUINOMID_READINNTERTEMP_H

#include <LiquidCrystal.h>
#include "MainFunc.h"

unsigned int ecuSnsCount = 0;

/**
 * Read temperature sensor
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

int long lastEcuRead = 0;

void getEcuSignalAmplitude() {

    if (millis() >= lastEcuRead + 6000) {
        lastEcuRead = millis();
        ecuSnsCount = 0;
    }

    ecuSnsCount = getSensorAmplitudeRead(ECU_SGN_PIN, ecuSnsCount);
    lcd.setCursor(0, 2);
    lcd.print("ECU:");
    lcd.print(ecuSnsCount);

}


// Program to count pulses per revolution in an automobile
// TODO: Includes a running average to insure proper RPM output
// TODO: Output a clean averaged 5v Sq wave of RPM
// NOTE: May need to go to (us) as opposed to (ms) for better resolution

int Cycle = 0;                  // set to 0 for PulseStartTime and set to
//   1 for PulseEndTime
unsigned long PulseStartTime;   // Saves Start of pulse in ms
unsigned long PulseEndTime;     // Saves End of pulse in ms
unsigned long PulseTime;        // Stores dif between start and stop of pulse
unsigned long RPM = 0;          // RPM to ouptut (30*1000/PulseTime)

void setup()
{
    Serial.begin(9600);            // OPENS SERIAL PORT SETS DATA TO 9600 bps
    attachInterrupt(0, RPMPulse, RISING); // Attaches interrupt to Digi Pin 2
}

void RPMPulse()
{
    if (Cycle == 0)                // Check to see if start pulse
    {
        PulseStartTime = millis();  // stores start time
        CycleOnOrOff = 1;           // sets counter for start of pulse
        return;                     // a return so it doesnt run the next if
    }
    if (Cycle == 1)             // Check to see if end pulse
    {
        detachInterrupt(0);         // Turns off inturrupt for calculations
        PulseEndTime = millis();    // stores end time
        Cycle = 0;                  // resets counter for pulse cycle
        calcRPM();                  // call to calculate pulse time
    }
}

void calcRPM()
{
    PulseTime = PulseEndTime - PulseStartTime; // Gets pulse duration
    Serial.print("PulseTime =");               // Output pulse time for debug
    Serial.print(PulseTime);                   // Pulse debug output
    Serial.print(" ");
    RPM = 30*1000/PulseTime*2;                 // Calculates RPM
    attachInterrupt(0, RPMPulse, RISING);      // re-attaches interrupt to Digi Pin 2
}

void loop()
{
    Serial.print("RPM = ");      // Output RPM for debug
    Serial.print(int(RPM));      // RPM debug output
    Serial.print(" ");
    delay(1000);                  // 1 sec delay for debug output
}

#endif //ARDUINOMID_READINNTERTEMP_H

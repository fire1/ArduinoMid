//
// Created by Angel Zaprianov on 9/17/2016.
//
#include <OneWire.h>
#include <DallasTemperature.h>


#ifndef ARDUINOMID_TMPSENS_H
#define ARDUINOMID_TMPSENS_H

#define  DEBUG_TEMPERATURE_OU
#define  DEBUG_TEMPERATURE_IN

// Data wire is plugged into pin A7 on the Arduino
#define ONE_WIRE_BUS A7

OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature temperatureSensors(&oneWire);


bool isInitTemperature = 1;

float CUR_OUT_TMP = 0;

float getTmpOut() {
    return CUR_OUT_TMP;
}


void setupTemperature() {
    temperatureSensors.begin();
}

/**
 * Temperature sensor
 */
void sensTmp() {

#if defined(DEBUG_TEMPERATURE_IN)
    if (ampInt.isMid()) {
        Serial.print("Dallas temperature: \n");
        Serial.println(temperatureSensors.getTempCByIndex(0)); // Why "byIndex"?
    }
#endif

    float temperatureC;
    //
    // 141 = 22* ??
    // 120 = 28*
    // 119 = 29*
    // 116 = 33* ???
    // 180 = 16*
    // 197 = 14*
    // 202 = 13*
    //~~~~~~~~~~~~~~~~
    // 131 = 27.00
    //
    //
    // New measurement
    // --------------------------------------------
    // 147 = 21*
    // 140/139 = 22 / 21
    // 139 =  25/6
    // 118 = 27/8
    if (isInitTemperature || ampInt.isBig()) {
        //
        // Read new data
        int reading = analogRead(TMP_PIN_OUT);

        //      255 max reading
        //      4.34 is voltage passes temperature sensor
        float cofVolt = 4.34;

        // not correct
        /// new type  id: (147 / 2.666666 - 76) *1
        //
        // separate reading
        temperatureC = ((reading / cofVolt) - (255 / cofVolt)) * -1;

#if defined(DEBUG_TEMPERATURE_OU)
        if (ampInt.isMid()) {
            Serial.print("Read Temp: ");
            Serial.println(reading);
        }
#endif

        //
        // Close first loop
        isInitTemperature = 0;

        //
        // Pass value to global
        CUR_OUT_TMP = temperatureC;
    }

}

#endif //ARDUINOMID_TMPSENS_H

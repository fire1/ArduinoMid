//
// Created by Angel Zaprianov on 9/17/2016.
//

#ifndef ARDUINOMID_TMPSENS_H
#define ARDUINOMID_TMPSENS_H


const bool DebugTemperatures = 0;

float CUR_OUT_TMP = 0;

int getTmpOut() {
    return (int) CUR_OUT_TMP;
}

/**
 * Temperature sensor
 */
void sensTmp() {

    float temperatureC;
    if (isSensorReadMid()) {
        int reading = analogRead(TMP_PIN_OUT);


        // converting that reading to voltage, for 3.3v Astra use  resistor for ~ 2.3v
        float voltage = reading * 2.3;
        voltage /= 1024.0;

        //converting from 10 mv per degree wit 500 mV offset
        temperatureC = (((voltage /*- 0.5*/) * 100) - 55) * -1;
        //to degrees ((voltage - 500mV) times 100)

        if (DebugTemperatures) {

            Serial.print("Read Temp: ");
            Serial.print(reading);
            Serial.print("\t");
            Serial.print("Voltage Temp: ");
            Serial.print(voltage);
            Serial.print("\t");
            Serial.print("Resolve Temp: ");
            Serial.print(temperatureC);
            Serial.print("\n");
        }

        CUR_OUT_TMP = temperatureC;
    }

}


#endif //ARDUINOMID_TMPSENS_H

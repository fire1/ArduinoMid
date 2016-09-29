//
// Created by Angel Zaprianov on 9/17/2016.
//

#ifndef ARDUINOMID_TMPSENS_H
#define ARDUINOMID_TMPSENS_H


const bool DebugTemperatures = 0;

float CUR_OUT_TMP = 0;

float getTmpOut() {
    return CUR_OUT_TMP;
}

/**
 * Temperature sensor
 */
void sensTmp() {

    float temperatureC;
    if (isSensorReadLow()) {
        //
        // 141 = 22*
        // 88
        int reading = analogRead(TMP_PIN_OUT);

        if (DebugTemperatures) {

            Serial.print("Read Temp: ");
            Serial.print(reading);
            Serial.print("\n");
        }

        Serial.print("Read Temp: ");
        Serial.println(reading);
        float voltage = reading  /*3.3*/; // Maybe readings needs to be zeroed in order to lower the values
        voltage /= 1024.0;

        if (DebugTemperatures) {

            Serial.print("Read Temp: ");
            Serial.print(reading);
            Serial.print("Read Volts: ");
            Serial.print(voltage);
            Serial.print("\n");
        }
        CUR_OUT_TMP = reading - 121;
//        CUR_OUT_TMP = (((voltage /*- 0.5*/) * 100) /*- 68*/) * -1;
    }

}
//

//
//        //
//        // converting that reading to voltage, for 3.3v Astra use  resistor for ~ 2.3v
//        float voltage = reading * 3.3; // Maybe readings needs to be zeroed in order to lower the values
//        voltage /= 1024.0;
//
//        //converting from 10 mv per degree wit 500 mV offset
//        temperatureC = (((voltage /*- 0.5*/) * 100) - 80) * -1;
//        //to degrees ((voltage - 500mV) times 100)

#endif //ARDUINOMID_TMPSENS_H

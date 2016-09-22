//
// Created by Angel Zaprianov on 9/17/2016.
//

#ifndef ARDUINOMID_TMPSENS_H
#define ARDUINOMID_TMPSENS_H


float CUR_OUT_TMP = 0;

int getTmpOut() {
    return (int) CUR_OUT_TMP;
}

/**
 * Temperature sensor
 */
void sensTmp() {

    float temperatureC;
    if (isSensorReadLow()) {
        int reading = analogRead(TMP_PIN_OUT);

//
//        Serial.print("RealTempSens: ");
//        Serial.printnl(reading);

        // converting that reading to voltage, for 3.3v Astra use  resistor for ~ 2.3v
        float voltage = reading * 2.3;
        voltage /= 1024.0;

        // now print out the temperature
        temperatureC = ((voltage /*- 0.5*/) * 100) /2;  //converting from 10 mv per degree wit 500 mV offset
        //to degrees ((voltage - 500mV) times 100)

        CUR_OUT_TMP = temperatureC;
    }

}


#endif //ARDUINOMID_TMPSENS_H

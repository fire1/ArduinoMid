//
// Created by Angel Zaprianov on 9/17/2016.
//

#ifndef ARDUINOMID_TMPSENS_H
#define ARDUINOMID_TMPSENS_H


const bool DebugTemperatures = 1;
unsigned long tempCounter = 0;

float CUR_OUT_TMP = 0;

float getTmpOut() {
    return CUR_OUT_TMP / tempCounter;
}


/**
 * Temperature sensor
 */
void sensTmp() {

    float temperatureC;
    //
    // 141 = 22*
    // 120 = 28*
    // 119 = 29*
    // 116 = 33*
    // 197 = 19*
    //
    int reading = analogRead(TMP_PIN_OUT);


//    temperatureC = ((((reading*1.5) /1024) *100) -50 ) *-1;
    temperatureC = ((reading / 1.8) - 100) * -1;

    if (DebugTemperatures && ampInt.isMid()) {
        Serial.print("Read Temp: ");
        Serial.println(reading);
    }


    tempCounter++;
    CUR_OUT_TMP += temperatureC;
//    CUR_OUT_TMP += (reading - 150) * -1;
//        CUR_OUT_TMP = (((voltage /*- 0.5*/) * 100) /*- 68*/) * -1;


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

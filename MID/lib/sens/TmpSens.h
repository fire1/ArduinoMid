//
// Created by Angel Zaprianov on 9/17/2016.
//

#ifndef ARDUINOMID_TMPSENS_H
#define ARDUINOMID_TMPSENS_H


const bool DebugTemperatures = 1;
unsigned long tempCounter = 0;

float CUR_OUT_TMP = 0;

float getTmpOut() {
    return CUR_OUT_TMP;
}


/**
 * Temperature sensor
 */
void sensTmp() {

    float temperatureC;
    //
    // 141 = 22* ??
    // 120 = 28*
    // 119 = 29*
    // 116 = 33* ???
    // 180 = 16*
    // 197 = 14*
    // 202 = 13*
    //
    int reading = analogRead(TMP_PIN_OUT);


    //
    //      255 max reading
    //      4.34 is voltage passes temperature sensor
    float cofVolt = 4.3;

    //
    // separate reading
    temperatureC = ((reading / cofVolt) - (255 / cofVolt)) * -1;

    if (DebugTemperatures && ampInt.isMid()) {
        Serial.print("Read Temp: ");
        Serial.println(reading);
    }


//    tempCounter++;
    CUR_OUT_TMP = temperatureC;

}

#endif //ARDUINOMID_TMPSENS_H

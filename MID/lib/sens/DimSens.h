//
// Created by Angel Zaprianov on 5.8.2016 г..
//


#ifndef ARDUINOMID_DimSens_H
#define ARDUINOMID_DimSens_H


//
// Set up pins
//
// vars for resolve dim value
int  backLightDefault = 22;       // value to dim display when car lights are off
int  backLightLevel = 0;  // resolved display dim

const int dimCorrection = 45;

static void sensDim(void);

/**
 * Setup pins of display dim
 */
void setupBackLight(int pinInputInstrumentValue, int pinOutputDisplayContrast) {
    pinMode(pinInputInstrumentValue, INPUT);
    pinMode(pinOutputDisplayContrast, OUTPUT);

    analogWrite(pinOutputDisplayContrast, backLightDefault);
    //
    // Sens dim level at setup
    sensDim();

}


//const int numReadingsDim = 100;
//int indexReadValDim = 0;
//int lastReadingsDim[numReadingsDim];
//int totalReadingDim = 0;
/**
 * Handle display dim
 */
int long lastReadValueDim = 0;
/**
 * Detection of back-light
 */
static void sensDim(void) {

    int defaultActive = 0;
    int dimReadVal = analogRead(DIM_PIN_VAL);

    backLightLevel = (int)map(dimReadVal, 0, 1023, 0, 255);

    if (backLightLevel < 25) {
        backLightLevel = backLightDefault;
        defaultActive = 1;
    } else {
        defaultActive = 0;
    }

    if (lastReadValueDim != backLightLevel  && backLightLevel > 0) {
        lastReadValueDim = backLightLevel;

        if (defaultActive == 0) {
            backLightLevel = backLightLevel - dimCorrection;
        }
        analogWrite(DIM_PIN_OUT, backLightLevel);
    }
}



#endif //ARDUINOMID_DimSens_H

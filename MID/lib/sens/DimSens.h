//
// Created by Angel Zaprianov on 5.8.2016 г..
//


#ifndef ARDUINOMID_DimSens_H
#define ARDUINOMID_DimSens_H


//
// Set up pins
//
// vars for resolve dim value
int backLightDefault = 16;       // value to dim display when car lights are off
int isGaugesActive = LOW;         // is car lights on
int unsigned backLightLevel = 0;  // resolved display dim

const int dimCorrection = 45;

static void handleBackLight(void);

/**
 * Setup pins of display dim
 */
void setupBackLight(int pinInputInstrumentValue, int pinOutputDisplayContrast) {
    pinMode(pinInputInstrumentValue, INPUT);
    pinMode(pinOutputDisplayContrast, OUTPUT);

    analogWrite(pinOutputDisplayContrast, backLightDefault);
    handleBackLight();
}

/**
 * Handle display dim
 */
const int numReadingsDim = 100;
int indexReadValDim = 0;
int lastReadingsDim[numReadingsDim];
int totalReadingDim = 0;
int lastReadValueDim = 0;

static void handleBackLight(void) {

    int defaultActive = 0;
    int dimReadVal = analogRead(DIM_PIN_VAL);

    backLightLevel = map(dimReadVal, 0, 1023, 0, 255);

    if (backLightLevel < 25) {
        backLightLevel = backLightDefault;
        defaultActive = 1;
    } else {
        defaultActive = 0;
        backLightLevel = backLightLevel;
    }

    if (lastReadValueDim != backLightLevel) {
        lastReadValueDim = backLightLevel;

        if (defaultActive == 0) {
            backLightLevel = backLightLevel - dimCorrection;
        }
        analogWrite(DIM_PIN_OUT, backLightLevel);
    }
}


#endif //ARDUINOMID_DimSens_H

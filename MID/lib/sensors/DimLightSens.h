//
// Created by Angel Zaprianov on 5.8.2016 г..
//

#ifndef ARDUINOMID_DIMLIGHTSENS_H
#define ARDUINOMID_DIMLIGHTSENS_H


//
// Set up pins
//
// vars for resolve dim value
int backLightDefault = 16;       // value to dim display when car lights are off
int isGaugesActive = LOW;         // is car lights on
int unsigned backLightLevel = 0;  // resolved display dim



static void handleBackLight(void);

/**
 * Setup pins of display dim
 */
void setupBackLight(void) {
    pinMode(DIM_PIN_VAL, INPUT);
    pinMode(DIM_PIN_OUT, OUTPUT);

    analogWrite(DIM_PIN_OUT, backLightDefault);
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


    int dimReadVal = analogRead(DIM_PIN_VAL);

    backLightLevel = map(dimReadVal, 0, 1023, 0, 255);
    totalReadingDim = totalReadingDim - lastReadingsDim[indexReadValDim];
    lastReadingsDim[indexReadValDim] = dimReadVal;
    totalReadingDim = totalReadingDim + lastReadingsDim[indexReadValDim];

    //
    // Set backLight
    // backLightLevel(analogRead) / 4 for analogWrite




    indexReadValDim = indexReadValDim + 1;
    // if we're at the end of the array...
    if (indexReadValDim >= numReadingsDim) {
        // ...wrap around to the beginning:
        indexReadValDim = 0;
    }

//    backLightDefault = totalReadingDim / indexReadValDim;


    //
    //



    if (lastReadValueDim != backLightLevel) {
//        Serial.println(backLightLevel);
    }

    if (backLightLevel < 15) {
        backLightLevel = backLightDefault;
    }

    if (lastReadValueDim != backLightLevel && backLightLevel > 0) {
        lastReadValueDim = backLightLevel;
        analogWrite(DIM_PIN_OUT, backLightLevel);
    }
}


#endif //ARDUINOMID_DIMLIGHTSENS_H

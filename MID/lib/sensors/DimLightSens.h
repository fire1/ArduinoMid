//
// Created by Angel Zaprianov on 5.8.2016 г..
//

#ifndef ARDUINOMID_DIMLIGHTSENS_H
#define ARDUINOMID_DIMLIGHTSENS_H


//
// Set up pins
//
// vars for resolve dim value
int backLightDefault = 26;       // value to dim display when car lights are off
int isGaugesActive = LOW;         // is car lights on
int unsigned backLightLevel = 0;  // resolved display dim



static void handleBackLight(void);

/**
 * Setup pins of display dim
 */
void setupBackLight(void) {
    pinMode(DIM_PIN_VAL, INPUT);
    pinMode(DIM_PIN_OUT, OUTPUT);


    handleBackLight();
}

/**
 * Handle display dim
 */

int lastReadTimeDim = 0;


static void handleBackLight(void) {


    int dimReadVal = analogRead(DIM_PIN_VAL);
    backLightLevel = map(dimReadVal, 0, 1023, 0, 255);
    //
    //
    if (backLightLevel < 10) {
        backLightLevel = backLightDefault;
    }
    //
    // Set backLight
    // backLightLevel(analogRead) / 4 for analogWrite


    Serial.print(dimReadVal);
    Serial.print("\n");


    if (lastReadTimeDim != backLightLevel) {
        analogWrite(DIM_PIN_OUT, backLightLevel);
    }
}


#endif //ARDUINOMID_DIMLIGHTSENS_H

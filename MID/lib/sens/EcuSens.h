//
// Created by Angel Zaprianov on 25.7.2016 г..
//

#ifndef ARDUINOMID_EcuSens_H
#define ARDUINOMID_EcuSens_H


const bool EcuSensDebug = 1;

int ecuHitsCount = 0;
int ecuCycles = 0;
int long ecuTimerStart = 0, ecuTimerEnds = 0;
int ecuTimeDif = 0;
int ecuTimeHits = 0;

void catchEcuHits() {
    ecuHitsCount++;
    ecuTimeHits = millis();
}

void setupEcuSens(int pinTarget){
    //
    // Pin mode ...
    pinMode(pinTarget, INPUT_PULLUP);
    //
    // Set as interrupt pin
    attachInterrupt(digitalPinToInterrupt(pinTarget), catchEcuHits, FALLING );
}

/**
 * Working version
 */
static int getEcuSens() {

    //
    //
    ecuTimerEnds = millis();
    if (ecuTimerEnds >= (ecuTimerStart + 150)) {
        ecuTimerStart = ecuTimerEnds;
        ecuCycles = ecuHitsCount;
        ecuTimeDif = millis() - ecuTimeHits;
        //
        // debug info
        if (EcuSensDebug){
            Serial.print ("\n");
            Serial.print (" ecu diff:  \t");
            Serial.print (ecuTimeDif);
            Serial.print (" ecu is:  \t");
            Serial.print (ecuCycles * 200);
            Serial.print (" ecu count:  \t");
            Serial.print (ecuHitsCount);
            Serial.print ("\n");
        }

        //ecuTimeHits = 0;
        ecuHitsCount = 0;
        }
    return ecuCycles * 200;
}

#endif //ARDUINOMID_EcuSens_H

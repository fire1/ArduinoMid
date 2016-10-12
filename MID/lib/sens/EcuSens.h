//
// Created by Angel Zaprianov on 27.6.2016 г..
//

#ifndef ARDUINOMID_EcuSens_H
#define ARDUINOMID_EcuSens_H

//
// Sensor configs
const bool EcuSensDebug = 0;
const float EcuCorrection = 1.2;
//
// Rpm Container
int CUR_ECU = 0;
//
// Working vars
int ecuHitsCount = 0;
int long ecuTimerStart = 0, ecuTimerEnds = 0;
int ecuTimeHits = 0;
int long ecuWaveTime1, ecuWaveTime2 = 0;
int long CUR_IPT = 0; // Injector pulse time
/**
 * Callback attachInterrupt
 */
void catchEcuHits() {
    ecuWaveTime2 = micros();
    ecuHitsCount++;
    CUR_IPT = CUR_IPT + (ecuWaveTime2 - ecuWaveTime1);
    ecuWaveTime1 = micros();
}

/**
  * Setup Ecu
 */
void setupEcuSens(int pinTarget) {
    pinMode(pinTarget, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pinTarget), catchEcuHits, FALLING);
}

/**
 * Gets current Ecu
 */
int getEcuSens() {
    return CUR_ECU;
}

/**
 * Detect Ecu
 */
void sensEcu() {

    ecuTimerEnds = millis();
    if (ecuTimerEnds >= (ecuTimerStart + 150)) {
        //
        // Handle cycles
        ecuTimerStart = ecuTimerEnds;
        //
        // Pass ecu to global
        CUR_ECU = int(ecuHitsCount * EcuCorrection);
        //
        // debug info
        if (EcuSensDebug) {
            Serial.print("\n");
            Serial.print(" ecu count:  \t");
            Serial.print(ecuHitsCount);
            Serial.print(" ecu is:  \t");
            Serial.print(ecuHitsCount * 1.6);
            Serial.print("\n");
        }

        ecuHitsCount = 0;
    }

}

#endif //ARDUINOMID_EcuSens_H

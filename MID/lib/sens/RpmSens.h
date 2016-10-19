//
// Created by Admin on 7/23/2016.
//

#ifndef ARDUINOMID_RpmSens_H
#define ARDUINOMID_RpmSens_H

//
// Sensor configs
//#define RPM_SENS_DEBUG
const float RpmCorrection = 32.8 /*32.4*/;
//45
const int rpmLoopTime = 200;
//
// Working vars
int rpmHitsCount = 0;
int rpmCycles = 0;
int long rpmTimerStart = 0, rpmTimerEnds = 0;

/**
 * Callback attachInterrupt
 */
void catchRpmHits() {
    rpmHitsCount++;
}


void setupRpmSens(uint8_t pinTarget) {
    pinMode(pinTarget, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt (pinTarget), catchRpmHits, FALLING);
}

/**
 * Gets current RPM
 */
int getRpmSens() {
    return CUR_RPM;
}


/**
 * Detect RPMs
 */
void sensRpm() {
//    rpmTimerEnds = millis();
//    if (rpmTimerEnds >= (rpmTimerStart + rpmLoopTime)) {
    if (ampInt.isSens()) {
        //
        // Handle cycles
        rpmTimerStart = rpmTimerEnds;
        //
        // Pass rpm to global
        CUR_RPM = int(rpmHitsCount * RpmCorrection);

        //
        // debug info
#if defined(RPM_SENS_DEBUG) || defined(GLOBAL_SENS_DEBUG)
        Serial.print("\n");
        Serial.print(" RPM count:  \t");
        Serial.print(rpmHitsCount);
        Serial.print(" RPM is:  \t");
        Serial.print(rpmHitsCount * RpmCorrection);
        Serial.print("\n");
#endif
        //
        // Clear value
        rpmHitsCount = 0;
    }
}


#endif //ARDUINOMID_RpmSens_H


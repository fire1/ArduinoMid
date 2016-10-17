//
// Created by Angel Zaprianov on 27.6.2016 г..
//

#ifndef ARDUINOMID_VssSens_H
#define ARDUINOMID_VssSens_H

#include <USBAPI.h>

unsigned long testVarVtd = 0;
//
// Sensor configs
//#define VSS_SENS_DEBUG

const bool VssSAlarmSpeed = 1;
const int VssAlarmCitySpeed = 63;
//
// Correction of VSS
const float VssCorrection = 3.4; // One mile 1.621371192 [changed from int to float]
//const double VssCorrection = 1.621371192; // One mile 1.621371192
const int VssLoopLength = 200;
//
// Vehicle Speed sensor Container
int CUR_VSS = 0;
//
// Vehicle Distance sensor Container
double CUR_DST = 0;
//
// Working vars
int vssHitsCount = 0;
int long vssTimerStart = 0, vssTimerEnds = 0;
int vssTimeHits = 0;


/**
 * Callback attachInterrupt
 */
void catchVssHits() {
    vssHitsCount++;
}

/**
  * Setup Vss
 */
void setupVssSens(uint8_t pinTarget) {
    pinMode(pinTarget, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt (pinTarget), catchVssHits, FALLING);
}

/**
 * Gets current Vss
 */
int getVssSens() {
    return CUR_VSS;
}

float getDstSens() {
    return CUR_DST / 1000;
}

/**
 * Detect Vss
 */
void sensVss() {

//    vssTimerEnds = millis();
//    if (vssTimerEnds >= (vssTimerStart + VssLoopLength)) {
    if (ampInt.isSens()) {
        //
        // Handle cycles
        vssTimerStart = vssTimerEnds;
        //
        // Pass vss to global
        CUR_VSS = int(vssHitsCount / VssCorrection);
        CUR_DST = CUR_DST + ((CUR_VSS / MILLIS_PER_HR) * VssLoopLength);
        testVarVtd++;
//
// debug info
#if defined(VSS_SENS_DEBUG) || defined(GLOBAL_SENS_DEBUG)
        Serial.print("\n");
        Serial.print(" vss count:  \t");
        Serial.print(vssHitsCount);
        Serial.print(" vss is:  \t");
        Serial.print(vssHitsCount * VssCorrection);
        Serial.print("\n");
#endif

        //
        // Reset pulse counter
        vssHitsCount = 0;
    }

    //
    // Alarm speeding at city
    if (ampInt.isSec() && CUR_VSS > VssAlarmCitySpeed) {
        tone(ADT_ALR_PIN, 4000, 500);
    }

//    if (ampInt.isSec()) {
//        Serial.print("Vss count: \n");
//        Serial.println(vssHitsCount);
//    }

}

#endif //ARDUINOMID_VssSens_H

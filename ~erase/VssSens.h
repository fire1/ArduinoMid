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
//
// 3.357 - two high
const float VssCorrection = 3.2; // One mile 1.621371192 [changed from int to float]
//const double VssCorrection = 1.621371192; // One mile 1.621371192
const int VssLoopLength = 200;
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

int long getDstSens() {
    return CUR_VDS /* real value is divided 1000, be more precise */;
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
        CUR_VDS = vssHitsCount + CUR_VDS;
//        CUR_VDS = CUR_VDS + ((CUR_VSS / MILLIS_PER_HR) * VssLoopLength);

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

    if (ampInt.isBig()) {
        Serial.print("Counted VSD is: ");
        Serial.println(CUR_VDS);
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

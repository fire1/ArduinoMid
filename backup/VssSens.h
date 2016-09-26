//
// Created by Angel Zaprianov on 27.6.2016 г..
//

#ifndef ARDUINOMID_VssSens_H
#define ARDUINOMID_VssSens_H

#include <USBAPI.h>

//
// Sensor configs
const bool VssSensDebug = 0;
const bool VssSAlarmSpeed = 1;
const int VssAlarmCitySpeed = 60;
//
// Correction of VSS
const float VssCorrection = 1; // One mile 1.621371192 [changed from int to float]
//const double VssCorrection = 1.621371192; // One mile 1.621371192
const int VssLoopLength = 250;
//
// Rpm Container
int CUR_VSS = 0;
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
void setupVssSens(int pinTarget) {
    pinMode(pinTarget, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt (pinTarget), catchVssHits, FALLING);
}

/**
 * Gets current Vss
 */
int getVssSens() {
    return CUR_VSS;
}

/**
 * Detect Vss
 */
void sensVss() {

    vssTimerEnds = millis();
    if (vssTimerEnds >= (vssTimerStart + VssLoopLength)) {
        //
        // Handle cycles
        vssTimerStart = vssTimerEnds;
        //
        // Pass vss to global
        CUR_VSS = int(vssHitsCount * VssCorrection);
        //
        // debug info
        if (VssSensDebug) {
            Serial.print("\n");
            Serial.print(" vss count:  \t");
            Serial.print(vssHitsCount);
            Serial.print(" vss is:  \t");
            Serial.print(vssHitsCount * VssCorrection);
            Serial.print("\n");
        }

        //
        // Alarm speeding at city
        if (isSensorReadMid() && VssSAlarmSpeed && CUR_VSS > VssAlarmCitySpeed) {
            tone(ADT_ALR_PIN, 4000, 1000);
        }

        //
        // Reset pulse counter
        vssHitsCount = 0;
    }

}

#endif //ARDUINOMID_VssSens_H

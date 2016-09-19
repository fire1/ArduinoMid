//
// Created by Angel Zaprianov on 27.6.2016 г..
//

#ifndef ARDUINOMID_VssSens_H
#define ARDUINOMID_VssSens_H

#include <USBAPI.h>

//
// Sensor configs
const bool VssSensDebug = 0;

const int VssCorrection = 1.7; // One mile * 10 000
const int VssLoopLength = 250;
//
// Rpm Container
int CUR_VSS = 0;
int long CUR_VDT = 0;
int long CUR_VTT = 0; //travel time
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
        // Check is vehicle moving
        if (CUR_VSS > 0) {

            //
            // Travel distance detection
            CUR_VDT = CUR_VDT + vssHitsCount;
            //
            // Vehicle Time Travel detection
            CUR_VTT = VssLoopLength + CUR_VTT;
        }
        //
        // Reset pulse counter
        vssHitsCount = 0;
    }

}

#endif //ARDUINOMID_VssSens_H

//
// Created by Admin on 7/23/2016.
//

#define RpmSensDebug = true;

#ifndef ARDUINOMID_RpmSens_H
#define ARDUINOMID_RpmSens_H


int rpmHitsCount = 0;
int rpmCycles = 0;
int rpmTimerStart = 0, rpmTimerEnds = 0;
int rpmTimeDif = 0;
int rpmTimeHits = 0;

void catchRpmHits() {
    rpmHitsCount++;
    rpmTimeHits = millis();
}

void setupRpmSens(int pinTarget){
  //
  // Pin mode ...
  pinMode(pinTarget, INPUT_PULLUP);
  //
  // Set as interrupt pin
  attachInterrupt(digitalPinToInterrupt(pinTarget), catchRpmHits, FALLING );
}

/**
 * Working version
 */
static int getRpmSens() {

    rpmTimerEnds = millis();
    if (rpmTimerEnds >= (rpmTimerStart + 150)) {
        rpmTimerStart = rpmTimerEnds;
        rpmCycles = rpmHitsCount;
        rpmTimeDif = millis() - rpmTimeHits;
        //
        // debug info
//      #ifndef RpmSensDebug
//          Serial.print("\n");
//          Serial.print(" RPM diff:  \t");
//          Serial.print(rpmTimeDif);
//          Serial.print(" RPM is:  \t");
//          Serial.print(rpmCycles * 200);
//          Serial.print(" RPM count:  \t");
//          Serial.print(rpmHitsCount);
//          Serial.print("\n");
//      #endif

        //rpmTimeHits = 0;
        rpmHitsCount = 0;
    }

    return rpmCycles * 200;

}

#endif //ARDUINOMID_RpmSens_H

//
// OLD code
//    rpmTimerEnds = millis();
//    if (rpmTimerEnds >= (rpmTimerStart + 1000)) {
//        rpmCycles = rpmHitsCount;
//        rpmHitsCount = 0;
//        rpmTimerStart = rpmTimerEnds;
//    }
//
//
//    if (digitalRead(RPM_SNS_PIN) == HIGH) {
//        if (!TachometerCounted) {
//            TachometerCounted = true;
//            rpmHitsCount++;
//
//        }
//    } else {
//        TachometerCounted = false;
//    }

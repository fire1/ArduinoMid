//
// Created by Admin on 7/23/2016.
//



#ifndef ARDUINOMID_RpmSens_H
#define ARDUINOMID_RpmSens_H

const bool RpmSensDebug = 1;


int rpmHitsCount = 0;
int rpmCycles = 0;
int long rpmTimerStart = 0, rpmTimerEnds = 0;
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
      if(RpmSensDebug)
        {
          Serial.print ("\n");
          Serial.print (" RPM diff:  \t");
          Serial.print (rpmTimeDif);
          Serial.print (" RPM is:  \t");
          Serial.print (rpmCycles * 200);
          Serial.print (" RPM count:  \t");
          Serial.print (rpmHitsCount);
          Serial.print ("\n");
        }

        //rpmTimeHits = 0;
        rpmHitsCount = 0;
    }

    return rpmCycles * 200;

}

#endif //ARDUINOMID_RpmSens_H


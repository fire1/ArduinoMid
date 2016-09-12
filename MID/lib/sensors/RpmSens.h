//
// Created by Admin on 7/23/2016.
//

#ifndef ARDUINOMID_RpmSens_H
#define ARDUINOMID_RpmSens_H

#define RpmSensDebug

unsigned long TachometerTimeOld;

int TachometerHits = 0;
bool TachometerCounted = false;
int TachometerRps = 0;
int TachometerTimerStart = 0, TachometerTimerEnds = 0;
int rpmTimeRpmDif = 0;
int timeRpmHits = 0;

void catchRpmHits ()
{
  TachometerHits++;
  timeRpmHits = millis ();
}

/**
 * Working version
 */
static int getRpmSens ()
{

//    TachometerTimerEnds = millis();
//    if (TachometerTimerEnds >= (TachometerTimerStart + 1000)) {
//        TachometerRps = TachometerHits;
//        TachometerHits = 0;
//        TachometerTimerStart = TachometerTimerEnds;
//    }
//
//
//    if (digitalRead(RPM_SNS_PIN) == HIGH) {
//        if (!TachometerCounted) {
//            TachometerCounted = true;
//            TachometerHits++;
//
//        }
//    } else {
//        TachometerCounted = false;
//    }


  TachometerTimerEnds = millis ();
  if (TachometerTimerEnds >= (TachometerTimerStart + 500))
    {
      TachometerTimerStart = TachometerTimerEnds;
      TachometerRps = TachometerHits;
      rpmTimeRpmDif = millis () - timeRpmHits;

      #ifndef RpmSensDebug
            Serial.print ("\n");
            Serial.print ("RPM diff:  \t");
            Serial.print (rpmTimeRpmDif);
            Serial.print ("RPM count:  \t");
            Serial.print (TachometerHits);
            Serial.print ("\n");
      #endif

      timeRpmHits = 0;
      TachometerHits = 0;
    }

  return rpmTimeRpmDif / 60;

}
#endif //ARDUINOMID_RpmSens_H

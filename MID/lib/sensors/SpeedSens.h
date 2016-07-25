//
// Created by Admin on 7/24/2016.
//

#ifndef ARDUINOMID_SPEEDSENS_H
#define ARDUINOMID_SPEEDSENS_H


int SpeedSensTimerStart = 0, SpeedSensTimerEnds = 0;
int SpeedSensRps = 0;
//
// Speed = (FirstDistance - SecondDistance) / (SecondTime - FirstTime)
static int getDigitalSpeedKmh() {

    SpeedSensTimerEnds = millis();
    if (SpeedSensTimerEnds >= (SpeedSensTimerStart + 1000)) {
        SpeedSensRps = SpeedSensHits;
        SpeedSensHits = 0;
        SpeedSensTimerStart = SpeedSensTimerEnds;
    }


    if (digitalRead(RPM_SNS_PIN) == HIGH) {


        if (!TachometerCounted) {
            TachometerCounted = true;
            SpeedSensHits++;
        }
    } else {
        TachometerCounted = false;
    }

    return SpeedSensRps * 30;
}

#endif //ARDUINOMID_SPEEDSENS_H

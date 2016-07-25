//
// Created by Admin on 7/24/2016.
//

#ifndef ARDUINOMID_SPEEDSENS_H
#define ARDUINOMID_SPEEDSENS_H


unsigned int long SpeedSensTimerStart = 0, SpeedSensTimerEnds = 0;
int SpeedSensRps = 0;
bool SpeedSensCounted = false;
unsigned int SpeedSensHits = 0;

// rpm*(circumference of your wheel in inches)*(60 min/hr)*(1/63,360 miles/inches)=speed in MPH
// Speed = (FirstDistance - SecondDistance) / (SecondTime - FirstTime)
static int getDigitalSpeedKmh() {

    SpeedSensTimerEnds = millis();
    if (SpeedSensTimerEnds >= (SpeedSensTimerStart + 1000)) {
        SpeedSensRps = SpeedSensHits;
        SpeedSensHits = 0;
        SpeedSensTimerStart = SpeedSensTimerEnds;
    }


    if (digitalRead(RPM_SNS_PIN) == HIGH) {


        if (!SpeedSensCounted) {
            SpeedSensCounted = true;
            SpeedSensHits++;
        }
    } else {
        SpeedSensCounted = false;
    }

//    return SpeedSensRps * 30;

   int long cmTravel =  microsecondsToCentimeters(SpeedSensRps);
}



#endif //ARDUINOMID_SPEEDSENS_H

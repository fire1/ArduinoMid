//
// Created by Admin on 7/23/2016.
//

#ifndef ARDUINOMID_TACHOMETER_H
#define ARDUINOMID_TACHOMETER_H


static void calcRPM();

static void rpmPulseCount();

static void digitalReadRpm();

volatile byte halfRevolutions;

unsigned long int TachometerRPM;
unsigned long int PulseStartTime, PulseEndTime;

unsigned long int countTachometerRpm, maxRPM;  //  DEFINE RPM AND MAXIMUM RPM
unsigned long TachometerTimeOld;

int TachometerCycle, CycleOnOrOff = 0;




void setupTachometer() {
    pinMode(RPM_SNS_PIN, INPUT);
}

/*int getTachometerRpm() {

    if (halfRevolutions >= 20) {
        countTachometerRpm = 30 * 1000 / (millis() - TachometerTimeOld) * halfRevolutions;
        TachometerTimeOld = millis();
        halfRevolutions = 0;
//        Serial.println(rpm, DEC);
//        return countTachometerRpm; // or
        return TachometerRPM;
    }
}*/

/*
static void calcRPM() {

    Serial.println("Cal");
    Serial.print("\t");


    int PulseTime = PulseEndTime - PulseStartTime; // Gets pulse duration

    TachometerRPM = 30 * 1000 / PulseTime * 2;                 // Calculates RPM
    attachInterrupt(20, rpmPulseCount, RISING);      // re-attaches interrupt to Digi Pin 2
}*/


int TachometerHits = 0;
bool TachometerCounted = false;
int TachometerRps = 0;
int TachometerTimerStart = 0, TachometerTimerEnds = 0;

/**
 * Working version
 */
static int getDigitalTachometerRpm() {

    TachometerTimerEnds = millis();
    if (TachometerTimerEnds >= (TachometerTimerStart + 1000)) {
        TachometerRps = TachometerHits;
        TachometerHits = 0;
        TachometerTimerStart = TachometerTimerEnds;
    }


    if (digitalRead(RPM_SNS_PIN) == HIGH) {
        
        
        if (!TachometerCounted) {
            TachometerCounted = true;
            TachometerHits++;
        }
    } else {
        TachometerCounted = false;
    }

    return TachometerRps * 30;
}
/*

static void rpmPulseCount() // EVERYTIME WHEN THE SENSOR GOES FROM LOW TO HIGH , THIS FUNCTION WILL BE INVOKED
{
    if (TachometerCycle == 0)  // Check to see if start pulse
    {
        PulseStartTime = millis();  // stores start time
        CycleOnOrOff = 1;           // sets counter for start of pulse
        return;                     // a return so it doesnt run the next if
    }
    if (TachometerCycle == 1)             // Check to see if end pulse
    {
        detachInterrupt(0);         // Turns off inturrupt for calculations
        PulseEndTime = millis();    // stores end time
        TachometerCycle = 0;                  // resets counter for pulse cycle
        calcRPM();                  // call to calculate pulse time
    }

//    if (digitalRead (RPM_SNS_PIN) == HIGH)
    halfRevolutions++;                                         // INCREASE REVOLUTIONS
}*/
#endif //ARDUINOMID_TACHOMETER_H

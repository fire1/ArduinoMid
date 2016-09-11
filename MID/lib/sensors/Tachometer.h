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
    if (TachometerTimerEnds >= (TachometerTimerStart + 600)) {
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

int sdvstate = 0;
int rpmstate = 0;
int sdvstateold = 0;
int rpmstateold = 0;
int sdvhcount = 0;
float sdvhmax = 0;
int sdvhimax = 0;
int rpmhcount = 0;

int sdvbreak = 0;
int rpmbreak = 0;

int rpm2s = 0;
int rpm3s = 0;

int rpmh = 0;
int rpmh2 = 0;

int rpmvalue = 0;
int rpmfloat = 0;

int rpmstart = 0;

int rpmstatestart = 0;
char rpmdisp[4];

unsigned long time;
unsigned long timeold;
unsigned long timediff;
unsigned long timediff2;

static char getDigitalTachometerRpm2() {

    timeold = micros();
    while (rpmstart == LOW) {
        time = micros();                                      //Engines RPM
        timediff2 = time - timeold;
        if (timediff2 > 40000)
            rpmstart = 1;
        rpmstateold = rpmstate;
        rpmstate = digitalRead(RPM_SNS_PIN);
        delayMicroseconds(150);
        if (rpmstate == HIGH && rpmstateold == LOW)            //wait for rising edge
            rpmstart = 1;
    }


    timeold = micros();
    while (rpmhcount < 5) {
        rpmstateold = rpmstate;
        rpmstate = digitalRead(RPM_SNS_PIN);
        delayMicroseconds(150);
        if (rpmstate == HIGH && rpmstateold == LOW)        //wait for rising edge of signal
            rpmhcount++;
        time = micros();                                    //caching old time value...
        timediff2 = time - timeold;                        //
        if (timediff2 > 187500)                            //--> RPM < 800/min
        {
            rpmbreak = 1;
            break;
        }
    }
    time = micros();

    rpmstart = 0;

    rpmhcount = 0;

    timediff = time - timeold;

    rpmfloat = float(timediff);

    rpmfloat = (1 / (rpmfloat / 1000000)) * 150;          //calculate

    rpmvalue = int(rpmfloat);

    rpmh = rpmvalue / 1000;

    rpmh2 = rpmvalue / 100;

    rpm2s = rpmh2 - (rpmh * 10);

    rpm3s = (rpmvalue / 10) - (rpmh2 * 10);


    return rpmfloat;

    rpmdisp[0] = '0' + rpmh;
    rpmdisp[1] = '0' + rpm2s;
    rpmdisp[2] = '0' + rpm3s;
    rpmdisp[3] = '0' + (rpmvalue % 10);
    rpmdisp[4] = '\0';

    if (rpmbreak == HIGH)                                //display 0000 if rpm < 800
    {
        rpmdisp[0] = '0';
        rpmdisp[1] = '0';
        rpmdisp[2] = '0';
        rpmdisp[3] = '0';
    }

    rpmbreak = 0;

//        MyTid.display_message(rpmdisp,1,1);
    return rpmfloat;
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

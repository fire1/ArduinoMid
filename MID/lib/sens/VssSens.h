//
// Created by Admin on 7/24/2016.
//

/**

 As I said I have high quality schematic (I paid for them 450eur, it's about 3000 pages describing every one nut,
 component and wire in very detail and exactly for my car) and you can see that the sensor is under bonet
 and only 2 wires are going from it and they are going into instrument cluster.
 And when I said it's 5Khz for 3600kmh => 60km/minute => 1km / second.
 So if you travel 1km per second and it will make 5000hz so it's 5000 pulses per 1km and
 thats about 8000 pulses per mile (not 50k) so you can bet what you want, but you lose.
 And the point is not to measure everything else
 I just need a way to measure that analog signal (I won't find anything else digital,
 I have just that 2 wires and I need to do something with them)

 */



#ifndef ARDUINOMID_VssSens_H
#define ARDUINOMID_VssSens_H

const bool VssSensDebug = 1;

int vssHitsCount = 0;
int vssCycles = 0;
int long vssTimerStart = 0, vssTimerEnds = 0;
int vssTimeDif = 0;
int vssTimeHits = 0;

void catchVssHits() {
    vssHitsCount++;
    vssTimeHits = micros();

}

void setupVssSens(int pinTarget) {
    pinMode(pinTarget, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pinTarget), catchVssHits, FALLING);
}

static int getVssSens() {

    vssTimerEnds = millis();
    if (vssTimerEnds >= (vssTimerStart + 150)) {
        vssTimerStart = vssTimerEnds;
        vssCycles = vssHitsCount;
        vssTimeDif = millis() - vssTimeHits;
        //
        // debug info
        if(VssSensDebug){
            Serial.print ("\n");
            Serial.print (" vss diff:  \t");
            Serial.print (vssTimeDif);
            Serial.print (" vss is:  \t");
            Serial.print (vssCycles * 1.6);
            Serial.print (" vss count:  \t");
            Serial.print (vssHitsCount);
            Serial.print ("\n");
        }

        //vssTimeHits = 0;
        vssHitsCount = 0;
    }

    int kmhSens = vssCycles * 1.6;

    return kmhSens;
}

#endif //ARDUINOMID_VssSens_H

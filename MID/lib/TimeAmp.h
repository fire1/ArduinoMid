//
// Created by Angel Zaprianov on 29.9.2016 г..
//
#include <Arduino.h>

#ifndef ARDUINOMID_TIMEAMP_H
#define ARDUINOMID_TIMEAMP_H


/** @description
 *
 */
class TimeAmp {

private:
    unsigned long curLow = 0, curSec = 0, curMid = 0, curMin = 0;
    int ampLow = 0, ampSec = 0, ampMid = 0, ampMin = 0;
    int _isLow = 0, _isSec = 0, _isMid = 0, _isMin = 0;
    unsigned long loopCounter = 0;
public:
    TimeAmp(int intervalLow, int intervalMid, int intervalMin, int intervalSec);

    void listener();

    bool isLow() { return (boolean) _isLow; }

    bool isMin() { return (boolean) _isMin; }

    bool isSec() { return (boolean) _isSec; }

    bool isMid() { return (boolean) _isMid; }
};


/*  ***     CPP part of file    *** */

TimeAmp::TimeAmp(int intervalLow, int intervalMid, int intervalMin, int intervalSec) {

    ampMin = intervalMin;
    ampLow = intervalLow;
    ampSec = intervalSec;
    ampMid = intervalMid;

}

void TimeAmp::listener() {

    unsigned long curTime = loopCounter;
    if (curTime >= curMin + ampMin) {
        curMin = curTime;
        _isMin = 1;
    } else {
        _isMin = 0;
    }

    if (curTime >= curLow + ampLow) {
        curLow = curTime;
        _isLow = 1;
    } else {
        _isLow = 0;
    }


    if (curTime >= curSec + ampSec) {
        curSec = curTime;
        _isSec = 1;
    } else {
        _isSec = 0;
    }
    Serial.print(" \n");
    Serial.print(curTime);
    Serial.print(" \t");
    Serial.println(curMid + ampMid);

    if (curTime >= curMid + ampMid) {
        curMid = curTime;
        _isMid = 1;
    } else {
        _isMid = 0;
    }

    loopCounter++;
};
#endif //ARDUINOMID_TIMEAMP_H

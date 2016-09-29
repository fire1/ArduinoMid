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
    bool _isLow = 0, _isSec = 0, _isMid = 0, _isMin = 0;
public:
    TimeAmp(int intervalLow, int intervalMid, int intervalMin, int intervalSec);

    void listener();

    bool isLow() { return _isLow; }

    bool isMin() { return _isMin; }

    bool isSec() { return _isSec; }

    bool isMid() { return _isMid; }
};


/*  ***     CPP part of file    *** */

TimeAmp::TimeAmp(int intervalLow, int intervalMid, int intervalMin, int intervalSec) {

    ampMin = intervalMin;
    ampLow = intervalLow;
    ampSec = intervalSec;
    ampMid = intervalMid;

}

void TimeAmp::listener() {

    unsigned long curTime = millis();

    if (curTime <= curMin + ampMin) {
        curMin = curTime;
        _isMin = true;
    } else {
        _isMin = false;
    }

    if (curTime <= curLow + ampLow) {
        curLow = curTime;
        _isLow = true;
    } else {
        _isLow = false;
    }


    if (curTime <= curSec + ampSec) {
        curSec = curTime;
        _isSec = true;
    } else {
        _isSec = false;
    }


    if (curTime <= curMid + ampMid) {
        curMid = curTime;
        _isMid = true;
    } else {
        _isMid = false;
    }
};
#endif //ARDUINOMID_TIMEAMP_H

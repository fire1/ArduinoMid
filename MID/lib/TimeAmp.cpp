//
// Created by Angel Zaprianov on 29.9.2016 г..
//
#include <Arduino.h>
#include "TimeAmp.h"


TimeAmp::TimeAmp(int intervalLow,  int intervalMid, int intervalMin, int intervalSec) {

    ampMin = intervalMin;
    ampLow = intervalLow;
    ampSec = intervalSec;
    ampMid = intervalMid;

}

void TimeAmp::loop() {
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
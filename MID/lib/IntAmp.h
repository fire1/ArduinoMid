//
// Created by Angel Zaprianov on 29.9.2016 г..
//
#include <Arduino.h>

#ifndef ARDUINOMID_TIMEAMP_H
#define ARDUINOMID_TIMEAMP_H


#ifndef MILLIS_SENS
#define MILLIS_SENS 200
#endif

#if !defined(MILLIS_PER_HR)
#define MILLIS_PER_HR 3600000
#endif

#if !defined(MILLIS_PER_MN)
#define MILLIS_PER_MN 60000
#endif

#if !defined(MILLIS_PER_SC)
#define MILLIS_PER_SC 1000
#endif

/** @description
 *
 */
class IntAmp {

private:
    unsigned long timer;
    unsigned long curLow = 0, curSec = 0, curMid = 0, curMin = 0, curBig = 0, curMax = 0;
    long int ampLow = 0, ampSec = 0, ampMid = 0, ampMin = 0, ampBig = 0, ampMax = 0;
    long int _isLow = 0, _isSec = 0, _isMid = 0, _isMin = 0, _isBig = 0, _isMax = 0;
    long int _isSecond = 0, _isMinute = 0, _isHour = 0, _isSens = 0, _is10Seconds = 0, _is5Seconds = 0;
    long int curSecond = 0, curMinute = 0, curHour = 0, curSens = 0, cur10Seconds = 0, cur5Seconds = 0;

    /**
     * MAX 1,193,046 Hour	(h)
     */
    unsigned long loopCounter = 0;
public:
    IntAmp(int intervalMin, int intervalLow, int intervalMid, int intervalSec, int intervalBig, int intervalMax);

    void listener();

    unsigned long getLoopIndex() { return loopCounter; }

    bool isLow() { return (bool) _isLow; }

    bool isMin() { return (bool) _isMin; }

    bool isSec() { return (bool) _isSec; }

    bool isMid() { return (bool) _isMid; }

    bool isBig() { return (bool) _isBig; }

    bool isMax() { return (bool) _isMax; }

    /************** Real Time *********************/

    bool isSecond() { return (bool) _isSecond; }

    bool is10Seconds() { return (bool) _is10Seconds; }

    bool is5Seconds() { return (bool) _is5Seconds; }

    bool isMinute() { return (bool) _isMinute; }

    bool isHour() { return (bool) _isHour; }

    bool isSens() { return (bool) _isSens; }

    void setTimer(unsigned long time);


};


/***********************************************************************************************
 *                                                                                             *
 *                                   CPP part of file                                          *
 *                                                                                             *
 ***********************************************************************************************/


IntAmp::IntAmp(int intervalMin, int intervalLow, int intervalMid, int intervalSec, int intervalBig, int intervalMax) {

    ampMin = intervalMin;
    ampLow = intervalLow;
    ampSec = intervalSec;
    ampMid = intervalMid;
    ampBig = intervalBig;
    ampMax = intervalMax;

}

/**
 * Sets external timer
 */
void IntAmp::setTimer(unsigned long time) {
    timer = time;
}

/**
 * Listen cases
 */
void IntAmp::listener() {

    unsigned long curIndex = loopCounter;

    if (curIndex >= curMin + ampMin && !_isMin) {
        curMin = curIndex;
        _isMin = 1;
    } else {
        _isMin = 0;
    }

    if (curIndex >= curLow + ampLow && !_isLow) {
        curLow = curIndex;
        _isLow = 1;
    } else {
        _isLow = 0;
    }


    if (curIndex >= curSec + ampSec && !_isSec) {
        curSec = curIndex;
        _isSec = 1;
    } else {
        _isSec = 0;
    }

    if (curIndex >= curMid + ampMid && !_isMid) {
        curMid = curIndex;
        _isMid = 1;
    } else {
        _isMid = 0;
    }

    if (curIndex >= curBig + ampBig && !_isBig) {
        curBig = curIndex;
        _isBig = 1;
    } else {
        _isBig = 0;
    }

    if (curIndex >= curMax + ampMax && !_isMax) {
        curMax = curIndex;
        _isMax = 1;
    } else {
        _isMax = 0;
    }

    loopCounter++;

    /************** Real Time *********************/

    if (timer >= curSecond + MILLIS_PER_SC) {
        curSecond = timer;
        _isSecond = 1;
    } else {
        _isSecond = 0;
    }

    if (timer >= cur5Seconds + (MILLIS_PER_SC * 5)) {
        cur5Seconds = timer;
        _is5Seconds = 1;
    } else {
        _is5Seconds = 0;
    }

    if (timer >= cur10Seconds + (MILLIS_PER_SC * 10)) {
        cur10Seconds = timer;
        _is10Seconds = 1;
    } else {
        _is10Seconds = 0;
    }

    if (timer >= curSecond + MILLIS_PER_MN) {
        curMinute = timer;
        _isMinute = 1;
    } else {
        _isMinute = 0;
    }

    if (timer >= curSecond + MILLIS_PER_HR) {
        curHour = timer;
        _isHour = 1;
    } else {
        _isHour = 0;
    }

    if (timer >= curSens + MILLIS_SENS) {
        curSens = timer;
        _isSens = 1;
    } else {
        _isSens = 0;
    }


};
#endif //ARDUINOMID_TIMEAMP_H

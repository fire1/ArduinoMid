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
 * Initialization Amplitudes
 */
class IntAmp {

private:
    unsigned long timer;
    //
    // Toggle timers
    unsigned long curLow = 0, curSec = 0, curMid = 0, curMin = 0, curBig = 0, curMax = 0;
    unsigned long curSecond = 0, curMinute = 0, curHour = 0, curSens = 0, cur10Seconds = 0, cur5Seconds = 0, cur2Seconds, cur4Seconds;
    unsigned int ampLow = 0, ampSec = 0, ampMid = 0, ampMin = 0, ampBig = 0, ampMax = 0;
    boolean _isLow = 0, _isSec = 0, _isMid = 0, _isMin = 0, _isBig = 0, _isMax = 0;
    boolean _isSecond = 0, _isMinute = 0, _isHour = 0, _isSens = 0, _is10Seconds = 0, _is5Seconds = 0, _is2Seconds, _is4Seconds;
    boolean _isToggleDef = 0;


    /**
     * MAX 1,193,046 Hour	(h)
     */
    unsigned long loopCounter = 0;
public:
    IntAmp(unsigned int intervalMin, unsigned int intervalLow, unsigned int intervalMid, unsigned int intervalSec,
           unsigned int intervalBig, unsigned int intervalMax);

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

    bool is2Seconds() { return (bool) _is2Seconds; }

    bool is4Seconds() { return (bool) _is4Seconds; }

    bool isMinute() { return (bool) _isMinute; }

    bool isHour() { return (bool) _isHour; }

    bool isSens() { return (bool) _isSens; }

    void setTimer(unsigned long time);



    /************** Time Toggle *********************/

    bool isToggle() { return (bool) _isToggleDef; }

};


/***********************************************************************************************
 *                                                                                             *
 *                                   CPP part of file                                          *
 *                                                                                             *
 ***********************************************************************************************/


IntAmp::IntAmp(unsigned int intervalMin, unsigned int intervalLow, unsigned int intervalMid, unsigned int intervalSec,
               unsigned int intervalBig, unsigned int intervalMax) {

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

    if (loopCounter >= curMin + ampMin && !_isMin) {
        curMin = loopCounter;
        _isMin = 1;
    } else {
        _isMin = 0;
    }

    if (loopCounter >= curLow + ampLow && !_isLow) {
        curLow = loopCounter;
        _isLow = 1;
    } else {
        _isLow = 0;
    }

    if (loopCounter >= curSec + ampSec && !_isSec) {
        curSec = loopCounter;
        _isSec = 1;
    } else {
        _isSec = 0;
    }

    if (loopCounter >= curMid + ampMid && !_isMid) {
        curMid = loopCounter;
        _isMid = 1;
    } else {
        _isMid = 0;
    }

    if (loopCounter >= curBig + ampBig && !_isBig) {
        curBig = loopCounter;
        _isBig = 1;
    } else {
        _isBig = 0;
    }

    if (loopCounter >= curMax + ampMax && !_isMax) {
        curMax = loopCounter;
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

    if (timer >= cur2Seconds + (MILLIS_PER_SC * 2)) {
        cur2Seconds = timer;
        _is2Seconds = 1;
    } else {
        _is2Seconds = 0;
    }

    if (timer >= cur4Seconds + (MILLIS_PER_SC * 4)) {
        cur4Seconds = timer;
        _is4Seconds = 1;
    } else {
        _is4Seconds = 0;
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

    if (timer >= curMinute + MILLIS_PER_MN) {
        curMinute = timer;
        _isMinute = 1;
    } else {
        _isMinute = 0;
    }

    if (timer >= curHour + MILLIS_PER_HR) {
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

/************** Time Toggle *********************/

    if (_is2Seconds && _isToggleDef == 1) {
        _isToggleDef = 0;
    }

    if (_is2Seconds && _isToggleDef == 0) {
        _isToggleDef = 1;
    }
};
#endif //ARDUINOMID_TIMEAMP_H

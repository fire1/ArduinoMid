//
// Created by Angel Zaprianov on 29.9.2016 г..
//


#ifndef ARDUINOMID_TIMEAMP_H
#define ARDUINOMID_TIMEAMP_H

#include <Arduino.h>

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

//    ampInt(min,low,mid,sec, big, max);
//AmpTime ampInt(5, 10, 50, 100, 250, 1000);

#ifndef AMP_CUSTOM
#define AMP_MIN 5 // MIN
#define AMP_LOW 10 // LOW
#define AMP_MID 50 // MID
#define AMP_SEC 100 // SEC
#define AMP_BIG 250 // BIG
#define AMP_MAX 500 // MAX
#endif

/** @description
 * Initialization Amplitudes
 */
class AmpTime {

    //
    // Toggle timers
    unsigned long curLow = 0, curSec = 0, curMid = 0, curMin = 0, curBig = 0, curMax = 0, curSecond = 0, curMinute = 0, curHour = 0, curSens = 0, cur10Seconds = 0, cur5Seconds = 0, cur2Seconds, cur4Seconds;
    boolean _isLow = 0, _isSec = 0, _isMid = 0, _isMin = 0, _isBig = 0, _isMax = 0;
    boolean _isSecond = 0, _isMinute = 0, _isHour = 0, _isSens = 0, _is10Seconds = 0, _is5Seconds = 0, _is2Seconds, _is4Seconds;
    boolean _isToggleDef = 0;


    /**
     * MAX 1,193,046 Hour	(h)
     */
public:
    AmpTime() {}

    void listener();


    inline boolean isLow() { return (boolean) _isLow; }

    inline boolean isMin() { return (boolean) _isMin; }

    inline boolean isSec() { return (boolean) _isSec; }

    inline boolean isMid() { return (boolean) _isMid; }

    inline boolean isBig() { return (boolean) _isBig; }

    inline boolean isMax() { return (boolean) _isMax; }

    /************** Real Time *********************/

    inline boolean isSecond() { return (boolean) _isSecond; }

    inline boolean is10Seconds() { return (boolean) _is10Seconds; }

    inline boolean is5Seconds() { return (boolean) _is5Seconds; }

    inline boolean is2Seconds() { return (boolean) _is2Seconds; }

    inline boolean is4Seconds() { return (boolean) _is4Seconds; }

    inline boolean isMinute() { return (boolean) _isMinute; }

    inline boolean isHour() { return (boolean) _isHour; }

    inline boolean isSens() { return (boolean) _isSens; }


    /************** Time Toggle *********************/

    boolean isToggle() { return (boolean) _isToggleDef; }

};


/***********************************************************************************************
 *                                                                                             *
 *                                   CPP part of file                                          *
 *                                                                                             *
 ***********************************************************************************************/


/**
 * Listen cases
 */
void AmpTime::listener() {

    unsigned long timer = millis();

    if (timer >= curMin + AMP_MIN && !_isMin) {
        curMin = timer;
        _isMin = 1;
    } else {
        _isMin = 0;
    }

    if (timer >= curLow + AMP_LOW && !_isLow) {
        curLow = timer;
        _isLow = 1;
    } else {
        _isLow = 0;
    }

    if (timer >= curSec + AMP_SEC && !_isSec) {
        curSec = timer;
        _isSec = 1;
    } else {
        _isSec = 0;
    }

    if (timer >= curMid + AMP_MID && !_isMid) {
        curMid = timer;
        _isMid = 1;
    } else {
        _isMid = 0;
    }

    if (timer >= curBig + AMP_BIG && !_isBig) {
        curBig = timer;
        _isBig = 1;
    } else {
        _isBig = 0;
    }

    if (timer >= curMax + AMP_MAX && !_isMax) {
        curMax = timer;
        _isMax = 1;
    } else {
        _isMax = 0;
    }


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

    timer = 0;
/************** Time Toggle *********************/

//    if (_is2Seconds && _isToggleDef == 1) {
//        _isToggleDef = 0;
//    }
//
//    if (_is2Seconds && _isToggleDef == 0) {
//        _isToggleDef = 1;
//    }
};
#endif //ARDUINOMID_TIMEAMP_H

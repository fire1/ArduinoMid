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

//
// Thous values affect calculation of board computer!!!
// If you change them, calculation will be changed!
#ifndef AMP_CUSTOM
#define AMP_MIN 5 // MIN
#define AMP_LOW 10 // LOW
#define AMP_MID 50 // MID
#define AMP_SEC 100 // SEC
#define AMP_BIG 200 // BIG
#define AMP_MAX 400 // MAX
#define AMP_RFR 601 // TODO check 800 // Screen Refresh rate
#endif

/** @description
 * Initialization Amplitudes
 */
class AmpTime {

    //
    // Toggle timers
    unsigned long
            curLow = 0, curSec = 0, curMid = 0, curMin = 0, curBig = 0, curMax = 0, curSecond = 0, curMinute = 0,
            curHour = 0, curSens = 0, cur10Seconds = 0, cur5Seconds = 0, cur2Seconds, cur4Seconds, curRefresh;
    boolean _isLow = false, _isSec = false, _isMid = false, _isMin = false, _isBig = false, _isMax = false;
    boolean _isSecond = false, _isMinute = false, _isHour = false, _isSens = false, _is10Seconds = false,
            _is5Seconds = false, _is2Seconds, _is4Seconds, _isRfr = false;
    boolean _isToggleDef = false;


    /**
     * MAX 1,193,046 Hour	(h)
     */
public:
    AmpTime() {};

    void listener();


    inline boolean isLow() { return (boolean) _isLow; }

    inline boolean isMin() { return (boolean) _isMin; }

    inline boolean isSec() { return (boolean) _isSec; }

    inline boolean isMid() { return (boolean) _isMid; }

    inline boolean isBig() { return (boolean) _isBig; }

    inline boolean isMax() { return (boolean) _isMax; }

    inline boolean isRfr() { return (boolean) _isRfr; }

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

    cli();
    unsigned long timer = millis();

    if (timer >= curMin + AMP_MIN && !_isMin) {
        curMin = timer;
        _isMin = true;
    } else {
        _isMin = false;
    }

    if (timer >= curLow + AMP_LOW && !_isLow) {
        curLow = timer;
        _isLow = true;
    } else {
        _isLow = false;
    }

    if (timer >= curSec + AMP_SEC && !_isSec) {
        curSec = timer;
        _isSec = true;
    } else {
        _isSec = false;
    }

    if (timer >= curMid + AMP_MID && !_isMid) {
        curMid = timer;
        _isMid = true;
    } else {
        _isMid = false;
    }

    if (timer >= curBig + AMP_BIG && !_isBig) {
        curBig = timer;
        _isBig = true;
    } else {
        _isBig = false;
    }

    if (timer >= curMax + AMP_MAX && !_isMax) {
        curMax = timer;
        _isMax = true;
    } else {
        _isMax = false;
    }

    if (timer >= curRefresh + AMP_RFR && !_isRfr) {
        curRefresh = timer;
        _isRfr = true;
    } else {
        _isRfr = false;
    }

    /************** Real Time *********************/

    if (timer >= curSecond + MILLIS_PER_SC && !_isSecond) {
        curSecond = timer;
        _isSecond = true;
    } else {
        _isSecond = false;
    }

    if (timer >= cur2Seconds + (MILLIS_PER_SC * 2) && !_is2Seconds) {
        cur2Seconds = timer;
        _is2Seconds = true;
    } else {
        _is2Seconds = false;
    }

    if (timer >= cur4Seconds + (MILLIS_PER_SC * 4) && !_is4Seconds) {
        cur4Seconds = timer;
        _is4Seconds = true;
    } else {
        _is4Seconds = false;
    }

    if (timer >= cur5Seconds + (MILLIS_PER_SC * 5) && !_is5Seconds) {
        cur5Seconds = timer;
        _is5Seconds = true;
    } else {
        _is5Seconds = false;
    }

    if (timer >= cur10Seconds + (MILLIS_PER_SC * 10) && !_is10Seconds) {
        cur10Seconds = timer;
        _is10Seconds = true;
    } else {
        _is10Seconds = false;
    }

    if (timer >= curMinute + MILLIS_PER_MN && !_isMinute) {
        curMinute = timer;
        _isMinute = true;
    } else {
        _isMinute = false;
    }

    if (timer >= curHour + MILLIS_PER_HR && !_isHour) {
        curHour = timer;
        _isHour = true;
    } else {
        _isHour = false;
    }

    if (timer >= curSens + MILLIS_SENS && !_isSens) {
        curSens = timer;
        _isSens = true;
    } else {
        _isSens = false;
    }
    sei();

/************** Time Toggle *********************/

//    if (_is2Seconds && _isToggleDef == 1) {
//        _isToggleDef = false;
//    }
//
//    if (_is2Seconds && _isToggleDef == 0) {
//        _isToggleDef = true;
//    }
};
#endif //ARDUINOMID_TIMEAMP_H

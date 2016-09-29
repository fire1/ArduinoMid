//
// Created by Angel Zaprianov on 29.9.2016 г..
//

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

    void looper();

    bool isLow() { return _isLow; }

    bool isMin() { return _isMin; }

    bool isSec() { return _isSec; }

    bool isMid() { return _isMid; }
};


#endif //ARDUINOMID_TIMEAMP_H

//
// Created by Angel Zaprianov on 16.9.2016 г..
//

#include <Average.h>

#ifndef ARDUINOMID_AVRGCALC_H
#define ARDUINOMID_AVRGCALC_H

const int numReadingsAverageVss = 100;
const int numReadingsAverageRpm = 100;

Average<int> AveVss(numReadingsAverageVss);
Average<int> AveRpm(numReadingsAverageRpm);

void sensAvr(void) {
    AveVss.push(getVssSens());
    AveRpm.push(getRpmSens());
}

/**
 * Gets Average speed
 */
int getAverageVss() {
    return AveVss.mean();
}

/**
 * Gets Average Rpm
 */
int getAverageRpm() {
    return AveRpm.mean();
}

#endif //ARDUINOMID_AVRGCALC_H

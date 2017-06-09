//
// Created by Angel Zaprianov on 9.6.2017 г..
//

#include <avr/io.h>
#include "AmpTime.h"

#ifndef ARDUINO_MID_COP_TRIP_H
#define ARDUINO_MID_COPT_RIP_H


#ifndef COP_TRIP_LEN
#define COP_TRIP_LEN 100
#endif

typedef struct cop_trip {
    uint8_t index;
    uint8_t value;
};

/**
 * Coefficient of performance collection
 */
class CopTrip {
    cop_trip collection[COP_TRIP_LEN];
    AmpTime *amp;


public:

    void accumulate(uint8_t value) {
        for (int n = (COP_TRIP_LEN - 1); n > 0; n--) {
            this->collection[n] = this->collection[n - 1];
        }
        this->collection[0].index = (millis() / MILLIS_PER_MN) - this->collection[1].index;
        this->collection[0].value = value;

    }


    cop_trip *result(void) {
        return this->collection;
    }
};

#endif //ARDUINOMID_COPTRIP_H

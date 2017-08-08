//
// Created by Angel Zaprianov on 1.8.2017 г..
//

#ifndef ARDUINO_MID_CAR_COLLECT_H
#define ARDUINO_MID_CAR_COLLECT_H

#define CAR_COLLECT_LEN 6

#include "CarSens.h"

struct DataCollection {
    uint8_t vss;
    uint8_t odo;
    uint16_t rpm;
    uint16_t tmp; // time point
};


class CarCollect {


    CarSens *car;
    AmpTime *amp;

    DataCollection c_sc[CAR_COLLECT_LEN];
    DataCollection c_mn[CAR_COLLECT_LEN];
    DataCollection c_hr[CAR_COLLECT_LEN];
    DataCollection cll;

    uint8_t sec_ind, min_ind, hrs_ind;
    uint16_t avr_rpm, avr_vss, avr_ind;

    unsigned long lastMin = 0;
    unsigned long lastHrs = 0;

public:
    CarCollect(CarSens &_car, AmpTime &_amp) : car(&_car), amp(&_amp) {

    }


    void listener() {
        this->sensData();
        this->sensAvrSec();
        this->passAvrMin();
        this->passAvrHrs();
    }


    boolean isDrowsiness() {

    }

    uint8_t getLen() {
        return CAR_COLLECT_LEN;
    }

    DataCollection *getSc() {
        return c_sc;
    }

    DataCollection *getMn() {
        return c_mn;
    }

    DataCollection *getHr() {
        return c_hr;
    }

protected:

    //
    // Capture 10 sec averages
    void sensAvrSec() {

        if (amp->is10Seconds()) {
            c_sc[sec_ind].rpm = avr_rpm / avr_ind;
            c_sc[sec_ind].vss = avr_vss / avr_ind;
            c_sc[sec_ind].odo = uint8_t(car->getDst() / 10);
            c_sc[sec_ind].tmp = uint8_t(millis() / MILLIS_PER_MN);
            sec_ind++;
        }
    }

    //
    // Pass seconds collection to minute
    void passAvrMin() {
        if ((millis() - lastMin) > MILLIS_PER_MN) {

            uint32_t sum_rpm, sum_tmp;
            uint16_t sum_vss, sum_odo;
            for (int i = 0; i < CAR_COLLECT_LEN; ++i) {

                sum_rpm += c_sc[i].rpm;
                c_sc[i].rpm = 0;

                sum_tmp += c_sc[i].tmp;
                c_sc[i].tmp = 0;

                sum_vss += c_sc[i].vss;
                c_sc[i].vss = 0;

                sum_vss += c_sc[i].odo;
                c_sc[i].odo = 0;
            }

            c_mn[min_ind].rpm = sum_rpm / sec_ind;
            c_mn[min_ind].tmp = sum_tmp / sec_ind;
            c_mn[min_ind].vss = sum_vss / sec_ind;
            c_mn[min_ind].odo = sum_odo / sec_ind;
            sec_ind = 0;

            lastMin = millis();
        }
    }


    //
    // Pass seconds collection to minute
    void passAvrHrs() {
        if ((millis() - lastHrs) > MILLIS_PER_HR) {

            uint32_t sum_rpm, sum_tmp;
            uint16_t sum_vss, sum_odo;
            for (int i = 0; i < CAR_COLLECT_LEN; ++i) {

                sum_rpm += c_mn[i].rpm;
                c_mn[i].rpm = 0;

                sum_tmp += c_mn[i].tmp;
                c_mn[i].tmp = 0;

                sum_vss += c_mn[i].vss;
                c_mn[i].vss = 0;

                sum_vss += c_mn[i].odo;
                c_mn[i].odo = 0;
            }


            c_hr[hrs_ind].rpm = sum_rpm / min_ind;
            c_hr[hrs_ind].tmp = sum_tmp / min_ind;
            c_hr[hrs_ind].vss = sum_vss / min_ind;
            c_hr[hrs_ind].odo = sum_odo / min_ind;
            min_ind = 0;
            lastHrs = millis();
        }
    }

    void sensData() {
        if (amp->isSens()) {
            avr_rpm += car->getRpm() / 100;
            avr_vss += car->getVss() / 10;
            avr_ind++;
        }
    }


};


#endif //ARDUINOMID_CARCAPTURE_H

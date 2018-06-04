//
// Created by Angel Zaprianov on 21.9.2017 г..
//

#ifndef ARDUINO_MID_MELODY_H
#define ARDUINO_MID_MELODY_H

#include <Arduino.h>
#include "AmpTime.h"
#include "../MID.h"


class Melody {

    AmpTime *amp;
    uint8_t melody = 0;
    uint8_t index;

    boolean tempoFast() {
        if (amp->isMid()) {
            index++;
//            if (index > 30) {
//                index = 0;
//            }
            return true;
        }
        return false;
    }

    boolean tempoSlow() {
        if (amp->isSec()) {
            index++;
//            if (index > 50) {
//                index = 0;
//            }
            return true;
        }
        return false;
    }

/**
 * Checker to high fast
 */
    void m1() {
        if (tempoFast()) {
            uint16_t rise = 1000;
            if (index <= 5) {
                rise = rise + index * 400;
            }
            if (index > 5) {
                rise = 3000;
            }
            if (index < 10) {
                tone(TONE_ADT_PIN, rise, 30);

            }
        }
    }

/**
 * Checker to high slow
 */
    void m2() {
        if (tempoSlow()) {
            uint16_t rise = 1000;
            if (index <= 5) {
                rise = rise + index * 400;
            }
            if (index > 5) {
                rise = 3000;
            }
            if (index < 10) {
                tone(TONE_ADT_PIN, rise, 30);

            }
        }
    }

/**
 * Checker wave to high
 */
    void m3() {
        if (tempoSlow()) {
            uint16_t rise = 0;
            if (index <= 2) {
                rise = index * 200;
            }
            if (index <= 4 && index >= 2) {
                rise = index * 400;
            }
            if (index > 4) {
                rise = 800;
            }
            if (index < 10) {
                tone(TONE_ADT_PIN, 1000 + rise, 30);

            }
        }
    }

/**
 * To High fast melody
 */
    void m4() {
        if (tempoFast()) {
            uint16_t rise = 1000;
            if (index <= 8) {
                rise = rise + index * 200;
            }

            if (index < 10) {
                tone(TONE_ADT_PIN, rise, 30);

            }
        }
    }

/**
 * Alarm melody
 */
    void m5() {
        if (tempoSlow()) {
            if (index % 5) {
                tone(TONE_ADT_PIN, 2000, 30);
            }
        }
    }

/**
 * To low beep melody
 */
    void m6() {
        if (tempoSlow()) {
            uint16_t rise = 2000;
            if (index <= 3) {
                rise = rise - index * 500;
            }
            if (index > 3) {
                rise = 400;
            }
            if (index < 7) {
                tone(TONE_ADT_PIN, rise, 30);

            }
        }

    }

/**
 * 3 beeps low
 */
    void m7() {
        if (tempoSlow()) {

            if (index < 4) {
                tone(TONE_ADT_PIN, 500, 30);

            }
        }

    }

/**
 * 3 beeps high
 */
    void m8() {
        if (tempoSlow()) {
            if (index < 4) {
                tone(TONE_ADT_PIN, 2500, 30);

            }
        }

    }

/**
 * 2 beeps high
 */
    void m9() {
        if (tempoSlow()) {


            if (index == 2) {
                tone(TONE_ADT_PIN, 2000, 30);

            }

            if (index == 4) {
                tone(TONE_ADT_PIN, 2500, 30);

            }
        }

    }

/**
 * 2 beeps low
 */
    void m10() {
        if (tempoSlow()) {

            if (index == 2) {
                tone(TONE_ADT_PIN, 750, 30);
            }

            if (index == 4) {
                tone(TONE_ADT_PIN, 750, 30);
            }
        }

    }

public:
    Melody(AmpTime &ampInt) : amp(&ampInt) {

    }

    void begin() {
        pinMode(TONE_ADT_PIN, OUTPUT);
    }

/**
 * Sets melody to play
 * @param value
 */
    void play(uint8_t value) {
        melody = value;
    }

    void playSave() {
        melody = 2;
    }

    void playSpeed() {
        melody = 8;
    }

    uint8_t getPlay() {
        return melody;
    }

/**
 * general executor of melody
 */
    void listener() {

        switch (melody) {
            case 0:
            default:
                break;

            case 1:
                m1();
                break;
            case 2:
                m2();
                break;
            case 3:
                m3();
                break;
            case 4:
                m4();
                break;
            case 5:
                m5();
                break;
            case 6:
                m6();
                break;
            case 7:
                m7();
                break;
            case 8:
                m8();
                break;
            case 9:
                m9();
                break;
            case 10:
                m10();
                break;
        }

        //
        // Zeroing melody
        if (melody > 0 && index > 25) {
            melody = 0;
            index = 0;
        }

//
//        Serial.print("Melody index: ");
//        Serial.print(index);
//        Serial.print(" Selection ");
//        Serial.println(melody);
//
    }

};


#endif //ARDUINOMID_MELODY_H

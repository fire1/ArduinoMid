


#include <Arduino.h>
#include "AmpTime.h"


#define TONE_ADT_PIN 11

class Melody {

    AmpTime *amp;


    uint8_t index;

    boolean tempoFast() {
        if (amp->isMid()) {
            index++;
            if (index > 50) {
                index = 0;
            }
            return true;
        }
        return false;
    }

    boolean tempoSlow() {
        if (amp->isSec()) {
            index++;
            if (index > 50) {
                index = 0;
            }
            return true;
        }
        return false;
    }

public:
    Melody(AmpTime &ampInt) : amp(&ampInt) {

    }

    void begin() {
        pinMode(TONE_ADT_PIN, OUTPUT);
    }

    void listener() {

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
                Serial.println(rise);
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
                Serial.println(rise);
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
                Serial.println(1000 + rise);
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
                Serial.println(rise);
            }
        }
    }
/**
 * Alarm melody
 */
    void m5() {
        if (tempoSlow()) {
            if (index % 5) {
                tone(TONE_ADT_PIN, 1200, 30);
                Serial.println(1200);
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
                Serial.println(rise);
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
                Serial.println(500);
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
                Serial.println(2500);
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
                Serial.println(2000);
            }

            if (index == 4) {
                tone(TONE_ADT_PIN, 2500, 30);
                Serial.println(2500);
            }
        }

    }

};

AmpTime ampInt;

Melody mld(ampInt);

void setup() {
    mld.begin();
    Serial.begin(115200);
}


void loop() {
    ampInt.listener();

    mld.listener();

    mld.m8();
}
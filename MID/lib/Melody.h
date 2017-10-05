//
// Created by Angel Zaprianov on 21.9.2017 г..
//

#ifndef ARDUINO_MID_MELODY_H
#define ARDUINO_MID_MELODY_H

#include <Arduino.h>
#include "AmpTime.h"

typedef const uint16_t Symphony[];


class Melody {

    boolean outputTone = false;
    boolean starts = false;

    uint8_t pinSpeakerA;
    uint16_t soundIndex = 0;
    uint16_t length;
// BEFORE SETUP
    const int tonePin = 8;
    unsigned long previousMillis = 0;
    const long pauseBetweenNotes;
    const long noteDuration;

    Symphony melody;

public:

    void play(Symphony val) {
        starts = true;
        melody = val;
        length = (sizeof(melody)/sizeof(*melody));
    }

    void begin(uint8_t pinBuzzer) {
        pinSpeakerA = pinBuzzer;

    }

    // IN LOOP
    void listener() {

        if (starts) {
            unsigned long currentMillis = millis();
            if (outputTone) {
                // We are currently outputting a tone
                // Check if it's been long enough and turn off if so
                if (currentMillis - previousMillis >= noteDuration) {
                    previousMillis = currentMillis;
                    noTone(tonePin);
                    outputTone = false;
                }
            } else {
                // We are currently in a pause
                // Check if it's been long enough and turn on if so
                if (currentMillis - previousMillis >= pauseBetweenNotes) {
                    previousMillis = currentMillis;
                    tone(tonePin, melody[soundIndex]);
                    outputTone = true;
                }
            }
            soundIndex++;
            //
            // Send end of
            if(length > soundIndex)
                starts = false;
        }

    }



};





#endif //ARDUINOMID_MELODY_H

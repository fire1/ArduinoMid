//
// Created by Angel Zaprianov on 21.9.2017 г..
//

#ifndef ARDUINO_MID_MELODY_H
#define ARDUINO_MID_MELODY_H

#include <Arduino.h>

class Melody{

// BEFORE SETUP
    const int tonePin = 8;
    unsigned long previousMillis = 0;
    const long  pauseBetweenNotes;
    const long  noteDuration;

    boolean outputTone = false;

    void begin(uint8_t pinBuzzer)
    {
        pinSpeakerA=pinBuzzer;

    }
    // IN LOOP
    void listener()
    {

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
                tone(tonePin, melody[thisNote]);
                outputTone = true;
            }
        }


    }

};

#endif //ARDUINOMID_MELODY_H

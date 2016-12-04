#include <Arduino.h>
#include <Wire.h>

uint8_t dataPin = A5;
uint8_t clockPin = A4;

void setup() {
    pinMode(dataPin, INPUT);
    pinMode(clockPin, INPUT);

    Serial.begin(115200);
}

int lastRead = 0;
uint8_t i = 0;
uint8_t value = 0;
uint8_t temp;

void loop() {


//    int currentState = analogRead(dataPin);

    value = 0;
    /*if (currentState > lastRead) {
        temp |= 1 << i;
    } else {
        temp |= 0 << i;
    }
/*
    temp |= digitalRead(clockPin) << i;
    lastRead = currentState;





    if (value) {
        Serial.println(value, BIN);
    }*/

    temp |= digitalRead(clockPin) << i;

    i++;
    if (i >= 8) {
        i = 0;
        value = temp;
        temp = 0;
    }

    if (value) {
        Serial.println(value);
    }

}

/*

 11111110
11111111
11111111


 */



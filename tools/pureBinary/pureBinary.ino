#include <Arduino.h>
#include <Wire.h>

uint8_t dataPin = A5;
uint8_t clockPin = A4;

void setup() {
    pinMode(dataPin, INPUT);
    pinMode(clockPin, INPUT);

    Serial.begin(250000);
}

int lastRead = 0;
uint8_t i = 0;
uint8_t value = 0;
uint8_t temp;

void loop() {


    int currentState = analogRead(dataPin);

    value = 0;
    if (currentState > lastRead) {
        temp |= digitalRead(clockPin) << i;
        lastRead = currentState;
    }

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



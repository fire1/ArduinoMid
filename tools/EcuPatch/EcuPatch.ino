#include <Arduino.h>


uint16_t ecuHitsCount = 0;
unsigned long avrReadings = 0;

void EngSens_catchEcuHits() {
    ecuHitsCount++;

}

const uint8_t pinTarget = 2;

void setup() {

    Serial.begin(115200);
    pinMode(pinTarget, INPUT);

    attachInterrupt(digitalPinToInterrupt(pinTarget), EngSens_catchEcuHits, FALLING);
//    digitalWrite(18,HIGH);
//    digitalWrite(20,HIGH);
//    digitalWrite(pinTarget,HIGH);
}

uint8_t loopIndex = 0;

void loop() {

    avrReadings += analogRead(pinTarget);
    loopIndex++;

    if (loopIndex > 200) {

        Serial.print("\t Cur: ");
        Serial.print(analogRead(pinTarget));

        Serial.print("\t Avr: ");
        Serial.print(avrReadings / loopIndex);

        Serial.print("\t Hint: ");
        Serial.println(ecuHitsCount);
        ecuHitsCount = 0;

        avrReadings = 0;
        loopIndex = 0;

    }

}

#include <Arduino.h>


#define PIN_OUT 3


void setup() {
    Serial.begin(115200);
    pinMode(PIN_OUT, OUTPUT);
    digitalWrite(PIN_OUT, HIGH);
    Serial.println("LPG Test signal begins in 200ms ");
    delay(200);

}

void doB0() {
    digitalWrite(PIN_OUT, LOW);
    delay(3);
    digitalWrite(PIN_OUT, HIGH);
    delay(2);
}

void doB1() {
    digitalWrite(PIN_OUT, LOW);
    delay(3);
    digitalWrite(PIN_OUT, HIGH);
    delay(4);
}

void loop() {
    Serial.println("LPG ping start");
    doB1();
    doB0();
    doB0();
    doB0();
    doB0();
    doB1();
    doB0();
    doB0();
    doB0();
    doB0();
    doB1();
    doB0();
    doB1();
    doB1();
    Serial.println("LPG ping wait 16ms");
    delay(16);
    doB1();
    doB0();
    doB0();
    doB0();
    doB0();
    doB1();
    doB0();
    doB0();
    doB1();
    doB0();
    doB0();
    doB0();
    doB1();
    doB1();
    Serial.println("LPG ping wait 1600ms");
    delay(1600);
}
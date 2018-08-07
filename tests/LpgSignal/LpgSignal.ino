
#include <Arduino.h>


#define PIN_OUT 3


void setup() {
    Serial.begin(115200);
    pinMode(PIN_OUT, OUTPUT);
    digitalWrite(PIN_OUT, HIGH);
    Serial.println("LPG Test signal begins in 500ms ");
    delay(500);

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
void doEnd(){
    digitalWrite(PIN_OUT, LOW);
    delay(3);
}

void loop() {
    delay(100);
    Serial.println("LPG ping start 10000100001011");
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
    doEnd();
    Serial.println("LPG ping wait 16ms 10000100100011");
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
    doEnd();
    Serial.println("LPG ping wait 1600ms");
    delay(1600);
}
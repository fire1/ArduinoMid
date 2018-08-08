
#include <Arduino.h>

/**
 * This is simple pulse generator for LPG communication simulation.
 * Only purpose is to test LpgSens class
 */

//
// Arduino output pin
#define PIN_OUT 3


void setup() {
    Serial.begin(115200);
    pinMode(PIN_OUT, OUTPUT);
    digitalWrite(PIN_OUT, HIGH);
    Serial.println("LPG Test signal begins in 500ms ");
    delay(500);

}

//
// Simulates 0 bit
void doB0() {
    digitalWrite(PIN_OUT, LOW);
    delay(3);
    digitalWrite(PIN_OUT, HIGH);
    delay(2);
}
//
// Simulates 1 bit
void doB1() {
    digitalWrite(PIN_OUT, LOW);
    delay(3);
    digitalWrite(PIN_OUT, HIGH);
    delay(4);
}
//
// Writes ends of data
void doEnd(){
    digitalWrite(PIN_OUT, LOW);
    delay(3);
}
//
// Simulation loop
// Example:
//3982 // logic 1
//1993 // logic 0
//1993
//1993
//1994
//3978 // logic 1
//1993
//1993
//3978
//1994
//1993
//1995
//3979 // logic 1
//3978 // logic 1
//...
//*
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
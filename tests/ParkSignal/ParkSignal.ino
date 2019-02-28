
#include <Arduino.h>

/**
   This is simple pulse generator for LPG communication simulation.
   Only purpose is to test LpgSens class
*/

//
// Arduino output pin
#define PIN_OUT 3


void setup() {
    Serial.begin(115200);
    pinMode(PIN_OUT, OUTPUT);
    digitalWrite(PIN_OUT, HIGH);
    Serial.println("Parktronic Test signal begins in 500ms ");
    delay(500);

}

void doStart() {

    digitalWrite(PIN_OUT, LOW);
    delay(955);
}

//
// Simulates 0 bit
void doB0() {
    digitalWrite(PIN_OUT, LOW);
    delay(100);
    digitalWrite(PIN_OUT, HIGH);
    delay(200);
}

//
// Simulates 1 bit
void doB1() {
    digitalWrite(PIN_OUT, LOW);
    delay(200);
    digitalWrite(PIN_OUT, HIGH);
    delay(100);
}

//
// Writes ends of data
void doEnd() {
    digitalWrite(PIN_OUT, LOW);
    delay(3500);
}

unsigned long count = 0;
//
// Simulation loop
void loop() {

    digitalWrite(PIN_OUT,HIGH);
    Serial.print(count);
    Serial.println(" Sending signals start ...");
    delay(300);

    doStart();
    for (uint8_t i = 0; i < 5; i++) {
        doB1();
        doB0();
        doB0();
        doB0();
        doB0();
        doB0();
        doB1();
        doB0();
    }
    doEnd();


}

#include <Arduino.h>
#include <Wire.h>

uint8_t pullPin = A5;
uint8_t pushPin = A4;

void setup() {
    pinMode(pullPin, INPUT);
    pinMode(pushPin, INPUT);

    Serial.begin(250000);
}

int pressRead = 0;
int autoRead = 0;
uint8_t i = 0;
uint8_t value = 0;
uint8_t temp;


ISR(TIMER1_0VF_vect) {
    int pushState = digitalRead(pushPin);
    int pullState = digitalRead(pullPin);

    if (pullState == LOW) {
        pressRead++;
    }

    if (pushState == LOW) {
        autoRead++;
    }

    if (pressRead) {
        Serial.print("press: ");
        Serial.println(pressRead);
    }


    if (autoRead) {
        Serial.print("auto: ");
        Serial.println(autoRead);
    }
}

void loop() {

//
//    Serial.print(" data: ");
//    Serial.print(digitalRead(pullPin));
//
//    Serial.print(" clock: ");
//    Serial.print(digitalRead(pushPin));
//    Serial.print("\n\n\n");

    /*
    int pushState = digitalRead(pushPin);
    int pullState = digitalRead(pullPin);

    if (pullState == LOW) {
        pressRead++;
    }

    if (pushState == LOW) {
        autoRead++;
    }

    if (pressRead) {
        Serial.print("press: ");
        Serial.println(pressRead);
    }


    if (autoRead) {
        Serial.print("auto: ");
        Serial.println(autoRead);
    }

*/
}



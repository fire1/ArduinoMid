
#include <Arduino.h>
#include <Wire.h>

void setup() {
//    Wire.begin(8);                // join i2c bus with address #8
    Wire.begin();                // join i2c bus with address #8
    Wire.onReceive(receiveEvent); // register event
    Serial.begin(250000);           // start serial for output
}

void loop() {
    delay(100);
}

// function that executes whenever data is received from master
// this function is registered as an event, see begin()
void receiveEvent(int howMany) {
    while (1 < Wire.available()) { // loop through all but the last
        char c = Wire.read(); // receive byte as a character
        Serial.print(c);         // print the character
    }
    int x = Wire.read();    // receive byte as an integer
    Serial.println(x);         // print the integer
}
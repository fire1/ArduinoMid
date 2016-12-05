#include <Boards.h>
#include <Arduino.h>
#include <Firmata.h>


//
// 4 Pins LPG fuel switch/gauge
//      Two wires are for power supply, other two wires is for displayed information.
//      * Check wiring diagram in order to determine wiring
const uint8_t LPG_DAT_PIN = A5;     //  None        Data Line   Tank fuel level
const uint8_t LPG_CLC_PIN = A4;     //  None        Fuel switcher



int analogInputsToReport = 0; // bitwise array to store pin reporting
int analogPin = LPG_DAT_PIN; // counter for reading analog pins
/* timer variables */
unsigned long currentMillis;     // store the current value from millis()
unsigned long previousMillis;    // for comparison with currentMillis


void analogWriteCallback(byte pin, int value) {
    pinMode(pin, OUTPUT);
    analogWrite(pin, value);
}

void setup() {
    Firmata.setFirmwareVersion(FIRMATA_MAJOR_VERSION, FIRMATA_MINOR_VERSION);
    Firmata.attach(ANALOG_MESSAGE, analogWriteCallback);
    Firmata.begin();
//    Firmata.begin(57600);
}

void loop() {
    while (Firmata.available())
        Firmata.processInput();
    currentMillis = millis();
    if (currentMillis - previousMillis > 20) {
        previousMillis += 20;                   // run this every 20ms
        for (analogPin = 0; analogPin < TOTAL_ANALOG_PINS; analogPin++) {
            if (analogInputsToReport & (1 << analogPin))
                Firmata.sendAnalog(analogPin, analogRead(analogPin));
        }
    }
}



#include <Arduino.h>

const uint8_t pinInput = 17;

boolean serial2Low = false;
boolean Serial2High = true;
uint8_t serial2Index;
uint16_t serial2Length;
unsigned long serial2Timing;
//char dataBuff[] = {};
uint32_t dataBuff = 0;

void serialEvent2_() {


    if (digitalRead(pinInput) == LOW) {
        serial2Length = (uint8_t) (millis() - serial2Timing); //if it is low, end the time
        serial2Timing = millis();


        if (serial2Length <= 6) {
            dataBuff |= 0 << serial2Index;
//            dataBuff[serial2Index] = 0;
            serial2Index++;
        }

        if (serial2Length > 6 && serial2Length <= 8) {
            dataBuff |= 1 << serial2Index;
//            dataBuff[serial2Index] = 1;
            serial2Index++;
        }


    }
    // LPG
    // 2221 2201
    // 101000000001 101000000001

    // BNZ
    // 1501 1521 C01 1C00 201
    // 110000000001




}

byte serial2DataBuffer[16] = {};
boolean serial2EventStart = false;
void serialEvent2() {

    while (Serial2.available()) {
        // get the new byte:
        Serial2.readBytes(serial2DataBuffer, 15);
    }
    serial2EventStart = true;
}


void setup() {
    pinMode(pinInput, INPUT);

//    attachInterrupt(digitalPinToInterrupt(pinInput), serialEvent2, FALLING );
    Serial.begin(115200);
    Serial.println("Beginning ... ");
    Serial2.begin(400);
}


void loop() {
//    serialEvent2();
    if (serial2EventStart) {
        Serial.print(F("BIN "));
        for (uint8_t i = 0; i < 15; ++i) {
            Serial.print(serial2DataBuffer[i], BIN);
            serial2DataBuffer[i] = 0;
        }


        Serial.println();

        serial2EventStart = false;
    }
}
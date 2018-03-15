
#include <Arduino.h>

#define BitVal(data, y) ( (data>>y) & 1)

const uint8_t pinInput = 17;

boolean serial2Change = false;
uint8_t serial2Length;
boolean serial2Readed = false;
uint8_t serial2Offset;
unsigned long serial2LastTime;

uint32_t dataBuff;

void serialListener() {

    int pinState = digitalRead(pinInput);


    if (pinState == HIGH) {
        serial2Change = true;
        serial2Length = 0;
    }

    if (pinState == LOW && serial2Change) {
        serial2Length = (uint8_t) (millis() - serial2LastTime); //if it is low, end the time
        serial2LastTime = millis();
        serial2Change = false;

        if (serial2Length >= 2 && serial2Length <= 6) { // 5ms read
            dataBuff += 0 << serial2Offset;
            serial2Offset++;

        }


        if (serial2Length > 6 && serial2Length <= 8) { // 7ms read
            dataBuff += 1 << serial2Offset;
            serial2Offset++;
        }

        //
        // 110100000001 // act LPG full

        if (serial2Length > 8) { // 15 downs
            serial2Readed = true;
            serial2Length = 0;
            serial2Offset = 0;
        }

    }


}

// LPG
// 2221 2201
// 101000000001 101000000001

// BNZ
// 1501 1521 C01 1C00 201
// 110000000001


void setup() {
    pinMode(pinInput, INPUT);

//    attachInterrupt(digitalPinToInterrupt(pinInput), serialEvent2, FALLING );
    Serial.begin(115200);
    Serial.println("Beginning ... ");
}


void loop() {

    serialListener();

    if (serial2Readed) {
        Serial.println(BitVal(dataBuff, 0));
        Serial.print(F("BIN "));
        Serial.print(dataBuff, BIN);
        dataBuff = 0;
        Serial.println();
        serial2Readed = false;
    }
}
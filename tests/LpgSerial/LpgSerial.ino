
#include <Arduino.h>


const uint8_t pinInput = 17;

uint8_t serial2Length;
boolean serial2Readed = true;
uint8_t serial2Offset;
unsigned long serial2LastTime;

byte dataBuff[] = {};

void serialListener() {

        if (digitalRead(pinInput) == LOW) {
            serial2Length = (uint8_t)(millis() - serial2LastTime); //if it is low, end the time
            serial2LastTime = millis();


            if (serial2Length >= 2 && serial2Length <= 6) { // 5ms read
                dataBuff[serial2Offset] = 0;
                serial2Offset++;
            }


            if (serial2Length > 6 && serial2Length <= 8) { // 7ms read
                dataBuff[serial2Offset] = 1;
                serial2Offset++;
            }

            //
            // Error
            if(serial2Length > 10){
                serial2Offset = 0;
            }

        }
    if(serial2Offset > 14) { // 15 downs
        serial2Readed = true;
        serial2Length = 0;
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
    Serial2.begin(550);
}


void loop() {

    serialListener();

    if (serial2Readed) {
        Serial.print(F("BIN "));
        for (uint8_t i = 0; i < 15; ++i) {
            Serial.print(dataBuff[i], BIN);
            dataBuff[i] = 0;
        }
        Serial.println();
        serial2Readed = false;
    }
}


#include <Arduino.h>


const uint8_t pinLpgDat = A1;     //  [brown]     Switch DATA     Tank fuel level     /// A8
const uint8_t pinLpgClc = A2;     //  [blue]      Switch button   Fuel switcher       /// A9

#define LPG_SENS_MESSAGE_LENGTH 8


void setup() {
    Serial.begin(250000);

    Serial.println("Start LPG listening .... ");
};

boolean startReceiving = false;
int indexReceiving = 0;
uint8_t receivedBuffer, receivingData;

void loop() {


    if (digitalRead(pinLpgClc) == LOW && !startReceiving) {
        startReceiving = true;
    }


    if (indexReceiving < LPG_SENS_MESSAGE_LENGTH && startReceiving) {
        receivedBuffer |= digitalRead(pinLpgDat) << indexReceiving;
    }

    indexReceiving++;

    if (indexReceiving > LPG_SENS_MESSAGE_LENGTH) {
        receivingData = receivedBuffer;
        receivedBuffer = 0;
        indexReceiving = 0;
        startReceiving = 0;
    }


    if (receivingData && receivingData < 255) {
        Serial.println(receivingData, BIN);
        receivingData = 0;
    }

};

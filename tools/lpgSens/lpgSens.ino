

#include <Arduino.h>
#include "LpgSens.h"

const uint8_t LPG_DAT_PIN = A11;     //  [brown]     Switch DATA     Tank fuel level     /// A8
const uint8_t LPG_CLC_PIN = A12;     //  [blue]      Switch button   Fuel switcher       /// A9



void setup() {
    Serial.begin(250000);

};

boolean startReceiving = false;
int indexReceving = 0;
uint8_t receivedData;

void loop() {


    if (digitalRead(LPG_DAT_PIN) == HIGH) {
        receivedData |= digitalRead(LPG_CLC_PIN) << indexReceving;
        indexReceving++;

    }

    if (indexReceving >= 8 && receivedData < 255 && receivedData > 0) {
        Serial.println(receivedData);
        receivedData = 0;
        indexReceving = 0;
    }

};


/*
 boolean startReceiving = false;
int indexReceving = 0;

void loop() {

    if(!digitalRead(LPG_DAT_PIN) && !digitalRead(LPG_CLC_PIN)){
        startReceiving = true;
        indexReceving= 0;
    }

};
 */
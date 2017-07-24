
#include "LpgSens.h"


#define LPG_SENS_MESSAGE_LENGTH 8




//Note:
//        Not all pins on the Mega and Mega 2560 support change interrupts,
//so only the following can be usedMenu for RX:
// 10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69


uint8_t shiftListenIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder) {
    uint8_t value = 0;
    uint8_t i;

    for (i = 0; i < 8; ++i) {
        digitalWrite(clockPin, HIGH);
        if (bitOrder == LSBFIRST)
            value |= digitalRead(dataPin) << i;
        else
            value |= digitalRead(dataPin) << (7 - i);
        digitalWrite(clockPin, LOW);
    }
    return value;
}

LpgSens lpg(A1);

void setup() {
    Serial.begin(115200);
};


void loop() {
    if (lpg.available()) {
        Serial.println(lpg.read());
    }
};

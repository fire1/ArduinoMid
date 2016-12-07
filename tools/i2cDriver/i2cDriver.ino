


#ifndef ARDUINO_I2C_FUEL_DETECTION_SWITCH
#define ARDUINO_I2C_FUEL_DETECTION_SWITCH

#include <Arduino.h>
#include "../../MID/drivers/I2cSimpleListener.h"

const uint8_t LPG_DAT_PIN = A5;     //  [brown]     Switch DATA     Tank fuel level
const uint8_t LPG_CLC_PIN = A4;     //  [blue]      Switch button   Fuel switcher


I2cSimpleListener i2c(LPG_DAT_PIN, LPG_CLC_PIN);

void setup() {

    Serial.begin(9600);
}

void loop() {

    uint8_t value = i2c.listen();
    if (value) {
        Serial.println(value);
    }


}

#endif
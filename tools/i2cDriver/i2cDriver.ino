


#ifndef ARDUINO_I2C_FUEL_DETECTION_SWITCH
#define ARDUINO_I2C_FUEL_DETECTION_SWITCH

#include <Arduino.h>


const uint8_t LPG_DAT_PIN = A5;     //  [brown]     Switch DATA     Tank fuel level
const uint8_t LPG_CLC_PIN = A4;     //  [blue]      Switch button   Fuel switcher

#define SCL_PIN  LPG_CLC_PIN
#define SCL_PORT PORTD
#define SDA_PIN  LPG_DAT_PIN
#define SDA_PORT PORTC
//#define I2C_SLOWMODE 1 // todo

#include <SoftI2CMaster.h>


void setup() {

    Serial.begin(9600);
}


void loop() {

    uint8_t value = i2c_read(false);
    if (value) {
        Serial.println(value);
    }


}

#endif
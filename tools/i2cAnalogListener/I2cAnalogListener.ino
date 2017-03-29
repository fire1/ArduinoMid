#include <Arduino.h>

#ifndef I2C_ADT_VAL
#define I2C_ADT_VAL 1020
#endif

#ifndef I2C_ACL_VAL
#define I2C_ACL_VAL 1020
#endif

class I2cAnalogListener {

public:
/**
 *
 */
    I2cAnalogListener(uint8_t analogDataPin, uint8_t analogClockPin, uint8_t _bitOrder = LSBFIRST) {
        dataPin = analogClockPin;
        clockPin = analogClockPin;
        bitOrder = _bitOrder;
    }

    void begin() {
        pinMode(dataPin, INPUT_PULLUP);
        pinMode(clockPin, INPUT_PULLUP);
    }

/**
 *
 */
    uint8_t listen() {
        uint8_t value = 0;
        uint8_t index;
        for (index = 0; index < 8; ++index) {
            int clock = analogRead(clockPin);
            if (clock > I2C_ACL_VAL && clock > lastClock) {
                value = read(index, value);
                lastClock = clock;
            } else {
                lastClock = clock;
            }
        }
        return value;
    }

protected:
    int lastClock;
    uint8_t clockPin, dataPin;
    uint8_t bitOrder;

/**
 *
 */
    uint8_t read(uint8_t index, uint8_t value) {

        bool data;
        if (analogRead(dataPin) > I2C_ADT_VAL)
            data = 1;
        else
            data = 0;


        if (bitOrder == LSBFIRST)
            value |= data << index;
        else
            value |= data << (7 - index);

        return value;
    }

};

//
// Test

I2cAnalogListener i2c(A4, A5);

void setup() {
    Serial.begin(115200);
    i2c.begin();

}

void loop() {
    Serial.println(i2c.listen());
}
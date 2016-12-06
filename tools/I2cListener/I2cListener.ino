

#include <Arduino.h>


class I2cListener {

private:
    //
    // Pins containers
    uint8_t _sclPin, _sdaPin;
    //
    // Mask container
    uint8_t _sclBitMask, _sdaBitMask;


    volatile uint8_t *_sclPortReg;
    volatile uint8_t *_sdaPortReg;
    volatile uint8_t *_sclDirReg;
    volatile uint8_t *_sdaDirReg;


protected:

    uint8_t i2cRead(void);

    uint8_t i2cReadBit(void);

public:
    I2cListener(void);

    I2cListener(uint8_t sclPin, uint8_t sdaPin);

    void setPins(uint8_t sdaPin, uint8_t sclPin);

    uint8_t listen(void);

};
/* ================================================================
 * Cpp part of file
 * ================================================================
 */
/**
 * Constructor
 */
I2cListener::I2cListener(void) {
    // do nothing, use setPins() later
}

/**
 *
 * @param sclPin
 * @param sdaPin
 */
I2cListener::I2cListener(uint8_t sclPin, uint8_t sdaPin) {
    setPins(sclPin, sdaPin);
}

/**
 * Sets pin as inputs
 * @param sdaPin
 * @param sclPin
 */
void I2cListener::setPins(uint8_t sdaPin, uint8_t sclPin) {
    uint8_t port;


    _sclPin = sclPin;
    _sdaPin = sdaPin;

    _sclBitMask = digitalPinToBitMask(sclPin);
    _sdaBitMask = digitalPinToBitMask(sdaPin);

    port = digitalPinToPort(sclPin);
    _sclPortReg = portInputRegister(port);
    _sclDirReg = portModeRegister(port);

}

// read a byte from the I2C slave device
//
uint8_t I2cListener::i2cRead(void) {
    uint8_t res = 0;

    for (uint8_t i = 0; i < 8; i++) {
        res <<= 1;
        res |= i2cReadBit();
    }


    return res;
}

/**
 * Reads message when is coming
 * @return bits
 */
uint8_t I2cListener::listen() {
    if (!digitalRead(_sclPin))
        return i2cRead();
}
/**
 * Reads single bit
 * @return uint8_t
 */
uint8_t I2cListener::i2cReadBit(void) {
    uint8_t port = digitalPinToPort(_sdaPin);
    volatile uint8_t *pinReg = portInputRegister(port);
    uint8_t c = *pinReg;  // I2C_PIN;
    return (c & _sdaBitMask) ? 1 : 0;
}



I2cListener i2c(A5,A4);

void setup() {
    Serial.begin(9600);           // start serial for output
}

void loop() {
    uint8_t val = i2c.listen();
    if(val)
        Serial.println(val);
}












// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
    while (1 < Wire.available()) { // loop through all but the last
        char c = Wire.read(); // receive byte as a character
        Serial.print(c);         // print the character
    }
    int x = Wire.read();    // receive byte as an integer
    Serial.println(x);         // print the integer
}
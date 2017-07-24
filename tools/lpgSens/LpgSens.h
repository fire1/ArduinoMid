//
// Created by Angel Zaprianov on 5.12.2016 г..
//

#ifndef ARDUINO_I2C_FUEL_DETECTION_SWITCH
#define ARDUINO_I2C_FUEL_DETECTION_SWITCH


#include <Arduino.h>

#ifndef LPG_RECEIVE_BUFFER_LENGTH
#define LPG_RECEIVE_BUFFER_LENGTH 7
#endif


/**
 *
 */
class LpgSens {

private:
    boolean transmissionBegin;
    uint8_t _pinDt, _pinBt;
    uint8_t receiveBuffer;
    uint8_t receivedValue;


    int receiveBufferIndex;

    unsigned long lowStart = 0;
    unsigned long highStart = 0;
//    CarSens *car;
protected:

    uint8_t lowTime(uint8_t pin) {
        if (lowStart == 0 && digitalRead(pin) == LOW) {
            lowStart = millis();
        }

        if (lowStart > 0 && digitalRead(pin) == HIGH) {
            uint8_t diff = millis() - lowStart;
            lowStart = 0;
            return diff;
        }
    }

    uint8_t highTime(uint8_t pin) {
        if (highStart == 0 && digitalRead(pin) == LOW) {
            highStart = millis();
        }

        if (highStart > 0 && digitalRead(pin) == HIGH) {
            uint8_t diff = millis() - highStart;
            highStart = 0;
            return diff;
        }
    }

    void addBitHigh() {
        this->receiveBuffer |= 1 << this->receiveBufferIndex;
        ++this->receiveBufferIndex;
    }

    void addBitLow() {
        this->receiveBuffer |= 0 << this->receiveBufferIndex;
        ++this->receiveBufferIndex;
    }

public:

    LpgSens(uint8_t dataPin) {
        _pinDt = dataPin;
    }

    LpgSens(uint8_t dataPin, uint8_t buttonPin) {
        _pinDt = dataPin;
        _pinBt = buttonPin;
    }


/**
 * Listen communication
 */
    void listener() {
        //
        // Just shorting state var.
        boolean trans = this->transmissionBegin;
        if (lowTime(_pinDt) >= 3) {
            addBitLow();
        }


        if (trans && highTime(_pinDt) == 2) {
            addBitHigh();
        }
        if (trans && highTime(_pinDt) == 4) {
            addBitHigh();
            addBitHigh();
        }
        if (trans && highTime(_pinDt) == 6) {
            addBitHigh();
            addBitHigh();
            addBitHigh();
        }

        if (trans && this->receiveBufferIndex >= LPG_RECEIVE_BUFFER_LENGTH) {
            this->transmissionBegin = false;
            this->receivedValue = this->receiveBuffer;
            this->receiveBuffer = 0;
        }

        if (lowTime(_pinDt) >= 3) {
            this->transmissionBegin = true;
        }

    }

/**
 * Returns transmition value and clear buffer
 */
    uint8_t read() {
        if (this->receivedValue) {
            uint8_t value = this->receivedValue;
            this->receivedValue = 0;
            return value;
        }
    }


    boolean available() {
        return this->receivedValue > 0 ? true : false;
    }


    /**
     * Gets data from transmition
     * @return
     */
    uint8_t getTrnsition(void) {
        return this->receivedValue;
    }


};


#endif //ARDUINOMID_LPGSENS_H

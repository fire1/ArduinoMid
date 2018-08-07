//
// Created by Angel Zaprianov on 2018-08-07.
//

#ifndef ARDUINOMID_LPGSERIAL_H_H
#define ARDUINOMID_LPGSERIAL_H_H


#define LPG_TOLERANCE 2 // tolerance time

unsigned volatile lpgLastRise = 0;
uint8_t volatile interval;

class LpgSerial {


    boolean lowEnds = true, receive = false, transEnd = false;
    uint8_t sync = 0, offset = 0;
    int lastState = LOW;
    unsigned long lowBegin = 0;
    unsigned long buffer, transmission;


private:
    boolean isStart() {
        return lastState == HIGH;
    }


    uint8_t getInputLen() {
        unsigned long pulseLen = 0;
        int in = digitalRead(LPG_INPUT);


        if (lowBegin > 0 && in == LOW && isStart()) {
            pulseLen = millis() - lowBegin;
            lowBegin = 0;
        }

        if (lowBegin == 0 && in == LOW && isStart()) {
            lowBegin = millis();
            transEnd = false;
        }

        if (pulseLen > 15) {
            transEnd = true;
            return 0;
        }


        lastState = in;
        return (uint8_t) pulseLen;
    }

public:

    void begin() {
        pinMode(LPG_INPUT, INPUT);
    }


    void listener() {
        uint8_t len = getInputLen();
        if (offset == 0 && receive != 0) {
            buffer = '\0';
            receive = false;
        }

        if (offset > LPG_BITS || transEnd) {
            transEnd = false;
            offset = 0;
            receive = true;
            transmission = buffer;
        }
        if (len == 0) {
            return;
        }

        if (len > 0) {
            Serial.print(len);
        }
        if (len >= LPG_BYTE_HIGH && len <= (LPG_BYTE_HIGH + LPG_TOLERANCE)) {
            buffer |= 1 << offset;
            offset++;
        } else if (len > 0 && len < LPG_BYTE_HIGH) {
            buffer |= 0 << offset;
            offset++;
        }


    }

    boolean isReceive() {
        return receive;
    }

    uint32_t getData() {
        return transmission;
    }


};

#endif //ARDUINOMID_LPGSERIAL_H_H

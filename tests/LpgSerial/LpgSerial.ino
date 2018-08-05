
#include <Arduino.h>

#define LPG_INPUT 19
#define LPG_TIMEOUT  8
#define LPG_BITS 14
#define LPG_BYTE_HIGH 6 // time for HIGH 1 byte
#define LPG_TOLERANCE 1 // tolerance time

class LpgSerial {


    boolean lowEnds = true, receive = false;
    uint8_t sync = 0, offset = 0;
    int lastState = LOW;
    unsigned long lowBegin = 0;
    uint32_t buffer, transmission;


private:
    boolean isStart() {
        return lastState == HIGH;
    }


    uint8_t getInputLen() {
        unsigned long highLen = 0;
        int in = digitalRead(LPG_INPUT);

        if (lowBegin == 0 && in == LOW && isStart()) {
            lowBegin = millis();
            lowEnds = false;
        }

        if (!lowEnds && in == LOW && isStart()) {
            highLen = millis() - lowBegin;
        }

        if (highLen > LPG_TIMEOUT) {
            return 0;
        }

        lastState = in;
        return (uint8_t) highLen;
    }

public:

    void begin() {
//        if (sync == 0) {
//            sync = uint8_t(pulseIn(LPG_INPUT, HIGH, LPG_TIMEOUT) / 1000);
//        }
    }

    void listener() {
        uint8_t len = getInputLen();

        if (offset == 0 && receive != 0) {
            buffer = '\0';
            receive = false;
        }

        if (len > LPG_BYTE_HIGH && len < (LPG_BYTE_HIGH + LPG_TOLERANCE)) {
            buffer += B1;
            offset++;
        } else {
            buffer += B0;
            offset++;
        }

        if (offset >= LPG_BITS) {
            offset = 0;
            receive = true;
            transmission = buffer;
        }


    }

    boolean isReceive() {
        return receive;
    }

    uint32_t getData() {
        return transmission;
    }


};

// LPG
// 2221 2201
// 101000000001 101000000001

// BNZ
// 1501 1521 C01 1C00 201
// 110000000001

LpgSerial lpg;

void setup() {
    lpg.begin();
}


void loop() {
    lpg.listener();
    if (lpg.isReceive()) {
        Serial.println(lpg.getData(), BIN);
    }
}
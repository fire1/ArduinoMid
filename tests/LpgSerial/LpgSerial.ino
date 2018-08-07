
#include <Arduino.h>

#define LPG_INPUT 17
#define LPG_TIMEOUT  12
#define LPG_BITS 13
#define LPG_BYTE_HIGH 6 // time for HIGH 1 byte
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

// LPG
// 2221 2201
// 101000000001 101000000001

// BNZ
// 1501 1521 C01 1C00 201
// 110000000001

LpgSerial lpg;


void setup() {
    Serial.begin(115200);
    Serial2.begin(180, SERIAL_7N2);
    lpg.begin();
}

boolean lpgReceive = false;
uint8_t lpgOffset = 0;
uint32_t lpgBuffer = '\0',lpgData ='\0';
void serialEvent2() {

    uint16_t width = pulseIn(LPG_INPUT, HIGH, 14000);
//    Serial.println(width);
//    return;
    if (width == 0) {
     if(lpgBuffer){
         lpgOffset = 0;
         lpgData = lpgBuffer;
         lpgBuffer = '\0';
         lpgReceive = true;
     }else{
         lpgData = '\0';
         lpgReceive = false;
     }
        return;
    }

    if (width > 3000 && width < 5000) {
        lpgBuffer |= 1 << lpgOffset;
        lpgOffset++;
    }else{
        lpgBuffer |= 0 << lpgOffset;
        lpgOffset++;
    }

}

void loop() {
//    lpg.listener();
//    if (lpg.isReceive()) {
//        Serial.println();
//        Serial.println(lpg.getData(), BIN);
//    }
//
   if (lpgReceive) {
        Serial.println();
        Serial.println(lpgData, BIN);
    }


}
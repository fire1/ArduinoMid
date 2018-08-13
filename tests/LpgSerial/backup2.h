
#include <Arduino.h>


/**
 *  ArduinoMid tutorial to providing LPG switch signal.
 * In order to capture signals from LPG ECU to digital"Switch Unit"
 * check 4 wires for voltages with multimeter.
 * Two of them needs to provide power supply for switch,
 * other two needs to  provide data and button press sensing.
 *
 * Data signal wire will have some voltage drop over time cycle (max voltage needs to be 5V),
 * when you find this wire run this script with enabled (uncomment) <LPG_TIME_SENS> to
 * determinate data pulse time size.
 *
 * Pulse sizes need to be two types (one for logic "1" and one for logic "0").
 * Define size only for "logic 1" pulse in maximum tolerance.
 *
 *  Example:
 *
        0
        0
        3982 // logic 1
        1993 // logic 0
        1993
        1993
        1994
        3978 // logic 1
        1993
        1993
        3978
        1994
        1993
        1995
        3979 // logic 1
        3978 // logic 1
        0
        0
        0
        ...
 *
 *     Since logic 0 have maximum pulse time of ~2000
 *     for logic 1 pulse time will be from 3500 to 4500
 *
 */

//#define LPG_TIME_SENS           // Uncomment to see pulses width in microseconds

//
// Basic settings
#ifndef LPG_INPUT
#define LPG_INPUT 17            // Input pin
#endif
#ifndef LPG_TIMEOUT
#define LPG_TIMEOUT  14000      // Timeout between data signals
#endif
#ifndef LPG_BITS
#define LPG_BITS 13             // Bits to count
#endif
#ifndef LPG_BYTE_HIGH_MIN
#define LPG_BYTE_HIGH_MIN 3500  // Minimum starting time for HIGH state (1 bit)
#endif
#ifndef LPG_BYTE_HIGH_MAX
#define LPG_BYTE_HIGH_MAX 4500  // Maximum starting time for HIGH state (1 bit)
#endif
#ifndef LPG_BAUD_RATE
#define LPG_BAUD_RATE 180       // Baud rate of serial2
#endif




//
// Public vars used:
bool lpg_isReceive;
uint8_t lpg_dataOffset;
uint32_t lpg_dataBuffer, lpg_recData;

/**
 * Capturing event
*/



class LpgSens {


    uint8_t ending = 0;
    uint16_t time;

    boolean lowEnds = true, receive = false;
    uint8_t sync = 0, offset = 0;
    boolean lastState = 0;
    unsigned long lowBegin = 0;
    unsigned long buffer, transmission;

private:
    boolean isStart() {
        return lastState == 1;
    }

public:
    volatile uint8_t *port;
    uint8_t mask;
    volatile uint16_t width;

    void begin() {
        Serial2.begin(LPG_BAUD_RATE, SERIAL_7N2);
        Serial2.setTimeout(10);

        // do this once at setup
        mask = digitalPinToBitMask(LPG_INPUT);
        port = portInputRegister(digitalPinToPort(LPG_INPUT));
    }


    void count() {
        ++width;
        if (width > 10000) width = 0;
    }

    inline void end() {
        if (width > 0) {
            time = clockCyclesToMicroseconds(width * 16 + 16);
        }
        width = 0;
    }

    uint16_t getInputLen(boolean in) {
        unsigned long highLen = 0;


        if (lowBegin > 0 && in == 0 && isStart()) {
            highLen = micros() - lowBegin;
            lowBegin = 0;
            lowEnds = true;
        }

        if (lowBegin == 0 && in == 0 && isStart()) {
            lowBegin = micros();
            lowEnds = false;
        }


        if (highLen > LPG_TIMEOUT) {
            return 0;
        }


        lastState = in;
        return (uint16_t) highLen;
    }

    uint16_t getTime() {
        uint16_t old = time;
        time = 0;
        return old;
    }

    boolean isReceive() {
        return lpg_isReceive;
    }

    uint32_t getData() {
        return lpg_recData;
    }


};

LpgSens lpg;
uint16_t timer;

void serialEvent2() {


    if (Serial2.available()) {
        // read the pin

        timer = lpg.getInputLen((*lpg.port & lpg.mask) != 0);



//        uint16_t width = pulseInto(LPG_INPUT, HIGH, LPG_TIMEOUT);
//
//#ifdef LPG_TIME_SENS
//        Serial.println(width);
//        return;
//#endif
//        if (width == 0) {
//            if (lpg_dataBuffer) {
//                lpg_dataOffset = 0;
//                lpg_recData = lpg_dataBuffer;
//                lpg_dataBuffer = '\0';
//                lpg_isReceive = true;
//            } else {
//                lpg_isReceive = false;
//            }
//            return;
//        }
//
//        if (width > LPG_BYTE_HIGH_MIN && width < LPG_BYTE_HIGH_MAX) {
//            lpg_dataBuffer |= 1 << lpg_dataOffset;
//            lpg_dataOffset++;
//        } else {
//            lpg_dataBuffer |= 0 << lpg_dataOffset;
//            lpg_dataOffset++;
//        }
    }
}

//
// Output
void setup() {
    Serial.begin(115200);
    lpg.begin();
}

unsigned long loopCounter;

void loop() {

    if (loopCounter % 1000 == 0) {
        Serial.print(" Loop offset ");
        Serial.print(loopCounter);
        Serial.print(" DAta offset ");
        Serial.print(timer);
        Serial.println();

    }


    if (lpg_isReceive) {
        //
        // NOTE: original send 10000100100011 will be inverted in receive
        Serial.println();
        Serial.print(" Data output ");
        Serial.println(lpg_recData, BIN);
        Serial.print(" && bit 13 is: ");
        Serial.println(bitRead(lpg_recData, 13));
    }
    loopCounter++;
}
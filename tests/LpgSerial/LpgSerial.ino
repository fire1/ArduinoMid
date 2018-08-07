
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
void serialEvent2() {

    if (Serial2.available()) {
        uint16_t width = pulseIn(LPG_INPUT, HIGH, LPG_TIMEOUT);

#ifdef LPG_TIME_SENS
        Serial.println(width);
        return;
#endif
        if (width == 0) {
            if (lpg_dataBuffer) {
                lpg_dataOffset = 0;
                lpg_recData = lpg_dataBuffer;
                lpg_dataBuffer = '\0';
                lpg_isReceive = true;
            } else {
                lpg_isReceive = false;
            }
            return;
        }

        if (width > LPG_BYTE_HIGH_MIN && width < LPG_BYTE_HIGH_MAX) {
            lpg_dataBuffer |= 1 << lpg_dataOffset;
            lpg_dataOffset++;
        } else {
            lpg_dataBuffer |= 0 << lpg_dataOffset;
            lpg_dataOffset++;
        }
    }
}


class LpgSens {

public:


    void begin() { Serial2.begin(LPG_BAUD_RATE, SERIAL_7N2); }

    boolean isReceive() {
        return lpg_isReceive;
    }

    uint32_t getData() {
        return lpg_recData;
    }


};

LpgSens lpg;


//
// Output
void setup() {
    Serial.begin(115200);
    lpg.begin();
}

unsigned long loopCounter;
void loop() {

    if (loopCounter % 500 == 0) {
        Serial.print("Loop offset ");
        Serial.print(loopCounter);
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

#include <Arduino.h>


/**
 *  ArduinoMid tutorial to providing LPG switch signal.
 * In order to capture signals from LPG ECU to digital"Switch Unit"
 * check 4 wires for voltages with multimeter.
 * Two of them needs to provide power supply for switch,
 * other two needs to  provide data and button press sensing.
 *
 * Data signal wire will have some voltage drop over time cycle,
 * when you find this wire run this script with enabled (uncomment) <LPG_TIME_SENS> to determinate data input.
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
#define LPG_BYTE_HIGH_MIN 3000  // Minimum starting time for HIGH state (1 bit)
#endif
#ifndef LPG_BYTE_HIGH_MAX
#define LPG_BYTE_HIGH_MAX 5000  // Maximum starting time for HIGH state (1 bit)
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
            lpg_recData = '\0';
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


void loop() {

    if (lpg_isReceive) {
        Serial.println();
        Serial.println(lpg_recData, BIN);
        Serial.print("13 bit is: ");
        Serial.println(bitRead(lpg_recData, 13));

    }
}
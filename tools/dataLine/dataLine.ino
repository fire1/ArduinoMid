#include <Arduino.h>
#include <OneWire.h>

//
// 4 Pins LPG fuel switch/gauge
//      Two wires are for power supply, other two wires is for displayed information.
//      * Check wiring diagram in order to determine wiring
const uint8_t LPG_LVL_PIN = A5;     //  None        Data Line   Tank fuel level
const uint8_t LPG_SWT_PIN = A4;     //  None        Fuel switcher


OneWire ds(LPG_LVL_PIN);

static void discoverOneWireDevices(void);

void setup(void) {
    Serial.begin(9600);
    discoverOneWireDevices();
}

static void discoverOneWireDevices(void) {
    byte i;
    byte present = 0;
    byte data[12];
    byte addr[8];

    Serial.print("Looking for 1-Wire devices...\n\r");
    while(ds.search(addr)) {
        Serial.print("\n\rFound \'1-Wire\' device with address:\n\r");
        for( i = 0; i < 8; i++) {
            Serial.print("0x");
            if (addr[i] < 16) {
                Serial.print('0');
            }
            Serial.print(addr[i], HEX);
            if (i < 7) {
                Serial.print(", ");
            }
        }
        if ( OneWire::crc8( addr, 7) != addr[7]) {
            Serial.print("CRC is not valid!\n");
            return;
        }
    }
    Serial.print("\n\r\n\rThat's it.\r\n");
    ds.reset_search();
    return;
}

void loop(void) {
    // nothing to see here
}


//
// Created by Angel Zaprianov on 10.2.2017 г..
//

#ifndef ARDUINOMID_CONF_H
#define ARDUINOMID_CONF_H
//
// Sets screen size
#define SCREEN 24064 // Glcd 240x64
//#define SCREEN 162 // lcd 16x2
//
// Serial configuration
#define SERIAL_INJECT_DATA          // Inject data from serial monitor
#define SERIAL_MONITOR_BAUD 115200  // 115200 - Normal Speed of monitoring messages

//
// Time to wait when holding >R button
#define AWAITING_HOLD_BTN 2000
//
// Uncommented to debug basics
//#define GLOBAL_SENS_DEBUG
//
// Sensing instrument interval time
#define MILLIS_SENS 200
//
// Time information
#define MILLIS_PER_HR    3600000    // Hour
#define MILLIS_PER_MN    60000      // Minute
#define MILLIS_PER_SC    1000       // Second
//
// From this distance will trigger
// menu for saving trip
const uint8_t SHUTDOWN_SAVE_TRIP PROGMEM = 25; // more than 25km
//
// MID plug pins definition over Arduino
//
// Define button pins for steering controller
const uint8_t BTN_PIN_UP PROGMEM = 8;       //  Plug:23     Column switch
const uint8_t BTN_PIN_DW PROGMEM = 9;       //  Plug:24     Column switch navigation
//
// Shutdown protection pin
const uint8_t SAV_PIN_CTR PROGMEM = A6;     //  Plug:4      Disconnect supply voltage
const uint8_t SAV_PIN_DTC PROGMEM = A7;     //  Plug:16     Detect ignition key off state
//
// Engine pins
const uint8_t ENG_CLT_PIN PROGMEM = A0;     //  Plug:31     Engine Temp.  [may be 50uf capacitor]
const uint8_t BRK_LGH_PIN PROGMEM = 11;     //  Plug:       Brake light detection
const uint8_t RPM_SNS_PIN PROGMEM = 2;      //  Plug:6      RPM [attachInterrupt]
const uint8_t SPD_SNS_PIN PROGMEM = 3;      //  Plug:12     Speed sensor hub [attachInterrupt]
const uint8_t ECU_SGN_PIN PROGMEM = 19;     //  Plug:27     ECU  signal
//
// Car state pins
const uint8_t STT_BRK_PIN PROGMEM = A4;     //  Plug        Critical Brake ware
const uint8_t STT_CLN_PIN PROGMEM = A2;     //  Plug        Critical Coolant level
const uint8_t STT_OIL_PIN PROGMEM = A3;     //  Plug        Critical oil level
const uint8_t STT_WNW_PIN PROGMEM = A5;     //  Plug        Critical window washer
const uint8_t STT_VLT_PIN PROGMEM = A7;     // Duplicating  SAV_PIN_DTC


//
// Display pins map
//  NOTE: pins are inverted due to sockets
//
//  General pins
const uint8_t DSP_PIN_GD1 PROGMEM = 26; // Power GND
const uint8_t DSP_PIN_GD2 PROGMEM = 27; // Power GND
const uint8_t DSP_PIN_VCC PROGMEM = 29; // Power positive
const uint8_t DSP_PIN_WR PROGMEM = 31;  // Write Signal
const uint8_t DSP_PIN_RD PROGMEM = 30;  // Read Signal
const uint8_t DSP_PIN_CE PROGMEM = 33;  // Chip Enable Signal
const uint8_t DSP_PIN_CD PROGMEM = 32;  // Instruction Code
const uint8_t DSP_PIN_FS PROGMEM = 45;  // Font selection
const uint8_t DSP_PIN_RST PROGMEM = 34; // Reset signal
const uint8_t DSP_PIN_LDA PROGMEM = 46; // Blacklight Anode (+5V)
const uint8_t DSP_PIN_LDK PROGMEM = 47; // Blacklight cathode (0v)
//
// Data pins
const uint8_t DSP_PIN_DT1 PROGMEM = 37;
const uint8_t DSP_PIN_DT2 PROGMEM = 36;
const uint8_t DSP_PIN_DT3 PROGMEM = 39;
const uint8_t DSP_PIN_DT4 PROGMEM = 38;
const uint8_t DSP_PIN_DT5 PROGMEM = 41;
const uint8_t DSP_PIN_DT6 PROGMEM = 40;
const uint8_t DSP_PIN_DT7 PROGMEM = 43;
const uint8_t DSP_PIN_DT8 PROGMEM = 42;

//
// Display dim pins
const uint8_t
        DIM_PIN_VAL PROGMEM = A10;    //  Plug:7      Display back-light
#if SCREEN == 162 || !defined(SCREEN)
const uint8_t DIM_PIN_OUT = 46;     //              Output dim of playEntry
#elif SCREEN == 24064
const uint8_t DIM_PIN_OUT PROGMEM = 49;     //              Output dim of playEntry
#endif;
//
// Temperatures
const uint8_t TMP_PIN_OUT PROGMEM = A9;     // Plug:3       External temperature sensor

/* Extras ...   ******/
//
// Alarm / Tone pin
#define TONE_ADT_PIN 11
//
// Alpine / Steering Wheel buttons
const uint8_t ALP_PIN_INP PROGMEM = A8;
const uint8_t ALP_PIN_OUT PROGMEM = 53;
const uint8_t ALP_PIN_VOL PROGMEM = 14; // deprecated

//
// Change state of shutdown "press to save"
const uint8_t SHUTDOWN_SAVE_STATE PROGMEM = LOW;
const uint8_t SHUTDOWN_SAVE_BUTTON PROGMEM = 9;
//
// External chip from eep rom
#define EEP_ROM_ADDRESS 0x50    //Address of 24LC256 eeprom chip
//
//  DallasTemperature
// Additional temperature sensor for
// inside temperature [very cheep and ease to use]
//
#define INSIDE_TEMPERATURE_DS // Comment to disable it
//
// Configuration of oneWire protocol
#if defined(INSIDE_TEMPERATURE_DS)
//
// Defining one wire BUS used for temperature sensor
#define ONE_WIRE_BUS 7 // Pin define
#endif
//
//
//
/***************************************************************************
 * LPG fuel support configuration
 */

//
// In my case  ... about LPG installation:
// My LPG fuel installation is EuropeGas "Avance 32" and communication between LPG ECU and fuel switch is I2C protocol
//  so ... i made simple driver "I2cSimpleListener" to listen communication without make any connection to devices
#define ADT_FUEL_SYSTEM_I2C // comment to disable additional fuel system such as LPG
//
// Include simple driver
#ifdef ADT_FUEL_SYSTEM_I2C

#include "lib/drivers/I2cSimpleListener.h"

#endif
//
// This definition is for carSens class
// Additional fuel installation
#define LPG_INSTALLATION
#ifdef LPG_INSTALLATION
//
// [LPG ECU Avance 32]
// 4 Pins 5V LPG fuel switch/gauge
//      Two wires are for power supply, other two wires is for displayed information.
//      * Check wiring diagram in order to determine your wiring
// 20, 21
//
// bla aaa http://arduino.stackexchange.com/questions/9481/why-is-my-interrupt-code-not-working?answertab=active#tab-top
// Not all pins on the Mega and Mega 2560 support change interrupts, so only the following can be usedMenu for RX:
// 10, 11, 12, 13, 14, 15, 50, 51, 52, 53, A8 (62), A9 (63), A10 (64), A11 (65), A12 (66), A13 (67), A14 (68), A15 (69).
const uint8_t pinLpgDat = A11;     //  [brown]     Switch DATA     Tank fuel level     /// A8
const uint8_t LPG_CLC_PIN = A12;     //  [blue]      Switch button   Fuel switcher       /// A9
#endif
//
/***************************************************************************/
//
//
//
// Global Menu cursor

volatile uint8_t MidCursorMenu = 0;

class LcdMenuInterface {
public:

    virtual void draw() = 0;

    virtual void intro() = 0;

    virtual void begin() = 0;
};

class MidMenuInterface {
public:

    virtual void setup() = 0;

    virtual void menuChanged(MenuChangeEvent change) = 0;

    virtual MenuBackend getMB() = 0;

    virtual void moveUp() = 0;

    virtual void moveDw() = 0;
};


static void menuUseEvent(MenuUseEvent used);

static void menuChangeEvent(MenuChangeEvent changed);

#define MID_VERSION "0.3.5"
//
// Reserving RAM
#include "lib/drivers/ResRam.h"
//
// Init reserving RAM
ResRam resRam;

#endif //ARDUINOMID_CONF_H

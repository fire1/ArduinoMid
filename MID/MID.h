//
// Created by Angel Zaprianov on 10.2.2017 г..
//

#ifndef ARDUINO_MID_H
#define ARDUINO_MID_H

#include <Arduino.h>
#include <MenuBackend.h>
#include "conf.h"
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

//
// From this distance will trigger
// menu for saving trip
const uint8_t SHUTDOWN_SAVE_TRIP = 25; // more than 25km
//
// MID plug pins definition over Arduino
//
// Divider Digital
// [DVD]
// 14.6V+   [PIN]---- [10K] --- [4.7K] --- GND
//                           |
//                          MCU
//
// Divider Analog
//                              .----- [37uF] --- GND
// [DVA]                        |
// 14.6V+   [PIN]-- [5.2K] ----------- [4.7K] --- GND
//                           |    |
//                          MCU [ZNR] ----------- GND
//
//
//      [meg] -> Connection to mega
//      [znr] - Zenner diod 5.1v to GND
//      [dva] - Voltage divider = 5.2k to MID <- to mega ->  4.6k to GND
//
// Define button pins for steering controller
const uint8_t BTN_PIN_UP = 9;       //  Plug:23  [meg] [10k] [+5v]    Column switch
const uint8_t BTN_PIN_DW = 8;       //  Plug:24  [meg] [10k] [+5v]    Column switch navigation
//
// Shutdown protection pin
const uint8_t SAV_PIN_CTR = A6;     //  Plug:4    [to +5v relay]  Disconnect supply voltage
const uint8_t SAV_PIN_DTC = A7;     //  Plug:16   [20k] -> [meg] -> [znr]   Detect ignition key off state
//
// Engine pins
const uint8_t ENG_CLT_PIN = A0;     //  Plug:31     [dva + 50uf] Engine Temperature
const uint8_t BRK_LGH_PIN = 12;     //  Plug:15     [100k] + [znr] Brake light detection
const uint8_t RPM_SNS_PIN = 2;      //  Plug:6      [dvd] RPM [attachInterrupt]
const uint8_t SPD_SNS_PIN = 3;      //  Plug:12     [dvd] Speed sensor hub [attachInterrupt]
const uint8_t ECU_SGN_PIN = 19;     //  Plug:27     [dvd] ECU  signal
//
// Car state pins
const uint8_t STT_UL1_PIN = A11;    //   Plug 17 Unusable for now (do not connect)
const uint8_t STT_UL2_PIN = A12;    //   Plug 18 Unusable for now (do not connect)
const uint8_t STT_BRK_PIN = A13;     //  Plug:19        [meg]    Critical Brake ware
const uint8_t STT_CLN_PIN = A14;     //  Plug:20        [meg]    Critical Coolant level
const uint8_t STT_WNW_PIN = A15;     //  Plug:22        [meg]   Critical window washer
const uint8_t STT_OIL_PIN = A1;     //   Plug:13        [meg]   Critical oil level
const uint8_t STT_VLT_PIN = A7;     //   Duplicating  SAV_PIN_DTC
//
// Display dim pins
const uint8_t DIM_PIN_VAL = A10;    //  Plug:7  [dva + 50uf ]     Display back-light
//
// Temperatures
const uint8_t TMP_PIN_OUT = A9;     // Plug:3     [+5v] [150R]   Front temperature sensor

#if SCREEN == 162 || !defined(SCREEN)
//
// Display pins map
//  NOTE: pins are inverted due to sockets
//
//  General pins
const uint8_t DSP_PIN_GD1 = 26; // Power GND
const uint8_t DSP_PIN_GD2 = 27; // Power GND
const uint8_t DSP_PIN_VCC = 29; // Power positive
const uint8_t DSP_PIN_WR = 31;  // Write Signal
const uint8_t DSP_PIN_RD = 30;  // Read Signal
const uint8_t DSP_PIN_CE = 33;  // Chip Enable Signal
const uint8_t DSP_PIN_CD = 32;  // Instruction Code
const uint8_t DSP_PIN_FS = 45;  // Font selection
const uint8_t DSP_PIN_RST = 34; // Reset signal
const uint8_t DSP_PIN_LDA = 46; // Blacklight Anode (+5V)
const uint8_t DSP_PIN_LDK = 47; // Blacklight cathode (0v)
//
// Data pins
const uint8_t DSP_PIN_DT1 = 37;
const uint8_t DSP_PIN_DT2 = 36;
const uint8_t DSP_PIN_DT3 = 39;
const uint8_t DSP_PIN_DT4 = 38;
const uint8_t DSP_PIN_DT5 = 41;
const uint8_t DSP_PIN_DT6 = 40;
const uint8_t DSP_PIN_DT7 = 43;
const uint8_t DSP_PIN_DT8 = 42;

const uint8_t DIM_PIN_OUT = 46;     //              Output dim of playEntry

#elif SCREEN == 24064
//
// Add language
#include "lib/ui/language/en.h"

//
// Display pins map
//  NOTE: pins are inverted due to LCD sockets
//
//  General pins
const uint8_t DSP_PIN_GD1 = 26; // Power GND
const uint8_t DSP_PIN_GD2 = 27; // Power GND
const uint8_t DSP_PIN_VCC = 29; // Power positive
const uint8_t DSP_PIN_WR = 31;  // Write Signal
const uint8_t DSP_PIN_RD = 30;  // Read Signal
const uint8_t DSP_PIN_CE = 33;  // Chip Enable Signal
const uint8_t DSP_PIN_CD = 32;  // Instruction Code
const uint8_t DSP_PIN_FS = 45;  // Font selection
const uint8_t DSP_PIN_RST = 34; // Reset signal
const uint8_t DSP_PIN_LDA = 46; // Blacklight Anode (+5V)
const uint8_t DSP_PIN_LDK = 47; // Blacklight cathode (0v)
//
// Data pins
const uint8_t DSP_PIN_DT1 = 37;
const uint8_t DSP_PIN_DT2 = 36;
const uint8_t DSP_PIN_DT3 = 39;
const uint8_t DSP_PIN_DT4 = 38;
const uint8_t DSP_PIN_DT5 = 41;
const uint8_t DSP_PIN_DT6 = 40;
const uint8_t DSP_PIN_DT7 = 43;
const uint8_t DSP_PIN_DT8 = 42;

const uint8_t DIM_PIN_OUT = DSP_PIN_LDA;     //              Output dim of playEntry
#endif;


/* Extras ...   ******/
//
// Alarm / Tone pin
#define TONE_ADT_PIN 11
//
// Steering wheel buttons
//
// Digital Potentiometer
// 50k digital potentiometer [MCP41050]
// [meg] 53     to pin [pot] 1 [CS]
// [meg] 52     to pin [pot] 2 [SCK]
// [meg] 51     to pin [pot] 3 [SI]
//     GND      to pin [pot] 4
//     +5V      to pin [pot] 8
//     GND      to pin [pot] 7
// Stereo input to pin [pot] 6 (through  RELAY to stereo remote control)
//                     [pot] 5 NC
// Alpine / Steering Wheel buttons
const uint8_t ALP_PIN_INP = A8; // +5V ->  [240R] to Radio Unit plug pin 1
const uint8_t ALP_PIN_OUT = 53; //  Dig pot signal
const uint8_t ALP_PIN_RLY = 50; //  mg50  ->  1N5406 ->  Unit input ||  5V -> RELAY
// * Signal HIGH will be send from pin 50 to hide dig pot resistance
// * 1N5406 is used in mega pin 50 to block GND signal from pin (only +5V need to be forward )
// * 5V to relay is used as signal cutter when MID is out of power
//
// Change state of shutdown "press to save"
const uint8_t SHUTDOWN_SAVE_STATE = LOW;
const uint8_t SHUTDOWN_SAVE_BUTTON = 9;
//
// External chip from eep rom
//#define EEP_ROM_ADDRESS 0x50    //Address of 24LC256 eeprom chip
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
//#define ADT_FUEL_SYSTEM_I2C // comment to disable additional fuel system such as LPG
//
// Include simple driver
#ifdef ADT_FUEL_SYSTEM_I2C

#include <MenuBackend.h>
//#include "lib/drivers/I2cSimpleListener.h"

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

/**
 * LCD  interface
 */
class LcdUiInterface {
public:

    virtual void draw() = 0;

    virtual void intro() = 0;

    virtual void begin() = 0;

    virtual void drawShutdownBegin() = 0;

    virtual void drawShutdownShort() = 0;

    virtual void drawShutdownCount(char[2]) = 0;

    virtual void draWShutdownTripSave() = 0;

    virtual void draWShutdownTripSkip() = 0;

protected:

#ifdef ARDUINO_MID_LAN
    char messageBuffer[128];
#endif
    //
    // Defining content generate container variables
    char char_2[3];
    char char_3[4];
    char char_4[5];
    char char_6[7];
    char char_7[8];


#ifdef ARDUINO_MID_LAN

    /**
 * Gets string message
 */
    const char *getMsg(uint8_t i) {
        strcpy_P(messageBuffer, (char *) pgm_read_word(&(LcdMsgTable[i])));
        return messageBuffer;
    }

#endif
};

/**
 *  Menu interface
 */
class MenuUiInterface {
public:

    virtual void setup() = 0;

    virtual void menuChanged(MenuChangeEvent change) = 0;

    virtual MenuBackend getMB() = 0;

    virtual void moveUp() = 0;

    virtual void moveDw() = 0;
};


static void menuUseEvent(MenuUseEvent used);

static void menuChangeEvent(MenuChangeEvent changed);

/**
 * EepRom  data
 */

struct SavedData {
    float fuel_adt;
    float fuel_def;
    float dist_trv;
    float total_km;
    float time_trp;
    float dist_trp;

    float sens_vss;
    float sens_rpm;
    float sens_dst;
    float sens_ecu;
};

#define MID_VERSION "1.5+"
//
// Reserving RAM
//#include "lib/drivers/ResRam.h"

//
// Init reserving RAM
//ResRam resRam;

#endif //ARDUINOMID_CONF_H

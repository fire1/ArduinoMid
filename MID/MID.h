//
// Created by Angel Zaprianov on 10.2.2017
//

#ifndef ARDUINO_MID_H
#define ARDUINO_MID_H

#include <Arduino.h>

#include "conf.h"


#define DEBUG 1
//#define DEBUG_MENU
#define DEBUG_STATE
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
// Components list https://docs.google.com/document/d/11Yb3lh6lio_FKBEzur-n49dYgrX5xFf_UQObUGn2u_c/edit?usp=sharing
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
const uint8_t BTN_PIN_UP = 8;  // old 9         //  Plug:23  [meg] [10k] [+5v]    Column switch
const uint8_t BTN_PIN_DW = 9;  // old 8         //  Plug:24  [meg] [10k] [+5v]    Column switch navigation
//
// Shutdown protection pin
const uint8_t SAV_PIN_CTR = A6;     //  Plug:16    [to +5v relay]  Disconnect supply voltage
const uint8_t SAV_PIN_DTC = A7;     //  proto A7     //  Plug:16   [20k] -> [meg] -> [znr]   Detect ignition key off state
//
// Live pins
const uint8_t ENG_CLT_PIN = A0;     //  Plug:31     [dva + 47uf] Engine Temperature
const uint8_t FUL_TNK_PIN = A2;     //  Plug:31     [dva + 47uf] Fuel Tank TODO
const uint8_t BRK_LGH_PIN = 12;     //  Plug:15     [50k] + [znr] Brake light detection
const uint8_t RPM_SNS_PIN = 2;      //  Plug:6      [dvd] RPM [attachInterrupt]
const uint8_t SPD_SNS_PIN = 3;      //  Plug:12     [dvd] Speed sensor hub [attachInterrupt]
const uint8_t ECU_SGN_PIN = 19;     //  Plug:27     [dvd] ECU  signal

//
// Car state pins
const uint8_t STT_UL1_PIN = A11;    //      Plug 17 Unusable for now (do not connect)
const uint8_t STT_UL2_PIN = A12;    //      Plug 18 Unusable for now (do not connect)
const uint8_t STT_BRK_PIN = A13;    //      Plug:19        [meg]    Critical Brake ware
const uint8_t STT_CLN_PIN = A14;    //      Plug:20        [meg]    Critical Coolant level
const uint8_t STT_WNW_PIN = A15;    //      Plug:22        [meg]   Critical window washer
const uint8_t STT_OIL_PIN = A1;     //      Plug:13        [meg]   Critical oil level
const uint8_t STT_VLT_PIN = SAV_PIN_DTC;     //   Duplicating  SAV_PIN_DTC
//
// Display dim pins
const uint8_t DIM_PIN_VAL = A10;    //  Plug:7  [dva + 50uf ]     Display back-light
//
// Temperatures
// todo test values:
// Bais resistor 2490
//      -40C / 100700
//      30C  / 2238
//      90C  / 177
const uint8_t TMP_PIN_OUT = A8;     // Plug:3     [+5v] [150R]   Front temperature sensor

#if SCREEN == 162 || !defined(SCREEN)
//
// Display pins map
//  NOTE: pins are inverted due to sockets
//
//  General pins
const uint8_t DSP_PIN_GD1 = 25; // Power GND
const uint8_t DSP_PIN_VCC = 31; // Power positive
const uint8_t DSP_PIN_CD = 35;  // RS / Data instruction
const uint8_t DSP_PIN_WR = 37;  // R/W Write signal (HIGH)
const uint8_t DSP_PIN_CE = 39;  // Chip Enable Signal

//
// Data pins
const uint8_t DSP_PIN_DT1 = 32;
const uint8_t DSP_PIN_DT2 = 34;
const uint8_t DSP_PIN_DT3 = 36;
const uint8_t DSP_PIN_DT4 = 38;
//
//
const uint8_t DSP_PIN_LDA = 27; // Black-light Anode (+5V)
const uint8_t DSP_PIN_LDK = 29; // Black-light cathode (0v)

const uint8_t DIM_PIN_OUT = DSP_PIN_LDA;     //              Output dim of playEntry

#elif SCREEN == 24064
//
// Add language
//#include "lib/ui/language/en.h"
#include "lib/ui/language/bg.h"

//
// Display pins map
//  NOTE: pins are inverted due to LCD sockets
//
//  General pins
const uint8_t DSP_PIN_GD1 = 24; // Power GND
const uint8_t DSP_PIN_GD2 = 25; // Power GND
const uint8_t DSP_PIN_VCC = 27; // Power positive
const uint8_t DSP_PIN_WR = 29;  // Write Signal
const uint8_t DSP_PIN_RD = 28;  // Read Signal
const uint8_t DSP_PIN_CE = 31;  // Chip Enable Signal
const uint8_t DSP_PIN_CD = 30;  // Instruction Code
const uint8_t DSP_PIN_FS = 43;  // Font selection
const uint8_t DSP_PIN_RST = 32; // Reset signal
const uint8_t DSP_PIN_LDA = 45; // Black-light Anode (+5V)
const uint8_t DSP_PIN_LDK = 44; // Black-light cathode (0v)
//
// Data pins
const uint8_t DSP_PIN_DT1 = 35;
const uint8_t DSP_PIN_DT2 = 34;
const uint8_t DSP_PIN_DT3 = 37;
const uint8_t DSP_PIN_DT4 = 36;
const uint8_t DSP_PIN_DT5 = 39;
const uint8_t DSP_PIN_DT6 = 38;
const uint8_t DSP_PIN_DT7 = 41;
const uint8_t DSP_PIN_DT8 = 40;


const uint8_t DIM_PIN_OUT = DSP_PIN_LDA;     //              Output dim of playEntry
#endif

#define VSS_ALARM_ENABLED 1;
/* Extras ...   ******/
//
// Alarm / Tone pin
#define TONE_ADT_PIN 11 // buzzer
//
// Steering wheel buttons

// Digital Potentiometer pin map
// 50k digital potentiometer [MCP41050]
// [meg] 53     to pin [pot] 1 [CS]
// [meg] 52     to pin [pot] 2 [SCK]
// [meg] 51     to pin [pot] 3 [SI]
//     GND      to pin [pot] 4
//     +5V      to pin [pot] 8 [VCC]
//     GND      to pin [pot] 7
// Stereo input to pin [pot] 6 (through  RELAY to stereo remote control)
//                     [pot] 5 NC
// SPI map
// 50 MISO
// 51 MOSI
// 52 SCK
// 53 SS

// Alpine / Steering Wheel buttons todo A9
const uint8_t ALP_PIN_INP = A9; // +5V -> [220R] to Radio Unit plug pin 1
const uint8_t ALP_PIN_OUT = 53; //  Dig pot signal
const uint8_t ALP_PIN_MSK = 46; //  mg46  ->  1N5406 ->  Unit input ||  5V -> RELAY
// * Signal HIGH will be send from pin 50 to hide dig pot resistance
// * 1N5406 is used in mega pin 50 to block GND signal from pin (only +5V need to be forward )
// * 5V to relay is used as signal cutter when MID is out of power
//
// Change state of shutdown "press to save"
//const uint8_t SHUTDOWN_SAVE_STATE = LOW;
//const uint8_t SHUTDOWN_SAVE_BUTTON = 8;
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
//
//  +5V pull-up 4.7k resistor to this pin
#define ONE_WIRE_BUS A3 // Pin define
#define TEMPERATURE_DS_VCC A5
#define TEMPERATURE_DS_GND A4
#endif
//
//
// In my case  ... about LPG installation:
// My LPG fuel installation is EuropeGas "Avance 32" and communication between LPG ECU and fuel switch is I2C protocol
// 4 Pins 5V LPG fuel switch/gauge
//      Two wires are for power supply, other two wires is for  information.
//      * Check wiring diagram in order to determine your wiring
//      * This code uses pin17 as slow serial communication
//

//
// Include simple driver
#ifdef ADT_FUEL_SYSTEM_SERIAL

#ifndef LPG_INSTALLATION
#define LPG_INSTALLATION

//
// Define default use of fuel system
#ifndef  DEFAULT_FUEL_USING
#define DEFAULT_FUEL_USING 1
#endif

#endif

#ifndef LPG_SWITCHING_DETECT
#define LPG_SWITCHING_DETECT


#endif
#endif

#ifdef LPG_SWTC_PIN
#define LPG_INSTALLATION
#define LPG_SWITCHING_DETECT

//
// Define default use of fuel system
#ifndef  DEFAULT_FUEL_USING
#define DEFAULT_FUEL_USING 1
#endif

#endif


#define MID_VERSION "1.5.7"

#include "glob.h"


#endif //ARDUINOMID_CONF_H

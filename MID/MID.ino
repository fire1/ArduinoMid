/*
---------------------------------------------------
    Arduino MID

     Author: Angel Zaprianov <me@fire1.eu>
     Version: 0.1 [pre-alpha]
     Start Date: 09.06.2016

     For the full copyright and license information, please view the LICENSE
     file that was distributed with this source code.
---------------------------------------------------

 MID function menu
*/


#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <OneWire.h>
#include <MenuBackend.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>

//
// Inject data from serial monitor
#define SERIAL_INJECT_DATA
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
// Inside temperature [very cheep temperature sensor]
// additional mounted temperature sensor from DallasTemperature
#define INSIDE_TEMPERATURE_DS
#define ADDITIONAL_FUEL_SYSTEM // comment to disable additional fuel system such as LPG
//
// MID plug pins definition over Arduino
//
// Define button pins for steering controller
const uint8_t BTN_PIN_UP = 8;       //  Plug:23     Column switch
const uint8_t BTN_PIN_DW = 9;       //  Plug:24     Column switch navigation
//
// Shutdown protection pin
const uint8_t SAV_PIN_CTR = A6;     //  Plug:4      Disconnect supply voltage
const uint8_t SAV_PIN_DTC = A7;     //  Plug:16     Detect ignition key off state
//
// Engine pins
const uint8_t ENG_CLT_PIN = A0;     //  Plug:31     Engine Temp.  [may be capacitor is needed]
const uint8_t BRK_LGH_PIN = 11;     //  Plug:       Brake light detection
const uint8_t RPM_SNS_PIN = 2;      //  Plug:6      RPM [attachInterrupt]
const uint8_t SPD_SNS_PIN = 3;      //  Plug:12     Speed sensor hub [attachInterrupt]
const uint8_t ECU_SGN_PIN = 19;     //  Plug:27     ECU  signal

#define LPG_INSTALLATION
#ifdef LPG_INSTALLATION
//
// [Used LPG ECU is EG Avance 32]
// 4 Pins 5V LPG fuel switch/gauge
//      Two wires are for power supply, other two wires is for displayed information.
//      * Check wiring diagram in order to determine your wiring
const uint8_t LPG_LVL_PIN = A5;     //  [brown]     Switch DATA     Tank fuel level
const uint8_t LPG_SWT_PIN = A4;     //  [blue]      Switch button   Fuel switcher
#endif
//
// Display dim pins
const uint8_t DIM_PIN_VAL = A10;    //  Plug:7      Display back-light
const uint8_t DIM_PIN_OUT = 46;     //              Output dim of lcdDisplay
//
// Temperatures
const uint8_t TMP_PIN_OUT = A9;     // Plug:3       External temperature sensor

/* Extras ...   ******/
//
// Alarm / Tone pin
const uint8_t ADT_ALR_PIN = 11;
//
// Alpine / Steering Wheel buttons
const uint8_t ALP_PIN_INP = A8;
const uint8_t ALP_PIN_OUT = 53;
const uint8_t ALP_PIN_VOL = 14;


/*********************** Global Vars ***********************/
//
//  volatile Vehicle time travel
//volatile float CUR_VTT = 0;
float TTL_TTD; // Total travel distance
float TTL_CNS_ADT; // Total  LPG Travel Liters
float TTL_CNS_DEF; // Total  BNZ Travel Liters
//float CRT_LPG; // Current LPG Consumption trip
//float CRT_BNZ; // Current BNZ Consumption trip
float TTL_WRD; // Total Work distance [changing the timing belt wear collection ]
//
// Change state of shutdown "press to save"
#define SHUTDOWN_SAVE_STATE LOW
#define SHUTDOWN_SAVE_BUTTON 9
//
// LiquidCrystal library
// Including from Arduino IDE
#include <LiquidCrystal.h>

//
// Interval / Amplitude
#include "lib/IntAmp.h"
//
// Adding Alphine emulator
#include "lib/WhlSens.h"
//
// Main Sensor handler
#include "lib/MainFunc.h"
//
// Read LPG fuel switch
//#include "lib/LpgSens.h" // Test mode
//
// Engine sensors
#include "lib/CarSens.h"
//
// Adding menu source
#include "lib/MidMenu.h"
//
//
#include "lib/EepRom.h"
//
// Add library
#include "lib/ShutDw.h"

//
// Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(32, 33, 34, 35, 36, 37);
//
// Menu cursor
int cursorMenu = 0;
//
// Amplitude interval
//    ampInt(min,low,mid,sec, big, max);
IntAmp ampInt(5, 10, 50, 100, 200, 1000);
//
// Constructing the class
CarSens carSens(&ampInt);
//
// Constructing the class
WhlSens whlSens(&ampInt);
//
// Data storage
EepRom eepRom(&carSens);
//
// Menu
MidMenu midMenu(&ampInt, &carSens, &eepRom);
//
// Shutdown constructor
ShutDw shutDown(&eepRom, &ampInt, &carSens);
//
// Special characters
#include "lib/LcdChar.h"
//
// Display driver
#include "lib/Lcd16x2.h"


//
// Define Welcome screen
static void playWelcomeScreen();

//
// Setup the code...
void setup() {

    //
    // Shutdown setupEngine
    shutDown.setup(SAV_PIN_CTR, SAV_PIN_DTC, ADT_ALR_PIN);
    //
    // Turn lcdDisplay off
    lcd.noDisplay();
    //
    // Debug serial
    Serial.begin(9600);
    //
    //
//    lpgSens.setup(4800, LPG_LVL_PIN, LPG_TXC_PIN);
    //
    // Change timer 3
    setupTimer3();
    //
    //
    eepRom.setup();


//    long bnzFuel[2] = {FUEL_BNZ_IFC, FUEL_BNZ_CNS};
//    bnzFuel.ifc = FUEL_BNZ_IFC;
//    bnzFuel.cns = FUEL_BNZ_CNS;
//    //
//    //
//    lpgFuel.ifc = FUEL_LPG_IFC;
//    lpgFuel.cns = FUEL_LPG_CNS;


    //
    // Sets Default Fuel as Benzine (fuel that engine will start) and additional LPG
//    Fuel bnzFuel = {FUEL_BNZ_IFC,FUEL_BNZ_CNS}, lpgFuel = {FUEL_LPG_IFC,FUEL_LPG_CNS};
    //
    // Setup fuel
    carSens.setupFuel({FUEL_BNZ_IFC, FUEL_BNZ_CNS}, {FUEL_LPG_IFC, FUEL_LPG_CNS});
    //
    // Setup fuel gauge two required wires
    carSens.setupAdtFuel(LPG_LVL_PIN, LPG_SWT_PIN);
    //
    // consumption
    // Engine / Speed sensors
    carSens.setupEngine(SPD_SNS_PIN, RPM_SNS_PIN, ECU_SGN_PIN, ENG_CLT_PIN);
    //
    // Screen back-light
    carSens.setupScreen(DIM_PIN_VAL, DIM_PIN_OUT);
    //
    // Outside temperature (car stock temp sns)
    carSens.setupTemperature(TMP_PIN_OUT);
    //
    // Initializes the interface to the LCD screen
    lcd.begin(16, 2);
    // and clear it ...
    lcd.clear();
    //
    // Adding custom characters to LCD
    setLcdBaseChar();
    lcd.display();
    //
    // Show welcome from car
    playWelcomeScreen();
    //
    // Set MID menu
//    setupMenu();
    midMenu.setup(BTN_PIN_UP, BTN_PIN_DW, ADT_ALR_PIN);
    //
    // Setup SPI lib
    whlSens.setup(ALP_PIN_INP, ALP_PIN_OUT, ALP_PIN_VOL);
    //
    // Restore data
    eepRom.loadCurrentData();

    pinMode(18, INPUT_PULLUP);

    pinMode(LPG_LVL_PIN, INPUT);
    pinMode(LPG_SWT_PIN, INPUT);

#if defined(USBCON)
    Serial.println("USB Connected ...");
#endif

}


void loop() {


    //
    // Set new time every begin
    ampInt.setTimer(millis());
    //
    // Amplitude loop init
    ampInt.listener();

    if (ampInt.isMax()) {
        Serial.print("Window washer value: ");
        Serial.println(digitalRead(18));
    }

    //
    // Listen engine
    carSens.listener();
    if (ampInt.isBig()) {
        Serial.print("Current fuel state is: ");
        Serial.print(carSens.getFuelState());
        Serial.print("        DUMP||  swt: ");
        Serial.print(dumpFuelSwitchSwt);
        Serial.print(" ||  tnk: ");
        Serial.print(dumpFuelSwitchLvl);
        Serial.print(" || cnt: ");
        Serial.println(dumpFuelSwitchCnt);
    }

    //
    // Reads buttons from steering
    whlSens.listener();
    //
    // Listener shutdown
    shutDown.listener();
    //
    //  Read main buttons
    midMenu.listener(&whlSens, cursorMenu);
    //
    //  Switch to shutdown menu
    shutDown.cursor(cursorMenu);
    //
    // Display menu
#ifdef ARDUINOMID_LCD_DISPLAY_16x2_H
    switch (cursorMenu) {
        case MidMenu::MENU_ENTER:
            midMenu.lcdDisplay(&lcd);
            break;
            //
            // Main / first menu
        case 1:
            displayTotalCons();
            displayTotalDst();
            displayOutTmp();
            displayInsTmp();
            break;
            //
            // Dashboard
        case 11:
            displayEngRPM();
            displayCarKMH();
            displayCarECU();
            displayEngTmp();
            break;

        case 12:
            displayTest();
            break;

            //
            // Travel menu
        case 2:
            displayTrip();
            break;
        case 21:
            displayAverage();
            break;

            //
            // Fuel menu
        case 3:
            displayConsumption();
            break;
        case 32:
            displayFuelTanks();
            break;

        case 4:
            break;

        case ShutDw::MENU_SHUTDOWN:
            shutDown.lcdDisplay(&lcd);
            break;
    }
#endif
    //
    // Commands that changes global value from serial monitor
    //
    // ttd=<0000> INJECTS: Total distance
    // ttc=<0000> INJECTS: Total fuel consumed
    // clc=<0000> INJECTS: Total trip fuel consumed
    serialInjectData();

}

/**
 * Welcome screen ...
 */
static void playWelcomeScreen() {
    lcd.setCursor(0, 0);
    lcd.print("    ASTRA       ");
    //
    // Test tone
    tone(ADT_ALR_PIN, 400, 20);
    delay(10);
    lcd.setCursor(0, 1);
    lcd.print("  ");
    lcd.write((uint8_t) 2);
    lcd.print(" Bertnone    ");
    delay(1500);
    lcd.clear();
}

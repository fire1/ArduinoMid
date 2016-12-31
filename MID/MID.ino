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
#include <Firmata.h>
#include <MenuBackend.h>
//#include <SoftwareSerial.h>
#include <DallasTemperature.h>
//
// Serial configuration
#define SERIAL_INJECT_DATA          // Inject data from serial monitor
#define SERIAL_MONITOR_BAUD 250000  // 115200 - Normal Speed of monitoring messages
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
//
// Car state pins
const uint8_t STT_BRK_PIN = A4;     //  Plug        Critical Brake ware
const uint8_t STT_CLN_PIN = A2;     //  Plug        Critical Coolant level
const uint8_t STT_OIL_PIN = A3;     //  Plug        Critical oil level
const uint8_t STT_WNW_PIN = A5;     //  Plug        Critical window washer
const uint8_t STT_VLT_PIN = A7;      // Duplicating  SAV_PIN_DTC
//
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
#define TONE_ADT_PIN 11
//
// Alpine / Steering Wheel buttons
const uint8_t ALP_PIN_INP = A8;
const uint8_t ALP_PIN_OUT = 53;
const uint8_t ALP_PIN_VOL = 14;

//
// Change state of shutdown "press to save"
#define SHUTDOWN_SAVE_STATE LOW
#define SHUTDOWN_SAVE_BUTTON 9
//
//
/// Additional temperature sensor
// Inside temperature [very cheep temperature sensor]
// additional mounted temperature sensor from DallasTemperature
#define INSIDE_TEMPERATURE_DS
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
// Not all pins on the Mega and Mega 2560 support change interrupts, so only the following can be used for RX:
// 10, 11, 12, 13, 14, 15, 50, 51, 52, 53, A8 (62), A9 (63), A10 (64), A11 (65), A12 (66), A13 (67), A14 (68), A15 (69).
const uint8_t pinLpgDat = A11;     //  [brown]     Switch DATA     Tank fuel level     /// A8
const uint8_t LPG_CLC_PIN = A12;     //  [blue]      Switch button   Fuel switcher       /// A9
#endif
//
/***************************************************************************/
//
//
// LiquidCrystal library
// Including from Arduino IDE
#include <LiquidCrystal.h>
//
// I2C detector driver
//#include "lib/LpgSens.h"
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
// Engine sensors
#include "lib/CarSens.h"
//
// Car state
#include "lib/CarState.h"
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
// Add car games
#include "lib/CarGames.h"

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
// Constructing car state
CarState carStat(&ampInt);
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
ShutDw shutDown(&eepRom, &ampInt, &carSens, &whlSens);
//
// Car games
CarGames carGames(&ampInt, &carSens, &midMenu);

//
//
//LpgSens lpgSens;
//
// Display driver
#include "lib/Lcd16x2.h"

#ifdef ADT_FUEL_SYSTEM_I2C
I2cSimpleListener i2cLpg(pinLpgDat, LPG_CLC_PIN);
#endif

//
// Define Welcome screen
static void playWelcomeScreen();

//
// Setup the code...
void setup() {

    //
    // Shutdown setupEngine
    shutDown.setup(SAV_PIN_CTR, SAV_PIN_DTC, TONE_ADT_PIN);
    //
    //
//    lpgSens.setup(pinLpgDat, LPG_CLC_PIN);
    //
    // Turn lcdDisplay off
    lcd.noDisplay();
    //
    // Debug serial
    Serial.begin(SERIAL_MONITOR_BAUD);
    //
    // Change timer 3
    setupTimer3();
    //
    //
    eepRom.setup();
    //
    // Sets Default Fuel as Benzine (fuel that engine will start) and additional LPG
    carSens.setupFuel({FUEL_BNZ_IFC, FUEL_BNZ_CNS}, {FUEL_LPG_IFC, FUEL_LPG_CNS});
    //
    // Setup fuel gauge two required wires
    carSens.setupAdtFuel(pinLpgDat, LPG_CLC_PIN);
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
    //  Setup car state pins to detect
    carStat.setup(STT_OIL_PIN, STT_CLN_PIN, STT_WNW_PIN, STT_BRK_PIN, STT_VLT_PIN);
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
    midMenu.setup(BTN_PIN_UP, BTN_PIN_DW, TONE_ADT_PIN);
    //
    // Setup SPI lib
    whlSens.setup(ALP_PIN_INP, ALP_PIN_OUT, ALP_PIN_VOL);
    //
    // Test usb detection
#if defined(USBCON)
    Serial.println("USB Connected ...");
#endif
    //
    // Restore data
    eepRom.loadCurrentData();
    //
    // Pass saved data to car state for calculation
    carStat.setWorkState(eepRom.getWorkDistance());
    //
    // Setup LPG pins
    pciSetup(LPG_CLC_PIN); //   attachInterrupt (digitalPinToInterrupt (LPG_CLC_PIN), isr, CHANGE);
    pciSetup(pinLpgDat); //   attachInterrupt (digitalPinToInterrupt (pinLpgDat), isr, CHANGE);
}
//
//
// The interrupt can be enabled for each pin individually (analog and digital!),
// but there are only 3 interrupt vectors, so 6-8 pins share one service routine:
// ISR (PCINT1_vect) pin change interrupt for A0 to A5
//ISR (PCINT1_vect) { // handle pin change interrupt for A0 to A5 here
//
//    // Install Pin change interrupt for a pin, can be called multiple times from pciSetup()
//    Serial.print("\n\n\n\n");
//    Serial.print(" LPG dat: ");
//    Serial.print(digitalRead(pinLpgDat));
//    Serial.print(" LPG clc: ");
//    Serial.print(digitalRead(LPG_CLC_PIN));
//    Serial.print("\n\n\n\n");
//}

void loop() {
    //
    // Set new time every begin
    ampInt.setTimer(millis());
    //
    // Amplitude loop init
    ampInt.listener();

    //
    // Listen engine
    carSens.listener();
    if (ampInt.isBig() && false) {
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
    // Listen state pins
    carStat.listener();

#ifdef ADT_FUEL_SYSTEM_I2C
    cli();
    carSens.listenerI2cLpg(&i2cLpg);
    sei();
#endif



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
            displayAlert();
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
        case 22:
            displayTotalTrip();
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
        case 41:
        case 42:
        case 43:
        case 44:
        case 45:
            displayCarState();
            break;
            //
            // Games menu
        case 5:
            displayCarGames();
            break;
        case 51:
            carGames.listenAStopwatch();
            displayCarGameWatch();
            break;
        case 52:
            displayCarGameDrag();
            break;
        case 53:
            carGames.listen0to100();
            displayCarGameT100();
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
    // lpg=<0000> INJECTS: lpg consumption
    // bnz=<0000> INJECTS: bnz consumption
    eepRom.injectFromSerial();

}

/**
 * Welcome screen ...
 */
static void playWelcomeScreen() {
    lcd.setCursor(0, 0);
    lcd.print("    ASTRA       ");
    //
    // Test tone
    tone(TONE_ADT_PIN, 400, 20);
    delay(10);
    lcd.setCursor(0, 1);
    lcd.print("   ");
    lcd.print(" Bertnone    ");
    delay(1500);
    lcd.clear();
}

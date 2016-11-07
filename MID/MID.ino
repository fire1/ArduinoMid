#include <Arduino.h>
#include <SPI.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <MenuBackend.h>


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
#define SERIAL_INJECT_DATA
//
// Uncommented to debug basics
//#define GLOBAL_SENS_DEBUG
//
// Time information
#define MILLIS_PER_HR    3600000 // Hour
#define MILLIS_PER_MN    60000    // Minute
#define MILLIS_PER_SC    1000    // Second
//
//
bool THROTTLE_UP = false; // Is open throttle  (acceleration)
const double CON_ENG_CC = 1.796; // Engine capacity [X18XE1]
const int CON_ENG_CL = 4; // Cylinders
const double FLW_MTR_FR = 1.414; // Flowmeter factor (revers-pressure)
const double AIR_FUL_RT = 14.70; // 14.7(oxygen) : 1(fuel) for burning
const double VEC_FUL_RT = 1.0;    // 14.7(oxygen) : 1(fuel) for burning
const double FuelDensityGramsPerLiter = 750.0;   // константа - грамм бензина в 1 литре бензина
const double AirFuelRatio = 14.70;  // константа расхода 14,7 воздуха к 1 литра бензина, у дизеля своя, у газа своя

//
// MID plug pins definition over Arduino
//
// Define button pins for steering controller
const uint8_t BTN_PIN_UP = 8;
const uint8_t BTN_PIN_DW = 9;
//
// Shutdown protection pin
const uint8_t SAV_PIN_CTR = A6; //
const uint8_t SAV_PIN_DTC = A7; //
//
// Engine pins
const uint8_t ENG_CLT_PIN = A5; // Engine Temp. MID32 RPM [attachInterrupt]
const uint8_t RPM_SNS_PIN = 2;  //  old:10 MID6 RPM [attachInterrupt]
const uint8_t SPD_SNS_PIN = 3;  //  MID12 Speed sensor hub [attachInterrupt]
const uint8_t ECU_SGN_PIN = 19; //  ECU signal

const uint8_t LPG_LVL_PIN = A4; // LPG tank sensor [20k resistance is mine]
//
// Display dim pins
const uint8_t DIM_PIN_VAL = A10; // MID7 input Dim of display
const uint8_t DIM_PIN_OUT = 46; // output dim of display
//
// Temperatures
const uint8_t TMP_PIN_OUT = A9; // External temperature sensor

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
float TTL_TLC; // Total Liters per hour consumed
float TTL_CLC; // Total Consumption trip


/*
#include <SerialDebug.h>
#define DEBUG true
*/
//
//
//#include <SoftwareSerial.h>

//
// Includes Libraries
//#include <SPI.h>



#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
/* Todo
#include <Wire.h>
*/

#include <LiquidCrystal.h>

//
// Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(32, 33, 34, 35, 36, 37);

//
// Menu cursor
int cursorMenu = 0;
//
// Global interval
const int SNS_INTERVAL_TIME_LOW = 150; // Low sensor interval
const int SNS_INTERVAL_TIME_MID = 2000; // Mid sensor inter
int showerCounter = 0;

//
//
#include "lib/IntAmp.h"
//
// Adding Alphine emulator
#include "lib/WhlSens.h"
//
// Main Sensor handler
#include "lib/MainFunc.h"
//
//
#include "lib/LcdChar.h"
//
// Engine sensors
#include "lib/CarSens.h"

//
// Amplitude interval
//      between loops
IntAmp ampInt(/* min */5, /* low */10, /* mid */50, /* sec */100, /* big */200, /* max */ 1000); // TODO need tests
//
// Constructing the class
CarSens carSens(&ampInt);
//
// Constructing the class
WhlSens whlSens(&ampInt);
//
// other sensors initialization
// Adding sensors
#include "lib/SensInit.h"

//
// Adding menu source
#include "lib/MidMenu.h"
//
//
#include "lib/EepRom.h"

//
// Data storage
EepRom eepRom;

//
// Change state of shutdown "press to save"
#define SHUTDOWN_SAVE_STATE LOW
//
// Add library
#include "lib/ShutDw.h"

//
// Shutdown constructor
ShutDw shutDown(&eepRom, &ampInt, &carSens);

//
//
static void playWelcomeScreen();


//
// Setup the code...
void setup() {


    //
    // Shutdown setupEngine
    shutDown.setup(SAV_PIN_CTR, SAV_PIN_DTC, BTN_PIN_UP, ADT_ALR_PIN);
    //
    // Turn display off
    lcd.noDisplay();
    //
    // Debug serial
    Serial.begin(9600);
    //
    // Change timer 3
    setupUseTimer3();
    //
    //
    eepRom.setup();

    //
    //
    carSens.setupLpg(LPG_LVL_PIN);

    //
    // Engine / Speed sensors
    carSens.setupEngine(SPD_SNS_PIN, RPM_SNS_PIN, ECU_SGN_PIN, ENG_CLT_PIN);
    //
    // Screen back-light
    carSens.setupScreen(DIM_PIN_VAL, DIM_PIN_OUT);
//    setupBackLight(DIM_PIN_VAL, DIM_PIN_OUT);

    setupTemperature();
    //
    // Initializes the interface to the LCD screen
    lcd.begin(16, 2);
    lcd.clear();
    //
    // main setupEngine
    setupMain();
    //
    // Adding custom characters to LCD
    setLcdBaseChar();
    lcd.display();
    //
    // Show welcome from car
//    playWelcomeScreen();
    //
    // Set MID menu
    setupMenu();
    //
    // Setup SPI lib
    whlSens.setup(ALP_PIN_INP, ALP_PIN_OUT, ALP_PIN_VOL);
    //
    // Restore data
    eepRom.loadCurrentData();

//    TTL_TLC = 12.34;

}


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
    //
    // Sensors
    sensorsInit();
    //
    // Reads buttons from steering
    whlSens.listenButtons();
    //
    // Simulate resistance in radio
    whlSens.sendRadioButtons();
    //
    // Listener shutdown
    shutDown.listener();
    //
    //  Read main buttons
    readButtons(BTN_PIN_UP, BTN_PIN_DW);
    //
    // Handle navigation
    navigateMenu();
    //
    //  Switch to shutdown menu
    shutDown.cursor(cursorMenu);
    //
    // Switch menu from cursor
    switch (cursorMenu) {
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
        case 4:
            displayAverage();
            break;
            //
            // Travel menu
        case 2:
            displayTrip();
            break;
        case 3:
            displayConsumption();
            break;
        case 32:
            displayFuelTanks();
            break;
        case ShutDw::MENU_SHUTDOWN:
            shutDown.display();
            break;
    }



//
// Serial injection
#if defined(SERIAL_INJECT_DATA)
    String srlAllData;
    String srlStrName;
    String srlOutputs;
    //
    // Default message
    srlOutputs = " None ... Sorry! ";
    //
    // Execute command from serial
    if (Serial.available() > 0) {
        srlStrName = Serial.readStringUntil('=');
        //
        // So ... C++ is static language...
        //      is not good idea to re-set dynamically
        if (srlStrName == "TTD") {
            TTL_TTD = Serial.readStringUntil('\n').toInt() * 0.01;
            srlOutputs = "TTL_TTD";
            srlOutputs += TTL_TTD;
        }
        if (srlStrName == "TLC") {
            TTL_TLC = Serial.readStringUntil('\n').toInt() * 0.01;
            srlOutputs = "TTL_TLC";
            srlOutputs += TTL_TLC;
        }
        if (srlStrName == "CLC") {
            TTL_CLC = Serial.readStringUntil('\n').toInt() * 0.01;
            srlOutputs = "TTL_CLC";
            srlOutputs += TTL_CLC;
        }
        //
        // Show command information to human
        Serial.println("[MID $]> Affected value of " + srlOutputs);
        //
        // Remove containers
        srlStrName = "";
        srlAllData = ""; // Clear recieved buffer
        srlOutputs = "";
    }
#endif

}

/**
 * Welcome screen ...
 */
static void playWelcomeScreen() {
    lcd.setCursor(0, 0);
    lcd.print("    ASTRA       ");
    //
    // Test tone
    tone(ADT_ALR_PIN, 400, 100);
    delay(10);
    lcd.setCursor(0, 1);
    lcd.print("    Bertnone    ");
    delay(1000);
    lcd.clear();
}



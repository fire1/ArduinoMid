//
// Created by Angel Zaprianov on 19.10.2016 г..
//
// Consumption calculation of this file is base over OBDuino
//
#include <Arduino.h>
#include "../MID.h"
#include "AmpTime.h"
#include "MainFunc.h"
#include "Melody.h"
//#define ECU_SENS_DEBUG
//
#ifndef ARDUINO_MID_CAR_SENS_H
#define ARDUINO_MID_CAR_SENS_H

// TODO this is test mode
#ifndef ADT_FUEL_SYSTEM_SERIAL
#define ADT_FUEL_SYSTEM_I2C
#endif
//#ifdef ADT_FUEL_SYSTEM_I2C
//
//#include "drivers/I2cSimpleListener.h"
//
//#endif
//
//

//#define DEBUG_ENG_TEMP
//
// Show information from consumption
//#define DEBUG_CONS_INFO
//
// Speeding alarm
#define VSS_ALARM_CITY_SPEED  58 // km
#define VSS_ALARM_VWAY_SPEED  100 // km
#define VSS_ALARM_HWAY_SPEED  140 // km
#define VSS_ALARM_VERIFICATE  8 //
//#define VSS_ALARM_ENABLED // Comment to disable speeding alarms
//
// --------------------------------------------------------------------------------------------------------------------
// ABOUT ECU signal
//      (actuated consumption signal)
//
//  On the GM hot film MAFs, you can also tap into the onboard computer data stream
//  with a scan tool to read the MAF sensor output in "grams per second" (GPS).
//  The reading might go from 3 to 5 GPS at idle up to 100 to 240 GPS at wide open throttle and 5000(+) RPM.
//  The frequency range varies from 30 to 150 Hz, with 30 Hz being average for idle and 150 Hz for wide open throttle.
//  Using as frequency MAF. Wide open throttle test reaches ~244 gps.
// --------------------------------------------------------------------------------------------------------------------
//
#ifndef CAR_SENS_CUSTOM_CORRECTION
/**
 * 92.23 * X = 22.5L  [full fill with 18.37L]
 * X = 2250 / 9223
 * X = 0.24396 [0.24395532906863276591130868480971  pulses given from signal]
 *
 * So ... 18.37 / 0.24396 = ~75.3
 * 75.3 * 0.24396 = 18.370188
 * ECU_CORRECTION = 75
 * 88.1
 *
 */
//
// 75.3 because there no LPG switch detection (engine runs in benzene to work temperature )...
// must be clear 75
// ECU Consumption correction
#define ECU_CORRECTION 75 //45   75 // 147.23 ///  346 /// to high 692
//
// Speed correction
#define VSS_CORRECTION 1.6 // 2.6  v1.5 = 1   // V1.4 = 1.6 //  fast 3.767
//
// Revs correction
#define RPM_CORRECTION  75//  47 / 45    fast 33.767
////Impulse Odometer:
/// If as stated above, use the " pulse according to " Set here the number of pulses that traveled for 1km.
/// The value of this parameter, we find holding the button.
/// reset daily kilometers to flight instruments while turning the key in the ignition to the active position.
/// For vehicles Astra G it is usually worth the 15385 (mul by 2 = 30770). For vehicles Frontera , it is the value of the 2700.
//25.2 / 1.8l
// Distance correction
#define DST_CORRECTION  30800  // v1.4 30800.00  //  15383.29  //   15385
//
#define TRS_CORRECTION 0 // 0.064444 a proximity  6(~6)%
//
//#define VSD_SENS_DEBUG;
#define SCREEN_DEF_LIGHT 22 // This value is mul. by *10
//
// Screen default value
#define SCREEN_GO_TO_DEF 15

#endif


#ifndef CAR_SENS_CUSTOM_FUELS
//
// Default fuel state
#define DEFAULT_FUEL_STATE 0
/**************************/
/* GASOLINE ENGINE CONFIG */
/**************************/
// [CONFIRMED not tested over MID] For gas car use 3355 (1/14.7/730*3600)*10000
#define FUEL_BNZ_IFC 3355
#define FUEL_BNZ_CNS 10731 // 14.7*730 = 10731

/************************/
/* LPG ENGINE CONFIG    */
/************************/
//LPG mass/volume is 520-580gr/ltr depending on propane/butane mix

// LPG/air ratio:
// 15.8:1 if 50/50 propane/butate is usedMenu
// 15:1 if 100 propane is usedMenu
// 15.4 if 60/40 propane/butane is usedMenu
// experiments shows that something in middle should be usedMenu eg. 15.4:1 :)



// [CONFIRMED] For lpg(summer >20C) car use 4412 (1/15.4/540*3600)*10000
#ifndef LPG_SWITCHING_DETECT
// // NOTE: this value is set without detection of fuel switch (mixed with benzine cons)
#define FUEL_LPG_IFC 4329
#define FUEL_LPG_CNS 8316  // [CONFIRMED]

#else
// // NOTE: With fuel switching must be 4412, but this 3915  value depends over LPG fuel configuration .... :/
#define FUEL_LPG_IFC 3915  // up to 3936 [NOT CONFIRMED]
#define FUEL_LPG_CNS 7586 // 7586 or 8316  // ORIGINAL 15.4*540 = 8316 [CONFIRMED (no switching)]
// 8316 default
// 7586 below 10*C

// At ~20°C 8479
//
// (old may be changed from noise) 7435 [NOT CONFIRMED]
#define LPG_SWTC_PIN 17
#endif
#endif
/************************/
/* DIESEL ENGINE CONFIG */
/************************/
// [NOT TESTED] For diesel car use ??? (1/??/830*3600)*10000
//#define FUEL_DSL_CNS ????
//#define DSL_MAF_CNST ???   // ??*830*10
#define FUEL_ADJUST 1

/************************/
/*        ENGINE CONFIG */
/************************/
// https://en.wikipedia.org/wiki/Engine_displacement
// displacement = 0.7854.. X bore2 X stroke X Numb of Cylinders
//  Chevy bore is 4.312 in, and the stroke is 3.65 in, therefore the displacement for this eight-cylinder engine is:
//          3.1416/4 × (4.312 in)2 × 3.65 in × 8 = 426.4 cu in
/* Example
X18XE1  - 1796 cm3 = 17.96 dl
 */
#define ENGINE_DSP  17.96   // engine displacement in dL
#define CONS_TGL_VSS  3       // speed from which we toggle to fuel/hour (km/h)
#ifndef SWITCH_FUEL_ON_STATE
#define SWITCH_FUEL_ON_STATE LOW
#endif
struct Fuel {
    uint16_t ifc;
    uint16_t cns;
};

#ifndef CAR_SENS_CUSTOM_GEARS
//
// Gears resolver constants
#define CAR_GEAR_Dia  616
#define CAR_GEAR_Pi  3.14
//
/*
 * Original backup
#define CAR_GEAR_G1  3.308
#define CAR_GEAR_G2  2.13
#define CAR_GEAR_G3  1.483
#define CAR_GEAR_G4  1.139
#define CAR_GEAR_G5  0.949
#define CAR_GEAR_G6  0.816
 */
// Car's gears ratio
#define CAR_GEAR_G1  3.73
#define CAR_GEAR_G2  2.14
#define CAR_GEAR_G3  1.41
#define CAR_GEAR_G4  1.12
#define CAR_GEAR_G5  0.89
#define CAR_GEAR_G6  0.816 //  Additional functionality :)
#define CAR_GEAR_FD   3.74 // Final drive
#define CAR_GEAR_MX  5 // Last gear
#endif
//
// Additional temperature sensor
//      [Dallas Temperature temperature sensor]
#ifndef INSIDE_TEMPERATURE_DS // Comment this to disable (check for same definition in main *.ino file )
//
// Inside temperature [very cheep temperature sensor]
// additional mounted temperature sensor from DallasTemperature
#define INSIDE_TEMPERATURE_DS
#endif
//#define  DEBUG_TEMPERATURE_OU
//#define  DEBUG_TEMPERATURE_IN


//
// External Temperature
#define EXT_TMP_VSS  5
#define EXT_TMP_RST  10000
#define EXT_TMP_MSR  4      // 10 // measured temperature
#define EXT_TMP_MVL  409    // 328 // measured value of temperature

#if defined(INSIDE_TEMPERATURE_DS)

#include <OneWire.h>

#ifndef OneWire_h

#include "../../libraries/OneWire/OneWire.h"

#endif

#include <DallasTemperature.h>

#ifndef DallasTemperature_h

#include "../../libraries/DallasTemperature/DallasTemperature.h"
#include "InitObj.h"

#endif

// Data wire is plugged into pin A7 on the Arduino

#if !defined(ONE_WIRE_BUS)
#define ONE_WIRE_BUS 7
#endif
//
// Initialization of buss
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature temperatureSensors(&oneWire);
#endif

//
// Using Arduino API for Attach interrupt
static void EngSens_catchRpmHits();

static void EngSens_catchVssHits();

static void EngSens_catchEcuHits();

//
// Hint counters
unsigned volatile int vssHitsCount, rpmHitsCount, ecuHitsCount;

/**
 * Car's sensing class
 */
class CarSens {

    //
    // Take a pointer to time amplitude instance
    AmpTime *amp;

    Melody *mld;

    SavedData savedData;

private:

    //
    //
    boolean initializeAverage = 0;
    //
    // bool for read sensor at first loop
    boolean isInitializedLoop = 1;
    uint8_t pinBreaks;
    uint8_t pinTmpOut;
    uint8_t pinFulTnk;
    //
    // Engine temperature pin
    uint8_t pinTemp;
    uint8_t indexEngineTemp;
    uint8_t indexFuelTank;
    //
    boolean vehicleStopped = false;
    boolean _isEngineSens = false;
    boolean _isVehicleSens = false;
    boolean CUR_DIM_ON = false;
    uint8_t pinScreenInput, pinScreenOutput;
    //
    // Detect fuel switch
    uint8_t FUEL_STATE;
    //
    //
    uint8_t carGearNum = 0;
    Fuel FUEL_PARAM_DEF, FUEL_PARAM_ADT;
    uint8_t backLightIndex = 0; // Container of current level
    uint8_t CUR_ENT; // Engine temperature
    //
    // Car's reached ...
    uint8_t maxReachedSpeed = 0, maxReachedRevs = 0;
    //
    // Speeding alarms
    uint8_t speedAlarmCursor = 0, speedAlarmActive = 0;
    //
    // Human Results
    uint8_t CUR_VSS;
    uint16_t CUR_RPM;
    // Instant Fuel consumption counter
    uint16_t indexIfc;
    uint16_t tmp_outIndex = 0;
    //
    uint16_t smoothEngineTemp;
    uint16_t smoothFuelTank;
    uint32_t CUR_ECU;
    //
    //
    uint32_t tmp_outCollection = 0;

    //
    // LPG tank
    int CUR_LTK;
    //
    //
    float BREAK_TIME = 0;
    unsigned long breakTimeStart = 0;
    //
    //
    float FUEL_INST_CONS;
    float FUEL_WASTED = 0;
    float FUEL_TANK;
    //
    // Temperatures
    float CUR_OUT_TMP = 0; // Outside temperature
#if defined(INSIDE_TEMPERATURE_DS)
    float CUR_INS_TMP = 0; // Inside temperature /Dallas Temperature Sensors/
#endif
    float FUEL_AVRG_INST_CONS;

    //
    // Fuel consumption variables
    unsigned long FL_CNS_DEF, FL_CNS_ADT, FL_WST_DEF, FL_WST_ADT;
    //
    unsigned long CUR_VTT;// Travel time


    double collectionIfc;
    //
    // Car's average
    unsigned long averageAllVssValues = 0;
    unsigned long averageAllRpmValues = 0;
    /**
     * Handle playEntry dim
     */
    unsigned long averageDivider = 0;
    unsigned long sensDeltaCnsOldTime,
            CONS_DELTA_TIME,
            TTL_FL_CNS,  // Consumed fuel
            TTL_FL_WST;  // Waste fuel
    //
    // Distance container
    unsigned long CUR_VDS_collection;


    double CUR_VDS;


    /**
     * Handles speeding alarms
     */
    void sensAlarms();


    /**
    *
    * @param value
    */
    void setConsumedFuel(long value);

protected:
    /**
      * Setup RPM
      * @param pinTarget
      */
    void setupRpmSens(uint8_t pinTarget);

    /**
      * Setup VSS
      * @param pinTarget
      */
    void setupVssSens(uint8_t pinTarget);

    /**
      * Setup Ecu
      * @param pinTarget
      */
    void setupEcuSens(uint8_t pinTarget);

    /**
     * Gets calculate constant for instant consumption
     * @return int
     */
    int getIfcFuelVal();

    /**
     * Gets calculated constant for consumption
     * @return
     */
    long getCnsFuelVal();


private:


    void sensVss();

    void sensRpm();

    void sensEcu();

    void sensDim();

    void sensAvr();

    void sensCns();

    void sensIfc();

    void sensEnt();

    void sensTmp();

    void sensDlt();

    void sensBkt();

    void sensTnk();

public:

    float getCorVss() {
        return (savedData.sens_vss > 0) ? savedData.sens_vss : (float) VSS_CORRECTION;
    }

    float getCorRpm() {
        return (savedData.sens_rpm > 0) ? savedData.sens_rpm : (float) RPM_CORRECTION;
    }

    float getCorDst() {
        return (savedData.sens_dst > 0) ? savedData.sens_dst : (float) DST_CORRECTION;
    }

    float getCorEcu() {
        return (savedData.sens_ecu > 0) ? savedData.sens_ecu : (float) ECU_CORRECTION;
    }

/**
 *
 */
    void setSave(SavedData data) {
        savedData = data;
    }


    /**
 * Construct class
  * @param ampInt
  */
    CarSens(AmpTime &ampInt, Melody &melody) : amp(&ampInt), mld(&melody) {

    }

    Melody *passMelodyClass() {
        return mld;
    }

    //
    // Speeding alarm modes
    const unsigned int DISABLE_SPEED_AL = 0, ENABLE_SPEED_CT = 1, ENABLE_SPEED_VW = 2, ENABLE_SPEED_HW = 3;

    /**
     * Makes move of alarm cursor to up
     */
    void speedingAlarmsUp(void);

    /**
     * Makes move of alarm cursor to down
     */
    void speedingAlarmsDw(void);

    /**
     * Clear peak
     */
    void clearBuffer();

    uint8_t getGear();

    /**
     * Setup engine
      * @param pinVss
      * @param pinRpm
      * @param pinEcu
      * @param pinTmp
      */
    void setupVehicle(uint8_t pinVss, uint8_t pinRpm, uint8_t pinEcu, uint8_t pinTmp, uint8_t pinBrk, uint8_t pinTnk);

    /**
     * Is the engine was keen
     * @return bool
     */
    bool isRunEng() { return _isEngineSens; }

    /**
     * Is vehicle make some move
     * @return
     */
    bool isRunDst() { return _isVehicleSens; }

    /**
     * Setup additional fuel line
      */
#ifdef ADT_FUEL_SYSTEM_SERIAL

    void setFuelListener(LpgFuel *f) {
        if (f->isBNZ()) {
            FUEL_STATE = 0;
        } else if (f->isLPG()) {
            FUEL_STATE = 1;
        } else {
            FUEL_STATE = 1;
        }
    }

#endif

    void setFuelListener(boolean no_switch) {
        FUEL_STATE = 1;
    }


    void setFuelListener(void) {
        FUEL_STATE = DEFAULT_FUEL_STATE;
    }


    /**
    * Setup screen pins
    */
    void setupScreen(uint8_t pinInputInstrumentValue, uint8_t pinOutputDisplayContrast);

    /**
     * Setups fuel lines to listen
      * @param defFuel
      * @param adtFuel
      */
    void setupFuel(Fuel defFuel, Fuel adtFuel = {0, 0});

    /**
     * Setup temperature
      * @param pinOutsideTemperature
      */
    void setupTemperature(uint8_t pinOutsideTemperature);

    /**
     * Returns true if car standing on one position (no miving)
     */
    inline boolean isStopped() {
        return vehicleStopped;
    }

    /**
     * Gets outside temperature
     * @return
     */
    inline float getTmpOut() { return CUR_OUT_TMP; }

    /**
     * Gets inside temperature [with ext. Dallas Temperature sensor ]
     * @return
     */
    inline float getTmpIns() { return CUR_INS_TMP; }

    /**
     * Gets instant fuel consumption
     */
    inline float getIfc() { return FUEL_INST_CONS; }

    /**
     * Gets Instant fuel consumption average value
     */
    inline float getIfcAvr() { return FUEL_AVRG_INST_CONS; }

    /**
     * Gets default fuel line consumption
     */
    inline float getDefFuelCns() { return float(FL_CNS_DEF * 0.00001); }

    /**
     * Gets additional fuel line consumption
     */
    inline float getAdtFuelCns() { return float(FL_CNS_ADT * 0.00001); }

    /**
     * Gets fuel state  usedMenu
     */
    inline float getCurFuelCns() {
        if (getFuelState() == 0) return float(FL_CNS_DEF * 0.00001);
        if (getFuelState() == 1) return float(FL_CNS_ADT * 0.00001);
    }

    inline float getCurFuelWasted() {
        return FUEL_WASTED * 0.00001;
    }

    /**
     * Gets current fuel state
     */
    inline int getFuelState() { return FUEL_STATE; }

    /**
     * Gets default fuel levels in liters
     */
    inline float getFuelTnk() { return FUEL_TANK; }

    /**
     * Gets engine temperature
     *  resistor 5.6k  to GND || pull-up resistor 4.2k from MID plug pin 31 with 10mf cap
     */
    inline int getEngTmp() { return CUR_ENT; /* Range from 70 to 120*/}

    /**
     * Gets current Vss
     */
    inline uint8_t getVss() { return CUR_VSS; }

    /**
     * Gets current Rpm
     */
    inline uint16_t getRpm() { return CUR_RPM; }

    /**
     * Gets current Ecu
     */
    inline uint32_t getEcu() { return CUR_ECU; }

    inline int getTnkLpg() { return CUR_LTK; }

    inline int getTnkLpgPer() { return (int) CUR_LTK;/* map(CUR_LTK, 10, 100, 0, 100)*/}

    inline int getTnkBnz() { return 0; }

    inline int getTnkBnzPer() { return 0; }

    /**
     * Gets break time
     */
    inline float getBreakTime() { return BREAK_TIME / 60; }

    /**
     *  Gets travel time
     */
    int long getTime() { return CUR_VTT; }

    /**
     *  Gets Human time
     */
    char *getHTm(float saved = 0);

    /**
     *
     * @param saved
     * @param dspTime
     */
    void getHTm(float saved, char *dspTime);

    /**
     *  Gets current Distance
     */
    inline float getDst() { return float(CUR_VDS); }

    /**
     * Gets all pulses from speed sensor to calculate distance
     * @return
     */
    inline unsigned long getVdsDump(void) { return CUR_VDS_collection; }

    /** @deprecated
     * Gets Total fuel consumption
     * @return long
     */
    inline long getTfc() { return TTL_FL_CNS; }

    /**
     * Gets Average Vss
     */
    uint16_t getAvrVss();

    /**
     * Gets Average Rpm
     */
    uint16_t getAvrRpm();

    /**
     * Gets maximum car speed
     */
    uint8_t getMxmVss();

    /**
     * Gets maximum RPM reached during the trip
     */
    uint16_t getMxmRpm();

    /**
     *
     */
    inline uint8_t isDimOn() { return CUR_DIM_ON; }


    /**
     *  Listen sensors
     */
    void listener(void);

};

/***********************************************************************************************
 * ########################################################################################### *
 * ########################################################################################### *
 *                                                                                             *
 *                                   CPP part of file                                          *
 *                                                                                             *
 * ########################################################################################### *
 * ########################################################################################### *
 ***********************************************************************************************/
//unsigned long lastTimeVss = 0, vssPulseLen = 0;

/**
 * Interrupt function Vss
 */
void EngSens_catchVssHits() {
    vssHitsCount++;
//    unsigned long time = micros();
//    vssPulseLen = elapsedMicroseconds(lastTimeVss, time);
//    lastTimeVss = time;
}

/**
 * Interrupt function Rpm
 */
void EngSens_catchRpmHits() {
    rpmHitsCount++;
}

/**
 * Interrupt function Ecu
 */
void EngSens_catchEcuHits() {
    ecuHitsCount++;
}
/*******************************************************************
 *      SETUP
 */
/**
  * Setup RPM
  * @param pinTarget
  */
void CarSens::setupRpmSens(uint8_t pinTarget) {
    pinMode(pinTarget, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pinTarget), EngSens_catchRpmHits, HIGH);
}

/**
  * Setup VSS
  * @param pinTarget
  */
void CarSens::setupVssSens(uint8_t pinTarget) {
    pinMode(pinTarget, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pinTarget), EngSens_catchVssHits, HIGH);
}

/**
  * Setup Ecu
  * @param pinTarget
  */
void CarSens::setupEcuSens(uint8_t pinTarget) {
#ifndef DEBUG // Fixes simulation of fuel
    pinMode(pinTarget, INPUT_PULLUP);
#endif
    attachInterrupt(digitalPinToInterrupt(pinTarget), EngSens_catchEcuHits, HIGH);
}

/**
 *  Setup engine pins
 * @param pinVss
 * @param pinRpm
 * @param pinEcu
 * @param pinTmp
 */
void CarSens::setupVehicle(uint8_t pinVss, uint8_t pinRpm, uint8_t pinEcu, uint8_t pinTmp, uint8_t pinBrk,
                           uint8_t pinTnk) {
    setupRpmSens(pinRpm);
    setupVssSens(pinVss);
    setupEcuSens(pinEcu);
    //
    // Engine temperature
    pinMode(pinTmp, INPUT);
    pinMode(pinTnk, INPUT);


    digitalWrite(pinTmp, LOW); // TODO test here
    analogWrite(pinTnk, 0); // TODO test here
//    pinTemp = pinTmp;
    pinBreaks = pinBrk;
    pinFulTnk = pinTnk;
}

//void deatach(){
//    detachInterrupt(pinVss);
//}

/**
 * Gets calculate constant for instant consumption
 * @return int
 */
int CarSens::getIfcFuelVal() {
    if (getFuelState() == 0) return FUEL_PARAM_DEF.ifc;
    if (getFuelState() == 1) return FUEL_PARAM_ADT.ifc;
    return 0;
}

/**
 * Gets calculated constant for consumption
 * @return
 */
long CarSens::getCnsFuelVal() {
    if (getFuelState() == 0) return FUEL_PARAM_DEF.cns;
    if (getFuelState() == 1) return FUEL_PARAM_ADT.cns;
    return 0;
}

/**
 * Setup fuel line data
 * @param defFuel
 * @param adtFuel
 */
void CarSens::setupFuel(Fuel defFuel, Fuel adtFuel) {
    FUEL_PARAM_DEF = defFuel;
    //
    // Optional
    if (adtFuel.cns > 0 && adtFuel.ifc > 0) {
        FUEL_PARAM_ADT = adtFuel;
    }
}

/**
 * Setup screen data
 * @param pinInputInstrumentValue
 * @param pinOutputDisplayContrast
 */
void CarSens::setupScreen(uint8_t pinInputInstrumentValue, uint8_t pinOutputDisplayContrast) {

    pinScreenInput = pinInputInstrumentValue;
    pinScreenOutput = pinOutputDisplayContrast;

    pinMode(pinScreenInput, INPUT);
    pinMode(pinScreenOutput, OUTPUT);
    //
    // Set default value
    analogWrite(pinScreenOutput, SCREEN_DEF_LIGHT + 50);
    //
    // Sens dim level at start
    sensDim();
}

/**
 * Setup temperatures
 * @param pinOutsideTemperature
 */
void CarSens::setupTemperature(uint8_t pinOutsideTemperature) {
    pinTmpOut = pinOutsideTemperature;
    //
    // Setup outside pin
    pinMode(pinOutsideTemperature, INPUT);
    //
    // Setup inside pin
#if defined(INSIDE_TEMPERATURE_DS)
    analogWrite(TEMPERATURE_DS_VCC, 255);
    analogWrite(TEMPERATURE_DS_GND, 0);
    temperatureSensors.begin();
    //
    // Sends initial request (first from two)
    temperatureSensors.requestTemperatures();
    CUR_INS_TMP = temperatureSensors.getTempCByIndex(0);
#endif
};


/*******************************************************************
 *      LISTENER
 */
void CarSens::listener() {

    // Important!
    // No Interrupts
    cli();
    sensVss();
    sensRpm();
    sensEcu();
    // Interrupts
    //
    sei();

    //
    // Other
    sensAvr();
    sensEnt();
    sensTmp();
    sensTnk();
    //
    // Consumption
    sensDlt();
    sensCns();
    sensIfc();
    sensBkt();

    //
    // Test Ecu
//        sensDre();
    //
    // I don't know way but this is a fix ... ?
    // Only like this way base vars are initialized every single loop
    // or this is a bug in Arduino IDE
    if (amp->isSens()) {
        int foo;
        foo = getEcu();
        foo = getRpm();
        foo = getVss();
        foo = getAvrVss();
        foo = getAvrRpm();


        uint8_t vss = getVss();
        //
        // Detect time
        if (vss < 1) {
            vehicleStopped = 1;
        } else {
            vehicleStopped = 0;
        }
    }
    //
    // This method has private isSens handling
    sensDim();
    //
    // Speed alarms
    sensAlarms();

    //
    // Mark engine on
    if (CUR_RPM > 500) {
        _isEngineSens = true;
    }
    //
    // Close first loop
    if (isInitializedLoop) {
        isInitializedLoop = 0;
    }
}


/*******************************************************************
 * Detect Vss
 */
void CarSens::sensVss() {

    //
    // 200hz for 50km/h
    if (amp->isSens()) {

        //
        // Pass vss to global
        CUR_VSS = uint8_t(vssHitsCount / (getCorVss() + TRS_CORRECTION));
//        CUR_VSS = uint8_t(vssPulseLen / vssHitsCount) * 15;
//
//        Serial.print(vssHitsCount);
//        Serial.print(" / ");
//        Serial.print(vssPulseLen);
//        Serial.print(" / ");
//        Serial.print(CUR_VSS);
//        Serial.println();

        //
        // Calculate distance
        CUR_VDS = (vssHitsCount / (getCorDst() + TRS_CORRECTION)) + CUR_VDS;
        //
        // Odometer collection
        CUR_VDS_collection = vssHitsCount + CUR_VDS_collection;
        //
        // Reset pulse counter
        vssHitsCount = 0;
    }


//
// debug info
#if defined(DEBUG) && defined(DEBUG_VSS)
    if (DBG_CMD(amp, "vss")) {
        DBG_PS(CUR_VSS);
        DBG_PI(F("Hits contend: "))
        DBG_PD(vssHitsCount);
        DBG_PI(F("Distance contend: "))
        DBG_PD(CUR_VDS);
    }
#endif

}

/*******************************************************************
 * Detect RPMs
 */
void CarSens::sensRpm() {
    //
    // 200hz for 6000 rpm
    if (amp->isSens()) {
        //
        // Pass rpm to global
        CUR_RPM = uint16_t(rpmHitsCount * getCorRpm());
        //
        // Clear value
        rpmHitsCount = 0;
    }

    //
// debug info
#if defined(DEBUG) && defined(DEBUG_RPM)
    if (DBG_CMD(amp, "rpm")) {
        DBG_PS(CUR_RPM);
        DBG_PI(F("Hits contend: "))
        DBG_PD(rpmHitsCount);
    }
#endif
}

/*******************************************************************
* Detect Ecu
*/
void CarSens::sensEcu() {

    if (amp->isSens()) {
        //
        // Pass ecu to global
        CUR_ECU = uint32_t(ecuHitsCount * getCorEcu());
//
// debug info
#if defined(DEBUG) && defined(DEBUG_ECU)
        if (DBG_CMD(amp, "ecu")) {
            DBG_PS(CUR_ECU);
            DBG_PI(F("Hits contend: "))
            DBG_PD(ecuHitsCount);
        }
#endif


        ecuHitsCount = 0;
    }

}

//#define DEBUG_ALARM
/*******************************************************************
* Speed Alarms
*/
void CarSens::sensAlarms() {
#if defined(VSS_ALARM_ENABLED)

    if (amp->isMax()) {
        uint8_t currentSpeed = 0;
        //
        // Alarm high way
        if (CUR_VSS > VSS_ALARM_HWAY_SPEED) {
//            currentSpeed = VSS_ALARM_HWAY_SPEED;
        } else
            //
            // Alarm between villages
        if (CUR_VSS > VSS_ALARM_VWAY_SPEED) {
//            currentSpeed = VSS_ALARM_VWAY_SPEED;
        } else
            //
            // Alarm in city
        if (CUR_VSS > VSS_ALARM_CITY_SPEED) {
            currentSpeed = VSS_ALARM_CITY_SPEED;
        } else
            //
            // Zeroing the speed cursor
        if (CUR_VSS < VSS_ALARM_CITY_SPEED) {
            currentSpeed = 0;
            speedAlarmActive = 0;
        }

#if defined(DEBUG) && defined(DEBUG_ALARM)
        Serial.print("Speed alarm  ");
        Serial.print(CUR_VSS);
        Serial.print(" / ");
        Serial.print(currentSpeed);
        Serial.print(" / ");
        Serial.print(speedAlarmCursor);
        Serial.print(" / ");
        Serial.print(speedAlarmActive);
        Serial.println();
#endif
        //
        // Resolve alarm type
        if (currentSpeed != speedAlarmCursor) {
            //
            //
            if (currentSpeed > speedAlarmCursor) {
                speedAlarmActive++;
            } else {
                speedAlarmActive = 0;
                speedAlarmCursor = currentSpeed;
            }

            if (speedAlarmActive > VSS_ALARM_VERIFICATE) {
                mld->playSpeed();
                speedAlarmCursor = currentSpeed;
                speedAlarmActive = 0;
                Serial.println(F("Alarm play at "));
                Serial.println(CUR_VSS);

            }

        }


    }

#endif
}

/**
 * Clears peek data
 */
void CarSens::clearBuffer() {
    CUR_VSS = 0, CUR_RPM = 0, CUR_ECU = 0;
};

/**
 * Alarm cursor changer UP
 */
void CarSens::speedingAlarmsUp() {
    speedAlarmCursor++;
}

/**
 * Alarm cursor changer Down
 */
void CarSens::speedingAlarmsDw() {
    speedAlarmCursor--;
}

/**
 * Gets human time
 * @return char*
 */
char *CarSens::getHTm(float saved) {

    unsigned long x;
    uint8_t seconds, minutes, hours;
    x = millis() / 1000;
    seconds = x % 60;
    x /= 60;
    minutes = x % 60;
    x /= 60;
    hours = x % 24;

    int old[1];
    separateFloat(saved, old);
    /* 11 = len of clock time + 1 char for \0*/

    char *dspTime;
    sprintf(dspTime, "%02d:%02d", hours + old[0], minutes + old[1]);
    return dspTime;
};

void CarSens::getHTm(float saved, char *dspTime) {

    unsigned long x;
    uint8_t seconds, minutes, hours;
    x = millis() / 1000;
    seconds = x % 60;
    x /= 60;
    minutes = x % 60;
    x /= 60;
    hours = x % 24;

    int old[1];
    separateFloat(saved, old);
    /* 11 = len of clock time + 1 char for \0*/

    sprintf(dspTime, "%02d:%02d", hours + old[0], minutes + old[1]);

};

/**
 *  Sensing Screen DIM
 */
void CarSens::sensDim() {
    if (amp->isMid()) {
        uint16_t backLightLevel = (uint16_t) map(analogRead(pinScreenInput), 0, 1023, 0, 29);
        if (backLightLevel > 25) {
            backLightLevel = 25;
        }

        if (backLightLevel < 1) {
            backLightLevel = SCREEN_DEF_LIGHT;
            CUR_DIM_ON = false;
        }
        CUR_DIM_ON = true;
        analogWrite(pinScreenOutput, backLightLevel * 10);

    }

#if  defined(DEBUG) && defined(DEBUG_DIM)
    if (DBG_CMD(amp, "dim")) {
        DBG_PS(pinScreenOutput);
        DBG_PI(F("Hits contend: "))
        DBG_PD(backLightLevel);
    }
#endif

}

//uint16_t readPeekCollection;
//uint16_t readPulseLine;
//unsigned long lastRead;
//
//void sensEnt2() {
//
////    if (ampInt.isLow()) {
//        uint16_t read = analogRead(ENG_CLT_PIN);
//        if (read < readPeekCollection) {
//            readPulseLine++;
//        }else{
//            readPeekCollection = read;
//        }
//
////    }
//}

/**
 *  Engine temperature
 */
void CarSens::sensEnt() {

    if (amp->isSens()) {
        //
        // Make more measurements to calculate average
        smoothEngineTemp = analogRead(pinTemp) + smoothEngineTemp;
        indexEngineTemp++;
    }


    if (amp->isSecond()) {
        int val = (int) (smoothEngineTemp / indexEngineTemp);
        indexEngineTemp = 0;
        smoothEngineTemp = 0;

        // 220/215 -79C
        // 150 / 60
        // 200 / 70
        // cap 47uf 225 - 80C / 515 - 90C
        if(val > 224) {
            CUR_ENT = (uint8_t) map(val, 225, 500, 80, 90);
        }else{
            CUR_ENT = (uint8_t) map(val, 150, 224, 55, 79);
        }

    }

#if  defined(DEBUG) && defined(DEBUG_ENT)
    if (DBG_CMD(amp, "ent")) {
        DBG_PS(CUR_ENT);
        DBG_PI(F("Hits contend: "))
        DBG_PD((smoothEngineTemp / indexEngineTemp);
    }
#endif

}

/**
 *  Average speed & revolution
 *      and also maximum speed
 */
void CarSens::sensAvr() {

    uint16_t rpm = getRpm();
    uint8_t maxRpm = rpm / 100;
    //
    // Start average collection after run
    if (!initializeAverage && getVss() > 1) {
        initializeAverage = 1;
    }

    //
    // Check is initialize Average
    if (amp->isSens() && initializeAverage) {
        averageAllVssValues += getVss();
        if (rpm > 0) {
            averageAllRpmValues += rpm;
        }
        averageDivider += 1;
        //
        //  Resolve maximum speed reached
        if (maxReachedSpeed < getVss()) {
            maxReachedSpeed = getVss();
        }

        if (maxReachedRevs < maxRpm) {
            maxReachedRevs = maxRpm;
        }

    }

}

/**
 * Gets Average Vss
 */
uint16_t CarSens::getAvrVss() {
    return uint16_t(averageAllVssValues / averageDivider);
}

/**
 * Gets Average Rpm
 */
uint16_t CarSens::getAvrRpm() {
    return uint16_t(averageAllRpmValues / averageDivider);
}

/**
 * Gets Max reached revolutions / 100
 */
uint16_t CarSens::getMxmRpm() {
    return maxReachedRevs * 100;
}

/**
 * Gets Max reached speed
 */
uint8_t CarSens::getMxmVss() {
    return maxReachedSpeed;
}

#define DEBUG_TEMPERATURE_OU

// resistance at 25 degrees C
#define THERMISTOR_NOMINAL 5000
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURE_NOMINAL 24.2
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCO_EFFICIENT 3880 // NTC Thermistor Beta https://www.ametherm.com/thermistor/ntc-thermistor-beta
// the value of the 'other' resistor
#define SERIES_RESISTOR 20000

/**
 * Temperature sensing
 */
void CarSens::sensTmp() {

    float temperatureC;
    uint16_t liveValue;
    //
    // TODO use ResponsiveAnalogRead driver
    if (isInitializedLoop || amp->isSens() && this->getVss() > 6 && this->getVss() < 15) {
        //
        // Usable value
        liveValue = (uint16_t) analogRead(pinTmpOut);
        tmp_outCollection += liveValue;
        tmp_outIndex++;
    }




/*******************************     DS    temperature sensor ******************************************/
    //
    // Read inside temperature
#if defined(INSIDE_TEMPERATURE_DS)
    //
    // Since this library slow down main loop ... will increase temperature read to 1 minute
    if (amp->isMinute() || isInitializedLoop) {
        temperatureSensors.requestTemperatures();
        float currentInside = temperatureSensors.getTempCByIndex(0);
        if (currentInside > -100 && !isInitializedLoop || isInitializedLoop) {
            CUR_INS_TMP = currentInside;
        }
    }

#if  defined(DEBUG) && defined(DEBUG_TIN)
    if (DBG_CMD(amp, "tin")) {
        DBG_PS(temperatureSensors.getTempCByIndex(0));
    }
#endif

#endif


    /******************************* Car's temperature sensor ******************************************/

    /**
     * About GM Temperature sensor
     *      Temperature range to [°C]: 250
     *      Resistance [Ohm]: 5000
     * https://www.hackster.io/Marcazzan_M/how-easy-is-it-to-use-a-thermistor-e39321
     * ~ 40     °C      value 117
     * ~ 41     °C      value 120
     * ~ 36     °C      value 136
     * ~ 23+    °C      value 203 <- inside garage
     * ~ 22     °C      value 220
     * ~ 19     °C      value 225 <- hot engine
     * ~ 16     °C      value 238
     * ~ 20     °C      value 226
     * ~ 11.1   °C      value 281
     * ~ 8 (14) °C      value 295 <- guess
     * ~  15    °C      value 293 <- guess
     * ~  9     °C      value 335 <- guess
     */

//    float average, calculation;

    if (isInitializedLoop || amp->is10Seconds()) {
        liveValue = (uint16_t) analogRead(pinTmpOut);
        // Cold engine
        if (isInitializedLoop || this->getEngTmp() < 80 && this->getVss() == 0) {
            tmp_outCollection += liveValue;
            tmp_outIndex++;
        } else {
            tmp_outCollection += (liveValue + (tmp_outCollection / tmp_outIndex) * 8) / 9;
            tmp_outIndex++;
        }
    }


    if (isInitializedLoop || amp->isMinute() && tmp_outCollection > 0) {

        //
        // Get more precise average value
        float resistanceReadings = (tmp_outCollection / tmp_outIndex);

        //
        // uint16_t readings = uint16_t(tmp_outCollection / tmp_OutIndex * 10);
        // (map(readings, 4100, 1200, 15, 390) * 0.1)
        // (map(readings, 2810, 1170, 160, 405) * 0.1) <- use this corrected to 16°C
        // temperatureC = (map(readings, 2810, 1170, 167, 403) * 0.1);
        // temperatureC = (map(readings, 3445, 1170, 90, 400) * 0.1);
        //

        //
        // convert the value to resistance
        temperatureC = 1023 / resistanceReadings - 1;
        temperatureC = SERIES_RESISTOR / temperatureC;
        //
        // Convert to temperature
        temperatureC = temperatureC / THERMISTOR_NOMINAL;           // (R/Ro)
        temperatureC = log(temperatureC);                           // ln(R/Ro)
        temperatureC /= BCO_EFFICIENT;                              // 1/B * ln(R/Ro)
        temperatureC += 1.0 / (TEMPERATURE_NOMINAL + 273.15);       // + (1/To)
        temperatureC = 1.0 / temperatureC;                          // Invert
        temperatureC -= 273.15;                                     // remove Kelvin (temperature resistance)
        // temperatureC = calculation;

        //
        // Wind chill patch
#if defined (WIND_CHILD)
        // TODO tests for verification
        // WIND CHILL calculations
        float avrSpeed = this->getAvrVss();
        if (this->getVss() > 3 && avrSpeed > 5 && temperatureOutFirst < 18) {
        float Rcv = 1.34756; // resistor correction value
            //
            // Formula for vehicle speed coefficient
            uint8_t vehicleSpeed = (uint8_t) ((this->getVss()) + avrSpeed) / 2;
            /*
                 temperatureC    is temperature
                 vehicleSpeed    is wind speed
                ^0.16            is an exponent.

                If using ºF and mph:    Wind chill temperature = 35.74 + 0.6215T - 35.75V0.16 + 0.4275TV0.16
                If using ºC and km/h:   Wind chill temperature = 13.12 + 0.6215T - 11.37V0.16 + 0.3965TV0.16

             */
            //
            // Formula for Wind Chill reversed to remove effect of wind
            temperatureC = ((13.12 * Rcv) - 0.6215 / temperatureC - 11.37 / pow(vehicleSpeed, 0.16) -
                            0.3965 / temperatureC / pow(vehicleSpeed, 0.16));
        }
#endif
        //
        // Keep current value for more smooth data
        tmp_outCollection = (uint16_t) floor(resistanceReadings) * 3;
        tmp_outIndex = 3;
        //
        // Pass value to global
        CUR_OUT_TMP = temperatureC;
    }
//
// debug info
#if defined(DEBUG) && defined(DEBUG_TOU)
    if (DBG_CMD(amp, "tou")) {
        DBG_PS(temperatureC);
        DBG_PI(F("Hits contend: "))
        DBG_PD(resistanceReadings);
        DBG_PI(F("Live contend: "))
        DBG_PD(liveValue);
    }
#endif

}

/**
 * Detect delta time for consumption
 */
void CarSens::sensDlt() {
    // time elapsed
    if (amp->isSens()) {
        unsigned long time_now;
        time_now = millis();
        CONS_DELTA_TIME = time_now - sensDeltaCnsOldTime;
        sensDeltaCnsOldTime = time_now;
    }
}

/**
 * Trip consumption
 */
void CarSens::sensCns() {

    // add MAF result to trip
    // we want fuel usedMenu in ÂµL
    // maf gives grams of air/s
    // divide by 100 because our MAF return is not divided!
    // divide by 14.7 (a/f ratio) to have grams of fuel/s
    // divide by 730 to have L/s
    // mul by 1000000 to have ÂµL/s

    // Formula:
    //      MPG = (14.7 * 6.17 * 454 * VSS * 0.621371) / (3600 * MAF / 100)
    //  We using short way:
    //      LKM =  MAF * time /  <fuel sum>
    // Since we want only consumption VSS is skipped

    if (amp->isSens()) {
        long deltaFuel = 0;
//        Serial.println(getCnsFuelVal());
        if (CUR_ECU > 0) {
            deltaFuel = (CUR_ECU * FUEL_ADJUST * CONS_DELTA_TIME) / getCnsFuelVal();
            // Direct correction in constant
        }

        setConsumedFuel(deltaFuel);
        //
        // Collect wasted fuel
        if (this->getVss() < 1) {
            FUEL_WASTED = FUEL_WASTED + deltaFuel;
        }
    }


}

/**
 * Instance Fuel Consumption
 */
void CarSens::sensIfc() {
    float cons;
    unsigned long delta_dist;


    // Simulation of formula
    //
    // divide MAF by 100 because our function return MAF*100
    // but multiply by 100 for double digits precision
    // divide MAF by 14.7 air/fuel ratio to have g of fuel/s
    // divide by 730 (g/L at 15°C) according to Canadian Gov to have L/s
    // multiply by 3600 to get litre per hour
    // formula: (3600 * MAF) / (14.7 * 730 * VSS)



    if (amp->isSec()) {
        if (CUR_VSS < CONS_TGL_VSS) {
            cons = ((CUR_ECU * getIfcFuelVal()) / (MILLIS_PER_HR / 10) * 2);
        } else {
            cons = ((CUR_ECU * getIfcFuelVal()) / (CUR_VSS * 10000)) * 0.1;
        }
        // 4329


        if (cons > 99) {
            cons = 99;
        }
        FUEL_INST_CONS = cons;
        //
        // Average consumption collection
        collectionIfc += cons;
        indexIfc++;
    }
    // deprecated
    // Average IFC for 5 sec
    // Keep last value as 1:3 rate

    if (amp->isSecond()) {
        //
        // Average instance fuel consumption for 5 sec
        FUEL_AVRG_INST_CONS = float(collectionIfc / indexIfc);//
        collectionIfc = FUEL_AVRG_INST_CONS;
        indexIfc = 1;

    }


}

/**
 * Car gear
 * TODO Needs testing
 */
uint8_t CarSens::getGear() {
    float Ratio;

    int Rpm = this->CUR_RPM;
    int CarSpeed = this->CUR_VSS;

    if (CarSpeed != 0) {

        Ratio = (Rpm * CAR_GEAR_Pi * CAR_GEAR_Dia * 60) / (CarSpeed * CAR_GEAR_FD * 1000000);

        carGearNum = 7;

        if ((-0.1 < Ratio - CAR_GEAR_G1) and (Ratio - CAR_GEAR_G1 < 0.1)) carGearNum = 1;
        if ((-0.1 < Ratio / 6 - CAR_GEAR_G2) and (Ratio - CAR_GEAR_G2 < 0.1)) carGearNum = 2;
        if ((-0.1 < Ratio - CAR_GEAR_G3) and (Ratio - CAR_GEAR_G3 < 0.1)) carGearNum = 3;
        if ((-0.1 < Ratio - CAR_GEAR_G4) and (Ratio - CAR_GEAR_G4 < 0.1)) carGearNum = 4;
        if ((-0.1 < Ratio - CAR_GEAR_G5) and (Ratio - CAR_GEAR_G5 < 0.1)) carGearNum = 5;
        if ((-0.1 < Ratio - CAR_GEAR_G6) and (Ratio - CAR_GEAR_G6 < 0.1)) carGearNum = 6;
    } else carGearNum = 0;

    return carGearNum;
}

/**
 * Sets Fuel consumed by engine
 * @param value
 */
void CarSens::setConsumedFuel(long value) {

    //
    // Recording wasted fuel
    if (CUR_VSS > CONS_TGL_VSS) {
        if (getFuelState() == 0) {
            FL_WST_DEF = FL_WST_DEF + value;
        } else {
            FL_WST_ADT = FL_WST_ADT + value;
        }
    }

    //
    //  Recording usedMenu fuel
    if (getFuelState() == 0) {
        FL_CNS_DEF = FL_CNS_DEF + value;
    } else {
        FL_CNS_ADT = FL_CNS_ADT + value;
    }

}

/**
 *
 */
void CarSens::sensBkt() {
    if (breakTimeStart == 0 && digitalRead(pinBreaks) == HIGH) {
        breakTimeStart = millis();
    }

    if (breakTimeStart > 0 && digitalRead(pinBreaks) == LOW) {
        BREAK_TIME = BREAK_TIME + ((millis() - breakTimeStart) / MILLIS_PER_SC);
        breakTimeStart = 0;
    }
}

/**
 *
 */
void CarSens::sensTnk() {
    if (amp->isSens()) {
        //
        // Make more measurements to calculate average
        smoothFuelTank = analogRead(pinFulTnk) + smoothFuelTank;
        indexFuelTank++;
    }

    uint16_t val;
    if (amp->isSecond()) {
        val = (uint16_t) (smoothFuelTank / indexFuelTank);
        indexFuelTank = 0;
        smoothFuelTank = 0;

        //
        //
        // cap 47uf
        // lamp 595 (10% / 5.2l)
        // fill 546 - 5liters
        FUEL_TANK = map(val, 595, 546, 52, 57) / 10;
    }
//
// debug info
#if defined(DEBUG) && defined(DEBUG_TNK)
    if (DBG_CMD(amp, "tnk")) {
        DBG_PS(FUEL_TANK);
        DBG_PI(F("Hits contend: "))
        DBG_PD(val);
    }
#endif

}



//ARDUINO_MID_CAR_SENS_H
#endif



//
// Created by Angel Zaprianov on 19.10.2016 г..
//
#include <Arduino.h>
#include "../MID.h"
#include "AmpTime.h"
#include "MainFunc.h"
//#define ECU_SENS_DEBUG
//
#ifndef ARDUINO_MID_CAR_SENS_H
#define ARDUINO_MID_CAR_SENS_H

// TODO this is test mode
#ifndef ADT_FUEL_SYSTEM_I2C
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
// Version of MID plug driver
#define MID_CAR_SENS_VERSION 0.1
//
// Show information from consumption
#define DEBUG_CONS_INFO
//
// Speeding alarm
#define VSS_ALARM_CITY_SPEED  60 // km
#define VSS_ALARM_VWAY_SPEED  100 // km
#define VSS_ALARM_HWAY_SPEED  140 // km
//#define VSS_ALARM_ENABLED // Comment to disable speeding alarms
#define VSS_ALARM_AWAITING 5
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
#define ECU_CORRECTION 75.3// 92.23  //  75 // 147.23 ///  346 /// to high 692
//
// Speed correction
#define VSS_CORRECTION 1.6  //    fast 3.767
//
// Revs correction
#define RPM_CORRECTION 75   //    fast 33.767
//
// Distance correction
#define DST_CORRECTION  30800.58  // Tested >30800.58 //  15383.29  //   <sens:200> 15383.29
//
#define TRS_CORRECTION 0 // 0.064444 a proximity  6(~6)%
//
//#define VSD_SENS_DEBUG;
#define SCREEN_DEF_LIGHT 75 // 22
//
// Screen default value
#define SCREEN_GO_TO_DEF 15

#endif


#ifndef CAR_SENS_CUSTOM_FUELS
//
// Default fuel state
#define DEFAULT_FUEL_STATE 1
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
// // Note: this value is set without detection of fuel switch (mixed with benzine)
#define FUEL_LPG_IFC 4329
#define FUEL_LPG_CNS 8316  // 15.4*540 = 8316
#define LPG_SWTC_PIN 7
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
    unsigned int ifc;
    unsigned long cns;
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
#define  DEBUG_TEMPERATURE_OU
//#define  DEBUG_TEMPERATURE_IN


//
// External Temperature
#define EXT_TMP_VSS  5
#define EXT_TMP_RST  10000
#define EXT_TMP_MSR  4      // 10 // measured temperature
#define EXT_TMP_MVL  409    // 328 // measured value of temperature

#if defined(INSIDE_TEMPERATURE_DS)

#include <OneWire.h>
#include <DallasTemperature.h>
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
    //
    // Engine temperature pin
    uint8_t pinTemp;
    uint8_t indexEngineTemp;
    //
    boolean vehicleStopped = false;
    boolean _isEngineSens = false;
    boolean _isVehicleSens = false;
    uint8_t pinScreenInput, pinScreenOutput;
    //
    // Detect fuel switch
    uint8_t FUEL_STATE;
    uint8_t pinLpgClock, pinLpgData;
    //
    //
    uint8_t carGearNum = 0;
    Fuel FUEL_PARAM_DEF, FUEL_PARAM_ADT;
    uint16_t smoothEngineTemp;
    //
    // Screen back light vars
    uint8_t backLightLevel = 0; // Container of current level
    uint8_t CUR_ENT; // Engine temperature

    //
    // Car's reached ...
    uint8_t maxReachedSpeed = 0, maxReachedRevs = 0;
    //
    // Human Results
    uint16_t CUR_VSS, CUR_RPM;
    uint32_t CUR_ECU;
    //
    // Instant Fuel consumption counter
    uint16_t indexIfc;
    uint16_t temperatureOutIndex = 0;
    //
    //
    int pushLpgIndex = 0;
    //
    // LPG tank
    int CUR_LTK;
    //
    // Speeding alarms
    unsigned int speedAlarmCursor = 1, speedAlarmCounter = 0;
    //
    // Fuel detection
    unsigned int pullLpgIndex = 0, combLpgIndex = 0;
    //
    //
    float BRAK_TIME = 0;
    unsigned long breakTimeStart = 0;
    //
    //
    unsigned long temperatureOutCollection = 0;
    //
    //
    float FUEL_INST_CONS;
    float FUEL_WASTED = 0;
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


    unsigned long collectionIfc;
    unsigned long lastDetectionLpg = 0;
    //
    // Car's average
    unsigned long averageAllVssValues = 0;
    unsigned long averageAllRpmValues = 0;
    /**
     * Handle playEntry dim
     */
    unsigned long lastReadValueDim = 0;
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
    void speedingAlarms();

    boolean isAlrWtn();

    boolean isAlrCur(unsigned int curSet);

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

public:

    float getCorVss() {
//        return VSS_CORRECTION; //TODO Remove after test
        return (savedData.sens_vss > 0) ? savedData.sens_vss : (float) VSS_CORRECTION;
    }

    float getCorRpm() {
//        return RPM_CORRECTION; //TODO Remove after test
        return (savedData.sens_rpm > 0) ? savedData.sens_rpm : (float) RPM_CORRECTION;
    }

    float getCorDst() {
//        return DST_CORRECTION; // Remove after test
        return (savedData.sens_dst > 0) ? savedData.sens_dst : (float) DST_CORRECTION;
    }

    float getCorEcu() {
//        return ECU_CORRECTION; //TODO Remove after test
        return (savedData.sens_ecu > 0) ? savedData.sens_ecu : (float) ECU_CORRECTION;
    }

/**
 *
 */
    void setSave(SavedData data) {
        savedData = data;
    }

    int getLpgPull();

    int getLpgPush();

#ifdef ADT_FUEL_SYSTEM_I2C

//    void listenerI2cLpg(I2cSimpleListener *i2c);

#endif

    /**
 * Construct class
  * @param ampInt
  */
    CarSens(AmpTime &ampInt) : amp(&ampInt) {

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
     *  Makes switch between fuel lines
     */
    void switchCurrentFuel(void);

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
    void setupVehicle(uint8_t pinVss, uint8_t pinRpm, uint8_t pinEcu, uint8_t pinTmp, uint8_t pinBrk);

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
      * @param pinTank
      * @param pinSwitch
      */
    void setupAdtFuel(uint8_t pinTank, uint8_t pinSwitch);

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
     * Gets engine temperature
     *  resistor 5.6k  to GND || pull-up resistor 4.2k from MID plug pin 31 with 10mf cap
     */
    inline int getEngTmp() { return CUR_ENT; /* Range from 70 to 120*/}

    /**
     * Gets current Vss
     */
    inline uint16_t getVss() { return CUR_VSS; }

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
    inline float getBrakTime() { return BRAK_TIME; }

    /**
     *  Gets travel time
     */
    int long getTime() { return CUR_VTT; }

    /**
     *  Gets Human time
     */
    char *getHTm(float saved = 0);

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

    uint16_t getMxmRpm();

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



/**
 * Interrupt function Vss
 */
void EngSens_catchVssHits() {
    vssHitsCount++;
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
};

/**
  * Setup VSS
  * @param pinTarget
  */
void CarSens::setupVssSens(uint8_t pinTarget) {
    pinMode(pinTarget, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pinTarget), EngSens_catchVssHits, HIGH);
};

/**
  * Setup Ecu
  * @param pinTarget
  */
void CarSens::setupEcuSens(uint8_t pinTarget) {
    pinMode(pinTarget, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pinTarget), EngSens_catchEcuHits, HIGH);
};

/**
 *  Setup engine pins
 * @param pinVss
 * @param pinRpm
 * @param pinEcu
 * @param pinTmp
 */
void CarSens::setupVehicle(uint8_t pinVss, uint8_t pinRpm, uint8_t pinEcu, uint8_t pinTmp, uint8_t pinBrk) {
    setupRpmSens(pinRpm);
    setupVssSens(pinVss);
    setupEcuSens(pinEcu);
    //
    // Engine temperature
    pinMode(pinTmp, INPUT);
    pinTemp = pinTmp;
    pinBreaks = pinBrk;
};

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
};

/**
 * Gets calculated constant for consumption
 * @return
 */
long CarSens::getCnsFuelVal() {
    if (getFuelState() == 0) return FUEL_PARAM_DEF.cns;
    if (getFuelState() == 1) return FUEL_PARAM_ADT.cns;
};

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
};

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
};

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
    temperatureSensors.begin();
#endif
};

/**
 * Setup additional fuel line pins
 * @param pinTank
 * @param pinSwitch
 */
void CarSens::setupAdtFuel(uint8_t pinTank, uint8_t pinSwitch) {

    FUEL_STATE = DEFAULT_FUEL_STATE;

    pinMode(pinTank, INPUT);
    pinMode(pinSwitch, INPUT);
    //
    //
    pinLpgClock = pinSwitch;
    pinLpgData = pinTank;
}


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
        sensDim();
    }


    //
    // Sens display Dim more frequently by skipping Sens loop
    if (amp->isLow() && backLightLevel > SCREEN_GO_TO_DEF && !amp->isSens()) {
        sensDim();
    }
    //
    // Mark engine on
    if (CUR_RPM > 500) {
        _isEngineSens = true;
    }
    //
    // Close first loop
    if(isInitializedLoop){
        isInitializedLoop = 0;
    }
}


/*******************************************************************
 * Detect Vss
 */
void CarSens::sensVss() {

    if (amp->isSens()) {

        //
        // Pass vss to global
        CUR_VSS = uint8_t(vssHitsCount / (getCorVss() + TRS_CORRECTION));
        //
        // Calculate distance
        CUR_VDS = (vssHitsCount / (getCorDst() + TRS_CORRECTION)) + CUR_VDS;

        CUR_VDS_collection = vssHitsCount + CUR_VDS_collection;


//
// debug info
#if defined(VSS_SENS_DEBUG) || defined(GLOBAL_SENS_DEBUG)
        Serial.print("\n");
        Serial.print(" vss count:  \t");
        Serial.print(vssHitsCount);
        Serial.print(" vss is:  \t");
        Serial.print(vssHitsCount * VssCorrection);
        Serial.print("\n");
#endif
        //
        // Reset pulse counter
        vssHitsCount = 0;
    }
    //
    // Alarms
    speedingAlarms();
    //
    // Distance
#if defined(VSD_SENS_DEBUG) || defined(GLOBAL_SENS_DEBUG)
    if (amp->isBig()) {
        Serial.print("Counted VSD is: ");
        Serial.println(CUR_VDS);
    }
#endif

}

/*******************************************************************
 * Detect RPMs
 */
void CarSens::sensRpm() {

    if (amp->isSens()) {
        //
        // Pass rpm to global
        CUR_RPM = uint16_t(rpmHitsCount * getCorRpm());

//
// debug info
#if defined(RPM_SENS_DEBUG) || defined(GLOBAL_SENS_DEBUG)
        Serial.print("\n");
        Serial.print(" RPM count:  \t");
        Serial.print(rpmHitsCount);
        Serial.print(" RPM is:  \t");
        Serial.print(rpmHitsCount * RpmCorrection);
        Serial.print("\n");
#endif
        //
        // Clear value
        rpmHitsCount = 0;
    }
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
#if defined(ECU_SENS_DEBUG) || defined(GLOBAL_SENS_DEBUG)
        Serial.print("\n");
        Serial.print(" ecu count:  \t");
        Serial.print(ecuHitsCount);
        Serial.print(" ecu is:  \t");
        Serial.print(ecuHitsCount * ECU_CORRECTION);
        Serial.print("\n");
#endif

        ecuHitsCount = 0;
    }

}


boolean CarSens::isAlrWtn() {
    if (speedAlarmCounter >= VSS_ALARM_AWAITING) {
        return 1;
    }
    return 0;
}

boolean CarSens::isAlrCur(unsigned int curSet) {
    return speedAlarmCursor == curSet;
};

/*******************************************************************
* Speed Alarms
*/
void CarSens::speedingAlarms() {
#if defined(VSS_ALARM_ENABLED)

    boolean activeAlarm = false;

    if (speedAlarmCursor < DISABLE_SPEED_AL) {
        speedAlarmCursor = ENABLE_SPEED_HW;
    }

    //
    // Alarm city
    if (amp->is2Seconds() && CUR_VSS > VSS_ALARM_CITY_SPEED && isAlrCur(ENABLE_SPEED_CT)) {
        if (!isAlrWtn()) {
            speedAlarmCounter++;
            tone(TONE_ADT_PIN, 4000, 150);
        }
        activeAlarm = true;

    }
    //
    // Alarm village way
    if (amp->is5Seconds() && CUR_VSS > VSS_ALARM_VWAY_SPEED && isAlrCur(ENABLE_SPEED_VW)) {
        if (!isAlrWtn()) {
            speedAlarmCounter++;
            tone(TONE_ADT_PIN, 4000, 200);
        }
        activeAlarm = true;
    }
    //
    // Alarm high way
    if (amp->isMinute() && CUR_VSS > VSS_ALARM_HWAY_SPEED && isAlrCur(ENABLE_SPEED_HW)) {
        if (!isAlrWtn()) {
            speedAlarmCounter++;
            tone(TONE_ADT_PIN, 4000, 200);
        }
        activeAlarm = true;
        speedAlarmCounter++;
    }

    if (speedAlarmCursor > ENABLE_SPEED_HW) {
        speedAlarmCursor = DISABLE_SPEED_AL;
    }

    //
    // Reset alarm set if speed is lower
    if (isAlrWtn() && amp->is5Seconds() && !activeAlarm) {
        speedAlarmCounter = 0;
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

    char dspTime[6] = "00:00";
    unsigned long tmSec;
    int tmMin, tmHrs;

    tmSec = millis() / 1000;
    tmMin = int(tmSec / 60);
    tmHrs = tmMin / 60;

    int old[1];
    separateFloat(saved, old);
    /* 11 = len of clock time + 1 char for \0*/

    sprintf(dspTime, "%02d:%02d", tmHrs + old[0], tmMin + old[1]);
    return dspTime;
};

/**
 *  Sensing Screen DIM
 */
void CarSens::sensDim() {

    boolean defaultActive = 0;

    backLightLevel = (uint8_t) map(analogRead(pinScreenInput), 0, 1023, 0, 255);

//    Serial.println(pinScreenOutput);
    if (backLightLevel < SCREEN_GO_TO_DEF) {
        backLightLevel = SCREEN_DEF_LIGHT;
        defaultActive = 1;
    } else {
        defaultActive = 0;
    }

    if (lastReadValueDim != backLightLevel && backLightLevel > 0) {
        lastReadValueDim = backLightLevel;

        if (defaultActive == 0) {
            backLightLevel = backLightLevel - SCREEN_DEF_LIGHT;
        }
        analogWrite(pinScreenOutput, backLightLevel);
    }
}


#ifdef ADT_FUEL_SYSTEM_I2C

#endif

int CarSens::getLpgPull() {
    return pullLpgIndex;
}

int CarSens::getLpgPush() {
    return pushLpgIndex;
}

/**
 *  Engine temperature
 */
void CarSens::sensEnt() {

    if (amp->isBig()) {
        //
        // Make more measurements to calculate average
        smoothEngineTemp = analogRead(pinTemp) + smoothEngineTemp;
        indexEngineTemp++;
    }


    if (amp->isMax()) {
        //
        // 80 = 390
        // 82 = 420
        // 90 = 630
        int val = (int) (smoothEngineTemp / indexEngineTemp);
        indexEngineTemp = 0;
        smoothEngineTemp = 0;
        //
        // Seems this values do not comes from coolant temperature sensor
        // So this is best wey to determinate them...
        if (val < 390) {
            //
            // Mapping below 80deg C temperature
            CUR_ENT = (uint8_t) map(val, 0, 390, -4, 80);
        } else { // old 385
            CUR_ENT = (uint8_t) map(val, 390, 620, 80, 90);
        }
        //
        // Over heating ALARM
        if (amp->isSecond() && CUR_ENT > 95) {
            tone(TONE_ADT_PIN, 2000, 350);
        }
#ifdef DEBUG_ENG_TEMP

        Serial.print("Engine temperature: ");
        Serial.print(val);
        Serial.print(" / result:");
        Serial.println(CUR_ENT);
#endif
    }
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

/**
 * Temperature sensing
 */
void CarSens::sensTmp() {

/*******************************     DS    temperature sensor ******************************************/
    //
    // Read inside temperature
#if defined(INSIDE_TEMPERATURE_DS)

#if defined(DEBUG_TEMPERATURE_IN)
    if (ampInt.isBig()) {
        temperatureSensors.requestTemperatures();
        Serial.print("Dallas temperature: \t");
        Serial.println(temperatureSensors.getTempCByIndex(0)); // Why "byIndex"?
    }
#endif

    if (amp->isBig()) {
        temperatureSensors.requestTemperatures();
        CUR_INS_TMP = temperatureSensors.getTempCByIndex(0);
    }
#endif
//    temperatureSensors.requestTemperatures();
//    temperatureSensors.getTempCByIndex(0);


    /******************************* Car's temperature sensor ******************************************/
    float temperatureC;
    /**
     * About GM Temperature sensor
     *      Temperature range to [°C]: 250
     *      Resistance [Ohm]: 5000
     * https://www.hackster.io/Marcazzan_M/how-easy-is-it-to-use-a-thermistor-e39321
     * ~ 40°C value 117
     * ~ 41°C value 120
     * ~ 36°C value 136
     * ~ 22°C value 203
     * ~ 19°C value 2250
     */
    temperatureOutCollection += (uint16_t) analogRead(TMP_PIN_OUT);
    temperatureOutIndex++;
    if (isInitializedLoop || amp->is5Seconds()) {
        //
        // Get more precise average value
        uint16_t readings = ((temperatureOutCollection / temperatureOutIndex) * 10);

        temperatureC = (map(readings, 2200, 1200, 195, 410) * 0.1);
        temperatureOutCollection = 0;
        temperatureOutIndex = 0;


#if defined(DEBUG_TEMPERATURE_OU)
        Serial.print("Read Temp  value: ");
        Serial.print(readings);
        Serial.print(" | calculation:");
        Serial.println(temperatureC);

#endif

        //
        // Pass value to global
        CUR_OUT_TMP = temperatureC;
    }

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
            cons = ((CUR_ECU * CONS_DELTA_TIME) / (getIfcFuelVal()));
        } else {
            cons = ((CUR_ECU * CONS_DELTA_TIME) / (CUR_VSS * getIfcFuelVal()));
        }
        // 4329
#if defined(DEBUG_CONS_INFO) || defined(GLOBAL_SENS_DEBUG)


        Serial.print(F("\n\n Fuel Cons  | INS: "));
        Serial.print(FUEL_INST_CONS);
        Serial.print(F(" || TTL: "));
        Serial.print(TTL_FL_CNS);
        Serial.print(F(" || ECU: "));
        Serial.print(CUR_ECU);
        Serial.print(F(" || CNS: "));
        Serial.print(cons);
        Serial.print(F("\n\n "));

#endif

        if (cons > 99) {
            cons = 99;
        }
        FUEL_INST_CONS = cons;
        //
        // Average consumption collection
        collectionIfc += (cons);
        indexIfc++;
    }
    // deprecated
    // Average IFC for 5 sec
    // Keep last value as 1:3 rate

    if (amp->is2Seconds()) {
        //
        // Average instance fuel consumption for 5 sec
        FUEL_AVRG_INST_CONS = (collectionIfc / indexIfc);//
        indexIfc = 0;
        collectionIfc = 0;
#if defined(DEBUG_CONS_INFO) || defined(GLOBAL_SENS_DEBUG)
        Serial.print(F("\n\n Fuel Cons  | INS: "));
        Serial.print(FUEL_INST_CONS);
        Serial.print(F(" || AVR: "));
        Serial.print(FUEL_AVRG_INST_CONS);
        Serial.print(F(" || TTL: "));
        Serial.print(TTL_FL_CNS);
        Serial.print(F(" || ECU: "));
        Serial.print(CUR_ECU);
        Serial.print(F(" || CNS: "));
        Serial.print(cons);
        Serial.print(F("\n\n "));
#endif
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

//
// TODO driver to detect fuel level and fuel type
//
unsigned long dumpFuelSwitchCnt = 0;
unsigned long dumpFuelSwitchLvl = 0;
unsigned long dumpFuelSwitchSwt = 0;


void CarSens::sensBkt() {
    if (breakTimeStart == 0 && digitalRead(pinBreaks) == HIGH) {
        breakTimeStart = millis();
    }

    if (breakTimeStart > 0 && digitalRead(pinBreaks) == LOW) {
        BRAK_TIME = BRAK_TIME + ((millis() - breakTimeStart) / MILLIS_PER_SC);
        breakTimeStart = 0;
    }
}

/**
 * Makes fuel switch
 */
void CarSens::switchCurrentFuel() {
    unsigned long currentTime = millis();
    if (lastDetectionLpg + 1000 > currentTime) {
        lastDetectionLpg = currentTime;

        if (FUEL_STATE == 1) {
            FUEL_STATE = 0;
        } else {
            FUEL_STATE = 1;
        }
        Serial.print(F("CHANGED FUEL STATE TO "));
        Serial.println(FUEL_STATE);
    }
}

//ARDUINO_MID_CAR_SENS_H
#endif



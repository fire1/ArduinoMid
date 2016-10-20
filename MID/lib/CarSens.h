//
// Created by Angel Zaprianov on 19.10.2016 г..
//
#include <Arduino.h>
#include "TimeAmp.h"

#ifndef ARDUINOMID_ENGSENS_H
#define ARDUINOMID_ENGSENS_H
//
// City Speed alarm
#define VSS_ALARM_CITY_SPEED  61 // km
#define VSS_ALARM_VWAY_SPEED  101 // km
#define VSS_ALARM_HWAY_SPEED  141 // km
#define VSS_ALARM_ENABLED // Comment to disable speeding alarms
//
// Sensor correctors
#define VSS_CORRECTION 3.673788 // original value is 3.609344 my tires are smaller so + 0.064444
#define RPM_CORRECTION 32.767 // RPM OBD PID: 16,383.75 [*2] || [old: 32.8]
#define ECU_CORRECTION 1.8
#define TRS_CORRECTION 0.064444 // a proximity  6,4(~6.5)%
#define VSD_SENS_DEBUG;


static void EngSens_catchRpmHits();

static void EngSens_catchVssHits();

static void EngSens_catchEcuHits();

//
// Hint counters
volatile int vssHitsCount, rpmHitsCount, ecuHitsCount;

class CarSens {
private:
    //
    //
    // Take a pointer to time amplitude instance
//    TimeAmp _amp;

    //
    // Speeding alarm modes
    boolean ENABLE_SPEED_CT = 1, ENABLE_SPEED_VW = 0, ENABLE_SPEED_HW = 0;
    //
    // Engine temperature pin
    uint8_t pinTemp;

    int
    //
    // Human Results
            CUR_VSS, CUR_RPM, CUR_ECU;
    unsigned long int CUR_VDS;

    /**
     * Handles speeding alarms
     */
    void speedingAlarms() {
#if defined(VSS_ALARM_ENABLED)
        //
        // Alarm speeding at city
        if (ampInt.isSec() && CUR_VSS > VSS_ALARM_CITY_SPEED && ENABLE_SPEED_CT) {
            tone(ADT_ALR_PIN, 4000, 500);
        }

        if (ampInt.isSec() && CUR_VSS > VSS_ALARM_VWAY_SPEED && ENABLE_SPEED_VW) {
            tone(ADT_ALR_PIN, 4000, 500);
        }

        if (ampInt.isSec() && CUR_VSS > VSS_ALARM_HWAY_SPEED && ENABLE_SPEED_HW) {
            tone(ADT_ALR_PIN, 4000, 500);
        }
#endif
    }


protected:

    /**
      * Setup RPM
     */
    void setupRpmSens(uint8_t pinTarget) {
        pinMode(pinTarget, INPUT_PULLUP);
        attachInterrupt(digitalPinToInterrupt (pinTarget), EngSens_catchRpmHits, FALLING);
    }

    /**
      * Setup VSS
     */
    void setupVssSens(uint8_t pinTarget) {
        pinMode(pinTarget, INPUT_PULLUP);
        attachInterrupt(digitalPinToInterrupt (pinTarget), EngSens_catchVssHits, FALLING);
    }

    /**
      * Setup Ecu
     */
    void setupEcuSens(uint8_t pinTarget) {
        pinMode(pinTarget, INPUT_PULLUP);
        attachInterrupt(digitalPinToInterrupt(pinTarget), EngSens_catchEcuHits, FALLING);
    }


    void sensVss();

    void sensRpm();

    void sensEcu();


public:

    /**
     * Clear peak
     */
    void clearBaseData() {
        CUR_VSS = 0, CUR_RPM = 0, CUR_ECU = 0;
    }

    /**
     * Setup engine
     */
    void setupEngine(uint8_t pinVss, uint8_t pinRpm, uint8_t pinEcu, uint8_t pinTmp) {
        setupRpmSens(pinRpm);
        setupVssSens(pinVss);
        setupEcuSens(pinEcu);
        //
        // Engine temperature
        pinMode(pinTmp, INPUT);
        pinTemp = pinTmp;
    }

    /**
     * Sets time amplitudes
     */
//    void setTimeAmp(TimeAmp timeAmplitudes) {
//        _amp = timeAmplitudes;
//    }

    /**
     * Gets engine temperature
     */
    int getTmp() {
        return (analogRead(pinTemp) / 4 - 41);
    }

    /**
     * Gets current Vss
     */
    int getVss() {
        return CUR_VSS;
    }

    /**
     * Gets current Rpm
     */
    int getRpm() {
        return CUR_RPM;
    }

    /**
     * Gets current Ecu
     */
    int getEcu() {
        return CUR_ECU;
    }

    /**
     *  Gets current Distance
     */
    float getDst() {
        /* my tires are smaller then original ... so my number must be lower, original must be / 16093.44 // one mile */
        float km = CUR_VDS / (16093.44 - TRS_CORRECTION);

        if (km <= 0) {
            km = 0;
        }
        return km;
    }

    /**
     *  Listen sensors
     */
    void listener() {
        //
        // No Interrupts
        cli();
        sensVss();
        sensRpm();
        sensEcu();
        //
        // Interrupts
        sei();
    }

};


void EngSens_catchVssHits() {
    vssHitsCount++;
}

void EngSens_catchRpmHits() {
    rpmHitsCount++;
}

void EngSens_catchEcuHits() {
    ecuHitsCount++;
}


/*******************************************************************
 * Detect Vss
 */
void CarSens::sensVss() {

    if (ampInt.isSens()) {

        //
        // Pass vss to global
        CUR_VSS = int(vssHitsCount / (VSS_CORRECTION + TRS_CORRECTION));
        CUR_VDS = vssHitsCount + CUR_VDS;

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
    if (ampInt.isBig()) {
        Serial.print("Counted VSD is: ");
        Serial.println(CUR_VDS);
    }
#endif

}


/*******************************************************************
 * Detect RPMs
 */
void CarSens::sensRpm() {

    if (ampInt.isSens()) {
        //
        // Pass rpm to global
        CUR_RPM = int(rpmHitsCount * RPM_CORRECTION);

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

    if (ampInt.isSens()) {
        //
        // Pass ecu to global
        CUR_ECU = int(ecuHitsCount * ECU_CORRECTION);
//
// debug info
#if defined(ECU_SENS_DEBUG) || defined(GLOBAL_SENS_DEBUG)
        Serial.print("\n");
        Serial.print(" ecu count:  \t");
        Serial.print(ecuHitsCount);
        Serial.print(" ecu is:  \t");
        Serial.print(ecuHitsCount * EcuCorrection);
        Serial.print("\n");
#endif

        ecuHitsCount = 0;
    }

}


#endif //ARDUINOMID_ENGSENS_H

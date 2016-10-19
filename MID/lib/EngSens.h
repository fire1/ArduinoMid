//
// Created by Angel Zaprianov on 19.10.2016 г..
//
#include <Arduino.h>

#ifndef ARDUINOMID_ENGSENS_H
#define ARDUINOMID_ENGSENS_H
//
// City Speed alarm
#define VSS_ALARM_CITY_SPEED  61 // km
//
// Sensor correctors
#define VSS_CORRECTION 3.1767 // 3.2
#define RPM_CORRECTION 32.767 // RPM OBD PID: 16,383.75 [old: 32.8]
#define ECU_CORRECTION 1.8

#define VSD_SENS_DEBUG;


static  void EngSens_catchRpmHits();
static  void EngSens_catchVssHits();
static  void EngSens_catchEcuHits();

//
// Hint counters
volatile int vssHitsCount, rpmHitsCount, ecuHitsCount;

class EngSens {
private:

    int
    //
    // Human Results
            CUR_VSS, CUR_VDS, CUR_RPM, CUR_ECU;
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



    void setup(uint8_t pinVss, uint8_t pinRpm, uint8_t pinEcu) {
        setupRpmSens(pinRpm);
        setupVssSens(pinVss);
        setupEcuSens(pinEcu);

    }

    /**
     * Gets current Vss
     */
    int getVssSens() {
        return CUR_VSS;
    }

    int getRpmSens() {
        return CUR_RPM;
    }

    int getEcuSens() {
        return CUR_ECU;
    }

    int long getDstSens() {
        return CUR_VDS /* real value is divided 1000, be more precise */;
    }

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
void EngSens::sensVss() {

    if (ampInt.isSens()) {

        //
        // Pass vss to global
        CUR_VSS = int(vssHitsCount / VSS_CORRECTION);
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

#if defined(VSD_SENS_DEBUG) || defined(GLOBAL_SENS_DEBUG)
    if (ampInt.isBig()) {
        Serial.print("Counted VSD is: ");
        Serial.println(CUR_VDS);
    }
#endif



    //
    // Alarm speeding at city
    if (ampInt.isSec() && CUR_VSS > VSS_ALARM_CITY_SPEED) {
        tone(ADT_ALR_PIN, 4000, 500);
    }


}


/*******************************************************************
 * Detect RPMs
 */
void EngSens::sensRpm() {

    if (ampInt.isSens()) {
        //
        // Pass rpm to global
        CUR_RPM = int( rpmHitsCount * RPM_CORRECTION);

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
void EngSens::sensEcu() {

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

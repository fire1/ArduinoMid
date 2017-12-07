/**
 *
 *  Created by Angel Zaprianov on 5/10/2016.
 *
 *  Class converts Opel||Vauxhall steering wheel to Sony remote control.
 *     + This class support SPI communication to digital potentiometer.
 *     + 50k digital potentiometer [MCP41050] is used to simulate
 *          Alphine remote controller.
 *     + 10k pull-up resistor from 5v Supplying voltage is used to
 *          determinate steering wheel button.
 *     + Some of buttons are out of resistant range if used digit pot.
 *
 *
 */

#ifndef ARDUINO_MID_STR_SENS_H
#define ARDUINO_MID_STR_SENS_H

#include <SPI.h>
#include "AmpTime.h"
#include "InitObj.h"
#include "CarSens.h"

//#define STR_DEBUG
//#define STR_WHL_SEND_A
#define STR_WHL_SEND_B

//
// Creates test with maximum send value
#define ADR_DIG_POD B10001
//
// pull up Resistance ~ 220 ohm
#define STR_READ_RESIST_B
#define WHL_DEBOUCE_TIME 3

//
// Uncomment to send resistance values from terminal
//#define STR_INJ_SRL true
class WhlSens {

    AmpTime *amp;
    CarSens *car;

    //
    // Used for shortcuts ...
    boolean isDisabled = 0;
    boolean isNewResist = 0;
    unsigned long timer;
    int currentStateButton;
    int lastStateButton = 0;
    uint8_t pinSteering, pinDigPotCtr, pinMaskHide;


public:
    //
    // Define buttons values
    static constexpr uint8_t STR_BTN_NON = 0;
    static constexpr uint8_t STR_BTN_VLD = 1;
    static constexpr uint8_t STR_BTN_VLU = 2;
    static constexpr uint8_t STR_BTN_SKU = 3;
    static constexpr uint8_t STR_BTN_SKD = 4;
    static constexpr uint8_t STR_BTN_BCK = 5;
    static constexpr uint8_t STR_BTN_ATT = 6;
    static constexpr uint8_t STR_BTN_MNT = 7;

    /**
     *
     */
    WhlSens(AmpTime &_amp, CarSens &_car) : amp(&_amp), car(&_car) {

    }


    /**
     * Setup Steering Wheel to Sony audio
      * @param pinTargetSteering
      * @param pinDigitalPod
      * @param pinMask
      */
    void begin(uint8_t pinTargetSteering, uint8_t pinDigitalPod, uint8_t pinMask) {


        pinSteering = pinTargetSteering;
        pinDigPotCtr = pinDigitalPod;
        pinMaskHide = pinMask;


        pinMode(pinSteering, INPUT);
        pinMode(pinDigPotCtr, OUTPUT);
        pinMode(pinMaskHide, OUTPUT);
//        digitalWrite(pinMaskHide, HIGH); // Hide dig pot
        analogWrite(pinMaskHide, 255); // Hide dig pot
        //  pinMode (SPICCLOCK, OUTPUT);//Needed to be defined?
        //  pinMode (SLAVESELECT,OUTPUT); //same as above?
        //
        // initialize SPI:
        SPI.begin();
        SPI.setDataMode(SPI_MODE0);//SPI_MODE0, SPI_MODE1, SPI_MODE2, or SPI_MODE3
        SPI.setBitOrder(MSBFIRST); // LSBFIRST or MSBFIRST

    }

    /**
           SONY buttons value

                resistance	voltage
        src      	    2200	4.1
        att	            4400	3.47
        off	            35	    4.98
        vol down	    23850	1.47
        vol up	        17000	1.85
        seek down	    12000	2.25
        seek up	        8800	2.65
        back	        6660	3
        front	        33940	1.13
        bottom	        48800	0.84
     */
    void listener() {

        resolveButton(analogRead(pinSteering));
        //
        // Simulate resistance in radio
        sendRadioButtons();
    }

    /**
     * Public Send resistance to pod
     * @param resistance
     */
    void sendRadioButtons(uint8_t resistance) {
        Serial.print("WHL Current resistance ");
        Serial.println(resistance);


        digitalWrite(pinMaskHide, LOW);
        digitalWrite(pinDigPotCtr, LOW);

        setDigitalPot(resistance);
        delay(5); // Some separation fix
        digitalWrite(pinDigPotCtr, HIGH);
        delay(500);
        digitalWrite(pinMaskHide, HIGH);


    }

    /**
     *
     * @param resistance
     * @param voltage
     */
    void sendRadioButtons(uint8_t resistance, uint8_t voltage) {

        uint8_t volts = (uint8_t) map(voltage, 0, 50, 0, 255);
        Serial.print("WHL Current resistance ");
        Serial.print(resistance);
        Serial.print(" voltage ");
        Serial.print(voltage);
        Serial.print(" at ");
        Serial.println(volts);

        analogWrite(pinMaskHide, volts);
        digitalWrite(pinDigPotCtr, LOW);

        setDigitalPot(resistance);
        delay(5); // Some separation fix
        digitalWrite(pinDigPotCtr, HIGH);
        delay(500);
        digitalWrite(pinMaskHide, HIGH);


    }


    /**
     *  GEts current pressed button
      * @return  int
      */
    int getCurrentState() {
        return currentStateButton;
    }

    void disable() {
        isDisabled = 1;
    }

    void enable() {
        isDisabled = 0;
    }

    boolean isDisable() {
        return isDisabled;
    }


    /**
     * Change dig pod to shutdown mode
     */
    void shutdownMode(void) {
        //
        // New type
#ifdef SHUTDOWN_RADIO
        this->sendRadioButtons(1);
        this->sendRadioButtons(1);
#else
        this->sendRadioButtons(255);
        this->sendRadioButtons(255);
#endif


    }

private:


//    void setDigitalPot(uint8_t resistance);
    /**
     * Send value to dig-pot
     * @param  resistanceValue
     */
    void setDigitalPot(uint8_t resistanceValue) {
        SPI.transfer(ADR_DIG_POD); // 17
        SPI.transfer(resistanceValue);
    }

    /**
     * Sets current button press
     */
    void setCurrentState(int currentButton) {
        currentStateButton = currentButton;

#if defined(STR_DEBUG)
        if (amp->isSecond()) {
        Serial.print("WHL Button detect ");
        Serial.println(currentStateButton);
    }
#endif
    }


    /**
     *
     * @param readVoltage
     */
    void resolveButton(int readVoltage) {
//
//        if(amp->isSecond()){
//            Serial.print("Read voltage ");
//            Serial.println(readVoltage);
//        }

        if (readVoltage > 900) {
            setCurrentState(STR_BTN_NON);
        } else
            //
            // Volume down
        if (readVoltage > 200 && readVoltage < 350) {
            setCurrentState(STR_BTN_VLD);
        } else
            //
            // Volume up
        if (readVoltage > 350 && readVoltage < 499) {
            setCurrentState(STR_BTN_VLU);
        } else
            //
            // Zero button
        if (readVoltage > 500 && readVoltage < 650) {
            setCurrentState(STR_BTN_ATT);
        } else
            //
            // Right arrow / seek up
        if (readVoltage > 650 && readVoltage < 750) {
            setCurrentState(STR_BTN_SKU);
        } else
            //
            // Left arrow / seek down
        if (readVoltage > 750 && readVoltage < 820) {
            setCurrentState(STR_BTN_SKD);
        } else
            //
            // Back button
        if (readVoltage > 820 && readVoltage < 900) {
            setCurrentState(STR_BTN_BCK);
        }
    }

    /**
     * @param  int
     */
    void setButtonStateParser(int currentState) {

        if (car->getRpm() > 500) {
            // TODO
        } else {

        }
        //
        // Sony XPlod
#ifdef WHL_RDO_XPLOD
        if (currentState == STR_BTN_VLU) setDigitalPot(95);// Volume up
        if (currentState == STR_BTN_VLD) setDigitalPot(125);// Volume down
        if (currentState == STR_BTN_ATT) setDigitalPot(20); // Zero
        if (currentState == STR_BTN_MNT) setDigitalPot(155);// Mute
        if (currentState == STR_BTN_SKU) setDigitalPot(50);// seek up
        if (currentState == STR_BTN_SKD) setDigitalPot(70);// seek down
        if (currentState == STR_BTN_BCK) setDigitalPot(15);// back button
//        if (currentState == STR_BTN_NON) setDigitalPot(0);// return to default
#endif

#ifdef WHL_RDO_KDX330


#endif

    }

    /**
 * Sends commands to radio
 */
    void sendRadioButtons() {

        int currentState = getCurrentState();

        //
        // Disable commands to radio
        if (isDisable()) {
            return;
        }
#if defined(STR_DEBUG)
        if (amp->isSecond()) {
        Serial.print("WHL Current State ");
        Serial.println(currentState);
    }
#endif
        //
        // When is not none state
        if (currentState != STR_BTN_NON) {
            if (lastStateButton != currentState) {

                if (!isNewResist) {
                    digitalWrite(pinDigPotCtr, LOW);
                    setButtonStateParser(currentState);
                    isNewResist = 1;
                }
                // delay(3);
                if (amp->isMin()) {// TODO test here
                    // Test done .... it seems to be O.K.
                    digitalWrite(pinDigPotCtr, HIGH);
                    digitalWrite(pinMaskHide, LOW);
                    lastStateButton = currentState;
                    isNewResist = 0;
                }
            } else digitalWrite(pinMaskHide, LOW);
        } else {
            digitalWrite(pinMaskHide, HIGH);
        }
    }

};


#endif
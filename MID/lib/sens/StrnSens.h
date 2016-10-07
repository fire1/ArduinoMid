//
// Created by Angel Zaprianov on 5/10/2016.
//


#ifndef ARDUINO_MID_STR_SENS_H
#define ARDUINO_MID_STR_SENS_H

#include <SPI.h>


//
// Creates test with maximum send value
#define TST_DIG_POD 256 // Test full range resistance of digital potentiometer
#define DIG_POD_KOM 50 // 50k digital potentiometer
#define DIG_POD_STP 128 // Max steps of digital potentiometer
#define ADR_DIG_POD B10001
#define STR_BTN_INF true

/**
 * Class converts Opel||Vauxhall steering wheel to Sony remote control
 */
class StrButtonsSony {

private:

    bool isButtonPressActive = 0;

    int testIndex = 0;
    uint8_t pinSteering, pinDigitalOut, pinOutVoltage;

    void testDigitalPod();

    int digitalPotWrite(float resistance, float voltage);

    int getVoltageStep(float voltage);

    int getResistanceStep(float resistance);


public:
    StrButtonsSony(uint8_t pinTargetSteering, uint8_t pinDigitalPod, uint8_t pinVoltage) {
        pinSteering = pinTargetSteering;
        pinDigitalOut = pinDigitalPod;
        pinOutVoltage = pinVoltage;
    }

    void setup();

    void listenButtons();

};

/***********************************************************************************************
 *                                                                                             *
 *                                   CPP part of file                                          *
 *                                                                                             *
 ***********************************************************************************************/

/**
 *  Gets step from given voltage
 */
int StrButtonsSony::getVoltageStep(float voltage) {
    return int(255 * (voltage / 5));
}

/**
 *  Gets step from given resistance
 */
int StrButtonsSony::getResistanceStep(float resistance) {
    return int(DIG_POD_STP * (resistance / DIG_POD_KOM));
}

/**
 * Send command to pod
 */
int StrButtonsSony::digitalPotWrite(float resistance, float voltage) {

    int volts = getVoltageStep(voltage);
    int resist = getResistanceStep(resistance);

    digitalWrite(pinDigitalOut, LOW);
    //send in the address and value via SPI:
    SPI.transfer(ADR_DIG_POD);
    SPI.transfer(byte(resist));
    analogWrite(pinOutVoltage, volts);
    // take the SS pin high to de-select the chip
    digitalWrite(pinDigitalOut, HIGH);
    //
    // Mark button activated
    // [ next loop will return digPot to default]
    isButtonPressActive = 0;

#if defined(STR_BTN_INF)
    Serial.print(" Sending value to plot: ");
    Serial.print("  \t RESIST:");
    Serial.print(resist);
    Serial.print("  \t VOLTS:");
    Serial.println(volts);
#endif
}

/*
 * Test method of digital potentiometer
 */
void StrButtonsSony::testDigitalPod() {

#if defined(TST_DIG_POD)

    // adjust high and low resistance of potentiometer
    // adjust Highest Resistance .
//    digitalPotWrite(0x00);
//    delay(1000);
//
//    // adjust  wiper in the  Mid point  .
//    digitalPotWrite(0x80);
//    delay(1000);
//
//    // adjust Lowest Resistance .
//    digitalPotWrite(0xFF);
//    delay(1000);


    digitalWrite(pinDigitalOut, LOW);
    SPI.transfer(B10001); // 17
    SPI.transfer(testIndex);
    digitalWrite(pinDigitalOut, HIGH);
    //
    // Show value
    Serial.print("\n Test of digital pod is: ");
    Serial.println(testIndex);

    testIndex++;
    delay(100);
    if (testIndex >= TST_DIG_POD) {
        testIndex = 0;
    }

#endif
}

/*
 * Setup Steering Wheel to Sony audio
 */
void StrButtonsSony::setup() {
    pinMode(pinSteering, INPUT);
    pinMode(pinDigitalOut, OUTPUT);
//  pinMode (SPICCLOCK, OUTPUT);//Needed to be defined?
//  pinMode (SLAVESELECT,OUTPUT); //same as above?
// initialize SPI:
    SPI.begin();
    SPI.setDataMode(SPI_MODE0);//SPI_MODE0, SPI_MODE1, SPI_MODE2, or SPI_MODE3
    SPI.setBitOrder(MSBFIRST); // LSBFIRST or MSBFIRST

}

/*
 *          SONY buttons value
 *
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
void StrButtonsSony::listenButtons() {

    int readingSteeringButton = analogRead(pinSteering);
    //
    // Testing method
    // StrButtonsSony::testDigitalPod();

    //
    // Default value  for sony whe Steering wheel is not used
    if (readingSteeringButton > 250 && isButtonPressActive == 0) {
        digitalPotWrite(0, 5);
        //
        // Do not enter in here next loop
        isButtonPressActive = 1;
    }
    //
    // Zero button
    if (readingSteeringButton > 25 && readingSteeringButton < 30) {
        digitalPotWrite(44, 3.47);
    }
    //
    // Volume up
    if (readingSteeringButton > 5 && readingSteeringButton < 14) {
        digitalPotWrite(17, 1.85);//        digitalPotWrite(byte(94), 1.85);
    }
    //
    // Volume down
    if (readingSteeringButton > 0 && readingSteeringButton < 5) {
        digitalPotWrite(23.85, 1.47);
    }
    //
    // Right arrow / seek up
    if (readingSteeringButton > 40 && readingSteeringButton < 50) {
        digitalPotWrite(88, 2.65);
    }
    //
    // Left arrow / seek down
    if (readingSteeringButton > 70 && readingSteeringButton < 80) {
        digitalPotWrite(12, 2.25);
    }
    //
    // Back button
    if (readingSteeringButton > 130 && readingSteeringButton < 140) {
        digitalPotWrite(66.6, 3);
    }


}


#endif
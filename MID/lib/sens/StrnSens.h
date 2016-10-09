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

    int digitalPotWrite(int resistance);


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
    delay(700);
    if (testIndex >= TST_DIG_POD) {
        testIndex = 10;
    }

#endif
}

/*
 * Setup Steering Wheel to Sony audio
 */
void StrButtonsSony::setup() {
    pinMode(pinSteering, INPUT);
    pinMode(pinDigitalOut, OUTPUT);
    pinMode(pinOutVoltage, OUTPUT);
    digitalWrite(pinOutVoltage, HIGH);
//  pinMode (SPICCLOCK, OUTPUT);//Needed to be defined?
//  pinMode (SLAVESELECT,OUTPUT); //same as above?
// initialize SPI:
    SPI.begin();
    SPI.setDataMode(SPI_MODE0);//SPI_MODE0, SPI_MODE1, SPI_MODE2, or SPI_MODE3
    SPI.setBitOrder(MSBFIRST); // LSBFIRST or MSBFIRST

}

/**
 * Send command to pod
 */
int StrButtonsSony::digitalPotWrite(int resistanceValue) {

    digitalWrite(pinDigitalOut, LOW);
    delay(30);
    digitalWrite(pinOutVoltage, LOW);
    SPI.transfer(B10001); // 17
    SPI.transfer(resistanceValue);
    digitalWrite(pinDigitalOut, HIGH);
    delay(150);
    digitalWrite(pinOutVoltage, HIGH);

    isButtonPressActive = 0;
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

//    StrButtonsSony::testDigitalPod();

//    if(ampInt.isMid()){
//        Serial.println(readingSteeringButton);
//    }


//    digitalWrite(pinDigitalOut, HIGH);

    if (Serial.available()) {
        int val = Serial.parseInt();
        Serial.println(val);
        if (val != 0) {
            digitalWrite(pinDigitalOut, LOW);
            delay(30);
            digitalWrite(pinOutVoltage, LOW);
            SPI.transfer(B10001); // 17
            SPI.transfer(val);
            digitalWrite(pinDigitalOut, HIGH);
            delay(30);
            digitalWrite(pinOutVoltage, HIGH);
        }
    }

    //
    // Default value  for sony whe Steering wheel is not used
    if (readingSteeringButton > 250 && isButtonPressActive == 0) {

        digitalWrite(pinOutVoltage, HIGH);
        //
        // Do not enter in here next loop
        isButtonPressActive = 1;
    }

    if (ampInt.isSec()) {
        Serial.println(readingSteeringButton);
    }


    //
    // Zero button
    if (readingSteeringButton > 20 && readingSteeringButton < 30) {
        //
        // TODO long press 155 volume press button
        digitalPotWrite(225); // moda
    }
    //
    // Volume up
    if (readingSteeringButton > 9 && readingSteeringButton < 20) {
        digitalPotWrite(95);
    }
    //
    // Volume down
    if (readingSteeringButton >= 0 && readingSteeringButton < 9) {
        digitalPotWrite(115);
    }
    //
    // Right arrow / seek up
    if (readingSteeringButton > 35 && readingSteeringButton < 50) {
        digitalPotWrite(45);
    }
    //
    // Left arrow / seek down
    if (readingSteeringButton > 60 && readingSteeringButton < 80) {
        digitalPotWrite(65);
    }
    //
    // Back button
    if (readingSteeringButton > 130 && readingSteeringButton < 160) {
        digitalPotWrite(15);
        // TODO long press 225 MODE
        // 5 - off
        //
    }


}


#endif
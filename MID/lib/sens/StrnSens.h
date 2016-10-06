//
// Created by Angel Zaprianov on 5/10/2016.
//


#ifndef ARDUINO_MID_STR_SENS_H
#define ARDUINO_MID_STR_SENS_H

int testDigitalPodIndex = 0;

//
// Creates test with maximum send value
#define TEST_DIG_POD 128

#include <SPI.h>

class StrButtonsSony {

private:

    uint8_t pinSteering, pinDigitalOut;

    void testDigitalPod();


public:
    StrButtonsSony(uint8_t pinTargetSteering, uint8_t pinDigitalPod) {
        pinSteering = pinTargetSteering;
        pinDigitalOut = pinDigitalPod;
    }

    void setup();

    void listenButtons();

};


void StrButtonsSony::testDigitalPod() {

#if defined(TEST_DIG_POD)

    if (readingSteeringButton > 250) {
        digitalWrite(pinDigitalOut, LOW);
        SPI.transfer(255);

        digitalWrite(pinDigitalOut, HIGH);
        testDigitalPodIndex = 1;
    }

    digitalWrite(pinDigitalOut, LOW);
    SPI.transfer(B10001);
    SPI.transfer(byte(testDigitalPodIndex));
    digitalWrite(pinDigitalOut, HIGH);
    //
    // Show value
    Serial.print("\n Value of digital pod is: ");
    Serial.println(testDigitalPodIndex);

    testDigitalPodIndex++;
    delay(1500);
    if (testDigitalPodIndex >= TEST_DIG_POD) {
        testDigitalPodIndex = 0;
    }

#endif
}


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

    testDigitalPod();

    if (Serial.available()) {

        SPI.transfer(Serial.read());
    }

    //
    // Zero button
    if (readingSteeringButton > 25 && readingSteeringButton < 30) {
        Serial.print("Zero button");
        digitalWrite(pinDigitalOut, LOW);
        SPI.transfer(B10001);
        SPI.transfer(byte(250));
        digitalWrite(pinDigitalOut, HIGH);
        testDigitalPodIndex = 0;
    }

    if (ampInt.isMid()) {
        Serial.println(readingSteeringButton);
    }
    //
    // Volume up
    if (readingSteeringButton > 5 && readingSteeringButton < 14) {
        digitalWrite(pinDigitalOut, LOW);
        SPI.transfer(B10001);
        SPI.transfer(byte(94));
        digitalWrite(pinDigitalOut, HIGH);

        if (ampInt.isMid()) {
            Serial.print("Volume Up");
            Serial.print("\t value:");
//            Serial.print(getVoltage(1.85));
            Serial.print("\n");
        }
        testDigitalPodIndex = 0;
    }

    //
    // Volume down
    if (readingSteeringButton > 0 && readingSteeringButton < 5) {
        digitalWrite(pinDigitalOut, LOW);
        SPI.transfer(B10001);
        SPI.transfer(byte(167));
        digitalWrite(pinDigitalOut, HIGH);
        if (ampInt.isMid()) {
            Serial.print("Volume Down");
            Serial.print("\t value:");
//            Serial.print(getVoltage(1.37));
            Serial.print("\n");
        }
        testDigitalPodIndex = 0;
    }


    //
    // Right arrow / seek up
    if (readingSteeringButton > 40 && readingSteeringButton < 50) {

        SPI.transfer(131);
        if (ampInt.isMid()) {
            Serial.print("Seek Up");
            Serial.print("\t value:");
//            Serial.print(getVoltage(2.55));
            Serial.print("\n");
        }
        testDigitalPodIndex = 0;
    }

    //
    // Left arrow / seek down
    if (readingSteeringButton > 70 && readingSteeringButton < 80) {
        digitalWrite(pinDigitalOut, LOW);
        SPI.transfer(B10001);
        SPI.transfer(byte(60));
        digitalWrite(pinDigitalOut, HIGH);
        if (ampInt.isMid()) {
            Serial.print("Seek Down");
            Serial.print("\t value:");
//            Serial.print(getVoltage(2.15));
            Serial.print("\n");
        }
        testDigitalPodIndex = 0;
    }


    //
    // Back button
    if (readingSteeringButton > 130 && readingSteeringButton < 140) {
//src
//        digitalWrite(pinDigitalOut,LOW);
//        SPI.transfer(B10001);
//        SPI.transfer(byte(118));
//        digitalWrite(pinDigitalOut,HIGH);


        SPI.transfer(200);
        if (ampInt.isMid()) {
            Serial.print("Back Button");
            Serial.print("\t value:");
//            Serial.print(getVoltage(2.95));
            Serial.print("\n");
        }
        testDigitalPodIndex = 0;
    }


}


#endif
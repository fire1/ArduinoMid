//
// Created by Angel Zaprianov on 3.10.2016 г..
//

#ifndef ARDUINOMID_INJCDATA_H
#define ARDUINOMID_INJCDATA_H

#include <Arduino.h>

//
// Start of Command ...
#ifndef INJ_SRL_ENTRY
#define INJ_SRL_ENTRY '?'

//
// Separate  of type ...
#ifndef INJ_SRL_TYPE
#define INJ_SRL_TYPE '.'

//
// String length resolver
#ifndef INJ_SRL_STR_LEN
#define INJ_SRL_STR_LEN 3

//
// Type length resolver
#ifndef INJ_SRL_TYP_LEN
#define INJ_SRL_TYP_LEN 3


/**
 * Injects data from serial command
 */
class InjSrl {

private:
    int LINE_BUFFER_SIZE = 32; // max line length is one less than this

    boolean parseInput(String name, String type);


public:

    InjSrl(int bufferSize);

    void listener();

    void listener_();

};


/*  ***     CPP part of file    *** */


InjSrl::InjSrl(int bufferSize) {
    LINE_BUFFER_SIZE = bufferSize;
}


void InjSrl::listener_() {
    // Buffer to store incoming commands from serial port
    String allData;

    char bufferRead[LINE_BUFFER_SIZE];
    int indexRead = 0;
    int indexName = 0;
    int indexType = 0;
    String stringName;
    String stringType;


    while (Serial.available() > 0) {
        char recieved = Serial.read();
        bufferRead[indexRead] = recieved;
        allData += recieved;

        // Begin
        //  Read string name
        if (bufferRead[0] == INJ_SRL_ENTRY && indexName < INJ_SRL_STR_LEN) {
            stringName += recieved;
            indexName++;
        }

        // Then
        //  Read string type
        if (bufferRead[INJ_SRL_STR_LEN + 1] == INJ_SRL_TYPE && indexType < INJ_SRL_TYP_LEN) {
            stringType += recieved;
            indexType++;
        }





        // Process message when new line character is recieved
        if (recieved == '\n') {

            boolean result = parseInput(stringName, stringType);

            Serial.print("[MID $]> ");
            Serial.print(allData);
            Serial.println(result ? " \t\t [OK] " : " \t\t [NO] ");

            allData = ""; // Clear recieved buffer
        }

        indexRead++;
    }
}

boolean InjSrl::parseInput(String name, String type) {

    if (type == "flt") {
        float value = Serial.parseInt() * 0.01;
    }
    if (type == "int") {
        int value = Serial.parseInt();
    }

    if (name == "TTL_TTD") {
        TTL_TTD = value;
    }

}


void InjSrl::listener() {

    String allData;
    String stringName;


    if (Serial.available() > 0) {
        stringName = Serial.readStringUntil('=');
        if (stringName == "TTD") TTL_TTD = Serial.readStringUntil('\n').toInt() * 0.01;
    }

    while (Serial.available() > 0) {
        char recieved = Serial.read();
        // Process message when new line character is recieved
        if (recieved == '\n') {
            Serial.print("[MID $]> ");
            Serial.print(allData);
            Serial.println(result ? " \t\t [OK] " : " \t\t [NO] ");

            stringName = "";
            allData = ""; // Clear recieved buffer
        }
    }
}


#endif //ARDUINOMID_INJCDATA_H

//
// Created by Angel Zaprianov on 9.5.2017 г..
//

#ifndef ARDUINO_MID_LAN
#define ARDUINO_MID_LAN

#include <Arduino.h>

//
// Maximum size of message is 128 symbols
//
//
const char msg0[] PROGMEM = "Goodbye ...     ";
const char msg1[] PROGMEM = "until next time!";
const char msg2[] PROGMEM = "ENTRY";
const char msg3[] PROGMEM = "Brake wearing state: ";
const char msg4[] PROGMEM = "Coolant level state: ";
const char msg5[] PROGMEM = "Window washer state: ";
const char msg6[] PROGMEM = "Engine Oil level is: ";
const char msg7[] PROGMEM = "CORRECT";
const char msg8[] PROGMEM = "WARNING";
const char msg9[] PROGMEM = "";
const char msg10[] PROGMEM = "";
//
//
const char msg11[] PROGMEM = "MAIN MENU";
const char msg12[] PROGMEM = "CURRENT TRIP";
const char msg13[] PROGMEM = "FUEL MENU";
const char msg14[] PROGMEM = "SERVICING";
const char msg15[] PROGMEM = "INFORMATION";
const char msg16[] PROGMEM = "DASHBOARD";
const char msg17[] PROGMEM = "";
const char msg18[] PROGMEM = "";
const char msg19[] PROGMEM = "";
//
//
const char msg20[] PROGMEM = "To reset trips recorded in Board Comp";
const char msg21[] PROGMEM = "Hold >R and break pedal ";
const char msg22[] PROGMEM = "Do you really want to reset data?  ";
const char msg23[] PROGMEM = "S< Yes  /  R< No";
//
// Generate message table for LCD
const char *const LcdMsgTable[] PROGMEM = {
        msg0, msg1, msg2, msg3, msg4, msg5, msg6, msg7, msg8, msg9, msg10, msg11, msg12, msg13, msg14, msg15,
        msg16, msg17, msg18, msg19, msg20, msg21, msg22, msg23
};

#endif //ARDUINOMID_EN_H_H

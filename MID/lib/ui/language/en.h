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
const char msg0[] PROGMEM = "String 0";
const char msg1[] PROGMEM = "String 1";
const char msg2[] PROGMEM = "String 2";
const char msg3[] PROGMEM = "String 3";
const char msg4[] PROGMEM = "String 4";
const char msg5[] PROGMEM = "String 5";
//
// Generate message table for LCD
const char * const LcdMsgTable[] PROGMEM = {msg0, msg1, msg2, msg3, msg4, msg4, msg5};

#endif //ARDUINOMID_EN_H_H

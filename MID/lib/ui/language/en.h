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
const char msg3[] PROGMEM = "Brake wear is O.K. ";
const char msg4[] PROGMEM = "CHECK your brake pads wear!";
const char msg5[] PROGMEM = "Coolant level is O.K. ";
const char msg6[] PROGMEM = "Coolant level is to low!";
const char msg7[] PROGMEM = "Window washer liquid level is O.K. ";
const char msg8[] PROGMEM = "Window washer liquid is to low!";
const char msg9[] PROGMEM = "Oil level is  O.K.";
const char msg10[] PROGMEM = "Engine oil is to low!";
//
//
const char msg11[] PROGMEM = "MAIN MENU";
const char msg12[] PROGMEM = "CURRENT TRIP";
const char msg13[] PROGMEM = "FUEL MENU";
const char msg14[] PROGMEM = "CAR SERVICING";
const char msg15[] PROGMEM = "TRIPs RESET";


const char msg20[] PROGMEM = "To reset trips recorded in Board computer";
const char msg21[] PROGMEM = "Press >S and >R together ";
const char msg22[] PROGMEM = "Do you really want to reset data?  ";
const char msg23[] PROGMEM = "S< Yes  /  R< No";
//
// Generate message table for LCD
const char *const LcdMsgTable[] PROGMEM = {
        msg0, msg1, msg2, msg3, msg4, msg4, msg5, msg6, msg7, msg8, msg9, msg10, msg11, msg12, msg13, msg14, msg15,

        msg20, msg21, msg22, msg23
};

#endif //ARDUINOMID_EN_H_H

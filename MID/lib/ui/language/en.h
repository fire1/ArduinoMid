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
const char msg0[] PROGMEM = "";
const char msg72[] PROGMEM = "Goodbye...     ";
const char msg1[] PROGMEM = "until next time!";
const char msg2[] PROGMEM = "ENTRY";
//
// Car state
const char msg3[] PROGMEM = "Brake wearing state: ";
const char msg4[] PROGMEM = "Coolant level state: ";
const char msg5[] PROGMEM = "Window washer state: ";
const char msg6[] PROGMEM = "Engine Oil level is: ";
const char msg7[] PROGMEM = "        £ "; // Is fine message
const char msg8[] PROGMEM = "        @"; // Problem message for liquid
const char msg9[] PROGMEM = "WORN OUT"; // Problem message for brakes
const char msg10[] PROGMEM = "WENT OUT"; // Problem message for lamps
const char msg99[] PROGMEM = "VOLTMETER";
const char msg100[] PROGMEM = "TIMING BELT";
//
// Titles
const char msg11[] PROGMEM = "HOME MENU";
const char msg12[] PROGMEM = "TRIP RECORDS";
const char msg13[] PROGMEM = "FUEL MENU";
const char msg14[] PROGMEM = "SERVICING";
const char msg15[] PROGMEM = "ABOUT";
const char msg16[] PROGMEM = "DASHBOARD";
const char msg17[] PROGMEM = "CALIBRATION";
const char msg18[] PROGMEM = "CURRENT";
const char msg19[] PROGMEM = "DEFAULT";
//
//
const char msg20[] PROGMEM = "To reset trip data";
const char msg21[] PROGMEM = "Hold >R and break pedal ";
const char msg22[] PROGMEM = "Firmware update: www.git.io/vHlkd";
const char msg23[] PROGMEM = "ArduinoMID version ";
const char msg24[] PROGMEM = "Press $ to enter in settings";
const char msg25[] PROGMEM = "Use $ to navigate ";
const char msg26[] PROGMEM = "Press $+# to change value";
const char msg27[] PROGMEM = "VEHICLE SPEED"; // Vehicle speed sensor
const char msg28[] PROGMEM = "REVOLUTIONS";
const char msg29[] PROGMEM = "ODOMETER";
const char msg30[] PROGMEM = "CONSUMPTION";
const char msg31[] PROGMEM = "RESULT";
const char msg32[] PROGMEM = "Turn engine Off to save data";
const char msg33[] PROGMEM = "Use BREAK pedal for end";
const char msg34[] PROGMEM = " Take a break";
const char msg35[] PROGMEM = " Drowsiness detected";
const char msg36[] PROGMEM = " Bulb failure!";
//
// Warning winter
const char msg37[] PROGMEM = " ROAD ICING WARNING!";
const char msg38[] PROGMEM = " ";
const char msg39[] PROGMEM = "Please drive safe!";
//
// Warning Oil level
const char msg40[] PROGMEM = " LOW OIL LEVEL!";
const char msg41[] PROGMEM = "Oil level is  below 25%";
const char msg42[] PROGMEM = "Top up motor oil";
//
// Warning Battery
const char msg43[] PROGMEM = "ELECTRICAL PROBLEM";
const char msg44[] PROGMEM = "A voltage of: ";
const char msg45[] PROGMEM = "not in work range";
//
// Warning breaks
const char msg46[] PROGMEM = " WORN BRAKES!";
const char msg47[] PROGMEM = "Determined pad wear into ";
const char msg48[] PROGMEM = " front breaks! ";
//
// Warning coolant
const char msg49[] PROGMEM = " LOW COOLANT!";
const char msg50[] PROGMEM = "Check the coolant level";
const char msg51[] PROGMEM = "if is necessary add more.";
//
// Warning overheating
const char msg52[] PROGMEM = " OVERHEATING!!!";
const char msg53[] PROGMEM = "Engine temperature ";
const char msg54[] PROGMEM = "is too high, overheating";
//
// Warning window washer
const char msg55[] PROGMEM = " LOW WINDOW WASHER!";
const char msg56[] PROGMEM = "Check window washer ";
const char msg57[] PROGMEM = "if is necessary add more.";
//
// Warning bulb
const char msg58[] PROGMEM = " BULB BURN OUT ";
const char msg59[] PROGMEM = "Incandescent malfunction";
const char msg60[] PROGMEM = "In front lights ";
const char msg61[] PROGMEM = "In back lights ";
//
// Warning timing  Belt ware
const char msg62[] PROGMEM = " WORN TIMING BELT";
const char msg63[] PROGMEM = "Timing belt worn-out";
const char msg64[] PROGMEM = "After change reset data";
//
// Warning Air filter ware @deprecated
const char msg65[] PROGMEM = " OIL/AIR FILTER WORN OUT ";
const char msg66[] PROGMEM = "Air filter  replacement after";
const char msg67[] PROGMEM = " (After change reset servicing)";
//
// Value tags
const char msg68[] PROGMEM = "´"; // l
const char msg70[] PROGMEM = "³"; // h
const char msg71[] PROGMEM = "´÷¥"; // L/100km
//
// Guidance
const char msg110[] PROGMEM = " 10% resistor tolerance";
const char msg111[] PROGMEM = " Timing belt distance: ";
const char msg112[] PROGMEM = "Press # to reset ";
const char msg113[] PROGMEM = "To reset hold #+$";
const char msg114[] PROGMEM = "To increase, lower the value";
const char msg115[] PROGMEM = "Increase or reduce value";
//
// Ohter
const  char msg120[]  PROGMEM = "00.0";
const  char msg121[]  PROGMEM = "00";
const  char msg122[]  PROGMEM = "  ";
const  char msg123[]  PROGMEM = "Range: ";


#endif //ARDUINOMID_EN_H_H

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
//
// Car state
const char msg3[] PROGMEM = "Brake wearing state: ";
const char msg4[] PROGMEM = "Coolant level state: ";
const char msg5[] PROGMEM = "Window washer state: ";
const char msg6[] PROGMEM = "Engine Oil level is: ";
const char msg7[] PROGMEM = "O.K."; // Is fine message
const char msg8[] PROGMEM = "BELOW 25%"; // Problem message for liquid
const char msg9[] PROGMEM = "WORN OUT"; // Problem message for brakes
const char msg10[] PROGMEM = "WENT OUT"; // Problem message for lamps
//
// Titles
const char msg11[] PROGMEM = "MAIN MENU";
const char msg12[] PROGMEM = "TRIP DETAILS";
const char msg13[] PROGMEM = "FUEL MENU";
const char msg14[] PROGMEM = "SERVICING";
const char msg15[] PROGMEM = "ABOUT";
const char msg16[] PROGMEM = "DASHBOARD";
const char msg17[] PROGMEM = "SETTINGS";
const char msg18[] PROGMEM = "CURRENT";
const char msg19[] PROGMEM = "DEFAULT";
//
//
const char msg20[] PROGMEM = "To reset trip data";
const char msg21[] PROGMEM = "Hold >R and break pedal ";
const char msg22[] PROGMEM = "Firmware update: www.git.io/vHlkd";
const char msg23[] PROGMEM = "ArduinoMID version ";
const char msg24[] PROGMEM = "Press \"S<\" to enter in settings";
const char msg25[] PROGMEM = "Use \"S<\" to navigate ";
const char msg26[] PROGMEM = "Use \"R<\" to change value";
const char msg27[] PROGMEM = "VEHICLE SPEED"; // Vehicle speed sensor
const char msg28[] PROGMEM = "REVOLUTIONS";
const char msg29[] PROGMEM = "ODOMETER";
const char msg30[] PROGMEM = "CONSUMPTION";
const char msg31[] PROGMEM = "RESULT";
const char msg32[] PROGMEM = "Turn engine On/Off to save";
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
const char msg40[] PROGMEM = " THE LEVEL OF MOTOR OIL IS LOW!";
const char msg41[] PROGMEM = "Oil level is detected as below 25%.";
const char msg42[] PROGMEM = " Please add more.";
//
// Warning Battery
const char msg43[] PROGMEM = "  MALFUNCTION IN EL. SYSTEM!";
const char msg44[] PROGMEM = "A voltage of: ";
const char msg45[] PROGMEM = "which is not in  working limits.";
//
// Warning breaks
const char msg46[] PROGMEM = " BREAK WARN OUT!";
const char msg47[] PROGMEM = "Determined pad wear in one of ";
const char msg48[] PROGMEM = "the front breaks ";
//
// Warning coolant
const char msg49[] PROGMEM = " COOLANT LEVEL IS LOW!";
const char msg50[] PROGMEM = "Check the coolant level";
const char msg51[] PROGMEM = "if is necessary add more.";
//
// Warning overheating
const char msg52[] PROGMEM = " DANGER! OVERHEATING!!!";
const char msg53[] PROGMEM = "The temperature in the cooling system";
const char msg54[] PROGMEM = "is too high, possible overheating!";
//
// Warning window washer
const char msg55[] PROGMEM = " LOW LEVEL OF WINDOW WASHER!";
const char msg56[] PROGMEM = "Window washer liquid level is below ";
const char msg57[] PROGMEM = "25%. Please add more. ";
//
// Warning bulb
const char msg58[] PROGMEM = " BURNED OUT BULB";
const char msg59[] PROGMEM = "Incandescent  malfunction in: ";
const char msg60[] PROGMEM = "Front lights ";
const char msg61[] PROGMEM = "Back lights ";
//
// Warning timing  Belt ware
const char msg62[] PROGMEM = " TIMING BELT WORN OUT WARNING";
const char msg63[] PROGMEM = "The timing belt needs replacement after";
const char msg64[] PROGMEM = "85000km (After change reset servicing)";
//
// Warning Air filter ware
const char msg65[] PROGMEM = " OIL/AIR FILTER WORN OUT WARNING";
const char msg66[] PROGMEM = "The air filter needs replacement after";
const char msg67[] PROGMEM = "42000km (After change reset servicing)";

//
// Generate message table for LCD
const char *const LcdMsgTable[] PROGMEM = {
        msg0, msg1, msg2, msg3, msg4, msg5, msg6, msg7, msg8, msg9, msg10, msg11, msg12, msg13, msg14, msg15,
        msg16, msg17, msg18, msg19, msg20, msg21, msg22, msg23, msg24, msg25, msg26, msg27, msg28, msg29, msg30,
        msg31, msg32, msg33, msg34, msg35, msg36, msg37, msg38, msg39, msg40, msg41, msg42, msg43, msg44, msg45,
        msg46, msg47, msg48, msg49, msg50, msg51, msg52, msg53, msg54, msg55, msg56, msg57, msg58, msg59, msg60,
        msg61, msg62, msg63, msg64, msg65, msg66, msg67
};



#endif //ARDUINOMID_EN_H_H

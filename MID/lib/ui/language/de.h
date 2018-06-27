//
// Created by Andreas Hagen on 27.06.2018
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
const char msg1[] PROGMEM = "bis zum nächsten mal!";
const char msg2[] PROGMEM = "ENTRY";
//
// Car state
const char msg3[] PROGMEM = "Bremsbelag: ";
const char msg4[] PROGMEM = "Kählmittelstand: ";
const char msg5[] PROGMEM = "Waschwasserstand: ";
const char msg6[] PROGMEM = "Motorölstand: ";
const char msg7[] PROGMEM = "        £ "; // Is fine message
const char msg8[] PROGMEM = "        @"; // Problem message for liquid
const char msg9[] PROGMEM = "abgenutzt"; // Problem message for brakes
const char msg10[] PROGMEM = "durchgebrannt"; // Problem message for lamps
const char msg99[] PROGMEM = "VOLTMETER";
const char msg100[] PROGMEM = "Zahnriemen";
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
const char msg37[] PROGMEM = " Glatteisgefahr!";
const char msg38[] PROGMEM = " ";
const char msg39[] PROGMEM = "Bitte vorsichtig fahren!";
//
// Warning Oil level
const char msg40[] PROGMEM = " Motorölstand!";
const char msg41[] PROGMEM = "Motorölstand unter 25%";
const char msg42[] PROGMEM = "Motoröl auffüllen";
//
// Warning Battery
const char msg43[] PROGMEM = "Batterie Problem";
const char msg44[] PROGMEM = "Spannung ist: ";
const char msg45[] PROGMEM = "ausserhalb der Vorschrift";
//
// Warning breaks
const char msg46[] PROGMEM = " Bremsbelag!";
const char msg47[] PROGMEM = "Bitte Bremsbeläge an ";
const char msg48[] PROGMEM = "vorderen Bremsen überprüfen  ";
//
// Warning coolant
const char msg49[] PROGMEM = " Kühlmittelstand!";
const char msg50[] PROGMEM = "Kühlmittelstand überprüfen";
const char msg51[] PROGMEM = "Wenn nötig bitte auffüllen.";
//
// Warning overheating
const char msg52[] PROGMEM = " Überhitzung!!!";
const char msg53[] PROGMEM = "Motortemperatur ";
const char msg54[] PROGMEM = "ist zu hoch, überhitzung";
//
// Warning window washer
const char msg55[] PROGMEM = " Waschwasser!";
const char msg56[] PROGMEM = "Wasserstand überprüfen ";
const char msg57[] PROGMEM = "Wenn nötig bitte auffüllen.";
//
// Warning bulb
const char msg58[] PROGMEM = " Glühbirne defekt ";
const char msg59[] PROGMEM = "Gühbirnen fehlfunktion";
const char msg60[] PROGMEM = "vordere Beleuchtung ";
const char msg61[] PROGMEM = "hintere Beleuchtung ";
//
// Warning timing  Belt ware
const char msg62[] PROGMEM = " Zahnriemen ";
const char msg63[] PROGMEM = "Zahnriemen verschlissen";
const char msg64[] PROGMEM = "Nach Wechsel Service Reset";
//
// Warning Air filter ware @deprecated
const char msg65[] PROGMEM = " Öl und Luftfilter ";
const char msg66[] PROGMEM = "Filterwechsel nach: ";
const char msg67[] PROGMEM = " Nach Wechsel Service Reset";
//
// Value tags
const char msg68[] PROGMEM = "´"; // l
const char msg69[] PROGMEM = "¯"; // km
const char msg70[] PROGMEM = "³"; // h
const char msg71[] PROGMEM = "´÷¥"; // L/100km
const char msg73[] PROGMEM = "¥"; // 100km
//
// Guidance
const char msg110[] PROGMEM = " 10% Widerstandtoleranz";
const char msg111[] PROGMEM = " Zahnriemen Intervall: ";
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

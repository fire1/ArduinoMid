//
// Created by Andreas Hagen on 9.5.2017 г..
//
// Translation: Otto Wurzer
//

#ifndef ARDUINO_MID_LAN
#define ARDUINO_MID_LAN

#include <Arduino.h>

//
// Maximum size of message is 128 symbols
// Die Maximale Größe der Nachricht beträgt 128 Zeichen
//
//
const char msg0[] PROGMEM = "";
const char msg72[] PROGMEM = "Auf Wiedersehen...";
const char msg1[] PROGMEM = "bis zum nächsten Mal!";
const char msg2[] PROGMEM = "EINGABE";
//
// Car state
const char msg3[] PROGMEM = "Bremsenzustand: ";
const char msg4[] PROGMEM = "Kühlmittelstand: ";
const char msg5[] PROGMEM = "Wischwasserstand: ";
const char msg6[] PROGMEM = "Motorölstand: ";
const char msg7[] PROGMEM = "        £ "; // Is fine message
const char msg8[] PROGMEM = "        @"; // Problem message for liquid
const char msg9[] PROGMEM = "VERSCHLISSEN"; // Problem message for brakes
const char msg10[] PROGMEM = "AUSGEBRANT"; // Problem message for lamps
const char msg99[] PROGMEM = "VOLTMETER";
const char msg100[] PROGMEM = "ZAHNRIEMEN";
//
// Titles
const char msg11[] PROGMEM = "HAUPTMENÜ";
const char msg12[] PROGMEM = "FAHRTENBUCH";
const char msg13[] PROGMEM = "KRAFTSTOFFMENÜ";
const char msg14[] PROGMEM = "WARTUNG";
const char msg15[] PROGMEM = "ÜBER";
const char msg16[] PROGMEM = "INSTRUMENTENTAFEL";
const char msg17[] PROGMEM = "KALIBRIERUNG";
const char msg18[] PROGMEM = "AKTUELL";
const char msg19[] PROGMEM = "STANDARD";
//
//
const char msg20[] PROGMEM = "To reset trip data";
const char msg21[] PROGMEM = "Hold >R and break pedal ";
const char msg22[] PROGMEM = "Firmware update: www.git.io/vHlkd";
const char msg23[] PROGMEM = "ArduinoMID version ";
const char msg24[] PROGMEM = "Drücken Sie $, um in die Einstellungen zu gelangen";
const char msg25[] PROGMEM = "Benutzen Sie $ zum navigieren ";
const char msg26[] PROGMEM = "Dücken Sie $+#, um den Wert zu ändenr";
const char msg27[] PROGMEM = "FAHRZEUGGESCHWINDIGKEIT"; // Vehicle speed sensor
const char msg28[] PROGMEM = "DREHZAHL";
const char msg29[] PROGMEM = "KILOMETERZÄHLER";
const char msg30[] PROGMEM = "VERBRAUCH";
const char msg31[] PROGMEM = "ERGEBNIS";
const char msg32[] PROGMEM = "Motor abstellen um daten zu speichern";
const char msg33[] PROGMEM = "BREMSE betätigen zum beenden";
const char msg34[] PROGMEM = " Mach eine Pause";
const char msg35[] PROGMEM = " Benommenheit festgestellt";
const char msg36[] PROGMEM = " Lampenausfall!";
//
// Warning winter
const char msg37[] PROGMEM = " VEREISTE STRASSEN MÖGLICH!";
const char msg38[] PROGMEM = " ";
const char msg39[] PROGMEM = "Bitte fahren Sie vorsichtig!";
//
// Warning Oil level
const char msg40[] PROGMEM = " NIEDRIGER MOTORÖLSTAND!";
const char msg41[] PROGMEM = "Motorölstand liegt unter 25%";
const char msg42[] PROGMEM = "Motoröl auffüllen";
//
// Warning Battery
const char msg43[] PROGMEM = "ELEKTRISCHES PROBLEM";
const char msg44[] PROGMEM = "Eine Spannung von: ";
const char msg45[] PROGMEM = "nicht im Arbeitsbereich";
//
// Warning breaks
const char msg46[] PROGMEM = " BREMSEN VERSCHLISSEN!";
const char msg47[] PROGMEM = "Bremsbelag in den ";
const char msg48[] PROGMEM = " vorderen Bremsen prüfen! ";
//
// Warning coolant
const char msg49[] PROGMEM = " KÜHLMITTEL NIEDRIG!";
const char msg50[] PROGMEM = "Kühlmittelstand prüfen,";
const char msg51[] PROGMEM = "falls nötig, nachfüllen.";
//
// Warning overheating
const char msg52[] PROGMEM = " ÜBERHITZUNG!!!";
const char msg53[] PROGMEM = "Motortemperatur ";
const char msg54[] PROGMEM = "ist zu hoch, Überhitzung";
//
// Warning window washer
const char msg55[] PROGMEM = " NIEDRIGER WISCHWASSERSTAND!";
const char msg56[] PROGMEM = "Wischwasserstand prüfen ";
const char msg57[] PROGMEM = "wenn nötig, nachfüllen.";
//
// Warning bulb
const char msg58[] PROGMEM = " GLÜHLAMPE AUSGEBRANNT ";
const char msg59[] PROGMEM = "Glühlampe Fehlfunktion";
const char msg60[] PROGMEM = "im Frontscheinwerfer: ";
const char msg61[] PROGMEM = "im Stopplicht ";
//
// Warning timing  Belt ware
const char msg62[] PROGMEM = " ZAHNRIEMEN VERSCHLISSEN";
const char msg63[] PROGMEM = "Zahnriemen abgenutzt";
const char msg64[] PROGMEM = "Nachdem Wechsel, daten zurücksetzen";
//
// Warning Air filter ware @deprecated
const char msg65[] PROGMEM = " ÖL/LUFTFILTER VERSCHLISSEN ";
const char msg66[] PROGMEM = "Luftfilter  ersetzen nach: ";
const char msg67[] PROGMEM = " (Nachdem wechsel, Service zurücksetzen)";
//
// Value tags
const char msg68[] PROGMEM = "´"; // l
const char msg69[] PROGMEM = "¯"; // km
const char msg70[] PROGMEM = "³"; // h
const char msg71[] PROGMEM = "´÷¥"; // L/100km
const char msg73[] PROGMEM = "¥"; // 100km
//
// Guidance
const char msg110[] PROGMEM = " 10% Widerstandstoleranz";
const char msg111[] PROGMEM = " Zahnriemen KM-Leistung: ";
const char msg112[] PROGMEM = "Drücken Sie # zum zurücksetzen ";
const char msg113[] PROGMEM = "Zum zurücksetzen #+$ gedrückt halten";
const char msg114[] PROGMEM = "Zum erhöhen, wert verringern";
const char msg115[] PROGMEM = "erhöhen oder reduzieren sie den Wert";
//
// Ohter
const  char msg120[]  PROGMEM = "00.0";
const  char msg121[]  PROGMEM = "00";
const  char msg122[]  PROGMEM = "  ";
const  char msg123[]  PROGMEM = "Reichweite: ";

#endif //ARDUINOMID_EN_H_H
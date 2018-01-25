//
// Created by Angel Zaprianov on 9.5.2017 г..
//
// Translation: Nemanja Bošković
//

#ifndef ARDUINO_MID_LAN
#define ARDUINO_MID_LAN

#include <Arduino.h>

//
// Maximum size of message is 128 symbols
//
//
const char msg0[] PROGMEM = "";
const char msg72[] PROGMEM = "Dovidjenja...     ";
const char msg1[] PROGMEM = "do sledeceg puta!";
const char msg2[] PROGMEM = "ULAZ";
//
// Car state
const char msg3[] PROGMEM = "Stanje disk plocica: ";
const char msg4[] PROGMEM = "Stanje rashladne tecnosti: ";
const char msg5[] PROGMEM = "Stanje tecnosti za prozore: ";
const char msg6[] PROGMEM = "Nivo motornog ulja je: ";
const char msg7[] PROGMEM = "U redu."; // Is fine message
const char msg8[] PROGMEM = "ISPOD 25%"; // Problem message for liquid
const char msg9[] PROGMEM = "POTROSENE"; // Problem message for brakes
const char msg10[] PROGMEM = "IZGORILA"; // Problem message for lamps
//
// Titles
const char msg11[] PROGMEM = "GLAVNI MENI";
const char msg12[] PROGMEM = "PUTNI ZAPISI";
const char msg13[] PROGMEM = "MENI GORIVA";
const char msg14[] PROGMEM = "SERVISIRANJE";
const char msg15[] PROGMEM = "O UREDJAJU";
const char msg16[] PROGMEM = "K. TABLA";
const char msg17[] PROGMEM = "PODESAVANJE";
const char msg18[] PROGMEM = "TRENUTNO";
const char msg19[] PROGMEM = "PODRAZUMJEVANO";
//
//
const char msg20[] PROGMEM = "Da resetujes pod. puta";
const char msg21[] PROGMEM = "Zadrzi # i pritisni kocnicu";
const char msg22[] PROGMEM = "Firmware update: www.git.io/vHlkd";
const char msg23[] PROGMEM = "ArduinoMID version ";
const char msg24[] PROGMEM = "Pritisni $ da udjes u podesavanje";
const char msg25[] PROGMEM = "Koristi $ za kretanje ";
const char msg26[] PROGMEM = "Zadrzi #+$ za promjenu vrijednosti";
const char msg27[] PROGMEM = "BRZINA VOZILA"; // Vehicle speed sensor
const char msg28[] PROGMEM = "RPM";
const char msg29[] PROGMEM = "ODOMETAR";
const char msg30[] PROGMEM = "POTROSNJA";
const char msg31[] PROGMEM = "REZULTAT";
const char msg32[] PROGMEM = "KONTAKT Ukljuci/Iskljuci da sacuvas";
const char msg33[] PROGMEM = "Koristi pedalu kocnice za kraj";
const char msg34[] PROGMEM = "   Uzmi pauzu";
const char msg35[] PROGMEM = "  Umor detektovan";
const char msg36[] PROGMEM = " Kvar sijalice!";
//
// Warning winter
const char msg37[] PROGMEM = " UPOZORENJE! Klizav kolovoz!";
const char msg38[] PROGMEM = " ";
const char msg39[] PROGMEM = "Molim vozi pazljivo!";
//
// Warning Oil level
const char msg40[] PROGMEM = " NIZAK NIVO ULJA!";
const char msg41[] PROGMEM = "Nivo ulja je ispod 25%";
const char msg42[] PROGMEM = "Dospite ulja u motor";
//
// Warning Battery
const char msg43[] PROGMEM = "ELEKTRICNI KVAR!";
const char msg44[] PROGMEM = "NAPON JE: ";
const char msg45[] PROGMEM = "Nije u radnom rasponu";
//
// Warning breaks
const char msg46[] PROGMEM = " ISTROSENE DISK PLOCICE!";
const char msg47[] PROGMEM = "Utvrdjena istrosenost na ";
const char msg48[] PROGMEM = " prednjim plocicama! ";
//
// Warning coolant
const char msg49[] PROGMEM = " NIZAK NIVO R. TECNOSTI!";
const char msg50[] PROGMEM = "Provjerite nivo i ";
const char msg51[] PROGMEM = "ako je potrebno dospite.";
//
// Warning overheating
const char msg52[] PROGMEM = " PREGRIJAVANJE!!!";
const char msg53[] PROGMEM = "Temperatura motora ";
const char msg54[] PROGMEM = "je previsoka!!!";
//
// Warning window washer
const char msg55[] PROGMEM = " NIZAK NIVO VODE BRISACA!";
const char msg56[] PROGMEM = "Provjerite nivo i ";
const char msg57[] PROGMEM = "ako je potrebno dospite.";
//
// Warning bulb
const char msg58[] PROGMEM = " SIJALICA IZGORILA ";
const char msg59[] PROGMEM = "Inkandescent kvar";
const char msg60[] PROGMEM = "Prednjih svjetala ";
const char msg61[] PROGMEM = "Zadnjih svjetala ";
//
// Warning timing  Belt ware
const char msg62[] PROGMEM = " ISTROSEN TAJMING KAIS";
const char msg63[] PROGMEM = "Zupcasti kais istrosen";
const char msg64[] PROGMEM = "Posle zamjene resetujte";
//
// Warning Air filter ware @deprecated
const char msg65[] PROGMEM = "FILTER ULJE/VAZDUH ISTROSEN ";
const char msg66[] PROGMEM = "Filter vazduha zamjena posle";
const char msg67[] PROGMEM = " (Posle zamjene resetujte servis)";
//
// Value tags
const char msg68[] PROGMEM = "±"; // л
const char msg69[] PROGMEM = "¯"; // km
const char msg70[] PROGMEM = "²"; // ч
const char msg71[] PROGMEM = "±÷¥"; // Л/100km
const char msg73[] PROGMEM = "¥"; // 100km
//
// Icons
const char msg74[] PROGMEM = "°"; // celsius
const char msg75[] PROGMEM = "¢"; // instant
const char msg76[] PROGMEM = "^"; // point
const char msg77[] PROGMEM = "`"; // time
const char msg78[] PROGMEM = "®"; // break
const char msg79[] PROGMEM = ""; //
const char msg80[] PROGMEM = "¡"; // road
const char msg81[] PROGMEM = "¶"; // coffee
const char msg82[] PROGMEM = "·"; // temperature
const char msg83[] PROGMEM = "¼"; // connection
const char msg84[] PROGMEM = "½"; // settings
const char msg85[] PROGMEM = "¾"; // petrol station
const char msg86[] PROGMEM = "¿"; // car
const char msg87[] PROGMEM = "¨"; // cloudy
const char msg88[] PROGMEM = "©"; // oven /ice
const char msg89[] PROGMEM = "ª"; // sun
const char msg90[] PROGMEM = "«"; // petrol
const char msg91[] PROGMEM = "¬"; // bottom
const char msg92[] PROGMEM = "®"; // break
const char msg93[] PROGMEM = "¹"; // lamp
const char msg94[] PROGMEM = "ø"; // average
const char msg95[] PROGMEM = "¬"; // Switch arrow
const char msg96[] PROGMEM = "»"; // LPG
const char msg97[] PROGMEM = "¦"; // inside temperature
const char msg98[] PROGMEM = "§"; // outside temperature


//
// Generate message table for LCD
const char *const LcdMsgTable[] PROGMEM = {
        msg0, msg1, msg2, msg3, msg4, msg5, msg6, msg7, msg8, msg9, msg10, msg11, msg12, msg13, msg14, msg15,
        msg16, msg17, msg18, msg19, msg20, msg21, msg22, msg23, msg24, msg25, msg26, msg27, msg28, msg29, msg30,
        msg31, msg32, msg33, msg34, msg35, msg36, msg37, msg38, msg39, msg40, msg41, msg42, msg43, msg44, msg45,
        msg46, msg47, msg48, msg49, msg50, msg51, msg52, msg53, msg54, msg55, msg56, msg57, msg58, msg59, msg60,
        msg61, msg62, msg63, msg64, msg65, msg66, msg67, msg68, msg69, msg70, msg71, msg72, msg73, msg74, msg75,
        msg76, msg77, msg78, msg79, msg80, msg81, msg82, msg83, msg84, msg85, msg86, msg87, msg88, msg89, msg90,
        msg91, msg92, msg93, msg94, msg95, msg96, msg97, msg98
};


#endif //ARDUINOMID_EN_H_H
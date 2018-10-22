//
// Created by Angel Zaprianov on 2018-10-22.
//

#ifndef ARDUINO_MID_LT_H
#define ARDUINO_MID_LT_H


#include <Arduino.h>
//
// Maximum size of message is 128 symbols
//
//
const char msg0[] PROGMEM = "";
const char msg72[] PROGMEM = "Viso...     ";
const char msg1[] PROGMEM = "iki kito karto!";
const char msg2[] PROGMEM = "ĮVESTIS";
//
// Car state
const char msg3[] PROGMEM = "Stabdžių nusidėvėjimo būklė: ";
const char msg4[] PROGMEM = "Aušinimo skysčio lygis: ";
const char msg5[] PROGMEM = "Langų plovimo skysčio lygis: ";
const char msg6[] PROGMEM = "Variklio alyvos lygis: ";
const char msg7[] PROGMEM = "        £ "; // Is fine message
const char msg8[] PROGMEM = "        @"; // Problem message for liquid
const char msg9[] PROGMEM = "Nusidėvėjo!"; // Problem message for brakes
const char msg10[] PROGMEM = "Pasibaigė!"; // Problem message for lamps
const char msg99[] PROGMEM = "VOLTMETRAS";
const char msg100[] PROGMEM = "PASKIRSTYMO DIRŽAS";
//
// Titles
const char msg11[] PROGMEM = "PAGRINDINIS MENIU";
const char msg12[] PROGMEM = "KELIONIŲ ĮRAŠAI";
const char msg13[] PROGMEM = "KURO MENIU";
const char msg14[] PROGMEM = "SERVISAS";
const char msg15[] PROGMEM = "APIE";
const char msg16[] PROGMEM = "PRIETAISŲ SKYDELIS";
const char msg17[] PROGMEM = "KALIBRACIJA";
const char msg18[] PROGMEM = "NUSTATYTAS";
const char msg19[] PROGMEM = "NUMATYTAS";
//
//
const char msg20[] PROGMEM = "Nustatyti iš naujo kelionės duomenis";
const char msg21[] PROGMEM = "Laikykite >R ir stabdžio pedalą";
const char msg22[] PROGMEM = "Programinės įrangos atnaujinimas";
const char msg23[] PROGMEM = "ArduinoMID versija ";
const char msg24[] PROGMEM = "Paspauskite $ jog įeitumėte į nustatymus";
const char msg25[] PROGMEM = "Naudokite $ naršymui ";
const char msg26[] PROGMEM = "Paspauskite $+# jog pakeistumėte reikšmes";
const char msg27[] PROGMEM = "Automobilio greitis"; // Vehicle speed sensor
const char msg28[] PROGMEM = "APSUKOS";
const char msg29[] PROGMEM = "ODOMETRAS";
const char msg30[] PROGMEM = "SĄNAUDOS";
const char msg31[] PROGMEM = "REZULTATAS";
const char msg32[] PROGMEM = "Užgesinkite variklį jog išsaugotumėte duomenis";
const char msg33[] PROGMEM = "Norint užbaigti – nuspauskite stabdžio pedalą";
const char msg34[] PROGMEM = "Pailsėkite!";
const char msg35[] PROGMEM = "Pastebėtas mieguistumas";
const char msg36[] PROGMEM = "Lemputės gedimas!";
//
// Warning winter
const char msg37[] PROGMEM = "PERSPĖJIMAS: APLEDĖJĘS KELIAS!";
const char msg38[] PROGMEM = " ";
const char msg39[] PROGMEM = "Vairuokite atsargiai!";
//
// Warning Oil level
const char msg40[] PROGMEM = "ŽEMAS ALYVOS LYGIS!";
const char msg41[] PROGMEM = "Alyvos lygis žemesnis nei 25%";
const char msg42[] PROGMEM = "Papildykite variklio alyvos kiekį";
//
// Warning Battery
const char msg43[] PROGMEM = "ELEKTROS GEDIMAS";
const char msg44[] PROGMEM = "Įtampa: ";
const char msg45[] PROGMEM = "neatitinka darbinio intervalo";
//
// Warning breaks
const char msg46[] PROGMEM = "SUSIDEVĖJĘ STABDŽIAI!";
const char msg47[] PROGMEM = "Nustatytas kaladėlių nusidevėjimas ";
const char msg48[] PROGMEM = " priekinių stabdžių! ";
//
// Warning coolant
const char msg49[] PROGMEM = "ŽEMAS AUŠINIMO SKYSČIO LYGIS!";
const char msg50[] PROGMEM = "Patikrinkite aušinimo skysčio kiekį";
const char msg51[] PROGMEM = "jei reikia - papildykite.";
//
// Warning overheating
const char msg52[] PROGMEM = "PERKAITIMAS!!!";
const char msg53[] PROGMEM = "Variklio temperatūra ";
const char msg54[] PROGMEM = "yra per aukšta!";
//
// Warning window washer
const char msg55[] PROGMEM = "ŽEMAS LANGŲ PLOVIKLIO LYGIS!";
const char msg56[] PROGMEM = "Patikrinkite langų ploviklio kiekį ";
const char msg57[] PROGMEM = "jei reikia - papildykite.";
//
// Warning bulb
const char msg58[] PROGMEM = " PERDEGUSI LEMPUTĖ! ";
const char msg59[] PROGMEM = "Žibinto sutrikimas";
const char msg60[] PROGMEM = "Priekinio žibinto ";
const char msg61[] PROGMEM = "Galinio žibinto ";
//
// Warning timing  Belt ware
const char msg62[] PROGMEM = " NUSIDEVĖJĘS PASKIRSTYMO DIRŽAS";
const char msg63[] PROGMEM = "Paskirstymo diržas nusidevėjęs";
const char msg64[] PROGMEM = "Po pakeitimo – atnaujinkite duomenis";
//
// Warning Air filter ware @deprecated
const char msg65[] PROGMEM = " ORO/TEPALŲ FILTRAS NUSIDEVĖJĘS ";
const char msg66[] PROGMEM = "Oro filtro keitimas po:";

const char msg67[] PROGMEM = "Po pakeitimo – atnaujinkite duomenis ";
//
// Value tags
const char msg68[] PROGMEM = "´"; // l
const char msg70[] PROGMEM = "³"; // h
const char msg71[] PROGMEM = "´÷¥"; // L/100km
//
// Guidance
const char msg110[] PROGMEM = " Rezistoriaus paklaida - 10%";
const char msg111[] PROGMEM = " Paskirstymo diržo tarnavimo laikas: ";
const char msg112[] PROGMEM = "Paspauskite # jog nustatytumėtę iš naujo ";
const char msg113[] PROGMEM = "Nustatyti iš naujo – paspauskite #+$";
const char msg114[] PROGMEM = "Padidinti, mažinti reikšmę";
const char msg115[] PROGMEM = "Padidinri arba mažinti reikšmę";
//
// Ohter
const  char msg120[]  PROGMEM = "00.0";
const  char msg121[]  PROGMEM = "00";
const  char msg122[]  PROGMEM = "  ";
const  char msg123[]  PROGMEM = "Intervalas: ";

#endif //ARDUINOMID_LT_H_H

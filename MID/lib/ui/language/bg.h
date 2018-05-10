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
const char msg72[] PROGMEM = "Довиждане...     ";
const char msg1[] PROGMEM = "до следващия път!";
const char msg2[] PROGMEM = "Преход ";
//
// Car state
const char msg3[] PROGMEM = "   Предни  накладки: ";
const char msg4[] PROGMEM = "Охладителна течност: ";
const char msg5[] PROGMEM = "Течност за чистачки: ";
const char msg6[] PROGMEM = " Ниво моторно масло: ";
const char msg7[] PROGMEM = "        £ "; // Is fine message
const char msg8[] PROGMEM = "        @"; // Problem message for liquid
const char msg9[] PROGMEM = "Износени"; // Problem message for brakes
const char msg10[] PROGMEM = "Изгорели"; // Problem message for lamps
//
// Titles
const char msg11[] PROGMEM = "НАЧАЛО";
const char msg12[] PROGMEM = "ПЪТУВАНИЯ";
const char msg13[] PROGMEM = "ГОРИВА";
const char msg14[] PROGMEM = "ОБСЛУЖВАНЕ";
const char msg15[] PROGMEM = "ОТНОСНО";
const char msg16[] PROGMEM = "ИНСТРУМЕНТИ";
const char msg17[] PROGMEM = "НАСТРОЙКИ";
const char msg18[] PROGMEM = "ТЕКУЩО";
const char msg19[] PROGMEM = "ЗАВОДСКИ";
const char msg99[] PROGMEM = "ВОЛТМЕТЪР";
const char msg100[] PROGMEM = "АНГ. РЕМЪК";
//
//
const char msg20[] PROGMEM = "За нулиране на пътуването";
const char msg21[] PROGMEM = "Задръжте # и натиснете спирачката ";
const char msg22[] PROGMEM = "За нови версии: www.git.io/vHlkd";
const char msg23[] PROGMEM = "ArduinoMID версия ";
const char msg24[] PROGMEM = "Натиснете $ за промяна";
const char msg25[] PROGMEM = "Използвай $ за навигация ";
const char msg26[] PROGMEM = "Използвай #+$ за промяна";
const char msg27[] PROGMEM = "СКОРОСТ"; // Vehicle speed sensor
const char msg28[] PROGMEM = "ОБОРОТИ";
const char msg29[] PROGMEM = "ОДОМЕТЪР";
const char msg30[] PROGMEM = "КОНСУМАЦИЯ";
const char msg31[] PROGMEM = "РЕЗУЛТАТ";
const char msg32[] PROGMEM = "Изгасете двигателя за запазване.";
const char msg33[] PROGMEM = "Използвайте спирачката за край";
const char msg34[] PROGMEM = " Направете почивка";
const char msg35[] PROGMEM = " Опасност сънливост ";
const char msg36[] PROGMEM = " Не изправност на крушка";
//
// Warning winter
const char msg37[] PROGMEM = "ЗАЛЕДЯВАНИЯ ПО ПЪТЯ"; // max 19 char
const char msg38[] PROGMEM = " ";
const char msg39[] PROGMEM = "Шофирайте внимателно";
//
// Warning Oil level
const char msg40[] PROGMEM = "СМАЗОЧНА СИСТЕМА";
const char msg41[] PROGMEM = "Проверете нивото на";
const char msg42[] PROGMEM = "моторното масло";
//
// Warning Battery
const char msg43[] PROGMEM = "ЕЛ. СИСТЕМА";
const char msg44[] PROGMEM = "Напрежение от: ";
const char msg45[] PROGMEM = "не е в работни граници";
//
// Warning breaks
const char msg46[] PROGMEM = "СПИРАЧНА СИСТЕМА";
const char msg47[] PROGMEM = "Износване на ";
const char msg48[] PROGMEM = "предните спирачки  ";
//
// Warning coolant
const char msg49[] PROGMEM = "ОХЛ. СИСТЕМА";
const char msg50[] PROGMEM = "Проверете нивото на";
const char msg51[] PROGMEM = "охладителната течност";
//
// Warning overheating
const char msg52[] PROGMEM = "ОПАСНОСТ ПРЕГРЯВАНЕ";
const char msg53[] PROGMEM = "Температурата ";
const char msg54[] PROGMEM = "на двигателя е висока!";
//
// Warning window washer
const char msg55[] PROGMEM = "ТЕЧНОСТ ЗА ЧИСТАЧКИ";
const char msg56[] PROGMEM = "Проверете нивото на";
const char msg57[] PROGMEM = "течността за чистачки";
//
// Warning window washer
const char msg58[] PROGMEM = "ИЗГОРЯЛА КРУШКА";
const char msg59[] PROGMEM = "Неработеща крушка в:";
const char msg60[] PROGMEM = "Предни светлини";
const char msg61[] PROGMEM = "Задни светлини ";
//
// Warning timing  Belt ware
const char msg62[] PROGMEM = "АНГРЕНАЖЕН РЕМЪК";
const char msg63[] PROGMEM = "Ангренаженият ремък";
const char msg64[] PROGMEM = "е за смяна (85000km+)";
//
// Warning Air filter ware
const char msg65[] PROGMEM = " НУЖНА СМЯНА ВЪЗДУШЕН/МАСЛЕН ФИЛТЪР ";
const char msg66[] PROGMEM = "Въздушният филтър се сменя на над";
const char msg67[] PROGMEM = "42000km (След смяна нулирайте данните)";
//
// Value tags
const char msg68[] PROGMEM = "±"; // л
const char msg69[] PROGMEM = "¯"; // km
const char msg70[] PROGMEM = "²"; // ч
const char msg71[] PROGMEM = "±÷¥"; // Л/100km
const char msg73[] PROGMEM = "¥"; // 100km
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

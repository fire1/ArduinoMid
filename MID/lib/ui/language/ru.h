//
// Created by Angel Zaprianov on 9.5.2017 г..
//
// Translation: Artyr Prozenko
//

#ifndef ARDUINO_MID_LAN
#define ARDUINO_MID_LAN

#include <Arduino.h>

//
// Maximum size of message is 128 symbols
//
//
const char msg0[] PROGMEM = "";
const char msg72[] PROGMEM = "Прощай...     ";
const char msg1[] PROGMEM = "до скорого!";
const char msg2[] PROGMEM = "ENTRY";
//
// Car state
const char msg3[] PROGMEM = "Износ тормазных колодок: ";
const char msg4[] PROGMEM = "Уровень О.Ж.: ";
const char msg5[] PROGMEM = "уровень жидкости стеклоомывателя: ";
const char msg6[] PROGMEM = "Уровень масла в двигателе: ";
const char msg7[] PROGMEM = "O.K."; // Is fine message
const char msg8[] PROGMEM = "ниже 25%"; // Problem message for liquid
const char msg9[] PROGMEM = "Изношены"; // Problem message for brakes
const char msg10[] PROGMEM = "замени лампу"; // Problem message for lamps
//
// Titles
const char msg11[] PROGMEM = "Главное меню";
const char msg12[] PROGMEM = "TRIP RECORDS";
const char msg13[] PROGMEM = "топлевное меню";
const char msg14[] PROGMEM = "техобслуживание";
const char msg15[] PROGMEM = "ABOUT";
const char msg16[] PROGMEM = "DASHBOARD";
const char msg17[] PROGMEM = "Настройки";
const char msg18[] PROGMEM = "текущие";
const char msg19[] PROGMEM = "По умолчанию";
const char msg99[] PROGMEM = "ВОЛЬТМЕТР";
const char msg100[] PROGMEM = "РЕМЕНЬ ГРМ";
//
//
const char msg20[] PROGMEM = "Сброс данны о поездки";
const char msg21[] PROGMEM = "Удерживать >R и педаль тормаза ";
const char msg22[] PROGMEM = "Firmware update: www.git.io/vHlkd";
const char msg23[] PROGMEM = "ArduinoMID version ";
const char msg24[] PROGMEM = "Нажмите S< для входа в настройки";
const char msg25[] PROGMEM = "используйте S< для навигации ";
const char msg26[] PROGMEM = "Dabble press R<+S< to change value";
const char msg27[] PROGMEM = "Скорость авто"; // Vehicle speed sensor
const char msg28[] PROGMEM = "ОБОРОТИ";
const char msg29[] PROGMEM = "ОДОМЕТЪР";
const char msg30[] PROGMEM = "РАСХОД";
const char msg31[] PROGMEM = "РЕЗУЛЬТАТ";
const char msg32[] PROGMEM = "Вкл/вык двигатель, чтобы сохранить";
const char msg33[] PROGMEM = "Использовать тормоза для край";
const char msg34[] PROGMEM = "Сделайте перерыв";
const char msg35[] PROGMEM = "Обнаружена сонливость";
const char msg36[] PROGMEM = "Сбой лампы!";
//
// Warning winter
const char msg37[] PROGMEM = " ROAD ICING WARNING!";
const char msg38[] PROGMEM = " ";
const char msg39[] PROGMEM = "Please drive safe!";
//
// Warning Oil level
const char msg40[] PROGMEM = " Низкий уровень масла!";
const char msg41[] PROGMEM = "масла ниже 25%";
const char msg42[] PROGMEM = "Долить масло";
//
// Warning Battery
const char msg43[] PROGMEM = "Проблемы питания";
const char msg44[] PROGMEM = "Напряжение: ";
const char msg45[] PROGMEM = "Вне диапазона";
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
const char msg54[] PROGMEM = "is too high, overheating!";
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
const char msg79[] PROGMEM = "·"; // waste drop
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
const char msg101[] PROGMEM = " "; // empty space

//
// Generate message table for LCD
const char *const LcdMsgTable[] PROGMEM = {
        msg0, msg1, msg2, msg3, msg4, msg5, msg6, msg7, msg8, msg9, msg10, msg11, msg12, msg13, msg14, msg15,
        msg16, msg17, msg18, msg19, msg20, msg21, msg22, msg23, msg24, msg25, msg26, msg27, msg28, msg29, msg30,
        msg31, msg32, msg33, msg34, msg35, msg36, msg37, msg38, msg39, msg40, msg41, msg42, msg43, msg44, msg45,
        msg46, msg47, msg48, msg49, msg50, msg51, msg52, msg53, msg54, msg55, msg56, msg57, msg58, msg59, msg60,
        msg61, msg62, msg63, msg64, msg65, msg66, msg67, msg68, msg69, msg70, msg71, msg72, msg73, msg74, msg75,
        msg76, msg77, msg78, msg79, msg80, msg81, msg82, msg83, msg84, msg85, msg86, msg87, msg88, msg89, msg90,
        msg91, msg92, msg93, msg94, msg95, msg96, msg97, msg98, msg101
};


#endif //ARDUINOMID_EN_H_H

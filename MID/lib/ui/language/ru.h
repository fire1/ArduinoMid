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
const char msg2[] PROGMEM = "Запись";
//
// Car state
const char msg3[] PROGMEM = "Износ тормазных колодок: ";
const char msg4[] PROGMEM = "Уровень О.Ж.: ";
const char msg5[] PROGMEM = "уровень жидкости стеклоомывателя: ";
const char msg6[] PROGMEM = "Уровень масла в двигателе: ";
const char msg7[] PROGMEM = "        £ "; // Is fine message
const char msg8[] PROGMEM = "        @"; // Problem message for liquid
const char msg9[] PROGMEM = "Изношены"; // Сообщение об износе колодок
const char msg10[] PROGMEM = "замени лампу"; // Сообщение о неисправности лампы
//
// Titles
const char msg11[] PROGMEM = "Главное меню";
const char msg12[] PROGMEM = "ОТЗОВЫ";
const char msg13[] PROGMEM = "ТОПЛЕВНОЕ МЕНЮ";
const char msg14[] PROGMEM = "ТЕХОБСЛУЖИВАНИЕ";
const char msg15[] PROGMEM = "О АВТОРЕ";
const char msg16[] PROGMEM = "ПАНЕЛЬ ПРИБОРОВ";
const char msg17[] PROGMEM = "НАСТРОЙКИ";
const char msg18[] PROGMEM = "ТЕКУЩИЕ";
const char msg19[] PROGMEM = "ПО УМОЛЧАНИЮ";
const char msg99[] PROGMEM = "ВОЛЬТМЕТР";
const char msg100[] PROGMEM = "РЕМЕНЬ ГРМ";
//
//
const char msg20[] PROGMEM = "Сброс данны о поездки";
const char msg21[] PROGMEM = "Удерживать >R и педаль тормаза ";
const char msg22[] PROGMEM = "Обновление прошивки: www.git.io/vHlkd";
const char msg23[] PROGMEM = "Версия ArduinoMID ";
const char msg24[] PROGMEM = "Нажмите S< для входа в настройки";
const char msg25[] PROGMEM = "используйте S< для навигации ";
const char msg26[] PROGMEM = "Dabble нажмите R<+S< чтобы изменить значение";
const char msg27[] PROGMEM = "Скорость авто"; // Датчик скорости автомобиля
const char msg28[] PROGMEM = "ОБОРОТЫ";
const char msg29[] PROGMEM = "ОДОМЕТР";
const char msg30[] PROGMEM = "РАСХОД";
const char msg31[] PROGMEM = "РЕЗУЛЬТАТ";
const char msg32[] PROGMEM = "Вкл/вык двигатель, чтобы сохранить";
const char msg33[] PROGMEM = "Использовать тормоза для край";
const char msg34[] PROGMEM = "Сделайте перерыв";
const char msg35[] PROGMEM = "Обнаружена сонливость";
const char msg36[] PROGMEM = "Сбой лампы!";
//
// Warning winter
const char msg37[] PROGMEM = " ОБЛЕДЕНЕНИЕ ДОРОГИ!";
const char msg38[] PROGMEM = " ";
const char msg39[] PROGMEM = "Пожалуйста пристегнитесь!";
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
const char msg46[] PROGMEM = " РАБОЧИЕ ТОРМОЗА!";
const char msg47[] PROGMEM = "Определенный износ подушки в ";
const char msg48[] PROGMEM = " передние тормоза! ";
//
// Warning coolant
const char msg49[] PROGMEM = " НИЗКОЕ ОХЛАЖДАЮЩАЯ ЖИДКОСТЬ!";
const char msg50[] PROGMEM = "Проверьте уровень охлаждающей жидкости";
const char msg51[] PROGMEM = "если необходимо добавить еще.";
//
// Warning overheating
const char msg52[] PROGMEM = " ПЕРЕГРЕВ!!!";
const char msg53[] PROGMEM = "Температура двигателя ";
const char msg54[] PROGMEM = "высок риск перегрева!";
//
// Warning window washer
const char msg55[] PROGMEM = " УРОВЕНЬ ОМЫВАЙКИ!";
const char msg56[] PROGMEM = "Проверте уровень ";
const char msg57[] PROGMEM = "если необходимо добавить еще.";
//
// Warning bulb
const char msg58[] PROGMEM = " ЛАМПЫ ";
const char msg59[] PROGMEM = "Лампа накаливания";
const char msg60[] PROGMEM = "В передних фарах ";
const char msg61[] PROGMEM = "В задних фонарях ";
//
// Warning timing  Belt ware
const char msg62[] PROGMEM = " ПОРЯДОК РАБОТЫ";
const char msg63[] PROGMEM = "Износ ремня ГРМ";
const char msg64[] PROGMEM = "После обслуживания сбросить";
//
// Warning Air filter ware @deprecated
const char msg65[] PROGMEM = " МАСЛЕНЫЙ/ВОЗДУШНЫЙ ФИЛЬТР ";
const char msg66[] PROGMEM = "Замените воздушный фильтр";
const char msg67[] PROGMEM = " (После обслуживания сбросить)";
//
// Value tags
const char msg68[] PROGMEM = "±"; // л
const char msg69[] PROGMEM = "?"; // km
const char msg70[] PROGMEM = "?"; // ч
const char msg71[] PROGMEM = "±??"; // Л/100km
const char msg73[] PROGMEM = "?"; // 100km


#endif //ARDUINOMID_EN_H_H
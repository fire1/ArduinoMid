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
const char msg72[] PROGMEM = "Bir dahaki sefere kadar, ";
const char msg1[] PROGMEM = " HOSÇAKALIN... ";
const char msg2[] PROGMEM = " GIRIŞ";
//
// Car state
const char msg3[] PROGMEM = " Fren balatası durumu   : ";
const char msg4[] PROGMEM = " Soğutma sıvısı seviyesi: ";
const char msg5[] PROGMEM = " Cam suyu seviyesi        : ";
const char msg6[] PROGMEM = " Motor Yağı seviyesi       : ";
const char msg7[] PROGMEM = "        £ "; // Is fine message
const char msg8[] PROGMEM = "        @"; // Problem message for liquid
const char msg9[] PROGMEM = " YIPRANMIŞ"; // Problem message for brakes
const char msg10[] PROGMEM = "SÖNÜK"; // Problem message for lamps
//
// Titles
const char msg11[] PROGMEM = "ANA MENÜ";
const char msg12[] PROGMEM = "SEYAHAT";
const char msg13[] PROGMEM = "YAKIT MENÜSÜ";
const char msg14[] PROGMEM = "SERVIS";
const char msg15[] PROGMEM = "HAKKINDA";
const char msg16[] PROGMEM = "GÖSTERGE";
const char msg17[] PROGMEM = "AYARLAR";
const char msg18[] PROGMEM = "GÜNCEL";
const char msg19[] PROGMEM = "VARSAYILAN";
const char msg99[] PROGMEM = "VOLTMETRE";
const char msg100[] PROGMEM = "ZAMAN KAYIŞI";
//
//
const char msg20[] PROGMEM = "Seyahat verisini sıfırlamak için ";
const char msg21[] PROGMEM = "> R ve fren pedalını basılı tutun ";
const char msg22[] PROGMEM = "CreateD by S_H";
const char msg23[] PROGMEM = "Cihaz versiyon ";
const char msg24[] PROGMEM = "Ayarlara girmek için \"S<\" Tıklayınız";
const char msg25[] PROGMEM = "Navigasyonu \"S<\" kullan ";
const char msg26[] PROGMEM = "Ayarları Değiştir R<+S< Çift Tıkla";
const char msg27[] PROGMEM = "ARAÇ HIZI "; // Vehicle speed sensor
const char msg28[] PROGMEM = "DEVIR";
const char msg29[] PROGMEM = "KILOMETRE";
const char msg30[] PROGMEM = "TÜKETIM";
const char msg31[] PROGMEM = "SONUÇ";
const char msg32[] PROGMEM = "Motoru kaydetmek için On / Off";
const char msg33[] PROGMEM = "Bitirmek için FREN pedalını kullan";
const char msg34[] PROGMEM = " Ara ver";
const char msg35[] PROGMEM = " Yorgunluk tespit edildi";
const char msg36[] PROGMEM = " Ampul arızası!";
//
// Warning winter
const char msg37[] PROGMEM = " BUZLANMA UYARISI";
const char msg38[] PROGMEM = " ";
const char msg39[] PROGMEM = " Lütfen güvenli sür !";
//
// Warning Oil level
const char msg40[] PROGMEM = "DÜŞÜK YAĞ SEVIYESI!";
const char msg41[] PROGMEM = "Yağ seviyesi %25'in alt.";
const char msg42[] PROGMEM = "Motor yağı doldurun";
//
// Warning Battery
const char msg43[] PROGMEM = " ELEKTRIK KONTROLU!";
const char msg44[] PROGMEM = " Çalışma aralığında olmayan ";
const char msg45[] PROGMEM = " gerilim :";
//
// Warning breaks
const char msg46[] PROGMEM = " FRENLER KAPALI !";
const char msg47[] PROGMEM = " Balatalarda yıpranma ";
const char msg48[] PROGMEM = " Ondeki frenler ! ";
//
// Warning coolant
const char msg49[] PROGMEM = " ANTIFIRIZ AZALDI !";
const char msg50[] PROGMEM = " Antifirizi kontrol et";
const char msg51[] PROGMEM = " Gerekirse ekleme yap";
//
// Warning overheating
const char msg52[] PROGMEM = " AŞIRI ISINMA!!!";
const char msg53[] PROGMEM = "Aşırı ısınmış";
const char msg54[] PROGMEM = "Motor sıcaklığı!";
//
// Warning window washer
const char msg55[] PROGMEM = " CAM SUYU AZALDI !";
const char msg56[] PROGMEM = " Cam suyunu kontrol et ";
const char msg57[] PROGMEM = " Gerekirse ekleme yap";
//
// Warning bulb
const char msg58[] PROGMEM = " Lamba Arızası(Yanma) ";
const char msg59[] PROGMEM = " Akkor arızası ";
const char msg60[] PROGMEM = " Ondeki farlarda ";
const char msg61[] PROGMEM = " Arka farlarda ";
//
// Warning timing  Belt ware
const char msg62[] PROGMEM = " TRIGER KAYISI ARIZASI";
const char msg63[] PROGMEM = " Triger kayışı yıpranmış";
const char msg64[] PROGMEM = " Veriyi sıfırla sonra değiştir";
//
// Warning Air filter ware @deprecated
const char msg65[] PROGMEM = " YAG/HAVA FILTRESI YIPRANMIŞ";
const char msg66[] PROGMEM = " Hava filtresi degisimi ";
const char msg67[] PROGMEM = "(Sıfırlama - servis sonrası)";
//
// Value tags
const char msg68[] PROGMEM = "´"; // l
const char msg70[] PROGMEM = "³"; // h
const char msg71[] PROGMEM = "´÷¥"; // L/100km
//
// Guidance
const char msg110[] PROGMEM = " 10%  direnç toleransı";
const char msg111[] PROGMEM = "Triger kayışı mesafesi: ";
const char msg112[] PROGMEM = "Sıfırlamak için # tuşuna basın ";
const char msg113[] PROGMEM = "Beklemeyi sıfırlamak için #+$";
const char msg114[] PROGMEM = "Artırmak, değerini düşürmek";
const char msg115[] PROGMEM = "Değeri artırın veya azaltın";
//
// Ohter
const  char msg120[]  PROGMEM = "00.0";
const  char msg121[]  PROGMEM = "00";
const  char msg122[]  PROGMEM = "  ";
const  char msg123[]  PROGMEM = "Menzil: ";

#endif //ARDUINOMID_TR_H_H

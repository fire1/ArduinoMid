//
// Created by Angel Zaprianov on 8.2.2017 г..
//

#ifndef ARDUINO_MID_LCD240X64_H
#define ARDUINO_MID_LCD240X64_H
//#include "CarGames.h"
#include <Arduino.h>
#include "../conf.h"
#include "MainFunc.h"
#include "Menu16x2.h"
#include "ShutDw.h"
#include "CarState.h"
#include <U8g2lib.h>
#include "Menu240x64.h"
#include "graphics/240x64-logo.h"

#ifndef _U8G2LIB_HH
// Some IDE syntax mishmash fixer
#include "../../libraries/U8g2/src/U8g2lib.h"

#endif


class Lcd240x62 : virtual public LcdMenuInterface {


    U8G2 *lcd;
    IntAmp *amp;
    CarSens *car;
    EepRom *eep;
    CarState *stt;
    WhlSens *whl;
    MenuBase *mbs;
    ShutDw *sdw;


protected:
    uint8_t drawState = 0;

    void menus(uint8_t);

    void aniHrzChar(u8g2_uint_t x, u8g2_uint_t y, const char *str) {
        lcd->drawUTF8(aniIndex * 3, 36, str);
    }

    void prepareScreen() {
        // default u8g2_font_6x10_tf
        lcd->setFont(u8g2_font_6x10_tf);
        lcd->setFontRefHeightExtendedText();
        lcd->setDrawColor(1);
        lcd->setFontPosTop();
        lcd->setFontDirection(0);
    }

/**
 *
 * @param index of loop
 */
    void displayEntry(uint8_t index) {
        lcd->drawStr(0, 0, "MENU CHANGE ");
        switch (index) {

            case 0:
                lcd->clear();
                mbs->startEntry();
                break;
            case 1:
                tone(TONE_ADT_PIN, 2800, 16);
            case 2:
            case 3:
            case 4:
                lcd->drawStr(0, 15, MenuBase::usedBack);
                lcd->drawStr(0, 30, MenuBase::usedMenu);
                lcd->drawStr(0, 45, MenuBase::usedNext);
                lcd->drawStr(78, 30, MenuBase::usedNext);
                lcd->drawUTF8(76, 30, "➞");
                break;
            default:
            case 5:
                lcd->clearBuffer();
                lcd->clear();
                mbs->finishEntry();
                drawState = 0;
                break;
        }
    }

public:
/**
 *
 * @param _lcd
 * @param _btn
 * @param _mbs
 * @param _sdw
 */
    Lcd240x62(U8G2 *_lcd, MenuBtn *_btn, MenuBase *_mbs, ShutDw *_sdw) {
        lcd = _lcd;
        amp = _btn->passAmp();
        car = _btn->passCar();
        eep = _btn->passEep();
        whl = _btn->passWhl();
        stt = _btn->passStt();
        sdw = _sdw;

    }

    void intro(void) {
        //
        // Test tone
        tone(TONE_ADT_PIN, 400, 20);
        delay(10);
        lcd->firstPage();
        do {
//            lcd->drawXBM(49, 16, 142, 33, OpelLogoBits);
            lcd->drawXBMP(68, 2, 104, 60, Fire1LogoBits);;
        } while (lcd->nextPage());
        lcd->sendBuffer();
        delay(1500);
        lcd->clear();
//        tone(TONE_ADT_PIN, 800, 10);
//        delay(10);
//        lcd->firstPage();
//        do {
//            lcd->drawXBMP(51, 20, 137, 25, ArduinoLogoBits);
//        } while (lcd->nextPage());
//        lcd->sendBuffer();
//        tone(TONE_ADT_PIN, 800, 10);
//        delay(1500);
//        lcd->clear();
//        tone(TONE_ADT_PIN, 800, 10);
//        delay(10);
//        lcd->firstPage();
//        do {
//            lcd->drawXBMP(68, 2, 104, 60, Fire1LogoBits);
//        } while (lcd->nextPage());
//        lcd->sendBuffer();
//        tone(TONE_ADT_PIN, 800, 10);
//        delay(1500);
//        delay(10);
        tone(TONE_ADT_PIN, 800, 10);
        lcd->clear();
        prepareScreen();
        mbs->savedCursor = 0;
    }

    void begin(void) {
        lcd->begin();
        prepareScreen();
    }

/**
 * Draw graphic
 */
    void draw() {
        if (amp->isMid()) {
            lcd->firstPage();
            do {
                menus(drawState);
            } while (lcd->nextPage());
            drawState++;
        }
// default state must reset it  ...
//        if (drawState >= 11)
//            drawState = 0;
    }

private:
    unsigned int aniIndex;

    void setTextMode(void) {
        //
        // u8g2_font_4x6_tf, u8g2_font_5x7_tf
        lcd->setFont(u8g2_font_6x10_tf);
        lcd->setFontRefHeightExtendedText();
        lcd->setDrawColor(1);
        lcd->setFontPosTop();
        lcd->setFontDirection(0);
    }

    void setCharMode(void) {
        lcd->drawStr(0, 0, "Unicode");
        // u8g_font_04b_03b
        lcd->setFont(u8g2_font_unifont_t_symbols);
        lcd->setFontPosTop();
    }


};

/**
 *
 * @param index
 */
void Lcd240x62::menus(uint8_t index) {
    switch (MidCursorMenu) {
        default:
        case MENU_ENTRY:
            displayEntry(index);
            break;
            //
            // Main / first menu
        case 1:
            lcd->drawStr(0, 0, "HOME MENU");
            break;
            //
            // Dashboard
        case 11:
            lcd->drawStr(0, 0, "Dashboard");
            break;

        case 12:
            lcd->drawStr(0, 0, "Menu");
            break;

            //
            // Travel menu
        case 2:
            lcd->drawStr(0, 0, "Travel menu");
            break;
        case 21:
            lcd->drawStr(0, 0, "Travel");
            break;
        case 22:
            lcd->drawStr(0, 0, "Travel");
            break;

            //
            // Fuel menu
        case 3:
            lcd->drawStr(0, 0, " Fuel menu");
            break;
        case 32:
            lcd->drawStr(0, 0, " Fuel menu");
            break;
        case 4:
        case 41:
        case 42:
        case 43:
        case 44:
        case 45:
            lcd->drawStr(0, 0, "Servicing Menu");
            break;
/*            //
            // Games menu
        case 5:

            break;
        case 51:
            gms->listenAStopwatch();

            break;
        case 52:

            break;
        case 53:
            gms->listen0to100();

            break;*/

        case ShutDw::MENU_SHUTDOWN:
            lcd->drawStr(0, 0, "Shut down");
            break;
    }

}


#endif //ARDUINOMID_LCD240X64_H

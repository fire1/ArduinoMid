//
// Created by Angel Zaprianov on 8.2.2017 г..
//

#ifndef ARDUINO_MID_LCD240X64_H
#define ARDUINO_MID_LCD240X64_H

#include <Arduino.h>
#include "../conf.h"
#include "MainFunc.h"
#include "Menu16x2.h"
#include "ShutDw.h"
#include "CarState.h"
#include "CarGames.h"
#include "Menu240x64.h"
#include <U8g2lib.h>


class Lcd240x62 : virtual public ILcdMenu {
    U8G2_T6963_240X64_2_8080 *lcd;
    IntAmp *amp;
    CarSens *car;
    EepRom *eep;
    CarState *stt;
    CarGames *gms;
    WhlSens *whl;
    IMidMenu *mmn;
    ShutDw *sdw;
protected:
    void aniHrzChar(u8g2_uint_t x, u8g2_uint_t y, const char *str) {
        lcd->drawUTF8(aniIndex * 3, 36, str);
    }

public:
    //
    // Class constructor ...
    Lcd240x62(U8G2_T6963_240X64_2_8080 *_lcd, MenuBtn *_btn, IMidMenu *_mmn, CarGames *_gms, ShutDw *_sdw) {
        lcd = _lcd;
        amp = _btn->passAmp();
        car = _btn->passCar();
        eep = _btn->passEep();
        whl = _btn->passWhl();
        stt = _btn->passStt();
        mmn = _mmn;
        sdw = _sdw;
    }

    void intro(void);

    void begin(void);

    void draw(void);

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


void Lcd240x62::begin(void) {
    lcd->begin();
}

void Lcd240x62::intro(void) {

}

void Lcd240x62::draw(void ) {
    switch (cursorMenu) {
        default:
        case MENU_ENTRY:

            break;
            //
            // Main / first menu
        case 1:

            break;
            //
            // Dashboard
        case 11:

            break;

        case 12:

            break;

            //
            // Travel menu
        case 2:

            break;
        case 21:

            break;
        case 22:

            break;

            //
            // Fuel menu
        case 3:

            break;
        case 32:

            break;
        case 4:
        case 41:
        case 42:
        case 43:
        case 44:
        case 45:

            break;
            //
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

            break;

        case ShutDw::MENU_SHUTDOWN:

            break;
    }

}


#endif //ARDUINOMID_LCD240X64_H

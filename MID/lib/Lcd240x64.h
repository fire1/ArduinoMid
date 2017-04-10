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
//
// Drowing counter
    uint8_t drawEntry = 0;

protected:

    //
    // Defining content generate container variables
    char displayChar_2[2];
    char displayChar_3[3];
    char displayChar_4[4];

    void menus(uint8_t);

    void aniHrzChar(u8g2_uint_t x, u8g2_uint_t y, const char *str) {
        lcd->drawUTF8(aniIndex * 3, 36, str);
    }

    void prepareScreen() {
        // default u8g2_font_6x10_tf
//        lcd->setFont(u8g2_font_crox1t_tf   );
        lcd->setFont(u8g2_font_6x10_tf);
        lcd->setFontRefHeightExtendedText();
        lcd->setDrawColor(1);
        lcd->setFontPosTop();
        lcd->setFontDirection(0);
    }

    void useUtf8() {
        lcd->setFont(u8g2_font_unifont_t_symbols);
        lcd->setFontPosTop();
    }


/**
 *
 * @param index of loop
 */
    void displayEntry(uint8_t index) {
        if (drawEntry > 10) {
            drawEntry = 0;
        }


        switch (index) {
            default:
                drawEntry = 0;
                break;

            case 0:
                lcd->clear();
                mbs->startEntry();
                drawEntry = 0;
                btnMenu.setNavigationState(false);
                tone(TONE_ADT_PIN, 2800, 16);
                break;
            case 1:
            case 2:
            case 3:
                lcd->drawStr(12, 5 - (index * 5), usedMenu.back);
                lcd->drawStr(12, 35 - (index * 5), usedMenu.used);
                lcd->drawStr(72, 35 - (index * 5), usedMenu.down);
                lcd->drawStr(12, 20 - (index * 5), usedMenu.next);
                lcd->drawStr(12, 50 - (index * 5), usedMenu.last);

                lcd->drawFrame(10, 18, 212, 15);
                break;
            case 4:
                lcd->clearBuffer();
                lcd->clear();
                break;
            case 5:

                btnMenu.setNavigationState(true);
                usedMenu = {};
                mbs->finishEntry();
                lcd->clear();
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

/**
 * Mid's intro
 */
    void intro(void) {
        //
        // Test tone
        tone(TONE_ADT_PIN, 400, 20);
        delay(10);
        lcd->firstPage();
        do {
            lcd->drawXBMP(0, 0, 240, 64, OpelLogoBits);
        } while (lcd->nextPage());
        delay(1500);
        lcd->clear();
        tone(TONE_ADT_PIN, 800, 10);
    }

    void begin(void) {
        lcd->begin();
        prepareScreen();
        lcd->enableUTF8Print();

    }

/**
 * Draw graphic
 */
    void draw() {
        if (amp->isSec()) {
            lcd->firstPage();
            do {
                menus(drawEntry);
            } while (lcd->nextPage());
            drawEntry++;
        }
    }

private:

    float getConsumedFuel() {
        //
        // Load saved data
        SavedData data = eep->getData();
        //
        // Switching between LPG / BNZ
        if (car->getFuelState() == 0) { // BNZ [default]
            return data.fuel_def + car->getDefFuelCns();
        }
        if (car->getFuelState() == 1) { // LPG [additional]
            return data.fuel_adt + car->getAdtFuelCns();
        }
    }

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

/**
 * Displays consumed fuel
 */
    void displayTotalConsumption() {

//        useUtf8();
//        lcd->drawUTF8(2, 10, "⛽ ");//\\u26FD
//        lcd->drawStr(2, 20, "Φ ");
//        prepareScreen();
//        lcd->drawStr(2, 15, "Cons.: ");
        //        lcd->drawUTF8(190, 45, "℃");

        displayFloat(getConsumedFuel(), displayChar_3);
        lcd->drawStr(20, 20, displayChar_3);
        lcd->drawStr(58, 20, "L");

        displayFloat(((eep->getData().dist_trv + car->getDst()) / getConsumedFuel()), displayChar_3);
        lcd->drawStr(20, 30, displayChar_3);
        lcd->drawStr(58, 30, "L/100km");
    }


    void displayInsideTemperature() {
        displayFloat(car->getTmpIns(), displayChar_3);
        lcd->drawStr(56, 45, displayChar_3);
        displayFloat(car->getTmpOut(), displayChar_3);
        lcd->drawStr(20, 45, displayChar_3);
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
            displayInsideTemperature();
            displayTotalConsumption();

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
            lcd->drawStr(0, 0, "Sub Travel");
            break;
        case 22:
            lcd->drawStr(0, 0, "Sub Travel");
            break;

            //
            // Fuel menu
        case 3:
            lcd->drawStr(0, 0, " Fuel menu");
            break;
        case 32:
            lcd->drawStr(0, 0, " Sub Fuel");
            break;
        case 4:
        case 41:
        case 42:
        case 43:
        case 44:
        case 45:
            lcd->drawStr(0, 0, "Servicing ");
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

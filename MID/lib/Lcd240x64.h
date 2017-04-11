//
// Created by Angel Zaprianov on 8.2.2017 г..
//

#ifndef ARDUINO_MID_LCD240X64_H
#define ARDUINO_MID_LCD240X64_H
//#include "CarGames.h"
#include <Arduino.h>
#include "../conf.h"
#include "MainFunc.h"
#include "ShutDw.h"
#include "CarState.h"
#include <U8g2lib.h>
#include "Menu240x64.h"
#include "graphics/240x64-logo.h"
#include "graphics/gLcd-icons.h"

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

    void useTextMode() {
        lcd->setFont(u8g2_font_6x10_tf);
        lcd->setFontRefHeightExtendedText();
        lcd->setFontDirection(0);
        lcd->setDrawColor(1);
        lcd->setFontPosTop();
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
        useTextMode();
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


/**
 * Displays consumed fuel
 */
    void displayTotalConsumption() {

        lcd->drawXBMP(0, 15, 18, 18, fuel_18x18_bits);
        displayFloat(getConsumedFuel(), displayChar_3);
        lcd->drawStr(20, 20, displayChar_3);
//        lcd->drawStr(60, 20, "L");
        lcd->drawXBMP(60, 22, 4, 8, mark_liter_4x8_bits);

        displayFloat(((eep->getData().dist_trv + car->getDst()) / getConsumedFuel()), displayChar_3);
        lcd->drawStr(20, 30, displayChar_3);
//        lcd->drawStr(58, 30, "L/100km");
        lcd->drawXBMP(60, 32, 8, 8, mark_liter_per_8x8_bits);
        lcd->drawXBMP(68, 32, 10, 8, mark_100_10x8_bits);
        lcd->drawXBMP(78, 32, 9, 8, mark_km_9x8_bits);
    }


    void displayWeatherTemperatures() {
        lcd->drawXBMP(0, 15, 18, 18, temp_18x18_bits);
        displayFloat(car->getTmpOut(), displayChar_3);
        lcd->drawStr(20, 45, displayChar_3);
        lcd->drawXBMP(52, 47, 4, 8, mark_cels_4x8_bits);

        displayFloat(car->getTmpIns(), displayChar_3);
        lcd->drawStr(60, 45, displayChar_3);
        lcd->drawXBMP(92, 47, 4, 8, mark_cels_4x8_bits);

    }

/**
 * Displays trip
 */
    void displayTrip() {
        SavedData saved = eep->getData();

        //// TODO use road_18x18_bits icon

        //
        // Travel distance
        lcd->drawStr(2, 15, "Dist:");
        displayFloat(car->getDst() + saved.dist_trp, displayChar_4);
        lcd->drawStr(70, 15, displayChar_4);
        lcd->drawStr(105, 15, "km");

        //
        // Travel distance
        lcd->drawStr(150, 15, "Trip:");
        displayFloat(eep->getWorkDistance(), displayChar_4);
        lcd->drawStr(205, 15, displayChar_4);
        lcd->drawStr(105, 15, "km");

        //
        // Travel time
        lcd->drawStr(2, 30, "Time:");
        lcd->drawStr(70, 30, car->getHTm(saved.time_trp));
        lcd->drawStr(105, 15, "h");

        //
        // Travel time
        lcd->drawStr(2, 30, "Time:");
        lcd->drawStr(70, 30, car->getHTm(saved.time_trp));
        lcd->drawStr(105, 15, "km");


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
            displayWeatherTemperatures();
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
//            lcd->drawStr(0, 0, "Trip");
            displayTrip();
            break;
            //
            // Fuel menu
        case 3:
            lcd->drawStr(0, 0, "Fuel menu");
            break;
        case 4:
            lcd->drawStr(0, 0, "Car State ");
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

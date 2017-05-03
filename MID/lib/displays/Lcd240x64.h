//
// Created by Angel Zaprianov on 8.2.2017 г..
//

#ifndef ARDUINO_MID_LCD240X64_H
#define ARDUINO_MID_LCD240X64_H
//#include "CarGames.h"
#include <Arduino.h>
#include <U8g2lib.h>
#include "../../conf.h"
#include "../MainFunc.h"
#include "../ShutDw.h"
#include "../CarState.h"

#include "Menu240x64.h"
#include "../graphics/240x64-logo.h"
#include "../graphics/gLcd-icons.h"

#ifndef _U8G2LIB_HH
// Some IDE syntax mishmash fixer
#include "../../libraries/U8g2/src/U8g2lib.h"

#endif

#define LCD_ROW_1 14
#define LCD_ROW_2 24
#define LCD_ROW_3 36
#define LCD_ROW_4 46


class Lcd240x62 : virtual public LcdMenuInterface {

    String interfaceBuffer;
    U8G2 *lcd;
    AmpTime *amp;
    CarSens *car;
    EepRom *eep;
    CarState *stt;
    WhlSens *whl;
    MenuBase *mbs;
    ShutDw *sdw;
    MenuBtn *btn;
//
// Drowing counter
    uint8_t drawIndex = 0;
    char drawEntry = 0;

public:
/**
 *
 * @param _lcd
 * @param _btn
 * @param _mbs
 * @param _sdw
 */
    Lcd240x62(U8G2 &_lcd, MenuBtn &_btn, MenuBase &_mbs, ShutDw &_sdw) :
            lcd(&_lcd), btn(&_btn), mbs(&_mbs), amp(_btn.passAmp()), car(_btn.passCar()), eep(_btn.passEep()),
            whl(_btn.passWhl()), stt(_btn.passStt()), sdw(&_sdw) {}

/**
 * Mid's intro
 */
    void intro(void) {
        //
        // Test tone
        tone(TONE_ADT_PIN, 400, 20);
        delay(10);
        lcd->clearBuffer();
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
//        lcd->enableUTF8Print();
//        lcd->setAutoPageClear(1);
    }
// TODO..
    /****************************************************************

/**
 * Draws shutdown begin for trip save
 */
    void drawShutdownBegin() {

    }

/**
 * Draws shutdown begin for trip save
 */
    void drawShutdownShort() {

    }

/**
 * Draws countdown time for saving trip
 */
    void drawShutdownCount(char sec[2]) {

    }

/**
 *
 */
    void draWShutdownTripSave() {

    }

/**
 *
 */
    void draWShutdownTripSkip() {

    }

/**
 * Draw graphic
 */
    void draw() {
        if (amp->isMax()) {
            lcd->firstPage();
            do {
                menus();
            } while (lcd->nextPage());
            drawIndex++;
            if (drawIndex > 5) {
                drawIndex = 0;
            }
        }
    }

protected:

    //
    // Defining content generate container variables
//    char displayChar_2[3];
//    char displayChar_3[4];
//    char displayChar_4[5];

    void menus();

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
    void displayEntryO() {


        switch (drawEntry) {
            default:
                drawEntry = 0;
                break;

            case 0:
                lcd->clear();
                mbs->startEntry();
                drawIndex = 0;
                btn->setNavigationState(false);
                tone(TONE_ADT_PIN, 2800, 16);
                break;
            case 1:
            case 2:
            case 3:
                lcd->drawStr(12, 5, usedMenu.back);
                lcd->drawStr(12, 35, usedMenu.used);
                lcd->drawStr(72, 35, usedMenu.down);
                lcd->drawStr(12, 20, usedMenu.next);
                lcd->drawStr(12, 50, usedMenu.last);
                lcd->drawFrame(10 + (drawIndex * 5), 18, 212, 15);
                break;
            case 4:
                lcd->clearBuffer();
                lcd->clear();
                break;
            case 5:

                btn->setNavigationState(true);
                mbs->finishEntry();
                lcd->clear();
                drawEntry = -1;
                break;
        }
        drawEntry++;
    }

    void displayEntry() {


        switch (drawIndex) {
            default:
                drawIndex = 0;
                break;

            case 0:
                lcd->clear();
                lcd->clearBuffer();
                mbs->startEntry();
                drawIndex = 0;
                btn->setNavigationState(false);
                tone(TONE_ADT_PIN, 2800, 16);
                //
                // Generate menu fo U8g2
                interfaceBuffer = String(usedMenu.back) + '\n' + usedMenu.used + '\n' + usedMenu.used;
                break;
            case 1:
            case 2:
            case 3:
                lcd->userInterfaceSelectionList("Entry", drawIndex, interfaceBuffer.c_str());
                break;
            case 4:
                interfaceBuffer = "";
                lcd->clearBuffer();
                lcd->clear();
                break;
            case 5:

                btn->setNavigationState(true);
                mbs->finishEntry();
                lcd->clear();
                break;
        }
    }

private:
/**
 * Draws km as string
 * @param x
 * @param y
 */
    inline void drawKm(u8g2_uint_t x, u8g2_uint_t y) {
        lcd->drawXBMP(x, y, 9, 8, mark_km_9x8_bits);
    }

/**
 * Draws average char
 * @param x
 * @param y
 */
    inline void drawAverage(u8g2_uint_t x, u8g2_uint_t y) {
        lcd->drawXBMP(x, y, 8, 8, mark_phi_8x8_bits);
    }

/**
 * Draws  L/100km
 * @param x
 * @param y
 */
    inline void drawL100km(u8g2_uint_t x, u8g2_uint_t y) {
        lcd->drawXBMP(/*50*/x, y, 8, 8, mark_liter_per_8x8_bits);
        lcd->drawXBMP(/*58*/x + 8, y, 10, 8, mark_100_10x8_bits);
        lcd->drawXBMP(/*69*/x + 19, y, 9, 8, mark_km_9x8_bits);
    }


    unsigned int aniIndex;


/**
 * Displays consumed fuel
 */
    void displayHomeConsumption() {

        lcd->drawXBMP(4, 15, 18, 18, fuel_18x18_bits);
        displayFloat(eep->getConsumedFuel(), displayChar_3);
        lcd->drawStr(25, LCD_ROW_1, displayChar_3);
        lcd->drawXBMP(50, LCD_ROW_1, 4, 8, mark_liter_4x8_bits);

        displayFloat(eep->getAverageLitersPer100km(), displayChar_3);
        lcd->drawStr(25, LCD_ROW_2, displayChar_3);
        drawL100km(50, LCD_ROW_2);

//        lcd->drawXBMP(50, LCD_ROW_2, 8, 8, mark_liter_per_8x8_bits);
//        lcd->drawXBMP(58, LCD_ROW_2, 10, 8, mark_100_10x8_bits);
//        lcd->drawXBMP(69, LCD_ROW_2, 9, 8, mark_km_9x8_bits);
    }

/**
 * Display temperatures
 */
    void displayHomeTemperatures() {

        //
        // Outside graph
        lcd->drawXBMP(0, LCD_ROW_3, 18, 18, temp_18x18_bits);
        lcd->drawXBMP(20, LCD_ROW_3, 18, 10, car_out_18x10_bits);
        lcd->drawXBMP(70, LCD_ROW_3, 4, 8, mark_cel_4x8_bits);
        //
        //
        displayFloat(car->getTmpOut(), displayChar_3);
        lcd->drawStr(45, LCD_ROW_3, displayChar_3);

        //
        // Inside Graph
        lcd->drawXBMP(20, LCD_ROW_4, 18, 10, car_ins_18x10_bits);
        lcd->drawXBMP(70, LCD_ROW_4, 4, 8, mark_cel_4x8_bits);
        //
        // Data
        displayFloat(car->getTmpIns(), displayChar_3);
        lcd->drawStr(45, LCD_ROW_4, displayChar_3);
    }

/**
 * Displays trip
 */
    void displayCurrentTrip() {
        SavedData saved = eep->getData();
        lcd->drawXBMP(120, 15, 18, 18, road_18x18_bits);
        lcd->drawXBMP(120, LCD_ROW_3, 18, 18, gage_18x18_bits);
        //
        // Travel distance
        lcd->drawStr(/*25*/ 155, LCD_ROW_1, "Range:");
        displayFloat(car->getDst() + saved.dist_trp, displayChar_4);
        lcd->drawStr(/*50*/195, LCD_ROW_1, displayChar_4);
        drawKm(/*82*/230, 15);
        //
        // Travel time
        lcd->drawStr(155, LCD_ROW_2, "Time:");
        // TODO Check for FIX !
        lcd->drawStr(195, LCD_ROW_2, car->getHTm(saved.time_trp));
        lcd->drawStr(230, LCD_ROW_2, "h");
        //
        // Average speed
        drawAverage(155, LCD_ROW_3);
        sprintf(displayChar_2, "%02d", car->getAvrVss());
        lcd->drawStr(175, LCD_ROW_3, displayChar_2);
        drawKm(205, LCD_ROW_3);
        //
        // Average liters per 100km
        drawAverage(155, LCD_ROW_4);
        displayFloat(car->getDst() / car->getCurFuelCns(), displayChar_3);
        lcd->drawStr(175, LCD_ROW_4, displayChar_3);
        drawL100km(205, LCD_ROW_4);
        // Todo wasted fuel
    }


};

/**
 *
 * @param index
 */
void Lcd240x62::menus() {
    switch (MidCursorMenu) {
        default:
        case MENU_ENTRY:
            displayEntryO();
//            displayEntry();
            break;
            //
            // Main / first menu
        case 1:
            lcd->drawStr(0, 0, "HOME MENU");
            displayHomeTemperatures();
            displayHomeConsumption();
            displayCurrentTrip();
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
            lcd->drawStr(0, 0, "CURRENT TRIP");

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

//
// Created by Angel Zaprianov on 8.2.2017 г..
//

#ifndef ARDUINO_MID_LCD240X64_H
#define ARDUINO_MID_LCD240X64_H
//#include "CarGames.h"
#include <Arduino.h>
#include <U8g2lib.h>
#include "Menu240x64.h"
#include "../../MID.h"
#include "../MainFunc.h"
#include "../ShutDw.h"
#include "../CarState.h"
#include "graphics/240x64-logo.h"
#include "graphics/gLcd-icons.h"


#ifndef _U8G2LIB_HH
// Some IDE syntax mishmash fixer
#include "../../libraries/U8g2/src/U8g2lib.h"

#endif

#define LCD_ROW_1 18
#define LCD_ROW_2 28
#define LCD_ROW_3 40
#define LCD_ROW_4 50

#define LCD_COL_L11 5
#define LCD_COL_L12 25
#define LCD_COL_L21 35
#define LCD_COL_L22 45
#define LCD_COL_L23 70

#define LCD_CNR 120 // Center of the screen
#define LCD_CNR_1 125 // Center of the screen
#define LCD_CNR_2 145 // Center of the screen

#define LCD_COL_R11 155
#define LCD_COL_R12 175
#define LCD_COL_R21 195
#define LCD_COL_R22 210
#define LCD_COL_R23 230

class Lcd240x62 : virtual public LcdUiInterface {

    U8G2 *lcd;

    //
    // Define base objects
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
    uint8_t drawEntry = 0;

    boolean animateFast = false;
    boolean initializeDraw = false;
    boolean resetingFuelAndDistanceMenu = false;

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
            whl(_btn.passWhl()), stt(_btn.passStt()), sdw(&_sdw) { }

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
        delay(5000);
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
        lcd->drawStr(15, LCD_ROW_1, this->getMsg(0));
        lcd->drawStr(15, LCD_ROW_2, this->getMsg(1));
    }

/**
 * Draws countdown time for saving trip
 */
    void drawShutdownCount(char sec[2]) {

    }

    void showHeader(const char *title) {
        lcd->drawStr(12, 2, title);
        lcd->drawLine(0, 12, 240, 11);
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

        if (!animateFast) {
            if (amp->isMax()) {
                lcd->firstPage();
                do {
                    menus();
                } while (lcd->nextPage());
                drawIndex++;

                if (drawIndex > 5) {
                    drawIndex = 0;
                    initializeDraw = false;
                }


            }
        } else {
            if (amp->isBig()) {
                lcd->firstPage();
                do {
                    menus();
                } while (lcd->nextPage());
                drawIndex++;
                if (MidCursorMenu == MENU_ENTRY) {
                    drawEntry++;
                } else {
                    drawEntry = 0;
                }
            }
        }


    }

protected:


//
// Defining content generate container variables
//    char char_2[3];
//    char char_3[4];
//    char char_4[5];

    void menus();

    void aniHrzChar(u8g2_uint_t x, u8g2_uint_t y, const char *str) {
        lcd->drawUTF8(aniIndex * 3, 36, str);
    }

    void useTextMode() {
        // Cyrillic font u8g2_font_crox1c_tf
        // u8g2_font_crox1cb_tf
        //u8g2_font_mercutio_basic_nbp_t_all
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
    void displayEntry() {
        u8g2_uint_t backW = lcd->getStrWidth(usedMenu.back);
        u8g2_uint_t usedW = lcd->getStrWidth(usedMenu.back);
        u8g2_uint_t nextW = lcd->getStrWidth(usedMenu.back);


        switch (drawEntry) {
            default:
                drawEntry = 0;
                break;
            case 0:
                animateFast = true;
                lcd->clear();
                mbs->startEntry();
                btn->setNavigationState(false);
                tone(TONE_ADT_PIN, 2800, 16);
                break;
            case 1:
            case 2:
            case 3:
                lcd->drawStr(108, 1, this->getMsg(2));
                lcd->drawLine(0, 12, lcd->getDisplayWidth(), 12);
                lcd->drawFrame(10, 12 + (drawEntry * 5), 212, 15);
                lcd->drawStr(120 - (backW / 2), 15, usedMenu.back);
                lcd->drawStr(120 - (usedW / 2), 30, usedMenu.used);
                lcd->drawStr(120 - (nextW / 2), 45, usedMenu.next);
                break;
            case 4:
                lcd->drawStr(120 + (usedW / 2), 45, " > ");
                lcd->drawStr((120 + (usedW / 2)) + 30, 45, usedMenu.down);
                break;
            case 5:
                btn->setNavigationState(true);
                mbs->finishEntry();
                lcd->clearBuffer();
                lcd->clear();
                drawEntry = 0;
                initializeDraw = true;
                animateFast = false;
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
        displayFloat(eep->getConsumedFuel(), char_3);
        lcd->drawStr(25, LCD_ROW_1, char_3);
        lcd->drawXBMP(50, LCD_ROW_1, 4, 8, mark_liter_4x8_bits);

        displayFloat(eep->getAverageLitersPer100km(), char_3);
        lcd->drawStr(25, LCD_ROW_2, char_3);
        drawL100km(50, LCD_ROW_2);

//        lcd->drawXBMP(50, LCD_ROW_2, 8, 8, mark_liter_per_8x8_bits);
//        lcd->drawXBMP(58, LCD_ROW_2, 10, 8, mark_100_10x8_bits);
//        lcd->drawXBMP(69, LCD_ROW_2, 9, 8, mark_km_9x8_bits);
    }

/**
 * Display temperatures
 */
    void displayHomeTemperatures() {

//        if (initializeDraw) {
        //
        // Outside graph
        lcd->drawXBMP(0, LCD_ROW_3, 18, 18, temp_18x18_bits);
        lcd->drawXBMP(20, LCD_ROW_3, 18, 10, car_out_18x10_bits);
        lcd->drawXBMP(70, LCD_ROW_3, 4, 8, mark_cel_4x8_bits);
        //
        // Inside Graph
        lcd->drawXBMP(20, LCD_ROW_4, 18, 10, car_ins_18x10_bits);
        lcd->drawXBMP(70, LCD_ROW_4, 4, 8, mark_cel_4x8_bits);
//        }
        //
        //
        displayFloat(car->getTmpOut(), char_3);
        lcd->drawStr(45, LCD_ROW_3, char_3);


        //
        // Data
        displayFloat(car->getTmpIns(), char_3);
        lcd->drawStr(45, LCD_ROW_4, char_3);
    }

/**
 * Displays trip
 */
    void displayCurrentTrip() {
        SavedData saved = eep->getData();
        lcd->drawXBMP(120, 15, 18, 18, road_18x18_bits);
        lcd->drawXBMP(120, LCD_ROW_3, 18, 18, grap_18x18_bits);
        //
        // Travel distance
        lcd->drawXBMP(LCD_COL_R11, LCD_ROW_1, 5, 8, car_dist_5x8_bits);
        displayFloat(car->getDst() + saved.dist_trp, char_4);
        lcd->drawStr(LCD_COL_R12, LCD_ROW_1, char_4);
        drawKm(LCD_COL_R22, 15);
        //
        // Travel time
        lcd->drawXBMP(LCD_COL_R11, LCD_ROW_2, 5, 8, car_time_5x8_bits);
        lcd->drawStr(LCD_COL_R12, LCD_ROW_2, car->getHTm(saved.time_trp));
        lcd->drawStr(LCD_COL_R22, LCD_ROW_2, "h");
        //
        // Average speed
        drawAverage(155, LCD_ROW_3);
        sprintf(char_2, "%02d", car->getAvrVss());
        lcd->drawStr(LCD_COL_R12, LCD_ROW_3, char_2);
        drawKm(LCD_COL_R22, LCD_ROW_3);
        //
        // Average liters per 100km
        drawAverage(LCD_COL_R11, LCD_ROW_4);
        displayFloat(car->getDst() / car->getCurFuelCns(), char_3);
        lcd->drawStr(LCD_COL_R12, LCD_ROW_4, char_3);
        drawL100km(LCD_COL_R22, LCD_ROW_4);
        // Todo wasted fuel
    }

    /****************************************************************
 * Display Dashboard
 */

    void displayCarVss() {
        lcd->drawStr(5, LCD_ROW_1, "KMH:");
        sprintf(char_3, "%03d", car->getVss());
        lcd->drawStr(35, LCD_ROW_1, char_3);
    }

    void displayEngRpm() {
        lcd->drawStr(5, LCD_ROW_2, "RPM:");
        sprintf(char_4, "%04d", car->getRpm());
        lcd->drawStr(35, LCD_ROW_2, char_4);
    }

    void displayCarEcu() {
        lcd->drawStr(5, LCD_ROW_3, "ECU:");
        sprintf(char_2, "%02d", car->getEcu());
        lcd->drawStr(35, LCD_ROW_3, char_2);
    }

    void displayCarDsp() {
        /*
         const int n = snprintf(NULL, 0, "%lu", ulong_value);
        assert(n > 0);
        char buf[n+1];
        int c = snprintf(buf, n+1, "%lu", ulong_value);
        assert(buf[n] == '\0');
        assert(c == n);
         */
        lcd->drawStr(155, LCD_ROW_1, "VDS:");
        if (car->getVss() == 0) {
            char vds[20];
            sprintf(vds, "%lu", car->getVdsDump());
            lcd->drawStr(195, LCD_ROW_1, vds);
        } else {
            lcd->drawStr(195, LCD_ROW_1, "?");
        }
    }


    /****************************************************************
 * Reset Fuel
 */
    void displayResetFuel() {

        lcd->drawStr(5, LCD_ROW_1, getMsg(20));
        lcd->drawStr(5, LCD_ROW_2, getMsg(21));
        /*
        if (btn->getNavigationState()) {
            //
            // Info  how to enter in menu
            lcd->drawStr(5, LCD_ROW_2, getMsg(21));
        }

        if (btn->isMn()) {
            btn->setNavigationState(0);
        }

        if (!btn->getNavigationState()) {
            //
            // Ask for reset
            lcd->drawStr(5, LCD_ROW_2, getMsg(22));

            const char *msg = getMsg(23);
            u8g2_uint_t askW = lcd->getStrWidth(msg);

            lcd->drawStr(120 - askW, LCD_ROW_3, msg);

            //
            // Listen the buttons
            if (btn->isOk()) {
                eep->saveResetData();
                btn->setNavigationState(1);
                tone(TONE_ADT_PIN, 2500, 100);
                resetingFuelAndDistanceMenu = false;
                mbs->triggerMoveUp();
            }

            if (btn->isNo()) {
                btn->setNavigationState(1);
                tone(TONE_ADT_PIN, 800, 100);
                resetingFuelAndDistanceMenu = false;
                mbs->triggerMoveUp();
            }
        }
*/
    }

/****************************************************************
* Car Servicing
*/
    void displayCarState() {


        lcd->drawStr(LCD_COL_L12, LCD_ROW_1, getMsg(3));
        if (stt->getLiveBrk()) lcd->drawStr(LCD_COL_R12, LCD_ROW_1, getMsg(7));
        else lcd->drawStr(LCD_COL_R12, LCD_ROW_1, getMsg(8));

        lcd->drawStr(LCD_COL_L12, LCD_ROW_2, getMsg(4));
        if (stt->getLiveCnt()) lcd->drawStr(LCD_COL_R12, LCD_ROW_2, getMsg(7));
        else lcd->drawStr(LCD_COL_R12, LCD_ROW_2, getMsg(8));

        lcd->drawStr(LCD_COL_L12, LCD_ROW_3, getMsg(5));
        if (stt->getLiveWin()) lcd->drawStr(LCD_COL_R12, LCD_ROW_3, getMsg(8));
        else lcd->drawStr(LCD_COL_R12, LCD_ROW_3, getMsg(7));

        lcd->drawStr(LCD_COL_L12, LCD_ROW_4, getMsg(6));
        if (stt->getLiveOil()) lcd->drawStr(LCD_COL_R12, LCD_ROW_4, getMsg(8));
        else lcd->drawStr(LCD_COL_R12, LCD_ROW_4, getMsg(7));


        if (MidCursorMenu == 46) {
            lcd->print(F("Range: "));
            lcd->print(eep->getWorkDistance());
            lcd->write((uint8_t) 2);
            lcd->print(" ");
        }
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
            displayEntry();
            break;
            //
            // Main / first menu
        case 1:
            showHeader(getMsg(11));
            displayHomeTemperatures();
            displayHomeConsumption();
            displayCurrentTrip();
            break;
            //
            // Dashboard
        case 11:
            displayCarVss();
            displayEngRpm();
            displayCarEcu();
            displayCarDsp();
            break;
        case 12:
            showHeader(getMsg(15));
            displayResetFuel();
            break;
            //
            // Travel menu
        case 2:
            showHeader(getMsg(12));

            break;
            //
            // Fuel menu
        case 3:
            showHeader(getMsg(13));
            break;
        case 4:
            showHeader(getMsg(14));
            displayCarState();
            break;


        case ShutDw::MENU_SHUTDOWN:
            sdw->menu(this);
            break;
    }

}


#endif //ARDUINOMID_LCD240X64_H

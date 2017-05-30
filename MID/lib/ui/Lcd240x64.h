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
            lcd->drawXBMP(0, 2, 240, 60, BertoneBits);
        } while (lcd->nextPage());
        delay(5100);
        lcd->clear();


        tone(TONE_ADT_PIN, 800, 10);
    }

    void begin(void) {
        lcd->begin();
        useTextMode();
//        lcd->enableUTF8Print();
//        lcd->setAutoPageClear(1);
    }

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
        if (stt->isAlert() && drawIndex % 5 == 0) {
            lcd->drawXBMP(230, 3, 10, 10, wrench_10x10_bits);
            lcd->drawCircle(225, 1, 6, U8G2_DRAW_ALL);
        }
        if (car->getEngTmp() > 99 && drawIndex % 2 == 0) {
            lcd->drawXBMP(215, 3, 10, 10, fire_10x10_bits);
            lcd->drawCircle(215, 1, 6, U8G2_DRAW_ALL);
        }

        u8g2_uint_t width = lcd->getStrWidth(title);
        lcd->drawStr((LCD_CNR / 2) - (width / 2), 2, title);
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

        //
        // Slow Animation
        if (!animateFast) {
            if (amp->isMax()) {
                lcd->firstPage();
                do {
                    menus();
                } while (lcd->nextPage());
                drawIndex++;

                if (drawIndex > 10) {
                    drawIndex = 0;
                    initializeDraw = false;
                }


            }
            //
            // Fast animation
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
        uint8_t subAnimateIndex = drawEntry & 4;

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
            case 5:
            case 6:
            case 7:

                lcd->drawFrame(10, 12 + (3 * 5), 212, 15);
                lcd->drawStr(120 - (backW / 2), 15, usedMenu.back);
                lcd->drawStr(120 - (usedW / 2), 30, usedMenu.used);
                lcd->drawStr(120 - (nextW / 2), 45, usedMenu.next);
                if (usedMenu.down) {
                    lcd->drawStr(120 + (usedW / 2) + subAnimateIndex, 30, ">");
                    lcd->drawStr((120 + (usedW / 2)) + 40, 30, usedMenu.down);
                }
                break;
            case 8:
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
    inline void showKm(u8g2_uint_t x, u8g2_uint_t y) {
        lcd->drawXBMP(x, y + 1, 9, 8, mark_km_9x8_bits);
    }

/**
 * Draws average char
 * @param x
 * @param y
 */
    inline void showAverage(u8g2_uint_t x, u8g2_uint_t y) {
        lcd->drawXBMP(x, y, 8, 8, mark_phi_8x8_bits);
    }

/**
 * Draws  L/100km
 * @param x
 * @param y
 */
    inline void showL100km(u8g2_uint_t x, u8g2_uint_t y) {
        lcd->drawXBMP(/*50*/x, y, 8, 8, mark_liter_per_8x8_bits);
        lcd->drawXBMP(/*58*/x + 8, y, 10, 8, mark_100_10x8_bits);
        lcd->drawXBMP(/*69*/x + 19, y, 9, 8, mark_km_9x8_bits);
    }

/**
 * Draws  celsius symbol
 * @param x
 * @param y
 */
    inline void showCels(u8g2_uint_t x, u8g2_uint_t y) {
        lcd->drawXBMP(x, y, 4, 8, mark_cel_4x8_bits);
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
        showL100km(50, LCD_ROW_2);
    }

/**
 * Display temperatures
 */
    void displayHomeTemperatures() {

        //
        // Outside graph
        lcd->drawXBMP(0, LCD_ROW_3, 18, 18, temp_18x18_bits);
        lcd->drawXBMP(20, LCD_ROW_3, 18, 10, car_out_18x10_bits);
        this->showCels(LCD_COL_L23, LCD_ROW_3);
        //
        // Inside Graph
        lcd->drawXBMP(20, LCD_ROW_4, 18, 10, car_ins_18x10_bits);


        //
        //
        displayFloat(car->getTmpOut(), char_3);
        lcd->drawStr(45, LCD_ROW_3, char_3);

        if (car->getTmpIns() < -99) {
            lcd->drawStr(45, LCD_ROW_4, "none");
        } else {
            //
            // Data
            displayFloat(car->getTmpIns(), char_3);
            lcd->drawStr(45, LCD_ROW_4, char_3);
            this->showCels(LCD_COL_L23, LCD_ROW_4);
        }
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
        showKm(LCD_COL_R22, 19);
        //
        // Travel time
        lcd->drawXBMP(LCD_COL_R11, LCD_ROW_2, 5, 8, car_time_5x8_bits);
        lcd->drawStr(LCD_COL_R12, LCD_ROW_2, car->getHTm(saved.time_trp));
        lcd->drawStr(LCD_COL_R22, LCD_ROW_2, "h");
        //
        // Average speed
        showAverage(155, LCD_ROW_3);
        sprintf(char_2, "%02d", car->getAvrVss());
        lcd->drawStr(LCD_COL_R12, LCD_ROW_3, char_2);
        showKm(LCD_COL_R22, LCD_ROW_3);
        //
        // Average liters per 100km
        showAverage(LCD_COL_R11, LCD_ROW_4);
        displayFloat((car->getCurFuelCns() * 100) / car->getDst(), char_3); // ( litres X 100) / km distance
        lcd->drawStr(LCD_COL_R12, LCD_ROW_4, char_3);
        showL100km(LCD_COL_R22, LCD_ROW_4);
        // Todo wasted fuel
    }

    /****************************************************************
 * Display Dashboard
 */

    void displayCarVss() {
        lcd->drawStr(LCD_COL_L11, LCD_ROW_1, "KMH:");
        sprintf(char_3, "%03d", car->getVss());
        lcd->drawStr(LCD_COL_L21, LCD_ROW_1, char_3);
    }

    void displayEngRpm() {
        lcd->drawStr(LCD_COL_L11, LCD_ROW_2, "RPM:");
        sprintf(char_4, "%04d", car->getRpm());
        lcd->drawStr(LCD_COL_L21, LCD_ROW_2, char_4);
    }

    void displayCarEcu() {
        lcd->drawStr(LCD_COL_L11, LCD_ROW_3, "ECU:");
        sprintf(char_2, "%02d", car->getEcu());
        lcd->drawStr(LCD_COL_L21, LCD_ROW_3, char_2);
    }

    void displayCarDst() {
        lcd->drawStr(LCD_COL_L11, LCD_ROW_4, "DST:");
        displayFloat(car->getDst(), char_3);
        lcd->drawStr(LCD_COL_L21, LCD_ROW_4, char_3);
        showKm(LCD_COL_L23, LCD_ROW_4);
    }

    void displayCarOdm() {
        lcd->drawStr(LCD_COL_R11, LCD_ROW_1, "ODO:");
        if (car->getVss() == 0) {
            char vds[20];
            sprintf(vds, "%lu", car->getVdsDump());
            lcd->drawStr(LCD_COL_R21, LCD_ROW_1, vds);
        } else {
            lcd->drawStr(LCD_COL_R21, LCD_ROW_1, "?");
        }
    }

    void displayCarEnt() {
        lcd->drawStr(LCD_COL_R11, LCD_ROW_2, "ENG:");
        sprintf(char_3, "%03d", car->getEngTmp());
        lcd->drawStr(LCD_COL_R21, LCD_ROW_2, char_3);
        this->showCels(LCD_COL_R23, LCD_ROW_2);
    }

    void displayCarGrs() {
        lcd->drawStr(LCD_COL_R11, LCD_ROW_3, "GRS:");
        sprintf(char_2, "%02d", car->getGear());
        lcd->drawStr(LCD_COL_R21, LCD_ROW_3, char_2);
    }

    void displayCarWrk() {
        lcd->drawStr(LCD_COL_R11, LCD_ROW_4, "WRK:");
        sprintf(char_6, "%06d", eep->getWorkDistance());
        lcd->drawStr(LCD_COL_R21, LCD_ROW_4, char_6);
        showKm(LCD_COL_R23, LCD_ROW_4);
    }


    /****************************************************************
 * About Fuel
 */
    void displayInfo() {
        lcd->drawStr(LCD_COL_L11, LCD_ROW_1, getMsg(20));
        lcd->drawStr(LCD_COL_L11, LCD_ROW_2, getMsg(21));
        lcd->drawStr(LCD_COL_L11, LCD_ROW_3, getMsg(22));
        lcd->drawStr(LCD_COL_L11, LCD_ROW_4, getMsg(23));
        lcd->drawStr(LCD_COL_R22, LCD_ROW_4, MID_VERSION);
    }

    void displayEdit() {
        lcd->drawStr(LCD_COL_L11, LCD_ROW_1, getMsg(24));
        lcd->drawStr(0, LCD_ROW_3, getMsg(25));
        lcd->drawStr(0, LCD_ROW_4, getMsg(26));
    }

/****************************************************************
* Car Servicing
*/
    void displayCarState() {


        lcd->drawStr(LCD_COL_L12, LCD_ROW_1, getMsg(3));
        if (stt->getLiveBrk()) lcd->drawStr(LCD_COL_R12, LCD_ROW_1, getMsg(8));
        else lcd->drawStr(LCD_COL_R12, LCD_ROW_1, getMsg(7));

        lcd->drawStr(LCD_COL_L12, LCD_ROW_2, getMsg(4));
        if (stt->getLiveCnt()) lcd->drawStr(LCD_COL_R12, LCD_ROW_2, getMsg(8));
        else lcd->drawStr(LCD_COL_R12, LCD_ROW_2, getMsg(7));

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

    void displayTrip() {

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
            showHeader(getMsg(16));
            displayCarVss();
            displayEngRpm();
            displayCarEcu();
            displayCarDst();
            displayCarOdm();
            displayCarEnt();
            displayCarGrs();
            displayCarWrk();
            break;
        case 12:
            showHeader(getMsg(17));
            displayEdit();
            break;
        case 121:
        case 122:
        case 123:
        case 124:
            break;
        case 13:
            showHeader(getMsg(15));
            displayInfo();
            break;
            //
            // Travel menu
        case 2:
            showHeader(getMsg(12));
            displayTrip();
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

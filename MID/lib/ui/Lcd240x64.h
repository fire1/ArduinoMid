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
#define LCD_COL_L22 50
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


    const uint8_t *fontSelect;
    uint8_t lastValue;
//
// Drowing counter
    uint8_t drawIndex = 0;
    uint8_t drawEntry = 0;

    boolean animateFast = false;
    boolean animateUltra = false;
    boolean initializeDraw = false;

    //
    // from 14 to 64
    uint8_t graphTest[10] = {54, 20, 48, 14, 64, 46, 18, 35, 15, 48};

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
            whl(_btn.passWhl()), stt(_btn.passStt()), sdw(&_sdw) {


    }

/**
 * Mid's intro
 */
    void intro(void) {
        //
        // Test tone
        tone(TONE_ADT_PIN, 400, 20);
        if (/*car->getRpm() < 699*/ false) {
            delay(10);
            lcd->clearBuffer();
            lcd->firstPage();
            do {
                lcd->drawXBMP(0, 2, 240, 60, OpelLogoBits);
            } while (lcd->nextPage());
            delay(2550);
            lcd->clearBuffer();
            lcd->firstPage();
            do {
                lcd->drawXBMP(0, 2, 240, 60, BertoneBits);
            } while (lcd->nextPage());
            delay(2550);
        }
        lcd->clear();


        tone(TONE_ADT_PIN, 800, 10);
    }

    void begin(void) {
        lcd->begin();
        useDefaultMode();
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
        lcd->drawLine(0, 12, 240, 11);

        if (stt->isAlert() /*&& drawIndex % 3 == 0*/ || millis() < 7000 && drawIndex % 2 == 0) {
            lcd->drawXBMP(135, 1, 10, 10, wrench_10x10_bits);
            lcd->drawCircle(139, 6, 6, U8G2_DRAW_ALL);
        }
        if (car->getEngTmp() > 99 && drawIndex % 3 == 0) {
            lcd->drawXBMP(155, 1, 10, 10, fire_10x10_bits);
            lcd->drawCircle(159, 6, 6, U8G2_DRAW_ALL);
        }
        lcd->drawStr(12, 2, title);

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

    void setFont(const uint8_t *font) {
        fontSelect = font;
    }

    void changeFont() {
        if (MidCursorMenu == 21) {
            lcd->setFont(u8g2_font_logisoso58_tr);
            lcd->setFontRefHeightExtendedText();
            lcd->setFontDirection(0);
            lcd->setDrawColor(1);
            lcd->setFontPosTop();
        } else {
            useDefaultMode();
        }
    }

    void handleDrawerEntry() {
        drawIndex++;
        if (MidCursorMenu == MENU_ENTRY) {
            drawEntry++;
        } else {
            drawEntry = 0;
        }
    }

    void handleDrawer() {
        drawIndex++;

        if (drawIndex > 11) {
            drawIndex = 0;
            initializeDraw = false;
        }
    }

    void makeDraw() {
        lcd->firstPage();
        do {
            menus();
        } while (lcd->nextPage());
    }


/**
 * Draw graphic
 */
    void draw() {
        changeFont();

        //
        // Ultra fast animation
        if (animateUltra) {
            if (amp->isMid()) {
                makeDraw();
                handleDrawerEntry();
            }
        }

        //
        // Fast animation
        if (animateFast) {
            if (amp->isBig()) {
                makeDraw();
                handleDrawer();

            }
        }

        //
        // Slow / Normal animation
        if (!animateFast && !animateUltra) {
            if (amp->isMax()) {
                makeDraw();
                handleDrawer();
            }
        }


    }

protected:
/**
 * NOT RECOMMENDED use
 * only for short time
 */
    inline void playUltra() {
        animateUltra = true;
        animateFast = false;
    }

    inline void playFast() {
        animateFast = true;
        animateUltra = false;
    }

    inline void playSlow() {
        animateFast = false;
        animateUltra = false;
    }
//
// Defining content generate container variables
//    char char_2[3];
//    char char_3[4];
//    char char_4[5];

    void menus();

    void aniHrzChar(u8g2_uint_t x, u8g2_uint_t y, const char *str) {
//        lcd->drawUTF8(aniIndex * 3, 36, str);
    }

    void useDefaultMode() {
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
                this->playUltra();
                lcd->clear();
                mbs->startEntry();
                //
                // Reset button handler
                btn->setNavigationState(false);
                btn->useDebounceNormal();
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
                btn->resetStates();
                mbs->finishEntry();
                lcd->clearBuffer();
                lcd->clear();
                drawEntry = 0;
                drawIndex = 0;
                initializeDraw = true;
                this->playSlow();
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
 * Draws instant char
 * @param x
 * @param y
 */
    inline void showInstant(u8g2_uint_t x, u8g2_uint_t y) {
        lcd->drawXBMP(x, y, 8, 8, mark_now_5x8_bits);
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

/**
 *
 */
    inline void showLiter(u8g2_uint_t x, u8g2_uint_t y) {
        lcd->drawXBMP(x, y, 4, 8, mark_liter_4x8_bits);
    }

/**
 * Displays consumed fuel
 */
    void displayHomeConsumption() {

        lcd->drawXBMP(4, LCD_ROW_1, 18, 18, fuel_18x18_bits);
        displayFloat(eep->getConsumedFuel(), char_3);
        lcd->drawStr(LCD_COL_L12, LCD_ROW_1, char_3);
        showLiter(LCD_COL_L22, LCD_ROW_1);

        displayFloat(eep->getAverageLitersPer100km(), char_3);
        lcd->drawStr(LCD_COL_L12, LCD_ROW_2, char_3);
        showL100km(LCD_COL_L22, LCD_ROW_2);
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
    void displayHomeTrip() {
        SavedData saved = eep->getData();
        lcd->drawXBMP(LCD_CNR, LCD_ROW_1, 18, 18, road_18x18_bits);
        lcd->drawXBMP(LCD_CNR, LCD_ROW_3, 18, 18, car_18x18_bits); // grap_18x18_bits
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
        showAverage(LCD_COL_R11, LCD_ROW_3);
        sprintf(char_2, "%02d", car->getAvrVss());
        lcd->drawStr(LCD_COL_R12, LCD_ROW_3, char_2);
        showKm(LCD_COL_R22, LCD_ROW_3);
        //
        // Instant cons per 100km
        showInstant(LCD_COL_R11, LCD_ROW_4);
        displayFloat(car->getIfcAvr(), char_3);
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
        sprintf(char_4, "%07d", car->getRpm());
        lcd->drawStr(LCD_COL_L21, LCD_ROW_2, char_4);
    }

    void displayCarEcu() {
        lcd->drawStr(LCD_COL_L11, LCD_ROW_3, "ECU:");
        sprintf(char_2, "%02d", car->getEcu());
        lcd->drawStr(LCD_COL_L21, LCD_ROW_3, char_2);
    }

    void displayCarDst() {
        lcd->drawStr(LCD_COL_L11, LCD_ROW_4, "DST:");
        displayFloat(eep->getTravelDistance(), char_3);
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
        lcd->drawStr(LCD_COL_R21 - 5, LCD_ROW_4, char_6);
        showKm(LCD_COL_R23, LCD_ROW_4);
    }


    /****************************************************************
 * About Fuel
 */
    void displayInfo() {
        lcd->drawStr(LCD_COL_L11, LCD_ROW_1, getMsg(20));
        lcd->drawStr(LCD_COL_L11, LCD_ROW_2, getMsg(21));

        if (drawIndex < 5) {
            lcd->drawStr(LCD_COL_L11, LCD_ROW_4, getMsg(22));
        } else {
            lcd->drawStr(LCD_COL_L11, LCD_ROW_4, getMsg(23));
            lcd->drawStr(LCD_COL_R22, LCD_ROW_4, MID_VERSION);
        }
    }

    void displayEdit() {
        lcd->enableUTF8Print();
        lcd->setCursor(LCD_COL_L11, LCD_ROW_2);
        if (drawIndex < 2)
            lcd->print(getMsg(24));
        else if (drawIndex > 2 && drawIndex < 6)
            lcd->print(getMsg(25));
        else if (drawIndex > 6 && drawIndex < 10)
            lcd->print(getMsg(26));

        lcd->drawStr(LCD_COL_L11, LCD_ROW_4, getMsg(32));
    }

/****************************************************************
* Car Servicing
*/
    void displayCarState() {

        Diagnostic history = stt->getResult();


        if (history.brk) {
            lcd->setCursor(LCD_COL_L11, LCD_ROW_1);
            lcd->print(F("*"));
        }
        lcd->drawStr(LCD_COL_L12, LCD_ROW_1, getMsg(3));
        if (stt->getLiveBrk()) lcd->drawStr(LCD_COL_R12, LCD_ROW_1, (drawIndex < 5) ? getMsg(9) : getMsg(8));
        else lcd->drawStr(LCD_COL_R12, LCD_ROW_1, getMsg(7));

        if (history.cnt) {
            lcd->setCursor(LCD_COL_L11, LCD_ROW_2);
            lcd->print(F("*"));
        }
        lcd->drawStr(LCD_COL_L12, LCD_ROW_2, getMsg(4));
        if (stt->getLiveCnt()) lcd->drawStr(LCD_COL_R12, LCD_ROW_2, getMsg(8));
        else lcd->drawStr(LCD_COL_R12, LCD_ROW_2, getMsg(7));


        if (history.win) {
            lcd->setCursor(LCD_COL_L11, LCD_ROW_3);
            lcd->print(F("*"));
        }
        lcd->drawStr(LCD_COL_L12, LCD_ROW_3, getMsg(5));
        if (stt->getLiveWin()) lcd->drawStr(LCD_COL_R12, LCD_ROW_3, getMsg(8));
        else lcd->drawStr(LCD_COL_R12, LCD_ROW_3, getMsg(7));


        if (history.oil) {
            lcd->setCursor(LCD_COL_L11, LCD_ROW_4);
            lcd->print(F("*"));
        }
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

/****************************************************************
 * Trip graphic
 */
    void displayTrip() {
        //
        // Trip max rpm
        lcd->drawXBMP(4, LCD_ROW_1 + 2, 18, 18, gage_max_18x18_bits);
        sprintf(char_4, "%04d", car->getMxmRpm());
        lcd->drawStr(LCD_COL_L12, LCD_ROW_1, char_4);
        lcd->setCursor(LCD_COL_L22, LCD_ROW_1);
        lcd->print(F("rpm"));
        //
        // Trip max vss
        displayFloat(car->getMxmVss(), char_3);
        lcd->drawStr(LCD_COL_L12, LCD_ROW_2, char_3);
        showKm(LCD_COL_L22, LCD_ROW_2);
//        lcd->drawXBMP(4, LCD_ROW_3, 18, 18, gage_18x18_bits);
        //
        // Breaking time
        lcd->drawXBMP(LCD_COL_L11, LCD_ROW_3, 18, 10, car_brk_18x10_bits);
        displayFloat(car->getBrakTime(), char_3);
        lcd->drawStr(LCD_COL_L21, LCD_ROW_3, char_3);
        lcd->setCursor(LCD_COL_L23, LCD_ROW_3);
        lcd->print(F("h"));
        //
        // Avr RPM time
        lcd->drawXBMP(LCD_COL_L11 + 1, LCD_ROW_4, 18, 10, eng_avr_18x10_bits);
        sprintf(char_4, "%04d", car->getAvrRpm());
        lcd->drawStr(LCD_COL_L21, LCD_ROW_4, char_4);

        //
        // Travel fuel
        lcd->drawXBMP(LCD_CNR, LCD_ROW_1, 18, 18, fuel_18x18_bits);
        lcd->drawXBMP(LCD_COL_R11 + 1, LCD_ROW_1, 5, 8, drop_5x8_bits);
        displayFloat(car->getCurFuelCns(), char_3);
        lcd->drawStr(LCD_COL_R12, LCD_ROW_1, char_3);
        showLiter(LCD_COL_R22, LCD_ROW_1);
        //
        // Travel fuel waste
        lcd->drawXBMP(LCD_COL_R11, LCD_ROW_2, 6, 8, trash_6x8_bits);
        displayFloat(car->getCurFuelWasted(), char_3);
        lcd->drawStr(LCD_COL_R12, LCD_ROW_2, char_3);
        showLiter(LCD_COL_R22, LCD_ROW_2);
        //
        // Consumed fuel per 100km
        showAverage(LCD_COL_R11, LCD_ROW_3);
        displayFloat((car->getCurFuelCns() * 100) / (car->getDst()), char_3);
        lcd->drawStr(LCD_COL_R12, LCD_ROW_3, char_3);
        showL100km(LCD_COL_R22, LCD_ROW_3);

    }
/****************************************************************
 * Draw graph
 */
    void displayGraph() {
        uint8_t arrSize = 10;
        uint8_t wdDsp = lcd->getWidth();
        uint8_t hgDsp = lcd->getHeight();

        if (initializeDraw) {
            playFast();
            graphLines(arrSize, wdDsp, drawIndex);
        } else {
            this->lastValue = 25;
            graphLines(arrSize, wdDsp, arrSize);
            playSlow();
        }
    }


    void graphLine(uint8_t arrSize, uint8_t wdDsp, uint8_t index) {
        uint8_t cur = /*value*/ graphTest[index];
        lcd->drawLine(((wdDsp / arrSize) * index), this->lastValue,
                      ((wdDsp / arrSize) * (index + 1)), cur);
        this->lastValue = cur;
    }


    void graphLines(uint8_t arrSize, uint8_t wdDsp, uint8_t toIndex) {
        this->lastValue = 25;
        for (uint8_t i = 0; i < toIndex; ++i) {
            graphLine(arrSize, wdDsp, i);
        }
    }


/****************************************************************
 * Fuel information
 */
    void displayFuel() {
/* TODO Show fuel information
 * */

    }


/****************************************************************
 * Settings editor
 */
    void displayEditor() {
        //
        // Max value 65535
        float defVal = 0, curVal = 0, oldVal = 0, result = 0;

        btn->setEditorState(true);

        switch (MidCursorMenu) {
            case 121:
                defVal = VSS_CORRECTION;
                oldVal = curVal = car->getCorVss();
                result = car->getVss();
                break;
            case 122:
                defVal = RPM_CORRECTION;
                oldVal = curVal = car->getCorRpm();
                result = car->getRpm();
                break;
            case 123:
                defVal = DST_CORRECTION;
                oldVal = curVal = car->getCorDst();
                result = (uint32_t) car->getDst();
                break;
            case 124:
                defVal = ECU_CORRECTION;
                oldVal = curVal = car->getCorEcu();
                result = car->getEcu();
                break;
            default:
                break;
        }
        if (drawIndex < 2 && initializeDraw) {
            //
            // Change values and speed of buttons
            btn->setValueControlled(curVal);
            //
            // Change speed of screen
            playUltra();
        }
        //
        // Getting back modified value
        curVal = btn->getValueControlled();

        //
        // Current value

//        dtostrf(curVal, 5, 2, char_6);
//        sprintf(char_6, "%06s", char_7);

        displayFloat(curVal, char_7);
        lcd->drawStr(LCD_COL_L12, LCD_ROW_1, getMsg(18));
        lcd->drawStr(LCD_COL_R11, LCD_ROW_1, char_7);

        if (!btn->getNavigationState() && drawIndex % 4 == 0) {
            lcd->drawStr(LCD_COL_R11 - (lcd->getStrWidth("[") + 5), LCD_ROW_1, "[");
            lcd->drawStr(LCD_COL_R11 + lcd->getStrWidth(char_7) + 5, LCD_ROW_1, "]");
        }



        /**
         * Set data to carSens
         */
        if (!btn->getNavigationState() && curVal != oldVal) {
            switch (MidCursorMenu) {
                case 121:
                    eep->setSensVss(curVal * 0.01);
                    break;
                case 122:
                    eep->setSensRpm(curVal);
                    break;
                case 123:
                    eep->setSensDst(curVal);
                    break;
                case 124:
                    eep->setSensEcu(curVal);
                    break;
            }
            car->setSave(eep->getData());
        }


        displayFloat(defVal, char_7);
        //
        // Default value
//        sprintf(char_7, "%0d.%02d", (uint16_t) defVal, (uint8_t) (defVal * 100) % 100);
        lcd->drawStr(LCD_COL_L12, LCD_ROW_3, getMsg(19));
        lcd->drawStr(LCD_COL_R11, LCD_ROW_3, char_7);

        //
        // Result value
        sprintf(char_7, "%07d", (uint16_t) result);
        lcd->drawStr(LCD_COL_L12, LCD_ROW_4, getMsg(31));
        lcd->drawStr(LCD_COL_R11, LCD_ROW_4, char_7);

    }

/****************************************************************
 * Rally gage
 */
    void displayRallyGages() {

//
//        sprintf(char_3, "%03d", car->getVss());
//        lcd->drawStr(64, 2, char_3);

        const uint8_t wdDsp = 180;
        const uint8_t hgDsp = 64;
        const uint16_t maxPwr = 5400;
        const uint8_t all_blocks = 26;


        uint16_t rpm = car->getRpm();
        uint8_t current = (uint8_t) map(rpm, 0, 7500, 0, all_blocks);
        if (current < 1) {
            current = 1;
        }

        uint8_t width = wdDsp / all_blocks - 1;
        uint8_t height = hgDsp / all_blocks;
        //
        // Power compensation
        uint8_t res = (uint8_t) map(maxPwr, 0, 7000, 0, all_blocks);

        //
        // Draw rpm
        for (uint8_t i = 1; i < current; i++) {

            uint8_t crh = height * i;
            if (res < i) {
                //
                // Stepping down the power
                crh = crh - (all_blocks / res) * ((i - res) * 3);
            }

            lcd->drawBox(60 + (wdDsp / all_blocks) * i, hgDsp - crh - res, width, crh);
        }




        // GEARS
        // W47 H63
        // https://github.com/olikraus/u8g2/wiki/fntgrpinconsolata
        uint8_t gear = car->getGear();
        lcd->setCursor(10, 3);

        if (gear > 6 || gear < 1) {
            lcd->print("N");
        } else {
            sprintf(char_2, "%01d", gear);
            lcd->print(char_2);
        }

        // Reset fonts


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
            displayHomeTrip();
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
        case 121: // VSS
        case 122: // RPM
        case 123: // DTS
        case 124: // CNS
            showHeader(getMsg(MidCursorMenu - 94));
            displayEditor();
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
        case 21:
            displayRallyGages();
            break;
            //
            // Fuel menu
        case 3:
            showHeader(getMsg(13));
            displayFuel();
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

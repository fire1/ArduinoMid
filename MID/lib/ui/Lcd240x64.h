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

#define LCD_ROW_1 16
#define LCD_ROW_2 27
#define LCD_ROW_3 39
#define LCD_ROW_4 50

#define LCD_COL_L10 0
#define LCD_COL_L11 8
#define LCD_COL_L12 30
#define LCD_COL_L21 35
#define LCD_COL_L22 65
#define LCD_COL_L23 85

#define LCD_CNR 120 // Center of the screen
#define LCD_CNR_1 125 // Center of the screen
#define LCD_CNR_2 145 // Center of the screen

#define LCD_COL_R11 154
#define LCD_COL_R12 175
#define LCD_COL_R21 195
#define LCD_COL_R22 215
#define LCD_COL_R23 230

#define LCD_PADDING  5

#define LCD_ICO_HIGH 2
#define LCD_ENTRY_FRAME 2

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


    boolean animateFast = false;
    boolean animateUltra = false;
    boolean initializeDraw = false;
    boolean changedFont = false;


    const uint8_t *fontSelect;
    uint8_t lastValue;
//
// Drowing counter
    uint8_t drawIndex = 0;
    uint8_t drawEntry = 0;
    uint8_t tripCursor = 1;
    uint8_t tripActive = 1;
    uint8_t tripCompare = 1;
    uint8_t tripReset = 0;
    uint8_t wordWidth = 0;

    //
    // from 14 to 64
    uint8_t graphTest[10] = {54, 20, 48, 14, 64, 46, 18, 35, 15, 48};
    const uint8_t *currentFont;

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
        lcd->enableUTF8Print();
//        lcd->setAutoPageClear(1);
    }

/**
 *
 * @param col
 * @param row
 * @param before
 * @param tag
 */
    void showTag(uint8_t col, uint8_t row, const char *before, uint8_t tag, uint8_t padding = LCD_PADDING) {
        wordWidth = lcd->getStrWidth(before);
        lcd->drawStr(col + wordWidth + padding, row, getMsg(tag));
    }

/**
 *
 * @param col
 * @param row
 * @param before
 * @param data
 * @param padding
 */
    void showDin(uint8_t col, uint8_t row, const char *before, const char *data, uint8_t padding = LCD_PADDING) {
        wordWidth = lcd->getStrWidth(before);
        lcd->drawStr(col + padding + wordWidth, row, data);
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


    void HeaderIcons() {
        this->setFont(u8g2_font_unifont_t_symbols);
        float temperature = car->getTmpOut();
        if (temperature > 20) {
            lcd->drawGlyph(1, 1, 0x2600);
        } else if (temperature < 20 && temperature > 0) {
            lcd->drawGlyph(1, 1, 0x2601);
        } else if (temperature < 0) {
            lcd->drawGlyph(1, 1, 0x2603);
        } else {
            lcd->drawGlyph(1, 1, 0x2610);
        }


    }

    void showHeader(const char *title) {
        lcd->enableUTF8Print();
        lcd->drawLine(0, 13, 240, 12);

        if (stt->isAlert() /*&& drawIndex % 3 == 0*/ || millis() < 7000 && drawIndex % 2 == 0) {
            lcd->drawXBMP(135, 1, 10, 10, wrench_10x10_bits);
            lcd->drawCircle(139, 6, 6, U8G2_DRAW_ALL);
        }
        if (car->getEngTmp() > 99 && drawIndex % 3 == 0) {
            lcd->drawXBMP(155, 1, 10, 10, fire_10x10_bits);
            lcd->drawCircle(159, 6, 6, U8G2_DRAW_ALL);
        }
        lcd->drawStr(15, 1, title);


    }

/**
 * Icons for fuel switching BNZ
 */
    void showIconBnz(uint8_t x, uint8_t y) {
        lcd->drawXBMP(x, y, 16, 16, bnz_16x16_bits);
    }

/**
 * Icons for fuel switching LPG
 */
    void showIconLpg(uint8_t x, uint8_t y) {
        lcd->drawXBMP(x, y, 16, 16, lpg_16x16_bits);
    }

//
// Servicing screens
    void warnCoolant() {
        lcd->drawXBMP(0, 0, 85, 64, coolant_level_85x64_bits);
        lcd->drawStr(90, LCD_ROW_1, this->getMsg(49));
        lcd->drawStr(90, LCD_ROW_3, this->getMsg(50));
        lcd->drawStr(90, LCD_ROW_4, this->getMsg(51));
    }

    void warnBattery(float Voltage) {
        lcd->drawXBMP(0, 0, 85, 64, low_battery_85x64_bits);
        lcd->drawStr(90, LCD_ROW_1, this->getMsg(43));
        lcd->drawStr(90, LCD_ROW_3, this->getMsg(44));

        displayFloat(Voltage, char_3);
        lcd->print(char_3);
        lcd->print("V");

        lcd->drawStr(90, LCD_ROW_4, this->getMsg(45));
    }

    void warnMotorOil() {
        lcd->drawXBMP(0, 0, 85, 64, oil_level_85x64_bits);
        lcd->drawStr(90, LCD_ROW_1, this->getMsg(40));
        lcd->drawStr(85, LCD_ROW_3, this->getMsg(41));
        lcd->drawStr(85, LCD_ROW_4, this->getMsg(42));
    }

    void warnLightsFront() {
        lcd->drawXBMP(0, 0, 85, 64, blub_replacment_85x64_bits);
        lcd->drawStr(90, LCD_ROW_1, this->getMsg(58));
        lcd->drawStr(85, LCD_ROW_3, this->getMsg(59));
        lcd->drawStr(85, LCD_ROW_4, this->getMsg(60));
    }


    void warnLightsBack() {
        lcd->drawXBMP(0, 0, 85, 64, blub_replacment_85x64_bits);
        lcd->drawStr(90, LCD_ROW_1, this->getMsg(58));
        lcd->drawStr(85, LCD_ROW_3, this->getMsg(59));
        lcd->drawStr(85, LCD_ROW_4, this->getMsg(61));
    }

    void warnBreakWare() {
        lcd->drawXBMP(0, 0, 85, 64, brake_ware_85x64_bits);
        lcd->drawStr(90, LCD_ROW_1, this->getMsg(46));
        lcd->drawStr(85, LCD_ROW_3, this->getMsg(47));
        lcd->drawStr(85, LCD_ROW_4, this->getMsg(48));
    }

    void warnOverheat() {
        lcd->drawXBMP(0, 0, 85, 64, overheating_85x64_bits);
        lcd->drawStr(90, LCD_ROW_1, this->getMsg(52));
        lcd->drawStr(85, LCD_ROW_3, this->getMsg(53));
        lcd->drawStr(85, LCD_ROW_4, this->getMsg(54));
    }

    void warnWinter() {
        lcd->drawXBMP(0, 0, 85, 64, winter_85x64_bits);
        lcd->drawStr(90, LCD_ROW_1, this->getMsg(37));
        lcd->drawStr(85, LCD_ROW_3, this->getMsg(38));
        lcd->drawStr(85, LCD_ROW_4, this->getMsg(39));
    }

    void warnWasher() {
        lcd->drawXBMP(0, 0, 85, 64, washer_85x64_bits);
        lcd->drawStr(90, LCD_ROW_1, this->getMsg(55));
        lcd->drawStr(85, LCD_ROW_3, this->getMsg(56));
        lcd->drawStr(85, LCD_ROW_4, this->getMsg(57));
    }

    void warnTmBelt() {
        lcd->drawXBMP(0, 0, 85, 64, servicing_85x64_bits);
        lcd->drawStr(90, LCD_ROW_1, this->getMsg(62));
        lcd->drawStr(85, LCD_ROW_3, this->getMsg(63));
        lcd->drawStr(85, LCD_ROW_4, this->getMsg(64));
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
     *  changes the font to given
     */
    void setFont(const uint8_t *font) {
        this->currentFont = font;
        this->changedFont = true;
    }
    //
    // TODO reset to default font...

    void drawFont() {
        if (this->changedFont) {
            lcd->setFont(this->currentFont);
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

        //
        // Font handler
        drawFont();

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
        // u8g2_font_crox1cb_tf
        // u8g2_font_crox1h_tf
        lcd->setFont(u8g2_font_crox1hb_tf); // u8g2_font_unifont_t_cyrillic
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
                lcd->drawStr(LCD_CNR - (backW / 2), 15, usedMenu.back);
                lcd->drawStr(LCD_CNR - (usedW / 2), 30, usedMenu.used);
                lcd->drawStr(LCD_CNR - (nextW / 2), 45, usedMenu.next);
                break;
            case 4:
            case 5:
            case 6:
            case 7:

                lcd->drawFrame(10, 12 + LCD_ENTRY_FRAME + (3 * 5), 212, 15);
                lcd->drawStr(LCD_CNR - (backW / 2), 15, usedMenu.back);
                lcd->drawStr(LCD_CNR - (usedW / 2), 30, usedMenu.used);
                lcd->drawStr(LCD_CNR - (nextW / 2), 45, usedMenu.next);
                if (usedMenu.down) {
                    lcd->drawStr(LCD_CNR + (usedW / 2) + subAnimateIndex + 20, 30, ">>");
                    lcd->drawStr((LCD_CNR + (usedW / 2)) + 40, 30, usedMenu.down);
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
        lcd->drawXBMP(x, y + 1 + LCD_ICO_HIGH, 9, 8, mark_km_9x8_bits);
    }

/**
 * Draws average char
 * @param x
 * @param y
 */
    inline void showAverage(u8g2_uint_t x, u8g2_uint_t y) {
        lcd->drawXBMP(x, y + LCD_ICO_HIGH, 8, 8, mark_phi_8x8_bits);
    }

/**
 * Draws instant char
 * @param x
 * @param y
 */
    inline void showInstant(u8g2_uint_t x, u8g2_uint_t y) {
        lcd->drawXBMP(x, y + LCD_ICO_HIGH, 8, 8, mark_now_5x8_bits);
    }


/**
 * Draws  L/100km
 * @param x
 * @param y
 */
    inline void showL100km(u8g2_uint_t x, u8g2_uint_t y) {
//        lcd->drawXBMP(/*50*/x, y + LCD_ICO_HIGH, 8, 8, mark_liter_per_8x8_bits);
//        lcd->drawXBMP(/*58*/x + 8, y + LCD_ICO_HIGH, 10, 8, mark_100_10x8_bits);
//        lcd->drawXBMP(/*69*/x + 19, y + LCD_ICO_HIGH, 9, 8, mark_km_9x8_bits);
        lcd->drawStr(x, y, getMsg(68));
        lcd->drawXBMP(x + 9, y + (LCD_ICO_HIGH / 2), 16, 9, per100km_16x9_bits);

    }

/**
 * Draws  celsius symbol
 * @param x
 * @param y
 */
    void showCels(u8g2_uint_t x, u8g2_uint_t y) {
        lcd->drawXBMP(x, y + LCD_ICO_HIGH, 4, 8, mark_cel_4x8_bits);
    }

/**
 *
 */
    inline void showLiter(u8g2_uint_t x, u8g2_uint_t y) {
        lcd->drawXBMP(x, y + LCD_ICO_HIGH, 4, 8, mark_liter_4x8_bits);
    }

/**
 * Displays consumed fuel
 */
    void displayHomeConsumption() {

        lcd->drawXBMP(4, LCD_ROW_1, 18, 18, fuel_18x18_bits);
        displayFloat(eep->getConsumedFuel(), char_3);
        lcd->drawStr(LCD_COL_L12, LCD_ROW_1, char_3);
        wordWidth = lcd->getStrWidth(char_3);

        lcd->drawStr(LCD_COL_L22, LCD_ROW_1, getMsg(68));
//        this->showTag(LCD_COL_L12, LCD_ROW_1, char_3, 68);
//        showLiter(LCD_COL_L22, LCD_ROW_1);
        //
        // When have several fuel lines
#ifdef LPG_SWITCHING_DETECT
        if (car->getFuelState() == 0) {
//            lcd->drawStr(LCD_COL_L23, LCD_ROW_1, "BNZ");
            this->showIconBnz(LCD_COL_L23 + 12, LCD_ROW_1);
        } else {
//            lcd->drawStr(LCD_COL_L23, LCD_ROW_1, "LPG");
            this->showIconLpg(LCD_COL_L23 + 12, LCD_ROW_1);
        }
#endif

        displayFloat(eep->getAverageLitersPer100km(), char_3);
        if (car->getDst() < 1 && eep->getTravelDistance() < 1) {
            lcd->drawStr(LCD_COL_L12, LCD_ROW_2, "00.0");
        } else {
            lcd->drawStr(LCD_COL_L12, LCD_ROW_2, char_3);
        }
        showL100km(LCD_COL_L22, LCD_ROW_2);
    }

/**
 * Display temperatures
 */
    void displayHomeTemperatures() {

        //
        // Outside graph
        lcd->drawXBMP(0, LCD_ROW_3, 18, 18, temp_18x18_bits);
        lcd->drawXBMP(20, LCD_ROW_3 + 2, 18, 10, car_out_18x10_bits);
        //
        // Inside Graph
        lcd->drawXBMP(20, LCD_ROW_4 + 2, 18, 10, car_ins_18x10_bits);


        //
        //
        displayFloat(car->getTmpOut(), char_3);
        lcd->drawStr(45, LCD_ROW_3, char_3);
        this->showCels(LCD_COL_L23 - 10, LCD_ROW_3);

//        lcd->drawGlyph(LCD_COL_L22, LCD_ROW_3, 0x2103);


        if (car->getTmpIns() < -99) {
            lcd->drawStr(45, LCD_ROW_4, "none");
        } else {
            //
            // Data
            displayFloat(car->getTmpIns(), char_3);
            lcd->drawStr(45, LCD_ROW_4, char_3);
            wordWidth = lcd->getStrWidth(char_3);
            this->showCels(LCD_COL_L23 - 10, LCD_ROW_4);
        }
//        lcd->setCursor(LCD_COL_L22, LCD_ROW_4);
//        lcd->print("~АА");
//        lcd->drawGlyph(LCD_COL_L22, LCD_ROW_3, 0x2103);

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
        lcd->drawStr(LCD_COL_R22, LCD_ROW_1, getMsg(69));
//        this->showTag(LCD_COL_R22, LCD_ROW_1, char_4, 69, 0);
//        showKm(LCD_COL_R22, LCD_ROW_1);
        //
        // Travel time
        car->getHTm(saved.time_trp, char_5);
        lcd->drawXBMP(LCD_COL_R11, LCD_ROW_2, 5, 8, car_time_5x8_bits);
        lcd->drawStr(LCD_COL_R12, LCD_ROW_2, char_5);
        lcd->drawStr(LCD_COL_R22, LCD_ROW_2, getMsg(70));
//        this->showTag(LCD_COL_R22, LCD_ROW_2, char_5, 70, 0);
//        lcd->drawStr(LCD_COL_R22, LCD_ROW_2, "h");
        //
        // Average speed
        showAverage(LCD_COL_R11, LCD_ROW_3);
        sprintf(char_2, "%02d", car->getAvrVss());
        lcd->drawStr(LCD_COL_R12, LCD_ROW_3, char_2);
        lcd->drawStr(LCD_COL_R22, LCD_ROW_3, getMsg(69));
//        this->showTag(LCD_COL_R22, LCD_ROW_3, char_5, 69, 0);
//        showKm(LCD_COL_R22, LCD_ROW_3);
        //
        // Instant cons per 100km
        showInstant(LCD_COL_R11, LCD_ROW_4);
        displayFloat(car->getIfcAvr(), char_3);
        lcd->drawStr(LCD_COL_R12, LCD_ROW_4, char_3);
        if (car->getVss() > 0) {
            showL100km(LCD_COL_R22, LCD_ROW_4);
        } else {
            lcd->drawStr(LCD_COL_R22, LCD_ROW_4, getMsg(71));
//            this->showTag(LCD_COL_R22, LCD_ROW_4, char_3, 71, 0);
        }

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
            lcd->setCursor(0, LCD_ROW_1);
            lcd->print(F("*"));
        }
        lcd->drawStr(LCD_COL_L11, LCD_ROW_1, getMsg(3));
        if (stt->getLiveBrk()) lcd->drawStr(LCD_COL_R12 - 20, LCD_ROW_1, (drawIndex < 5) ? getMsg(9) : getMsg(8));
        else lcd->drawStr(LCD_COL_R12 - 20, LCD_ROW_1, getMsg(7));

        if (history.cnt) {
            lcd->setCursor(0, LCD_ROW_2);
            lcd->print(F("*"));
        }
        lcd->drawStr(LCD_COL_L11, LCD_ROW_2, getMsg(4));
        if (stt->getLiveCnt()) lcd->drawStr(LCD_COL_R12 - 20, LCD_ROW_2, getMsg(8));
        else lcd->drawStr(LCD_COL_R12 - 20, LCD_ROW_2, getMsg(7));


        if (history.win) {
            lcd->setCursor(0, LCD_ROW_3);
            lcd->print(F("*"));
        }
        lcd->drawStr(LCD_COL_L11, LCD_ROW_3, getMsg(5));
        if (stt->getLiveWin()) lcd->drawStr(LCD_COL_R12 - 20, LCD_ROW_3, getMsg(8));
        else lcd->drawStr(LCD_COL_R12 - 20, LCD_ROW_3, getMsg(7));


        if (history.oil) {
            lcd->setCursor(0, LCD_ROW_4);
            lcd->print(F("*"));
        }
        lcd->drawStr(LCD_COL_L11, LCD_ROW_4, getMsg(6));
        if (stt->getLiveOil()) lcd->drawStr(LCD_COL_R12 - 20, LCD_ROW_4, getMsg(8));
        else lcd->drawStr(LCD_COL_R12 - 20, LCD_ROW_4, getMsg(7));


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
    void displayTripDetails() {
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
        //
        // Breaking time
        lcd->drawXBMP(LCD_COL_L11, LCD_ROW_3, 18, 10, car_brk_18x10_bits);
        displayFloat(car->getBreakTime(), char_3);
        lcd->drawStr(LCD_COL_L21, LCD_ROW_3, char_3);
        lcd->setCursor(LCD_COL_L23, LCD_ROW_3);
        lcd->print(F("min."));
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


    void buildInnerRowTrip(float carFuelConsumption, float dataFuel, float distance, uint8_t y) {
        displayFloat(dataFuel + carFuelConsumption, char_3);
        lcd->print(char_3);
        lcd->print("L ");
        showAverage(LCD_COL_R11, y);
        lcd->setCursor(LCD_COL_R12, y);
        displayFloat(((dataFuel + car->getAdtFuelCns()) * 100) / distance, char_3);
        if (distance > 1) {
            lcd->print(char_3);
        } else {
            lcd->print("00.0");
        }
        lcd->print("L  ");
    }

/**
 * Dysplay trip row
 * */
    void buildRowTrip(const char *name, TripData data, uint8_t y) {
        float dst = car->getDst() + data.range;
        lcd->setCursor(LCD_COL_L11, y);
        lcd->print(name);
        displayFloat(dst, char_4);
        lcd->print(char_4);
        lcd->print("km");
        lcd->setCursor(LCD_COL_L23 + 15, y);


#ifdef  DEFAULT_FUEL_USING
        if (DEFAULT_FUEL_USING == 1) {
            buildInnerRowTrip(car->getAdtFuelCns(), data.fuel, dst, y);
        } else {
            buildInnerRowTrip(car->getDefFuelCns(), data.fuel, dst, y);
        }
#else
        buildInnerRowTrip(car->getDefFuelCns(), data.fuel, dst, y);
#endif
    }

    void displayTrips() {
        this->buildRowTrip("T0:  ", eep->getTrip0(), LCD_ROW_1);
        this->buildRowTrip("T1:  ", eep->getTripA(), LCD_ROW_2);
        this->buildRowTrip("T2:  ", eep->getTripB(), LCD_ROW_3);
        this->buildRowTrip("T3:  ", eep->getTripC(), LCD_ROW_4);

        btn->setEditorState(true);

        if (drawIndex < 2 && initializeDraw) {
            //
            // Change values and speed of buttons
            btn->setValueControlled(tripCursor);
            //
            // Change speed of screen
            this->playUltra();
        }
        tripCompare = tripCursor;
        //
        // Manage section
        if (!btn->getNavigationState() && drawIndex % 2 == 0) {
            uint8_t cursor = (uint8_t) btn->getValueControlled();

            if (cursor > tripCompare && drawIndex % 4 == 0) {
                tripCursor++;
                tripReset = 0;

            }

            boolean delete_trip = false;
            if (tripReset > cursor && cursor < tripCompare) {

                Serial.println("NOTICE: Delete trip.... ");
                tripReset = 0;
                tripCompare = 0;
                delete_trip = true;
            }

            if (cursor < tripCompare) {
                lcd->drawStr(LCD_COL_R22, tripActive, "  X");
                tripReset = cursor;
            }
//

            if (tripCursor == 1) {
                lcd->setCursor(0, LCD_ROW_2);
                tripActive = LCD_ROW_2;
            } else if (tripCursor == 2) {
                lcd->setCursor(0, LCD_ROW_3);
                tripActive = LCD_ROW_3;
            } else if (tripCursor == 3) {
                lcd->setCursor(0, LCD_ROW_4);
                tripActive = LCD_ROW_4;
            } else {
                tripCursor = 1;// Clear wrong cursor
                btn->setValueControlled(tripCursor);
                Serial.println("ERROR: Trip cursor out of range .... ");
            }
            //
            // Display cursor blink
            if (drawIndex % 4 == 0) {
                lcd->print(">");

            }

            //
            // Reset trip data from EepRom
            if (delete_trip) {
                if (tripActive == LCD_ROW_2) {
                    eep->resetTripA();
                    Serial.println("ERROR: DELETE trip A ");
                } else if (tripActive == LCD_ROW_3) {
                    eep->resetTripB();
                    Serial.println("ERROR: DELETE trip B ");
                } else if (tripActive == LCD_ROW_4) {
                    eep->resetTripC();
                    Serial.println("ERROR: DELETE trip C ");
                } else {
                    Serial.println("ERROR: Cannot find cursor for deleting .... ");
                }
                tripCursor = 1;
                tripCompare = 1;
                tripActive = 0;
                tripReset = 0;
                btn->setNavigationState(true);
            }


        }
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
 * TODO add icons
 * */
#ifdef ADT_FUEL_SYSTEM_SERIAL
        sprintf(char_2, "%02d", lpgCom.getFuelTankLiters());
        lcd->setCursor(LCD_COL_L11, LCD_ROW_1);
        lcd->print(F("LPG tank:  "));
        lcd->print(char_2);
        lcd->print(F("l"));


        sprintf(char_2, "%02d", lpgCom.getCurrentValue());
        lcd->setCursor(LCD_COL_L11, LCD_ROW_2);
        lcd->print(F("LPG value: "));
        lcd->print(char_2);

        displayFloat(car->getFuelTnk(), char_3);
        lcd->setCursor(LCD_COL_R11, LCD_ROW_2);
        lcd->print(F("BNZ value: "));
        lcd->print(char_3);
        lcd->print("L");
#endif

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
            this->playUltra();
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
            this->changedFont = false;
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
            displayTrips();
            break;
            //
            // Fuel menu
        case 21:
            showHeader(getMsg(13));
            displayFuel();
            break;
        case 22:
            setFont(u8g2_font_logisoso58_tr);
            displayRallyGages();
            break;

        case 4:
            showHeader(getMsg(14));
            displayCarState();
            break;

        case CarState::MENU_SERVICE:
            stt->menu(this);
            break;

        case ShutDw::MENU_SHUTDOWN:
            sdw->menu(this);
            break;
    }

}


#endif //ARDUINOMID_LCD240X64_H

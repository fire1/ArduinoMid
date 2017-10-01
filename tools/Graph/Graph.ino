//
// Created by Angel Zaprianov on 2.6.2017 г..
//

#ifndef ARDUINO_MID_GRAP_H
#define ARDUINO_MID_GRAP_H

#include <U8g2lib.h>


#ifndef _U8G2LIB_HH
// Some IDE syntax mishmash fixer
#include "../../libraries/U8g2/src/U8g2lib.h"

#endif

double ox, oy;

/*

 @author Kris Kasprzak

  function to draw a cartesian coordinate system and plot whatever data you want
  just pass x and y and the graph will be drawn

  huge arguement list
  &d name of your display object
  x = x data point
  y = y datapont
  gx = x graph location (lower left)
  gy = y graph location (lower left)
  w = width of graph
  h = height of graph
  xlo = lower bound of x axis
  xhi = upper bound of x asis
  xinc = division of x axis (distance not count)
  ylo = lower bound of y axis
  yhi = upper bound of y asis
  yinc = division of y axis (distance not count)
  title = title of graph
  xlabel = x asis label
  ylabel = y asis label
  gcolor = graph line colors
  acolor = axi ine colors
  pcolor = color of your plotted data
  tcolor = text color
  bcolor = background color
  &redraw = flag to redraw graph on fist call only
*/


void Graph(U8G2 d, double x, double y, double gx, double gy, double w, double h, double xlo, double xhi, double xinc,
           double ylo, double yhi, double yinc, String title, String xlabel, String ylabel, boolean &redraw) {

    double ydiv, xdiv;
    // initialize old x and old y in order to draw the first point of the graph
    // but save the transformed value
    // note my transform funcition is the same as the map function, except the map uses long and we need doubles
    //static double ox = (x - xlo) * ( w) / (xhi - xlo) + gx;
    //static double oy = (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
    double i;
    double temp;
    int rot, newrot;

    if (redraw == true) {

        redraw = false;
        ox = (x - xlo) * (w) / (xhi - xlo) + gx;
        oy = (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
        // draw y scale
        for (i = ylo; i <= yhi; i += yinc) {
            // compute the transform
            temp = (i - ylo) * (gy - h - gy) / (yhi - ylo) + gy;

            if (i == 0) {
                d.drawLine(gx, temp, gx + w, temp);
            } else {
                d.drawLine(gx, temp, gx + w, temp);
            }

//            d.setTextSize(1);
//            d.setTextColor(tcolor, bcolor);
//            d.setCursor(gx - 40, temp);
//            // precision is default Arduino--this could really use some format control
//            d.println(i);
        }
        // draw x scale
        for (i = xlo; i <= xhi; i += xinc) {

            // compute the transform

            temp = (i - xlo) * (w) / (xhi - xlo) + gx;
            if (i == 0) {
                d.drawLine(temp, gy, temp, gy - h);
            } else {
                d.drawLine(temp, gy, temp, gy - h);
            }

//            d.setTextSize(1);
//            d.setTextColor(tcolor, bcolor);
//            d.setCursor(temp, gy + 10);
//            // precision is default Arduino--this could really use some format control
//            d.println(i);
        }

        //now draw the labels
//        d.setTextSize(2);
//        d.setTextColor(tcolor, bcolor);
//        d.setCursor(gx , gy - h - 30);
//        d.println(title);
//
//        d.setTextSize(1);
//        d.setTextColor(acolor, bcolor);
//        d.setCursor(gx , gy + 20);
//        d.println(xlabel);
//
//        d.setTextSize(1);
//        d.setTextColor(acolor, bcolor);
//        d.setCursor(gx - 30, gy - h - 10);
//        d.println(ylabel);


    }

    //graph drawn now plot the data
    // the entire plotting code are these few lines...
    // recall that ox and oy are initialized as static above
    x = (x - xlo) * (w) / (xhi - xlo) + gx;
    y = (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
    d.drawLine(ox, oy, x, y);
    d.drawLine(ox, oy + 1, x, y + 1);
    d.drawLine(ox, oy - 1, x, y - 1);
    ox = x;
    oy = y;

}

//
// Display pins map
//  NOTE: pins are inverted due to sockets
//
//  General pins
const uint8_t DSP_PIN_GD1 = 26; // Power GND
const uint8_t DSP_PIN_GD2 = 27; // Power GND
const uint8_t DSP_PIN_VCC = 29; // Power positive
const uint8_t DSP_PIN_WR = 31;  // Write Signal
const uint8_t DSP_PIN_RD = 30;  // Read Signal
const uint8_t DSP_PIN_CE = 33;  // Chip Enable Signal
const uint8_t DSP_PIN_CD = 32;  // Instruction Code
const uint8_t DSP_PIN_FS = 45;  // Font selection
const uint8_t DSP_PIN_RST = 34; // Reset signal
const uint8_t DSP_PIN_LDA = 46; // Blacklight Anode (+5V)
const uint8_t DSP_PIN_LDK = 47; // Blacklight cathode (0v)
//
// Data pins
const uint8_t DSP_PIN_DT1 = 37;
const uint8_t DSP_PIN_DT2 = 36;
const uint8_t DSP_PIN_DT3 = 39;
const uint8_t DSP_PIN_DT4 = 38;
const uint8_t DSP_PIN_DT5 = 41;
const uint8_t DSP_PIN_DT6 = 40;
const uint8_t DSP_PIN_DT7 = 43;
const uint8_t DSP_PIN_DT8 = 42;

const uint8_t DIM_PIN_OUT = 49;     //              Output dim of playEntry


/**
 * 16x2 && 240x64 power pins
 */
class LcdPwr {
private:
    uint8_t pwr[5];
    uint8_t adt[2];
    bool additional = false;
public:
/**
 *
 * @param pinGnd
 * @param pinVcc
 * @param pinBlA
 * @param pinBlB
 */
    LcdPwr(uint8_t pinGnd, uint8_t pinVcc, uint8_t pinBlA, uint8_t pinBlB, uint8_t pinRD) {
        pwr[0] = pinGnd;
        pwr[1] = pinVcc;
        pwr[2] = pinBlA;
        pwr[3] = pinBlB;
        pwr[4] = pinRD;
    }

/**
 *
 * @param pinGnd    Ground
 * @param pVcc    5V+
 * @param pBLn    BackLight Ground
 * @param pBLp    BackLight 5v+
 * @param pRD     Display's chip read
 * @param pFS     Display's Font Select
 * @param pGnd2   Second ground
 */
    LcdPwr(uint8_t pinGnd, uint8_t pVcc, uint8_t pBLn, uint8_t pBLp, uint8_t pRD, uint8_t pFS, uint8_t pGnd2 = 0) {
        pwr[0] = pinGnd;
        pwr[1] = pVcc;
        pwr[2] = pBLn;
        pwr[3] = pBLp;
        pwr[4] = pRD;
        additional = true;
        adt[0] = pFS;
        adt[1] = pGnd2;
    }


    void begin() {
        analogWrite(pwr[0], 0); // GND
        analogWrite(pwr[1], 255); // 5V
        analogWrite(pwr[3], 0); // back light
        analogWrite(pwr[2], 255); // back light
        analogWrite(pwr[4], 255); // RD

        if (additional) {
            analogWrite(adt[0], 0); // fs
            if (adt[1] > 0) analogWrite(adt[2], 0); // GND
        }

//        delete this;
    }

    ~LcdPwr() {};
};

LcdPwr lcdPwr = LcdPwr(DSP_PIN_GD1, DSP_PIN_VCC, DSP_PIN_LDK, DSP_PIN_LDA, DSP_PIN_RD, DSP_PIN_FS, DSP_PIN_GD2);
//
// Check https://github.com/olikraus/u8g2/wiki/u8g2setupcpp for display begin
U8G2_T6963_240X64_2_8080 // Screen constructor
        u8g2(U8G2_R0,
             DSP_PIN_DT1,
             DSP_PIN_DT2,
             DSP_PIN_DT3,
             DSP_PIN_DT4,
             DSP_PIN_DT5,
             DSP_PIN_DT6,
             DSP_PIN_DT7,
             DSP_PIN_DT8,
        /*WR*/ DSP_PIN_WR, /*CE*/ DSP_PIN_CE, /*dc8*/DSP_PIN_CD, /*RST*/ DSP_PIN_RST);
//U8G2_T6963_240X64_2_8080 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7,/*WR*/ 14, /*CE*/ 16, /*dc8*/17, /*RST*/ 18); // Connect RD with +5V, FS0 and FS1 with GND

void useTextMode() {
    // Cyrillic font u8g2_font_crox1c_tf
    // u8g2_font_crox1cb_tf
    //u8g2_font_mercutio_basic_nbp_t_all
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.setFontRefHeightExtendedText();
    u8g2.setFontDirection(0);
    u8g2.setDrawColor(1);
    u8g2.setFontPosTop();
}

void setup() {
    lcdPwr.begin();
    u8g2.begin();
    useTextMode();
}

uint8_t drawIndex = 0;

boolean displayed = true;
void loop() {
    double x, y;

    x = drawIndex * 0.01;
    y = ((sin(x)) * x + cos(x)) - log(x);
    Graph(u8g2, x, y, 0, 50, 240, 50, 12, 20, 1, -20, 20, 5, "Weird Function", "x", " bla", displayed);

    drawIndex++;
    if(drawIndex > 10){
        drawIndex = 0;
    }
    delay(100);

}

#endif //ARDUINOMID_GRAPH_H

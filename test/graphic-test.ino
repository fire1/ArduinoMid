/*

  GraphicsTest.ino

  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2016, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list
    of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright notice, this
    list of conditions and the following disclaimer in the documentation and/or other
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include <Arduino.h>
#include <U8g2lib.h>

//#ifdef U8X8_HAVE_HW_SPI
//#include <SPI.h>
//#endif
//#ifdef U8X8_HAVE_HW_I2C
//#include <Wire.h>
//#endif


/*
  mega 2560
  ________________________________________________________________
   T6963c LCD PIN   |    LCD PIN NAME  |     MEGA 2560            |
  ________________________________________________________________|
          1         |          FG      |       GND                |
          2         |         GND      |       GND                |
          3         |         VDD      |       +5V                |
          4         |          VO      |  10K POT WIPER – LEG 2   |
          5         |          WR      |        17                |
          6         |          RD      |        18                |
          7         |       CE/CS      |        14                |
          8         |         C/D      |        15                |
          9         |         RST      |        16                |
         10         |         DB0      |         8                |
         11         |         DB1      |         9                |
         12         |         DB2      |        10                |
         13         |         DB3      |        11                |
         14         |         DB4      |         4                |
         15         |         DB5      |         5                |
         16         |         DB6      |         6                |
         17         |         DB7      |         7                |
         18         |          FS      |       GND                |
         19         |         VEE      |    10K POT–LEG1          |
         20         |       LED A      |    +3.3V – 5V            |
  ________________________________________________________________|
 *
 *
 *
  U8glib Example Overview:
    Frame Buffer Examples: clearBuffer/sendBuffer. Fast, but may not work with all Arduino boards because of RAM consumption
    Page Buffer Examples: firstPage/nextPage. Less RAM usage, should work with all Arduino boards.
    U8x8 Text Only Example: No RAM usage, direct communication with display controller. No graphics, 8x8 Text only.

Ctrl : WR : analog pin 0
Ctrl : RD : analog pin 1
Ctrl : CE : analog pin 2
Ctrl : CD : analog pin 3
Ctrl : RESET : analog pin 4
Ctrl : FS : analog pin 5

d0 ... d7  Data lines of the parallel interface  D0 ... D7
cs  Chip select line  CS
dc  Data/command selection line (register select) D/C, A0, RS, ...
enable  "Write" for the 8080 interface, "enable" for the 6800 interface 8080: WR, 6800: E
reset Reset line

cs      Chip select line  CS
dc      Data/command selection line (register select) D/C, A0, RS, ...
enable  "Write" for the 8080 interface, "enable" for the 6800 interface 8080: WR, 6800: E
reset   Reset line


1   |   GND   |   power GND
2   |   GND   |   Power GND
3   |   VCC   |   Power positive
4   |   V0    |   Negative Voltage (LCD contrast)
5   |   WR    |   Write Signal                          |
6   |   RD    |   Read Signal
7   |   CE    |   Chip Enable Signal
8   |   CD    |   H: Data L:  Instruction Code          |       DC
9   |   NC    |   No connection
10  |   RST   |   Reset Signal Input
11  /   18    |   DB0..DB7 Data Bus Line
19  |   FS    |   Font selection
20  |   VEE   |   Negative Voltage Supply
21  |   LED_A |   Blacklight Anode (+5V)
22  |   LED_K |   Blacklight cathode (0v)


4  	 VO  	50k             POT WIPER (Leg 2)
5  	/WR    	wr  	35      WRITE SIGNAL
6  	/RD    	rd	    34      READ SIGNAL
7  	/CE  	cs	    37      CHIP ENABLE INPUT
8  	C/D  	        36      INSTRUCTION CODE, L DATA
9  	RST  	reset  	33      RESET SIGNAL


cs, cs1, cs2: Chip select, datasheet names: CS, CS1, CS2 or CE
a0, di: Data/instruction register select, datasheet names: A0, DI, CD or RS
rw: Read/write line         WR
en: Read/write enable       RD


D0      -   d7: Parallel data
CS      -   Chip select line       CS
DC      -   Data/command selection line (Data/instruction)              (register select)
ENABLE  -   "Write" for the 8080 interface,                             "enable" for the 6800 interface



#define GLCD_WR          0 // A3
#define GLCD_RD          1 //  A4
#define GLCD_CE          2 //  A0  //Should be able to XNOR this with WR and RD
#define GLCD_CD          3   //  A1
#define GLCD_RESET     4 //
*/

//#define U8G2_16BIT
U8G2_T6963_240X64_2_8080
        u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7,
        /*WR*/ 14, /*CE*/ 16, /*dc8*/17, /*RST*/ 18); // Connect RD with +5V, FS0 and FS1 with GND

//U8G2_T6963_240X64_1_8080 u8g2(U8G2_R0, 2, 3, 4, 5, 6, 7, 8, 9, /*enable=A3/5*/ A3, /*cs=A0/7*/ A0, /*dc=A1/8*/
//                              A1, /*reset=A2/10 */ A2); // Connect RD with +5V, FS0 and FS1 with GND
//U8G2_T6963_240X64_1_8080(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7,  enable,  cs,  dc)
// Please UNCOMMENT one of the contructor lines below
// U8g2 Contructor List (Frame Buffer)
// The complete list is available here: https://github.com/olikraus/u8g2/wiki/u8g2setupcpp
// Please update the pin numbers according to your setup. Use U8X8_PIN_NONE if the reset pin is not connected

// End of constructor list


void u8g2_prepare(void) {
    //
    // u8g2_font_4x6_tf, u8g2_font_5x7_tf
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.setFontRefHeightExtendedText();
    u8g2.setDrawColor(1);
    u8g2.setFontPosTop();
    u8g2.setFontDirection(0);
}

void u8g2_box_frame(uint8_t a) {
    u8g2.drawStr(0, 0, "drawBox");
    u8g2.drawBox(5, 10, 20, 10);
    u8g2.drawBox(10 + a, 15, 30, 7);
    u8g2.drawStr(0, 30, "drawFrame");
    u8g2.drawFrame(5, 10 + 30, 20, 10);
    u8g2.drawFrame(10 + a, 15 + 30, 30, 7);
}

void u8g2_disc_circle(uint8_t a) {
    u8g2.drawStr(0, 0, "drawDisc");
    u8g2.drawDisc(10, 18, 9);
    u8g2.drawDisc(24 + a, 16, 7);
    u8g2.drawStr(0, 30, "drawCircle");
    u8g2.drawCircle(10, 18 + 30, 9);
    u8g2.drawCircle(24 + a, 16 + 30, 7);
}

void u8g2_r_frame(uint8_t a) {
    u8g2.drawStr(0, 0, "drawRFrame/Box");
    u8g2.drawRFrame(5, 10, 40, 30, a + 1);
    u8g2.drawRBox(50, 10, 25, 40, a + 1);
}

void u8g2_string(uint8_t a) {
    u8g2.setFontDirection(1);
    u8g2.drawStr(30 + a, 31, " 0");
    u8g2.setFontDirection(1);
    u8g2.drawStr(30, 31 + a, " 90");
    u8g2.setFontDirection(2);
    u8g2.drawStr(30 - a, 31, " 180");
    u8g2.setFontDirection(3);
    u8g2.drawStr(30, 31 - a, " 270");
}

void u8g2_line(uint8_t a) {
    u8g2.drawStr(0, 0, "drawLine");
    u8g2.drawLine(7 + a, 10, 40, 55);
    u8g2.drawLine(7 + a * 2, 10, 60, 55);
    u8g2.drawLine(7 + a * 3, 10, 80, 55);
    u8g2.drawLine(7 + a * 4, 10, 100, 55);
}

void u8g2_triangle(uint8_t a) {
    uint16_t offset = a;
    u8g2.drawStr(0, 0, "drawTriangle");
    u8g2.drawTriangle(14, 7, 45, 30, 10, 40);
    u8g2.drawTriangle(14 + offset, 7 - offset, 45 + offset, 30 - offset, 57 + offset, 10 - offset);
    u8g2.drawTriangle(57 + offset * 2, 10, 45 + offset * 2, 30, 86 + offset * 2, 53);
    u8g2.drawTriangle(10 + offset, 40 + offset, 45 + offset, 30 + offset, 86 + offset, 53 + offset);
}

void u8g2_ascii_1() {
    char s[2] = " ";
    uint8_t x, y;
    u8g2.drawStr(0, 0, "ASCII page 1");
    for (y = 0; y < 3; y++) {
        for (x = 0; x < 16; x++) {
            s[0] = y * 16 + x + 32;
            u8g2.drawStr(x * 7, y * 10 + 10, s);
        }
    }
}

void u8g2_ascii_2() {
    char s[2] = " ";
    uint8_t x, y;
    u8g2.drawStr(0, 0, "ASCII page 2");
    for (y = 0; y < 3; y++) {
        for (x = 0; x < 16; x++) {
            s[0] = y * 16 + x + 160;
            u8g2.drawStr(x * 7, y * 10 + 10, s);
        }
    }
}

//u8g_font_04b_03b
void u8g2_extra_page(uint8_t a) {
    u8g2.drawStr(0, 0, "Unicode");
    // u8g_font_04b_03b
    u8g2.setFont(u8g2_font_unifont_t_symbols);
    u8g2.setFontPosTop();
    u8g2.drawUTF8(0, 24, "☀ ☁");
    switch (a) {
        case 0:
        case 1:
        case 2:
        case 3:
            u8g2.drawUTF8(a * 3, 36, "☂");
            break;
        case 4:
        case 5:
        case 6:
        case 7:
            u8g2.drawUTF8(a * 3, 36, "☔");
            break;
    }
    delay(5);
}


uint8_t draw_state = 0;

void draw(void) {
    u8g2_prepare();
    switch (draw_state >> 3) {
        case 0:
            u8g2_box_frame(draw_state & 7);
            break;
        case 1:
            u8g2_disc_circle(draw_state & 7);
            break;
        case 2:
            u8g2_r_frame(draw_state & 7);
            break;
        case 3:
            u8g2_string(draw_state & 7);
            break;
        case 4:
            u8g2_line(draw_state & 7);
            break;
        case 5:
            u8g2_triangle(draw_state & 7);
            break;
        case 6:
            u8g2_ascii_1();
            break;
        case 7:
            u8g2_ascii_2();
            break;
        case 8:
            u8g2_extra_page(draw_state & 7);
            break;
        case 9:
            u8g2.drawRFrame(1, 1, 120, 12,1); // drawRFrame
            u8g2.drawStr(5, 2, "Hello Angel! ");

            u8g2.drawLine(240, 0, 0, 0);

            //u8g2.drawUTF8((draw_state & 7) * 3, 28, " ❄");

            //u8g2.drawUTF8((draw_state & 7) * 3, 36, " 🛠");

            u8g2.drawStr((draw_state & 7) *3, 50, "How are you? ");

            break;
    }
}


void setup(void) {
    digitalWrite(15, HIGH);
    u8g2.begin();
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.setFontRefHeightExtendedText();
    u8g2.setDrawColor(1);
    u8g2.setFontPosTop();
    u8g2.setFontDirection(0);
    delay(1000);


    Serial.begin(9600);
}

void loop(void) {
    // picture loop
    //u8g2.clearBuffer();
    //u8g2.setCursor(240, 64);
    u8g2.firstPage();
    do {
        draw();
    } while ( u8g2.nextPage() );
    //u8g2.sendBuffer();

    // increase the state
    draw_state++;
    if (draw_state >= 9 * 9)
        draw_state = 0;

    // deley between each page
    delay(100);

}
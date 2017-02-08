


#define __PROG_TYPES_COMPAT__


#include "T6963.h"
#include "gfxdata.h"
#include "Times_New_Roman__14.h"

T6963 LCD(240, 64, 6, 32);// 240x64 Pixel and 6x8 Font


PROGMEM const char **animdemo[] =       // Flash Images for Animation 24 Frames
        {
                IMG01bmp, IMG02bmp, IMG03bmp, IMG04bmp, IMG05bmp, IMG06bmp, IMG07bmp, IMG08bmp, IMG09bmp, IMG10bmp,
                IMG11bmp, IMG12bmp, IMG13bmp, IMG14bmp, IMG15bmp, IMG16bmp, IMG17bmp,
                IMG18bmp, IMG19bmp, IMG20bmp, IMG21bmp, IMG22bmp, IMG23bmp, IMG24bmp
        };

char textmode[] = {'0', '5', '8', 'D'};
char *textmessage[] = {"           * Normal Display *           ", "          * Reverse Display *          ",
                       "      * Blink of normal Display *      ", "      * Blink of reverse Display *      "};
int pcount;

void setup() {
    Serial.begin(9600);
    Serial.print("TH: ");
    Serial.println(LCD.getTH());
    Serial.print("GH: ");
    Serial.println(LCD.getGH());

    LCD.Initialize();

    Serial.println("Initialized");


}

void loop() {

    for (byte _b = 0; _b < 240; _b++) {
        LCD.writePixel(_b, _b >> 1, 1);
    }
    for (byte _b = 0; _b < 64; _b += 8) {
        LCD.TextGoTo(_b / 8, _b / 8);
        LCD.writeString("Hello World");
    }
    delay(1000);
    LCD.clearGraphic();
    LCD.createLine(0, 0, 239, 63, 1);
    delay(1000);
    LCD.createLine(239, 0, 0, 63, 1);
    LCD.clearGraphic();
    delay(500);
    LCD.clearText();
    LCD.setPixel(0, 0);
    LCD.setPixel(239, 0);
    LCD.setPixel(239, 63);
    LCD.setPixel(0, 63);
    delay(1000);
    for (int x = 0; x < 64; x += 4) {
        LCD.createLine(0, x, x << 1, 63, 1);
        delay(50);
    }
    delay(1000);
    LCD.createCircle(80, 24, 16, 1);
    LCD.createCircle(90, 26, 16, 1);
    LCD.createCircle(100, 28, 16, 1);
    LCD.createCircle(110, 30, 16, 1);
    LCD.createCircle(120, 32, 16, 1);
    LCD.createCircle(130, 34, 16, 1);
    LCD.createCircle(140, 36, 16, 1);
    LCD.createCircle(150, 38, 16, 1);
    LCD.createCircle(160, 40, 16, 1);
    delay(1000);
    LCD.clearGraphic();
    LCD.clearText();

    // Text Attribute Mode by T6963C

    //Set Text attributes in Text Mode,Graphic Off
    LCD.setMode('T', 'I');//0=Normal X=Xor A=And T=Text Attribute Mode  ,  I=Intern CGram ,E=Extern CGram

    for (int i = 0; i < 4; i++) {
        LCD.TextGoTo(0, 0);
        LCD.writeString("TEXT DEMO Textattribute Set by T6963C");
        LCD.TextGoTo(0, 2);
        LCD.writeString(textmessage[i]);
        LCD.setTextAttrMode(
                textmode[i]); // 0=Normal display 5=Reverse display 3=Inhibit display 8=Blink of normal display D=Blink of reverse display B=Blink of inhibit display (TEXT ONLY)

        for (int x = 0; x <= 126; x++) {
            LCD.TextGoTo(x, 4);
            LCD.writeChar(x + 33);
            delay(100);
        }
        LCD.clearText();
    }
    LCD.setMode('0', 'I');//0=Normal X=Xor A=And T=Text Attribute Mode  ,  I=Intern CGram ,E=Extern CGram
    LCD.clearGraphic();

/* 
  LCD.setDispMode(true,true,true,false);
  for(byte _b=0;_b<8;_b++){
    for(byte _row = 0;_row < LCD.getTextRows();_row++){
      for(byte _col = 0;_col < LCD.getTextCols();_col++){
        LCD.setCursorPointer(_col,_row);
        delay(100);
      }
    }
    LCD.setCursorPattern(_b);
  }
  LCD.setDispMode(true,true,false,false);
*/


    // Filled Box Speed Demo
    for (int i = 0; i < 5; i++) {
        LCD.TextGoTo(0, 0);
        LCD.writeString("       Speed Demo with filled Box       ");
        LCD.TextGoTo(1, 7);
        LCD.writeString("    write Byte         write Pixel");
        LCD.Rectangle(4, 10, 112, 44,
                      1);                            //Position Horizontal, Position Vertikal, Länge, Höhe des Rechteck
        LCD.drawrectbyte(10, 12, 40, 18,
                         0b111111);                  // drawrectbyte(x, y, height, bytewidth, pattern) // x,y = Start Position, Height in Pixel, Widht in Byte (6x8Font =6 Bits 8x8Font = 8 Bits, Fill pattern in byte Example:0b10101010
        LCD.Rectangle(124, 10, 112, 44, 1);
        LCD.fillRect(126, 12, 233, 51, 1);                           //X,Y,X1,Y1
        delay(100);
        LCD.drawrectbyte(10, 12, 40, 18, 0b00000000);//Clear Box byte
        LCD.drawrectbyte(126, 12, 40, 18, 0b00000000);//Clear Box Pixel
    }
    LCD.clearGraphic();
    LCD.clearText();

// Arduino Image 1 + 2
    LCD.drawPic(72, 0, Arduinobmp, ArduinoHEIGHT,
                ArduinoBYTEWIDTH);    //x,y = Position, Picturename (not Filename), Height in Pixel, Widht in Byte
    delay(3000);
    LCD.drawPic(72, 0, Arduino1bmp, Arduino1HEIGHT,
                Arduino1BYTEWIDTH);    //x,y = Position, Picturename (not Filename), Height in Pixel, Widht in Byte
    delay(3000);
    LCD.clearGraphic();

// Animated Image
    LCD.setMode('x', 'I'); //Mode Set for Reverse Text
    for (int i = 0; i < 10; i++) {
        LCD.drawrectbyte(0, 0, 8, 40,
                         0b111111);                  // drawrectbyte(x, y, height, bytewidth,pattern) // x,y = Start Position, Height in Pixel, Widht in Byte (6x8Font =6 Pixel 8x8Font = 8 Pixel, Fill pattern in byte Example:0b10101010
        LCD.TextGoTo(1, 0);
        LCD.writeString("LCD GRAPHIC DISPLAY 240x64 with T6963C");
        LCD.drawAnim(62, 15, animdemo, 24, pictureHEIGHT, pictureBYTEWIDTH);

    }
    LCD.setMode('0', 'I');//Switch to Normal Mode
    LCD.clearGraphic();
    LCD.clearText();



    //Font Demo
    LCD.glcd_print1_P(3, 0, "Font Times New Roman14", &Times_New_Roman__14, 0);
    LCD.glcd_print1_P(60, 20, "Hello World", &Times_New_Roman__14, 0);
    LCD.glcd_print1_P(60, 40, "Hello World", &Times_New_Roman__14, 1);
    delay(4000);
    LCD.clearGraphic();

}

/*
---------------------------------------------------
    Arduino MID

     Author: Angel Zaprianov <me@fire1.eu>
     Version: 0.1 [pre-alpha]
     Start Date: 09.06.2016

     For the full copyright and license information, please view the LICENSE
     file that was distributed with this source code.
---------------------------------------------------

*/
#include <Arduino.h>
//
//  MenuBackend library - copyright by Alexander Brevig
// Import it from:
// https://github.com/WiringProject/Wiring/tree/master/framework/libraries/MenuBackend

#include <MenuBackend.h>
//
// Includes Libraries
#include <LiquidCrystal.h>

//
// Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(1, 2, 4, 5, 6, 7);


//
//
const int controlLedGr = 22;
const int controlLedRd = 23;

//
//

int cursorMenu = 0;

//
// Define button pins for steering controller
const int buttonPinUp = 8;
const int buttonPinDw = 9;
//
// Variables will change:
int buttonStateUp = 0;   // current state of the button
int buttonStateDw = 0;   // previous state of the button

//
// Custom character list
#include "CharList.h"
//
// Adding menu source
#include "MidMenu.h"
//
// Adding Alphine emulator
#include "EmlAlpine.h"

//
// Defines min/max menu amplitude
const int maxMenuNumber = 2;
const int minMenuNumber = 1;

//
//




void setup() {
    
    //
    //
    pinMode(controlLedGr, OUTPUT);
    pinMode(controlLedRd, OUTPUT);
    //
    // Pin button mode
    pinMode(buttonPinUp, INPUT);
    pinMode(buttonPinDw, INPUT);


    //
    // Setup menu
    setupMenu(); 
    //
    // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display }
    lcd.begin(16, 2);  
    //
    // Creates custom characters
    initChars();
    //
    // Define Alpine Pin
//    pinMode(alpinePin, OUTPUT);
    lcd.setCursor(0, 0);
    lcd.print("Welcome to Astra");
    delay(2000);
    lcd.setCursor(0, 1);
    lcd.print("Nice driving! ");
    delay(500);
    lcd.print((char)0);
    delay(2000);
    //
    // Move cursor to menu
    menu.moveDown();
    
}

void loop() {
    readButtons();  //I splitted button reading and navigation in two procedures because
    navigateMenus();  //in some situations I want to use the button for other purpose (eg. to change some settings)
    //delay(1); // if some issues appears

switch (cursorMenu){
  case 1:
  break;

  case 2:
    
  break;

  case 3:
    
  break;
    
}

}

int lastBlinkTime = 0;
int countLoopsBlink = 0;
void ledBlinkMode(){
  char current;
   if(millis() > lastBlinkTime + 500 && countLoopsBlink < 500 ){
      //digitalWrite(controlLedGr, HIGH);
      
        
        
      lastBlinkTime = millis();
   }
    if(countLoopsBlink >= 500 ) {
    //digitalWrite(controlLedGr, LOW) ;
    countLoopsBlink = 0;
    }
//        if(countLoopsBlink >= 1000){
//        countLoopsBlink = 0;
//    }
//  countLoopsBlink++;


  
}






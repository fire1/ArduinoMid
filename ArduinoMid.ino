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

//
// Includes the LiquidCrystal Library
#include <LiquidCrystal.h>
//
// Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(1, 2, 4, 5, 6, 7);
/*
    Steering wheel buttons to Sony Alpine

            resistance	voltage
---------------------------------------------------
src      	2200	    4.1
att	        4400	    3.47
off	        35	        4.98
vol down	23850	    1.47
vol up	    17000	    1.85
seek down	12000	    2.25
seek up	    8800	    2.65
back	    6660	    3
front	    33940	    1.13
bottom	    48800	    0.84
---------------------------------------------------
 Sony Voltages                                  */
const int sonySrc = 41; //4.1v
const int sonyAtt = 34;
const int sonyOff = 49;
const int sonyVolDw = 14;
const int sonyVolUp = 18;
const int sonySeekDw = 26;
const int sonySeekUp = 22;
const int sonyBack = 29;
const int sonyFront = 11;
const int sonyBottom = 8;
//
// Output for Sony Alpine emulation
const int sonyPin = A0;
const int sonyShiftPin = A2;
const int alpinePin = 3;
//
// Defines min/max menu amplitude
const int maxMenuNumber = 2;
const int minMenuNumber = 1;
//
// Define button pins for steering controller
const int buttonPinUp = 8;
const int buttonPinDw = 9;
//
// Variables will change:
int buttonStateUp = 0;   // current state of the button
int buttonStateDw = 0;   // previous state of the button
//
// Navigation state value
int navigationState = -1;


void setup() {
    //
    // Pin button mode
    pinMode(buttonPinUp, INPUT);
    pinMode(buttonPinDw, INPUT);
    //
    // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display }
    lcd.begin(16,2);
    //
    // Define Alpine Pin
    pinMode(alpinePin, OUTPUT);
}

void loop() {
    Serial.println("Entering in  loop...");
    //
    // Get navigation states
    buttonStateUp = digitalRead(buttonPinUp);
    buttonStateDw = digitalRead(buttonPinDw);
    //
    // Handle navigation buttons
    int navigation =     handleButtonNavigation(buttonStateUp,buttonStateDw);
    //
    // Switch menu
    switch ( navigation ) {
        //
        // First menu
        case 0:
            Serial.println("Entering in /0/");
            handleIndexMenu();
        break;
        //
        // Second menu
        case 1:

          break;
        //
        // Third menu
        case 2:

          break;
        //
        // MID Introduction ...
        default:
            Serial.println("Entering in  /default/ ");
            handleIntroduceMenu();
        break;
      }

}

/**
        Handle MID's steering buttons navigation
 */
int handleButtonNavigation(int stateUp,int stateDw){
    Serial.println(stateUp);
    Serial.println(stateDw);
    //
    // Cursor upper
    if (buttonStateUp == HIGH) {
        navigationState++;
    }
    //
    // Cursor down
     if (buttonStateDw == HIGH) {
        navigationState--;
     }
    //
    // Move cursor to Height number to loop chain
    if(navigationState >= maxMenuNumber){
        navigationState = minMenuNumber;
    }
    //
    // Move cursor to lower number to loop chain
    if(navigationState <= minMenuNumber){
        navigationState = maxMenuNumber;
    }
    // Debug info ...
    Serial.println(navigationState);
    //
    // Return navigation cursor
    return navigationState;
}

/**
        Handle Drive menu
 */
void handleDriveMenu(){

}
/**
        Handle Default menu
 */
void handleIndexMenu(){

}
/**
        Handle Introduce menu
 */
void handleIntroduceMenu(){
     lcd.print("Welcome... "); // Prints "Arduino" on the LCD
     delay(1500);
     lcd.clear();
     lcd.print("Opel");
     lcd.setCursor(7,0); // Sets the location at which subsequent text written to the LCD will be displayed
     lcd.print("Astra");
     delay(1500);
     lcd.setCursor(1,2);
          lcd.blink(); //Displays the blinking LCD cursor
          lcd.setCursor(2,1);
          lcd.print("Loading ...");

     delay(3000);
     lcd.clear();
     lcd.noBlink();
     delay(4000);
     lcd.noCursor(); // Hides the LCD cursor
     lcd.clear(); // Clears the LCD screen
     //
     //  When done introducing change move to nex menu
     navigationState++;
}

/**
        Handle Steering wheel media buttons
 */
void handleSteeringToSony(int voltage, boolean shift){

     // Debug info ...
     Serial.println(voltage);
     Serial.println(shift);
}



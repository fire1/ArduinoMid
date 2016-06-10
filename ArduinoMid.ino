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
// Includes Libraries
#include <MenuBackend.h>    //  MenuBackend library - copyright by Alexander Brevig
#include <LiquidCrystal.h>

//
// Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(1, 2, 4, 5, 6, 7);
//
//
static void menuChanged(MenuChangeEvent changed);
static void menuUsed(MenuUseEvent used);
//
//
MenuBackend menu = MenuBackend(menuUsed, menuChanged);
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
//
//
int lastButtonPushed = LOW;
int buttonLastDwSt = LOW;
int buttonLastUpSt = LOW;
//
// The debounce time
long debounceDelay = 370;
long lastEnterDebounceTime = 0;  // the last time the output pin was toggled
long lastEscDebounceTime = 0;  // the last time the output pin was toggled
long lastDebounceTimeDw = 0;  // the last time the output pin was toggled
long lastDebounceTimeUp = 0;  // the last time the output pin was toggled

MenuItem menu1Item1 = MenuItem("Item1");
MenuItem menu1Item2 = MenuItem("Item2");
MenuItem menu1Item3 = MenuItem("Item3");

void setup() {
    //
    // Pin button mode
    pinMode(buttonPinUp, INPUT);
    pinMode(buttonPinDw, INPUT);

    //configure menu
    menu.getRoot().add(menu1Item1);
    menu1Item1.addRight(menu1Item2).addRight(menu1Item3);
//    menu1Item1.add(menuItem1SubItem1).addRight(menuItem1SubItem2);
//    menu1Item2.add(menuItem2SubItem1).addRight(menuItem2SubItem2).addRight(menuItem3SubItem3);
    menu.toRoot();
    //
    // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display }
    lcd.begin(16, 2);
    //
    // Define Alpine Pin
//    pinMode(alpinePin, OUTPUT);
    lcd.setCursor(0,0);
    lcd.print("Astra Bertone BC");
}

void loop() {
    readButtons();  //I splitted button reading and navigation in two procedures because
    navigateMenus();  //in some situations I want to use the button for other purpose (eg. to change some settings)

    
}

/**
 * Handle Drive menu
 */
void handleDriveMenu() {

}

/**
        Handle Default menu
 */
void handleIndexMenu() {

}

/**
        Handle Introduce menu
 */
void handleIntroduceMenu() {
    lcd.print("Welcome... "); // Prints "Arduino" on the LCD
    delay(1500);
    lcd.clear();
    lcd.print("Opel");
    lcd.setCursor(7, 0); // Sets the location at which subsequent text written to the LCD will be displayed
    lcd.print("Astra");
    delay(1500);
    lcd.setCursor(1, 2);
    lcd.blink(); //Displays the blinking LCD cursor
    lcd.setCursor(2, 1);
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
void handleSteeringToSony(int voltage, boolean shift) {

    // Debug info ...
    Serial.println(voltage);
    Serial.println(shift);
}

/********************* Menu Handler **********************************************/

/**
 *
 */
  void menuUsed(MenuUseEvent used) {
    lcd.setCursor(0, 0);
    lcd.print("You used        ");
    lcd.setCursor(0, 1);
    lcd.print(used.item.getName());
    delay(3000);  //delay to allow message reading
    lcd.setCursor(0, 0);
    lcd.print("www.coagula.org");
    menu.toRoot();  //back to Main
}

/**
 *
 */
void menuChanged(MenuChangeEvent changed) {

    MenuItem newMenuItem = changed.to; //get the destination menu
    lcd.clear();
    lcd.print(newMenuItem.getName());
    
    lcd.clear();
    lcd.setCursor(0, 0); //set the start position for lcd printing to the second row


    if (newMenuItem.getName() == menu.getRoot()) {
        lcd.print("Main Menu       ");
    } else if (newMenuItem.getName() == "Item1") {
        lcd.print("Item1           ");
    } else if (newMenuItem.getName() == "Item1SubItem1") {
        lcd.print("Item1SubItem1");
    } else if (newMenuItem.getName() == "Item1SubItem2") {
        lcd.print("Item1SubItem2   ");
    } else if (newMenuItem.getName() == "Item2") {
        lcd.print("Item2           ");
    } else if (newMenuItem.getName() == "Item2SubItem1") {
        lcd.print("Item2SubItem1   ");
    } else if (newMenuItem.getName() == "Item2SubItem2") {
        lcd.print("Item2SubItem2   ");
    } else if (newMenuItem.getName() == "Item2SubItem3") {
        lcd.print("Item2SubItem3   ");
    } else if (newMenuItem.getName() == "Item3") {
        lcd.print("Item3           ");
    }
}


/**
 * 
 */
void navigateMenus() {
    MenuItem currentMenu = menu.getCurrent();

    switch (lastButtonPushed) {
        case buttonPinUp:
            menu.moveRight();
            lcd.clear();
            lcd.print("right");
            break;
        case buttonPinDw:
            menu.moveLeft();
            lcd.clear();
            lcd.print("Lect");
            break;
    }

    lastButtonPushed = 0; //reset the lastButtonPushed variable
}

/**
 * 
 */
void readButtons() {  //read buttons status
    int reading;

    int buttonDwState = LOW;             // the current reading from the input pin
    int buttonUpState = LOW;             // the current reading from the input pin

    //  Down button
    //      read the state of the switch into a local variable:
    reading = digitalRead(buttonPinUp);

    // If the switch changed, due to noise or pressing:
    if (reading != buttonLastUpSt) {
        // reset the debouncing timer
        lastDebounceTimeUp = millis();
    }

    if ((millis() - lastDebounceTimeUp) > debounceDelay) {
        // whatever the reading is at, it's been there for longer
        // than the debounce delay, so take it as the actual current state:
        buttonUpState = reading;
        lastDebounceTimeUp = millis();
    }

    // save the reading.  Next time through the loop,
    // it'll be the lastButtonState:
    buttonLastUpSt = reading;

    // Up button
    // read the state of the switch into a local variable:
    reading = digitalRead(buttonPinDw);

    // If the switch changed, due to noise or pressing:
    if (reading != buttonLastDwSt) {
        // reset the debouncing timer
        lastDebounceTimeDw = millis();
    }

    if ((millis() - lastDebounceTimeDw) > debounceDelay) {
        // whatever the reading is at, it's been there for longer
        // than the debounce delay, so take it as the actual current state:
        buttonDwState = reading;
        lastDebounceTimeDw = millis();
    }

    // save the reading.  Next time through the loop,
    // it'll be the lastButtonState:
    buttonLastDwSt = reading;
    //
    // records which button has been pressed
    if (buttonUpState == HIGH) {
        lastButtonPushed = buttonPinUp;
    } else if (buttonDwState == HIGH) {
        lastButtonPushed = buttonPinDw;
    } else {
        lastButtonPushed = 0;
    }
}



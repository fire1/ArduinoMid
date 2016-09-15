//
// Created by Angel Zaprianov on 27.6.2016 г..
//
#include "MainFunc.h"

static void menuUsed(MenuUseEvent used);

static void menuChanged(MenuChangeEvent changed);

void printNavMenuA();

void printNavMenuB();

void printNavMenuC();

void navigateMenus();

//
//
MenuBackend menu = MenuBackend(menuUsed, menuChanged);

//
//
int lastButtonPushed = HIGH;
int buttonLastDwSt = HIGH;
int buttonLastUpSt = HIGH;

bool activeStateMenu = false;

int isMainNavigationStatus = 0;
int isInSubMenu = 0;
//
// The debounce time
long debounceDelay = 350;
long lastEnterDebounceTime = 0;  // the last time the output pin was toggled
long lastEscDebounceTime = 0;  // the last time the output pin was toggled
long lastDebounceTimeDw = 0;  // the last time the output pin was toggled
long lastDebounceTimeUp = 0;  // the last time the output pin was toggled

MenuItem menuItem1 = MenuItem("Main");
MenuItem menuItem2 = MenuItem("Trip");
MenuItem menuItem3 = MenuItem("Info");

MenuItem menuItemMain1 = MenuItem("Temp");
MenuItem menuItemMain2 = MenuItem("Test");

static void setupMenu() {
    menu.getRoot().add(menuItem1).add(menuItem2).add(menuItem3);
    menuItem3.add(menuItem1); // Create Loop menu

    menuItem1.addRight(menuItemMain1).addRight(menuItemMain2);
    menuItemMain2.addRight(menuItemMain1); // loop
    //
    // Move cursor to menu
    menu.moveDown();
    menu.use();
}

/**
 * Event menu changed
 */
static void menuChanged(MenuChangeEvent changed) {


    MenuItem newMenuItem = changed.to; //get the destination menu
    lcd.clear();

    if (newMenuItem.getName() == "Main") {
        cursorMenu = 1;
        printNavMenuA();
//        isInSubMenu = 0;
    }
    else if (newMenuItem.getName() == "Temp") {

    }
    else if (newMenuItem.getName() == "Tests") {
        cursorMenu = 5;
    }
    else if (newMenuItem.getName() == "Trip") {
        printNavMenuB();
        cursorMenu = 2;
    }
    else if (newMenuItem.getName() == "Item2SubItem1") {
        lcd.print("Item2SubItem1   ");
    }
    else if (newMenuItem.getName() == "Item2SubItem2") {
        lcd.print("Item2SubItem2   ");
    }
    else if (newMenuItem.getName() == "Item2SubItem3") {
        lcd.print("Item2SubItem3   ");
    }
    else if (newMenuItem.getName() == "Info") {
        printNavMenuC();
        cursorMenu = 3;
    }
    else {
        lcd.print(newMenuItem.getName());
    }
}

/**
 * Read pin button states
 */
void readButtons(int buttonPinUp, int buttonPinDw) {  //read buttons status
    int reading;
    int buttonDwState = HIGH;             // the current reading from the input pin
    int buttonUpState = HIGH;             // the current reading from the input pin

    //  Up button
    //      read the state of the switch into a local variable:
    reading = digitalRead(buttonPinUp);

    if (reading != buttonLastUpSt) {
        lastDebounceTimeUp = millis();
    }

    if (millis() > debounceDelay + lastDebounceTimeUp && !activeStateMenu) {
        buttonUpState = reading;
        lastDebounceTimeUp = millis();
    }

    buttonLastUpSt = reading;

    reading = digitalRead(buttonPinDw);

    if (reading != buttonLastDwSt) {
        lastDebounceTimeDw = millis();
    }

    if (millis()  > debounceDelay + lastDebounceTimeDw && !activeStateMenu) {
        buttonDwState = reading;
        lastDebounceTimeDw = millis();
    }

    buttonLastDwSt = reading;
    //
    // records which button has been pressed
    if (buttonUpState == LOW) {
        lastButtonPushed = LOW;
    }
    else if (buttonDwState == LOW) {
        lastButtonPushed = LOW;
    } else {
        lastButtonPushed = LOW;
    }
}

int lastNavInfoTime = 0;

int isInfoTimeMenu() {

    while (1 > 0) {
        if (millis() > lastNavInfoTime) {
            lastNavInfoTime = millis();
            return 1;
        }
        if (millis() > lastNavInfoTime + 2000) {
            return 0;
        }
        return 0;
    }
}

/**
 * Resolve navigation between button press
 */
void navigateMenu() {

    if (isMainNavigationStatus == 0) {
        MenuItem currentMenu = menu.getCurrent();

        switch (lastButtonPushed) {
            case BTN_PIN_UP :
                if (isInSubMenu == 0) {
                    menu.moveDown();
                    menu.use();
                }
                else {
                    menu.moveRight();
                    menu.use();
                }
                break;
            case BTN_PIN_DW:
                if (isInSubMenu == 0) {
                    menu.moveRight();
                    menu.use();
                    isInSubMenu = 1;
                }
                else {
                    menu.moveBack();
                    menu.use();
                }
                break;
        }
    }
    lastButtonPushed = 0; //reset the lastButtonPushed variable
}

void printNavMenuA() {
    lcd.setCursor(13, 2);
    lcd.print((char) 3);
    lcd.print((char) 2);
    lcd.print((char) 2);
    lcd.setCursor(0, 0);

}

void printNavMenuB() {
    lcd.setCursor(13, 2);
    lcd.print((char) 2);
    lcd.print((char) 3);
    lcd.print((char) 2);
    lcd.setCursor(0, 0);

}

void printNavMenuC() {
    lcd.setCursor(13, 2);
    lcd.print((char) 2);
    lcd.print((char) 2);
    lcd.print((char) 3);
    lcd.setCursor(0, 0);

}

static void menuUsed(MenuUseEvent used) {

    lcd.setCursor(0, 0);
    //lcd.print("You are in:       ");
    //lcd.setCursor(0, 1);
    lcd.print(used.item.getName());
    delay(250);
    lcd.print(" Screen");
    delay(800);  //delay to allow message reading
    lcd.setCursor(0, 0);
    lcd.clear();
    //menu.toRoot();  //back to Main
}
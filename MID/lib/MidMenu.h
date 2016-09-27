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

void printNavMenuD();

//
//
MenuBackend menu = MenuBackend(menuUsed, menuChanged);

//
//
int lastButtonPushed = LOW;
int buttonLastDwSt = LOW;
int buttonLastUpSt = LOW;

bool activeStateMenu = false;

int isMainNavigationStatus = 0;
int isInSubMenu = 0;
//
// The debounce time
long debounceDelay = 43;
long lastEnterDebounceTime = 0;  // the last time the output pin was toggled
long lastEscDebounceTime = 0;  // the last time the output pin was toggled
long lastDebounceTimeDw = 0;  // the last time the output pin was toggled
long lastDebounceTimeUp = 0;  // the last time the output pin was toggled

MenuItem menuItem1 = MenuItem("Main");
MenuItem menuItem2 = MenuItem("Trip");
MenuItem menuItem3 = MenuItem("Fuel");
MenuItem menuItem4 = MenuItem("Average");

MenuItem menuItemMain1 = MenuItem("Temp");
MenuItem menuItemMain2 = MenuItem("Test");

static void setupMenu() {
    menu.getRoot().add(menuItem1).add(menuItem2).add(menuItem3).add(menuItem4);
    menuItem4.add(menuItem1); // Create Loop menu

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

    }
    else if (newMenuItem.getName() == "Temp") {

    }
    else if (newMenuItem.getName() == "Average") {
        printNavMenuD();
        cursorMenu = 4;
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
    else if (newMenuItem.getName() == "Fuel") {
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


/**
 * Lower the code
 */
void readButtons(uint8_t buttonPinUp, uint8_t buttonPinDw) {

    lastButtonPushed = LOW;

    //
    // Detect up state button
    if (!digitalRead(buttonPinUp) == HIGH) {

        delay(10);
        if (!digitalRead(buttonPinUp) == HIGH) {
            lastButtonPushed = buttonPinUp;
        }
    }

    //
    // Detect down state button
    if (!digitalRead(buttonPinDw) == HIGH) {
        delay(10);
        if (!digitalRead(buttonPinDw) == HIGH) {
            delay(700);
            if (!digitalRead(buttonPinDw) == HIGH && isInSubMenu == 0) {
                //
                // Enter inner level menu
//                isInSubMenu = 1;
                tone(ADT_ALR_PIN, 400, 200);
                //
                // Exit inner level menu
            } else if (!digitalRead(buttonPinDw) == HIGH && isInSubMenu == 1) {
                isInSubMenu = 0;
                delay(10);
                tone(ADT_ALR_PIN, 1600, 200);
            } else {
                lastButtonPushed = buttonPinDw;
            }
        }
    }
}

int lastSubMenuState = 0;

/**
 * Resolve navigation between button press
 */
void navigateMenu() {

    if (isMainNavigationStatus == 0) {
        MenuItem currentMenu = menu.getCurrent();
        Serial.print("Last button:");
        Serial.print(lastButtonPushed);
        Serial.print("Is sub menu:");
        Serial.print(isInSubMenu);
        Serial.print("\n");

        switch (lastButtonPushed) {
            case BTN_PIN_UP :
                if (isInSubMenu == 0) {
                    menu.moveDown();
                    menu.use();
                } else {
                    menu.moveRight();
                    menu.use();
                }
                break;
            case BTN_PIN_DW:
                if (lastSubMenuState == 1 && isInSubMenu == 0) {
//                    menu.use(menuItem1.getShortkey());
//                    menu.moveBack();
//                    menu.moveBack();
                    menu.use();
                } else if (isInSubMenu == 0) {
                    menu.moveUp();
                    menu.use();
                } else {
                    menu.moveLeft();
                    menu.use();
                }
                break;
        }
    }
    lastSubMenuState = isInSubMenu;
    lastButtonPushed = 0; //reset the lastButtonPushed variable
}

void printNavMenuA() {
    lcd.setCursor(12, 2);
    lcd.write((uint8_t) 3);
    lcd.write((uint8_t) 2);
    lcd.write((uint8_t) 2);
    lcd.write((uint8_t) 2);
    lcd.setCursor(0, 0);

}

void printNavMenuB() {
    lcd.setCursor(12, 2);
    lcd.write((uint8_t) 2);
    lcd.write((uint8_t) 3);
    lcd.write((uint8_t) 2);
    lcd.write((uint8_t) 2);
    lcd.setCursor(0, 0);

}

void printNavMenuC() {
    lcd.setCursor(12, 2);
    lcd.write((uint8_t) 2);
    lcd.write((uint8_t) 2);
    lcd.write((uint8_t) 3);
    lcd.write((uint8_t) 2);
    lcd.setCursor(0, 0);

}

void printNavMenuD() {
    lcd.setCursor(12, 2);
    lcd.write((uint8_t) 2);
    lcd.write((uint8_t) 2);
    lcd.write((uint8_t) 2);
    lcd.write((uint8_t) 3);
    lcd.setCursor(0, 0);

}

static void menuUsed(MenuUseEvent used) {

    lcd.clear();
    lcd.setCursor(0, 0);
    //lcd.print("You are in:       ");
    //lcd.setCursor(0, 1);
    lcd.print(used.item.getName());
    delay(250);
    lcd.print(" Menu");
    delay(500);  //delay to allow message reading
    lcd.setCursor(0, 0);
    lcd.clear();
    //menu.toRoot();  //back to Main
}


void _readButtons(uint8_t buttonPinUp, uint8_t buttonPinDw) {  //read buttons status
    int reading;
    int buttonDwState = LOW;             // the current reading from the input pin
    int buttonUpState = LOW;             // the current reading from the input pin

    //  Up button
    //      read the state of the switch into a local variable:
    reading = !digitalRead(buttonPinUp);

    if (reading != buttonLastUpSt) {
        lastDebounceTimeUp = millis();
    }

    if ((millis() - lastDebounceTimeUp) > debounceDelay && !activeStateMenu) {
        buttonUpState = reading;
        lastDebounceTimeUp = millis();
    }

    buttonLastUpSt = reading;

    reading = !digitalRead(buttonPinDw);

    if (reading != buttonLastDwSt) {
        lastDebounceTimeDw = millis();
    }

    if ((millis() - lastDebounceTimeDw) > debounceDelay && !activeStateMenu) {
        buttonDwState = reading;
        lastDebounceTimeDw = millis();
    }

    buttonLastDwSt = reading;
    //
    // records which button has been pressed
    if (buttonUpState == HIGH) {

        lastButtonPushed = buttonPinUp;
    }
    else if (buttonDwState == HIGH) {

        lastButtonPushed = buttonPinDw;
    }
    else {
        lastButtonPushed = LOW;
    }
}
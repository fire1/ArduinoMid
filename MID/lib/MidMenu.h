//
// Created by Angel Zaprianov on 27.6.2016 г..
//


#ifndef ARDUINO_MID_MENU
#define ARDUINO_MID_MENU
//
//  MenuBackend library - copyright by Alexander Brevig
// Import it from:
// https://github.com/WiringProject/Wiring/tree/master/framework/libraries/MenuBackend
//
// And good example can be found here:
// https://codebender.cc/sketch:37125#MenuBackend_sample.ino
#include <MenuBackend.h>

#include "MainFunc.h"
#include "CarSens.h"
#include "WhlSens.h"


static void menuUsed(MenuUseEvent used);

static void menuChanged(MenuChangeEvent changed);

//
//
MenuBackend menu = MenuBackend(menuUsed, menuChanged);
//
//
int lastButtonPushed = LOW;
int isMainNavigationStatus = 0;
int isInSubMenu = 0;

/* MID menu mMap



Main        Trip-------------------------------Trip----------------------------------------------------Fuel
        |                                   |
        Item1SubItem1-----Item1SubItem2     Item2SubItem1------Item2SubItem2------Item2SubItem3

 */

//
// Main menu
MenuItem mainMenu = MenuItem("Main", 1);


MenuItem tripMenu = MenuItem("Trip", 2);
MenuItem fuelMenu = MenuItem("Fuel", 3);
MenuItem averMenu = MenuItem("Average", 4);

//
// Sub menu for MAIN
MenuItem dshBoardMenu = MenuItem("Panel", 11);
MenuItem testingsMenu = MenuItem("Test", 12);
//
// Sub menu for fuel
MenuItem FuelTankMenu = MenuItem("F-Tanks");

static void setupMenu() {

    /*
     *
      menu.getRoot().add(menu1Item1);
      menu1Item1.addRight(menu1Item2).addRight(menu1Item3);
      menu1Item1.add(menuItem1SubItem1).addRight(menuItem1SubItem2);
      menu1Item2.add(menuItem2SubItem1).addRight(menuItem2SubItem2).addRight(menuItem3SubItem3);

     */

    menu.getRoot().add(mainMenu).add(tripMenu).add(fuelMenu).add(averMenu);
    averMenu.add(mainMenu); // Create Loop menu

    //
    // Main menu layers
    mainMenu.addRight(dshBoardMenu).addRight(testingsMenu);
    testingsMenu.addRight(dshBoardMenu); // loop
    //
    // add return menu
    dshBoardMenu.add(mainMenu);
    testingsMenu.add(mainMenu);
    //
    // Fuel  layers
    fuelMenu.addRight(FuelTankMenu);
    FuelTankMenu.add(fuelMenu);
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
    } else if (newMenuItem.getName() == "Panel") {
        cursorMenu = 11;
    } else if (newMenuItem.getName() == "Test") {
        cursorMenu = 12;
    } else if (newMenuItem.getName() == "Average") {

        cursorMenu = 4;
    } else if (newMenuItem.getName() == "Trip") {
        cursorMenu = 2;
//    } else if (newMenuItem.getName() == "Item2SubItem1") {
//        lcd.print("Item2SubItem1   ");
//    }
//    else if (newMenuItem.getName() == "Item2SubItem2") {
//        lcd.print("Item2SubItem2   ");
//    }
//    else if (newMenuItem.getName() == "Item2SubItem3") {
//        lcd.print("Item2SubItem3   ");
    } else if (newMenuItem.getName() == "Fuel") {
        cursorMenu = 3;
    } else if (newMenuItem.getName() == "F-Tanks") {
        cursorMenu = 32;
    } else {
        lcd.print(newMenuItem.getName());
    }
}

/**
 * Read pin button states
 */

unsigned long lastButtonPress = 0;

unsigned long entryDownState = 0;

boolean secondTone = 0;

/**
 * Lower the code
 */
void readButtons(uint8_t buttonPinUp, uint8_t buttonPinDw) {

    int OUTER_BUTTON_STATE = 0;

    lastButtonPushed = LOW;
    unsigned long curTime = millis();
    //
    // Detect up state button
    if (!digitalRead(buttonPinUp) == HIGH) {

        if (ampInt.isLow() && !digitalRead(buttonPinUp) == HIGH) {
            lastButtonPushed = buttonPinUp;
        }
    }

    //
    // Detect down state button
    if (!digitalRead(buttonPinDw) == HIGH) {
        //
        // Controlling start of press state
        if (entryDownState == 0) {
            whlSens.disable();
            entryDownState = millis();
        }
        //
        // Hold
        if (entryDownState + 1000 < millis() && !digitalRead(buttonPinDw) == HIGH) {
            //
            // If is still high state [pressed]
            if (!digitalRead(buttonPinDw) == HIGH) {
                //
                // Reset entry down state
                entryDownState = 0;

                /*********** [SHORTCUTS] *********** *********** *********** *********** START ***********/
                // Steering button is pressed
                if (whlSens.getCurrentState() == whlSens.STR_BTN_ATT) {
                    TTL_TLC = 0;
                    TTL_TTD = 0;
                    tone(ADT_ALR_PIN, 1000, 50);
                    delay(50);
                    tone(ADT_ALR_PIN, 1000, 50);
                    delay(50);
                    whlSens.enable();
                    return;
                }
                //
                // Change Speed alarm Up
                if (whlSens.getCurrentState() == whlSens.STR_BTN_VLU) {
                    carSens.speedingAlarmsUp();
                    tone(ADT_ALR_PIN, 800, 50);
                    delay(50);
                    tone(ADT_ALR_PIN, 1600, 80);
                    delay(80);
                    whlSens.enable();
                    return;
                }
                //
                // Change Speed alarm Down
                if (whlSens.getCurrentState() == whlSens.STR_BTN_VLD) {
                    carSens.speedingAlarmsDw();
                    tone(ADT_ALR_PIN, 1600, 50);
                    delay(50);
                    tone(ADT_ALR_PIN, 800, 80);
                    delay(80);
                    whlSens.enable();
                    return;
                }
                /*********** [SHORTCUTS] *********** *********** *********** *********** END   ***********/
                //
                // Check for subMenu if not got inner level entry
                if (isInSubMenu == 0) {
                    //
                    // Enter inner level menu
                    isInSubMenu = 1;
                    tone(ADT_ALR_PIN, 400, 100);
                    //
                    // Exit inner level menu
                } else if (isInSubMenu == 1) {
                    isInSubMenu = 0;
                    tone(ADT_ALR_PIN, 400, 50);
                    secondTone = 1;
                }
            } else {
                //
                // Perform button is released action
                lastButtonPushed = buttonPinDw;
                //
                // Reset entry down state
                entryDownState = 0;
            }
        }
//    } else if (entryDownState > 0  && !digitalRead(buttonPinDw) == LOW) {
//        //
//        // Perform button is released action
//        lastButtonPushed = buttonPinDw;
//        //
//        // Reset entry down state
//        entryDownState = 0;
//        whlSens.enable(); // unlock radio
//        //
//        //
    } else { // <- deprecated
        entryDownState = 0;
        whlSens.enable(); // unlock radio
    }

    if (ampInt.isSec() && secondTone) {
        tone(ADT_ALR_PIN, 800, 50);
        secondTone = 0;
    }

}

char lastMainMenuState = 0;

/**
 * Resolve navigation between button press
 */
void navigateMenu() {

    if (isMainNavigationStatus == 0) {
        MenuItem currentMenu = menu.getCurrent();

        switch (lastButtonPushed) {
            //
            // UP button
            case BTN_PIN_UP:
                if (isInSubMenu == 0) {
                    menu.moveDown();
                    menu.use();
                } else {
                    menu.moveRight();
                    menu.use();
                }
                break;

                //
                // Down button
            case BTN_PIN_DW:
                if (lastMainMenuState != 0 && isInSubMenu == 0) {
                    menu.moveBack();
                    menu.use();
                } else if (isInSubMenu == 0) {
                    menu.moveUp();
                    menu.use();
                } else {
                    lastMainMenuState = currentMenu.getShortkey();
                    menu.moveLeft();
                    menu.use();
                }
                break;
        }
    }
//    lastSubMenuState = isInSubMenu;
    lastButtonPushed = 0; //reset the lastButtonPushed variable
}


static void menuUsed(MenuUseEvent used) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(used.item.getName());
    delay(100);
    lcd.print(" Menu");
    delay(500);  //delay to allow message reading
    lcd.setCursor(0, 0);
    lcd.clear();
    //
    // fixes value peek
    // reset base global vars
    carSens.clearBaseData();
}

#endif
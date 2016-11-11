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


class MidMenu {

    MenuBackend menu;
    MenuItem
    //
    // Main menu
            mainMenu,
            dshBoardMenu,
            testingsMenu,
    //
    // Trip menu
            tripMenu,
    //
    // Averages
            averMenu,
    //
    // Fuel tank
            fuelMenu,
            FuelTankMenu;

public:

    MidMenu();

    void setup(uint8_t pinUp, uint8_t pinDw);

    void menuUsed(MenuUseEvent used);

    void menuChanged(MenuChangeEvent changed);

    void listener(int &cursor);

    int cursorMenu;

    int getCursorMenu() {
        return cursorMenu;
    }

private:

    uint8_t btnPinUp, btnPinDw;

    unsigned long lastButtonPress = 0;
    unsigned long entryDownState = 0;
    boolean secondTone = 0;
    //
    //
    int lastButtonPushed = LOW;
    int isMainNavigationStatus = 0;
    int isInSubMenu = 0;

    char lastMainMenuState = 0;

    void buttons(uint8_t buttonPinUp, uint8_t buttonPinDw);

    void shortcuts();

    void navigate();
};
/**
 * constructor
 */
MidMenu::MidMenu() :
        menu(MenuBackend(this->menuUsed, this->menuChanged)),
        mainMenu(MenuItem("Main", 1)),
        dshBoardMenu(MenuItem("Panel", 1)),
        testingsMenu(MenuItem("Test", 1)),
        tripMenu(MenuItem("Trip", 1)),
        averMenu(MenuItem("Average", 1)),
        fuelMenu(MenuItem("Fuel", 1)),
        FuelTankMenu(MenuItem("Tanks", 1)) {

}
/**
 *  Setup menu
 */
void MidMenu::setup(uint8_t pinUp, uint8_t pinDw) {

    btnPinUp = pinUp;
    btnPinDw = pinDw;
    //
    // Pin button mode
    pinMode(btnPinUp, INPUT);
    pinMode(btnPinDw, INPUT);

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
void MidMenu::menuChanged(MenuChangeEvent changed) {

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
    } else if (newMenuItem.getName() == "Fuel") {
        cursorMenu = 3;
    } else if (newMenuItem.getName() == "F-Tanks") {
        cursorMenu = 32;
    } else {
        lcd.print(newMenuItem.getName());
    }
}

/**
 * Event use changed
 */
void MidMenu::menuUsed(MenuUseEvent used) {
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


/**
 * Lower the code
 */
void MidMenu::listener(int &cursor) {
    buttons(btnPinUp, btnPinDw);
    navigate();
    cursorMenu = cursor;
}
/**
 * buttons
 */
void MidMenu::buttons(uint8_t buttonPinUp, uint8_t buttonPinDw) {

    lastButtonPushed = LOW;
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

                shortcuts();
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

    } else { // <- deprecated
        entryDownState = 0;
        whlSens.enable(); // unlock radio
    }

    if (ampInt.isSec() && secondTone) {
        tone(ADT_ALR_PIN, 800, 50);
        secondTone = 0;
    }

}
/**
 * Shortcuts
 */
void MidMenu::shortcuts() {
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
}


/**
 * Resolve navigation between button press
 */
void MidMenu::navigate() {

    if (isMainNavigationStatus == 0) {
        if (lastButtonPushed == btnPinUp) {
            if (isInSubMenu == 0) {
                menu.moveDown();
                menu.use();
            } else {
                menu.moveRight();
                menu.use();
            }
        }
        if (lastButtonPushed == btnPinDw) {
            if (lastMainMenuState != 0 && isInSubMenu == 0) {
                menu.moveBack();
                menu.use();
            } else if (isInSubMenu == 0) {
                menu.moveUp();
                menu.use();
            }
        }
    }
    lastButtonPushed = 0; //reset the lastButtonPushed variable
}


#endif
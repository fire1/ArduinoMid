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


#ifndef MENU_ENTRY
#define MENU_ENTRY 0
#endif

#define MENU_NAME_1 "Main"
#define MENU_NAME_11 "Panel"
#define MENU_NAME_12 "Test"
//
//
#define MENU_NAME_2 "Trip"
//
//
#define MENU_NAME_3 "Fuel"
#define MENU_NAME_31 "Fuel Level"
//
//
#define MENU_NAME_4 "Average"


static void MidMenu_menuUsed(MenuUseEvent used);

static void MidMenu_menuChanged(MenuChangeEvent changed);

/**
 *
 */
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
    const static int MENU_ENTER = MENU_ENTRY;

    //
    // External changer var
    const static char *where;

    //
    // External changer var
    int static cursorMenu;

    void setup(uint8_t pinUp, uint8_t pinDw);

    void listener(int &cursor);

    int getCursorMenu() {
        return MidMenu::cursorMenu;
    }


    void setCursor(int val) {
        MidMenu::cursorMenu = val;
    }

    void display();

    MidMenu();

private:

    //
    // Saves cursor between changes
    int savedCursor;

    const char *activeMenu;

    uint8_t btnPinUp, btnPinDw;

    unsigned long entryDownState = 0;
    boolean secondTone = 0;
    //
    //
    int lastButtonPushed = LOW;
    int isMainNavigationStatus = 0;
    int isInSubMenu = 0;

    char lastMainMenuState = 0;

    bool enterDisplay = 0;

    void buttons(uint8_t buttonPinUp, uint8_t buttonPinDw);

    void shortcuts();

    void navigate();


};

/***********************************************************************************************
 * ########################################################################################### *
 * ########################################################################################### *
 *                                                                                             *
 *                                   CPP part of file                                          *
 *                                                                                             *
 * ########################################################################################### *
 * ########################################################################################### *
 ***********************************************************************************************/


/**
 * constructor
 */
MidMenu::MidMenu() :
//
// Define menus
        menu(MenuBackend(MidMenu_menuUsed, MidMenu_menuChanged)),
        //
        // Main menu initialization
        mainMenu(MenuItem(MENU_NAME_1)),
        dshBoardMenu(MenuItem(MENU_NAME_11)),
        testingsMenu(MenuItem(MENU_NAME_12)),
        //
        // Trip menu initialization
        tripMenu(MenuItem(MENU_NAME_2)),
        averMenu(MenuItem(MENU_NAME_4)),
        fuelMenu(MenuItem(MENU_NAME_3)),
        FuelTankMenu(MenuItem(MENU_NAME_31)) {

}

/**
 * Event menu changed
 */
static void MidMenu_menuChanged(MenuChangeEvent changed) {

    MenuItem curMenuItem = changed.to; //get the destination menu

    if (curMenuItem.getName() == MENU_NAME_1) {
        MidMenu::cursorMenu = 1;
    } else if (curMenuItem.getName() == MENU_NAME_11) {
        MidMenu::cursorMenu = 11;
    } else if (curMenuItem.getName() == MENU_NAME_12) {
        MidMenu::cursorMenu = 12;
    } else if (curMenuItem.getName() == MENU_NAME_2) {
        MidMenu::cursorMenu = 2;
    } else if (curMenuItem.getName() == MENU_NAME_3) {
        MidMenu::cursorMenu = 3;
    } else if (curMenuItem.getName() == MENU_NAME_31) {
        MidMenu::cursorMenu = 32;
    } else if (curMenuItem.getName() == MENU_NAME_4) {
        MidMenu::cursorMenu = 4;
    }
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
    //
    //
    menu.getRoot().add(mainMenu).add(tripMenu).add(fuelMenu).add(averMenu);
    averMenu.add(mainMenu); // add last menu to create a Loop menu
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
 * Define static cursor
 */
int MidMenu::cursorMenu = 0;


const char *MidMenu::where = "";

/**
 * Event use changed
 */
static void MidMenu_menuUsed(MenuUseEvent used) {
    //
    // Pass argument to class
    MidMenu::where = used.item.getName();
}


/**
 * listen menu
 */
void MidMenu::listener(int &cursor) {
    buttons(btnPinUp, btnPinDw);
    navigate();
    //
    //
    if (MidMenu::where != activeMenu) {
        //
        // Keep cursor in save place
        savedCursor = MidMenu::cursorMenu;
        //
        // Change menu to show info
        cursor = MENU_ENTER;
    } else {
        cursor = MidMenu::cursorMenu;
    }
}

/**
 * Display menu entry
 */
void MidMenu::display() {

    if (ampInt.isSec()) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" ->");
    }

    if (ampInt.isMid()) {
        lcd.setCursor(0, 0);
        lcd.print("-> ");
    }

    if (!enterDisplay && ampInt.isBig()) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" ->");
        //
        //
        lcd.print(MidMenu::where);
        lcd.setCursor(0, 0);
        lcd.clear();
        enterDisplay = 1;
    }

    if (enterDisplay && ampInt.isBig()) {
        //
        // fixes value peek
        // reset base global vars
        carSens.clearBaseData();
        activeMenu = MidMenu::where;
        enterDisplay = 0;
        MidMenu::cursorMenu = savedCursor;
        lcd.clear();
    }

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
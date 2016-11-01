//
// Created by Angel Zaprianov on 1.11.2016 г..
//

#ifndef ARDUINOMID_MIDMENU_H
#define ARDUINOMID_MIDMENU_H

#include <Arduino.h>
#include <MenuBackend.h>
#include <LiquidCrystal.h>
#include "TimeAmp.h"
#include "WhlSens.h"
#include "CarSens.h"


class MidMenu {


private:
    TimeAmp *_amp;
    WhlSens *_whl;
    CarSens *_car;
//    MenuBackend menu;
    LiquidCrystal *_lcd;


    MenuBackend *menu;


    MenuItem menuMain = MenuItem("Main", 1);
    MenuItem menuTrip = MenuItem("Trip", 2);
    MenuItem menuFuel = MenuItem("Fuel", 3);
    MenuItem menuAverage = MenuItem("Average", 4);
    //
    // Sub menu for MAIN
    MenuItem menuDashboard = MenuItem("Dashboard", 11);
    MenuItem menuInstruments = MenuItem("Instruments", 12);


    //
    // Buttons
    uint8_t pinUp, pinDw,
            pinTn;

    //
    // Button handlers
    int lastButtonPushed = LOW;
    int isMainNavigationStatus = 0;
    int isInSubMenu = 0;
    int lastMainMenuState;
    //
    // Current position of menu
    int cursorMenu;
    //
    // Hold press handler
    unsigned long entryDownState = 0;

    //
    //
    void speedAlarmShortcuts();

    void navigateMenu();

public:
    MidMenu(LiquidCrystal *lcd, CarSens *car, TimeAmp *amp, WhlSens *whl);

//    void setMenuBackend();

    void setup(uint8_t pinBtnUp, uint8_t pinBtnDw, uint8_t pinAlarm);

    void menuUsed(MenuUseEvent used) {

    }

    void menuChanged(MenuChangeEvent changed);

    void listener();

    int getCursor() {
        return cursorMenu;
    }

};


/**
 * Class constructor
 */
MidMenu::MidMenu(LiquidCrystal *lcd, CarSens *car, TimeAmp *amp, WhlSens *whl) :
        menu(new MenuBackend((cb_use) &MidMenu::menuUsed, (cb_change) &MidMenu::menuChanged)) {


//    this->menu = new MenuBackend(menuUsed,menuChanged);


    _lcd = lcd;
    _amp = amp;
    _whl = whl;
    _car = car;

//    setMenuBackend();
    menu->getRoot().add(menuMain).add(menuTrip).add(menuFuel).add(menuAverage);
    menuAverage.add(menuMain); // Create Loop menu

    //
    // Main menu layers
    menuMain.addRight(menuDashboard).addRight(menuInstruments);
    menuInstruments.addRight(menuDashboard); // loop
    menuDashboard.add(menuMain);
    menuInstruments.add(menuMain);

}
/**
 * Set MenuBackend
 */

/**
 * Setup menu
 */
void MidMenu::setup(uint8_t pinBtnUp, uint8_t pinBtnDw, uint8_t pinAlarm) {
    pinUp = pinBtnUp;
    pinDw = pinBtnDw;
    pinTn = pinAlarm;
}

/**
 * Menu change handler
 */
void MidMenu::menuChanged(MenuChangeEvent changed) {
    MenuItem newMenuItem = changed.to; //get the destination menu
    _lcd->clear();

    if (newMenuItem.getName() == "Main") {
        cursorMenu = 1;
//        printNavMenuA();

    } else if (newMenuItem.getName() == "Panel") {
        cursorMenu = 11;
    } else if (newMenuItem.getName() == "Test") {
        cursorMenu = 12;
    } else if (newMenuItem.getName() == "Average") {
//        printNavMenuD();
        cursorMenu = 4;
    } else if (newMenuItem.getName() == "Trip") {
//        printNavMenuB();
        cursorMenu = 2;
    } else if (newMenuItem.getName() == "Item2SubItem1") {
        lcd.print("Item2SubItem1   ");
    }
    else if (newMenuItem.getName() == "Item2SubItem2") {
        lcd.print("Item2SubItem2   ");
    }
    else if (newMenuItem.getName() == "Item2SubItem3") {
        lcd.print("Item2SubItem3   ");
    }
    else if (newMenuItem.getName() == "Fuel") {
//        printNavMenuC();
        cursorMenu = 3;
    }
    else {
        _lcd->print(newMenuItem.getName());
    }
}

void MidMenu::speedAlarmShortcuts() {

}

void MidMenu::listener() {

    int OUTER_BUTTON_STATE = 0;

    lastButtonPushed = LOW;
    unsigned long curTime = millis();
    //
    // Detect up state button
    if (!digitalRead(pinUp) == HIGH) {

        if (_amp->isLow() && !digitalRead(pinUp) == HIGH) {
            lastButtonPushed = pinUp;
        }
    }

    //
    // Detect down state button
    if (!digitalRead(pinDw) == HIGH) {
        //
        // Controlling start of press state
        if (entryDownState == 0) {
            _whl->disable();
            entryDownState = millis();
        }
        //
        // Hold
        if (entryDownState + 1200 < millis() && !digitalRead(pinDw) == HIGH) {
            //
            // If is still high state [pressed]
            if (!digitalRead(pinDw) == HIGH) {
                //
                // Reset entry down state
                entryDownState = 0;

                /*********** [SHORTCUTS] *********** *********** *********** *********** START ***********/
                // Steering button is pressed
                if (_whl->getCurrentState() == _whl->STR_BTN_ATT) {
                    tone(pinTn, 1000, 50);
                    delay(50);
                    tone(pinTn, 1000, 50);
                    delay(50);
                    _whl->enable();
                    return;
                }
                //
                // Change Speed alarm Up
                if (_whl->getCurrentState() == _whl->STR_BTN_VLU) {
                    _car->speedingAlarmsUp();
                    tone(pinTn, 800, 50);
                    delay(50);
                    tone(pinTn, 1600, 80);
                    delay(80);
                    _whl->enable();
                    return;
                }
                //
                // Change Speed alarm Down
                if (_whl->getCurrentState() == _whl->STR_BTN_VLD) {
                    _car->speedingAlarmsDw();
                    tone(pinTn, 1600, 50);
                    delay(50);
                    tone(pinTn, 800, 80);
                    delay(80);
                    _whl->enable();
                    return;
                }
                /*********** [SHORTCUTS] *********** *********** *********** *********** END   ***********/
                //
                // Check for subMenu if not got inner level entry
                if (isInSubMenu == 0) {
                    //
                    // Enter inner level menu
                    isInSubMenu = 1;
                    tone(pinTn, 400, 100);
                    //
                    // Exit inner level menu
                } else if (isInSubMenu == 1) {
                    isInSubMenu = 0;
                    tone(pinTn, 1600, 100);
                }
            } else {
                //
                // Perform button is released action
                lastButtonPushed = pinDw;
                //
                // Reset entry down state
                entryDownState = 0;
            }
        }
    } else { // <- deprecated
        entryDownState = 0;
        _whl->enable(); // unlock radio
    }

    navigateMenu();
}


/**
 * Resolve navigation between button press
 */
void MidMenu::navigateMenu() {

    if (isMainNavigationStatus == 0) {
        MenuItem currentMenu = menu->getCurrent();

        if (lastButtonPushed == pinUp) {
            if (isInSubMenu == 0) {
                menu->moveDown();
                menu->use();
            } else {
                menu->moveRight();
                menu->use();
            }
        } else if (lastButtonPushed == pinDw) {
            if (lastMainMenuState != 0 && isInSubMenu == 0) {

                menu->moveBack();
                menu->use();

            } else if (isInSubMenu == 0) {
                menu->moveUp();
                menu->use();
            } else {
                lastMainMenuState = currentMenu.getShortkey();
                menu->moveLeft();
                menu->use();
            }
        }
    }
//    lastSubMenuState = isInSubMenu;
    lastButtonPushed = 0; //reset the lastButtonPushed variable
}

#endif //ARDUINOMID_MIDMENU_H

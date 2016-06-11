
static void menuUsed(MenuUseEvent used);
static void menuChanged(MenuChangeEvent changed);

MenuBackend  menu = MenuBackend(menuUsed, menuChanged);

//
//
int lastButtonPushed = LOW;
int buttonLastDwSt = LOW;
int buttonLastUpSt = LOW;

//
// The debounce time
long debounceDelay = 30;
long lastEnterDebounceTime = 0;  // the last time the output pin was toggled
long lastEscDebounceTime = 0;  // the last time the output pin was toggled
long lastDebounceTimeDw = 0;  // the last time the output pin was toggled
long lastDebounceTimeUp = 0;  // the last time the output pin was toggled

MenuItem menu1Item1 = MenuItem("Item1");
// MenuItem menuItem1SubItem1 = MenuItem("Item1SubItem1");
// MenuItem menuItem1SubItem2 = MenuItem("Item1SubItem2");
MenuItem menu1Item2 = MenuItem("Item2");
// MenuItem menuItem2SubItem1 = MenuItem("Item2SubItem1");
// MenuItem menuItem2SubItem2 = MenuItem("Item2SubItem2");
// MenuItem menuItem3SubItem3 = MenuItem("Item2SubItem3");
MenuItem menu1Item3 = MenuItem("Item3");

void setupMenu(){
      //configure menu
    menu.getRoot().add(menu1Item1);
    menu1Item1.add(menu1Item2).add(menu1Item3);
//    menu1Item1.add(menuItem1SubItem1).addRight(menuItem1SubItem2);
//    menu1Item2.add(menuItem2SubItem1).addRight(menuItem2SubItem2).addRight(menuItem3SubItem3);
    //menu.toRoot();
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



/**
 *
 */
void menuChanged(MenuChangeEvent changed) {

    MenuItem newMenuItem = changed.to; //get the destination menu
    lcd.clear();
    lcd.print(newMenuItem.getName());

    lcd.clear();
    lcd.setCursor(0, 0); //set the start position for lcd printing to the second row

/*
    if (newMenuItem.getName() == menu.getRoot()) {
        lcd.print("Main Menu       ");
    } else 
    */
    if (newMenuItem.getName() == "Item1") {
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
            lcd.print("left");
            break;
    }

    lastButtonPushed = 0; //reset the lastButtonPushed variable
}



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
    //menu.toRoot();  //back to Main
}

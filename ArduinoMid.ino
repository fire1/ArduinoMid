

//
// Display configs
#include <LiquidCrystal.h> // includes the LiquidCrystal Library 
LiquidCrystal lcd(1, 2, 4, 5, 6, 7); // Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7) 
//
// Define button pins
const int buttonPinOne = 8;
const int buttonPinTwo = 9;
//
// Variables will change:
int buttonPushCnt = 0;   // counter for the number of button presses
int buttonStateCr = 0;   // current state of the button
int buttonStateLs = 0;   // previous state of the button

void setup() {
    //
    // Pin button mode
    pinMode(buttonPinOne, INPUT);
    pinMode(buttonPinTwo, INPUT);
    //
    // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display }
    lcd.begin(16,2);
}

void loop() {

    buttonStateCr = digitalRead(buttonPinOne);
    buttonStateCr = digitalRead(buttonPinTwo);

    //switch(){ }

    lcd.print("Welcome to "); // Prints "Arduino" on the LCD
     delay(3000); // 3 seconds delay
     lcd.setCursor(11,0); // Sets the location at which subsequent text written to the LCD will be displayed
     lcd.print("Opel");
     delay(3000);
lcd.setCursor(1,2); 
 lcd.print("Astra");
 delay(3000);
 lcd.clear(); // Clears the display 
 lcd.blink(); //Displays the blinking LCD cursor 
 delay(4000); 
 lcd.setCursor(2,1); 
 lcd.print("Loading ...");
 delay(3000); 
 lcd.noBlink(); // Turns off the blinking LCD cursor 
 lcd.cursor(); // Displays an underscore (line) at the position to which the next character will be written 
 delay(4000); 
 lcd.noCursor(); // Hides the LCD cursor 
 lcd.clear(); // Clears the LCD screen 
}

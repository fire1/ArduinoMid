

#include <Arduino.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(32, 33, 34, 35, 36, 37);

void setup() {
    Serial.begin(250000);
    lcd.begin(16, 2);
}


int indexTest = 200;

void loop() {
    if (indexTest > 256) {
        indexTest = 0;
    }
    lcd.setCursor(0, 0);
    lcd.print((char) indexTest);

    lcd.setCursor(1, 1);
    lcd.print("   ");

    lcd.setCursor(1, 1);
    lcd.print(indexTest);
    indexTest++;

    lcd.setCursor(8, 1);
    lcd.print((char) 26);

    lcd.setCursor(9, 1);
    lcd.print((char) 246);

    delay(3000);

}
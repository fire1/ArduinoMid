//
// Created by Angel Zaprianov on 27.6.2016 г..
//

#ifndef ARDUINOMID_READINNTERTEMP_H
#define ARDUINOMID_READINNTERTEMP_H

/**
 * Read temperature sensor
 */
void readInnerTemp() {
    //
    // Inner vars
    int pinReadValue;
    int temperatureC;
    //
    // Read pin value
    pinReadValue = analogRead(sensorTempPin_1);
    //
    // Check is output time is allowed
    if (MID::isSensorReadAllow()) {

        //temperatureC = (pinReadValue * 100.0) / 1024.0;
        //temperatureC =  (200*pin_value)>>8;
        temperatureC = (225 * pinReadValue) >> 8;
        // converting that reading to voltage, for 3.3v arduino use 3.3
        int voltage = pinReadValue * 5.0;
        //voltage /= 1024.0;

        // now print out the temperature
        // int temperatureC = (voltage - 0.5) * 100 ;

        lcd.setCursor(8, 2);
        if (temperatureC > 0) {
            lcd.print((char)4);
            lcd.print((char)5);
        }
        lcd.print(temperatureC);
        lcd.print((char) 1);
    }

}

#endif //ARDUINOMID_READINNTERTEMP_H

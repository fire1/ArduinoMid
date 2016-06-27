//
// Created by Angel Zaprianov on 27.6.2016 г..
//

#ifndef ARDUINOMID_READINNTERTEMP_H
#define ARDUINOMID_READINNTERTEMP_H
/**
 * Read temperature sensor
 */
void readInnerTemp() {
  int pinReadValue;
  int temperatureC;


  pinReadValue = analogRead(temperaturePin);

  if (MID::isSensorReadAllow()) {

      temperatureC = (pinReadValue * 100.0) / 1024.0;
      //temperatureC =  (200*pin_value)>>8;

      // converting that reading to voltage, for 3.3v arduino use 3.3
      int voltage = pinReadValue * 5.0;
      //voltage /= 1024.0;

      // now print out the temperature
      // int temperatureC = (voltage - 0.5) * 100 ;

      lcd.setCursor(7, 2);
      lcd.print(temperatureC);
      lcd.print((char) 1);
    }

}
#endif //ARDUINOMID_READINNTERTEMP_H

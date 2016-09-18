//
// Created by Angel Zaprianov on 1.8.2016 г..
//

#ifndef ARDUINOMID_SPEEDSENSAALOG_H
#define ARDUINOMID_SPEEDSENSAALOG_H
void loop() {
  float volts1 = analogRead(IRpina)*0.0048828125;   // value from sensor * (5/1024) - if running 3.3.volts then change 5 to 3.3
  float distance1 = 65*pow(volts1, -1.10);          // worked out from graph 65 = theretical distance / (1/Volts)S - luckylarry.co.uk
  Serial.println(distance1);                        // print the distance
  delay(100);                                       // arbitary wait time.

  float volts2 = analogRead(IRpinb)*0.0048828125;   // value from sensor * (5/1024) - if running 3.3.volts then change 5 to 3.3
  float distance2 = 65*pow(volts2, -1.10);          // worked out from graph 65 = theretical distance / (1/Volts)S - luckylarry.co.uk
  Serial.println(distance2);                        // print the distance
  delay(100);                                       // arbitary wait time.

  float volts3 = analogRead(IRpinc)*0.0048828125;   // value from sensor * (5/1024) - if running 3.3.volts then change 5 to 3.3
  float distance3 = 65*pow(volts3, -1.10);          // worked out from graph 65 = theretical distance / (1/Volts)S - luckylarry.co.uk
  Serial.println(distance3);                        // print the distance
  delay(100);                                       // arbitary wait time.

  float volts4 = analogRead(IRpind)*0.0048828125;   // value from sensor * (5/1024) - if running 3.3.volts then change 5 to 3.3
  float distance4 = 65*pow(volts4, -1.10);          // worked out from graph 65 = theretical distance / (1/Volts)S - luckylarry.co.uk
  Serial.println(distance4);                        // print the distance
  delay(100);                                       // arbitary wait time.

  float volts5 = analogRead(IRpine)*0.0048828125;   // value from sensor * (5/1024) - if running 3.3.volts then change 5 to 3.3
  float distance5 = 65*pow(volts5, -1.10);          // worked out from graph 65 = theretical distance / (1/Volts)S - luckylarry.co.uk
  Serial.println(distance5);                        // print the distance
  delay(100);                                       // arbitary wait time.

  float volts6 = analogRead(IRpinf)*0.0048828125;   // value from sensor * (5/1024) - if running 3.3.volts then change 5 to 3.3
  float distance6 = 65*pow(volts6, -1.10);          // worked out from graph 65 = theretical distance / (1/Volts)S - luckylarry.co.uk
  Serial.println(distance6);                        // print the distance
  delay(100);                                      // arbitary wait time.
}
#endif //ARDUINOMID_SPEEDSENSAALOG_H

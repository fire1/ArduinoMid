//
// Created by Angel Zaprianov on 27.6.2016 г..
//

#ifndef ARDUINOMID_MID_H
#define ARDUINOMID_MID_H

#include <LiquidCrystal.h>



const int SNS_INTERVAL_TIME = 3000;

/**
 *
 */
class MID {
  LiquidCrystal lcd;

 public:
  void setup ();
  static void initMenu ();
  static void runMenu ();

  static bool isSensorReadAllow (long int interval = 0);

 private:
  void setupCustomCharacters ();

};

#endif //ARDUINOMID_UTILS_H

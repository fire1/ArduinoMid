//
// Created by Angel Zaprianov on 27.6.2016 г..
//

#ifndef ARDUINOMID_MID_H
#define ARDUINOMID_MID_H

#include <LiquidCrystal.h>



const int SNS_INTERVAL_TIME = 1000;

/**
 *
 */
class MID {
  LiquidCrystal lcd;

 public:
  void setup ();
  static void initMenu ();
  static void runMenu ();

  static bool isSensorReadAllow ();

 private:
  void setupCustomCharacters ();

};

#endif //ARDUINOMID_UTILS_H

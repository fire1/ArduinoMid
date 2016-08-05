//
// Created by Angel Zaprianov on 5.8.2016 г..
//

#ifndef ARDUINOMID_DIMLIGHTSENS_H
#define ARDUINOMID_DIMLIGHTSENS_H
//
// Set up pins
const DIM_PIN_VAL = A7; // input Dim of display
const DIM_PIN_OUT = A6; // output dim of display
const DIM_PIN_NGH = 50; // is car lights on input
//
// vars for resolve dim value
int backLightDefault = 621;       // value to dim display when car lights are off
int isGaugesActive = LOW;         // is car lights on
int unsigned backLightLevel = 0;  // resolved display dim

/**
 * Setup pins of display dim
 */
void setupBackLight (void)
{
  pinMode (DIM_PIN_VAL, INPUT);
  pinMode (DIM_PIN_NGH, INPUT);
  pinMode (DIM_PIN_OUT, OUTPUT);
}
/**
 * Handle display dim
 */
void handleBackLight (void)
{
  //
  // Get state of is active night mode
  isGaugesActive = digitalRead (DIM_PIN_NGH);

  if (isGaugesActive == HIGH)
    {
      backLightLevel = analogRead (DIM_PIN_VAL);
    }
  else
    {
      backLightLevel = backLightDefault;
    }
  //
  // Set backLight
  analogWrite (DIM_PIN_OUT, backLightLevel * 0.0049);
}

#endif //ARDUINOMID_DIMLIGHTSENS_H

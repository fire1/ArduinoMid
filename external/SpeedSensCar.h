//
// Created by Angel Zaprianov on 1.8.2016 г..
//

#ifndef ARDUINOMID_SPEEDSENSCAR_H
#define ARDUINOMID_SPEEDSENSCAR_H
// Matthew McMillan
// @matthewmcmillan
// http://matthewcmcmillan.blogspot.com
//
// Digital speedometer
//
// VSS on car connects to pin 5
// CLK on display to Analog pin 5
// DAT on display to Analog pin 4
//
// Requires two Adafruit libraries:
//   Adafruit_GFX
//   Adafruit_LEDBackpack

#include "Adafruit_GFX.h"    // Adafruit Core graphics library
#include "Adafruit_LEDBackpack.h" // Seven Segment display library
#include <SPI.h>
#include <Wire.h>

void setBrightness(uint8_t b, byte segment_address) {
  if (b > 15) b = 15; //Max brightness on this display is 15.
  if (b < 1) b = 1; // Brightness of 0 is too dim so make 1 the min.
  Wire.beginTransmission(segment_address);
  Wire.write(0xE0 | b); // write the brightness value to the hex address.
  Wire.endTransmission();
}

const int lightPin = 0;
const int hardwareCounterPin = 5;
const int samplePeriod = 1000; //in milliseconds
const float pulsesPerMile = 4000; // this is pulses per mile for Toyota. Other cars are different.
const float convertMph = pulsesPerMile/3600;
unsigned int count;
float mph;
unsigned int imph;
int roundedMph;
int previousMph;
int prevCount;

const int numReadings = 30;     // the number of readings for average brightness
int readings[numReadings];      // the readings array for the analog input
int index = 0;                  // the index of the current reading
int total = 0;                  // the running total
int average = 3;

Adafruit_7segment matrix = Adafruit_7segment();
byte segment_address = 0x70; //This is hex address of the 7 segment display
boolean drawDots = true;

void setup(void) {
  Serial.begin(9600);

  // initialize all the brightness readings to 3:
  for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 3;

  // Start up the 7 segment display and set initial vaules
  matrix.begin(segment_address);
  setBrightness(3, segment_address);
  matrix.println(0);
  matrix.writeDisplay();

  TCCR1A = 0; //Configure hardware counter
  TCNT1 = 0;  // Reset hardware counter to zero
}

void loop() {
  /////////////////////////////////////////////////////////////
  // Set the LCD brightness using a running average of
  // values to help smooth out changes in brightness.
  //
  // read from the sensor:
  int reading  = analogRead(lightPin);
  int brightness = (reading / 2) / 15;
  if(brightness > 15){
      brightness = 15;
    }
  if(brightness < 1){
      brightness = 1;
    }

  readings[index] = brightness;
  // add the reading to the total:
  total = total + readings[index];
  // advance to the next position in the array:
  index = index + 1;
  // if we're at the end of the array...
  if (index >= numReadings)
    // ...wrap around to the beginning:
    index = 0;
  // calculate the average:
  total = 0;
  for (int thisReading = 0; thisReading < numReadings; thisReading++){
      total = total + readings[thisReading];
    }
  average = total / numReadings;
  setBrightness(average, segment_address); //Set the brightness using the average


  /////////////////////////////////////////////////////////////
  // This uses the hardware pulse counter on the Arduino.
  // Currently it collects samples for one second.
  //
  bitSet(TCCR1B, CS12); // start counting pulses
  bitSet(TCCR1B, CS11); // Clock on rising edge
  delay(samplePeriod); // Allow pulse counter to collect for samplePeriod
  TCCR1B = 0; // stop counting
  count = TCNT1; // Store the hardware counter in a variable
  TCNT1 = 0;     // Reset hardware counter to zero
  mph = (count/convertMph)*10; // Convert pulse count into mph.
  imph = (unsigned int) mph; // Cast to integer. 10x allows retaining 10th of mph resolution.

  int x = imph / 10;
  int y = imph % 10;

  // Round to whole mile per hour
  if(y >= 5){
      roundedMph = x + 1;
    }else{
      roundedMph = x;
    }

  //If mph is less than 1mph just show 0mph.
  //Readings of 0.9mph or lower are some what erratic and can
  //occasionally be triggered by electrical noise.
  if(x == 0){
      roundedMph = 0;
    }

  // Don't display mph readings that are more than 50 mph higher than the
  // previous reading because it is probably a spurious reading.
  // Accelerating 50mph in one second is rocketship fast so it is probably
  // not real.
  if((roundedMph - previousMph) > 50){
      matrix.println(previousMph);
    }else{
      matrix.println(roundedMph);
    }

  matrix.writeDisplay(); // Write the value to the 7 segment display.

  previousMph = roundedMph; // Set previousMph for use in next loop.
}
#endif //ARDUINOMID_SPEEDSENSCAR_H

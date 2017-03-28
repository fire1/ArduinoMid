#include <Arduino.h>

uint8_t bitOrder = LSBFIRST;
uint8_t dataPin = A4;
uint8_t clockPin = A5;

void setup() {
// initialize Timer1
    noInterrupts(); // disable all interrupts
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;

    OCR1A = 31250; // compare match register 16MHz/256/2Hz
    TCCR1B |= (1 << WGM12); // CTC mode
    TCCR1B |= (1 << CS12); // 256 prescaler
    TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
    interrupts(); // enable all interrupts

    Serial.begin(115200);
}

uint8_t value, index;
ISR(Timer1_COMPA_vect) // timer compare interrupt service routine
{
    if (digitalRead(clockPin) == HIGH) {
        int data = digitalRead(dataPin);
        if (bitOrder == LSBFIRST)
            value |= data << index;
        else
            value |= data << (7 - index);

        index++;
    }
}

void loop() {
    Serial.println(value);
}
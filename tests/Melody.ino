


#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#if defined (__AVR_ATmega32U4__) || defined(__AVR_ATmega640__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__) || defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)
#define PWMT1AMASK DDB5
    #define PWMT1BMASK DDB6
    #define PWMT1DREG DDRB
    #define PWMT1PORT PORTB
#elif defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644P__)
#define PWMT1AMASK DDD4
    #define PWMT1BMASK DDD5
    #define PWMT1DREG DDRD
    #define PWMT1PORT PORTD
#else
#define PWMT1AMASK DDB1
#define PWMT1BMASK DDB2
#define PWMT1DREG DDRB
#define PWMT1PORT PORTB
#endif

#if defined(__AVR_ATmega8__) || defined(__AVR_ATmega128__)
#define TIMSK1 TIMSK
#endif

#define NOTONEAC 0
#define PLAY_FOREVER 0

void toneAC(unsigned long frequency = NOTONEAC, uint8_t volume = 10, unsigned long length = PLAY_FOREVER, uint8_t background = false);
void toneAC_playNote(unsigned long frequency, uint8_t volume);
void noToneAC();



unsigned long _tAC_time; // Used to track end note with timer when playing note in the background.
uint8_t _tAC_volume[] = { 200, 100, 67, 50, 40, 33, 29, 22, 11, 2 }; // Duty for linear volume control.

void toneAC(unsigned long frequency, uint8_t volume, unsigned long length, uint8_t background) {
    if (frequency == NOTONEAC || volume == 0) { noToneAC(); return; } // If frequency or volume are 0, turn off sound and return.
    if (volume > 10) volume = 10;       // Make sure volume is in range (1 to 10).

    toneAC_playNote(frequency, volume); // Routine that plays the note using timers.

    if (length == PLAY_FOREVER) return; // If length is zero, play note forever.

    if (background) {                   // Background tone playing, returns control to your sketch.
        _tAC_time = millis() + length;    // Set when the note should end.
        TIMSK1 |= _BV(OCIE1A);            // Activate the timer interrupt.
    } else {
        delay(length);                    // Just a simple delay, doesn't return control till finished.
        noToneAC();
    }
}

void toneAC_playNote(unsigned long frequency, uint8_t volume) {
    PWMT1DREG |= _BV(PWMT1AMASK) | _BV(PWMT1BMASK); // Set timer 1 PWM pins to OUTPUT (because analogWrite does it too).

    uint8_t prescaler = _BV(CS10);                  // Try using prescaler 1 first.
    unsigned long top = F_CPU / frequency / 2 - 1;  // Calculate the top.
    if (top > 65535) {                              // If not in the range for prescaler 1, use prescaler 256 (122 Hz and lower @ 16 MHz).
        prescaler = _BV(CS12);                        // Set the 256 prescaler bit.
        top = top / 256 - 1;                          // Calculate the top using prescaler 256.
    }

    ICR1   = top;                                     // Set the top.
    if (TCNT1 > top) TCNT1 = top;                     // Counter over the top, put within range.
    TCCR1B = _BV(WGM13)  | prescaler;                 // Set PWM, phase and frequency corrected (top=ICR1) and prescaler.
    OCR1A  = OCR1B = top / _tAC_volume[volume - 1];   // Calculate & set the duty cycle (volume).
    TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(COM1B0); // Inverted/non-inverted mode (AC).
}

void noToneAC() {
    TIMSK1 &= ~_BV(OCIE1A);        // Remove the timer interrupt.
    TCCR1B  = _BV(CS11);           // Default clock prescaler of 8.
    TCCR1A  = _BV(WGM10);          // Set to defaults so PWM can work like normal (PWM, phase corrected, 8bit).
    PWMT1PORT &= ~_BV(PWMT1AMASK); // Set timer 1 PWM pins to LOW.
    PWMT1PORT &= ~_BV(PWMT1BMASK); // Other timer 1 PWM pin also to LOW.
}

ISR(TIMER1_COMPA_vect) {                 // Timer interrupt vector.
    if (millis() >= _tAC_time) noToneAC(); // Check to see if it's time for the note to end.
}

// Melody liberated from the toneMelody Arduino example sketch by Tom Igoe.
int melody[] = { 262, 196, 196, 220, 196, 0, 247, 262 };
int noteDurations[] = { 4, 8, 8, 4, 4, 4, 4, 4 };

void setup() {} // Nothing to setup, just start playing!

void loop() {
    for (unsigned long freq = 125; freq <= 15000; freq += 10) {
        toneAC(freq); // Play the frequency (125 Hz to 15 kHz sweep in 10 Hz steps).
    }
    toneAC(); // Turn off toneAC, can also use noToneAC().

    delay(1000); // Wait a second.

    for (int thisNote = 0; thisNote < 8; thisNote++) {
        int noteDuration = 1000/noteDurations[thisNote];
        toneAC(melody[thisNote], 10, noteDuration, true); // Play thisNote at full volume for noteDuration in the background.
        delay(noteDuration * 4 / 3); // Wait while the tone plays in the background, plus another 33% delay between notes.
    }

    while(1); // Stop (so it doesn't repeat forever driving you crazy--you're welcome).
}





























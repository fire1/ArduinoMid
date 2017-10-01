#include <avr/interrupt.h>

/* First disable the timer overflow interrupt while we're configuring */
TIMSK3 &= ~(1<<TOIE3); // clear timer

/* Configure timer2 in normal mode (pure counting, no PWM etc.) */
TCCR3A &= ~((1<<WGM31) | (1<<WGM30));
TCCR3B &= ~(1<<WGM32);

/* Select clock source: internal I/O clock */
ASSR &= ~(1<<AS2);

/* Disable Compare Match A interrupt enable (only want overflow) */
TIMSK3 &= ~(1<<OCIE3A);

/* Now configure the prescaler to CPU clock divided by 128 */
TCCR3B |= (1<<CS32)  | (1<<CS30); // Set bits
TCCR3B &= ~(1<<CS31);             // Clear bit


//
//
/*
 timer3	16bit	B	-	pin 2
 timer3	16bit	C 	-	pin 3
 */

///##########################

cli();          // disable global interrupts
TCCR3A = 0;     // set entire TCCR3A register to 0
TCCR3B = 0;     // same for TCCR3B

// set compare match register to desired timer count: 800 Hz
OCR3B = 20;
// turn on CTC mode:
TCCR3B |= (1 << WGM12);
// Set CS10 and CS12 bits for 1024 prescaler:
TCCR3B |= (1 << CS30) | (1 << CS32);
// enable timer compare interrupt:
TIMSK3 |= (1 << OCIE3B);
// enable global interrupts:
sei();

unsigned long testTimer =0;
ISR(TIMER3_COMPA_vect)
{
    testTimer++;
}

///##########################


// Set timer1_counter to the correct value for our interrupt interval
// timer1_counter = 64911;   // preload timer 65536-16MHz/256/100Hz
// timer1_counter = 64286;   // preload timer 65536-16MHz/256/50Hz
// timer1_counter = 34286;   // preload timer 65536-16MHz/256/2Hz

TCNT1 = 34286;   // preload timer
TCCR1B |= (1 << CS12);    // 256 prescaler
TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt


/* First disable the timer overflow interrupt while we're configuring */

//TIMER3 reset
TIMSK3 = 0;
TCCR3A = 0;
TCCR3B = 0;
OCR3A = 10;
//
ASSR &= ~(1<<AS2);
TIMSK3 |= (1<<OCIE3A);
/* Setze Prescaler auf 64 */
TCCR3B |= ((1<<CS31) | (1<<CS30));  // Setze bit CS32
TCCR3B &= ~(1<<CS32); // Setze Bit CS31 und Bit CS30 auf 0


//TCCR3B |= _BV (CS32) | _BV (CS30);



TCCR2A = (1<<WGM21)| (1<<WGM20) | (1<<COM2A1) | (1<<COM2B1);   // set mode as FAST PWM (MODE 1 of Table 17-8),
TCCR2B = (1<<CS22);	//CS22=64, CS21=8		           // set CLOCK SELECT Output A/B frequency: 16 MHz / 64 scale / 255 bit / 2 = 490.196Hz

//1.2. CALCULATE DESIRED PERIOD
long int period=1000000/(16000000/64/255/2); Serial.println(period);//Clock = 16Mhz; CLOCK SELECT CS22 = 64; Timer2 is 8bit = 255;
long int desired_pulseA =  500, desired_pulseB = 1500;
long int ocra = (desired_pulseA * 255/period); //Ratio of Pulse and Period i.e. 255 (8bit) is 100% = 1/490Hz
long int ocrb = (desired_pulseB * 255/period);

//2.3 Set Pulse
OCR2A = ocra;
OCR2B = ocrb;



// mpguino timer dump

void init2(){
    // this needs to be called before begin() or some functions won't
    // work there
    sei();

    // timer 0 is used for millis2() and delay2()
    timer2_overflow_count = 0;
    // on the ATmega168, timer 0 is also used for fast hardware pwm
    // (using phase-correct PWM would mean that timer 0 overflowed half as often
    // resulting in different millis2() behavior on the ATmega8 and ATmega168)
    TCCR2A=1<<WGM20|1<<WGM21;
    // set timer 2 prescale factor to 64
    TCCR2B=1<<CS22;


//      TCCR2A=TCCR0A;
//      TCCR2B=TCCR0B;
    // enable timer 2 overflow interrupt
    TIMSK2|=1<<TOIE2;
    // disable timer 0 overflow interrupt
    TIMSK0&=!(1<<TOIE0);
}
ISR(TIMER2_OVF_vect)
{
    testTimer++;
}


/*
 TCCR1B |= (1 << CS10); and TCCR1B |= (1 << CS12);, the clock source is divided by 1024.
 This gives a timer resolution of 1/(16*10⁶ / 1024), or 0.000064 seconds (15625 Hz).
 Now the timer will overflow every (65535 * 6.4*10-5s), or 4.194s.
 */


cli();

TIMSK3 &= ~(1 << TOIE3);
TCCR3A &= ~((1 << WGM31) | (1 << WGM30));
TCCR3B &= ~(1 << WGM32);
TCCR3A = TCCR3A | (1 << COM3A1) | (1 << COM3B1) | (1 << COM3C1);

//    ASSR &= ~(1 << AS3);


TIMSK3 &= ~(1 << OCIE3A);
TCCR3B |= (1 << CS32) | (1 << CS30);
TCCR3B &= ~(1 << CS31);

sbi(TCCR3B, CS32);
sbi(TCCR3A, WGM30);

sei();

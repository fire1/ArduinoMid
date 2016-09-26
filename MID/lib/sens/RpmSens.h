//
// Created by Admin on 7/23/2016.
//

#ifndef ARDUINOMID_RpmSens_H
#define ARDUINOMID_RpmSens_H

//
// Sensor configs
const bool RpmSensDebug = 0;
const int RpmCorrection = 36;

//
// Rpm Container
int CUR_RPM = 0;

//
// Working vars
int rpmHitsCount = 0;
int rpmCycles = 0;
int long rpmTimerStart = 0, rpmTimerEnds = 0;

/**
 * Callback attachInterrupt
 */
void catchRpmHits () {
  rpmHitsCount++;
}


void setupRpmSens(int pinTarget) {
    pinMode(pinTarget, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt (pinTarget), catchRpmHits, FALLING);
}
/** deprecated
 * Setup Rpm
 */
void setupRpmSens__(int pinTarget){

    // set timer 2 prescale factor to 64
    sbi(TCCR2B, CS22);
    // configure timer 2 for phase correct pwm (8-bit)
    sbi(TCCR2A, WGM20);



    TCCR2A = 1 << WGM20 | 1 << WGM21;
    // set timer 2 prescale factor to 64
    TCCR2B = 1 << CS22;
    TIMSK2 |= 1 << TOIE2;
//    TIMSK0 &= !(1 << TOIE0);
}

void setupRpmSens_(int pinTarget) {
	/* First disable the timer overflow interrupt while we're configuring */
	TIMSK2 &= ~(1 << TOIE2);

	/* Configure timer2 in normal mode (pure counting, no PWM etc.) */
	TCCR2A &= ~((1 << WGM21) | (1 << WGM20));
	TCCR2B &= ~(1 << WGM22);

	/* Select clock source: internal I/O clock */
	ASSR &= ~(1 << AS2);

	/* Disable Compare Match A interrupt enable (only want overflow) */
	TIMSK2 &= ~(1 << OCIE2A);

	/* Now configure the prescaler to CPU clock divided by 128 */
	TCCR2B |= (1 << CS22) | (1 << CS20); // Set bits
	TCCR2B &= ~(1 << CS21);             // Clear bit

	/* We need to calculate a proper value to load the timer counter.
     * The following loads the value 131 into the Timer 2 counter register
     * The math behind this is:
     * (CPU frequency) / (prescaler value) = 125000 Hz = 8us.
     * (desired period) / 8us = 125.
     * MAX(uint8) + 1 - 125 = 131;
     */
	/* Save value globally for later reload in ISR */
	//tcnt2 = 131;

	/* Finally load end enable the timer */
	//TCNT2 = tcnt2;
	TCNT2 = 0;
	TIMSK2 |= (1 << TOIE2);

	// set timer 2 prescale factor to 64
	sbi(TCCR2B, CS22);
	// configure timer 2 for phase correct pwm (8-bit)
	sbi(TCCR2A, WGM20);
}

int LastRpm = LOW;
/* timer 2 (controls pin 10, 9)
 * Install the Interrupt Service Routine (ISR) for Timer2 overflow.
 * This is normally done by writing the address of the ISR in the
 * interrupt vector table but conveniently done by using ISR()  */
ISR(TIMER2_OVF_vect) {
	int current = digitalRead(RPM_SNS_PIN);
	if (current == HIGH && LastRpm == LOW)
		rpmHitsCount++;


	LastRpm = current;
}

/**
 * Gets current RPM
 */
int getRpmSens () {
	return CUR_RPM;
}


/**
 * Detect RPMs
 */
void sensRpm () {
	rpmTimerEnds = millis ();
	if (rpmTimerEnds >= (rpmTimerStart + 460))
	{
		//
		// Handle cycles
		rpmTimerStart = rpmTimerEnds;
		//
		// Pass rpm to global
		CUR_RPM = rpmHitsCount * RpmCorrection;

		//
		// debug info
		if (RpmSensDebug)
		{
			Serial.print ("\n");
			Serial.print (" RPM count:  \t");
			Serial.print (rpmHitsCount);
			Serial.print (" RPM is:  \t");
			Serial.print (rpmHitsCount * 450);
			Serial.print ("\n");
		}
		//
		// Clear value
		rpmHitsCount = 0;
	}
}


#endif //ARDUINOMID_RpmSens_H


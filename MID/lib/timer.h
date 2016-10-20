//
// Created by Angel Zaprianov on 23.9.2016 г..
//

#ifndef ARDUINOMID_TIMER_H
#define ARDUINOMID_TIMER_H


class Timer {

public:
    static long last_stamp;
    static long *stamps;
    static int *reversals;
    static int count;
    static int reversal_count;

    static void setup_timer() {
        // Setup Timer2 overflow to fire every 8ms (125Hz)
        //   period [sec] = (1 / f_clock [sec]) * prescale * (255-count)
        //                  (1/16000000)  * 1024 * (255-130) = .008 sec


        TCCR2B = 0x00;        // Disable Timer2 while we set it up

        TCNT2 = 130;         // Reset Timer Count  (255-130) = execute ev 125-th T/C clock
        TIFR2 = 0x00;        // Timer2 INT Flag Reg: Clear Timer Overflow Flag
        TIMSK2 = 0x01;        // Timer2 INT Reg: Timer2 Overflow Interrupt Enable
        TCCR2A = 0x00;        // Timer2 Control Reg A: Wave Gen Mode normal
        TCCR2B = 0x07;        // Timer2 Control Reg B: Timer Prescaler set to 1024

        count = 0;
        stamps = new long[50];
        reversals = new int[10];
        reversal_count = 0;
    }

    static long get_stamp() {
        stamps[count++] = millis();
        return count - 1;
    }

    static bool compare_stamps_by_id(int s1, int s2) {
        return s1 > s2;
    }

    static long long get_stamp_difference(int s1, int s2) {
        int no_of_reversals = 0;
        for (int j = 0; j < reversal_count; j++)
            if (reversals[j] < s2 && reversals[j] > s1)
                no_of_reversals++;
        return stamps[s2] - stamps[s1] + 49.7 * 86400 * 1000;
    }

};
/*****    USAGE    *****/
//
//long Timer::last_stamp;
//long *Timer::stamps;
//int *Timer::reversals;
//int Timer::count;
//int Timer::reversal_count;
//
//ISR(TIMER2_OVF_vect) {
//
//        long stamp = millis();
//        if(stamp < Timer::last_stamp) // reversal
//        Timer::reversals[Timer::reversal_count++] = Timer::count;
//        else
//        ; // no reversal
//        Timer::last_stamp = stamp;
//        TCNT2 = 130;     // reset timer ct to 130 out of 255
//        TIFR2 = 0x00;    // timer2 int flag reg: clear timer overflow flag
//};
//
//// Usage
//
//void setupEngine () {
//    Timer::setup_timer();
//
//    long s1 = Timer::get_stamp();
//    delay(3000);
//    long s2 = Timer::get_stamp();
//
//    Timer::compare_stamps_by_id(s1, s2); // true
//
//    Timer::get_stamp_difference(s1, s2); // return true difference, taking into account reversals
//}


#endif //ARDUINOMID_TIMER_H

//
// Created by Angel Zaprianov on 3.5.2017 г..
//

#ifndef ARDUINOMID_LCDPWR_H
#define ARDUINOMID_LCDPWR_H
/**
 *
 */
class LcdPwr {
private:
    uint8_t pwr[6];
    uint8_t adt[3];
    bool additional = false;
public:
/**
 *
 * @param pinGnd
 * @param pinVcc
 * @param pinBlA
 * @param pinBlB
 */
    LcdPwr(uint8_t pinGnd, uint8_t pinVcc, uint8_t pinBlA, uint8_t pinBlB, uint8_t pinRD) {
        pwr[0] = pinGnd;
        pwr[1] = pinVcc;
        pwr[2] = pinBlA;
        pwr[3] = pinBlB;
        pwr[4] = pinRD;
    }

/**
 *
 * @param pinGnd    Ground
 * @param pinVcc    5V+
 * @param pinBLn    BackLight Ground
 * @param pinBLp    BackLight 5v+
 * @param pinRD     Display's chip read
 * @param pinFS     Display's Font Select
 * @param pinGnd2   Second ground
 */
    LcdPwr(uint8_t pinGnd, uint8_t pinVcc, uint8_t pinBLn, uint8_t pinBLp, uint8_t pinRD, uint8_t pinFS,
           uint8_t pinGnd2 = 0) {
        pwr[0] = pinGnd;
        pwr[1] = pinVcc;
        pwr[2] = pinBLn;
        pwr[3] = pinBLp;
        pwr[4] = pinRD;
        additional = true;
        adt[0] = pinFS;
        adt[1] = pinGnd2;
    }


    void begin() {
        analogWrite(pwr[0], 0); // GND
        analogWrite(pwr[1], 255); // 5V
        analogWrite(pwr[3], 0); // back light
        analogWrite(pwr[2], 255); // back light
        analogWrite(pwr[4], 0); // back light

        if (additional) {
            digitalWrite(adt[0], HIGH); // RD
            analogWrite(adt[1], 0); // fs
            if (adt[1] > 0) analogWrite(adt[2], 0); // GND
        }

//        delete this;
    }

    ~LcdPwr() {};
};









#endif //ARDUINOMID_LCDPWR_H

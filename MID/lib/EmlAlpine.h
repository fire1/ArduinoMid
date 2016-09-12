/*
    Steering wheel buttons to Sony Alpine

            resistance  voltage
---------------------------------------------------
src       2200      4.1
att         4400      3.47
off         35          4.98
vol down  23850     1.47
vol up      17000     1.85
seek down 12000     2.25
seek up     8800      2.65
back      6660      3
front     33940     1.13
bottom      48800     0.84
---------------------------------------------------
 Sony Voltages                                  
 */
const int sonySrc = 41; //4.1v
const int sonyAtt = 34;
const int sonyOff = 49;
const int sonyVolDw = 14;
const int sonyVolUp = 18;
const int sonySeekDw = 26;
const int sonySeekUp = 22;
const int sonyBack = 29;
const int sonyFront = 11;
const int sonyBottom = 8;
//
// Output for Sony Alpine emulation
const int sonyPin = A0;
const int sonyShiftPin = A2;
const int alpinePin = 3;


/**
        Handle Steering wheel media buttons
 */
void handleSteeringToSony(int voltage, boolean shift) {

    // Debug info ...
//    Serial.println(voltage);
//    Serial.println(shift);
}

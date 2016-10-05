
#include <SPI.h>

// remote control commands for Pioneer
const int VOL_UP=1;
const int VOL_DN=2;
const int PREV_TR=3;
const int NEXT_TR=4;
const int MODE=5;
const int MUTE=6;


int csPin=4; // digipot pin

int wheelPin=A5; // steering wheel resistance reading pin

int i=0;
int prevButton=0;

void setup() {
    pinMode(csPin, OUTPUT);
    delay(50);

    SPI.begin();
    delay(50);
    SPI.transfer(0); // command
    SPI.transfer(0); // value

    pinMode(wheelPin, INPUT);

    delay(100);
    //Serial.begin(9600);
}


int getR() { // this function returns pressed button code or 0 if all buttons are released

    // read resistance value from the steering wheel buttons
    int r=analogRead(wheelPin);
    //Serial.println(r);

    // below values are for Suzuki Swift steering wheel controls and 10 kOhms known resistor
    if (r>=9 && r<=15) return(VOL_UP);
    if (r>=18 && r<=26) return(VOL_DN);
    if (r>=120 && r<=156) return(PREV_TR);
    if (r>=60 && r<=80) return(NEXT_TR);
    if (r>=33 && r<=47) return(MODE);
    if (r>=2 && r<=6) return(MUTE);
    return (0);
}

void loop() {
    int currButton=getR(); // get current pressed button code
    if (currButton!=prevButton) { // if it has changed since last reading
        delay(10);
        currButton=getR(); // wait 10ms and read again to make sure this is not just some noise
        if (currButton!=prevButton) {
            //Serial.println(currButton);
            prevButton=currButton;

            // send command to car stereo, below values are for Pioneer head units
            SPI.transfer(0);
            switch(currButton) {
                case VOL_UP: SPI.transfer(21); break;  // 16kOhm
                case VOL_DN: SPI.transfer(31); break;  // 24k
                case PREV_TR: SPI.transfer(14); break;  // 11k
                case NEXT_TR: SPI.transfer(10); break;  // 8k
                case MODE: SPI.transfer(2); break;  // 1.6k
                case MUTE: SPI.transfer(4); break;  // 3k
                default: SPI.transfer(0); break; // nothing
            }
        }
    }
    delay(5);
}

#include <ReceiveOnlySoftwareSerial.h>

ReceiveOnlySoftwareSerial mySerial(10); // RX

void setup()
{
    Serial.begin(9600);
    while (!Serial) { }  // wait for Serial to become available

    // set the data rate for the ReceiveOnlySoftwareSerial port
    mySerial.begin(9600);
}

void loop() // run over and over
{
    // echo from software serial to hardware serial
    if (mySerial.available())
        Serial.write(mySerial.read());
}
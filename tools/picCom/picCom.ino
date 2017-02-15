#include <Arduino.h>

#define  CLOCK  9
#define  DATA   8 // more then 330 ohm

void WriteBit(char nb)
{
    digitalWrite(CLOCK, HIGH);
    if(nb!='0')
    {
        digitalWrite(DATA,HIGH);
    }
    delayMicroseconds(3);
    digitalWrite(CLOCK, LOW);
    delayMicroseconds(3);

    digitalWrite(DATA,LOW);

}

byte ReadBit()
{
    byte valueRead = 0;

    digitalWrite(CLOCK, HIGH);
    delayMicroseconds(3);
    if(digitalRead(DATA)==HIGH)
    {
        valueRead = '1';
    }
    else
    {
        valueRead = '0';
    }
    digitalWrite(CLOCK, LOW);
    delayMicroseconds(3);

    return valueRead;
}

void ReadDataFromProgramMemory(char valueRead[])
{
//    digitalWrite(DATA,LOW);

    //Command

//    WriteBit('0');
//    WriteBit('0');
//    WriteBit('1');
//    WriteBit('0');
//    WriteBit('0');
//    WriteBit('0');

    pinMode(DATA, INPUT);
    delayMicroseconds(3);

    //DATA
    ReadBit();
    for(int i =13;i>=0;i--)
    {
        valueRead[i] = ReadBit();
    }
    ReadBit();





}
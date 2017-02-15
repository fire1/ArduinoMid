//
// Created by Angel Zaprianov on 15.2.2017 г..
//

#ifndef ARDUINOMID_TEST_H
#define ARDUINOMID_TEST_H
/*
PIC programmer using Arduino
This program is distibuted "AS IS", I offer no garantee.
By Ronan Gaillard
*/
#define  CLOCK  9
#define  DATA   8
#define  VPP1   6
#define  VPP2   5
#define  VPP3   4
#define  PGM    10 //HIGH/LOW voltage select mode
#define  MCLR   11 //LOW voltage only

void setup() {

    pinMode(CLOCK, OUTPUT);
    pinMode(VPP1, OUTPUT);
    pinMode(VPP2, OUTPUT);
    pinMode(VPP3, OUTPUT);
    pinMode(DATA, OUTPUT);
    pinMode(PGM, OUTPUT);

    Serial.begin(9600);
    Serial.println("Started");
//Serial.println("Send s to start (please put Vpp first)");
    while(true)
    {
        if (Serial.available() > 0)
        {
            if(Serial.read()=='s')//HIGH VOLTAGE MODE
            {
                //VIN should be around 12V max. is 13.5V


                digitalWrite(VPP1, HIGH);
                digitalWrite(VPP2, HIGH);
                digitalWrite(VPP3, HIGH);

                Serial.print("D");
                break;
            }
            else//low voltage mode
            {
                digitalWrite(PGM, LOW);
                digitalWrite(VPP1, LOW);
                digitalWrite(VPP2, LOW);
                digitalWrite(MCLR, LOW);
                digitalWrite(VPP3, LOW);
                delayMicroseconds(20);
                digitalWrite(VPP1, HIGH);
                digitalWrite(VPP2, HIGH);
                digitalWrite(VPP3, HIGH);
                delayMicroseconds(3);
                digitalWrite(PGM, HIGH);
                delayMicroseconds(3);
                digitalWrite(MCLR, HIGH);
                Serial.print("D");
                break;
            }
        }

    }
}

void loop() {

    char command = '\0';

    if (Serial.available() > 0) {

        command = Serial.read();

        switch(command)
        {
            case 'b' :
                BulkEraseProgramMemory();

                Serial.print('D');
                break;

            case 'E' :
                EndOfProgramming();
                Serial.print('D');
                break;

            case 'r' :
                char valeur[16];
                ReadDataFromProgramMemory(valeur);
                for(int i=0; i<14;i++)
                {
                    Serial.print(valeur[i]);
                }
                //Serial.print("D");
                break;

            case 'l' :
                char WritingValue[14];
                for(int i=0;i<14;i++)
                {
                    while(Serial.available() == 0)
                    {
                    }
                    WritingValue[i] = Serial.read();
                }


                LoadDataCommandForProgramMemory(WritingValue);


                Serial.print('D');
                break;

            case 'w' :
                BeginProgrammingOnlyCycle();
                Serial.print('D');
                break;

            case 'W' :
                BeginEraseProgrammingCycle();
                Serial.print('D');
                break;

            case 'i' :
                IncrementAddress();
                Serial.print('D');
                break;

            case 'c' :
                char ConfigValue[14];
                for(int i=0;i<14;i++)
                {
                    while(Serial.available() == 0)
                    {
                    }
                    ConfigValue[i] = Serial.read();
                }
                LoadConfiguration(ConfigValue);
                BeginProgrammingOnlyCycle();

                Serial.print('D');
                break;

            case 'R' :


                digitalWrite(VPP1, LOW);
                digitalWrite(VPP2, LOW);
                digitalWrite(VPP3, LOW);

                delay(3);

                digitalWrite(VPP1, HIGH);
                digitalWrite(VPP2, HIGH);
                digitalWrite(VPP3, HIGH);

                Serial.print("D");

                break;


        }
    }


}
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
    byte valeur = 0;

    digitalWrite(CLOCK, HIGH);
    delayMicroseconds(3);
    if(digitalRead(DATA)==HIGH)
    {
        valeur = '1';
    }
    else
    {
        valeur = '0';
    }
    digitalWrite(CLOCK, LOW);
    delayMicroseconds(3);

    return valeur;
}
void ReadDataFromProgramMemory(char valeur[])
{
    digitalWrite(DATA,LOW);

    //Command

    WriteBit('0');
    WriteBit('0');
    WriteBit('1');
    WriteBit('0');
    WriteBit('0');
    WriteBit('0');

    pinMode(DATA, INPUT);
    delayMicroseconds(3);

    //DATA
    ReadBit();
    for(int i =13;i>=0;i--)
    {
        valeur[i] = ReadBit();
    }
    ReadBit();



    pinMode(DATA, OUTPUT);


}

void LoadDataCommandForProgramMemory(char valeur[])
{
    digitalWrite(DATA,LOW);

    //Command

    WriteBit('0');
    WriteBit('1');
    WriteBit('0');
    WriteBit('0');
    WriteBit('0');
    WriteBit('0');


    delayMicroseconds(3);

    //DATA
    WriteBit('0');//always 0
    for(int i =13;i>=0;i--)
    {
        WriteBit(valeur[i]);
    }
    WriteBit('0');//always 0 (stop bit)

    delayMicroseconds(3);






}
void BeginProgrammingOnlyCycle()
{
    digitalWrite(DATA,LOW);

    WriteBit('0');
    WriteBit('0');
    WriteBit('0');
    WriteBit('1');
    WriteBit('1');
    WriteBit('0');


    delayMicroseconds(3);





    delay(20);
}

void BeginEraseProgrammingCycle()
{
    digitalWrite(DATA,LOW);

    WriteBit('0');
    WriteBit('0');
    WriteBit('0');
    WriteBit('1');
    WriteBit('0');
    WriteBit('0');


    delayMicroseconds(3);





    delay(20);
}

void BulkEraseProgramMemory()
{
    digitalWrite(DATA,LOW);

    WriteBit('1');
    WriteBit('0');
    WriteBit('0');
    WriteBit('1');
    WriteBit('0');
    WriteBit('0');


    delayMicroseconds(3);



    delayMicroseconds(3);

    delay(20);
}

void FullEraseProgramMemory()
{
    char Erase [] = {'1','1','1','1','1','1','1','1','1','1','1','1','1','1'};
    LoadDataCommandForProgramMemory(Erase);
    BulkEraseProgramMemory();
    BeginProgrammingOnlyCycle();
}

void IncrementAddress()
{
    digitalWrite(DATA,LOW);

    WriteBit('0');
    WriteBit('1');
    WriteBit('1');
    WriteBit('0');
    WriteBit('0');
    WriteBit('0');


    delayMicroseconds(3);
}

void LoadDataCommandForDataMemory(char valeur[])
{
    digitalWrite(DATA,LOW);

    //Command

    WriteBit('1');
    WriteBit('1');
    WriteBit('0');
    WriteBit('0');
    WriteBit('0');
    WriteBit('0');


    delayMicroseconds(3);

    //DATA
    WriteBit('0');//always 0
    for(int i =13;i>=0;i--)
    {
        WriteBit(valeur[i]);
    }
    WriteBit('0');//always 0 (stop bit)

    delayMicroseconds(3);






}
void LoadConfiguration(char valeur[])
{
    digitalWrite(DATA,LOW);

    //Command

    WriteBit('0');
    WriteBit('0');
    WriteBit('0');
    WriteBit('0');
    WriteBit('0');
    WriteBit('0');


    delayMicroseconds(3);

    //DATA
    WriteBit('0');//always 0
    for(int i =13;i>=0;i--)
    {
        WriteBit(valeur[i]);
    }
    WriteBit('0');//always 0 (stop bit)

    delayMicroseconds(3);






}

void EndOfProgramming()
{
    digitalWrite(DATA,LOW);

    //Command

    WriteBit('1');
    WriteBit('1');
    WriteBit('1');
    WriteBit('0');
    WriteBit('1');
    WriteBit('0');

    delayMicroseconds(3);
}






#endif //ARDUINOMID_TEST_H

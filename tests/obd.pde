//
// Created by Angel Zaprianov on 9.8.2016 г..
//

#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include "lib/OBD.h"
#define RxD 7                //Arduino pin connected to Tx of HC-05
#define TxD 8                //Arduino pin connected to Rx of HC-05

SoftwareSerial test_obd (RxD, TxD);

void getResponse(void){
  char inChar=0;
  if (off2)
    {
      while(inChar != '\r'){
          if(test_obd.available() > 0){
              if(test_obd.peek() == '\r'){
                  inChar=test_obd.read();
                  rxData[rxIndex]='\0';
                  rxIndex=0;
                }
              else {
                  inChar = test_obd.read();
                  rxData[rxIndex++]=inChar;
                }
            }
        }
    }
}

void SearchErrorEngine(void) {
  delay(delay_var);

  getResponse();
  getResponse();
  if ((strtol(&rxData[0],0,16)==65) and (strtol(&rxData[3],0,16)==1)) {
      kol_check_engine_error_val=strtol(&rxData[6], 0,16)-1;
    }
}

setup ()
{

  Serial.begin(9600);

  pinMode (RxD, INPUT);
  pinMode (TxD, OUTPUT);

  test_obd.flush ();
  test_obd.begin (38400); //default communication baud rate of HC-05 is 38400
  delay (delay_var);
  test_obd.println ("ATZ"); //rezet
  delay (900);
  test_obd.println ("ATSP0"); // протокол ATSP0 - авто, cirrus ATSP3
  delay (100);

  test_obd.flush();
  test_obd.print("0101");  //  	Проверка ошибок двигателя
  test_obd.print("\r");
  delay (100);
  getResponse();
  getResponse();

  Serial.print(&rxData[0]);

}
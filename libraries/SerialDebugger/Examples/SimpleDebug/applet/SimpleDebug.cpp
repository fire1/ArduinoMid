/*
||
|| @author Alexander Brevig
|| @version 1.1 Beta
||
|| @description
|| Demonstrates the functionality of the SerialDebugger
||
*/

#include <SerialDebug.h>

#define DEBUG true

#include "WProgram.h"
void setup();
void loop();
void setup(){
  SerialDebugger.enable(NOTIFICATION);
  SerialDebugger.enable(ERROR);//uncomment if you want to debug ERRORs
  if (DEBUG){
    //SerialDebugger.begin(2,9600); //can select uart
    SerialDebugger.begin(9600); //if this line is commented out, the binary sketch size will decrease by 332 bytes
  }
}
  
void loop(){
  SerialDebugger.debug(NOTIFICATION,"loop","notifications are enabled");
  if (SerialDebugger.debug(ERROR,"loop","errors are disabled")){
    //do actions that might be required when debugging errors
    //(re)setting variables
    //additional SerialDebugger.debug() calls
    SerialDebugger.print("it is fun wi").print("th cascaded fun").print("ction calls!");
  }
  delay(1000);
}

int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}


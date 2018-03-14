
const uint8_t pinInput = 17;

boolean serial2Low = false;
boolean Serial2High = true;
uint8_t serial2Index;
uint16_t serial2Length;
unsigned long serial2Timing;
//char dataBuff[] = {};
uint32_t dataBuff = 0;

void serialEvent2() {




    if (digitalRead(pinInput) == LOW ) {
        serial2Length = (uint8_t) (millis() - serial2Timing); //if it is low, end the time
        serial2Timing = millis();


        if (serial2Length <= 6) {
            dataBuff |= 0 << serial2Index;
//            dataBuff[serial2Index] = 0;
            serial2Index++;
        }

        if (serial2Length > 6 && serial2Length <= 8) {
            dataBuff |= 1 << serial2Index;
//            dataBuff[serial2Index] = 1;
            serial2Index++;
        }



    }
    // LPG
    // 2221 2201
    // 101000000001 101000000001

    // BNZ
    // 1501 1521 C01 1C00 201
    // 110000000001




}


void setup()
{
    pinMode(pinInput, INPUT);

    attachInterrupt(digitalPinToInterrupt(pinInput), serialEvent2, FALLING );
    Serial.begin(9600);
    Serial.println("Beginning ... ");
}






void loop()
{
    if ( dataBuff >= 7) {
        Serial.print(F("BIN "));
        Serial.print(dataBuff, BIN);

        Serial.println();
        dataBuff = 0;
        serial2Index = 0;
    }
}
const byte ledPin = 13;
const byte interruptPin = 2;
volatile byte state = LOW;
int countState = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, FALLING  );
  Serial.print(digitalPinToInterrupt(interruptPin));
}

void loop() {
  digitalWrite(ledPin, state);
}

void blink() {
  countState++;
  Serial.print(countState);
  state = !state;
}
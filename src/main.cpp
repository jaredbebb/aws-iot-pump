#include <Arduino.h>
#include <Ticker.h>

Ticker pumpticker;
unsigned long period = 30*1000;
unsigned long execution = 10*1000;
int relayPin = D8;
int state = 1;

void relay_pin_low(){
  state = 1;
  digitalWrite(relayPin, LOW);
  delay(execution);
}

void setup() {
  // initialize pin as output
  pinMode(relayPin, OUTPUT);
  Serial.begin(115200);
  pumpticker.attach_ms_scheduled(period, relay_pin_low);
}

void loop(){
  if (state){
    digitalWrite(relayPin, HIGH);
  }
}
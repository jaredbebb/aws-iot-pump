#include <Arduino.h>
#include <Ticker.h>

Ticker pumpticker;
unsigned long period = 30000;//14400*1000;
unsigned long execution = 15000;//20*1000;
int relayPin = D8;
int state;

void relay_pin_state(){
    state = 1;
    Serial.println("state = 1");
}

void setup() {
    // initialize pin as output
    Serial.begin(115200);
    delay(5000);
    Serial.printf("Starting up. time:%6ld \n",millis());
    pinMode(relayPin, OUTPUT);
    state = 0;
    pumpticker.attach_ms_scheduled(period, relay_pin_state);
}

void loop(){
    if (state){
        digitalWrite(relayPin, LOW);
        Serial.printf("digitalWrite(relayPin, LOW) time:%6ld \n",millis());
        unsigned long start_time = millis();
        while( (millis()-start_time) < execution){
            //let esp8266 run background functions.
            yield();
        }
        digitalWrite(relayPin, HIGH);
        Serial.printf("digitalWrite(relayPin, HIGH) time:%6ld \n",millis());
        state = 0;
  }
}
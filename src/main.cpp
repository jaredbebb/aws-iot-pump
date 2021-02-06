/**
* Example working with library v1.3
*
*/
#include <Arduino.h>
#include <Stream.h>
#include <time.h>

extern "C" {
  #include "user_interface.h"
}

#include <connect.h>

//MQTT config
const int maxMQTTpackageSize = 512;
const int maxMQTTMessageHandlers = 1;

Connection connection;


//count messages arrived
int arrivedcount = 0;

int relayPin = D8;

void setup() {  
    Serial.begin(115200);
    delay(5000); 
    connection.setup_pin(relayPin);
    connection.setup_connection();
}

void loop() {
    if (connection.getState()){
        digitalWrite(relayPin, LOW);
        Serial.printf("digitalWrite(relayPin, LOW) time:%6ld \n",millis());
        unsigned long start_time = millis();
        while( (millis()-start_time) < connection.getExecution()){
            //let esp8266 run background functions.
            yield();
        }
        digitalWrite(relayPin, HIGH);
        Serial.printf("digitalWrite(relayPin, HIGH) time:%6ld \n",millis());
        connection.setState(0);
    
        //keep the mqtt up and running
        //if (awsWSclient.connected ()) { 
        if (connection.awsWSclient_connected()) {    
            Serial.println("client was connected");
            connection.client_loop();
            connection.subscribe();
            connection.sendmessage(); 
        } else {
          //handle reconnection
          while(!connection.connect()){
            yield();
          }
            Serial.println("client was not connected");
            connection.subscribe();
            connection.sendmessage(); 
        }
    }
}
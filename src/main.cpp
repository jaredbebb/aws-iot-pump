/**
* Example working with library v1.3
*
*/
#include <Arduino.h>
#include <Stream.h>
#include <time.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

//AWS
#include "sha256.h"
#include "Utils.h"

//WEBSockets
#include <Hash.h>
#include <WebSocketsClient.h>

//MQTT PUBSUBCLIENT LIB 
#include <PubSubClient.h>

//AWS MQTT Websocket
#include "Client.h"
#include "AWSWebSocketClient.h"
#include "CircularByteBuffer.h"

//Scheduling functions
#include <Ticker.h>

#include "secrets.h"

extern "C" {
  #include "user_interface.h"
}

//MQTT config
const int maxMQTTpackageSize = 512;
const int maxMQTTMessageHandlers = 1;

ESP8266WiFiMulti WiFiMulti;

AWSWebSocketClient awsWSclient(1000);

PubSubClient client(awsWSclient);

//# of connections
long connection = 0;

//generate random mqtt clientID
char* generateClientID () {
  char* cID = new char[23]();
  for (int i=0; i<22; i+=1)
    cID[i]=(char)random(1, 256);
  return cID;
}

//count messages arrived
int arrivedcount = 0;

Ticker pumpticker;
unsigned long period = 14400*1000;
unsigned long execution = 20*1000;
int relayPin = D8;
int state;

void relay_pin_state(){
    state = 1;
    Serial.println("state = 1");
}


//callback to handle mqtt messages
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

//connects to websocket layer and mqtt layer
bool connect () {
    if (client.connected()) {    
        client.disconnect ();
    }  
    //delay is not necessary... it just help us to get a "trustful" heap space value
    delay (1000);
    Serial.print (millis ());
    Serial.print (" - conn: ");
    Serial.print (++connection);
    Serial.print (" - (");
    Serial.print (ESP.getFreeHeap ());
    Serial.println (")");


    //creating random client id
    char* clientID = generateClientID ();
    
    client.setServer(AWS_IOT_ENDPOINT, port);
    if (client.connect(clientID)) {
      Serial.println("connected");     
      return true;
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      return false;
    }
}

//subscribe to a mqtt topic
void subscribe () {
    client.setCallback(callback);
    client.subscribe(aws_topic);
   //subscript to a topic
    Serial.println("MQTT subscribed");
}

//send a message to a mqtt topic
void sendmessage () {
    //send a message   
    char buf[100];
    strcpy(buf, "{\"state\":{\"reported\":{\"on\": true}, \"desired\":{\"on\": false}}}");   
    int rc = client.publish(aws_topic, buf);
    Serial.printf("Client publish result:%d \n",rc);
}

void setClock() {
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
}

void setup() {   

    // initialize pin as output
    Serial.begin(115200);
    delay(5000);
    Serial.printf("Starting up. time:%6ld \n",millis());
    pinMode(relayPin, OUTPUT);
    state = 1;
    pumpticker.attach_ms_scheduled(period, relay_pin_state);

    wifi_set_sleep_type(NONE_SLEEP_T);

    Serial.setDebugOutput(1);

    //fill with ssid and wifi password
    WiFiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);
    Serial.println ("connecting to wifi");
    while(WiFiMulti.run() != WL_CONNECTED) {
        delay(100);
        Serial.print (".");
    }
    Serial.println ("\nconnected");
    
    setClock(); // Required for X.509 certificate  validation
    
    //fill AWS parameters    
    awsWSclient.setAWSRegion(aws_region);
    awsWSclient.setAWSDomain(AWS_IOT_ENDPOINT);
    awsWSclient.setAWSKeyID(aws_key);
    awsWSclient.setAWSSecretKey(aws_secret);
    awsWSclient.setUseSSL(true);
    awsWSclient.setCA(AWS_CERT_CA);
    //as we had to configurate ntp time to validate the certificate, we can use it to validate aws connection as well
    awsWSclient.setUseAmazonTimestamp(false);
    
    if (connect ()){
      subscribe ();
      sendmessage ();
    }
}

void loop() {
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

  //keep the mqtt up and running
  if (awsWSclient.connected ()) {    
      client.loop ();
  } else {
    //handle reconnection
    if (connect ()){
      subscribe ();      
    }
  }
}
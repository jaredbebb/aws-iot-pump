#include <Arduino.h>
#include "secrets.h"
#include "AWSWebSocketClient.h"
#include <PubSubClient.h>
#include "connect.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include "aws_root_certificate.h"


//# of connections
long connections = 0;

AWSWebSocketClient awsWSclient(1000);

PubSubClient client(awsWSclient);
ESP8266WiFiMulti WiFiMulti;

// int relayPin = D8;

int state;


char* generateClientID () {
  char* cID = new char[23]();
  for (int i=0; i<22; i+=1)
    cID[i]=(char)random(1, 256);
  return cID;
}

//callback to handle mqtt messages
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

//subscribe to a mqtt topic
void Connection::subscribe(){
    client.setCallback(callback);
    client.subscribe(aws_topic);
   //subscript to a topic
    Serial.println("MQTT subscribed");
}

//send a message to a mqtt topic
bool Connection::sendmessage(){
    //send a message   
    char buf[100];
    
    strcpy(buf, "{\"state\":{\"reported\":{\"on\": true}, \"desired\":{\"on\": true}}}");   
    bool rc = client.publish(aws_topic, buf);
    Serial.printf("Client publish result:%d \n",rc);
    return rc;
}

void setClock(){
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

bool Connection::connect(){
    if (client.connected()) {    
        client.disconnect();
    }  
    //delay is not necessary... it just help us to get a "trustful" heap space value
    delay (1000);
    Serial.print (millis ());
    Serial.print (" - conn: ");
    Serial.print (++connections);
    Serial.print (" - (");
    Serial.print (ESP.getFreeHeap ());
    Serial.println (")");

    //creating random client id
    char* clientID = generateClientID ();
    
    client.setServer(AWS_IOT_ENDPOINT, port);
    if (client.connect(clientID)) {
      Serial.println("connected");     
      delete[] clientID;
      clientID = 0;
      return true;
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      delete[] clientID;
      clientID = 0;
      return false;
    }
}

void relay_pin_state(){
    state = 1;
}

bool Connection::setup_connection(){
    // initialize pin as output

    Serial.printf("Starting up. time:%6ld \n",millis());

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
    
    if (connect()){
      subscribe ();
      sendmessage ();
    }

    // TODO need to return conditionally
    return true;
}

bool Connection::awsWSclient_connected(){
  return awsWSclient.connected();
}

void Connection::client_loop(){
  client.loop();
}

unsigned long Connection::getExecution(){
   return execution;
}

int Connection::getState(){
   return state;
}

void Connection::setState(int val){
   state = val;
}

void Connection::setup_pin(int relayPin){
  pinMode(relayPin, OUTPUT);
}


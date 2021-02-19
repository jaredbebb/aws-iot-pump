#include <Arduino.h>
#include <unity.h>
#include <connect.h>

/***
 * Test connections
 ***/
Connection connectionTest;

void  TestPublishSuccessful(){
    // To test flaky publishing Client publish result:0 , Client publish result:1 
    connectionTest.subscribe();
    TEST_ASSERT(connectionTest.sendmessage()); 
}

void TestPublishSuccessful20Times(){
    // Ensure that device can still publish with delay inbetween messages
    
    connectionTest.subscribe();
    delay(1000);
    for(int i(0); i < 20; i++){
        TEST_ASSERT(connectionTest.sendmessage());
    }
}

void TestConnectSuccessful(){
    // To test flakey connection "client was not connected"
    TEST_ASSERT(connectionTest.connect());
}

void TestPullMessageSuccessful(){
    // Test whether message truly made it to AWS IOT"
    TEST_ASSERT(0);
}

void TestConfirmNoDanglingPointers(){
    // To test that we're cleaning up pointers, managing memory
    TEST_ASSERT(0);
}


void setup() {
    Serial.begin(115200);
    delay(5000); 
    Serial.println(F("Setting up tests"));
    int relayPin = D8;
    connectionTest.setup_pin(relayPin);
    connectionTest.setup_connection();
}

uint16_t flag = 1;
void loop(){
    if (flag){
        Serial.println(F("loop test"));
        UNITY_BEGIN();
        RUN_TEST(TestPublishSuccessful);
        RUN_TEST(TestConnectSuccessful);
        RUN_TEST(TestPublishSuccessful20Times);
        UNITY_END();
        delay(2000);
        flag = 0;
    }
}
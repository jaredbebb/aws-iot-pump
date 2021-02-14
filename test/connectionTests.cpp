#include <Arduino.h>
#include <unity.h>

#include <connect.h>
#include <credential_flasher.h>


/***
 * Test connections
 ***/
Connection connectionTest;
CredentialFlasher credential_flasher(0x00074000);

void  TestPublishSuccessful(){
    // To test flaky publishing Client publish result:0 , Client publish result:1 
    connectionTest.subscribe();
    TEST_ASSERT(connectionTest.sendmessage()); 
}

void  TestPublishSuccessful20Times(){
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

/**
 * Test Flash Erase, Read, Write
 **/
void TestSPIReadSuccessfulAfterReflash(){
    credential_flasher.read();

    unsigned long sizeof_rbuff = credential_flasher.size_r_buff();
    for (int i = 0; i < sizeof_rbuff/sizeof(credential_flasher.get_r_buff(0)); ++i) {
        char rbuf_i = credential_flasher.get_r_buff(i);
        char wbuf_i = credential_flasher.get_w_buff(i);
        if (rbuf_i != wbuf_i) {
            Serial.printf("failed writing or reading at %d\n", credential_flasher.get_base_addr() + i * 4);
            Serial.printf("got %c, expected %c\n", rbuf_i, wbuf_i);
            TEST_ASSERT(0);
        }
    }
 }

 void TestSPIEraseWriteRead(){
    credential_flasher.erase();  
    credential_flasher.write();
    credential_flasher.read();
    TEST_ASSERT(1);
 }
void setup() {
    Serial.begin(115200);
    delay(5000); 
    Serial.println(F("Setting up tests"));
    int relayPin = D8;
    connectionTest.setup_pin(relayPin);
    // connectionTest.setup_connection(); //TODO uncomment this for connection
}

uint16_t flag = 1;
void loop(){
    if (flag){
        Serial.println(F("loop test"));
        UNITY_BEGIN();
        RUN_TEST(TestSPIEraseWriteRead);
        RUN_TEST(TestSPIReadSuccessfulAfterReflash); // TODO Move spi_flash capabilities. Somehow run this first after reflashing.. 
        RUN_TEST(TestSPIEraseWriteRead);
        //RUN_TEST(TestPublishSuccessful);
        //RUN_TEST(TestConnectSuccessful);
        //RUN_TEST(TestPublishSuccessful20Times);
        UNITY_END();
        delay(2000);
        flag = 0;
    }
}
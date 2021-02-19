#include <Arduino.h>
#include <unity.h>
#include <flasher.h>

extern "C" {
#include "user_interface.h"
}


/***
 * Test flashing
 ***/
ESP8266CredentialFlasher credential_flasher(0x00074000);

char buff [SPI_FLASH_SEC_SIZE / 4];
static uint32_t rbuff[SPI_FLASH_SEC_SIZE / 4];

uint32_t  base_addr = 0x00074000;


/**
 * Test Flash Erase, Read, Write
 **/
void TestSPIReadSuccessfulAfterReflash(){  
    memset(rbuff, 0, sizeof(rbuff));
    spi_flash_read(base_addr, rbuff, sizeof(rbuff));
    credential_flasher.read();
    for (int i = 0; i < credential_flasher.size_r_buff(); i++) {
        TEST_ASSERT_EQUAL_CHAR(credential_flasher.get_r_buff(i), buff[i] );
    }
 }

 void TestSPIEraseWriteRead(){
    credential_flasher.erase();
    credential_flasher.write(buff, sizeof(buff));
    credential_flasher.read();
    for (int i = 0; i < credential_flasher.size_r_buff(); i++) {
         TEST_ASSERT_EQUAL_CHAR(credential_flasher.get_r_buff(i),'k');
    }
 }

void TestFlashUtilityReadWrittenSequence(){
    //FlashUtility fu(credential_flasher);
    //std::vector<char> credentials = {'h','e','l','l','o', 'w','o','r','l','d'};
    //bool result = fu.uploadCredentials(credentials);

    //for(std::vector<char>::iterator it = credentials.begin(); it != credentials.end(); ++it ){
        //Serial.println(credential_flasher.get_r_buff(*it));
    //}

    TEST_ASSERT(0);
}

void setup() {
    Serial.begin(115200);
    delay(1000); 
    Serial.println("Setting up tests\n");
    //char buff [] = {'h','e','l','l','o','_', 'w','o','r','l','d','!'};
    
    for (uint32_t i = 0; i < sizeof(buff)/sizeof(buff[0]); ++i) {
        buff[i] =  'k';
    }
}

uint16_t flag = 1;
void loop(){
    if (flag){
        Serial.println(F("loop test"));
        UNITY_BEGIN();
        RUN_TEST(TestSPIEraseWriteRead);
        RUN_TEST(TestSPIReadSuccessfulAfterReflash);
        //RUN_TEST(TestSPIEraseWriteRead);
        //RUN_TEST(TestFlashUtilityReadWrittenSequence);
        UNITY_END();
        delay(2000);
        flag = 0;
    }
}
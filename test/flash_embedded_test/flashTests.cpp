#include <Arduino.h>
#include <unity.h>
#include <flasher.h>
#include <credential_utility.h>

extern "C" {
#include "user_interface.h"
}

uint32_t base_addr = 0x00074000;

/**
 * Test Flash Erase, Read, Write
 **/
void TestSPIEraseWriteRead(){
    ESP8266CredentialFlasher credential_flasher(base_addr);
    char buff [SPI_FLASH_SEC_SIZE / 4];

    // fill global char [] buff with default value
    std::fill_n(buff, sizeof(buff)/sizeof(buff[0]), 'k' );

    credential_flasher.erase();
    credential_flasher.write(buff, sizeof(buff));
    credential_flasher.read();
    for (int i = 0; i < credential_flasher.size_r_buff(); i++) {
         TEST_ASSERT_EQUAL_CHAR(buff[i], credential_flasher.get_r_buff(i));
    }
 }


void TestFlashUtilityReadWrittenSequence(){
    
    ESP8266CredentialFlasher credential_flasher(base_addr);
    FlashUtility fu;

    char delimeter = ';';
    char buff [SPI_FLASH_SEC_SIZE / 4];
    std::fill_n(buff, sizeof(buff), delimeter );

    char passw[]  = {'h','e','l','l','o', 'w','o','r','l','d'};

    std::copy(passw, passw+sizeof(passw)/sizeof(passw)[0],buff);
    
    credential_flasher.erase();
    bool result = fu.writeCredentials(buff, sizeof(buff), & credential_flasher);

    credential_flasher.read();
    for(int i = 0; i < sizeof(buff); i++){
        TEST_ASSERT_EQUAL_CHAR(buff[i], credential_flasher.get_r_buff(i) );
    }
    Serial.println();
}

void TestCredentialUtilityConstruct(){
    char delimeter = ';';
    char buff [SPI_FLASH_SEC_SIZE / 4];
    std::fill_n(buff, sizeof(buff), delimeter );

    char header[] = {'<','<','<','<'};
    char passw[]  = {'h','e','l','l','o', 'w','o','r','l','d'};
    char footer[] = {'>','>','>','>'};

    CredentialUtility cu(sizeof(header), sizeof(footer), sizeof(passw),sizeof(buff));
    cu.construct(header,footer,passw,delimeter,buff);
    TEST_ASSERT_EQUAL_CHAR(header[0], buff[0]);
}

void TestCredentialUtilityExtract(){
    char delimeter = ';';
    char buff [SPI_FLASH_SEC_SIZE / 4];
    std::fill_n(buff, sizeof(buff), delimeter );

    char header[] = {'<','<','<','<'};
    char passw[]  = {'h','e','l','l','o', 'w','o','r','l','d'};
    char footer[] = {'>','>','>','>'};

    CredentialUtility cu(sizeof(header), sizeof(footer), sizeof(passw),sizeof(buff));
    cu.construct(header,footer,passw,delimeter,buff);

    char extract_buff [sizeof(passw)];
    cu.extract(buff, extract_buff);
    for(int i = 0; i < sizeof(passw); i++){
        TEST_ASSERT_EQUAL_CHAR(passw[i], extract_buff[i]);
        Serial.printf("passw[i]=%c extract_buff[i]=%c \n",passw[i], extract_buff[i] );
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000); 
    Serial.println("Setting up tests\n");
}

uint16_t flag = 1;
void loop(){
    if (flag){
        Serial.println(F("loop test"));
        UNITY_BEGIN();
        RUN_TEST(TestSPIEraseWriteRead);
        RUN_TEST(TestFlashUtilityReadWrittenSequence);
        RUN_TEST(TestCredentialUtilityConstruct);
        RUN_TEST(TestCredentialUtilityExtract);
        UNITY_END();
        delay(2000);
        flag = 0;
    }
}
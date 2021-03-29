#include <Arduino.h>
#include <unity.h>
#include <flasher.h>
#include <credential_utility.h>
#include "secrets_new.h"

extern "C" {
#include "user_interface.h"
}

uint32_t base_addr = 0x00074000;


void TestCredentialWrite(){
    char delimeter = ';';
    char buff [SPI_FLASH_SEC_SIZE / 4];
    std::fill_n(buff, sizeof(buff), delimeter );

    char header[] = {'<','<','<','<'};
    char footer[] = {'>','>','>','>'};

    CredentialUtility cu(sizeof(header), sizeof(footer), sizeof(AWS_SECRET),sizeof(buff));
    cu.construct(header,footer,AWS_SECRET,delimeter,buff);

    ESP8266CredentialFlasher credential_flasher(base_addr);
    credential_flasher.erase();
    FlashUtility fu;
    bool result = fu.writeCredentials(buff, sizeof(buff), & credential_flasher);
}

void TestCredentialUtilityExtract(){
    char delimeter = ';';
    char buff [SPI_FLASH_SEC_SIZE / 4];
    std::fill_n(buff, sizeof(buff), delimeter );

    char header[] = {'<','<','<','<'};
    char footer[] = {'>','>','>','>'};

    CredentialUtility cu(sizeof(header), sizeof(footer), sizeof(AWS_SECRET),sizeof(buff));
    cu.construct(header,footer,AWS_SECRET,delimeter,buff);

    char extract_buff [sizeof(AWS_SECRET)];
    cu.extract(buff, extract_buff);
    for(int i = 0; i < sizeof(AWS_SECRET); i++){
        TEST_ASSERT_EQUAL_CHAR(AWS_SECRET[i], extract_buff[i]);
        Serial.printf("AWS_SECRET[i]=%c extract_buff[i]=%c \n",AWS_SECRET[i], extract_buff[i] );
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
        RUN_TEST(TestCredentialWrite);
        RUN_TEST(TestCredentialUtilityExtract);
        UNITY_END();
        delay(2000);
        flag = 0;
    }
}
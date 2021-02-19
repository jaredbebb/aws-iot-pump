#include <Arduino.h>
#include "flasher.h"
#include <spi_flash.h>


/**
 * Utilizes SPI read, write and erase functions. 
 * TODO research using partition scheme for credentials to avoid large builds overwritting credentials
**/

ESP8266CredentialFlasher::ESP8266CredentialFlasher(uint32_t _base_addr){
    base_addr = _base_addr;
}

void ESP8266CredentialFlasher::read() {
    spi_flash_read(base_addr, (uint32*) r_buff_, sizeof(r_buff_));
}

void ESP8266CredentialFlasher::write(char * w_buff, uint32 buff_size) const{
    for(int i = 0; i < 12; i++){
        Serial.printf("w_buff[i] = %c; Size of w_buff = %d \n", w_buff[i], buff_size);
    }
    printf("writing at %x\n", base_addr);
    spi_flash_write(base_addr, reinterpret_cast<uint32 *>(w_buff), SPI_FLASH_SEC_SIZE / 4);
}

void ESP8266CredentialFlasher::erase() const{
    spi_flash_erase_sector(base_addr / SPI_FLASH_SEC_SIZE);
}

uint32_t ESP8266CredentialFlasher::get_base_addr() const{
    return base_addr;
}

unsigned long ESP8266CredentialFlasher::size_r_buff() const{
    return sizeof(r_buff_);
}

char ESP8266CredentialFlasher::get_r_buff(uint32_t i) const{
    return r_buff_[i];
}

FlashUtility::FlashUtility(ESP8266CredentialFlasher credential_flasher){
    credential_flasher_ = & credential_flasher;
}

//Utility to loop over list and flash to memory in csv format
bool FlashUtility::uploadCredentials(std::vector<char> creds){
    for(std::vector<char>::iterator it = creds.begin(); it != creds.end(); ++it )
    {
        //credential_flasher_->write(*it);

    }
    return false;
}

FlashUtility::~FlashUtility(){
    delete credential_flasher_;
    credential_flasher_ = 0;
}


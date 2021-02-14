#include <Arduino.h>
#include "credential_flasher.h"
#include <spi_flash.h>


CredentialFlasher::CredentialFlasher(uint32_t _base_addr){
    base_addr = _base_addr;
}

void CredentialFlasher::read(){
    spi_flash_read(base_addr, (uint32*) r_buff, sizeof(r_buff));
}

void CredentialFlasher::write(){

    spi_flash_write(base_addr, (uint32*) w_buff, sizeof(w_buff));
}

void CredentialFlasher::erase(){
    spi_flash_erase_sector(base_addr / SPI_FLASH_SEC_SIZE);
}

uint32_t CredentialFlasher::get_base_addr(){
    return base_addr;
}

unsigned long CredentialFlasher::size_r_buff(){
    return sizeof(r_buff);
}

char CredentialFlasher::get_r_buff(uint32_t i){
    return r_buff[i];
}

char CredentialFlasher::get_w_buff(uint32_t i){
    return w_buff[i];
}

unsigned long CredentialFlasher::size_w_buff(){
    return sizeof(w_buff);
}



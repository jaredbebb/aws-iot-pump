#include <Arduino.h>
#include "credential_utility.h"

CredentialUtility::CredentialUtility(uint32 header_size, uint32 footer_size, uint32 passwords_size , uint32 buff_size){
    header_size_ = header_size;
    footer_size_ = footer_size;
    passwords_size_ = passwords_size;
    buff_size_ =  buff_size;

}

bool CredentialUtility::construct(char * header, char * footer, char * passwords, char delimeter, char * buff){
    Serial.printf("header_size_=%d passwords_size_=%d footer_size_=%d \n", header_size_, passwords_size_, footer_size_);
    //construct header
    int i = 0;
    for( i; i < header_size_; i ++){
        buff[i] = header[i];
    }

    //construct passwords
    int p = 0;
    for(p; p < passwords_size_; p++){
        buff[p+i] = passwords[p];
    }

    //construct footer
    for(int k = 0; k < footer_size_; k ++){
        buff[k+p+i] = footer[k];
    }
}

bool CredentialUtility::extract(char * buff, char * extract){

    uint32 i = header_size_;
    for(uint32 j = 0; j < passwords_size_; j++, i++){
        extract[j] = buff[i];
    }
    return true;
}
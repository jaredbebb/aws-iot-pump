class ESP8266CredentialFlasher{
    public:
        ESP8266CredentialFlasher(uint32_t _base_addr);

        void read() ;
        int write(char * w_buff, uint32 buff_size) const;
        void erase() const;
        unsigned long size_r_buff() const;
        unsigned long size_w_buff() const;
        char get_r_buff(uint32_t i) const;
        uint32_t get_base_addr() const;

    private:
        uint32_t base_addr;
        char r_buff_[SPI_FLASH_SEC_SIZE / 4];
};

class FlashUtility{
    public:
        bool writeCredentials(char * w_buff, uint32 buff_size, ESP8266CredentialFlasher * credential_flasher);
};

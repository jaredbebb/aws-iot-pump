class CredentialFlasher{
    public:
        CredentialFlasher(uint32_t _base_addr);
        void read();
        void write();
        void erase();
        unsigned long size_r_buff();
        unsigned long size_w_buff();
        char get_r_buff(uint32_t i);
        char get_w_buff(uint32_t i);
        uint32_t get_base_addr();

    private:
        uint32_t base_addr;
        char r_buff[SPI_FLASH_SEC_SIZE / 4];
        char w_buff[SPI_FLASH_SEC_SIZE / 4];
};
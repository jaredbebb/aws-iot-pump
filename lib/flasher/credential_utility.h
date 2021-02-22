class CredentialUtility{
    public:

        CredentialUtility( uint32 header_size, uint32 footer_size, uint32 passwords_size , uint32 buff_size);
        bool construct(char * header, char * footer,char * passwords, char delimeter, char * buff);
        bool extract(char * buff, char * extract);

    private:
        uint32 header_size_ = 0;
        uint32 footer_size_ = 0;
        uint32 passwords_size_ = 0;
        uint32 buff_size_ = 0;
};
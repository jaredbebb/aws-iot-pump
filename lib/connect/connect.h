#include <Ticker.h>

class Connection{
    public:
        bool connect();
        bool setup_connection();
        void subscribe();
        bool sendmessage();

        bool awsWSclient_connected();
        void client_loop();
        
        // TODO  functions and variables unrelated to Connection. Need to manage spaghetti code ..
        void setup_pin(int relayPin);
        unsigned long getExecution();
        
        unsigned long execution = 20*1000;
        unsigned long period = 60 * 1000;//14400*1000;

        int getState();
        void setState(int val);

        Ticker pumpticker;
};
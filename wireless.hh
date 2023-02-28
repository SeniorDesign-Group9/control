// wireless.hh

#ifndef WIRELESS_H
#define WIRELESS_H

#define SPAWN_TASK_PRIORITY     9
#define TASK_STACK_SIZE         2048
#define SL_STOP_TIMEOUT         0xFFFF // 200

// WiFi (station) class
class Wireless {
    public:
        // Singleton functions
        // Get Wireless instance
        static Wireless& instance();

        // Disallow copying
        Wireless& operator = (const Wireless&) = delete;
        Wireless(const Wireless&) = delete;

        // Disallow moving
        Wireless& operator = (Wireless&&) = delete;
        Wireless(Wireless&&) = delete;

        // Class functions
        void init();
        /*
        void SimpleLinkFatalErrorEventHandler();
        void SimpleLinkGeneralEventHandler();
        void SimpleLinkHttpServerEventHandler();
        void SimpleLinkNetAppEventHandler();
        void SimpleLinkNetAppRequestEventHandler();
        void SimpleLinkNetAppRequestMemFreeEventHandler();
        void SimpleLinkSockEventHandler();
        void SimpleLinkWlanEventHandler();
        */
    
    private:
        // Singleton variables
        Wireless();
        ~Wireless();

        // Class variables

};

#endif // WIRELESS_H

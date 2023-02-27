// wireless.hh

#ifndef WIRELESS_H
#define WIRELESS_H

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

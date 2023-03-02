// wireless.hh

#ifndef WIRELESS_H
#define WIRELESS_H

#include <stdint.h>

#define SL_STOP_TIMEOUT                 200     // sl_Stop timeout in ms
#define PROVISIONING_INACTIVITY_TIMEOUT 1200    // Provisioning inactivity timeout in s

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
        // Start and provision NWP
        int start(void);
        // Stop NWP
        int stop(void);
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
        const char *device_name = "GARDENIRS";
        const char *domain_name = "www.mygardenirs.net";

};

#endif // WIRELESS_H

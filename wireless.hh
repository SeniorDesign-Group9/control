// wireless.hh

#ifndef WIRELESS_H
#define WIRELESS_H

#define SL_STOP_TIMEOUT                 200     // sl_Stop timeout in ms
#define PROVISIONING_INACTIVITY_TIMEOUT 1200    // Provisioning inactivity timeout in s

// WiFi class
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

        int haltProvisioning(void);
    
    private:
        // Singleton variables
        Wireless();
        ~Wireless();

        // Class variables
        const char *device_name = "GARDENIRS";
        const char *domain_name = "gardenirs.localdomain";
};

#endif // WIRELESS_H

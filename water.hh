#ifndef WATER_H
#define WATER_H

// Water solenoid class
class WaterSolenoid {
    public:
        // Singleton functions
        // Get WaterSolenoid instance
        static WaterSolenoid& instance();

        // Disallow copying
        WaterSolenoid& operator = (const WaterSolenoid&) = delete;
        WaterSolenoid(const WaterSolenoid&) = delete;

        // Disallow moving
        WaterSolenoid& operator = (WaterSolenoid&&) = delete;
        WaterSolenoid(WaterSolenoid&&) = delete;

        // Class functions
        // Set water solenoid on or off
        void waterSet(bool on);

        // Toggle water solenoid
        void waterToggle(void);

        // Set GPIO
        void waterConfig(unsigned int gpio);
    
    private:
        // Singleton variables
        WaterSolenoid();
        ~WaterSolenoid();

        // Class variables
        unsigned int waterGpio;
        bool waterFlow;
};

#endif // WATER_H

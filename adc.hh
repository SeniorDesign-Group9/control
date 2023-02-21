// adc.hh

#ifndef ADC_H
#define ADC_H

class AdcInternal { 
    public:
        // Singleton functions
        // Get AdcInternal instance
        static AdcInternal& instance();

        // Disallow copying
        AdcInternal& operator = (const AdcInternal&) = delete;
        AdcInternal(const AdcInternal&) = delete;

        // Disallow moving
        AdcInternal& operator = (AdcInternal&&) = delete;
        AdcInternal(AdcInternal&&) = delete;

        // Class functions
        // int n getter
        int get_n(void);

        // int n setter
        void set_n(int x);
    
    private:
        // Singleton variables
        AdcInternal();
        ~AdcInternal();

        // Class variables
        int n;
};

#endif // ADC_H

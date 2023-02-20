#ifndef ADC_H
#define ADC_H

// ****************************************************************************
// *                                ADC CLASS                                 *
// ****************************************************************************
// Based on the Meyers Singleton, apparently thread-safe since C++11
// The C++ Programming Language, §8.2.3 Structures and Classes
class AdcInternal { 
    public:
        // Get AdcInternal instance
        static AdcInternal& instance();

        // int n getter
        int get_n(void);

        // int n setter
        void set_n(int x);

        // Disallow copying
        AdcInternal& operator = (const AdcInternal&) = delete;
        AdcInternal(const AdcInternal&) = delete;

        // Disallow moving
        AdcInternal& operator = (AdcInternal&&) = delete;
        AdcInternal(AdcInternal&&) = delete;
    
    private:
        // int n
        AdcInternal();
        ~AdcInternal();
        int n;
};
// ****************************************************************************
// *                              END ADC CLASS                               *
// ****************************************************************************

#endif // ADC_H

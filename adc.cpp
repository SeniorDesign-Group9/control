#include "adc.hh"

// ****************************************************************************
// *                                ADC CLASS                                 *
// ****************************************************************************
// Empty constructor
AdcInternal::AdcInternal() {}

// Empty destructor
AdcInternal::~AdcInternal() {}

// Based on the Meyers Singleton, apparently thread-safe since C++11
// The C++ Programming Language, §8.2.3 Structures and Classes
AdcInternal& AdcInternal::instance() {
    static AdcInternal inst;
    return inst;
}

// int n getter
int AdcInternal::get_n(void) {
    return n;
}

// int n setter
void AdcInternal::set_n(int x) {
    n = x;
    return;
}
// ****************************************************************************
// *                              END ADC CLASS                               *
// ****************************************************************************

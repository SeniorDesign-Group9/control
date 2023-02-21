// adc.cpp

#include "adc.hh"

// Singleton functions
// Empty constructor
AdcInternal::AdcInternal() {}

// Empty destructor
AdcInternal::~AdcInternal() {}

// Instance
AdcInternal& AdcInternal::instance() {
    static AdcInternal inst;
    return inst;
}

// Class functions
// int n getter
int AdcInternal::get_n(void) {
    return n;
}

// int n setter
void AdcInternal::set_n(int x) {
    n = x;
    return;
}

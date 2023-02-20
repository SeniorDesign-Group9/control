#include "adc.hh"

// ****************************************************************************
// *                                ADC CLASS                                 *
// ****************************************************************************
// Empty constructor
WaterSolenoid::WaterSolenoid() {
    waterGpio = VALVE_EN;
}

// Empty destructor
WaterSolenoid::~WaterSolenoid() {}

// Based on the Meyers Singleton, apparently thread-safe since C++11
// The C++ Programming Language, §8.2.3 Structures and Classes
WaterSolenoid& WaterSolenoid::instance() {
    static WaterSolenoid inst;
    return inst;
}

// int n getter
void WaterSolenoid::waterSet(bool on) {
    int
    return n;
}

// int n setter
void WaterSolenoid::waterToggle(void) {
    n = x;
    return;
}
// ****************************************************************************
// *                              END ADC CLASS                               *
// ****************************************************************************

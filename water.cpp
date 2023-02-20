//#include <ti/drivers/GPIO.h>
#include "water.hh"

// ****************************************************************************
// *                                ADC CLASS                                 *
// ****************************************************************************
// Empty constructor
WaterSolenoid::WaterSolenoid() {}

// Empty destructor
WaterSolenoid::~WaterSolenoid() {}

// Based on the Meyers Singleton, apparently thread-safe since C++11
// The C++ Programming Language, §8.2.3 Structures and Classes
WaterSolenoid& WaterSolenoid::instance() {
    static WaterSolenoid inst;
    return inst;
}

// Set GPIO
void WaterSolenoid::waterConfig(unsigned int gpio) {
    //waterGpio = gpio;
}

// Set water solenoid on or off
void WaterSolenoid::waterSet(bool on) {
    //GPIO_write(waterGpio, on);
    //waterFlow = on;
    return;
}

// Toggle water solenoid
void WaterSolenoid::waterToggle(void) {
    //GPIO_toggle(waterGpio);
    //waterFlow = !waterFlow;
    return;
}

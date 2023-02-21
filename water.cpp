// water.cpp

#include <ti/drivers/GPIO.h>
#include "ti_drivers_config.h"
#include "water.hh"

// Singleton functions
// Empty constructor
WaterSolenoid::WaterSolenoid() {
    waterGpio = VALVE_EN;                       // Solenoid GPIO is VALVE_EN
}

// Empty destructor
WaterSolenoid::~WaterSolenoid() {}

// Instance
WaterSolenoid& WaterSolenoid::instance() {
    static WaterSolenoid inst;
    return inst;
}

// Class functions
// Set water solenoid on or off
void WaterSolenoid::waterSet(bool on) {
    GPIO_write(waterGpio, (on ? GPIO_CFG_OUT_HIGH : GPIO_CFG_OUT_LOW));
    waterFlow = on;
    return;
}

// Toggle water solenoid
void WaterSolenoid::waterToggle(void) {
    GPIO_toggle(waterGpio);
    waterFlow = !waterFlow;
    return;
}

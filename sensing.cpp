// water.cpp

#include <unistd.h>
#include <ti/drivers/GPIO.h>
#include "ti_drivers_config.h"
#include "sensing.hh"
#include "adc.hh"


// Empty constructor
Sensing::Sensing() {
    lampGpio = LAMP_EN;
}

// Empty destructor
Sensing::~Sensing() {}

// Instance
Sensing& Sensing::instance() {
    static Sensing inst;
    return inst;
}

// Class functions
void Sensing::getResult(DRV8833& motor) {
    GPIO_write(AMBER_LED, 1);

    lampSet(true);
    usleep(250 * 1000);          // Sleep for 250ms to allow lamp to turn on

    // Increment through each position and take a reading
    for (int i = 0; i < POSITIONS; i++) {
        results.nir_results.at(i) = AdcExternal::instance().getRawResult(AdcExternal::CH0);
        results.vis_results.at(i) = AdcExternal::instance().getRawResult(AdcExternal::CH1);

        //printf("P %03d\n", i);
        //printf("   NIR:     %.3f V\n", resultRawToFloat(nir_result));
        //printf("   Visible: %.3f V\n\n", resultRawToFloat(vis_result));

        motor.stepPosition((20000 - ((20000/POSITIONS) * i)), 800);        // 10k is max position
    }

    lampSet(false);
    results.time = std::time(nullptr);

    GPIO_write(AMBER_LED, 0);
}

inline float Sensing::resultRawToFloat(uint16_t raw_result) {
    return static_cast<float>(raw_result) / static_cast<float>(ONE_VOLT);
}

// Set lamp on or off
void Sensing::lampSet(bool on) {
    GPIO_write(lampGpio, (on ? GPIO_CFG_OUT_HIGH : GPIO_CFG_OUT_LOW));
    lampOn = on;
    return;
}

// Toggle lamp
void Sensing::lampToggle(void) {
    GPIO_toggle(lampGpio);
    lampOn = !lampOn;
    return;
}

result_t Sensing::accessResult(void) {
    return results;
}



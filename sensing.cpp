// water.cpp

#include <unistd.h>
#include <ti/drivers/GPIO.h>
#include "ti_drivers_config.h"
#include "sensing.hh"

// Empty constructor
Sensing::Sensing() {}

// Empty destructor
Sensing::~Sensing() {}

// Instance
Sensing& Sensing::instance() {
    static Sensing inst;
    return inst;
}

// Step motor forward 1 step
void DRV8833::stepForwardOne(void) {
    stepSteps(static_cast<int32_t>(1));
    return;
}

// Step motor backward 1 step
void DRV8833::stepBackwardOne(void) {
    stepSteps(static_cast<int32_t>(-1));
    return;
}

// Step motor to position pos
void DRV8833::stepPosition(uint32_t desired_pos) {
    int32_t steps = 0;

    // FIXME: Lock current_pos
    steps = desired_pos - current_pos;
    // FIXME: Unlock current_pos
    stepSteps(steps);

    return;
}

// Step motor to position 0
void DRV8833::stepZero(void) {
    int32_t steps = 0;

    // FIXME: Lock current_pos
    steps = -current_pos;
    // FIXME: Unlock current_pos
    stepSteps(steps);

    return;
}

// Step motor to position max_steps
void DRV8833::stepMax(void) {
    int32_t steps = 0;

    // FIXME: Lock current_pos
    steps = MAX_STEP - current_pos;
    // FIXME: Unlock current_pos
    stepSteps(steps);

    return;
}



// water.cpp

#include <unistd.h>
#include <ti/drivers/GPIO.h>
#include "ti_drivers_config.h"
#include "sensing.hh"

// Singleton functions
// Empty constructor
DRV8833::DRV8833() {
    current_pos = 0;                // FIXME: Find current pos
    step_delay = 0;
}

// Empty destructor
DRV8833::~DRV8833() {}

// Instance
DRV8833& DRV8833::instance() {
    static DRV8833 inst;
    return inst;
}

// Empty constructor
Sensing::Sensing() {}

// Empty destructor
Sensing::~Sensing() {}

// Instance
Sensing& Sensing::instance() {
    static Sensing inst;
    return inst;
}

// Class functions
// Step motor steps number of steps (neg is backward)
void DRV8833::stepSteps(int32_t steps) {
    int32_t steps_left = 0;
    int32_t direction = 0;
    int32_t step_number = 0;

    if (steps > 0) {
      direction = 1;
      steps_left = steps;
    }
    if (steps < 0) {
      direction = 0;
      steps_left = -steps;
    }

    while (steps_left > 0) {
        if (direction > 0) {
          step_number = (step_number + 1) % 4;
        } else {
          step_number = (step_number + 3) % 4;
        }
        stepMotor(step_number);
        steps_left--;
        //sleep(this->step_delay);            // FIXME
    }

    return;
}

// Driver function to step motor in certain way
void DRV8833::stepMotor(int32_t step) {
    switch(step) {
        case 0: // 1010
            GPIO_write(AIN1, GPIO_CFG_OUT_HIGH);
            GPIO_write(AIN2, GPIO_CFG_OUT_LOW);
            GPIO_write(BIN1, GPIO_CFG_OUT_HIGH);
            GPIO_write(BIN2, GPIO_CFG_OUT_LOW);
        break;
        case 1: // 0110
            GPIO_write(AIN1, GPIO_CFG_OUT_LOW);
            GPIO_write(AIN2, GPIO_CFG_OUT_HIGH);
            GPIO_write(BIN1, GPIO_CFG_OUT_HIGH);
            GPIO_write(BIN2, GPIO_CFG_OUT_LOW);
        break;
        case 2: // 0101
            GPIO_write(AIN1, GPIO_CFG_OUT_LOW);
            GPIO_write(AIN2, GPIO_CFG_OUT_HIGH);
            GPIO_write(BIN1, GPIO_CFG_OUT_LOW);
            GPIO_write(BIN2, GPIO_CFG_OUT_HIGH);
        break;
        case 3: // 1001
            GPIO_write(AIN1, GPIO_CFG_OUT_HIGH);
            GPIO_write(AIN2, GPIO_CFG_OUT_LOW);
            GPIO_write(BIN1, GPIO_CFG_OUT_LOW);
            GPIO_write(BIN2, GPIO_CFG_OUT_HIGH);
        break;
        default:
            // FIXME: brake/stop
        break;
    }

    return;
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



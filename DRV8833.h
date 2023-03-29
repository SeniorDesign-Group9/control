//
// Created by Nicholas Chitty on 3/13/23.
//

#ifndef CONTROLLER_DRV8833_H
#define CONTROLLER_DRV8833_H

#include <ti/devices/cc32xx/inc/hw_memmap.h>

#define A_PORT   GPIOA1_BASE
#define B_PORT   GPIOA2_BASE
#define AIN1_BIT    0b00000100      // GPIOA1 [2] GPIO10
#define AIN2_BIT    0b00001000      // GPIOA1 [3] GPIO11
#define BIN1_BIT    0b00000001      // GPIOA2 [0] GPIO16
#define BIN2_BIT    0b00000010      // GPIOA2 [1] GOIO17

class DRV8833 {
public:
    DRV8833(uint_least8_t pin_a1, uint_least8_t pin_a2, uint_least8_t pin_b1,
            uint_least8_t pin_b2, uint_least8_t pin_fault);

    ~DRV8833();

    // Step motor steps number of steps (neg is backward)
    void stepSteps(int32_t steps, uint32_t rpm);

    // Step motor to position pos
    void stepPosition(uint32_t pos, uint32_t umps);

    // Step motor to position 0
    void stepZero(uint32_t umps);

    // Step motor to position max_steps
    void stepMax(uint32_t umps);

    void calibrate(uint32_t rpm);

    void stop(uint_least8_t pin);

private:
    // Class variables
    // todo: calculate MAX_POS
    const uint32_t MAX_POS = UINT32_MAX / 2;
    const uint_least8_t a1, a2, b1, b2, fault;
    // todo: convert to wavelength
    uint32_t current_pos;

    // Helper function to step motor
    void stepMotor(int step);
};


#endif //CONTROLLER_DRV8833_H

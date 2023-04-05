//
// Created by Nicholas Chitty on 3/13/23.
//
#include <unistd.h>
#include <cstdio>
#include <ti/drivers/GPIO.h>
#include <ti/devices/cc32xx/driverlib/gpio.h>
#include "DRV8833.h"


// This expects that the pins are correctly configured, the motor pins should be set as
// output, with fault going to NFAULT setup for interrupt on falling edge, when the system faults
// the motor will try to stop the motor;
DRV8833::DRV8833(uint_least8_t
                 pin_a1,
                 uint_least8_t pin_a2, uint_least8_t
                 pin_b1,
                 uint_least8_t pin_b2, uint_least8_t
                 pin_fault) :
        a1(pin_a1), a2(pin_a2), b1(pin_b1), b2(pin_b2),
        fault(pin_fault) {
    GPIO_enableInt(pin_fault);
    GPIODirModeSet(A_PORT, AIN1_BIT, GPIO_DIR_MODE_OUT);
    GPIODirModeSet(A_PORT, AIN2_BIT, GPIO_DIR_MODE_OUT);
    GPIODirModeSet(B_PORT, BIN1_BIT, GPIO_DIR_MODE_OUT);
    GPIODirModeSet(B_PORT, BIN2_BIT, GPIO_DIR_MODE_OUT);
}

// Step the motor a number of steps
void DRV8833::stepSteps(int32_t steps, uint32_t rpm) {
    int32_t steps_left = 0;
    int32_t direction = 0;
    int32_t step_number = 0;
    // us/step = (rpm * 200 steps/min * 1 min/60s * 1 s/1e6 us)^-1
    useconds_t step_delay = (60 * 1000000) / (rpm * 200);

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
        this->stepMotor(step_number);
        steps_left--;

        if (direction) {
            this->current_pos = current_pos < MAX_POS ? this->current_pos + 1 : this->MAX_POS;
        } else {
            this->current_pos = current_pos > 0 ? this->current_pos - 1 : 0;
        }

        usleep(step_delay);
    }

    return;
}

void DRV8833::stepPosition(uint32_t pos, uint32_t umps) {
    int32_t steps = pos - this->current_pos;
    uint32_t rpm = (umps * 60 * 200) / 5;

    this->stepSteps(steps, rpm);
}

void DRV8833::stepZero(uint32_t umps) {
    this->stepPosition(0, umps);
}

void DRV8833::stepMax(uint32_t umps) {
    this->stepPosition(this->MAX_POS, umps);
}

void DRV8833::stop(void) {
    this->stepMotor(-1);
}

// Driver function to step motor in certain way
void DRV8833::stepMotor(int32_t step) {
    unsigned char a_mask = 0x00;
    unsigned char b_mask = 0x00;
    switch (step) {
        case 0: // 1010
            a_mask = GPIOPinRead(A_PORT, 0xFF) & ~AIN2_BIT | AIN1_BIT;
            b_mask = GPIOPinRead(B_PORT, 0xFF) & ~BIN2_BIT | BIN1_BIT;
            GPIOPinWrite(A_PORT, AIN1_BIT | AIN2_BIT, a_mask);
            GPIOPinWrite(B_PORT, BIN1_BIT | BIN2_BIT, b_mask);
            break;
        case 1: // 0110
            a_mask = GPIOPinRead(A_PORT, 0xFF) & ~AIN1_BIT | AIN2_BIT;
            b_mask = GPIOPinRead(B_PORT, 0xFF) & ~BIN2_BIT | BIN1_BIT;
            GPIOPinWrite(A_PORT, AIN1_BIT | AIN2_BIT, a_mask);
            GPIOPinWrite(B_PORT, BIN1_BIT | BIN2_BIT, b_mask);
            break;
        case 2: // 0101
            a_mask = GPIOPinRead(A_PORT, 0xFF) & ~AIN1_BIT | AIN2_BIT;
            b_mask = GPIOPinRead(B_PORT, 0xFF) & ~BIN1_BIT | BIN2_BIT;
            GPIOPinWrite(A_PORT, AIN1_BIT | AIN2_BIT, a_mask);
            GPIOPinWrite(B_PORT, BIN1_BIT | BIN2_BIT, b_mask);
            break;
        case 3: // 1001
            a_mask = GPIOPinRead(A_PORT, 0xFF) & ~AIN2_BIT | AIN1_BIT;
            b_mask = GPIOPinRead(B_PORT, 0xFF) & ~BIN1_BIT | BIN2_BIT;
            GPIOPinWrite(A_PORT, AIN1_BIT | AIN2_BIT, a_mask);
            GPIOPinWrite(B_PORT, BIN1_BIT | BIN2_BIT, b_mask);
            break;
        default: // Brake
            a_mask = GPIOPinRead(A_PORT, 0xFF) | (AIN2_BIT | AIN1_BIT);
            b_mask = GPIOPinRead(B_PORT, 0xFF) | (BIN2_BIT | BIN1_BIT);
            GPIOPinWrite(A_PORT, AIN1_BIT | AIN2_BIT, a_mask);
            GPIOPinWrite(B_PORT, BIN1_BIT | BIN2_BIT, b_mask);
            break;
    }

    return;
}

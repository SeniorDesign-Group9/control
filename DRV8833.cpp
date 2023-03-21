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
    // GPIO_setCallback(
    //        pin_fault,
    //        DRV8833::stop
    // );
    GPIO_enableInt(pin_fault);
    GPIODirModeSet(AIN1_PORT, AIN1_BIT, GPIO_DIR_MODE_OUT);
    GPIODirModeSet(AIN2_PORT, AIN2_BIT, GPIO_DIR_MODE_OUT);
    GPIODirModeSet(BIN1_PORT, BIN1_BIT, GPIO_DIR_MODE_OUT);
    GPIODirModeSet(BIN2_PORT, BIN2_BIT, GPIO_DIR_MODE_OUT);
}

// Step the motor a number of steps
void DRV8833::stepSteps(int32_t steps, uint32_t rpm) {
    int32_t steps_left = 0;
    int32_t direction = 0;
    int32_t step_number = 0;
    // RPM * 200 steps/rev * us/min
    useconds_t step_delay = rpm * 200 / (60 * 100000);

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

        // todo fix position finding logic
        this->current_pos = direction == 1 ? current_pos + 1 : current_pos - 1;

        usleep(step_delay);
    }

    return;
}

void DRV8833::stepPosition(uint32_t pos, uint32_t rpm) {
    // todo add some conversion for wavelength and real distance
    int32_t steps = this->current_pos - pos;

    this->stepSteps(steps, rpm);
}

void DRV8833::stepZero(uint32_t rpm) {
    this->stepPosition(0, rpm);
}

void DRV8833::stepMax(uint32_t rpm) {
    this->stepPosition(this->MAX_POS, rpm);
}

void DRV8833::calibrate(uint32_t rpm) {
    while (GPIO_read(this->fault)) {
        this->stepSteps(-4, rpm);
    }
    GPIO_clearInt(this->fault);
    this->current_pos = 0;
}

void DRV8833::stop(uint_least8_t pin) {
    GPIOPinWrite(AIN1_PORT, AIN1_BIT, AIN1_BIT);
    GPIOPinWrite(AIN2_PORT, AIN2_BIT, AIN1_BIT);
    GPIOPinWrite(BIN1_PORT, BIN1_BIT, BIN1_BIT);
    GPIOPinWrite(BIN2_PORT, BIN2_BIT, AIN1_BIT);
    GPIO_clearInt(this->fault);
}

// Driver function to step motor in certain way
void DRV8833::stepMotor(int32_t step) {
    printf("stepMotor\n");
    switch (step) {
        case 0: // 1010
            GPIOPinWrite(AIN1_PORT, AIN1_BIT, AIN1_BIT);
            GPIOPinWrite(AIN2_PORT, AIN2_BIT, 0);
            GPIOPinWrite(BIN1_PORT, BIN1_BIT, BIN1_BIT);
            GPIOPinWrite(BIN2_PORT, BIN2_BIT, 0);
            /*
            GPIO_write(this->a1, GPIO_CFG_OUT_HIGH);
            GPIO_write(this->a2, GPIO_CFG_OUT_LOW);
            GPIO_write(this->b1, GPIO_CFG_OUT_HIGH);
            GPIO_write(this->b2, GPIO_CFG_OUT_LOW);
            */
            break;
        case 1: // 0110
            GPIOPinWrite(AIN1_PORT, AIN1_BIT, 0);
            GPIOPinWrite(AIN2_PORT, AIN2_BIT, AIN2_BIT);
            GPIOPinWrite(BIN1_PORT, BIN1_BIT, BIN1_BIT);
            GPIOPinWrite(BIN2_PORT, BIN2_BIT, 0);
            /*
            GPIO_write(this->a1, GPIO_CFG_OUT_LOW);
            GPIO_write(this->a2, GPIO_CFG_OUT_HIGH);
            GPIO_write(this->b1, GPIO_CFG_OUT_HIGH);
            GPIO_write(this->b2, GPIO_CFG_OUT_LOW);
            */
            break;
        case 2: // 0101
            GPIOPinWrite(AIN1_PORT, AIN1_BIT, 0);
            GPIOPinWrite(AIN2_PORT, AIN2_BIT, AIN2_BIT);
            GPIOPinWrite(BIN1_PORT, BIN1_BIT, 0);
            GPIOPinWrite(BIN2_PORT, BIN2_BIT, BIN2_BIT);
            /*
            GPIO_write(this->a1, GPIO_CFG_OUT_LOW);
            GPIO_write(this->a2, GPIO_CFG_OUT_HIGH);
            GPIO_write(this->b1, GPIO_CFG_OUT_LOW);
            GPIO_write(this->b2, GPIO_CFG_OUT_HIGH);
            */
            break;
        case 3: // 1001
            GPIOPinWrite(AIN1_PORT, AIN1_BIT, AIN1_BIT);
            GPIOPinWrite(AIN2_PORT, AIN2_BIT, 0);
            GPIOPinWrite(BIN1_PORT, BIN1_BIT, 0);
            GPIOPinWrite(BIN2_PORT, BIN2_BIT, BIN2_BIT);
            /*
            GPIO_write(this->a1, GPIO_CFG_OUT_HIGH);
            GPIO_write(this->a2, GPIO_CFG_OUT_LOW);
            GPIO_write(this->b1, GPIO_CFG_OUT_LOW);
            GPIO_write(this->b2, GPIO_CFG_OUT_HIGH);
            */
            break;
        default: // Brake
            GPIOPinWrite(AIN1_PORT, AIN1_BIT, AIN1_BIT);
            GPIOPinWrite(AIN2_PORT, AIN2_BIT, AIN2_BIT);
            GPIOPinWrite(BIN1_PORT, BIN1_BIT, BIN1_BIT);
            GPIOPinWrite(BIN2_PORT, BIN2_BIT, BIN2_BIT);
            /*
            GPIO_write(this->a1, GPIO_CFG_OUT_HIGH);
            GPIO_write(this->a2, GPIO_CFG_OUT_HIGH);
            GPIO_write(this->b1, GPIO_CFG_OUT_HIGH);
            GPIO_write(this->b2, GPIO_CFG_OUT_HIGH);
            */
            break;
    }

    return;
}

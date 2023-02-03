/*
 * DRV8833.cc
 *
 *  Created on: Dec 18, 2022
 *      Author: nicho
 */

#include "DRV8833.h"
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>

#include <ti/drivers/GPIO.h>

DRV8833::DRV8833(
        int number_of_steps,
        unsigned int motor_pin_a1,
        unsigned int motor_pin_a2,
        unsigned int motor_pin_b1,
        unsigned int motor_pin_b2
) {
    this->number_of_steps = number_of_steps;
    this->motor_pin_a1 = motor_pin_a1;
    this->motor_pin_a2 = motor_pin_a2;
    this->motor_pin_b1 = motor_pin_b1;
    this->motor_pin_b2 = motor_pin_b2;
}

void DRV8833::set_speed(int rpm) {
    this->step_delay = 60 * 1000 * 1000 / this->number_of_steps / rpm;
}

void DRV8833::step(int steps) {
          int steps_left = 0;
          int direction = 0;

          if (steps > 0) {
              direction = 1;
              steps_left = steps;
          }
          if (steps < 0) {
              direction = 0;
              steps_left = -steps;
          }

          int step_number = 0;

          while (steps_left > 0) {
              if (direction > 0) {
                  step_number = (step_number + 1) % 4;
              } else {
                  step_number = (step_number + 3) % 4;
              }
              step_motor(step_number);
              steps_left--;
              usleep(this->step_delay*1000);
          }
}

void DRV8833::step_motor(int step) {
    switch(step) {
        case 0: // 1010
            GPIO_write(this->motor_pin_a1, GPIO_CFG_OUT_HIGH);
            GPIO_write(this->motor_pin_a2, GPIO_CFG_OUT_LOW);
            GPIO_write(this->motor_pin_b1, GPIO_CFG_OUT_HIGH);
            GPIO_write(this->motor_pin_b2, GPIO_CFG_OUT_LOW);
        break;
        case 1: // 0110
            GPIO_write(this->motor_pin_a1, GPIO_CFG_OUT_LOW);
            GPIO_write(this->motor_pin_a2, GPIO_CFG_OUT_HIGH);
            GPIO_write(this->motor_pin_b1, GPIO_CFG_OUT_HIGH);
            GPIO_write(this->motor_pin_b2, GPIO_CFG_OUT_LOW);
        break;
        case 2: // 0101
            GPIO_write(this->motor_pin_a1, GPIO_CFG_OUT_LOW);
            GPIO_write(this->motor_pin_a2, GPIO_CFG_OUT_HIGH);
            GPIO_write(this->motor_pin_b1, GPIO_CFG_OUT_LOW);
            GPIO_write(this->motor_pin_b2, GPIO_CFG_OUT_HIGH);
        break;
        case 3: // 1001
            GPIO_write(this->motor_pin_a1, GPIO_CFG_OUT_HIGH);
            GPIO_write(this->motor_pin_a2, GPIO_CFG_OUT_LOW);
            GPIO_write(this->motor_pin_b1, GPIO_CFG_OUT_LOW);
            GPIO_write(this->motor_pin_b2, GPIO_CFG_OUT_HIGH);
        break;
    }
}


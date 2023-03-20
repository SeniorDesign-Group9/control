/*
 *  control.cpp
 */

// Driver header files
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>
#include <ti/drivers/UART2.h>
#include <ti/drivers/Watchdog.h>

// Driver configuration
#include "ti_drivers_config.h"

// Standard libraries
#include <unistd.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <iostream>

// Project header files
#include "adc.hh"
#include "DRV8833.h"
#include "water.hh"

// mainThread
void *mainThread(void *arg0) {
    GPIO_init();

    // Water test
    WaterSolenoid::instance().waterSet(true);
    std::cout << "Water on" << std::endl;
    DRV8833* motor = new DRV8833(AIN1, AIN2, BIN1, BIN2, DRV_FAULT);

        while (1) {
            motor->stepSteps(600, 60);
            sleep(1);
            motor->stepSteps(-600, 60);
            sleep(1);
        }
    sleep(1);
    WaterSolenoid::instance().waterToggle();
    std::cout << "Water off" << std::endl;

    // ADC test (WIP)
    std::cout << AdcInternal::instance().get_n() << std::endl;
    AdcInternal::instance().set_n(5);
    std::cout << AdcInternal::instance().get_n() << std::endl;

    // C++ ver (debug)
    std::cout << "ver" << __cplusplus << std::endl;

    while(1) {
        sleep(1);
    }
}

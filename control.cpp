/*
 *  control.cpp
 */

#include <unistd.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <iostream>

// Driver header files
#include <ti/drivers/GPIO.h>
// #include <ti/drivers/I2C.h>
// #include <ti/drivers/PWM.h>
// #include <ti/drivers/SPI.h>
// #include <ti/drivers/UART2.h>
// #include <ti/drivers/Watchdog.h>

// Driver configuration
#include "ti_drivers_config.h"

// Project header files
#include "adc.hh"
#include "water.hh"

// mainThread
void *mainThread(void *arg0) {
    GPIO_init();

    //WaterSolenoid::instance().waterConfig(VALVE_EN);

    /*
    GPIO_setConfig(MOTOR_A1, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(MOTOR_A2, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(MOTOR_B1, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(MOTOR_B2, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(LED_GREEN, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(LED_RED, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    GPIO_write(LED_GREEN, GPIO_CFG_OUT_HIGH);
    GPIO_write(LED_RED, GPIO_CFG_OUT_HIGH);

    sleep(1);

    GPIO_write(LED_GREEN, GPIO_CFG_OUT_LOW);
    GPIO_write(LED_RED, GPIO_CFG_OUT_LOW);
    */

    std::cout << AdcInternal::instance().get_n() << std::endl;
    AdcInternal::instance().set_n(5);
    std::cout << AdcInternal::instance().get_n() << std::endl;

    std::cout << "ver" << __cplusplus << std::endl;

    while(1) {
        sleep(1);
    }
}

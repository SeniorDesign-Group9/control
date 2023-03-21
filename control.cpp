/*
 *  control.cpp
 */

// Driver header files
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>
#include <ti/drivers/UART2.h>
#include <ti/drivers/Watchdog.h>
#include <ti/devices/cc32xx/driverlib/gpio.h>


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
    DRV8833 motor(AIN1, AIN2, BIN1, BIN2, DRV_FAULT);

    printf("AIN1_PORT 0x%x\n", AIN1_PORT);
    printf("AIN2_PORT 0x%x\n", AIN2_PORT);
    printf("BIN1_PORT 0x%x\n", BIN1_PORT);
    printf("BIN2_PORT 0x%x\n", BIN1_PORT);

    GPIODirModeSet(AIN1_PORT, AIN1_BIT, GPIO_DIR_MODE_OUT);
    GPIODirModeSet(AIN2_PORT, AIN2_BIT, GPIO_DIR_MODE_OUT);
    GPIODirModeSet(BIN1_PORT, BIN1_BIT, GPIO_DIR_MODE_OUT);
    GPIODirModeSet(BIN2_PORT, BIN2_BIT, GPIO_DIR_MODE_OUT);

    while(1) {
        GPIOPinWrite(AIN1_PORT, AIN1_BIT, AIN1_BIT);
        GPIOPinWrite(AIN2_PORT, AIN2_BIT, AIN2_BIT);
        GPIOPinWrite(BIN1_PORT, BIN1_BIT, BIN1_BIT);
        GPIOPinWrite(BIN2_PORT, BIN2_BIT, BIN2_BIT);
        sleep(1);
        GPIOPinWrite(AIN1_PORT, AIN1_BIT, 0);
        GPIOPinWrite(AIN2_PORT, AIN2_BIT, 0);
        GPIOPinWrite(BIN1_PORT, BIN1_BIT, 0);
        GPIOPinWrite(BIN2_PORT, BIN2_BIT, 0);
        sleep(1);

        /*
        // step quarters
        printf("Step -2400\n");
        motor.stepSteps(-2400, 60);
        sleep(1);
        printf("Step -2400\n");
        motor.stepSteps(-2400, 60);
        sleep(1);
        printf("Step -2400\n");
        motor.stepSteps(-2400, 60);
        sleep(1);
        printf("Step -2400\n");
        motor.stepSteps(-2400, 60);
        sleep(1);
        //return
        printf("Step 9600\n");
        motor.stepSteps(9600, 240);
        sleep(1);
        */
    }
    sleep(1);


    // C++ ver (debug)
    std::cout << "ver" << __cplusplus << std::endl;

    while(1) {
        sleep(1);
    }
}

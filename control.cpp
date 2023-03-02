/*
control.cpp
 */

// Driver header files
#include <ti/display/Display.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/UART2.h>
#include <ti/drivers/Watchdog.h>
#include <ti/drivers/net/wifi/device.h>
#include <ti/drivers/net/wifi/simplelink.h>
#include <ti/drivers/net/wifi/wlan.h>

// Driver configuration
#include "ti_drivers_config.h"

// Standard libraries
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>

// Project header files
#include "adc.hh"
#include "wireless.hh"
#include "water.hh"

// mainThread
void *mainThread(void *arg0) {
    GPIO_init();
    SPI_init();

    GPIO_setConfig(9, GPIO_CFG_OUTPUT);
    GPIO_setConfig(11, GPIO_CFG_OUTPUT);
    GPIO_setConfig(10, GPIO_CFG_OUTPUT);

    GPIO_write(9, 0);   // Red
    GPIO_write(11, 0);  // Green
    GPIO_write(10, 0);  // Amber

    sleep(1);

    // Wireless test
    Wireless::instance().start();
    printf("NWP started\n");
    //Wireless::instance().stop();

    // C++ ver (debug)
    std::cout << "ver" << __cplusplus << std::endl;

    while(1) {
        sleep(1);
    }
}

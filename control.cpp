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
#include <unistd.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <iostream>

// Project header files
#include "adc.hh"
#include "wireless.hh"
#include "water.hh"

UART2_Handle uart;

// mainThread
void *mainThread(void *arg0) {
    GPIO_init();
    SPI_init();

    // Wireless test
    Wireless::instance().init();

    // C++ ver (debug)
    std::cout << "ver" << __cplusplus << std::endl;

    while(1) {
        sleep(1);
    }
}

// control.cpp

// Driver header files
#include <ti/drivers/GPIO.h>
#include <ti/drivers/net/wifi/device.h>
//#include <ti/drivers/I2C.h>
#include <ti/drivers/SPI.h>
//#include <ti/drivers/Watchdog.h>

// Driver configuration
#include "ti_drivers_config.h"

// Standard libraries
#include <cstdio>
#include <iostream>
#include <unistd.h>

// Project header files
#include "adc.hh"
#include "http.hh"
#include "water.hh"
#include "wireless.hh"


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


    // Wireless init
    if (Wireless::instance().start() < 0) {
        printf("NWP startup error\n");
        while(1) {}
    } else {
        printf("NWP started successfully\n");
    }

    /*
    // HTTP server init
    if (HttpServer::instance().start() < 0) {
        printf("HTTP server startup error\n");
        while(1) {}
    } else {
        printf("HTTP server started successfully\n");
    }
    */~



    // C++ ver (debug)
    std::cout << "ver" << __cplusplus << std::endl;

    while(1) {
        sleep(1);
    }
}

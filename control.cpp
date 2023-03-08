// control.cpp

// TI Driver header files
#include <ti/drivers/GPIO.h>
#include <ti/drivers/net/wifi/device.h>
#include <ti/drivers/I2C.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/Watchdog.h>

// TI Driver configuration
#include "ti_drivers_config.h"

// Standard libraries
#include <cstdio>
#include <iostream>
#include <pthread.h>
#include <unistd.h>

// Project header files
#include "adc.hh"
#include "water.hh"
#include "wireless.hh"

// mainThread
void *mainThread(void *arg0) {
    int i = 0;
    bool prov = false;
    _u32 statusWlan;
    _u8 pConfigOpt;
    _u16 pConfigLen;
    pConfigOpt = SL_DEVICE_EVENT_CLASS_WLAN;
    pConfigLen = sizeof(_u32);

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



    // C++ ver (debug)
    std::cout << "ver" << __cplusplus << std::endl;

    while(1) {
        sleep(1);

        // Get provisioning status every 1 second
        std::cout << "[" << ++i << "] Provisioning? ";

        sl_DeviceGet(SL_DEVICE_STATUS,&pConfigOpt,&pConfigLen,(_u8 *)(&statusWlan));
        prov = (SL_WLAN_EVENT_PROVISIONING_STATUS & statusWlan) ? true : false;

        std::cout << (prov ? "Running." : "Stopped.") << std::endl;
        // Will output 1 if provisioning, 0 if not provisioned.
    }
}

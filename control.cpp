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
#include <string>
#include <unistd.h>

// Project header files
#include "adc.hh"
#include "file.hh"
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

    // FS
    std::string fn = "test_file.txt";
    FileManager fm(fn);

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

        // FIXME debug: stop provisioning
        sl_WlanProvisioning(SL_WLAN_PROVISIONING_CMD_STOP,0xFF,0,NULL, 0x0);

        // Get provisioning status every 1 second
        std::cout << "[" << ++i << "] Provisioning? ";

        sl_DeviceGet(SL_DEVICE_STATUS,&pConfigOpt,&pConfigLen,(_u8 *)(&statusWlan));
        prov = (SL_WLAN_EVENT_PROVISIONING_STATUS & statusWlan) ? true : false;

        std::cout << (prov ? "Running." : "Stopped.") << std::endl;

        // If not provisioning, break from loop
        if (!prov) {
            break;
        }
    }

    // FS test
    std::cout << "Filename:    " << fm.getFileName() << std::endl;

    int32_t rval = 0;
    rval = fm.writeToFile("Hello, World!");

    std::cout << "writeToFile: " << rval << std::endl;

    return 0;
}

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
#include "water.hh"

// mainThread
void *mainThread(void *arg0) {
    // I2C structures
    I2C_Handle      i2c;
    I2C_Params      i2cParams;
    I2C_Transaction i2cTransaction;
    uint8_t         adc_address = 0x1F;
    uint8_t         data;


    I2C_init();
    GPIO_init();

    // Create I2C for usage
    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_400kHz;
    i2c = I2C_open(MyI2C1, &i2cParams);
    if (i2c == NULL) {
        std::cout << "Error initializing I2C" << std::endl;
        while (1);
    } else {
        std::cout << "I2C initialized" << std::endl;
    }

    /*






    // Setup I2C transaction to find slave
    i2cTransaction.writeBuf = &data;
    i2cTransaction.writeCount = 1;
    i2cTransaction.readBuf = &data;
    i2cTransaction.readCount = 0;
    data = 0;

    i2cTransaction.slaveAddress = 0x1F;
    if (I2C_transfer(i2c, &i2cTransaction)) {
        fprintf(stdout, "I2C device found at 0x%x\n", i2cTransaction.slaveAddress);
    } else {
        fprintf(stdout, "Error finding I2C device at 0x%x\n", i2cTransaction.slaveAddress);
    }

    */



    // ADC init
    AdcExternal::instance().init(i2c, adc_address);


    // Water test
    WaterSolenoid::instance().waterSet(true);
    std::cout << "Water on" << std::endl;
    sleep(1);
    WaterSolenoid::instance().waterToggle();
    std::cout << "Water off" << std::endl;

    // ADC test (WIP)
    //std::cout << AdcInternal::instance().get_n() << std::endl;
    //AdcInternal::instance().set_n(5);
    //std::cout << AdcInternal::instance().get_n() << std::endl;

    // C++ ver (debug)
    std::cout << "ver" << __cplusplus << std::endl;

    while(1) {
        sleep(1);
    }
}

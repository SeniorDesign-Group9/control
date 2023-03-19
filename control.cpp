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
    const uint16_t  ONE_VOLT = static_cast<uint16_t>(static_cast<float>(UINT16_MAX) / static_cast<float>(3.3));
    I2C_Handle      i2c;
    I2C_Params      i2cParams;
    I2C_Transaction i2cTransaction;
    uint8_t         adc_address = 0x18;     // 0x18 for eval board, 0x1F for prod board
    uint16_t        ch0_result = 0;
    uint16_t        ch1_result = 0;
    float           ch0_voltage = 0.0F;
    float           ch1_voltage = 0.0F;
    int             i = 0;

    I2C_init();
    GPIO_init();

    // Create I2C for usage
    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_400kHz;
    i2c = I2C_open(MyI2C1, &i2cParams);
    if (i2c == NULL) {
        printf("Error initializing I2C\n");
        while (1) {}
    } else {
        printf("I2C initialized\n");
    }

    // ADC init
    if (AdcExternal::instance().init(i2c, adc_address)) {
        printf("ADC initialized\n");
    } else {
        printf("Error initializing ADC\n");
    }

    // ADC test
    while(1) {
        ch0_result = AdcExternal::instance().getRawResult(AdcExternal::CH0);
        ch1_result = AdcExternal::instance().getRawResult(AdcExternal::CH1);
        ch0_voltage = static_cast<float>(ch0_result) / static_cast<float>(ONE_VOLT);
        ch1_voltage = static_cast<float>(ch1_result) / static_cast<float>(ONE_VOLT);

        printf("[%2d] CH0 raw result: %u\n", i, ch0_result);
        printf("     CH0 voltage:    %.3f V\n", ch0_voltage);
        printf("     CH1 raw result: %u\n", ch1_result);
        printf("     CH1 voltage:    %.3f V\n", ch1_voltage);

        ++i;
    }

    // C++ ver (debug)
    std::cout << "ver" << __cplusplus << std::endl;

    while(1) {
        sleep(1);
    }
}

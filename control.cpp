// control.cpp

// Driver header files
#include <ti/drivers/GPIO.h>
#include <ti/drivers/net/wifi/device.h>
#include <ti/drivers/I2C.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/UART2.h>
#include <ti/drivers/Watchdog.h>
#include <ti/devices/cc32xx/driverlib/gpio.h>

// Driver configuration
#include "ti_drivers_config.h"

// Standard libraries
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <unistd.h>

// Project header files
#include "adc.hh"
#include "charger.hh"
#include "wireless.hh"
#include "DRV8833.hh"
#include "water.hh"

// mainThread
void *mainThread(void *arg0) {
    const uint16_t  ONE_VOLT = static_cast<uint16_t>(static_cast<float>(UINT16_MAX) / static_cast<float>(3.3));
    I2C_Handle      i2c;
    I2C_Params      i2cParams;
    I2C_Transaction i2cTransaction;
    uint8_t         adc_address = 0x1F;     // 0x18 for eval board, 0x1F for prod board
    uint16_t        ch0_result = 0;
    uint16_t        ch1_result = 0;
    float           ch0_voltage = 0.0F;
    float           ch1_voltage = 0.0F;
    int             i = 0;
    uint8_t         data = 0;

    I2C_init();
    GPIO_init();
    SPI_init();
    DRV8833 motor(AIN1, AIN2, BIN1, BIN2, DRV_FAULT);
    
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

/*
    sleep(3);

    // Setup I2C transaction to find slave
    i2cTransaction.writeBuf = &data;
    i2cTransaction.writeCount = 1;
    i2cTransaction.readBuf = &data;
    i2cTransaction.readCount = 0;
    data = 0;

    i2cTransaction.slaveAddress = 0x1F;
    if (I2C_transferTimeout(i2c, &i2cTransaction, CLOCKS_PER_SEC)) {
        printf("I2C device found at 0x%2x\n", i2cTransaction.slaveAddress);
    } else {
        printf("Error finding I2C device at 0x%x\n", i2cTransaction.slaveAddress);
    }


*/

    // Wireless init
    if (Wireless::instance().start() < 0) {
        printf("NWP startup error\n");
    } else {
        printf("NWP started successfully\n");
    }

    /*
    // Charge Controller init
    if (Charger::instance().init(i2c, 0xD6)) {
        printf("Charger initialized\n");
    } else {
        printf("Error initializing charger\n");
    }

    if(Charger::instance().setRawRegisterValue(Charger::Registers::ChargeOption0, 0x0E, 0x02)) {
        printf("Set charge options successfully\n");
    } else {
        printf("Failed to set charge options\n");
    }

    if(Charger::instance().setRawRegisterValue(Charger::Registers::ChargeCurrent, 0x08, 0x00)) {
        printf("Max charge current set");
    } else {
        printf("Did not set max charge current");
    }
    */
    // ADC init
    if (AdcExternal::instance().init(i2c, adc_address)) {
        printf("ADC initialized\n");
    } else {
        printf("Error initializing ADC\n");
    }

    // ADC test
    while(1) {
        WaterSolenoid::instance().waterSet(true);
        GPIO_write(RED_LED, 1);

        ch0_result = AdcExternal::instance().getRawResult(AdcExternal::CH0);
        ch1_result = AdcExternal::instance().getRawResult(AdcExternal::CH1);
        ch0_voltage = static_cast<float>(ch0_result) / static_cast<float>(ONE_VOLT);
        ch1_voltage = static_cast<float>(ch1_result) / static_cast<float>(ONE_VOLT);

        WaterSolenoid::instance().waterSet(false);
        GPIO_write(RED_LED, 0);

        printf("[%3d]", i);
        printf(" NIR raw result: 0x%04x\n", ch0_result);
        printf("      NIR voltage:    %.3f V\n", ch0_voltage);
        printf("      VIS raw result: 0x%04x\n", ch1_result);
        printf("      VIS voltage:    %.3f V\n", ch1_voltage);

        ++i;

        motor.stepSteps(77, 60);

        sleep(2);
    }

    /*
    while(1) {
        // step quarters
        motor.stepSteps(600, 60);
        sleep(1);
        motor.stepSteps(-600, 60);
        sleep(1);
    }
    sleep(1);
     */

    // C++ ver (debug)
    std::cout << "ver" << __cplusplus << std::endl;

    while(1) {
        sleep(1);
    }
}

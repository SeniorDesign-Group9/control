// control.cpp

// TI Driver header files
#include <ti/drivers/GPIO.h>
#include <ti/drivers/net/wifi/device.h>
#include <ti/drivers/I2C.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/UART2.h>
#include <ti/drivers/Watchdog.h>
#include <ti/devices/cc32xx/driverlib/gpio.h>

// TI Driver configuration
#include "ti_drivers_config.h"

// Standard libraries
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <pthread.h>
#include <string>
#include <unistd.h>

// Driver configuration
#include "ti_drivers_config.h"

// Project header files
#include "adc.hh"
#include "charger.hh"
#include "DRV8833.hh"
#include "sensing.hh"
#include "water.hh"
#include "wireless.hh"

#define UART_BUFFER_SIZE    128
#define SLEEP_SPECTROSCOPY  60      // Sleep 60 sec
#define SLEEP_SOLENOID      60      // Sleep 60 sec

// Globals
I2C_Handle      i2c;
I2C_Params      i2cParams;
I2C_Transaction i2cTransaction;
UART2_Handle    uart;
UART2_Params    uartParams;

static float convert(uint16_t raw_result) {
    static const uint16_t OV = static_cast<uint16_t>(static_cast<float>(UINT16_MAX) / static_cast<float>(3.3));
    return static_cast<float>(raw_result) / static_cast<float>(OV);
}

static void print_spectra(void) {
    char buffer[UART_BUFFER_SIZE] = "";
    time_t time_reading(Sensing::instance().accessResult().time);

    // Print time captured
    std::strftime(buffer, UART_BUFFER_SIZE, "%Y-%m-%d %H:%M:%S", localtime(&time_reading));
    std::strcat(buffer, "\r\n");
    UART2_write(uart, buffer, UART_BUFFER_SIZE, nullptr);

    // Print header
    snprintf(buffer, UART_BUFFER_SIZE, "\"POS\",\"NIR\",\"VIS\"\r\n");
    UART2_write(uart, buffer, UART_BUFFER_SIZE, nullptr);

    // Print spectra
    for (int j = 0; j < POSITIONS; j++) {
        snprintf(buffer, UART_BUFFER_SIZE, "%03d,%.8f,%.8f\r\n", j, convert(Sensing::instance().accessResult().nir_results[j]), convert(Sensing::instance().accessResult().vis_results[j]));
        UART2_write(uart, buffer, UART_BUFFER_SIZE, nullptr);
    }
}


// mainThread
void *mainThread(void *arg0) {
    uint8_t         adc_address = 0x1F;     // 0x18 for eval board, 0x1F for prod board

    printf("\n--- Initialization begin ---\n");

    I2C_init();
    GPIO_init();
    SPI_init();
    DRV8833 motor(AIN1, AIN2, BIN1, BIN2, DRV_FAULT);
    
    // Program running
    GPIO_write(GREEN_LED, 1);
    GPIO_write(RED_LED, 1);

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

    // Create a UART where the default read and write mode is BLOCKING
    UART2_Params_init(&uartParams);
    uartParams.baudRate = 115200;
    uart = UART2_open(MyUART1, &uartParams);
    if (uart == NULL) {
        printf("Error initializing UART\n");
        while (1) {}
    } else {
        printf("UART initialized\n");
    }

    // Wireless init
    if (Wireless::instance().start() < 0) {
        printf("NWP startup error\n");
    } else {
        printf("NWP started successfully\n");
    }

    // FIXME
    // Wireless::instance().haltProvisioning();

    // ADC init
    if (AdcExternal::instance().init(i2c, adc_address)) {
        printf("ADC initialized\n");
    } else {
        printf("Error initializing ADC\n");
    }

    // Done initializing
    GPIO_write(RED_LED, 0);

    motor.stepMax(200);

    printf("---- Initialization end ----\n\n");

    while(1) {
        Sensing::instance().getResult(motor);

        print_spectra();

        /*
        printf("--------- Spectra ----------\n\n");
        printf("\"POS\",\"NIR\",\"VIS\"\n");
        for (int j = 0; j < POSITIONS; j++) {
            printf("%03d,", j);
            std::cout << convert(((Sensing::instance().queuePeek()).nir_results[j]));
            std::cout << ",";
            std::cout << convert(((Sensing::instance().queuePeek()).vis_results[j]));
            std::cout << "" << std::endl;
        }
        */

        motor.stepMax(200);         // 200 micrometers/sec = 60 RPM

        sleep(SLEEP_SPECTROSCOPY);
    }

    return 0;
}

// powerThread
void *powerThread(void *arg0) {
    // Wait until I2C is initialized
    while(i2c == NULL) {
        sleep(1);
    }

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

    while(1) {
        sleep(60);
    }

    return 0;
}

// waterThread
void *waterThread(void *arg0) {
    WaterSolenoid::instance().waterSet(false);

    while(1) {
        // FIXME: Sleep for 24 hours
        sleep(SLEEP_SOLENOID);

        // Water on for 10 seconds
        WaterSolenoid::instance().waterSet(true);
        sleep(10);
        WaterSolenoid::instance().waterSet(false);
    }

    return 0;
}

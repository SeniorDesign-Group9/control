//
// Created by nicho on 4/3/2023.
//

#ifndef CONTROL_I2C_H
#define CONTROL_I2C_H

#include <stdbool.h>
#include <stdint.h>
#include <ti/drivers/I2C.h>
class I2C {
public:
    // Init ADC
    bool init(I2C_Handle i2cHandle, uint8_t device_address);

    // Performs I2C transfer with txBuffer, rxBuffer, txCount, and rxCount
    // Returns true on transfer success, false on failure
    bool transfer();

    // I2C error handler
    void i2cErrorHandler(I2C_Transaction *transaction);

protected:
    I2C_Handle i2cHandle;
    I2C_Transaction i2cTransaction;
    uint8_t device_address;
    uint8_t txBuffer[3];
    uint8_t txCount;
    uint8_t rxBuffer[2];
    uint8_t rxCount;
};

#endif //CONTROL_I2C_H

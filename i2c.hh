//
// Created by nicho on 4/3/2023.
//

#ifndef I2C_H
#define I2C_H

#include "ti_drivers_config.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>


class I2C {
public:
    // Init I2C
    bool init(I2C_Handle i2cHandle, uint8_t device_address) {
        this->i2cHandle = i2cHandle;
        this->device_address = device_address;
        i2cTransaction.slaveAddress = this->device_address;

        txCount = 0;
        rxCount = 0;

        return true;
    }

    // Performs I2C transfer with txBuffer, rxBuffer, txCount, and rxCount
    // Returns true on transfer success, false on failure
    bool transfer() {
        bool ret;

        /* Before performing any i2c transfer, verify we called init() first*/
        if (i2cHandle == NULL) {
            return false;
        }

        i2cTransaction.writeBuf = txBuffer;
        i2cTransaction.writeCount = txCount;
        i2cTransaction.readBuf = rxBuffer;
        i2cTransaction.readCount = rxCount;

        ret = I2C_transfer(i2cHandle, &i2cTransaction);

        if (!ret) {
            i2cErrorHandler(&i2cTransaction);
        }

        return ret;
    }

    // I2C error handler
    void i2cErrorHandler(I2C_Transaction *transaction) {
        switch (transaction->status) {
            case I2C_STATUS_TIMEOUT:
                printf("%s: I2C transaction timed out!", __FILE__);
                break;
            case I2C_STATUS_CLOCK_TIMEOUT:
                printf("%s: I2C serial clock line timed out!", __FILE__);
                break;
            case I2C_STATUS_ADDR_NACK:
                printf("%s: I2C slave address 0x%x not acknowledged!", __FILE__, transaction->slaveAddress);
                break;
            case I2C_STATUS_DATA_NACK:
                printf("%s: I2C data byte not acknowledged!", __FILE__);
                break;
            case I2C_STATUS_ARB_LOST:
                printf("%s: I2C arbitration to another master!", __FILE__);
                break;
            case I2C_STATUS_INCOMPLETE:
                printf("%s: I2C transaction returned before completion!", __FILE__);
                break;
            case I2C_STATUS_BUS_BUSY:
                printf("%s: I2C bus is already in use!", __FILE__);
                break;
            case I2C_STATUS_CANCEL:
                printf("%s: I2C transaction cancelled!", __FILE__);
                break;
            case I2C_STATUS_INVALID_TRANS:
                printf("%s: I2C transaction invalid!", __FILE__);
                break;
            case I2C_STATUS_ERROR:
                printf("%s: I2C generic error!", __FILE__);
                break;
            default:
                printf("%s: I2C undefined error case!", __FILE__);
                break;
        }
    }

protected:
    I2C_Handle i2cHandle;
    I2C_Transaction i2cTransaction;
    uint8_t device_address;
    uint8_t txBuffer[3];
    uint8_t txCount;
    uint8_t rxBuffer[2];
    uint8_t rxCount;
};

#endif // I2C_H

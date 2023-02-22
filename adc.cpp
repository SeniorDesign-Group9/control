// adc.cpp

#include "adc.hh"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <ti/drivers/I2C.h>

// Singleton functions
// Empty constructor
AdcExternal::AdcExternal() {}

// Empty destructor
AdcExternal::~AdcExternal() {}

// Instance
AdcExternal& AdcExternal::instance() {
    static AdcExternal inst;
    return inst;
}

// Class functions
// Initializes ADC (I2C bus and parameters)
// Returns true on success, false on failure
bool AdcExternal::init(I2C_Handle i2cHandle, uint8_t i2cAddress) {
    this->i2cHandle = i2cHandle;
    this->i2cAddress = i2cAddress;
    i2cTransaction.slaveAddress = this->i2cAddress;

    txCount = 0;
    rxCount = 0;

    // Expecting 0b00000000
    if(getRawRegisterValue(OPMODE_I2CMODE_STATUS) == -1) {
        return false;
    } else {
        return true;
    }

    /*
    // TODO: Set ADC config
    // Set manual mode with AUTO sequenc
    setRawRegisterValue(OPMODE_SEL, 0b00000100);            // FIXME: High precision mode (111b)?

    // Select channels in AUTO_SEQ_CFG
    // Both channels enabled by default
    */

}

// Performs I2C transfer with txBuffer, rxBuffer, txCount, and rxCount
// Returns true on transfer success, false on failure
bool AdcExternal::transfer() {
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

// Sends uint8_t data to uint8_t reg
// Returns true on transfer success, false on failure
bool AdcExternal::setRawRegisterValue(Register reg, uint8_t data) {
    txBuffer[0] = SINGLE_WRITE;
    txBuffer[1] = (uint8_t)reg;
    txBuffer[2] = data;
    txCount = 3;
    rxCount = 0;

    return transfer();
}

// Get the raw value of the register reg
// Returns value on success, -1 on failure
int8_t AdcExternal::getRawRegisterValue(Register reg) {
    txBuffer[0] = SINGLE_READ;
    txBuffer[1] = reg;
    txCount = 2;
    rxCount = 1;

    // if transfer fails, return -1
    if (!transfer()) {
        return -1;
    } else {
        return rxBuffer[0];
    }
}

// Set bit in reg
// Returns true on transfer success, false on failure
bool AdcExternal::setRawRegisterBit(Register reg, uint8_t bits) {
    txBuffer[0] = SET_BIT;
    txBuffer[1] = (uint8_t)reg;
    txBuffer[2] = bits;
    txCount = 3;
    rxCount = 0;

    return transfer();
}

// Clear bit in reg
// Returns true on success, false on failure
bool AdcExternal::clearRawRegisterBit(Register reg, uint8_t bits) {
    txBuffer[0] = CLEAR_BIT;
    txBuffer[1] = (uint8_t)reg;
    txBuffer[2] = bits;
    txCount = 3;
    rxCount = 0;

    return transfer();
}


// Get value of OPMODE_I2CMODE_STATUS register
// Returns value on success, -1 on failure
int8_t AdcExternal::getOpmodeStatus() {
    return getRawRegisterValue(OPMODE_I2CMODE_STATUS);
}

// Get value of OPMODE_I2CMODE_STATUS register
// Returns value on success, -1 on failure
int16_t AdcExternal::getRawResult(Channel ch) {
    uint16_t result = 0;

    // Set bit SEQ_START in START_SEQUENCE
    setRawRegisterBit(START_SEQUENCE, 0b00000001);

    //getRawRegisterValue()
    // Need to keep SCL on?

    // Set bit SEQ_ABORT in ABORT_SEQUENCE
    setRawRegisterBit(START_SEQUENCE, 0b00000001);

    // FIXME: debug
    return 0;
}

// I2C error handler
void AdcExternal::i2cErrorHandler(I2C_Transaction *transaction) {
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

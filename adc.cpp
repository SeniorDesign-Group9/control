// adc.cpp

#include "adc.hh"
#include "ti_drivers_config.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>

#define SLEEP 50000             // Sleep 50 ms (50000 us)

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

    // Set ADC config

    // OFFSET_CAL: OFFSET calibration on powerup
    // Offset disabled by default

    // CHANNEL_INPUT_CFG reg
    // Two-Channel, Single-Ended enabled by default

    // OPMODE_SEL reg
    // Set high-precision mode with AUTO sequencing
    if (!setRawRegisterValue(OPMODE_SEL, 0b00000111)) {
        return false;
    }

    // I2C speed
    // Standard mode as determined by MCU

    // Select channels in AUTO_SEQ_CFG
    // Both channels enabled by default

    // Select low-power oscillator
    if (!setRawRegisterValue(OSC_SEL, 0b00000001)) {
        return false;
    }

    // Enable accumulators
    if (!setRawRegisterValue(ACC_EN, 0b00001111)) {
        return false;
    }

    // FIXME: set stop burst mode
    // N/a in high-precision mode

    // Expecting 0b00000000
    /*
    if (getRawRegisterValue(OPMODE_I2CMODE_STATUS) == -1) {
        return false;
    }
    */

    // Set GPIO callback for BUSY/RDY
    //GPIO_setCallback(ADC_BUSY, (GPIO_CallbackFxn)readyCallback);

    return true;
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
uint8_t AdcExternal::getRawRegisterValue(Register reg) {
    txBuffer[0] = SINGLE_READ;
    txBuffer[1] = reg;
    txCount = 2;
    rxCount = 1;

    // if transfer fails, return -1
    if (!transfer()) {
        //return -1;
        return 0;
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
uint16_t AdcExternal::getRawResult(Channel ch) {
    uint16_t result = 0;
    int8_t result_msb = 0;
    int8_t result_lsb = 0;
    Register reg_lsb = ACC_CH0_LSB;
    Register reg_msb = ACC_CH0_MSB;

    if (ch == CH1) {
        reg_lsb = ACC_CH1_LSB;
        reg_msb = ACC_CH1_MSB;
    }

    // Set bit SEQ_START in START_SEQUENCE
    if (!setRawRegisterBit(START_SEQUENCE, 0b00000001)) {
        //return -1;
        return 0;
    }

    // FIXME: determine time to wait
    // TODO: wait until BUSY/RDY pin goes low
    sleep(2);

    // Get value from accumulators
    result_msb = getRawRegisterValue(reg_msb);
    result_lsb = getRawRegisterValue(reg_lsb);

    // Error checking
    if (result_msb == -1 || result_lsb == -1) {
        //return -1;
        return 0;
    }

    // Put the 8-bit results into a 16-bit number
    result = ((uint8_t)result_msb << 8) & 0xFF00;
    result |= (uint8_t)result_lsb & 0x00FF;

    // Return result
    return result;
}

void * readyCallback(void) {
    printf("ADC ready pin callback\n");
    return nullptr;
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

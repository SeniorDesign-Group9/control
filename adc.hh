// adc.hh

#ifndef ADC_H
#define ADC_H

#include "i2c.hh"
#include <stdbool.h>
#include <stdint.h>
#include <ti/drivers/I2C.h>

//#define N_RDY_TMP 24
//#define N_ALERT_TMP 25
//#define SCL_TMP 10
//#define SDA_TMP 11

class AdcExternal : public I2C {
public:
    // Singleton functions
    // Get AdcExternal instance
    static AdcExternal &instance();

    // Disallow copying
    AdcExternal &operator=(const AdcExternal &) = delete;

    AdcExternal(const AdcExternal &) = delete;

    // Disallow moving
    AdcExternal &operator=(AdcExternal &&) = delete;

    AdcExternal(AdcExternal &&) = delete;


    // Class enums
    enum Channel {
        CH0 = 0x00,
        CH1 = 0x01
    };

    enum Register {
        WKEY = 0x17,
        DEVICE_RESET = 0x14,
        OFFSET_CAL = 0x15,
        OPMODE_SEL = 0x1C,
        OPMODE_I2CMODE_STATUS = 0x00,
        CHANNEL_INPUT_CFG = 0x24,
        AUTO_SEQ_CHEN = 0x20,
        START_SEQUENCE = 0x1E,
        ABORT_SEQUENCE = 0x1F,
        SEQUENCE_STATUS = 0x04,
        OSC_SEL = 0x18,
        nCLK_SEL = 0x19,
        DATA_BUFFER_OPMODE = 0x2C,
        DOUT_FORMAT_CFG = 0x28,
        DATA_BUFFER_STATUS = 0x01,
        ACC_EN = 0x30,
        ACC_CH0_LSB = 0x08,
        ACC_CH0_MSB = 0x09,
        ACC_CH1_LSB = 0x0A,
        ACC_CH1_MSB = 0x0B,
        ACCUMULATOR_STATUS = 0x02,
        ALERT_DWC_EN = 0x37,
        ALERT_CHEN = 0x34,
        DWC_HTH_CH0_MSB = 0x39,
        DWC_HTH_CH0_LSB = 0x38,
        DWC_LTH_CH0_MSB = 0x3B,
        DWC_LTH_CH0_LSB = 0x3A,
        DWC_HYS_CH0 = 0x40,
        DWC_HTH_CH1_MSB = 0x3D,
        DWC_HTH_CH1_LSB = 0x3C,
        DWC_LTH_CH1_MSB = 0x3F,
        DWC_LTH_CH1_LSB = 0x3E,
        DWC_HYS_CH1 = 0x41,
        PRE_ALT_MAX_EVENT_COUNT = 0x36,
        ALERT_TRIG_CHID = 0x03,
        ALERT_LOW_FLAGS = 0x0C,
        ALERT_HIGH_FLAGS = 0x0E
    };

    enum Opcode {
        SINGLE_READ = 0b00010000,
        SINGLE_WRITE = 0b00001000,
        SET_BIT = 0b00011000,
        CLEAR_BIT = 0b00100000,
        CONT_READ = 0b00110000,
        CONT_WRITE = 0b00101000
    };

    // Class functions
    bool init(I2C_Handle i2cHandle, uint8_t device_address);

    // Performs I2C transfer with txBuffer, rxBuffer, txCount, and rxCount
    // Returns true on transfer success, false on failure
    bool transfer();

    // Sends uint8_t data to uint8_t reg
    // Returns true on transfer success, false on failure
    bool setRawRegisterValue(uint8_t reg, uint8_t data);

    // Get the raw value of the register reg
    // Returns value on success, -1 on failure
    uint8_t getRawRegisterValue(uint8_t reg);

    // Set bit in reg
    // Returns true on transfer success, false on failure
    bool setRawRegisterBit(uint8_t reg, uint8_t bits);

    // Clear bit in reg
    // Returns true on success, false on failure
    bool clearRawRegisterBit(uint8_t reg, uint8_t bits);

    // I2C error handler
    void i2cErrorHandler(I2C_Transaction *transaction);

    int8_t getOpmodeStatus();

    uint16_t getRawResult(Channel ch);

private:
    // Singleton variables
    AdcExternal();

    ~AdcExternal();
};

void *readyCallback(void);

#endif // ADC_H

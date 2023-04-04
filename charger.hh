//
// Created by nicho on 4/3/2023.
//

#ifndef CONTROL_CHARGER_H
#define CONTROL_CHARGER_H

#include "i2c.hh"
#include <stdbool.h>
#include <stdint.h>
#include <ti/drivers/I2C.h>


class Charger : public I2C {
public:
    // Singleton functions
    // Get Charger instance
    static Charger &instance();

    // Disallow copying
    Charger &operator=(const Charger &) = delete;

    Charger(const Charger &) = delete;

    // Disallow moving
    Charger &operator=(Charger &&) = delete;

    Charger(Charger &&) = delete;

    // These are all the registers LSB
    // the MSB is one register up
    enum Registers {
        ChargeOption0 = 0x00,
        ChargeCurrent = 0x02,
        MaxChargeVoltage = 0x04,
        ChargeOption1 = 0x30,
        ChargeOption2 = 0x32,
        ChargeOption3 = 0x34,
        ProchotOption0 = 0x36,
        ProchotOption1 = 0x38,
        ADCOption = 0x3A,
        ChargerStatus = 0x20,
        ProchotStatus = 0x22,
        IIN_DPM = 0x24,
        ADCVBUS_PSYS = 0x26,
        ADCIBAT = 0x28,
        ADCIINCMPIN = 0x2A,
        ADCVSYSVBAT = 0x2C,
        InputVoltage = 0x0A,
        MinSystemVoltage = 0x0C
    };

    // Class functions
    bool init(I2C_Handle i2cHandle, uint8_t device_address);

    // Performs I2C transfer with txBuffer, rxBuffer, txCount, and rxCount
    // Returns true on transfer success, false on failure
    bool transfer();

    // Sends uint8_t data to uint8_t reg
    // Returns true on transfer success, false on failure
    bool setRawRegisterValue(uint8_t reg, uint8_t lsb, uint8_t msb);

    // Get the raw value of the register reg
    // Returns value on success, -1 on failure
    uint16_t getRawRegisterValue(uint8_t reg);

    // I2C error handler
    void i2cErrorHandler(I2C_Transaction *transaction);

private:
    // Singleton variables
    Charger();

    ~Charger();
};


#endif //CONTROL_CHARGER_H

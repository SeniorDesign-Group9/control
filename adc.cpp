// adc.cpp

#include "adc.hh"

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
void AdcExternal::init(I2C_Handle i2cHandle, uint8_t i2cAddress) {
    this->i2cHandle = i2cHandle;
    this->i2cAddress = i2cAddress;
    i2cTransaction.slaveAddress = this->i2cAddress;

    txCount = 0;
    rxCount = 0;

    /* if getConfiguration returns -1, then we failed to
    * communicate with the OPT3001 sensor. Return false
    * to indicate that initialization failed. */
    if(getConfiguration() == -1)
    {
       return (false);
    }
    else
    {
       return (true);
    }


void AdcExternal::destroy() {

}

/*!
 *  @brief  Puts 'data' into the register 'reg' on the OPT3001
 *
 *  @param[in]  reg     A register from the Register enum
 *  @param[in]  data    A 16-bit value to store in reg
 *
 *  @return @p true for a successful transfer; @p false
 *          for an error (for example, an I2C bus fault (NACK)).
 */
bool AdcExternal::setRawRegisterValue(uint8_t reg, uint16_t data);

/*!
 *  @brief  Calls an i2c transfer with txBuffer and rxBuffer
 *          and the txCount and rxCount variables
 *
 *  @return @p true for a successful transfer; @p false
 *          for an error (for example, an I2C bus fault (NACK)).
 */
bool AdcExternal::transfer();
int32_t AdcExternal::getResultRaw(unsigned int channel);

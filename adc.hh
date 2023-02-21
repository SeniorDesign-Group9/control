// adc.hh

#ifndef ADC_H
#define ADC_H

#include <stdint.h>
#include <stdbool.h>
#include <ti/drivers/I2C.h>

class AdcExternal {
    public:
        // Singleton functions
        // Get AdcExternal instance
        static AdcExternal& instance();

        // Disallow copying
        AdcExternal& operator = (const AdcExternal&) = delete;
        AdcExternal(const AdcExternal&) = delete;

        // Disallow moving
        AdcExternal& operator = (AdcExternal&&) = delete;
        AdcExternal(AdcExternal&&) = delete;

        // Class functions
        void init();
        void destroy();

        /*!
         *  @brief  Puts 'data' into the register 'reg' on the OPT3001
         *
         *  @param[in]  reg     A register from the Register enum
         *  @param[in]  data    A 16-bit value to store in reg
         *
         *  @return @p true for a successful transfer; @p false
         *          for an error (for example, an I2C bus fault (NACK)).
         */
        bool setRawRegisterValue(uint8_t reg, uint16_t data);

        /*!
         *  @brief  Calls an i2c transfer with txBuffer and rxBuffer
         *          and the txCount and rxCount variables
         *
         *  @return @p true for a successful transfer; @p false
         *          for an error (for example, an I2C bus fault (NACK)).
         */
        bool transfer();
        int32_t getResultRaw(unsigned int channel);


    
    private:
        // Singleton variables
        AdcExternal();
        ~AdcExternal();

        // Class variables
        I2C_Handle i2cHandle;
        I2C_Transaction i2cTransaction;
        uint8_t i2cAddress;
        uint8_t txBuffer[3];
        uint8_t txCount;
        uint8_t rxBuffer[2];
        uint8_t rxCount;

        enum Channel {
            CH0 = 0,
            CH1 = 1
        };
};

#endif // ADC_H

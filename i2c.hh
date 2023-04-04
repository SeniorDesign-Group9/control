//
// Created by nicho on 4/3/2023.
//

#ifndef CONTROL_I2C_H
#define CONTROL_I2C_H
typedef struct register {
   uint8_t reg
}Register;
class I2C {
public:
    bool init(I2C_Handle i2cHandle, uint8_t i2cAddress);
    bool transfer();
    bool setRawRegisterValue(Register reg, uint8_t data);
    bool
};
#endif //CONTROL_I2C_H

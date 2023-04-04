//
// Created by nicho on 4/3/2023.
//

#include "charger.hh"

Charger::Charger() {}

Charger::~Charger() {}

Charger& Charger::instance() {
    static Charger inst;
    return inst;
}

bool Charger::setRawRegisterValue(uint8_t reg, uint8_t lsb, uint8_t msb) {
    txBuffer[0] = reg;
    txBuffer[1] = lsb;
    txBuffer[2] = msb;

    txCount = 3;
    rxCount = 0;

    return transfer();
}

uint16_t Charger::getRawRegisterValue(uint8_t reg) {
    txBuffer[0] = reg;

    txCount = 1;
    rxCount = 2;

    if(!transfer()) {
        return 0;
    } else {
        return (rxBuffer[1] << 8) + rxBuffer[0];
    }
}

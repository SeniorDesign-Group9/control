// water.cpp

#include <unistd.h>
#include <ti/drivers/GPIO.h>
#include "ti_drivers_config.h"
#include "sensing.hh"

// Empty constructor
Sensing::Sensing() {}

// Empty destructor
Sensing::~Sensing() {}

// Instance
Sensing& Sensing::instance() {
    static Sensing inst;
    return inst;
}




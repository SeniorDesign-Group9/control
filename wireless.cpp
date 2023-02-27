// wireless.cpp

#include <ti/drivers/GPIO.h>
#include "ti_drivers_config.h"
#include "wireless.hh"

// Singleton functions
// Empty constructor
Wireless::Wireless() {}

// Empty destructor
Wireless::~Wireless() {}

// Instance
Wireless& Wireless::instance() {
    static Wireless inst;
    return inst;
}

// Class functions

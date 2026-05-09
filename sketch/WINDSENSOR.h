#ifndef WINDSENSOR_H
#define WINDSENSOR_H

#include <Arduino.h>
#include <Arduino_RouterBridge.h>
#include "CONFIG.h"

class WINDSENSOR {
public:
    WINDSENSOR();
    void init();
    void update();
    uint16_t get_awa(); // angle vent apparent [0; 359]
private:
    uint16_t awa; // angle vent apparent (degre)
    uint16_t ADC_awa_value; // valeur retournee par l'ADC (0-1023)
};
#endif
#ifndef COMPASS_H
#define COMPASS_H

#include <Arduino.h>
#include <Arduino_RouterBridge.h>
#include <Wire.h>
#include <math.h>
#include "CONFIG.h"

class COMPASS {
public:
    COMPASS();
    void init();
    void update();
    uint16_t get_heading() const; // renvoie l'angle fait avec le nord magnétique en degrés

private:
    uint16_t heading;
    int16_t Bx, By, Bz;

};

#endif
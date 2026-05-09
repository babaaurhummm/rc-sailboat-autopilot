#ifndef RUDDER_H
#define RUDDER_H

#include <Arduino.h>
#include <Arduino_RouterBridge.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "CONFIG.h"

class RUDDER {
public:
    RUDDER();
    void init();
    void update();
    void set_rudder_angle(int16_t angle);
    void set_rudder_angle_pwm(uint16_t pwm_set);
    int16_t get_rudder_angle();
private:
    int16_t rudder_angle;
    int16_t ADC_rudder_value;
    Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
};

#endif

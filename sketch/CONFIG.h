#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

constexpr uint16_t PIN_COM_RUDDER = 2;
constexpr uint16_t PIN_MODE_CONTROL = 3;

constexpr uint16_t pwm_mode_threshold = 1500;
constexpr uint16_t pwm_rudder_center = 1500;
constexpr uint16_t pwm_rudder_max = 2000;
constexpr uint16_t pwm_rudder_min = 1100;
constexpr uint16_t com_pulse_timeout_us = 3000;

constexpr uint16_t ADC_rudder_center = 534;
// ADC_rudder_max = 606
// ADC_rudder_min = 452
// ADC_rudder_range = ADC_rudder_max - ADC_rudder_min
constexpr uint16_t ADC_rudder_range = 154;
constexpr int16_t rudder_max_angle = 30;

constexpr uint16_t max_pulse_rudder = 345;
constexpr uint16_t min_pulse_rudder = 275;

constexpr uint16_t PIN_WINDSENSOR = A0;
constexpr uint16_t PIN_RUDDER_ADC = A1;
constexpr uint16_t RUDDER_CHANNEL = 0;

constexpr uint8_t HMC5883L_ADDR = 0x1E;

constexpr float Kp = 1;

#define FULL_UNMANNED_MODE 0   
// 1 = manual-only build, 0 = full autopilot
#define AWA_FOLLOW_MODE   0   
// 1 = AWA follow, 0 = heading follow

constexpr int16_t heading_sp = 0;
constexpr int16_t awa_sp = 0;
constexpr uint16_t loop_period_ms = 100;


#endif
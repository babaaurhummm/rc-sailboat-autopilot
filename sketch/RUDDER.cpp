#include "RUDDER.h"

RUDDER::RUDDER() {
  rudder_angle = 0;
  ADC_rudder_value = ADC_rudder_center;
}


void RUDDER::init() {
  Monitor.println("[RUDDER] Initializating Adafruit Servo Shield...");

  pwm.begin();
  pwm.setPWMFreq(50);
  set_rudder_angle_pwm(pwm_rudder_center);
  delay(10);
}

void RUDDER::update(){
  ADC_rudder_value = analogRead(PIN_RUDDER_ADC);
  const float k = 2.0f * rudder_max_angle / ADC_rudder_range;
  rudder_angle = static_cast<int16_t>(k * (ADC_rudder_value - ADC_rudder_center));
}

int16_t RUDDER::get_rudder_angle() const {
  return rudder_angle;
}

void RUDDER::set_rudder_angle(int16_t angle){

  uint16_t pulse = map(angle, -rudder_max_angle, rudder_max_angle, min_pulse_rudder, max_pulse_rudder);

  if (pulse < min_pulse_rudder) {
    pulse = min_pulse_rudder;
  }
  if (pulse > max_pulse_rudder) {
    pulse = max_pulse_rudder;
  }
  pwm.setPWM(RUDDER_CHANNEL, 0, pulse);
}

void RUDDER::set_rudder_angle_pwm(uint16_t pwm_set){
  uint16_t pulse = map(pwm_set, pwm_rudder_min, pwm_rudder_max, min_pulse_rudder, max_pulse_rudder);

  if (pulse < min_pulse_rudder) {
    pulse = min_pulse_rudder;
  }
  if (pulse > max_pulse_rudder) {
    pulse = max_pulse_rudder;
  }
  pwm.setPWM(RUDDER_CHANNEL, 0, pulse);
}
#include "COM.h"

COM::COM() {
  rudder_value = pwm_rudder_center;
  mode_control_value = pwm_mode_treshold;
}

void COM::init() {
  Monitor.println("[COM] Initializing Communication...");

  pinMode(PIN_COM_RUDDER, INPUT);
  pinMode(PIN_MODE_CONTROL, INPUT);
  delay(1000);
  Monitor.println("[COM] Waiting for MANNED mode...");

  while (true) {
    mode_control_value = pulseIn(PIN_MODE_CONTROL, HIGH, 25000);

    if (mode_control_value > pwm_mode_treshold) {
      Monitor.println("[COM] MANNED mode detected.");
      break;
    }
  }
}

void COM::update() {
  rudder_value = pulseIn(PIN_COM_RUDDER, HIGH, com_pulse_timeout_us);
  mode_control_value = pulseIn(PIN_MODE_CONTROL, HIGH, com_pulse_timeout_us);

  if (mode_control_value < pwm_mode_threshold) {
    unmanned_status = true;
  } else {
    unmanned_status = false;
  }
}

uint16_t COM::get_com_rudder() {
  return rudder_value;
}

bool COM::is_unmanned() {
  return unmanned_status;
}
#include "COM.h"

COM::COM() {
  rudder_value = pwm_rudder_center;
  mode_control_value = pwm_mode_threshold;
}

void COM::init() {
  Monitor.println("[COM] Initializing Communication...");

  pinMode(PIN_COM_RUDDER, INPUT);
  pinMode(PIN_MODE_CONTROL, INPUT);
  delay(1000);
  Monitor.println("[COM] Waiting for MANNED mode...");

  // Block here until the operator confirms the RC link with the mode switch
  // in MANNED. This is intentional: the boat must not move until the human
  // has acknowledged radio control on the dock.
  while (true) {
    mode_control_value = pulseIn(PIN_MODE_CONTROL, HIGH, 25000);

    if (mode_control_value > pwm_mode_threshold) {
      Monitor.println("[COM] MANNED mode detected.");
      break;
    }
  }
}

void COM::update() {
  const uint16_t new_rudder = pulseIn(PIN_COM_RUDDER, HIGH, com_pulse_timeout_us);
  const uint16_t new_mode   = pulseIn(PIN_MODE_CONTROL, HIGH, com_pulse_timeout_us);

  // pulseIn returns 0 on timeout; keep the last good values in that case.
  if (new_rudder != 0 && new_mode != 0) {
    rudder_value = new_rudder;
    mode_control_value = new_mode;
  }

  unmanned_status = (mode_control_value < pwm_mode_threshold);
}

uint16_t COM::get_com_rudder() const {
  return rudder_value;
}

bool COM::is_unmanned() const {
  return unmanned_status;
}
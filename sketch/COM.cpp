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

  // pulseIn returns 0 on timeout. Keep the last good values and count
  // consecutive misses so we fail safe instead of silently flipping into
  // autopilot just because a frame was dropped.
  if (new_rudder == 0 || new_mode == 0) {
    if (lost_frames < 255) lost_frames++;
  } else {
    rudder_value = new_rudder;
    mode_control_value = new_mode;
    lost_frames = 0;
  }

  link_lost = (lost_frames >= com_lost_frames_threshold);

  if (link_lost) {
    // Failsafe: drop out of autopilot, recenter rudder.
    unmanned_status = false;
    rudder_value = pwm_rudder_center;
  } else if (mode_control_value < pwm_mode_threshold) {
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

bool COM::is_link_lost() {
  return link_lost;
}
#include <Arduino.h>
#include <Arduino_RouterBridge.h>
#include "COM.h"
#include "COMPASS.h"
#include "RUDDER.h"
#include "WINDSENSOR.h"
#include "CONFIG.h"

COM com;
COMPASS compass;
RUDDER rudder;
WINDSENSOR windsensor;

#if FULL_UNMANNED_MODE

void setup() {
  com.init();
  rudder.init();
}

void loop() {
  com.update();
  rudder.set_rudder_angle_pwm(com.get_com_rudder());
  wait_until_next_loop();
}

#else

void wait_until_next_loop();
void update();
void save_data();
int rudder_angle_sp();

void setup() {
  Bridge.begin();
  Monitor.begin();
  delay(1000);
  com.init();
  compass.init();
  rudder.init();
  windsensor.init();
}

void loop() {

  update();
  save_data();

  if (com.is_unmanned())
  {
    #if AWA_FOLLOW_MODE
    rudder.set_rudder_angle(rudder_angle_sp(awa_sp));
    #else
    rudder.set_rudder_angle(rudder_angle_sp(heading_sp)) ;
    #endif
  }
  else
  {
    rudder.set_rudder_angle(com.get_com_rudder());
  }

  wait_until_next_loop();
}

void update() {
  com.update();
  compass.update();
  windsensor.update();
  rudder.update();
}

void save_data() {
  const uint16_t heading = compass.get_heading();
  const int16_t r_angle = rudder.get_rudder_angle();
  uint16_t awa = windsensor.get_awa();
  bool unmanned_status = com.is_unmanned();

  Bridge.notify("save_data", millis(), heading, r_angle, awa, unmanned_status);
}


void wait_until_next_loop() {
  static unsigned long last_loop_start = millis();
  const unsigned long now = millis();
  const unsigned long elapsed = now - last_loop_start;

  if (elapsed < loop_period_ms) {
    delay(loop_period_ms - elapsed);
  }

  last_loop_start = millis();
}

#if AWA_FOLLOW_MODE

int rudder_angle_sp(int awa_sp) {
  const uint16_t awa = windsensor.get_awa();
  const int16_t e = static_cast<int16_t>(awa_sp) - static_cast<int16_t>(awa);
  return Kp * e;

}

#else

int rudder_angle_sp(int heading_sp) {
  const int16_t e = static_cast<int16_t>(heading_sp) - static_cast<int16_t>(compass.get_heading());
  return Kp * e;
}

#endif




#endif
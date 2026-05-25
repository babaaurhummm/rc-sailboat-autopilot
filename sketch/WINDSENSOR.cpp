#include "WINDSENSOR.h"

WINDSENSOR::WINDSENSOR() {
    awa = 0;
}


void WINDSENSOR::init() {
    delay(5000);
    Monitor.println("[WINDSENSOR] Initializing WindSensor (10 sec max)");
    Monitor.println("[WINDSENSOR] --> Put the WindSensor in motion");

    unsigned long startTime = millis();

    while (millis() - startTime < 10000) { //20sec
        Monitor.println(analogRead(PIN_WINDSENSOR));
        delay(50); // lecture toutes les 50 ms
    }
    Monitor.println("[WINDSENSOR] Initialized");
}


void WINDSENSOR::update() {
    const uint16_t ADC_awa_value = analogRead(PIN_WINDSENSOR);

    uint32_t tmp = static_cast<uint32_t>(ADC_awa_value) * 360u + 511u;
    awa = static_cast<uint16_t>(tmp / 1023u);

    if (awa >= 360u) {
        awa = 359u;
    }
}


uint16_t WINDSENSOR::get_awa() const {
  return awa;
}
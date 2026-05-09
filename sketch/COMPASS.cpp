#include "COMPASS.h"

COMPASS::COMPASS() {
    heading = 0;
    Bx = 0;
    By = 0;
    Bz = 0;
}

void COMPASS::init() {
    Wire.begin();

    Monitor.println("[COMPASS] Initialisation...");

    // 1. CHECK ADRESSE I2C

    Wire.beginTransmission(HMC5883L_ADDR);
    byte error = Wire.endTransmission();

    if (error != 0) {
        Monitor.print("[COMPASS] ERREUR I2C - capteur non detecte a l'adresse 0x");
        Monitor.println(HMC5883L_ADDR, HEX);
        return;
    }

    Monitor.println("[COMPASS] Capteur detecte sur bus I2C.");

    // 2. TEST FONCTIONNEL (registre identification indirect)

    Wire.beginTransmission(HMC5883L_ADDR);
    Wire.write(0x00); // registre identifiant fabricant (A chez HMC)
    error = Wire.endTransmission();

    if (error != 0) {
        Monitor.println("[COMPASS] ERREUR communication registre.");
        return;
    }

    Monitor.println("[COMPASS] Communication registres OK.");

    // 3. CONFIGURATION MODE CONTINU

    Wire.beginTransmission(HMC5883L_ADDR);
    Wire.write(0x02);
    Wire.write(0x00);
    if (Wire.endTransmission() != 0) {
        Monitor.println("[COMPASS] ERREUR config mode.");
        return;
    }

    // 4. CONFIGURATION GAIN

    Wire.beginTransmission(HMC5883L_ADDR);
    Wire.write(0x01);
    Wire.write(0x20);
    if (Wire.endTransmission() != 0) {
        Monitor.println("[COMPASS] ERREUR config gain.");
        return;
    }

    Monitor.println("[COMPASS] OK - magnetometre operationnel.");
}

void COMPASS::update() {

    // Lecture des registres de données
    Wire.beginTransmission(HMC5883L_ADDR);
    Wire.write(0x03);
    Wire.endTransmission();

    Wire.requestFrom((uint8_t)HMC5883L_ADDR, (uint8_t)6);

    if (Wire.available() == 6) {
        Bx = (Wire.read() << 8) | Wire.read();
        Bz = (Wire.read() << 8) | Wire.read();
        By = (Wire.read() << 8) | Wire.read();

        float angle = atan2(By, Bx) * 180.0 / M_PI;

        if (angle < 0) {
            angle += 360.0;
        }

        heading = (uint16_t)angle;
    }
}

uint16_t COMPASS::get_heading() const {
    return heading;
}
#ifndef COM_H
#define COM_H

#include <Arduino.h>
#include <Arduino_RouterBridge.h>
#include "CONFIG.h"

class COM {
public:
    COM();
    void init();
    void update();
    uint16_t get_com_rudder();
    bool is_unmanned();
    bool is_link_lost();
private:
    uint16_t rudder_value;
    uint16_t mode_control_value;
    bool unmanned_status = false;
    uint8_t lost_frames = 0;
    bool link_lost = false;
};

#endif

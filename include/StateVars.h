#ifndef STATEVARS
#define STATEVARS
#include <Arduino.h>
using u8 = uint8_t;


struct StateVars{
    u8 clutch;
    float eg_rpm;
    float targ_rpm;
    float wl_rpm;
    float p = 0;
    float d = 0;
    float low_target = 0;
    float high_target = 0;
    float act_pos = 0;
};

#endif
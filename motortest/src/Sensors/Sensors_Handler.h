#pragma once

#include <Arduino.h>
#include <Wire.h>

typedef struct
{
    String AMB1, OBJ1, VOL1, CUR1, RPM1, AMB2, OBJ2, VOL2, CUR2, RPM2;

} SensorsHandler_Value;

class Sensors_Handler
{
    public:
        void begin();
        void getValue(SensorsHandler_Value& sensorsValue);        
};

extern Sensors_Handler sensorsHandler;
#ifndef METER_PARAMETER_STRUCT_H
#define METER_PARAMETER_STRUCT_H

#include "header/device/device.h"

struct METER_PARAMETER
{
    DEVICE* device_pointer;
    std::string internal_var_name;
    unsigned int metered_side_bus;
};


#endif // METER_PARAMETER_STRUCT_H

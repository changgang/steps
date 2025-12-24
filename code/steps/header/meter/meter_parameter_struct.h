#ifndef METER_PARAMETER_STRUCT_H
#define METER_PARAMETER_STRUCT_H

#include "header/device/device.h"
#include "header/basic/dc_device_id.h"

struct METER_PARAMETER
{
    DEVICE* device_pointer;
    std::string sub_device_name;
    unsigned int metered_side_bus;
    std::string internal_var_name;
    DC_DEVICE_ID sub_dc_did;
};


#endif // METER_PARAMETER_STRUCT_H

#ifndef METER_PARAMETER_STRUCT_H
#define METER_PARAMETER_STRUCT_H

#include "header/device/device.h"
#include "header/basic/dc_device_id.h"

struct METER_PARAMETER
{
    DEVICE* device_pointer;
    std::string internal_var_name;
    unsigned int metered_side_bus;
    DC_DEVICE_ID sub_dc_did;
    std::string hybrid_dc_device_name;
};


#endif // METER_PARAMETER_STRUCT_H

#include "header/model/load_shedding_model/load_shedding_model.h"
#include "header/basic/utility.h"
#include <cstdio>
LOAD_SHEDDING_MODEL::LOAD_SHEDDING_MODEL()
{
    set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR("LOAD");
}

LOAD_SHEDDING_MODEL::~LOAD_SHEDDING_MODEL()
{
    ;
}

LOAD* LOAD_SHEDDING_MODEL::get_load_pointer() const
{
    return (LOAD*) get_device_pointer();
}

void LOAD_SHEDDING_MODEL::set_subsystem_type(SUBSYSTEM_TYPE subtype)
{
    subsystem_type = subtype;
}

SUBSYSTEM_TYPE LOAD_SHEDDING_MODEL::get_subsystem_type() const
{
    return subsystem_type;
}


string LOAD_SHEDDING_MODEL::get_detailed_model_name() const
{
    string model_name = get_model_name();
    switch(get_subsystem_type())
    {
        case BUS_SUBSYSTEM_TYPE:
        {
            model_name += "BL";
            break;
        }
        case AREA_SUBSYSTEM_TYPE:
        {
            model_name += "AR";
            break;
        }
        case ZONE_SUBSYSTEM_TYPE:
        {
            model_name += "ZN";
            break;
        }
        case OWNER_SUBSYSTEM_TYPE:
        {
            model_name += "OW";
            break;
        }
        default:
        {
            model_name += "AL";
            break;
        }
    }
    return model_name;
}


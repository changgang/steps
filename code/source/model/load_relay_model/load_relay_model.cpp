#include "header/model/load_relay_model/load_relay_model.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <cstdio>

LOAD_RELAY_MODEL::LOAD_RELAY_MODEL()
{
    set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR("LOAD");
}

LOAD_RELAY_MODEL::~LOAD_RELAY_MODEL()
{
    ;
}

LOAD* LOAD_RELAY_MODEL::get_load_pointer() const
{
    return (LOAD*) get_device_pointer();
}

void LOAD_RELAY_MODEL::set_subsystem_type(SUBSYSTEM_TYPE subtype)
{
    subsystem_type = subtype;
}

SUBSYSTEM_TYPE LOAD_RELAY_MODEL::get_subsystem_type() const
{
    return subsystem_type;
}


string LOAD_RELAY_MODEL::get_detailed_model_name() const
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


double LOAD_RELAY_MODEL::get_bus_frequency_in_Hz() const
{
    LOAD* load = get_load_pointer();
    if(load!=NULL)
    {
        BUS* bus = load->get_bus_pointer();
        if(bus!=NULL)
            return bus->get_frequency_in_Hz();
        else
            return 0.0;
    }
    else
        return 0.0;
}

double LOAD_RELAY_MODEL::get_bus_base_frequency_in_Hz() const
{
    LOAD* load = get_load_pointer();
    if(load!=NULL)
    {
        BUS* bus = load->get_bus_pointer();
        if(bus!=NULL)
            return bus->get_base_frequency_in_Hz();
        else
            return 0.0;
    }
    else
        return 0.0;
}

double LOAD_RELAY_MODEL::get_bus_voltage_in_pu() const
{
    LOAD* load = get_load_pointer();
    if(load!=NULL)
    {
        BUS* bus = load->get_bus_pointer();
        if(bus!=NULL)
            return bus->get_voltage_in_pu();
        else
            return 0.0;
    }
    else
        return 0.0;
}

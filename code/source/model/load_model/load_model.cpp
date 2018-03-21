#include "header/model/load_model/load_model.h"
#include "header/power_system_database.h"
#include "header/basic/utility.h"
#include <cstdio>

LOAD_MODEL::LOAD_MODEL()
{
    set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR("LOAD");

    set_load_scale(1.0);
}

LOAD_MODEL::~LOAD_MODEL()
{
    ;
}

string LOAD_MODEL::get_model_type() const
{
    return "LOAD CHARACTERISTICS";
}

double LOAD_MODEL::get_bus_voltage_in_pu() const
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    LOAD* load = (LOAD*) get_device_pointer();
    size_t bus = load->get_load_bus();

    return psdb->get_bus_voltage_in_pu(bus);
}
double LOAD_MODEL::get_bus_frequency_deviation_in_pu() const
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    LOAD* load = (LOAD*) get_device_pointer();
    size_t bus = load->get_load_bus();

    return psdb->get_bus_frequency_deviation_in_pu(bus);
}

void LOAD_MODEL::set_load_scale(double scale)
{
    load_scale = scale;
}

double LOAD_MODEL::get_load_scale() const
{
    return load_scale;
}

void LOAD_MODEL::set_subsystem_type(SUBSYSTEM_TYPE subtype)
{
    subsystem_type = subtype;
}

SUBSYSTEM_TYPE LOAD_MODEL::get_subsystem_type() const
{
    return subsystem_type;
}

string LOAD_MODEL::get_detailed_model_name() const
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

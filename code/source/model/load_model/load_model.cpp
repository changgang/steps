#include "header/model/load_model/load_model.h"
#include "header/power_system_database.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <cstdio>

LOAD_MODEL::LOAD_MODEL()
{
    set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR("LOAD");
    busptr = NULL;
}

LOAD_MODEL::~LOAD_MODEL()
{
    ;
}

LOAD* LOAD_MODEL::get_load_pointer() const
{
    return (LOAD*) get_device_pointer();
}

void LOAD_MODEL::set_bus_pointer()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    LOAD* load = get_load_pointer();
    size_t bus = load->get_load_bus();

    busptr = psdb.get_bus(bus);
    if(busptr==NULL)
    {
        ostringstream osstream;
        osstream<<"Warning. No bus pointer is set for "<<get_model_name()<<" model of "<<load->get_device_name()<<"\n"
                <<"Check model data.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

BUS* LOAD_MODEL::get_bus_pointer() const
{
    return busptr;
}

string LOAD_MODEL::get_model_type() const
{
    return "LOAD CHARACTERISTICS";
}

double LOAD_MODEL::get_bus_voltage_in_pu()
{
    BUS* busptr =  get_bus_pointer();
    if(busptr!=NULL)
        return busptr->get_voltage_in_pu();
    else
    {
        set_bus_pointer();
        if(get_bus_pointer()!=NULL)
            return get_bus_voltage_in_pu();
        else
            return 0.0;
    }
}
double LOAD_MODEL::get_bus_frequency_deviation_in_pu()
{
    BUS* busptr =  get_bus_pointer();
    if(busptr!=NULL)
        return busptr->get_frequency_deviation_in_pu();
    else
    {
        set_bus_pointer();
        if(get_bus_pointer()!=NULL)
            return get_bus_frequency_deviation_in_pu();
        else
            return 0.0;
    }
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

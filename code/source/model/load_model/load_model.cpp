#include "header/model/load_model/load_model.h"
#include "header/power_system_database.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <cstdio>

LOAD_MODEL::LOAD_MODEL(STEPS& toolkit) : MODEL(toolkit)
{
    set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR("LOAD");
}

LOAD_MODEL::~LOAD_MODEL()
{
    ;
}

LOAD* LOAD_MODEL::get_load_pointer() const
{
    return (LOAD*) get_device_pointer();
}


void LOAD_MODEL::set_voltage_source_flag(bool flag)
{
    voltage_source_flag = flag;
}

bool LOAD_MODEL::get_voltage_source_flag() const
{
    return voltage_source_flag;
}

bool LOAD_MODEL::is_voltage_source() const
{
    return voltage_source_flag==true;
}

string LOAD_MODEL::get_model_type() const
{
    return "LOAD CHARACTERISTICS";
}


void LOAD_MODEL::synchronize_bus_voltage_and_frequency()
{
    BUS* bus = get_bus_pointer();
    voltage_pu = bus->get_positive_sequence_voltage_in_pu();
    complex_voltage_pu = bus->get_positive_sequence_complex_voltage_in_pu();
    frequency_deviation_pu = bus->get_frequency_deviation_in_pu();
}

double LOAD_MODEL::get_bus_positive_sequence_voltage_in_pu() const
{
    return voltage_pu;

    BUS* bus = get_bus_pointer();
    return bus->get_positive_sequence_voltage_in_pu();
}

double LOAD_MODEL::get_bus_positive_sequence_voltage_in_kV() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_positive_sequence_voltage_in_kV();
}

complex<double> LOAD_MODEL::get_bus_positive_sequence_complex_voltage_in_pu() const
{
    return complex_voltage_pu;

    BUS* bus = get_bus_pointer();
    return bus->get_positive_sequence_complex_voltage_in_pu();
}

double LOAD_MODEL::get_bus_frequency_deviation_in_pu() const
{
    return frequency_deviation_pu;

    BUS* bus = get_bus_pointer();
    return bus->get_frequency_deviation_in_pu();
}

double LOAD_MODEL::get_bus_base_frequency_in_Hz() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_base_frequency_in_Hz();
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

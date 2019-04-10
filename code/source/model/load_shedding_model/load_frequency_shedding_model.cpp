#include "header/model/load_shedding_model/load_frequency_shedding_model.h"
#include "header/device/load.h"
#include "header/basic/utility.h"
#include "header/power_system_database.h"

LOAD_FREQUENCY_SHEDDING_MODEL::LOAD_FREQUENCY_SHEDDING_MODEL() : LOAD_SHEDDING_MODEL()
{
    ;
}

LOAD_FREQUENCY_SHEDDING_MODEL::~LOAD_FREQUENCY_SHEDDING_MODEL()
{
    ;
}

string LOAD_FREQUENCY_SHEDDING_MODEL::get_model_type() const
{
    return "LOAD FREQUENCY SHEDDING";
}

double LOAD_FREQUENCY_SHEDDING_MODEL::get_bus_frequency_in_Hz() const
{
    LOAD* load = get_load_pointer();
    if(load==NULL)
        return 0.0;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    size_t bus = load->get_load_bus();

    return psdb.get_bus_frequency_in_Hz(bus);
}

double LOAD_FREQUENCY_SHEDDING_MODEL::get_bus_base_frequency_in_Hz() const
{
    LOAD* load = get_load_pointer();
    if(load==NULL)
        return 0.0;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    size_t bus = load->get_load_bus();

    return psdb.get_bus_base_frequency_in_Hz(bus);
}

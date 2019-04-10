#include "header/model/load_shedding_model/load_voltage_shedding_model.h"
#include "header/device/load.h"
#include "header/basic/utility.h"
#include "header/power_system_database.h"

LOAD_VOLTAGE_SHEDDING_MODEL::LOAD_VOLTAGE_SHEDDING_MODEL() : LOAD_SHEDDING_MODEL()
{
    ;
}

LOAD_VOLTAGE_SHEDDING_MODEL::~LOAD_VOLTAGE_SHEDDING_MODEL()
{
    ;
}

string LOAD_VOLTAGE_SHEDDING_MODEL::get_model_type() const
{
    return "LOAD VOLTAGE SHEDDING";
}

double LOAD_VOLTAGE_SHEDDING_MODEL::get_bus_voltage_in_pu() const
{
    LOAD* load = get_load_pointer();
    if(load==NULL)
        return 0.0;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    size_t bus = load->get_load_bus();

    return psdb.get_bus_voltage_in_pu(bus);
}

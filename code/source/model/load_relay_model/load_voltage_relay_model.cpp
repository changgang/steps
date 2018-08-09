#include "header/model/load_relay_model/load_voltage_relay_model.h"
#include "header/device/load.h"
#include "header/power_system_database.h"

LOAD_VOLTAGE_RELAY_MODEL::LOAD_VOLTAGE_RELAY_MODEL() : LOAD_RELAY_MODEL()
{
    ;
}

LOAD_VOLTAGE_RELAY_MODEL::~LOAD_VOLTAGE_RELAY_MODEL()
{
    ;
}

string LOAD_VOLTAGE_RELAY_MODEL::get_model_type() const
{
    return "LOAD VOLTAGE RELAY";
}

double LOAD_VOLTAGE_RELAY_MODEL::get_bus_voltage_in_pu() const
{
    LOAD* load = get_load_pointer();
    if(load==NULL)
        return 0.0;

    POWER_SYSTEM_DATABASE* psdb = load->get_power_system_database();
    if(psdb == NULL)
        return 0.0;

    size_t bus = load->get_load_bus();

    return psdb->get_bus_voltage_in_pu(bus);
}

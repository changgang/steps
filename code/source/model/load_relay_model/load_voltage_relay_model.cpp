#include "header/model/load_relay_model/load_voltage_relay_model.h"
#include "header/device/load.h"
#include "header/basic/utility.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"

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


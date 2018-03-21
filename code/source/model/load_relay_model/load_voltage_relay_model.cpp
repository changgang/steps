#include "header/model/load_relay_model/load_voltage_relay_model.h"
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

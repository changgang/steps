#include "header/model/load_relay_model/load_frequency_relay_model.h"
#include "header/device/load.h"
#include "header/basic/utility.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"

LOAD_FREQUENCY_RELAY_MODEL::LOAD_FREQUENCY_RELAY_MODEL(STEPS& toolkit) : LOAD_RELAY_MODEL(toolkit)
{
    ;
}

LOAD_FREQUENCY_RELAY_MODEL::~LOAD_FREQUENCY_RELAY_MODEL()
{
    ;
}

string LOAD_FREQUENCY_RELAY_MODEL::get_model_type() const
{
    return "LOAD FREQUENCY RELAY";
}

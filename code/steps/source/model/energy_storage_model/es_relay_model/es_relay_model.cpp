#include "header/model/energy_storage_model/es_relay_model/es_relay_model.h"

#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <cstdio>

ES_RELAY_MODEL::ES_RELAY_MODEL(STEPS& toolkit) : ES_MODEL(toolkit)
{
    common_constructor();
}

ES_RELAY_MODEL::~ES_RELAY_MODEL()
{
    ;
}

void ES_RELAY_MODEL::common_constructor()
{
    set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR(STEPS_ENERGY_STORAGE);
}

string ES_RELAY_MODEL::get_model_type() const
{
    return "ES RELAY";
}

double ES_RELAY_MODEL::get_bus_frequency_in_pu() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_frequency_in_pu();
}

#include "header/model/pvu_models/pv_relay_model/pv_relay_model.h"

#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <cstdio>

PV_RELAY_MODEL::PV_RELAY_MODEL(STEPS& toolkit) : PVU_MODEL(toolkit)
{
    common_constructor();
}

PV_RELAY_MODEL::~PV_RELAY_MODEL()
{
    ;
}

void PV_RELAY_MODEL::common_constructor()
{
}

string PV_RELAY_MODEL::get_model_type() const
{
    return "PV RELAY";
}

double PV_RELAY_MODEL::get_bus_frequency_in_pu() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_frequency_in_pu();
}

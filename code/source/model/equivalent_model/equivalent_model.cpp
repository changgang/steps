#include "header/model/equivalent_model/equivalent_model.h"
#include "header/power_system_database.h"
#include "header/basic/utility.h"
#include <cstdio>

EQUIVALENT_MODEL::EQUIVALENT_MODEL(STEPS& toolkit) : MODEL(toolkit)
{
    set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR("EQUIVALENT DEVICE");
}

EQUIVALENT_MODEL::~EQUIVALENT_MODEL()
{
    ;
}

EQUIVALENT_DEVICE* EQUIVALENT_MODEL::get_equivalent_device_pointer() const
{
    return (EQUIVALENT_DEVICE*) get_device_pointer();
}

string EQUIVALENT_MODEL::get_model_type() const
{
    return "EQUIVALENT MODEL";
}

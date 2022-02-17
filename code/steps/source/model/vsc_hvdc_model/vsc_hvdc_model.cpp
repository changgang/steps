#include "header/model/vsc_hvdc_model/vsc_hvdc_model.h"
#include "header/steps_namespace.h"
#include "header/basic/utility.h"
#include <cstdio>
#include <istream>
#include <iostream>

using namespace std;
class VSC_HVDC;

VSC_HVDC_MODEL::VSC_HVDC_MODEL(STEPS& toolkit) : MODEL(toolkit)
{
    set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR(STEPS_VSC_HVDC);
}


VSC_HVDC_MODEL::~VSC_HVDC_MODEL()
{
}

VSC_HVDC* VSC_HVDC_MODEL::get_vsc_hvdc_pointer() const
{
    return (VSC_HVDC*) get_device_pointer();
}

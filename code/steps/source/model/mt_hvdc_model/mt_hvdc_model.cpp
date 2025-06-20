#include "header/model/mt_hvdc_model/mt_hvdc_model.h"
#include "header/steps_namespace.h"
#include "header/basic/utility.h"
#include <cstdio>
#include <istream>
#include <iostream>

using namespace std;
class MT_HVDC;

MT_HVDC_MODEL::MT_HVDC_MODEL(STEPS& toolkit) : MODEL(toolkit)
{
    set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR(STEPS_HYBRID_DC);
}


MT_HVDC_MODEL::~MT_HVDC_MODEL()
{
}

MT_HVDC* MT_HVDC_MODEL::get_mt_hvdc_pointer() const
{
    return (MT_HVDC*) get_device_pointer();
}

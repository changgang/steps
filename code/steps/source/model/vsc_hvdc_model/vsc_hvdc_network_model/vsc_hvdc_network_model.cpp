#include "header/model/vsc_hvdc_model/vsc_hvdc_network_model/vsc_hvdc_network_model.h"

VSC_HVDC_NETWORK_MODEL::VSC_HVDC_NETWORK_MODEL(STEPS& toolkit):VSC_HVDC_MODEL(toolkit)
{
    ;
}

VSC_HVDC_NETWORK_MODEL::~VSC_HVDC_NETWORK_MODEL()
{
    ;
}

string VSC_HVDC_NETWORK_MODEL::get_model_type() const
{
    return "VSC LCC_HVDC2T NETWORK";
}

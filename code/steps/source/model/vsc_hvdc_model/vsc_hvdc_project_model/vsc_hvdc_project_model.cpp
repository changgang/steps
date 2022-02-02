#include "header/model/vsc_hvdc_model/vsc_hvdc_project_model/vsc_hvdc_project_model.h"

VSC_HVDC_PROJECT_MODEL::VSC_HVDC_PROJECT_MODEL(STEPS& toolkit):VSC_HVDC_MODEL(toolkit)
{
    ;
}

VSC_HVDC_PROJECT_MODEL::~VSC_HVDC_PROJECT_MODEL()
{
    ;
}

string VSC_HVDC_PROJECT_MODEL::get_model_type() const
{
    return "VSC HVDC PROJECT";
}

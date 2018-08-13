#include "header/model/pvu_models/pv_irradiance_model/pv_irradiance_model.h"

#include "header/basic/utility.h"
#include <fstream>

PV_IRRADIANCE_MODEL::PV_IRRADIANCE_MODEL()
{
}

PV_IRRADIANCE_MODEL::~PV_IRRADIANCE_MODEL()
{
    ;
}

string PV_IRRADIANCE_MODEL::get_model_type() const
{
    return "PV IRRADIANCE";
}

double PV_IRRADIANCE_MODEL::get_nominal_irradiance_in_Wpm2() const
{
    PV_UNIT* pv_unit = get_pv_unit_pointer();
    if(pv_unit==NULL)
        return 0.0;
    PV_IRRADIANCE_MODEL* pvirrd = pv_unit->get_pv_irradiance_model();
    if(pvirrd==NULL)
        return 0.0;

    return pvirrd->get_nominal_irradiance_in_Wpm2();
}

double PV_IRRADIANCE_MODEL::get_solar_irradiance_in_Wpm2()
{
    double irrd0 = get_nominal_irradiance_in_Wpm2();
    double irrd = get_solar_irradiance_in_pu();
    return irrd*irrd0;
}

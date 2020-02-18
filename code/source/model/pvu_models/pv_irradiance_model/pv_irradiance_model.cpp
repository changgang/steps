#include "header/model/pvu_models/pv_irradiance_model/pv_irradiance_model.h"

#include "header/basic/utility.h"
#include <fstream>

PV_IRRADIANCE_MODEL::PV_IRRADIANCE_MODEL(STEPS& toolkit) : PVU_MODEL(toolkit)
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
    if(pv_unit!=NULL)
    {
        PV_IRRADIANCE_MODEL* pvirrd = pv_unit->get_pv_irradiance_model();
        if(pvirrd!=NULL)
            return pvirrd->get_nominal_irradiance_in_Wpm2();
        else
            return 0.0;
    }
    else
        return 0.0;
}

double PV_IRRADIANCE_MODEL::get_solar_irradiance_in_Wpm2()
{
    double irrd0 = get_nominal_irradiance_in_Wpm2();
    double irrd = get_solar_irradiance_in_pu();
    return irrd*irrd0;
}

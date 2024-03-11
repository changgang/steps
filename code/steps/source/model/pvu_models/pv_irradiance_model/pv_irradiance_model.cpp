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

double PV_IRRADIANCE_MODEL::get_initial_irradiance_in_Wpm2() const
{
    PV_UNIT* pv_unit = get_pv_unit_pointer();
    PV_PANEL_MODEL* pv_panel = pv_unit->get_pv_panel_model();
    if(pv_panel!=NULL)
        return pv_panel->get_S0_in_Wpm2();
    else
        return 0.0;
}

double PV_IRRADIANCE_MODEL::get_solar_irradiance_in_Wpm2()
{
    double irrd0 = get_initial_irradiance_in_Wpm2();
    double irrd = get_solar_irradiance_in_pu();
    return irrd*irrd0;
}

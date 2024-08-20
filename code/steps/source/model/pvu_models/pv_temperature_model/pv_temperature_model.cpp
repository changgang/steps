#include "header/model/pvu_models/pv_temperature_model/pv_temperature_model.h"

#include "header/basic/utility.h"
#include <fstream>

PV_TEMPERATURE_MODEL::PV_TEMPERATURE_MODEL(STEPS& toolkit) : PVU_MODEL(toolkit)
{
}

PV_TEMPERATURE_MODEL::~PV_TEMPERATURE_MODEL()
{
    ;
}

string PV_TEMPERATURE_MODEL::get_model_type() const
{
    return "PV TEMPERATURE";
}

double PV_TEMPERATURE_MODEL::get_Tref_in_C_deg() const
{
    PV_UNIT* pv_unit = get_pv_unit_pointer();
    PV_PANEL_MODEL* pv_panel = pv_unit->get_pv_panel_model();
    if(pv_panel!=NULL)
    {
        return pv_panel->get_Tref_in_C_deg();
    }
    else
        return 25.0;
}

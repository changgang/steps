#include "header/model/pvu_models/pv_electrical_model/pv_electrical_model.h"
#include "header/basic/utility.h"
#include "header/device/pv_unit.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"

PV_ELECTRICAL_MODEL::PV_ELECTRICAL_MODEL(STEPS& toolkit) : PVU_MODEL(toolkit)
{
    unbypass_model();
}

PV_ELECTRICAL_MODEL::~PV_ELECTRICAL_MODEL()
{
    ;
}

void PV_ELECTRICAL_MODEL::unbypass_model()
{
    flag_model_bypassed = false;
}

void PV_ELECTRICAL_MODEL::bypass_model()
{
    flag_model_bypassed = true;
}

bool PV_ELECTRICAL_MODEL::is_model_bypassed() const
{
    return flag_model_bypassed;
}

string PV_ELECTRICAL_MODEL::get_model_type() const
{
    return "PV ELECTRICAL";
}

complex<double> PV_ELECTRICAL_MODEL::get_pv_unit_terminal_generation_in_MVA() const
{
    PV_UNIT* pv_unit = get_pv_unit_pointer();
    PV_CONVERTER_MODEL* model = pv_unit->get_pv_converter_model();
    if(model!=NULL)
    {
        if(not model->is_model_initialized())
            model->initialize();

        return model->get_terminal_complex_power_in_MVA();
    }
    else
        return 0.0;
}

complex<double> PV_ELECTRICAL_MODEL::get_pv_unit_terminal_generation_in_pu_based_on_mbase() const
{
    return get_pv_unit_terminal_generation_in_MVA()/get_mbase_in_MVA();
}

complex<double> PV_ELECTRICAL_MODEL::get_pv_unit_terminal_complex_current_in_pu() const
{
    PV_UNIT* pv_unit = get_pv_unit_pointer();
    PV_CONVERTER_MODEL* model = pv_unit->get_pv_converter_model();
    if(model!=NULL and model->is_model_initialized())
        return model->get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase();
    else
        return 0.0;
}

double PV_ELECTRICAL_MODEL::get_pv_unit_terminal_current_in_pu() const
{
	return steps_fast_complex_abs(get_pv_unit_terminal_complex_current_in_pu());
}

double PV_ELECTRICAL_MODEL::get_active_power_reference_in_pu_from_panel_model() const
{
    PV_UNIT* pv_unit = get_pv_unit_pointer();
    PV_PANEL_MODEL* panelmodel = pv_unit->get_pv_panel_model();
    if(panelmodel!=NULL)
    {
        if(not panelmodel->is_model_initialized())
            panelmodel->initialize();

        return panelmodel->get_Pref_in_pu_base_on_mbase();
    }
    else
        return 0.0;
}

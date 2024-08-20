#include "header/model/wtg_models/wt_electrical_model/wt_electrical_model.h"
#include "header/basic/utility.h"
#include "header/device/wt_generator.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"

WT_ELECTRICAL_MODEL::WT_ELECTRICAL_MODEL(STEPS& toolkit) : WTG_MODEL(toolkit)
{
    unbypass_model();
}

WT_ELECTRICAL_MODEL::~WT_ELECTRICAL_MODEL()
{
    ;
}

void WT_ELECTRICAL_MODEL::unbypass_model()
{
    flag_model_bypassed = false;
}

void WT_ELECTRICAL_MODEL::bypass_model()
{
    flag_model_bypassed = true;
}

bool WT_ELECTRICAL_MODEL::is_model_bypassed() const
{
    return flag_model_bypassed;
}

string WT_ELECTRICAL_MODEL::get_model_type() const
{
    return "WT ELECTRICAL";
}

complex<double> WT_ELECTRICAL_MODEL::get_wt_generator_terminal_generation_in_MVA() const
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    WT_GENERATOR_MODEL* genmodel = gen->get_wt_generator_model();
    if(genmodel!=NULL)
    {
        if(not genmodel->is_model_initialized())
            genmodel->initialize();

        return genmodel->get_terminal_complex_power_in_MVA();
    }
    else
        return 0.0;
}

complex<double> WT_ELECTRICAL_MODEL::get_wt_generator_terminal_generation_in_pu_based_on_mbase() const
{
    return get_wt_generator_terminal_generation_in_MVA()/get_mbase_in_MVA();
}

complex<double> WT_ELECTRICAL_MODEL::get_wt_generator_terminal_complex_current_in_pu() const
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    WT_GENERATOR_MODEL* model = gen->get_wt_generator_model();
    if(model!=NULL and model->is_model_initialized())
        return model->get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase();
    else
        return 0.0;
}

double WT_ELECTRICAL_MODEL::get_wt_generator_terminal_current_in_pu() const
{
    return steps_fast_complex_abs(get_wt_generator_terminal_complex_current_in_pu());
}

double WT_ELECTRICAL_MODEL::get_wt_generator_speed_in_pu() const
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    WT_TURBINE_MODEL* turbinemodel = gen->get_wt_turbine_model();
    if(turbinemodel != NULL and turbinemodel->is_model_initialized())
        return turbinemodel->get_turbine_speed_in_pu();
    else
        return 1.0;
}

double WT_ELECTRICAL_MODEL::get_wt_generator_speed_reference_in_pu() const
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    WT_PITCH_MODEL* pitch_model = gen->get_wt_pitch_model();
    if (pitch_model != NULL)
    {
        if (not pitch_model->is_model_initialized())
            pitch_model->initialize();
        if (pitch_model->get_hold_wtg_speed_flag() == true)
            return pitch_model->get_const_wtg_speed_reference_in_pu();
        else
            return pitch_model->get_wt_generator_reference_speed_in_pu();
    }
    else
    {
        WT_AERODYNAMIC_MODEL* aero_model = gen->get_wt_aerodynamic_model();
        if (aero_model != NULL)
        {
            if (not aero_model->is_model_initialized())
                aero_model->initialize();
            return aero_model->get_turbine_reference_speed_in_pu();
        }
        else
            return 0.0;
    }
}



#include "header/model/wtg_models/wt_turbine_model/wt_turbine_model.h"

#include "header/basic/utility.h"
#include <cstdio>

WT_TURBINE_MODEL::WT_TURBINE_MODEL(STEPS& toolkit) : WTG_MODEL(toolkit)
{
    common_constructor();
}

WT_TURBINE_MODEL::~WT_TURBINE_MODEL()
{
    ;
}

void WT_TURBINE_MODEL::common_constructor()
{
    set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR("WT GENERATOR");

    set_damping_in_pu(0.0);
}

string WT_TURBINE_MODEL::get_model_type() const
{
    return "WT TURBINE";
}

void WT_TURBINE_MODEL::set_damping_in_pu(double D)
{
    damping = D;
}

double WT_TURBINE_MODEL::get_damping_in_pu() const
{
    return damping;
}

double WT_TURBINE_MODEL::get_wt_generator_active_power_generation_in_MW() const
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    WT_GENERATOR_MODEL* genmodel = gen->get_wt_generator_model();
    if(genmodel != NULL)
    {
        if(not genmodel->is_model_initialized())
            genmodel->initialize();

        return genmodel->get_terminal_active_power_in_MW();
    }
    else
        return 0.0;
}

double WT_TURBINE_MODEL::get_initial_wind_turbine_speed_in_pu_from_wt_areodynamic_model() const
{
    WT_GENERATOR* generator = get_wt_generator_pointer();
    WT_AERODYNAMIC_MODEL* aero_model = generator->get_wt_aerodynamic_model();
    if(aero_model!=NULL)
    {
        if(not aero_model->is_model_initialized())
            aero_model->initialize();

        return aero_model->get_initial_turbine_speed_in_pu();
    }
    else
        return 0.0;
}

double WT_TURBINE_MODEL::get_mechanical_power_in_pu_from_wt_aerodynamic_model() const
{
    WT_GENERATOR* generator = get_wt_generator_pointer();
    WT_AERODYNAMIC_MODEL* aero_model = generator->get_wt_aerodynamic_model();
    if(aero_model!=NULL)
    {
        if(not aero_model->is_model_initialized())
            aero_model->initialize();

        double one_over_mbase = get_one_over_mbase_in_one_over_MVA();
        return aero_model->get_turbine_mechanical_power_in_MW()*one_over_mbase;
    }
    else
        return 0.0;
}

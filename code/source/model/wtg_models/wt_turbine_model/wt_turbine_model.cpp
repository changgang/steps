#include "header/model/wtg_models/wt_turbine_model/wt_turbine_model.h"
#include "header/device/generator.h"

#include "header/basic/utility.h"
#include <cstdio>

WT_TURBINE_MODEL::WT_TURBINE_MODEL()
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
    return "WIND TURBINE";
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
    if(gen == NULL)
        return 0.0;

    WT_GENERATOR_MODEL* genmodel = gen->get_wt_generator_model();
    if(genmodel == NULL)
        return 0.0;

    if(not genmodel->is_model_initialized())
        genmodel->initialize();

    return genmodel->get_terminal_active_power_in_MW();
}

double WT_TURBINE_MODEL::get_initial_wind_turbine_speed_in_pu_from_wt_areodynamic_model() const
{
    WT_GENERATOR* generator = get_wt_generator_pointer();
    if(generator==NULL)
        return 0.0;
    WT_AERODYNAMIC_MODEL* aero_model = generator->get_wt_aerodynamic_model();
    if(aero_model==NULL)
        return 0.0;

    if(not aero_model->is_model_initialized())
        aero_model->initialize();

    double w = aero_model->get_turbine_reference_speed_in_rad_per_s();
    double wn = aero_model->get_nominal_turbine_speed_in_rad_per_s();

    return w/wn;
}

double WT_TURBINE_MODEL::get_mechanical_power_in_pu_from_wt_aerodynamic_model() const
{
    WT_GENERATOR* generator = get_wt_generator_pointer();
    if(generator==NULL)
        return 0.0;
    WT_AERODYNAMIC_MODEL* aero_model = generator->get_wt_aerodynamic_model();
    if(aero_model==NULL)
        return 0.0;

    if(not aero_model->is_model_initialized())
        aero_model->initialize();

    double mbase = get_mbase_in_MVA();
    return aero_model->get_turbine_mechanical_power_in_MW()/mbase;
}

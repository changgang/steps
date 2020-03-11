#include "header/model/wtg_models/wt_turbine_model/wt_turbine_model.h"

#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <cstdio>

WT_PITCH_MODEL::WT_PITCH_MODEL(STEPS& toolkit) : WTG_MODEL(toolkit)
{
    common_constructor();
}

WT_PITCH_MODEL::~WT_PITCH_MODEL()
{
    ;
}

void WT_PITCH_MODEL::common_constructor()
{
    set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR("WT GENERATOR");
    set_frequency_upper_deadband_in_pu(1.0);
    set_frequency_lower_deadband_in_pu(1.0);
    set_hold_wtg_speed_flag(false);
    set_const_wtg_speed_reference_in_pu(0.0);
}

string WT_PITCH_MODEL::get_model_type() const
{
    return "WT PITCH";
}


double WT_PITCH_MODEL::get_wt_generator_speed_in_pu() const
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    WT_TURBINE_MODEL* turbine = gen->get_wt_turbine_model();
    if(turbine!=NULL and turbine->is_model_initialized())
        return turbine->get_turbine_speed_in_pu();
    else
        return 0.0;
}

double WT_PITCH_MODEL::get_wt_generator_reference_speed_in_pu() const
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    WT_AERODYNAMIC_MODEL* aerd = gen->get_wt_aerodynamic_model();
    if (aerd != NULL and aerd->is_model_initialized())
        return aerd->get_turbine_reference_speed_in_pu();
    else
        return 0.0;
}

double WT_PITCH_MODEL::get_bus_frequency_in_pu() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_frequency_in_pu();
}

double WT_PITCH_MODEL::get_bus_frequency_deviation_in_pu() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_frequency_deviation_in_pu();
}

double WT_PITCH_MODEL::get_initial_pitch_angle_in_deg_from_wt_aerodynamic_model() const
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    WT_AERODYNAMIC_MODEL* aerd = gen->get_wt_aerodynamic_model();
    if(aerd!=NULL)
    {
        if(not aerd->is_model_initialized())
            aerd->initialize();
        return aerd->get_initial_pitch_angle_in_deg();
    }
    else
        return 0.0;
}

void WT_PITCH_MODEL::set_frequency_upper_deadband_in_pu(double freq)
{
    frequency_deadband_upper_in_pu = freq;
}

void WT_PITCH_MODEL::set_frequency_lower_deadband_in_pu(double freq)
{
    frequency_deadband_lower_in_pu = freq;
}

double WT_PITCH_MODEL::get_frequency_upper_deadband_in_pu() const
{
    return frequency_deadband_upper_in_pu;
}

double WT_PITCH_MODEL::get_frequency_lower_deadband_in_pu() const
{
    return frequency_deadband_lower_in_pu;
}

void WT_PITCH_MODEL::set_hold_wtg_speed_flag(bool flag)
{
    hold_wtg_speed_flag = flag;
}

void WT_PITCH_MODEL::set_const_wtg_speed_reference_in_pu(double w)
{
    const_wtg_speed_reference = w;
}

bool WT_PITCH_MODEL::get_hold_wtg_speed_flag() const
{
    return hold_wtg_speed_flag;
}

double WT_PITCH_MODEL::get_const_wtg_speed_reference_in_pu() const
{
    return const_wtg_speed_reference;
}

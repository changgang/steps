#include "header/model/wtg_models/wt_turbine_model/wt_turbine_model.h"
#include "header/device/generator.h"

#include "header/basic/utility.h"
#include <cstdio>

WT_PITCH_MODEL::WT_PITCH_MODEL()
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
    set_speed_reference_in_pu(1.0);
    set_frequency_reference_in_pu(1.0);
    set_power_reference_in_pu(1.0);
}

string WT_PITCH_MODEL::get_model_type() const
{
    return "WT PITCH";
}


double WT_PITCH_MODEL::get_wt_generator_speed_in_pu() const
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    if(gen==NULL)
        return 0.0;

    WT_TURBINE_MODEL* turbine = gen->get_wt_turbine_model();
    if(turbine!=NULL and turbine->is_model_initialized())
        return turbine->get_wind_turbine_generator_speed_in_pu();
    else
        return 0.0;
}

double WT_PITCH_MODEL::get_bus_frequency_in_pu() const
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    if(gen==NULL)
        return 1.0;

    POWER_SYSTEM_DATABASE* psdb = gen->get_power_system_database();
    if(psdb==NULL)
        return 1.0;

    size_t bus = gen->get_source_bus();
    return psdb->get_bus_frequency_in_pu(bus);
}

double WT_PITCH_MODEL::get_wt_active_power_command_in_pu() const
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    if(gen==NULL)
        return 0.0;

    WT_ELECTRICAL_MODEL* electrical = gen->get_wt_electrical_model();
    if(electrical!=NULL and electrical->is_model_initialized())
        return electrical->get_active_current_command_in_pu_based_on_mbase();
    else
        return 0.0;
}


void WT_PITCH_MODEL::set_speed_reference_in_pu(double speed)
{
    speed_reference_in_pu = speed;
}

void WT_PITCH_MODEL::set_frequency_reference_in_pu(double freq)
{
    frequency_reference_in_pu = freq;
}

void WT_PITCH_MODEL::set_power_reference_in_pu(double power)
{
    power_reference_in_pu = power;
}

double WT_PITCH_MODEL::get_speed_reference_in_pu() const
{
    return speed_reference_in_pu;
}

double WT_PITCH_MODEL::get_frequency_reference_in_pu() const
{
    return frequency_reference_in_pu;
}

double WT_PITCH_MODEL::get_power_reference_in_pu() const
{
    return power_reference_in_pu;
}

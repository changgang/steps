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

    set_initial_wind_speed_in_mps(13.0);
    set_initial_wind_speed_in_pu(1.0);
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
    WT_GENERATOR* gen = (WT_GENERATOR*) get_device_pointer();
    if(gen == NULL)
        return 0.0;

    WT_GENERATOR_MODEL* genmodel = gen->get_wt_generator_model();
    if(genmodel == NULL)
        return 0.0;

    if(not genmodel->is_model_initialized())
        genmodel->initialize();

    return genmodel->get_terminal_active_power_in_MW();
}

double WT_TURBINE_MODEL::get_pitch_angle_in_deg() const
{
    WT_GENERATOR* gen = (WT_GENERATOR*) get_device_pointer();
    if(gen == NULL)
        return 0.0;

    WT_PITCH_MODEL* pitchmodel = gen->get_wt_pitch_model();
    if(pitchmodel != NULL and pitchmodel->is_model_initialized())
        return pitchmodel->get_pitch_angle_in_deg();
    else
        return get_initial_pitch_angle_in_deg();
}

double WT_TURBINE_MODEL::get_wind_speed_in_mps() const
{
    WT_GENERATOR* gen = (WT_GENERATOR*) get_device_pointer();
    if(gen == NULL)
        return 0.0;

    WIND_SPEED_MODEL* windmodel = gen->get_wind_speed_model();
    if(windmodel != NULL and windmodel->is_model_initialized())
        return windmodel->get_wind_speed_in_mps();
    else
        return get_initial_wind_speed_in_mps();
}

double WT_TURBINE_MODEL::get_wind_speed_in_pu() const
{
    WT_GENERATOR* gen = (WT_GENERATOR*) get_device_pointer();
    if(gen == NULL)
        return 0.0;

    WIND_SPEED_MODEL* windmodel = gen->get_wind_speed_model();
    if(windmodel != NULL and windmodel->is_model_initialized())
        return windmodel->get_wind_speed_in_pu();
    else
        return get_initial_wind_speed_in_pu();
}

void WT_TURBINE_MODEL::set_mechanical_power_reference_in_pu_based_on_mbase(double P)
{
    mechanical_power_reference_in_pu = P;
}

double WT_TURBINE_MODEL::get_mechanical_power_reference_in_pu_based_on_mbase() const
{
    return mechanical_power_reference_in_pu;
}

void WT_TURBINE_MODEL::set_initial_pitch_angle_in_deg(double pitch)
{
    initial_pitch_angle_in_deg = pitch;
}

void WT_TURBINE_MODEL::set_initial_wind_speed_in_mps(double vwind)
{
    initial_wind_speed_in_mps = vwind;
}

void WT_TURBINE_MODEL::set_initial_wind_speed_in_pu(double vwind)
{
    initial_wind_speed_in_pu = vwind;
}

void WT_TURBINE_MODEL::set_initial_wind_turbine_speed_in_pu(double speed)
{
    initial_wind_turbine_speed_in_pu = speed;
}

double WT_TURBINE_MODEL::get_initial_pitch_angle_in_deg() const
{
    return initial_pitch_angle_in_deg;
}

double WT_TURBINE_MODEL::get_initial_wind_speed_in_mps() const
{
    return initial_wind_speed_in_mps;
}

double WT_TURBINE_MODEL::get_initial_wind_speed_in_pu() const
{
    return initial_wind_speed_in_pu;
}

double WT_TURBINE_MODEL::get_initial_wind_turbine_speed_in_pu() const
{
    return initial_wind_turbine_speed_in_pu;
}

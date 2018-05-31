#include "header/model/wtg_models/wt_aerodynamic_model/wt_aerodynamic_model.h"
#include "header/basic/utility.h"
#include <iostream>

using namespace std;

WT_AERODYNAMIC_MODEL::WT_AERODYNAMIC_MODEL()
{
    set_number_of_pole_pairs(2);
    set_generator_to_turbine_gear_ratio(100.0);
    set_gear_efficiency(1.0);
    set_nominal_wind_speed_in_mps(13.0);
    set_nominal_air_density_in_kgpm3(1.22);

    //set_air_density_in_kgpm3(get_nominal_air_density_in_kgpm3());
    //set_initial_pitch_angle_in_deg(0.0);
    //set_initial_wind_speed_in_mps(get_nominal_wind_speed_in_mps());
    //set_initial_turbine_speed_in_rad_per_s(get_nominal_turbine_speed_in_rad_per_s());
}

WT_AERODYNAMIC_MODEL::WT_AERODYNAMIC_MODEL(const WT_AERODYNAMIC_MODEL& model)
{
    copy_from_const_model(model);
}

WT_AERODYNAMIC_MODEL::~WT_AERODYNAMIC_MODEL()
{

}

WT_AERODYNAMIC_MODEL& WT_AERODYNAMIC_MODEL::operator=(const WT_AERODYNAMIC_MODEL& model)
{
    if(this==(&model))
        return *this;

    copy_from_const_model(model);
    return *this;
}

void WT_AERODYNAMIC_MODEL::copy_from_const_model(const WT_AERODYNAMIC_MODEL& model)
{
    set_number_of_pole_pairs(model.get_number_of_pole_pairs());
    set_generator_to_turbine_gear_ratio(model.get_generator_to_turbine_gear_ratio());
    set_gear_efficiency(model.get_gear_efficiency());
    set_turbine_blade_radius_in_m(model.get_turbine_blade_radius_in_m());
    set_nominal_wind_speed_in_mps(model.get_nominal_wind_speed_in_mps());
    set_nominal_air_density_in_kgpm3(model.get_nominal_air_density_in_kgpm3());

    set_air_density_in_kgpm3(model.get_air_density_in_kgpm3());
    set_overspeed_mode_flag(model.get_overspeed_mode_flag());
}

string WT_AERODYNAMIC_MODEL::get_model_type() const
{
    return "WT AERODYNAMIC";
}

double WT_AERODYNAMIC_MODEL::get_damping_in_pu() const
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    if(gen==NULL)
        return 0.0;

    WT_TURBINE_MODEL* turbine = gen->get_wt_turbine_model();
    if(turbine==NULL)
        return 0.0;
    else
        return turbine->get_damping_in_pu();
}

void WT_AERODYNAMIC_MODEL::set_number_of_pole_pairs(size_t n)
{
    if(n==0)
        n = 1;
    number_of_pole_pairs = n;
}

void WT_AERODYNAMIC_MODEL::set_generator_to_turbine_gear_ratio(double r)
{
    if(r<FLOAT_EPSILON)
        r = FLOAT_EPSILON;
    generator_to_turbine_gear_ratio = r;
}

void WT_AERODYNAMIC_MODEL::set_gear_efficiency(double eta)
{
    if(eta>1.0)
        eta = 1.0;
    if(eta<FLOAT_EPSILON)
        eta = FLOAT_EPSILON;
    gear_efficiency = eta;
}

void WT_AERODYNAMIC_MODEL::set_turbine_blade_radius_in_m(double r)
{
    if(r<FLOAT_EPSILON)
        r = FLOAT_EPSILON;
    turbine_blade_radius_in_m = r;
}

void WT_AERODYNAMIC_MODEL::set_nominal_wind_speed_in_mps(double v)
{
    if(v<FLOAT_EPSILON)
        v = FLOAT_EPSILON;
    nominal_wind_speed_in_mps = v;
}

void WT_AERODYNAMIC_MODEL::set_nominal_air_density_in_kgpm3(double rou)
{
    if(rou<FLOAT_EPSILON)
        rou = FLOAT_EPSILON;
    nominal_air_density_in_kgpm3 = rou;
}

size_t WT_AERODYNAMIC_MODEL::get_number_of_pole_pairs() const
{
    return number_of_pole_pairs;
}

double WT_AERODYNAMIC_MODEL::get_generator_to_turbine_gear_ratio() const
{
    return generator_to_turbine_gear_ratio;
}

double WT_AERODYNAMIC_MODEL::get_gear_efficiency() const
{
    return gear_efficiency;
}

double WT_AERODYNAMIC_MODEL::get_turbine_blade_radius_in_m() const
{
    return turbine_blade_radius_in_m;
}

double WT_AERODYNAMIC_MODEL::get_nominal_wind_speed_in_mps() const
{
    return nominal_wind_speed_in_mps;
}

double WT_AERODYNAMIC_MODEL::get_nominal_air_density_in_kgpm3() const
{
    return nominal_air_density_in_kgpm3;
}

double WT_AERODYNAMIC_MODEL::get_nominal_turbine_speed_in_rad_per_s() const
{
    double fbase = get_power_system_base_frequency_in_Hz();
    size_t npair = get_number_of_pole_pairs();
    double tratio = get_generator_to_turbine_gear_ratio();
    double fn = fbase/(npair*tratio);
    return hz2radps(fn);
}

double WT_AERODYNAMIC_MODEL::get_initial_turbine_speed_in_rad_per_s() const
{
    return initial_turbine_speed_in_rad_per_s;
}

void WT_AERODYNAMIC_MODEL::set_air_density_in_kgpm3(double rou)
{
    if(rou<FLOAT_EPSILON)
        rou = FLOAT_EPSILON;
    air_density_in_kgpm3 = rou;
}

void WT_AERODYNAMIC_MODEL::set_overspeed_mode_flag(bool flag)
{
    overspeed_mode_flag = flag;
}

double WT_AERODYNAMIC_MODEL::get_air_density_in_kgpm3() const
{
    return air_density_in_kgpm3;
}

bool WT_AERODYNAMIC_MODEL::get_overspeed_mode_flag() const
{
    return overspeed_mode_flag;
}


double WT_AERODYNAMIC_MODEL::get_wind_speed_in_mps() const
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    if(gen==NULL)
        return 0.0;

    WIND_SPEED_MODEL* model = gen->get_wind_speed_model();
    if(model!=NULL and model->is_model_initialized())
        return model->get_wind_speed_in_mps();
    else
        return get_nominal_wind_speed_in_mps();
}


double WT_AERODYNAMIC_MODEL::get_pitch_angle_in_deg() const
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    if(gen==NULL)
        return 0.0;

    WT_PITCH_MODEL* model = gen->get_wt_pitch_model();
    if(model!=NULL and model->is_model_initialized())
        return model->get_pitch_angle_in_deg();
    else
        return 0.0;
}

double WT_AERODYNAMIC_MODEL::get_turbine_speed_in_rad_per_s() const
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    if(gen==NULL)
        return 0.0;

    WT_TURBINE_MODEL* model = gen->get_wt_turbine_model();
    if(model!=NULL and model->is_model_initialized())
        return model->get_wind_turbine_generator_speed_in_pu();
    else
        return get_initial_turbine_speed_in_rad_per_s();
}

double WT_AERODYNAMIC_MODEL::get_turbine_frequency_in_Hz() const
{
    return radps2hz(get_turbine_speed_in_rad_per_s());
}

double WT_AERODYNAMIC_MODEL::get_total_wind_power_per_wt_generator_in_MW(double vwind) const
{
    double rou = get_air_density_in_kgpm3();
    double r = get_turbine_blade_radius_in_m();
    double r2 = r*r;
    double v3 = vwind*vwind*vwind;
    return 0.5*rou*PI*r2*v3*1e-6;
}


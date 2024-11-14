#include "header/model/common_model/primary_model/primary_model.h"

PRIMARY_MODEL::PRIMARY_MODEL(STEPS& toolkit):COMMON_MODEL(toolkit)
{
    clear();
}

PRIMARY_MODEL::~PRIMARY_MODEL()
{
    ;
}

void PRIMARY_MODEL::clear()
{
    set_speed_in_pu(1.0);
    set_active_power_reference_in_pu(1.0);
    set_speed_reference_in_pu(1.0);
    set_valve_reference_in_pu(1.0);
    set_initial_active_power_reference_in_pu(1.0);
    set_initial_speed_in_pu(1.0);
    set_initial_mechanical_power_in_pu(1.0);
    set_Ka(0.0);
    set_Eb(0.0);
    set_Bp(0.0);
}

void PRIMARY_MODEL::set_speed_in_pu(double s)
{
    speed_in_pu = s;
}

void PRIMARY_MODEL::set_active_power_reference_in_pu(double p)
{
    active_power_reference_in_pu = p;
}

void PRIMARY_MODEL::set_speed_reference_in_pu(double s)
{
    speed_reference_in_pu = s;
}

void PRIMARY_MODEL::set_valve_reference_in_pu(double v)
{
    valve_reference_in_pu = v;
}

void PRIMARY_MODEL::set_initial_active_power_reference_in_pu(double p)
{
    initial_Pref = p;
}

void PRIMARY_MODEL::set_initial_speed_in_pu(double s)
{
    initial_speed = s;
}

void PRIMARY_MODEL::set_initial_mechanical_power_in_pu(double p)
{
    initial_Pmech = p;
}

void PRIMARY_MODEL::set_initial_valve_reference_in_pu(double p)
{
    initial_valve_reference = p;
}

void PRIMARY_MODEL::set_Ka(double K)
{
    Ka = K;
}

void PRIMARY_MODEL::set_Eb(double K)
{
    Eb = K;
}

void PRIMARY_MODEL::set_Bp(double K)
{
    Bp = K;
}

double PRIMARY_MODEL::get_speed_in_pu() const
{
    return speed_in_pu;
}

double PRIMARY_MODEL::get_active_power_reference_in_pu() const
{
    return active_power_reference_in_pu;
}

double PRIMARY_MODEL::get_speed_reference_in_pu() const
{
    return speed_reference_in_pu;
}

double PRIMARY_MODEL::get_valve_reference_in_pu() const
{
    return valve_reference_in_pu;
}

double PRIMARY_MODEL::get_initial_active_power_reference_in_pu() const
{
    return initial_Pref;
}

double PRIMARY_MODEL::get_initial_speed_in_pu() const
{
    return initial_speed;
}

double PRIMARY_MODEL::get_initial_mechanical_power_in_pu() const
{
    return initial_Pmech;
}

double PRIMARY_MODEL::get_initial_valve_reference_in_pu() const
{
    return initial_valve_reference;
}

double PRIMARY_MODEL::get_Ka() const
{
    return Ka;
}

double PRIMARY_MODEL::get_Eb() const
{
    return Eb;
}

double PRIMARY_MODEL::get_Bp() const
{
    return Bp;
}


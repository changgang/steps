#include "header/model/converter_common_model/converter_model/vs_converter_model/vs_converter_model.h"
#include "header/basic/utility.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
#include <iostream>
using namespace std;
VS_CONVERTER_MODEL::VS_CONVERTER_MODEL(STEPS& toolkit) : CONVERTER_MODEL(toolkit)
{
    set_toolkit(toolkit);
}

VS_CONVERTER_MODEL::~VS_CONVERTER_MODEL()
{
    ;
}

void VS_CONVERTER_MODEL::set_Pref_in_pu_based_on_mbase(double P)
{
    Pref = P;
}

void VS_CONVERTER_MODEL::set_Qref_in_pu_based_on_mbase(double Q)
{
    Qref = Q;
}

void VS_CONVERTER_MODEL::set_Vref_in_pu(double V)
{
    Vref = V;
}

double VS_CONVERTER_MODEL::get_Pref_in_pu_based_on_mbase() const
{
    return Pref;
}

double VS_CONVERTER_MODEL::get_Qref_in_pu_based_on_mbase() const
{
    return Qref;
}

double VS_CONVERTER_MODEL::get_Vref_in_pu() const
{
    return Vref;
}

void VS_CONVERTER_MODEL::set_initial_virtual_voltage_in_pu(double V)
{
    V0 = V;
}

void VS_CONVERTER_MODEL::set_initial_virtual_angle_in_rad(double angle)
{
    angle0 = angle;
}
double VS_CONVERTER_MODEL::get_initial_virtual_voltage_in_pu() const
{
    return V0;
}

double VS_CONVERTER_MODEL::get_initial_virtual_angle_in_rad() const
{
    return angle0;
}

double VS_CONVERTER_MODEL::get_virtual_frequency_deviation_in_Hz() const
{
    return get_virtual_frequency_deviation_in_pu()*get_base_frequency_in_Hz();
}

double VS_CONVERTER_MODEL::get_virtual_frequency_in_pu() const
{
    return get_virtual_frequency_deviation_in_pu()+1.0;
}

double VS_CONVERTER_MODEL::get_virtual_frequency_in_Hz() const
{
    return get_virtual_frequency_deviation_in_Hz()+get_base_frequency_in_Hz();
}

double VS_CONVERTER_MODEL::get_virtual_angle_in_deg() const
{
    return rad2deg(get_virtual_angle_in_rad());
}

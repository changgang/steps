#include "header/model/converter_common_models/vsg_model/vsg_model2.h"
#include "header/basic/utility.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
#include <iostream>
using namespace std;
VSG_MODEL2::VSG_MODEL2(STEPS& toolkit)
{
    set_toolkit(toolkit);
}

VSG_MODEL2::~VSG_MODEL2()
{
    ;
}

void VSG_MODEL2::set_toolkit(STEPS& toolkit)
{
    this->toolkit = &toolkit;
}

STEPS& VSG_MODEL2::get_toolkit() const
{
    return *toolkit;
}

void VSG_MODEL2::set_Pref_in_pu_based_on_mbase(double P)
{
    Pref = P;
}

void VSG_MODEL2::set_Qref_in_pu_based_on_mbase(double Q)
{
    Qref = Q;
}

void VSG_MODEL2::set_Vref_in_pu(double V)
{
    Vref = V;
}

void VSG_MODEL2::set_P_in_pu_based_on_mbase(double P)
{
    this->P = P;
}

void VSG_MODEL2::set_Q_in_pu_based_on_mbase(double Q)
{
    this->Q = Q;
}

void VSG_MODEL2::set_V_in_pu(double V)
{
    this->V = V;
}

double VSG_MODEL2::get_Pref_in_pu_based_on_mbase() const
{
    return Pref;
}

double VSG_MODEL2::get_Qref_in_pu_based_on_mbase() const
{
    return Qref;
}

double VSG_MODEL2::get_Vref_in_pu() const
{
    return Vref;
}

double VSG_MODEL2::get_P_in_pu_based_on_mbase() const
{
    return P;
}

double VSG_MODEL2::get_Q_in_pu_based_on_mbase() const
{
    return Q;
}

double VSG_MODEL2::get_V_in_pu() const
{
    return V;
}

void VSG_MODEL2::set_initial_virtual_voltage_in_pu(double V)
{
    V0 = V;
}

void VSG_MODEL2::set_initial_virtual_angle_in_rad(double angle)
{
    angle0 = angle;
}
double VSG_MODEL2::get_initial_virtual_voltage_in_pu() const
{
    return V0;
}

double VSG_MODEL2::get_initial_virtual_angle_in_rad() const
{
    return angle0;
}

double VSG_MODEL2::get_virtual_frequency_deviation_in_pu() const
{
    return get_virtual_frequency_deviation_in_Hz()/get_base_frequency_in_Hz();
}

double VSG_MODEL2::get_virtual_frequency_in_pu() const
{
    return get_virtual_frequency_deviation_in_pu()+1.0;
}

double VSG_MODEL2::get_virtual_frequency_in_Hz() const
{
    return get_virtual_frequency_deviation_in_Hz()+get_base_frequency_in_Hz();
}

double VSG_MODEL2::get_virtual_angle_in_deg() const
{
    return rad2deg(get_virtual_angle_in_rad());
}

void VSG_MODEL2::initialize_base_frequency_and_base_angle_speed()
{
    double fbase = busptr->get_base_frequency_in_Hz();
    this->fbase_Hz = fbase;
    wbase_radps = DOUBLE_PI*fbase;
}

double VSG_MODEL2::get_base_frequency_in_Hz() const
{
    return fbase_Hz;
}

double VSG_MODEL2::get_base_angle_speed_in_radps() const
{
    return wbase_radps;
}


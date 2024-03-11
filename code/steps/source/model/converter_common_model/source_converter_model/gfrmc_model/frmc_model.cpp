#include "header/model/converter_common_models/source_converter_model/gfrmc_model/gfrmc_model.h"
#include "header/basic/utility.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
#include <iostream>
using namespace std;
GFRMC_MODEL::GFRMC_MODEL(STEPS& toolkit) : SOURCE_CONVERTER_MODEL(toolkit)
{
    set_toolkit(toolkit);
}

GFRMC_MODEL::~GFRMC_MODEL()
{
    ;
}

void GFRMC_MODEL::set_Pref_in_pu_based_on_mbase(double P)
{
    Pref = P;
}

void GFRMC_MODEL::set_Qref_in_pu_based_on_mbase(double Q)
{
    Qref = Q;
}

void GFRMC_MODEL::set_Vref_in_pu(double V)
{
    Vref = V;
}

void GFRMC_MODEL::set_P_in_pu_based_on_mbase(double P)
{
    this->P = P;
}

void GFRMC_MODEL::set_Q_in_pu_based_on_mbase(double Q)
{
    this->Q = Q;
}

void GFRMC_MODEL::set_V_in_pu(double V)
{
    this->V = V;
}

double GFRMC_MODEL::get_Pref_in_pu_based_on_mbase() const
{
    return Pref;
}

double GFRMC_MODEL::get_Qref_in_pu_based_on_mbase() const
{
    return Qref;
}

double GFRMC_MODEL::get_Vref_in_pu() const
{
    return Vref;
}

double GFRMC_MODEL::get_P_in_pu_based_on_mbase() const
{
    return P;
}

double GFRMC_MODEL::get_Q_in_pu_based_on_mbase() const
{
    return Q;
}

double GFRMC_MODEL::get_V_in_pu() const
{
    return V;
}

void GFRMC_MODEL::set_initial_virtual_voltage_in_pu(double V)
{
    V0 = V;
}

void GFRMC_MODEL::set_initial_virtual_angle_in_rad(double angle)
{
    angle0 = angle;
}
double GFRMC_MODEL::get_initial_virtual_voltage_in_pu() const
{
    return V0;
}

double GFRMC_MODEL::get_initial_virtual_angle_in_rad() const
{
    return angle0;
}

double GFRMC_MODEL::get_virtual_frequency_deviation_in_pu() const
{
    return get_virtual_frequency_deviation_in_Hz()/get_base_frequency_in_Hz();
}

double GFRMC_MODEL::get_virtual_frequency_in_pu() const
{
    return get_virtual_frequency_deviation_in_pu()+1.0;
}

double GFRMC_MODEL::get_virtual_frequency_in_Hz() const
{
    return get_virtual_frequency_deviation_in_Hz()+get_base_frequency_in_Hz();
}

double GFRMC_MODEL::get_virtual_angle_in_deg() const
{
    return rad2deg(get_virtual_angle_in_rad());
}

void GFRMC_MODEL::initialize_base_frequency_and_base_angle_speed()
{
    double fbase = get_bus_pointer()->get_base_frequency_in_Hz();
    this->fbase_Hz = fbase;
    wbase_radps = DOUBLE_PI*fbase;
}

double GFRMC_MODEL::get_base_frequency_in_Hz() const
{
    return fbase_Hz;
}

double GFRMC_MODEL::get_base_angle_speed_in_radps() const
{
    return wbase_radps;
}


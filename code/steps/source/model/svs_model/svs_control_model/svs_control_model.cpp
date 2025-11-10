#include "header/model/svs_model/svs_control_model/svs_control_model.h"
#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include "header/steps_namespace.h"
#include <cstdio>
#include <istream>
#include <iostream>

using namespace std;

SVS_CONTROL_MODEL::SVS_CONTROL_MODEL(STEPS& toolkit) : SVS_MODEL(toolkit)
{
    //set_current_source_flag(true);
}

SVS_CONTROL_MODEL::~SVS_CONTROL_MODEL()
{
    ;
}

string SVS_CONTROL_MODEL::get_model_type() const
{
    return "SVS CONTROL";
}

complex<double> SVS_CONTROL_MODEL::get_terminal_complex_current_in_pu_based_on_sbase()
{
    double Vterm = get_terminal_voltage_in_pu();
    double Iq = get_terminal_current_in_pu_based_on_sbase();
    return complex<double> (0.0, -Iq) * get_terminal_complex_voltage_in_pu() / Vterm;
}

double SVS_CONTROL_MODEL::get_terminal_reactive_power_in_MVar()
{
    STEPS& toolkit = get_toolkit();
    double sbase = toolkit.get_system_base_power_in_MVA();
    return get_terminal_reactive_power_in_pu_based_on_sbase() * sbase;
}

double SVS_CONTROL_MODEL::get_terminal_reactive_power_in_pu_based_on_sbase()
{
    return get_terminal_current_in_pu_based_on_sbase() * get_terminal_voltage_in_pu();
}

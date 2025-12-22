#ifndef LCC_HVDC2T_METER_GETTER_H
#define LCC_HVDC2T_METER_GETTER_H

#include "header/meter/meter_parameter_struct.h"
#include <string>
using namespace std;

namespace lcc_hvdc2t_meter_getter
{
    double get_dc_current_in_kA(const METER_PARAMETER& para);
    double get_rectifier_dc_current_in_kA(const METER_PARAMETER& para);
    double get_rectifier_ac_current_in_kA(const METER_PARAMETER& para);
    double get_inverter_dc_current_in_kA(const METER_PARAMETER& para);
    double get_inverter_ac_current_in_kA(const METER_PARAMETER& para);
    double get_rectifier_alpha_in_deg(const METER_PARAMETER& para);
    double get_inverter_gamma_in_deg(const METER_PARAMETER& para);
    double get_rectifier_mu_in_deg(const METER_PARAMETER& para);
    double get_inverter_mu_in_deg(const METER_PARAMETER& para);
    double get_rectifier_dc_voltage_in_kV(const METER_PARAMETER& para);
    double get_inverter_dc_voltage_in_kV(const METER_PARAMETER& para);
    double get_rectifier_ac_voltage_in_pu(const METER_PARAMETER& para);
    double get_inverter_ac_voltage_in_pu(const METER_PARAMETER& para);
    double get_rectifier_dc_power_in_MW(const METER_PARAMETER& para);
    double get_inverter_dc_power_in_MW(const METER_PARAMETER& para);
    double get_rectifier_ac_active_power_in_MW(const METER_PARAMETER& para);
    double get_inverter_ac_active_power_in_MW(const METER_PARAMETER& para);
    double get_rectifier_ac_reactive_power_in_MVar(const METER_PARAMETER& para);
    double get_inverter_ac_reactive_power_in_MVar(const METER_PARAMETER& para);
    double get_lcc_hvdc2t_model_internal_variable(const METER_PARAMETER& para);
}

#endif // LCC_HVDC2T_METER_GETTER_H

#ifndef PVU_METER_GETTER_H
#define PVU_METER_GETTER_H

#include "header/meter/meter_parameter_struct.h"
#include <string>
using namespace std;

namespace pvu_meter_interface
{
    double get_terminal_current_in_pu_mbase(const METER_PARAMETER& para);
    double get_terminal_current_in_pu_sbase(const METER_PARAMETER& para);
    double get_terminal_current_in_kA(const METER_PARAMETER& para);

    double get_terminal_active_power_in_pu_mbase(const METER_PARAMETER& para);
    double get_terminal_active_power_in_pu_sbase(const METER_PARAMETER& para);
    double get_terminal_active_power_in_MW(const METER_PARAMETER& para);

    double get_terminal_reactive_power_in_pu_mbase(const METER_PARAMETER& para);
    double get_terminal_reactive_power_in_pu_sbase(const METER_PARAMETER& para);
    double get_terminal_reactive_power_in_MVar(const METER_PARAMETER& para);

    double get_terminal_apparent_power_in_pu_mbase(const METER_PARAMETER& para);
    double get_terminal_apparent_power_in_pu_sbase(const METER_PARAMETER& para);
    double get_terminal_apparent_power_in_MVA(const METER_PARAMETER& para);

    double get_active_current_command_in_pu(const METER_PARAMETER& para);
    double get_reactive_current_command_in_pu(const METER_PARAMETER& para);
    double get_active_power_command_in_pu(const METER_PARAMETER& para);
    double get_reactive_power_command_in_pu(const METER_PARAMETER& para);
    double get_reactive_voltage_command_in_pu(const METER_PARAMETER& para);

    double get_solar_irradiance_pu_S0(const METER_PARAMETER& para);
    double get_solar_irradiance_pu_Sref(const METER_PARAMETER& para);
    double get_solar_irradiance_wpm2(const METER_PARAMETER& para);

    double get_temperature_c_deg(const METER_PARAMETER& para);

    double get_max_available_power_irradiance_pu_mbase(const METER_PARAMETER& para);
    double get_max_available_power_irradiance_pu_sbase(const METER_PARAMETER& para);
    double get_max_available_power_irradiance_MW(const METER_PARAMETER& para);

    double get_panel_power_in_pu_mbase(const METER_PARAMETER& para);
    double get_panel_power_in_MW(const METER_PARAMETER& para);

    double get_converter_model_internal_variable(const METER_PARAMETER& para);
    double get_panel_model_internal_variable(const METER_PARAMETER& para);
    double get_electrical_model_internal_variable(const METER_PARAMETER& para);
    double get_irradiance_model_internal_variable(const METER_PARAMETER& para);
    double get_temperature_model_internal_variable(const METER_PARAMETER& para);
    double get_vrt_model_internal_variable(const METER_PARAMETER& para);
    double get_relay_model_internal_variable(const METER_PARAMETER& para);
}

#endif // PVU_METER_GETTER_H

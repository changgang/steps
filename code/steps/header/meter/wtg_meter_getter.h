#ifndef WTG_METER_GETTER_H
#define WTG_METER_GETTER_H

#include "header/meter/meter_parameter_struct.h"
#include <string>
using namespace std;

namespace wtg_meter_getter
{
    double get_terminal_current_pu_mbase(const METER_PARAMETER& para);
    double get_terminal_current_pu_sbase(const METER_PARAMETER& para);
    double get_terminal_current_ka(const METER_PARAMETER& para);
    double get_terminal_active_power_pu_mbase(const METER_PARAMETER& para);
    double get_terminal_active_power_pu_sbase(const METER_PARAMETER& para);
    double get_terminal_active_power_mw(const METER_PARAMETER& para);
    double get_terminal_reactive_power_pu_mbase(const METER_PARAMETER& para);
    double get_terminal_reactive_power_pu_sbase(const METER_PARAMETER& para);
    double get_terminal_reactive_power_mvar(const METER_PARAMETER& para);
    double get_terminal_apparent_power_pu_mbase(const METER_PARAMETER& para);
    double get_terminal_apparent_power_pu_sbase(const METER_PARAMETER& para);
    double get_terminal_apparent_power_mva(const METER_PARAMETER& para);
    double get_mechanical_power_pu_mbase(const METER_PARAMETER& para);
    double get_mechanical_power_pu_sbase(const METER_PARAMETER& para);
    double get_mechanical_power_mw(const METER_PARAMETER& para);
    double get_max_available_mechanical_power_pu_mbase(const METER_PARAMETER& para);
    double get_max_available_mechanical_power_pu_sbase(const METER_PARAMETER& para);
    double get_max_available_mechanical_power_mw(const METER_PARAMETER& para);
    double get_speed_reference_pu(const METER_PARAMETER& para);
    double get_speed_reference_rad_s(const METER_PARAMETER& para);
    double get_turbine_speed_deviation_pu(const METER_PARAMETER& para);
    double get_turbine_speed_deviation_hz(const METER_PARAMETER& para);
    double get_turbine_speed_pu(const METER_PARAMETER& para);
    double get_turbine_speed_hz(const METER_PARAMETER& para);
    double get_rotor_speed_deviation_pu(const METER_PARAMETER& para);
    double get_rotor_speed_deviation_hz(const METER_PARAMETER& para);
    double get_rotor_speed_pu(const METER_PARAMETER& para);
    double get_rotor_speed_hz(const METER_PARAMETER& para);
    double get_rotor_angle_deg(const METER_PARAMETER& para);
    double get_rotor_angle_rad(const METER_PARAMETER& para);
    double get_active_current_command_pu(const METER_PARAMETER& para);
    double get_reactive_current_command_pu(const METER_PARAMETER& para);
    double get_active_power_command_pu(const METER_PARAMETER& para);
    double get_reactive_power_command_pu(const METER_PARAMETER& para);
    double get_reactive_voltage_command_pu(const METER_PARAMETER& para);
    double get_pitch_angle_deg(const METER_PARAMETER& para);
    double get_wind_speed_pu(const METER_PARAMETER& para);
    double get_wind_speed_mps(const METER_PARAMETER& para);
    double get_braking_resistor_power_pu(const METER_PARAMETER& para);
    double get_generator_model_internal_variable(const METER_PARAMETER& para);
    double get_aerodynamic_model_internal_variable(const METER_PARAMETER& para);
    double get_turbine_model_internal_variable(const METER_PARAMETER& para);
    double get_electrical_model_internal_variable(const METER_PARAMETER& para);
    double get_pitch_model_internal_variable(const METER_PARAMETER& para);
    double get_vrt_model_internal_variable(const METER_PARAMETER& para);
    double get_relay_model_internal_variable(const METER_PARAMETER& para);
    double get_windspeed_model_internal_variable(const METER_PARAMETER& para);
    double get_dbr_model_internal_variable(const METER_PARAMETER& para);
};

#endif // WTG_METER_GETTER_H

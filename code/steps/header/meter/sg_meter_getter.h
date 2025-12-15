#ifndef SG_METER_GETTER_H
#define SG_METER_GETTER_H

#include "header/meter/meter_parameter_struct.h"
#include "header/basic/steps_enum.h"
#include <string>
using namespace std;

namespace sg_meter_getter
{
    double get_rotor_angle_deg(const METER_PARAMETER& para);
    double get_rotor_angle_rad(const METER_PARAMETER& para);
    double get_rotor_speed_pu(const METER_PARAMETER& para);
    double get_rotor_speed_hz(const METER_PARAMETER& para);
    double get_rotor_speed_deviation_pu(const METER_PARAMETER& para);
    double get_rotor_speed_deviation_hz(const METER_PARAMETER& para);
    double get_terminal_voltage_pu(const METER_PARAMETER& para);
    double get_internal_voltage_pu(const METER_PARAMETER& para);
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
    double get_airgap_power_pu_mbase(const METER_PARAMETER& para);
    double get_airgap_power_pu_sbase(const METER_PARAMETER& para);
    double get_airgap_power_mw(const METER_PARAMETER& para);
    double get_accelerating_power_pu_mbase(const METER_PARAMETER& para);
    double get_accelerating_power_pu_sbase(const METER_PARAMETER& para);
    double get_accelerating_power_mw(const METER_PARAMETER& para);
    double get_mechanical_power_pu_mbase(const METER_PARAMETER& para);
    double get_mechanical_power_pu_sbase(const METER_PARAMETER& para);
    double get_mechanical_power_mw(const METER_PARAMETER& para);
    double get_mechanical_power_reference_pu_mbase(const METER_PARAMETER& para);
    double get_mechanical_power_reference_pu_sbase(const METER_PARAMETER& para);
    double get_mechanical_power_reference_mw(const METER_PARAMETER& para);
    double get_compensated_voltage_pu(const METER_PARAMETER& para);
    double get_voltage_reference_pu(const METER_PARAMETER& para);
    double get_stabilizing_signal_pu(const METER_PARAMETER& para);
    double get_excitation_voltage_pu(const METER_PARAMETER& para);
    double get_sync_generator_model_internal_variable(const METER_PARAMETER& para);
    double get_compensator_model_internal_variable(const METER_PARAMETER& para);
    double get_stabilizer_model_internal_variable(const METER_PARAMETER& para);
    double get_exciter_model_internal_variable(const METER_PARAMETER& para);
    double get_turbine_governor_model_internal_variable(const METER_PARAMETER& para);
    double get_turbine_load_controller_model_internal_variable(const METER_PARAMETER& para);
}

#endif // SG_METER_GETTER_H

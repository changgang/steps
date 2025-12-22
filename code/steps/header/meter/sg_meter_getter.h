#ifndef SG_METER_GETTER_H
#define SG_METER_GETTER_H

#include "header/meter/meter_parameter_struct.h"
#include "header/basic/steps_enum.h"
#include <string>
using namespace std;

namespace sg_meter_getter
{
    double get_rotor_angle_in_deg(const METER_PARAMETER& para);
    double get_rotor_angle_in_rad(const METER_PARAMETER& para);
    double get_rotor_speed_in_pu(const METER_PARAMETER& para);
    double get_rotor_speed_in_Hz(const METER_PARAMETER& para);
    double get_rotor_speed_deviation_in_pu(const METER_PARAMETER& para);
    double get_rotor_speed_deviation_in_Hz(const METER_PARAMETER& para);
    double get_terminal_voltage_in_pu(const METER_PARAMETER& para);
    double get_internal_voltage_in_pu(const METER_PARAMETER& para);
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
    double get_airgap_power_in_pu_mbase(const METER_PARAMETER& para);
    double get_airgap_power_in_pu_sbase(const METER_PARAMETER& para);
    double get_airgap_power_in_MW(const METER_PARAMETER& para);
    double get_accelerating_power_in_pu_mbase(const METER_PARAMETER& para);
    double get_accelerating_power_in_pu_sbase(const METER_PARAMETER& para);
    double get_accelerating_power_in_MW(const METER_PARAMETER& para);
    double get_mechanical_power_in_pu_mbase(const METER_PARAMETER& para);
    double get_mechanical_power_in_pu_sbase(const METER_PARAMETER& para);
    double get_mechanical_power_in_MW(const METER_PARAMETER& para);
    double get_mechanical_power_reference_in_pu_mbase(const METER_PARAMETER& para);
    double get_mechanical_power_reference_in_pu_sbase(const METER_PARAMETER& para);
    double get_mechanical_power_reference_in_MW(const METER_PARAMETER& para);
    double get_compensated_voltage_in_pu(const METER_PARAMETER& para);
    double get_voltage_reference_in_pu(const METER_PARAMETER& para);
    double get_stabilizing_signal_in_pu(const METER_PARAMETER& para);
    double get_excitation_voltage_in_pu(const METER_PARAMETER& para);
    double get_sync_generator_model_internal_variable(const METER_PARAMETER& para);
    double get_compensator_model_internal_variable(const METER_PARAMETER& para);
    double get_stabilizer_model_internal_variable(const METER_PARAMETER& para);
    double get_exciter_model_internal_variable(const METER_PARAMETER& para);
    double get_turbine_governor_model_internal_variable(const METER_PARAMETER& para);
    double get_turbine_load_controller_model_internal_variable(const METER_PARAMETER& para);
}

#endif // SG_METER_GETTER_H

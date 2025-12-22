#ifndef ES_METER_GETTER_H
#define ES_METER_GETTER_H

#include "header/meter/meter_parameter_struct.h"
#include <string>
using namespace std;

namespace es_meter_getter
{
    double get_terminal_current_pu_mbase(const METER_PARAMETER& para);
    double get_terminal_current_pu_sbase(const METER_PARAMETER& para);
    double get_terminal_current_kA(const METER_PARAMETER& para);
    double get_terminal_active_power_pu_mbase(const METER_PARAMETER& para);
    double get_terminal_active_power_pu_sbase(const METER_PARAMETER& para);
    double get_terminal_active_power_MW(const METER_PARAMETER& para);
    double get_terminal_reactive_power_pu_mbase(const METER_PARAMETER& para);
    double get_terminal_reactive_power_pu_sbase(const METER_PARAMETER& para);
    double get_terminal_reactive_power_MVar(const METER_PARAMETER& para);
    double get_terminal_apparent_power_pu_mbase(const METER_PARAMETER& para);
    double get_terminal_apparent_power_pu_sbase(const METER_PARAMETER& para);
    double get_terminal_apparent_power_MVA(const METER_PARAMETER& para);
    double get_active_current_command_pu(const METER_PARAMETER& para);
    double get_reactive_current_command_pu(const METER_PARAMETER& para);
    double get_active_power_command_pu(const METER_PARAMETER& para);
    double get_reactive_power_command_pu(const METER_PARAMETER& para);
    double get_reactive_voltage_command_pu(const METER_PARAMETER& para);
    double get_state_of_charge_pu(const METER_PARAMETER& para);
    double get_soc_power_pu_mbase(const METER_PARAMETER& para);
    double get_soc_power_pu_sbase(const METER_PARAMETER& para);
    double get_soc_power_MW(const METER_PARAMETER& para);
    double get_mechanical_power_pu_mbase(const METER_PARAMETER& para);
    double get_rotor_speed_pu(const METER_PARAMETER& para);
    double get_rotor_speed_reference_pu(const METER_PARAMETER& para);
    double get_valve_reference_pu(const METER_PARAMETER& para);
    double get_valve_pu(const METER_PARAMETER& para);
    double get_active_power_reference_pu_mbase(const METER_PARAMETER& para);
    double get_frequency_regulation_power_pu_mbase(const METER_PARAMETER& para);
    double get_converter_model_internal_variable(const METER_PARAMETER& para);
    double get_soc_model_internal_variable(const METER_PARAMETER& para);
    double get_electrical_model_internal_variable(const METER_PARAMETER& para);
    double get_vrt_model_internal_variable(const METER_PARAMETER& para);
    double get_relay_model_internal_variable(const METER_PARAMETER& para);
    double get_primary_model_internal_variable(const METER_PARAMETER& para);
    double get_rotor_model_internal_variable(const METER_PARAMETER& para);
    double get_auxiliary_model_internal_variable(const METER_PARAMETER& para);
}

#endif // ES_METER_GETTER_H

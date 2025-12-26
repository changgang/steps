#ifndef EQUIVALENT_DEVICE_METER_GETTER_H
#define EQUIVALENT_DEVICE_METER_GETTER_H

#include "header/meter/meter_parameter_struct.h"
#include <string>
using namespace std;

namespace equivalent_device_meter_interface
{
    double get_voltage_source_voltage_in_pu(const METER_PARAMETER& para);
    double get_voltage_source_voltage_angle_in_deg(const METER_PARAMETER& para);
    double get_voltage_source_resistance_in_pu(const METER_PARAMETER& para);
    double get_voltage_source_reactance_in_pu(const METER_PARAMETER& para);

    double get_active_constant_power_load_in_MW(const METER_PARAMETER& para);
    double get_reactive_constant_power_load_in_MVar(const METER_PARAMETER& para);
    double get_active_constant_current_load_in_MW(const METER_PARAMETER& para);
    double get_reactive_constant_current_load_in_MVar(const METER_PARAMETER& para);
    double get_active_constant_impedance_load_in_MW(const METER_PARAMETER& para);
    double get_reactive_constant_impedance_load_in_MVar(const METER_PARAMETER& para);

    double get_active_power_generation_in_MW(const METER_PARAMETER& para);
    double get_reactive_power_generation_in_MVar(const METER_PARAMETER& para);
    double get_active_power_load_in_MW(const METER_PARAMETER& para);
    double get_reactive_power_load_in_MVar(const METER_PARAMETER& para);
    double get_active_power_net_load_in_MW(const METER_PARAMETER& para);
    double get_reactive_power_net_load_in_MVar(const METER_PARAMETER& para);

    double get_active_constant_power_load_in_pu(const METER_PARAMETER& para);
    double get_reactive_constant_power_load_in_pu(const METER_PARAMETER& para);
    double get_active_constant_current_load_in_pu(const METER_PARAMETER& para);
    double get_reactive_constant_current_load_in_pu(const METER_PARAMETER& para);
    double get_active_constant_impedance_load_in_pu(const METER_PARAMETER& para);
    double get_reactive_constant_impedance_load_in_pu(const METER_PARAMETER& para);

    double get_active_power_generation_in_pu(const METER_PARAMETER& para);
    double get_reactive_power_generation_in_pu(const METER_PARAMETER& para);
    double get_active_power_load_in_pu(const METER_PARAMETER& para);
    double get_reactive_power_load_in_pu(const METER_PARAMETER& para);
    double get_active_power_net_load_in_pu(const METER_PARAMETER& para);
    double get_reactive_power_net_load_in_pu(const METER_PARAMETER& para);
}

#endif //EQUIVALENT_DEVICE_METER_GETTER_H

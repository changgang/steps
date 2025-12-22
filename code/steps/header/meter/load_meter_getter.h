#ifndef LOAD_METER_GETTER_H
#define LOAD_METER_GETTER_H

#include "header/meter/meter_parameter_struct.h"
#include <string>
using namespace std;

namespace load_meter_getter
{
    double get_active_power_in_MW(const METER_PARAMETER& para);
    double get_active_power_in_pu(const METER_PARAMETER& para);
    double get_reactive_power_in_MVar(const METER_PARAMETER& para);
    double get_reactive_power_in_pu(const METER_PARAMETER& para);
    double get_current_in_kA(const METER_PARAMETER& para);
    double get_current_in_pu(const METER_PARAMETER& para);
    double get_total_scale_in_pu(const METER_PARAMETER& para);
    double get_manually_scale_in_pu(const METER_PARAMETER& para);
    double get_relay_shed_scale_in_pu(const METER_PARAMETER& para);
    double get_load_model_internal_variable(const METER_PARAMETER& para);
    double get_frequency_relay_model_internal_variable(const METER_PARAMETER& para);
    double get_voltage_relay_model_internal_variable(const METER_PARAMETER& para);
}

#endif // LOAD_METER_GETTER_H

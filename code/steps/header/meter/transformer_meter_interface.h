#ifndef TRANSFORMER_METER_GETTER_H
#define TRANSFORMER_METER_GETTER_H

#include "header/meter/meter_parameter_struct.h"
#include <string>
using namespace std;

namespace transformer_meter_interface
{
    double get_primary_side_current_in_kA(const METER_PARAMETER& para);
    double get_secondary_side_current_in_kA(const METER_PARAMETER& para);
    double get_tertiary_side_current_in_kA(const METER_PARAMETER& para);

    double get_primary_side_current_in_pu(const METER_PARAMETER& para);
    double get_secondary_side_current_in_pu(const METER_PARAMETER& para);
    double get_tertiary_side_current_in_pu(const METER_PARAMETER& para);

    double get_primary_side_active_power_in_MW(const METER_PARAMETER& para);
    double get_secondary_side_active_power_in_MW(const METER_PARAMETER& para);
    double get_tertiary_side_active_power_in_MW(const METER_PARAMETER& para);

    double get_primary_side_active_power_in_pu(const METER_PARAMETER& para);
    double get_secondary_side_active_power_in_pu(const METER_PARAMETER& para);
    double get_tertiary_side_active_power_in_pu(const METER_PARAMETER& para);

    double get_primary_side_reactive_power_in_MVar(const METER_PARAMETER& para);
    double get_secondary_side_reactive_power_in_MVar(const METER_PARAMETER& para);
    double get_tertiary_side_reactive_power_in_MVar(const METER_PARAMETER& para);

    double get_primary_side_reactive_power_in_pu(const METER_PARAMETER& para);
    double get_secondary_side_reactive_power_in_pu(const METER_PARAMETER& para);
    double get_tertiary_side_reactive_power_in_pu(const METER_PARAMETER& para);

    double get_winding_current_in_kA(const METER_PARAMETER& para);
    double get_winding_current_in_pu(const METER_PARAMETER& para);
    double get_winding_active_power_in_MW(const METER_PARAMETER& para);
    double get_winding_active_power_in_pu(const METER_PARAMETER& para);
    double get_winding_reactive_power_in_MVar(const METER_PARAMETER& para);
    double get_winding_reactive_power_in_pu(const METER_PARAMETER& para);
}

#endif //TRANSFORMER_METER_GETTER_H

#ifndef TRANSFORMER_METER_GETTER_H
#define TRANSFORMER_METER_GETTER_H

#include "header/meter/meter_parameter_struct.h"
#include <string>
using namespace std;

namespace transformer_meter_getter
{
    double get_primary_side_current_kA(const METER_PARAMETER& para);
    double get_secondary_side_current_kA(const METER_PARAMETER& para);
    double get_tertiary_side_current_kA(const METER_PARAMETER& para);

    double get_primary_side_current_pu(const METER_PARAMETER& para);
    double get_secondary_side_current_pu(const METER_PARAMETER& para);
    double get_tertiary_side_current_pu(const METER_PARAMETER& para);

    double get_primary_side_active_power_MW(const METER_PARAMETER& para);
    double get_secondary_side_active_power_MW(const METER_PARAMETER& para);
    double get_tertiary_side_active_power_MW(const METER_PARAMETER& para);

    double get_primary_side_active_power_pu(const METER_PARAMETER& para);
    double get_secondary_side_active_power_pu(const METER_PARAMETER& para);
    double get_tertiary_side_active_power_pu(const METER_PARAMETER& para);

    double get_primary_side_reactive_power_MVar(const METER_PARAMETER& para);
    double get_secondary_side_reactive_power_MVar(const METER_PARAMETER& para);
    double get_tertiary_side_reactive_power_MVar(const METER_PARAMETER& para);

    double get_primary_side_reactive_power_pu(const METER_PARAMETER& para);
    double get_secondary_side_reactive_power_pu(const METER_PARAMETER& para);
    double get_tertiary_side_reactive_power_pu(const METER_PARAMETER& para);

    double get_winding_current_kA(const METER_PARAMETER& para);
    double get_winding_current_pu(const METER_PARAMETER& para);
    double get_winding_active_power_MW(const METER_PARAMETER& para);
    double get_winding_active_power_pu(const METER_PARAMETER& para);
    double get_winding_reactive_power_MVar(const METER_PARAMETER& para);
    double get_winding_reactive_power_pu(const METER_PARAMETER& para);
}

#endif //TRANSFORMER_METER_GETTER_H

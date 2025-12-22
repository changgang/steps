#ifndef TRANSFORMER_METER_GETTER_H
#define TRANSFORMER_METER_GETTER_H

#include "header/meter/meter_parameter_struct.h"
#include <string>
using namespace std;

namespace transformer_meter_getter
{
    double get_current_kA_primary(const METER_PARAMETER& para);
    double get_current_kA_secondary(const METER_PARAMETER& para);
    double get_current_kA_tertiary(const METER_PARAMETER& para);

    double get_current_pu_primary(const METER_PARAMETER& para);
    double get_current_pu_secondary(const METER_PARAMETER& para);
    double get_current_pu_tertiary(const METER_PARAMETER& para);

    double get_active_power_MW_primary(const METER_PARAMETER& para);
    double get_active_power_MW_secondary(const METER_PARAMETER& para);
    double get_active_power_MW_tertiary(const METER_PARAMETER& para);

    double get_active_power_pu_primary(const METER_PARAMETER& para);
    double get_active_power_pu_secondary(const METER_PARAMETER& para);
    double get_active_power_pu_tertiary(const METER_PARAMETER& para);

    double get_reactive_power_MVar_primary(const METER_PARAMETER& para);
    double get_reactive_power_MVar_secondary(const METER_PARAMETER& para);
    double get_reactive_power_MVar_tertiary(const METER_PARAMETER& para);

    double get_reactive_power_pu_primary(const METER_PARAMETER& para);
    double get_reactive_power_pu_secondary(const METER_PARAMETER& para);
    double get_reactive_power_pu_tertiary(const METER_PARAMETER& para);

    double get_current_kA_bus_side(const METER_PARAMETER& para);
    double get_current_pu_bus_side(const METER_PARAMETER& para);
    double get_active_power_MW_bus_side(const METER_PARAMETER& para);
    double get_active_power_pu_bus_side(const METER_PARAMETER& para);
    double get_reactive_power_MVar_bus_side(const METER_PARAMETER& para);
    double get_reactive_power_pu_bus_side(const METER_PARAMETER& para);
}

#endif //TRANSFORMER_METER_GETTER_H

#ifndef TRANSFORMER_METER_GETTER_H
#define TRANSFORMER_METER_GETTER_H

#include "header/meter/meter_parameter_struct.h"
#include <string>
using namespace std;

namespace transformer_meter_getter
{
    double get_current_ka_primary(const METER_PARAMETER& para);
    double get_current_ka_secondary(const METER_PARAMETER& para);
    double get_current_ka_tertiary(const METER_PARAMETER& para);

    double get_current_pu_primary(const METER_PARAMETER& para);
    double get_current_pu_secondary(const METER_PARAMETER& para);
    double get_current_pu_tertiary(const METER_PARAMETER& para);

    double get_active_power_mw_primary(const METER_PARAMETER& para);
    double get_active_power_mw_secondary(const METER_PARAMETER& para);
    double get_active_power_mw_tertiary(const METER_PARAMETER& para);

    double get_active_power_pu_primary(const METER_PARAMETER& para);
    double get_active_power_pu_secondary(const METER_PARAMETER& para);
    double get_active_power_pu_tertiary(const METER_PARAMETER& para);

    double get_reactive_power_mvar_primary(const METER_PARAMETER& para);
    double get_reactive_power_mvar_secondary(const METER_PARAMETER& para);
    double get_reactive_power_mvar_tertiary(const METER_PARAMETER& para);

    double get_reactive_power_pu_primary(const METER_PARAMETER& para);
    double get_reactive_power_pu_secondary(const METER_PARAMETER& para);
    double get_reactive_power_pu_tertiary(const METER_PARAMETER& para);

    double get_current_ka_bus_side(const METER_PARAMETER& para);
    double get_current_pu_bus_side(const METER_PARAMETER& para);
    double get_active_power_mw_bus_side(const METER_PARAMETER& para);
    double get_active_power_pu_bus_side(const METER_PARAMETER& para);
    double get_reactive_power_mvar_bus_side(const METER_PARAMETER& para);
    double get_reactive_power_pu_bus_side(const METER_PARAMETER& para);
}

#endif //TRANSFORMER_METER_GETTER_H

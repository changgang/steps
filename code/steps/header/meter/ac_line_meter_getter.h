#ifndef AC_LINE_METER_GETTER_H
#define AC_LINE_METER_GETTER_H

#include <string>
using namespace std;

#include "header/meter/meter_parameter_struct.h"

namespace ac_line_meter_getter {
    double get_current_in_ka(const METER_PARAMETER& para);
    double get_current_in_pu(const METER_PARAMETER& para);
    double get_active_power_in_mw(const METER_PARAMETER& para);
    double get_active_power_in_pu(const METER_PARAMETER& para);
    double get_reactive_power_in_mvar(const METER_PARAMETER& para);
    double get_reactive_power_in_pu(const METER_PARAMETER& para);
    double get_apparent_impedance_in_ohm(const METER_PARAMETER& para);
    double get_apparent_impedance_in_pu(const METER_PARAMETER& para);
    double get_apparent_impedance_angle_in_deg(const METER_PARAMETER& para);
    double get_apparent_impedance_angle_in_rad(const METER_PARAMETER& para);
    double get_current_at_sending_side_in_ka(const METER_PARAMETER& para);
    double get_active_power_at_sending_side_in_mw(const METER_PARAMETER& para);
    double get_reactive_power_at_sending_side_in_mvar(const METER_PARAMETER& para);
    double get_current_at_receiving_side_in_ka(const METER_PARAMETER& para);
    double get_active_power_at_receiving_side_in_mw(const METER_PARAMETER& para);
    double get_reactive_power_at_receiving_side_in_mvar(const METER_PARAMETER& para);
    double get_apparent_impedance_at_sending_side_in_ohm(const METER_PARAMETER& para);
    double get_apparent_impedance_angle_at_sending_side_in_deg(const METER_PARAMETER& para);
    double get_apparent_impedance_at_receiving_side_in_ohm(const METER_PARAMETER& para);
    double get_apparent_impedance_angle_at_receiving_side_in_deg(const METER_PARAMETER& para);
}

#endif // AC_LINE_METER_GETTER_H

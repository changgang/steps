#ifndef BUS_METER_GETTER_H
#define BUS_METER_GETTER_H

#include "header/meter/meter_parameter_struct.h"
#include <string>
using namespace std;

namespace bus_meter_interface
{
    double get_voltage_in_pu(const METER_PARAMETER& para);
    double get_voltage_in_kV(const METER_PARAMETER& para);
    double get_angle_in_deg(const METER_PARAMETER& para);
    double get_angle_in_rad(const METER_PARAMETER& para);
    double get_frequency_deviation_in_pu(const METER_PARAMETER& para);
    double get_frequency_deviation_in_Hz(const METER_PARAMETER& para);
    double get_frequency_in_pu(const METER_PARAMETER& para);
    double get_frequency_in_Hz(const METER_PARAMETER& para);
    double get_rocof_in_Hz_per_second(const METER_PARAMETER& para);
    double get_rocof_pu_per_second(const METER_PARAMETER& para);
    double get_rocov_kV_per_second(const METER_PARAMETER& para);
    double get_rocov_pu_per_second(const METER_PARAMETER& para);
}

#endif

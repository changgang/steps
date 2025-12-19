#ifndef VSC_HVDC_METER_GETTER_H
#define VSC_HVDC_METER_GETTER_H

#include "header/meter/meter_parameter_struct.h"
#include <string>
using namespace std;

namespace vsc_hvdc_meter_getter
{
    double get_converter_ac_voltage_in_pu(const METER_PARAMETER& para);
    double get_converter_ac_voltage_in_kv(const METER_PARAMETER& para);
    double get_converter_ac_current_in_ka(const METER_PARAMETER& para);
    double get_converter_dc_voltage_in_kv(const METER_PARAMETER& para);
    double get_converter_dc_current_in_ka(const METER_PARAMETER& para);
    double get_converter_dc_power_in_mw(const METER_PARAMETER& para);
    double get_converter_ac_active_power_in_mw(const METER_PARAMETER& para);
    double get_converter_ac_reactive_power_in_mvar(const METER_PARAMETER& para);
    double get_dc_bus_voltage_in_kv(const METER_PARAMETER& para);
    double get_dc_line_current_in_ka(const METER_PARAMETER& para);
    double get_dc_line_power_in_mw(const METER_PARAMETER& para);
    double get_vsc_hvdc_network_model_internal_variable(const METER_PARAMETER& para);
    double get_vsc_hvdc_converter_model_internal_variable(const METER_PARAMETER& para);
}

#endif //VSC_HVDC_METER_GETTER_H

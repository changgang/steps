#ifndef VSC_HVDC_METER_GETTER_H
#define VSC_HVDC_METER_GETTER_H

#include "header/meter/meter_parameter_struct.h"
#include <string>
using namespace std;

namespace vsc_hvdc_meter_interface
{
    double get_converter_ac_voltage_in_pu(const METER_PARAMETER& para);
    double get_converter_ac_voltage_in_kV(const METER_PARAMETER& para);
    double get_converter_ac_current_in_kA(const METER_PARAMETER& para);
    double get_converter_dc_voltage_in_kV(const METER_PARAMETER& para);
    double get_converter_dc_current_in_kA(const METER_PARAMETER& para);
    double get_converter_dc_power_in_MW(const METER_PARAMETER& para);
    double get_converter_ac_active_power_in_MW(const METER_PARAMETER& para);
    double get_converter_ac_reactive_power_in_MVar(const METER_PARAMETER& para);
    double get_dc_bus_voltage_in_kV(const METER_PARAMETER& para);
    double get_dc_line_current_in_kA(const METER_PARAMETER& para);
    double get_dc_line_power_in_MW(const METER_PARAMETER& para);
    double get_vsc_hvdc_network_model_internal_variable(const METER_PARAMETER& para);
    double get_vsc_hvdc_converter_model_internal_variable(const METER_PARAMETER& para);
}

#endif //VSC_HVDC_METER_GETTER_H

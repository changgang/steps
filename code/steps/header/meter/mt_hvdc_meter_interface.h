#ifndef MT_HVDC_METER_GETTER_H
#define MT_HVDC_METER_GETTER_H

#include "header/meter/meter_parameter_struct.h"
#include <string>
using namespace std;

namespace mt_hvdc_meter_interface
{
    // LCC converter meter getters
    double get_lcc_converter_dc_current_in_kA(const METER_PARAMETER& para);
    double get_lcc_converter_dc_voltage_in_kV(const METER_PARAMETER& para);
    double get_lcc_converter_dc_power_in_MW(const METER_PARAMETER& para);
    double get_lcc_converter_ac_current_in_kA(const METER_PARAMETER& para);
    double get_lcc_converter_firing_angle_in_deg(const METER_PARAMETER& para);
    double get_lcc_converter_active_power_in_MW(const METER_PARAMETER& para);
    double get_lcc_converter_reactive_power_in_MVar(const METER_PARAMETER& para);
    double get_lcc_converter_reference_angle_of_cc_in_deg(const METER_PARAMETER& para);
    double get_lcc_converter_reference_angle_of_cea_in_deg(const METER_PARAMETER& para);
    double get_lcc_converter_reference_angle_of_cv_in_deg(const METER_PARAMETER& para);
    double get_lcc_converter_idc_ref_in_kA(const METER_PARAMETER& para);

    // VSC converter meter getters
    double get_vsc_converter_dc_current_in_kA(const METER_PARAMETER& para);
    double get_vsc_converter_dc_voltage_in_kV(const METER_PARAMETER& para);
    double get_vsc_converter_dc_power_in_MW(const METER_PARAMETER& para);
    double get_vsc_converter_ac_current_in_kA(const METER_PARAMETER& para);
    double get_vsc_converter_active_power_in_MW(const METER_PARAMETER& para);
    double get_vsc_converter_reactive_power_in_MVar(const METER_PARAMETER& para);
    double get_vsc_converter_id_in_kA(const METER_PARAMETER& para);
    double get_vsc_converter_iq_in_kA(const METER_PARAMETER& para);

    // DC bus meter getters
    double get_dc_bus_voltage_in_kV(const METER_PARAMETER& para);

    // DC line meter getters
    double get_dc_line_current_in_kA(const METER_PARAMETER& para);

    // Internal variable meter getters (currently return 0.0)
    double get_hlcc_converter_model_internal_variable(const METER_PARAMETER& para);
    double get_hvsc_converter_model_internal_variable(const METER_PARAMETER& para);
    double get_dc_network_model_internal_variable(const METER_PARAMETER& para);
}

#endif // MT_HVDC_METER_GETTER_H

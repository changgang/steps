#ifndef VSC_HVDC_VAR_H
#define VSC_HVDC_VAR_H

#include "header/basic/steps_enum.h"

#include <complex>
#include <string>


using namespace std;

class BUS;

struct VSC_HVDC_CONVERTER_STRUCT
{
    unsigned int converter_bus;
    BUS* converter_busptr;
    VSC_HVDC_DC_CONTROL_MODE dc_control_mode;
    VSC_HVDC_AC_CONTROL_MODE ac_control_mode;
    double nominal_ac_active_power_in_MW;
    double nominal_dc_voltage_in_kV;
    double initial_dc_voltage_reference_in_kV;
    double initial_dc_active_power_reference_in_MW;
    double initial_power_voltage_droop_coefficient;
    double initial_dc_current_reference_in_kA;
    double initial_power_current_droop_coefficient;
    double nominal_ac_voltage_in_pu;
    double nominal_ac_reactive_power_in_Mvar;
    double converter_loss_coefficient_A_in_kW;
    double converter_loss_coefficient_B_in_kW_per_amp;
    double converter_loss_coefficient_C_in_kW_per_amp_squard;
    double min_converter_loss_in_kW;
    double converter_rated_capacity_in_MVA;
    double converter_rated_current_in_amp;
    double power_weighting_factor;
    double Qmax_MVar;
    double Qmin_MVar;
    double Udmax_kV;
    double Udmin_kV;
    unsigned int remote_bus_to_regulate;
    double remote_regulation_percent;
};

struct VSC_HVDC_DC_BUS_STRUCT
{
    unsigned int dc_bus_number;
    unsigned int ac_bus_number;
    unsigned int dc_bus_area;
    unsigned int dc_bus_zone;
    unsigned int dc_bus_name_index;
    double converter_ground_resistance_in_ohm;
    unsigned int owner;
    double dc_generation_power_in_MW;
    double dc_load_power_in_MW;
};

struct VSC_HVDC_DC_LINE_STRUCT
{
    unsigned int sending_side_bus;
    unsigned int receiving_side_bus;
    unsigned int identifier_index;
    unsigned int meter_end_bus;
    double line_R_in_ohm;
    double line_L_in_mH;
};

#endif // VSC_HVDC_VAR_H

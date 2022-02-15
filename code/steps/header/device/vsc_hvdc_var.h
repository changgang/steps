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
    unsigned int converter_name_index;
    bool status;
    VSC_HVDC_CONVERTER_ACTIVE_POWER_CONTROL_MODE dc_control_mode;
    unsigned int dc_voltage_control_priority;
    VSC_HVDC_CONVERTER_REACTIVE_POWER_CONTROL_MODE ac_control_mode;
    double nominal_ac_active_power_in_MW;
    double nominal_dc_voltage_in_kV;
    double initial_dc_voltage_reference_in_kV;
    double initial_dc_active_power_reference_in_MW;
    double droop_coefficient_for_droop_control;
    double initial_dc_current_reference_in_kA;
    double nominal_ac_voltage_in_pu;
    double nominal_ac_reactive_power_in_Mvar;
    double converter_loss_coefficient_A_in_kW;
    double converter_loss_coefficient_B_in_kW_per_amp;
    double converter_loss_coefficient_C_in_kW_per_amp_squard;
    double min_converter_loss_in_kW;
    double converter_rated_capacity_in_MVA;
    double converter_rated_current_in_amp;
    double converter_transformer_capacity_in_MVA;
    double converter_transformer_AC_side_base_voltage_in_kV;
    double converter_transformer_converter_side_base_voltage_in_kV;
    double converter_transformer_off_nominal_turn_ratio;
    complex<double> converter_transformer_impedance_in_pu;
    complex<double> converter_commutating_impedance_in_ohm;
    complex<double> converter_filter_admittance_in_siemens;
    double P_to_AC_bus_MW;
    double Q_to_AC_bus_MVar;
    double Pmax_MW;
    double Pmin_MW;
    double Qmax_MVar;
    double Qmin_MVar;
    double Udmax_kV;
    double Udmin_kV;
    double remote_bus_to_regulate;
    double remote_regulation_percent;

    double Pdc_command_in_MW;
    double Pac_command_in_MW;

    // dynamic variables
    double Pdc_flowing_out_Ceq_in_MW;
};

struct VSC_HVDC_DC_BUS_STRUCT
{
    unsigned int dc_bus_number;
    unsigned int converter_index;
    double Vdc_kV;
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
    bool status;
    unsigned int meter_end_bus;
    double line_R_in_ohm;
    double line_L_in_mH;

    double fault_location_to_sending_side_bus;
    double fault_R_in_ohm;
};

#endif // VSC_HVDC_VAR_H

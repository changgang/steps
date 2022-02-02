#include "header/device/vsc_hvdc.h"
#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include "header/STEPS.h"
#include "header/steps_namespace.h"
#include "header/model/hvdc_model/hvdc_models.h"
#include <istream>
#include <iostream>
#include <cstdio>
#include <cmath>

using namespace std;

VSC_HVDC::VSC_HVDC(STEPS& toolkit) : NONBUS_DEVICE(toolkit)
{
    vsc_hvdc_model = NULL;
    clear();
}
VSC_HVDC::VSC_HVDC(const VSC_HVDC& vsc) : NONBUS_DEVICE(vsc.get_toolkit())
{
    vsc_hvdc_model = NULL;
    clear();

    copy_from_const_vsc(vsc);
}

VSC_HVDC& VSC_HVDC::operator=(const VSC_HVDC& vsc)
{
    if(this==&vsc) return *this; // to avoid self assignment

    vsc_hvdc_model = NULL;
    clear();

    copy_from_const_vsc(vsc);

    return *this;
}

void VSC_HVDC::copy_from_const_vsc(const VSC_HVDC& vsc)
{
    set_toolkit(vsc.get_toolkit());

    set_name(vsc.get_name());
    set_identifier(vsc.get_identifier());
    set_status(vsc.get_status());
    set_converter_count(vsc.get_converter_count());
    set_dc_bus_count(vsc.get_dc_bus_count());
    set_dc_line_count(vsc.get_dc_line_count());
    set_dc_network_base_voltage_in_kV(vsc.get_dc_network_base_voltage_in_kV());

    unsigned int ncon=vsc.get_converter_count();
    unsigned int nbus=vsc.get_dc_bus_count();
    unsigned int ndc_line=vsc.get_dc_line_count();
    for(unsigned int i=0;i!=ncon;++i)
    {
        set_converter_ac_bus(i,vsc.get_converter_ac_bus(i));
        set_converter_active_power_operation_mode(i,vsc.get_converter_active_power_operation_mode(i));
        switch(vsc.get_converter_active_power_operation_mode(i))
        {
            case VSC_DC_VOLTAGE_CONTORL:
                set_converter_nominal_dc_voltage_command_in_kV(i,vsc.get_converter_nominal_dc_voltage_command_in_kV(i));
                break;
            case VSC_AC_ACTIVE_POWER_CONTORL:
                set_converter_nominal_ac_active_power_command_in_MW(i,vsc.get_converter_nominal_ac_active_power_command_in_MW(i));
                break;
            case VSC_DC_ACTIVE_POWER_VOLTAGE_DROOP_CONTROL:
                set_converter_initial_dc_active_power_reference_in_MW(i,vsc.get_converter_initial_dc_active_power_reference_in_MW(i));
                set_converter_initial_dc_voltage_reference_in_kV(i,vsc.get_converter_initial_dc_voltage_reference_in_kV(i));
                set_converter_initial_droop_coefficient_for_droop_control(i,vsc.get_converter_initial_droop_coefficient_for_droop_control(i));
                break;
            case VSC_DC_CURRENT_VOLTAGE_DROOP_CONTROL:
                set_converter_initial_dc_current_reference_in_kA(i,vsc.get_converter_initial_dc_current_reference_in_kA(i));
                set_converter_initial_dc_voltage_reference_in_kV(i,vsc.get_converter_initial_dc_voltage_reference_in_kV(i));
                set_converter_initial_droop_coefficient_for_droop_control(i,vsc.get_converter_initial_droop_coefficient_for_droop_control(i));
                break;
            case VSC_AC_VOLTAGE_ANGLE_CONTROL:
            default:
                break;
        }
        set_converter_reactive_power_operation_mode(i,vsc.get_converter_reactive_power_operation_mode(i));
        switch(vsc.get_converter_reactive_power_operation_mode(i))
        {
            case VSC_AC_REACTIVE_POWER_CONTROL:
                set_converter_nominal_ac_reactive_power_command_in_Mvar(i,vsc.get_converter_nominal_ac_reactive_power_command_in_Mvar(i));
                break;
            case VSC_AC_VOLTAGE_CONTROL:
                set_converter_nominal_ac_voltage_command_in_pu(i,vsc.get_converter_nominal_ac_voltage_command_in_pu(i));
                break;
            default:
                break;
        }
        set_converter_loss_factor_A_in_kW(i,vsc.get_converter_loss_factor_A_in_kW(i));
        set_converter_loss_factor_B_in_kW_per_amp(i,vsc.get_converter_loss_factor_B_in_kW_per_amp(i));
        set_converter_loss_factor_C_in_KW_per_amp_squared(i,vsc.get_converter_loss_factor_C_in_kW_per_amp_squard(i));
        set_converter_minimum_loss_in_kW(i,vsc.get_converter_minimum_loss_in_kW(i));
        set_converter_rated_capacity_in_MVA(i,vsc.get_converter_rated_capacity_in_MVA(i));
        set_converter_rated_current_in_A(i,vsc.get_converter_rated_current_in_A(i));

        set_converter_transformer_capacity_in_MVA(i, vsc.get_converter_transformer_capacity_in_MVA(i));
        set_converter_transformer_AC_side_base_voltage_in_kV(i, vsc.get_converter_transformer_AC_side_base_voltage_in_kV(i));
        set_converter_transformer_converter_side_base_voltage_in_kV(i, vsc.get_converter_transformer_converter_side_base_voltage_in_kV(i));
        set_converter_transformer_off_nominal_turn_ratio(i, vsc.get_converter_transformer_off_nominal_turn_ratio(i));
        set_converter_transformer_impedance_in_pu(i, vsc.get_converter_transformer_impedance_in_pu(i));
        set_converter_commutating_impedance_in_ohm(i, vsc.get_converter_commutating_impedance_in_ohm(i));
        set_converter_filter_admittance_in_siemens(i, vsc.get_converter_filter_admittance_in_siemens(i));

        set_converter_P_to_AC_bus_in_MW(i, vsc.get_converter_P_to_AC_bus_in_MW(i));
        set_converter_Q_to_AC_bus_in_MVar(i, vsc.get_converter_Q_to_AC_bus_in_MVar(i));

        set_converter_Pmax_in_MW(i, vsc.get_converter_Pmax_in_MW(i));
        set_converter_Pmin_in_MW(i, vsc.get_converter_Pmin_in_MW(i));
        set_converter_Qmax_in_MVar(i, vsc.get_converter_Qmax_in_MVar(i));
        set_converter_Qmin_in_MVar(i, vsc.get_converter_Qmin_in_MVar(i));
        set_converter_Udmax_in_kV(i, vsc.get_converter_Udmax_in_kV(i));
        set_converter_Udmin_in_kV(i, vsc.get_converter_Udmin_in_kV(i));
        set_converter_remote_bus_to_regulate(i,vsc.get_converter_remote_bus_to_regulate(i));
        set_converter_remote_regulation_percent(i,vsc.get_converter_remote_regulation_percent(i));
    }

    for(unsigned int i=0;i!=nbus;++i)
    {
        set_dc_bus_number(i,vsc.get_dc_bus_number(i));
        set_dc_bus_converter_index_with_ac_bus_number(i,vsc.get_converter_ac_bus_number_with_dc_bus_index(i));
        set_dc_bus_area(i,vsc.get_dc_bus_area(i));
        set_dc_bus_Vdc_in_kV(i,vsc.get_dc_bus_Vdc_in_kV(i));
        set_dc_bus_zone(i,vsc.get_dc_bus_zone(i));
        set_dc_bus_name(i,vsc.get_dc_bus_name(i));
        set_dc_bus_ground_resistance_in_ohm(i,vsc.get_dc_bus_ground_resistance_in_ohm(i));
        set_dc_bus_owner_number(i,vsc.get_dc_bus_owner_number(i));
        set_dc_bus_generation_power_in_MW(i,vsc.get_dc_bus_generation_power_in_MW(i));
        set_dc_bus_load_power_in_MW(i,vsc.get_dc_bus_load_power_in_MW(i));
    }

    for(unsigned int i=0;i!=ndc_line;++i)
    {
        set_dc_line_receiving_side_bus(i,vsc.get_dc_line_receiving_side_bus(i));
        set_dc_line_sending_side_bus(i,vsc.get_dc_line_sending_side_bus(i));
        set_dc_line_identifier(i,vsc.get_dc_line_identifier(i));
        set_dc_line_meter_end_bus(i,vsc.get_dc_line_meter_end_bus(i));
        set_dc_line_resistance_in_ohm(i,vsc.get_dc_line_resistance_in_ohm(i));
        set_dc_line_inductance_in_mH(i,vsc.get_dc_line_inductance_in_mH(i));
    }
    set_model(vsc.get_vsc_hvdc_model());
}

VSC_HVDC::~VSC_HVDC()
{
    delete_vsc_hvdc_model();
    clear();
}

void VSC_HVDC::clear()
{
    set_identifier("");
    set_name("");
    set_status(false);
    set_converter_count(0);
    set_dc_bus_count(0);
    set_dc_line_count(0);
    set_dc_network_base_voltage_in_kV(0);

    converters.clear();
    dc_buses.clear();
    dc_lines.clear();

    inphno.clear();
    dc_network_matrix.clear();
    jacobian.clear();
    current_active_power_control_mode.clear();
    current_active_power_control_mode.clear();

    iteration_count=0;
    set_convergence_flag(false);
}

void VSC_HVDC::set_identifier(const string id)
{
    string new_id = trim_string(id);
    add_string_to_str_int_map(new_id);
    identifier_index = get_index_of_string(new_id);
    vsc_hvdc_name_index = identifier_index;
}

void VSC_HVDC::set_name(const string name)
{
    string new_name = trim_string(name);
    add_string_to_str_int_map(name);
    vsc_hvdc_name_index = get_index_of_string(name);
    identifier_index = vsc_hvdc_name_index;
}

void VSC_HVDC::set_converter_count(unsigned int n)
{
    converters.clear();

    if(n<2) n = 2;

    converters.reserve(n);

    VSC_HVDC_CONVERTER_STRUCT converter;
    converter.converter_bus = 0;
    converter.converter_busptr = NULL;
    converter.status = true;
    converter.dc_control_mode = VSC_AC_ACTIVE_POWER_CONTORL;
    converter.ac_control_mode =  VSC_AC_REACTIVE_POWER_CONTROL;
    converter.nominal_ac_active_power_in_MW = 0;
    converter.nominal_dc_voltage_in_kV = 0;
    converter.initial_dc_voltage_reference_in_kV = 0;
    converter.initial_dc_active_power_reference_in_MW = 0;
    converter.droop_coefficient_for_droop_control = 0;
    converter.initial_dc_current_reference_in_kA = 0;
    converter.nominal_ac_voltage_in_pu = 1.0;
    converter.nominal_ac_reactive_power_in_Mvar = 0;
    converter.converter_loss_coefficient_A_in_kW = 0;
    converter.converter_loss_coefficient_B_in_kW_per_amp = 0;
    converter.converter_loss_coefficient_C_in_kW_per_amp_squard = 0;
    converter.min_converter_loss_in_kW = 0;
    converter.converter_rated_capacity_in_MVA = 100;
    converter.converter_rated_current_in_amp = 0;
    converter.converter_transformer_impedance_in_pu = 0;
    converter.converter_commutating_impedance_in_ohm = 0;
    converter.converter_filter_admittance_in_siemens = 0;
    converter.Pmax_MW = 100.0;
    converter.Pmin_MW = -100.0;
    converter.Qmax_MVar = 100.0;
    converter.Qmin_MVar = -100.0;
    converter.Udmax_kV = 9999.0;
    converter.Udmin_kV = 0;
    converter.remote_bus_to_regulate = 0;
    converter.remote_regulation_percent = 0;

    converter.P_to_AC_bus_MW = 0.0;
    converter.Q_to_AC_bus_MVar = 0.0;
    for(unsigned int i=0; i<n; ++i)
        converters.push_back(converter);
}

void VSC_HVDC::set_dc_bus_count(unsigned int n)
{
    dc_buses.clear();

    if(n<2) n = 2;

    dc_buses.reserve(n);
    VSC_HVDC_DC_BUS_STRUCT dc_bus;
    dc_bus.converter_index = INDEX_NOT_EXIST;
    dc_bus.Vdc_kV = 200.0;
    dc_bus.dc_bus_area = 1;
    dc_bus.dc_bus_zone = 1;
    dc_bus.converter_ground_resistance_in_ohm = 0.0;
    dc_bus.owner = 1;
    dc_bus.dc_generation_power_in_MW = 0.0;
    dc_bus.dc_load_power_in_MW = 0.0;
    for(unsigned int i=0; i<n; ++i)
        dc_buses.push_back(dc_bus);
}

void VSC_HVDC::set_dc_line_count(unsigned int n)
{
    dc_lines.clear();

    if(n<1) n = 1;

    dc_lines.reserve(n);
    VSC_HVDC_DC_LINE_STRUCT dc_line;
    dc_line.sending_side_bus = 1;
    dc_line.receiving_side_bus = 1;
    dc_line.line_R_in_ohm = 1;
    dc_line.line_L_in_mH = 0;
    for(unsigned int i=0; i<n; ++i)
        dc_lines.push_back(dc_line);
}

void VSC_HVDC::set_status(const bool status)
{
    this->status = status;
}

void VSC_HVDC::set_dc_network_base_voltage_in_kV(const double base_voltage)
{
    this->dc_base_voltage_in_kV = base_voltage;
}

bool VSC_HVDC::converter_index_is_out_of_range_in_function(const unsigned int index, const string& func) const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    if(index>=get_converter_count())
    {
        osstream<<"VSC_HVDC converter index "<<index<<" exceeds converter count "<<get_converter_count()
                <<" when calling VSC_HVDC::"<<func<<"()"<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);

        return true;
    }
    else
        return false;

}
void VSC_HVDC::set_converter_ac_bus(const unsigned int index, const unsigned int bus)
{
    if(converter_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;

    if(bus!=0)
    {
        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

        if(psdb.is_bus_exist(bus))
        {
            converters[index].converter_bus = bus;
            converters[index].converter_busptr = psdb.get_bus(bus);
        }
        else
        {
            ostringstream osstream;
            STEPS& toolkit = get_toolkit();
            osstream<<"Bus "<<bus<<" does not exist for setting up converter AC side bus of vsc-hvdc link."<<endl
                    <<"0 will be set to indicate invalid vsc-hvdc link.";
            toolkit.show_information_with_leading_time_stamp(osstream);
            converters[index].converter_bus = 0;
            converters[index].converter_busptr = NULL;
        }
    }
    else
    {
        ostringstream osstream;
        STEPS& toolkit = get_toolkit();
        osstream<<"Warning. Zero bus number (0) is not allowed for setting up converter AC bus of vsc-hvdc link."<<endl
                <<"0 will be set to indicate invalid vsc-hvdc link.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        converters[index].converter_bus = 0;
        converters[index].converter_busptr = NULL;
    }
}

void VSC_HVDC::set_converter_status(const unsigned int index, bool status)
{
    if(converter_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    else
        converters[index].status = status;
}

void VSC_HVDC::set_converter_active_power_operation_mode(const unsigned int index, const VSC_HVDC_CONVERTER_ACTIVE_POWER_CONTROL_MODE mode)
{
    if(converter_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    converters[index].dc_control_mode =  mode;
}

void VSC_HVDC::set_converter_dc_voltage_control_priority(const unsigned int index, const unsigned int p)
{
    if(converter_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    converters[index].dc_voltage_control_priority = p;
}


void VSC_HVDC::set_converter_reactive_power_operation_mode(const unsigned int index, const VSC_HVDC_CONVERTER_REACTIVE_POWER_CONTROL_MODE mode)
{
    if(converter_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    converters[index].ac_control_mode = mode;
}

void VSC_HVDC::set_converter_nominal_ac_active_power_command_in_MW(const unsigned int index, const double P)
{
    if(converter_index_is_out_of_range_in_function(index, __FUNCTION__))
    return;
    converters[index].nominal_ac_active_power_in_MW = P;
}

void VSC_HVDC::set_converter_nominal_dc_voltage_command_in_kV(const unsigned int index, const double V)
{
    if(converter_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    converters[index].nominal_dc_voltage_in_kV = V;
}

void VSC_HVDC::set_converter_initial_dc_voltage_reference_in_kV(const unsigned int index, const double V)
{
    if(converter_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    converters[index].initial_dc_voltage_reference_in_kV = V;
}
void VSC_HVDC::set_converter_initial_dc_active_power_reference_in_MW(const unsigned int index, const double P)
{
    if(converter_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    converters[index].initial_dc_active_power_reference_in_MW = P;
}

void VSC_HVDC::set_converter_initial_droop_coefficient_for_droop_control(const unsigned int index, const double droop)
{
    if(converter_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    converters[index].droop_coefficient_for_droop_control = droop;
}
void VSC_HVDC::set_converter_initial_dc_current_reference_in_kA(const unsigned int index, const double I)
{
    if(converter_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    converters[index].initial_dc_current_reference_in_kA = I;
}

void VSC_HVDC::set_converter_nominal_ac_voltage_command_in_pu(const unsigned int index, const double V)
{
    if(converter_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    converters[index].nominal_ac_voltage_in_pu = V;
}

void VSC_HVDC::set_converter_nominal_ac_reactive_power_command_in_Mvar(const unsigned int index, const double Q)
{
    if(converter_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    converters[index].nominal_ac_reactive_power_in_Mvar = Q;
}

void VSC_HVDC::set_converter_loss_factor_A_in_kW(const unsigned int index, const double A)
{
    if(converter_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    converters[index].converter_loss_coefficient_A_in_kW = A;
}

void VSC_HVDC::set_converter_loss_factor_B_in_kW_per_amp(const unsigned int index, const double B)
{
    if(converter_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    converters[index].converter_loss_coefficient_B_in_kW_per_amp = B;
}

void VSC_HVDC::set_converter_loss_factor_C_in_KW_per_amp_squared(const unsigned int index, const double C)
{
    if(converter_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    converters[index].converter_loss_coefficient_C_in_kW_per_amp_squard = C;
}

void VSC_HVDC::set_converter_minimum_loss_in_kW(const unsigned int index, const double P)
{
    if(converter_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    converters[index].min_converter_loss_in_kW = P;
}

void VSC_HVDC::set_converter_rated_capacity_in_MVA(const unsigned int index, const double S)
{
    if(converter_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    converters[index].converter_rated_capacity_in_MVA = S;
}

void VSC_HVDC::set_converter_rated_current_in_A(const unsigned int index, const double I)
{
    if(converter_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    converters[index].converter_rated_current_in_amp = I;
}

void VSC_HVDC::set_converter_transformer_capacity_in_MVA(const unsigned int index, const double S)
{
    if(converter_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    converters[index].converter_transformer_capacity_in_MVA = S;
}

void VSC_HVDC::set_converter_transformer_AC_side_base_voltage_in_kV(const unsigned int index, const double Vac)
{
    if(converter_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    converters[index].converter_transformer_AC_side_base_voltage_in_kV = Vac;
}

void VSC_HVDC::set_converter_transformer_converter_side_base_voltage_in_kV(const unsigned int index, const double Vac)
{
    if(converter_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    converters[index].converter_transformer_converter_side_base_voltage_in_kV = Vac;
}

void VSC_HVDC::set_converter_transformer_off_nominal_turn_ratio(const unsigned int index, const double turn_ratio)
{
    if(converter_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    converters[index].converter_transformer_off_nominal_turn_ratio = turn_ratio;
}

void VSC_HVDC::set_converter_transformer_impedance_in_pu(unsigned int index, const complex<double> z)
{
    if(converter_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    converters[index].converter_transformer_impedance_in_pu = z;
}

void VSC_HVDC::set_converter_commutating_impedance_in_ohm(unsigned int index, const complex<double> z)
{
    if(converter_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    converters[index].converter_commutating_impedance_in_ohm = z;
}

void VSC_HVDC::set_converter_filter_admittance_in_siemens(unsigned int index, const complex<double> y)
{
    if(converter_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    converters[index].converter_filter_admittance_in_siemens = y;
}

void VSC_HVDC::set_converter_P_to_AC_bus_in_MW(unsigned int index, double P)
{
    if(not converter_index_is_out_of_range_in_function(index, __FUNCTION__))
        converters[index].P_to_AC_bus_MW = P;
}

void VSC_HVDC::set_converter_Q_to_AC_bus_in_MVar(unsigned int index, double Q)
{
    if(not converter_index_is_out_of_range_in_function(index, __FUNCTION__))
        converters[index].Q_to_AC_bus_MVar = Q;
}

void VSC_HVDC::set_converter_Pmax_in_MW(const unsigned int index, const double P)
{
    if(converter_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    converters[index].Pmax_MW = P;
}

void VSC_HVDC::set_converter_Pmin_in_MW(const unsigned int index, const double P)
{
    if(converter_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    converters[index].Pmin_MW = P;
}

void VSC_HVDC::set_converter_Qmax_in_MVar(const unsigned int index, const double Q)
{
    if(converter_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    converters[index].Qmax_MVar = Q;
}

void VSC_HVDC::set_converter_Qmin_in_MVar(const unsigned int index, const double Q)
{
    if(converter_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    converters[index].Qmin_MVar = Q;
}

void VSC_HVDC::set_converter_Udmax_in_kV(const unsigned int index, const double Udmax)
{
    if(converter_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    converters[index].Udmax_kV = Udmax;
}

void VSC_HVDC::set_converter_Udmin_in_kV(const unsigned int index, const double Udmin)
{
    if(converter_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    converters[index].Udmin_kV = Udmin;
}

void VSC_HVDC::set_converter_remote_bus_to_regulate(const unsigned int index, const unsigned int bus)
{
    if(converter_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    converters[index].remote_bus_to_regulate = bus;
}

void VSC_HVDC::set_converter_remote_regulation_percent(const unsigned int index, const double rmpct)
{
    if(converter_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    converters[index].remote_regulation_percent = rmpct;
}

bool VSC_HVDC::dc_bus_index_is_out_of_range_in_function(const unsigned int index, const string& func) const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    if(index>=get_dc_bus_count())
    {
        osstream<<"VSC_HVDC dc bus index "<<index<<" exceeds dc bus count"<<get_dc_bus_count()
                <<" when calling VSC_HVDC::"<<func<<"()"<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);

        return true;
    }
    else
        return false;

}

void VSC_HVDC::set_dc_bus_number(const unsigned int index, const unsigned int n)
{
    if(dc_bus_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    dc_buses[index].dc_bus_number = n;
}

void VSC_HVDC::set_dc_bus_area(const unsigned int index, const unsigned int n)
{
    if(dc_bus_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    dc_buses[index].dc_bus_area = n;
}

void VSC_HVDC::set_dc_bus_zone(const unsigned int index, const unsigned int n)
{
    if(dc_bus_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    dc_buses[index].dc_bus_zone = n;
}

void VSC_HVDC::set_dc_bus_name(const unsigned int index, const string name)
{
    if(dc_bus_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    string tmp_name = name;
    tmp_name = trim_string(tmp_name);
    add_string_to_str_int_map(tmp_name);
    dc_buses[index].dc_bus_name_index = get_index_of_string(tmp_name);
}

void VSC_HVDC::set_dc_bus_converter_index_with_ac_bus_number(const unsigned int index, const unsigned int bus)
{
    if(dc_bus_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    //dc_buses[index].ac_bus_number = bus;
    unsigned int n_converter = get_converter_count();
    for(unsigned int i=0; i!=n_converter; ++i)
    {
        if(get_converter_ac_bus(i)==bus)
        {
            dc_buses[index].converter_index = i;
            return;
        }
    }
}

void VSC_HVDC::set_dc_bus_owner_number(const unsigned int index, const unsigned int n)
{
    if(dc_bus_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    dc_buses[index].owner = n;
}

void VSC_HVDC::set_dc_bus_ground_resistance_in_ohm(const unsigned int index, const double R)
{
    if(dc_bus_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    dc_buses[index].converter_ground_resistance_in_ohm = R;
}

void VSC_HVDC::set_dc_bus_generation_power_in_MW(const unsigned int index, const double P)
{
    if(dc_bus_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    dc_buses[index].dc_generation_power_in_MW=P;
}

void VSC_HVDC::set_dc_bus_load_power_in_MW(const unsigned int index, const double P)
{
    if(dc_bus_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    dc_buses[index].dc_load_power_in_MW=P;

}

void VSC_HVDC::set_dc_bus_Vdc_in_kV(const unsigned int index, const double Udc)
{
    if(dc_bus_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    dc_buses[index].Vdc_kV=Udc;

}

bool VSC_HVDC::dc_line_index_is_out_of_range_in_function(const unsigned int index, const string& func) const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    if(index>=get_dc_line_count())
    {
        osstream<<"VSC_HVDC dc line index "<<index<<" exceeds dc line count"<<get_dc_line_count()
                <<" when calling VSC_HVDC::"<<func<<"()"<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);

        return true;
    }
    else
        return false;

}

void VSC_HVDC::set_dc_line_sending_side_bus(const unsigned int index, const unsigned int bus)
{
    if(dc_line_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    dc_lines[index].sending_side_bus = bus;
}

void VSC_HVDC::set_dc_line_receiving_side_bus(const unsigned int index, const unsigned int bus)
{
    if(dc_line_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    dc_lines[index].receiving_side_bus = bus;
}

void VSC_HVDC::set_dc_line_identifier(const unsigned int index, const string line_id)
{
    if(dc_line_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    string tmp_id = line_id;
    tmp_id = trim_string(tmp_id);
    add_string_to_str_int_map(tmp_id);
    dc_lines[index].identifier_index = get_index_of_string(tmp_id);
}

void VSC_HVDC::set_dc_line_meter_end_bus(const unsigned int index, const unsigned int meter_bus)
{
    if(dc_line_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    if(meter_bus<=1) dc_lines[index].meter_end_bus = dc_lines[index].sending_side_bus;
    else             dc_lines[index].meter_end_bus = dc_lines[index].receiving_side_bus;
}

void VSC_HVDC::set_dc_line_resistance_in_ohm(const unsigned int index, const double R)
{
    if(dc_line_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    if(R>=0.0)

        dc_lines[index].line_R_in_ohm = R;

    else
    {
        ostringstream osstream;
        osstream<<"Error. Non-positive ("<<R<<" ohm) is not allowed for setting VSC_HVDC line resistance of "<<"get_compound_device_name()"<<endl
          <<"0.0 will be set to indicate invalid VSC_HVDC link.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        dc_lines[index].line_R_in_ohm = 0.0;
    }
}

void VSC_HVDC::set_dc_line_inductance_in_mH(const unsigned int index,const double L)
{
    if(dc_line_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    ostringstream osstream;
    if(L>=0.0)
        dc_lines[index].line_L_in_mH = L;
    else
    {

        osstream<<"Error. Non-positive ("<<L<<" mh) is not allowed for setting VSC_HVDC line resistance of "<<get_compound_device_name()<<endl
          <<"0.0 will be set to indicate invalid VSC_HVDC link.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        dc_lines[index].line_L_in_mH = 0.0;
    }
}


string VSC_HVDC::get_identifier() const
{
    return get_string_of_index(identifier_index);
}

unsigned int VSC_HVDC::get_identifier_index() const
{
    return identifier_index;
}

string VSC_HVDC::get_name() const
{
    return get_string_of_index(vsc_hvdc_name_index);
}

unsigned int VSC_HVDC::get_name_index() const
{
    return vsc_hvdc_name_index;
}

unsigned int VSC_HVDC::get_converter_count() const
{
    return converters.size();
}

unsigned int VSC_HVDC::get_dc_bus_count() const
{
    return dc_buses.size();
}

unsigned int VSC_HVDC::get_dc_line_count() const
{
    return dc_lines.size();
}

bool VSC_HVDC::get_status() const
{
    return status;
}

double VSC_HVDC::get_dc_network_base_voltage_in_kV() const
{
    return dc_base_voltage_in_kV;
}

VSC_HVDC_CONVERTER_STRUCT* VSC_HVDC::get_converter(unsigned int index)
{
    if(index<get_converter_count())
        return &(converters[index]);
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count when calling VSC_HVDC::"<<__FUNCTION__<<"()."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return NULL;
    }
}

unsigned int VSC_HVDC::get_converter_ac_bus(unsigned int index) const
{
    if(index<get_converter_count())
        return converters[index].converter_bus;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count when calling VSC_HVDC::"<<__FUNCTION__<<"()."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 0;
    }
}

BUS* VSC_HVDC::get_converter_ac_bus_pointer(unsigned int index) const
{
    if(index<get_converter_count())
        return converters[index].converter_busptr;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count when calling VSC_HVDC::"<<__FUNCTION__<<"()."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return NULL;
    }
}

unsigned int VSC_HVDC::get_converter_index_with_ac_bus(unsigned int bus) const
{
    if(is_connected_to_bus(bus))
    {
        unsigned int n = get_converter_count();
        for(unsigned int index=0; index!=n; ++index)
        {
            if(get_converter_ac_bus(index)==bus)
                return index;
        }
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. AC bus ("<<bus<<") is not valid converter AC bus for "<<get_compound_device_name()
                <<" when calling VSC_HVDC::"<<__FUNCTION__<<"()."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return INDEX_NOT_EXIST;
    }
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. "<<get_compound_device_name()<<" is not connected to AC bus ("<<bus
                <<" when calling VSC_HVDC::"<<__FUNCTION__<<"()."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return INDEX_NOT_EXIST;
    }
}

bool VSC_HVDC::get_converter_status(unsigned int index) const
{
    if(index<get_converter_count())
        return converters[index].status;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count when calling VSC_HVDC::"<<__FUNCTION__<<"()."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return false;
    }
}

VSC_HVDC_CONVERTER_ACTIVE_POWER_CONTROL_MODE VSC_HVDC::get_converter_active_power_operation_mode(unsigned int index) const
{
    if(index<get_converter_count())
        return converters[index].dc_control_mode;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count when calling VSC_HVDC::"<<__FUNCTION__<<"()."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return VSC_INVALID_DC_CONTORL;
    }
}

unsigned int VSC_HVDC::get_converter_dc_voltage_control_priority(const unsigned int index) const
{
    if(index<get_converter_count())
        return converters[index].dc_voltage_control_priority;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count when calling VSC_HVDC::"<<__FUNCTION__<<"()."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 0;
    }

}

VSC_HVDC_CONVERTER_REACTIVE_POWER_CONTROL_MODE VSC_HVDC::get_converter_reactive_power_operation_mode(unsigned int index) const
{
    if(index<get_converter_count())
        return converters[index].ac_control_mode;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count when calling VSC_HVDC::"<<__FUNCTION__<<"()."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return VSC_INVALID_AC_CONTORL;
    }
}

double VSC_HVDC::get_converter_nominal_ac_active_power_command_in_MW(unsigned int index) const
{
    if(index<get_converter_count())
        return converters[index].nominal_ac_active_power_in_MW;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count when calling VSC_HVDC::"<<__FUNCTION__<<"()."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

double VSC_HVDC::get_converter_nominal_ac_active_power_command_in_pu(unsigned int index) const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();
    if(index<get_converter_count())
        return converters[index].nominal_ac_active_power_in_MW/sbase;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count when calling VSC_HVDC::"<<__FUNCTION__<<"()."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}


double VSC_HVDC::get_converter_nominal_dc_voltage_command_in_kV(unsigned int index) const
{
    if(index<get_converter_count())
        return converters[index].nominal_dc_voltage_in_kV;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count when calling VSC_HVDC::"<<__FUNCTION__<<"()."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

double VSC_HVDC::get_converter_nominal_dc_voltage_command_in_pu(unsigned int index) const
{
    double vbase = get_dc_network_base_voltage_in_kV();
    if(index<get_converter_count())
        return converters[index].nominal_dc_voltage_in_kV/vbase;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count when calling VSC_HVDC::"<<__FUNCTION__<<"()."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

double VSC_HVDC::get_converter_nominal_ac_voltage_command_in_kV(unsigned int index) const
{
    double vbase = get_dc_network_base_voltage_in_kV();
    if(index<get_converter_count())
        return converters[index].nominal_ac_voltage_in_pu/vbase;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count when calling VSC_HVDC::"<<__FUNCTION__<<"()."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

double VSC_HVDC::get_converter_nominal_ac_voltage_command_in_pu(unsigned int index) const
{
    if(index<get_converter_count())
        return converters[index].nominal_ac_voltage_in_pu;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count when calling VSC_HVDC::"<<__FUNCTION__<<"()."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

double VSC_HVDC::get_converter_initial_dc_voltage_reference_in_kV(const unsigned int index)const
{
    if(index<get_converter_count())
        return converters[index].initial_dc_voltage_reference_in_kV;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count when calling VSC_HVDC::"<<__FUNCTION__<<"()."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

double VSC_HVDC::get_converter_initial_dc_active_power_reference_in_MW(const unsigned int index)const
{
    if(index<get_converter_count())
        return converters[index].initial_dc_active_power_reference_in_MW;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count when calling VSC_HVDC::"<<__FUNCTION__<<"()."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

double VSC_HVDC::get_converter_initial_dc_current_reference_in_kA(const unsigned int index)const
{
    if(index<get_converter_count())
        return converters[index].initial_dc_current_reference_in_kA;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count when calling VSC_HVDC::"<<__FUNCTION__<<"()."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

double VSC_HVDC::get_converter_initial_droop_coefficient_for_droop_control(const unsigned int index)const
{
    if(index<get_converter_count())
        return converters[index].droop_coefficient_for_droop_control;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count when calling VSC_HVDC::"<<__FUNCTION__<<"()."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

double VSC_HVDC::get_converter_nominal_ac_reactive_power_command_in_Mvar(unsigned int index) const
{
    if(index<get_converter_count())
        return converters[index].nominal_ac_reactive_power_in_Mvar;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count when calling VSC_HVDC::"<<__FUNCTION__<<"()."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

double VSC_HVDC::get_converter_nominal_reactive_power_command_in_pu(unsigned int index) const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();
    if(index<get_converter_count())
        return converters[index].nominal_ac_reactive_power_in_Mvar/sbase;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count when calling VSC_HVDC::"<<__FUNCTION__<<"()."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

double VSC_HVDC::get_converter_loss_factor_A_in_kW(unsigned int index) const
{
    if(index<get_converter_count())
        return converters[index].converter_loss_coefficient_A_in_kW;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count when calling VSC_HVDC::"<<__FUNCTION__<<"()."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

double VSC_HVDC::get_converter_loss_factor_B_in_kW_per_amp(unsigned int index) const
{
    if(index<get_converter_count())
        return converters[index].converter_loss_coefficient_B_in_kW_per_amp;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count when calling VSC_HVDC::"<<__FUNCTION__<<"()."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

double VSC_HVDC::get_converter_loss_factor_C_in_kW_per_amp_squard(unsigned int index) const
{
    if(index<get_converter_count())
        return converters[index].converter_loss_coefficient_C_in_kW_per_amp_squard;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count when calling VSC_HVDC::"<<__FUNCTION__<<"()."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

double VSC_HVDC::get_converter_minimum_loss_in_kW(unsigned int index) const
{
    if(index<get_converter_count())
        return converters[index].min_converter_loss_in_kW;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count when calling VSC_HVDC::"<<__FUNCTION__<<"()."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

double VSC_HVDC::get_converter_rated_capacity_in_MVA(unsigned int index) const
{
    if(index<get_converter_count())
        return converters[index].converter_rated_capacity_in_MVA;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count when calling VSC_HVDC::"<<__FUNCTION__<<"()."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

double VSC_HVDC::get_converter_rated_current_in_A(unsigned int index) const
{
    if(index<get_converter_count())
        return converters[index].converter_rated_current_in_amp;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count when calling VSC_HVDC::"<<__FUNCTION__<<"()."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

double VSC_HVDC::get_converter_transformer_capacity_in_MVA(unsigned int index) const
{
    if(index<get_converter_count())
        return converters[index].converter_transformer_capacity_in_MVA;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count when calling VSC_HVDC::"<<__FUNCTION__<<"()."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

double VSC_HVDC::get_converter_transformer_AC_side_base_voltage_in_kV(unsigned int index) const
{
    if(index<get_converter_count())
        return converters[index].converter_transformer_AC_side_base_voltage_in_kV;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count when calling VSC_HVDC::"<<__FUNCTION__<<"()."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

double VSC_HVDC::get_converter_transformer_converter_side_base_voltage_in_kV(unsigned int index) const
{
    if(index<get_converter_count())
        return converters[index].converter_transformer_converter_side_base_voltage_in_kV;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count when calling VSC_HVDC::"<<__FUNCTION__<<"()."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

double VSC_HVDC::get_converter_transformer_off_nominal_turn_ratio(unsigned int index) const
{
    if(index<get_converter_count())
        return converters[index].converter_transformer_off_nominal_turn_ratio;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count when calling VSC_HVDC::"<<__FUNCTION__<<"()."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

complex<double>VSC_HVDC::get_converter_transformer_impedance_in_pu(unsigned int index) const
{
    if(index<get_converter_count())
        return converters[index].converter_transformer_impedance_in_pu;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count when calling VSC_HVDC::"<<__FUNCTION__<<"()."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        complex<double> j(0.0,1.0);
        return 9999.9+j*9999.9;
    }
}

complex<double>VSC_HVDC::get_converter_commutating_impedance_in_ohm(unsigned int index) const
{
    if(index<get_converter_count())
        return converters[index].converter_commutating_impedance_in_ohm;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count when calling VSC_HVDC::"<<__FUNCTION__<<"()."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        complex<double> j(0.0,1.0);
        return 9999.9+j*9999.9;
    }
}

complex<double>VSC_HVDC::get_converter_filter_admittance_in_siemens(unsigned int index) const
{
    if(index<get_converter_count())
        return converters[index].converter_filter_admittance_in_siemens;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count when calling VSC_HVDC::"<<__FUNCTION__<<"()."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        complex<double> j(0.0,1.0);
        return 9999.9+j*9999.9;
    }
}

double VSC_HVDC::get_converter_P_to_AC_bus_in_MW(unsigned int index) const
{
    if(not converter_index_is_out_of_range_in_function(index, __FUNCTION__))
        return converters[index].P_to_AC_bus_MW;
    else
        return 0.0;
}

double VSC_HVDC::get_converter_Q_to_AC_bus_in_MVar(unsigned int index) const
{
    if(not converter_index_is_out_of_range_in_function(index, __FUNCTION__))
        return converters[index].Q_to_AC_bus_MVar;
    else
        return 0.0;
}

double VSC_HVDC::get_converter_Pmax_in_MW(const unsigned int index) const
{
    if(index<get_converter_count())
        return converters[index].Pmax_MW;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count when calling VSC_HVDC::"<<__FUNCTION__<<"()."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

double VSC_HVDC::get_converter_Pmin_in_MW(const unsigned int index) const
{
    if(index<get_converter_count())
        return converters[index].Pmin_MW;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count when calling VSC_HVDC::"<<__FUNCTION__<<"()."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return -9999.9;
    }
}


double VSC_HVDC::get_converter_Qmax_in_MVar(unsigned int index) const
{
    if(index<get_converter_count())
        return converters[index].Qmax_MVar;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count when calling VSC_HVDC::"<<__FUNCTION__<<"()."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

double VSC_HVDC::get_converter_Qmin_in_MVar(unsigned int index) const
{
    if(index<get_converter_count())
        return converters[index].Qmin_MVar;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count when calling VSC_HVDC::"<<__FUNCTION__<<"()."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return -9999.9;
    }
}

double VSC_HVDC::get_converter_Udmax_in_kV(const unsigned int index) const
{
    if(index<get_converter_count())
        return converters[index].Udmax_kV;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count when calling VSC_HVDC::"<<__FUNCTION__<<"()."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}
double VSC_HVDC::get_converter_Udmin_in_kV(const unsigned int index) const
{
    if(index<get_converter_count())
        return converters[index].Udmin_kV;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count when calling VSC_HVDC::"<<__FUNCTION__<<"()."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 0;
    }
}

unsigned int VSC_HVDC::get_converter_remote_bus_to_regulate(unsigned int index) const
{
    if(index<get_converter_count())
        return converters[index].remote_bus_to_regulate;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count when calling VSC_HVDC::"<<__FUNCTION__<<"()."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999;
    }
}

double VSC_HVDC::get_converter_remote_regulation_percent(unsigned int index) const
{
    if(index<get_converter_count())
        return converters[index].remote_regulation_percent;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count when calling VSC_HVDC::"<<__FUNCTION__<<"()."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

unsigned int VSC_HVDC::get_dc_bus_number(unsigned int index) const
{
    if(index<get_dc_bus_count())
        return dc_buses[index].dc_bus_number;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal dc bus count when calling "<<__FUNCTION__<<"()"<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return INDEX_NOT_EXIST;
    }
}

unsigned int VSC_HVDC::get_converter_ac_bus_number_with_dc_bus_index(unsigned int index) const
{
    unsigned int converter_index = get_dc_bus_converter_index_with_dc_bus_index(index);
    if(converter_index!=INDEX_NOT_EXIST)
        return get_converter_ac_bus(converter_index);
    else
        return 0;

}

unsigned int VSC_HVDC::dc_bus_no2index(unsigned int bus) const
{
    unsigned int nbus = get_dc_bus_count();
    for(unsigned int index=0; index!=nbus; ++index)
    {
        if(dc_buses[index].dc_bus_number==bus)
            return index;
    }
    return INDEX_NOT_EXIST;
}

unsigned int VSC_HVDC::dc_bus_index2no(unsigned int index) const
{
    if(index<get_dc_bus_count())
        return dc_buses[index].dc_bus_number;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal dc bus count when calling "<<__FUNCTION__<<"()"<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return INDEX_NOT_EXIST;
    }
}


unsigned int VSC_HVDC::dc_line_device_id2index(DC_DEVICE_ID dc_did) const
{
    unsigned int n_dc_line = dc_lines.size();
    for(unsigned int i=0; i!=n_dc_line; ++i)
    {
        TERMINAL terminal;
        terminal.append_bus(get_dc_line_sending_side_bus(i));
        terminal.append_bus(get_dc_line_receiving_side_bus(i));

        DC_DEVICE_ID did;
        did.set_device_type(STEPS_DC_LINE);
        did.set_device_terminal(terminal);
        did.set_device_identifier_index(get_index_of_string(get_dc_line_identifier(i)));

        if(did==dc_did) return i;
    }
    return INDEX_NOT_EXIST;
}

DC_DEVICE_ID VSC_HVDC::dc_line_index2device_id(unsigned int index) const
{
    DC_DEVICE_ID did;
    unsigned int n_dc_line = dc_lines.size();
    if(index<n_dc_line)
    {
        TERMINAL terminal;
        terminal.append_bus(get_dc_line_sending_side_bus(index));
        terminal.append_bus(get_dc_line_receiving_side_bus(index));

        did.set_device_type(STEPS_DC_LINE);
        did.set_device_terminal(terminal);
        did.set_device_identifier_index(get_index_of_string(get_dc_line_identifier(index)));
    }
    return did;
}

unsigned int VSC_HVDC::get_dc_bus_converter_index_with_dc_bus_index(unsigned int index) const
{
    if(index<get_dc_bus_count())
        return dc_buses[index].converter_index;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal dc bus count when calling "<<__FUNCTION__<<"()"<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return INDEX_NOT_EXIST;
    }
}

unsigned int VSC_HVDC::get_dc_bus_converter_index_with_dc_bus_number(unsigned int bus) const
{
    return get_dc_bus_converter_index_with_dc_bus_index(dc_bus_no2index(bus));
}

unsigned int VSC_HVDC::get_dc_bus_index_with_converter_index(unsigned int converter_index) const
{
    unsigned int nbus = get_dc_bus_count();
    unsigned int dc_bus_index=0;
    for(unsigned int index=0;index!=nbus;++index)
    {
        if(dc_buses[index].converter_index==converter_index)
            dc_bus_index=index;
    }
    return dc_bus_index;
}

unsigned int VSC_HVDC::get_dc_bus_area(unsigned int index) const
{
    if(index<get_dc_bus_count())
        return dc_buses[index].dc_bus_area;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal dc bus count when calling "<<__FUNCTION__<<"()"<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return INDEX_NOT_EXIST;
    }
}

unsigned int VSC_HVDC::get_dc_bus_zone(unsigned int index) const
{
    if(index<get_dc_bus_count())
        return dc_buses[index].dc_bus_zone;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal dc bus count when calling "<<__FUNCTION__<<"()"<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return INDEX_NOT_EXIST;
    }
}

string VSC_HVDC::get_dc_bus_name(unsigned int index) const
{
    if(index<get_dc_bus_count())
        return get_string_of_index(dc_buses[index].dc_bus_name_index);
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal dc bus count when calling "<<__FUNCTION__<<"()"<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return "9999.9";
    }
}

double VSC_HVDC::get_dc_bus_ground_resistance_in_ohm(unsigned int index) const
{
    if(index<get_dc_bus_count())
        return dc_buses[index].converter_ground_resistance_in_ohm;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal dc bus count when calling "<<__FUNCTION__<<"()"<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

unsigned int VSC_HVDC::get_dc_bus_owner_number(unsigned int index) const
{
    if(index<get_dc_bus_count())
        return dc_buses[index].owner;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal dc bus count when calling "<<__FUNCTION__<<"()"<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999;
    }
}

double VSC_HVDC::get_dc_bus_generation_power_in_MW(unsigned int index) const
{
    if(index<get_dc_bus_count())
    {
        return dc_buses[index].dc_generation_power_in_MW;
        cout<<"dc_buses[index].dc_generation_power_in_MW:"<<dc_buses[index].dc_generation_power_in_MW<<endl;
    }
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal dc bus count when calling "<<__FUNCTION__<<"()"<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999;
    }
}

double VSC_HVDC::get_dc_bus_load_power_in_MW(unsigned int index) const
{
    if(index<get_dc_bus_count())
        return dc_buses[index].dc_load_power_in_MW;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal dc bus count when calling "<<__FUNCTION__<<"()"<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999;
    }
}

double VSC_HVDC::get_dc_bus_Vdc_in_kV(unsigned int index) const
{
    if(index<get_dc_bus_count())
        return dc_buses[index].Vdc_kV;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal dc bus count when calling "<<__FUNCTION__<<"()"<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999;
    }
}

double VSC_HVDC::get_dc_bus_Vdc_in_kV_with_dc_bus_number(unsigned int bus) const
{
    //cout<<"get_dc_bus_Vdc_in_kV_with_dc_bus_number: bus:"<<bus<<endl;
    unsigned int index = dc_bus_no2index(bus);
    //cout<<"get_dc_bus_Vdc_in_kV_with_dc_bus_number: index:"<<index<<endl;
    return get_dc_bus_Vdc_in_kV(index);
}

unsigned int VSC_HVDC::get_dc_line_sending_side_bus(unsigned int index) const
{
    if(index<get_dc_line_count())
        return dc_lines[index].sending_side_bus;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal dc line count."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999;
    }
}

unsigned int VSC_HVDC::get_dc_line_receiving_side_bus(unsigned int index) const
{
    if(index<get_dc_line_count())
        return dc_lines[index].receiving_side_bus;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal dc line count."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999;
    }
}

string VSC_HVDC::get_dc_line_identifier(unsigned int index) const
{
    if(index<get_dc_line_count())
        return get_string_of_index(dc_lines[index].identifier_index);
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal dc line count."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return "";
    }
}

DC_DEVICE_ID VSC_HVDC::get_dc_line_device_id(unsigned int index) const
{
    DC_DEVICE_ID did;
    did.set_device_type(STEPS_DC_LINE);

    TERMINAL terminal;
    terminal.append_bus(get_dc_line_sending_side_bus(index));
    terminal.append_bus(get_dc_line_receiving_side_bus(index));

    did.set_device_terminal(terminal);

    did.set_device_identifier_index(get_index_of_string(get_dc_line_identifier(index)));

    return did;
}

unsigned int VSC_HVDC::get_dc_line_meter_end_bus(unsigned int index) const
{
    if(index<get_dc_line_count())
        return dc_lines[index].meter_end_bus;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal dc line count."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999;
    }
}

double VSC_HVDC::get_dc_line_resistance_in_ohm(unsigned int index) const
{
    if(index<get_dc_line_count())
        return dc_lines[index].line_R_in_ohm;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal dc line count."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

double VSC_HVDC::get_dc_line_inductance_in_mH(unsigned int index) const
{
    if(index<get_dc_line_count())
        return dc_lines[index].line_L_in_mH;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal dc line count when calling VSC_HVDC::"<<__FUNCTION__<<"()"<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

double VSC_HVDC::get_dc_line_current_in_kA(unsigned int index, LINE_SIDE meter_side) const
{
    if(index<get_dc_line_count())
    {
        unsigned int ibus = get_dc_line_sending_side_bus(index);
        unsigned int jbus = get_dc_line_receiving_side_bus(index);
        double R = get_dc_line_resistance_in_ohm(index);
        double Vi = get_dc_bus_Vdc_in_kV(dc_bus_no2index(ibus));
        double Vj = get_dc_bus_Vdc_in_kV(dc_bus_no2index(jbus));
        if(meter_side==SENDING_SIDE)
            return (Vi-Vj)/R;
        else
            return (Vj-Vi)/R;
    }
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal dc line count when calling VSC_HVDC::"<<__FUNCTION__<<"()"<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 0.0;
    }
}

double VSC_HVDC::get_dc_line_power_in_MW(unsigned int index, LINE_SIDE meter_side) const
{
    //cout<<"index: "<<index<<endl;
    if(index<get_dc_line_count())
    {
        double I = get_dc_line_current_in_kA(index, meter_side);
        //cout<<"I: "<<I<<endl;
        unsigned int dc_bus_number = INDEX_NOT_EXIST;
        if(meter_side==SENDING_SIDE)
            dc_bus_number=get_dc_line_sending_side_bus(index);
        else
            dc_bus_number=get_dc_line_receiving_side_bus(index);
        //cout<<"dc_bus_number:"<<dc_bus_number<<endl;
        double V = get_dc_bus_Vdc_in_kV_with_dc_bus_number(dc_bus_number);
        return V*I;
    }
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal dc line count when calling VSC_HVDC::"<<__FUNCTION__<<"()"<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 0.0;
    }
}

double VSC_HVDC::get_dc_line_current_in_kA(unsigned int index, unsigned int meter_side) const
{
    if(index<get_dc_line_count())
    {
        unsigned int ibus = get_dc_line_sending_side_bus(index);
        unsigned int jbus = get_dc_line_receiving_side_bus(index);
        double R = get_dc_line_resistance_in_ohm(index);
        double Vi = get_dc_bus_Vdc_in_kV(dc_bus_no2index(ibus));
        double Vj = get_dc_bus_Vdc_in_kV(dc_bus_no2index(jbus));
        if(meter_side==ibus)
            return (Vi-Vj)/R;
        else
            return (Vj-Vi)/R;
    }
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal dc line count when calling VSC_HVDC::"<<__FUNCTION__<<"()"<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 0.0;
    }
}

double VSC_HVDC::get_dc_line_power_in_MW(unsigned int index, unsigned int meter_side) const
{
    if(index<get_dc_line_count())
    {
        double V = get_dc_bus_Vdc_in_kV_with_dc_bus_number(meter_side);
        double I = get_dc_line_current_in_kA(index, meter_side);
        //cout<<"I: "<<I<<endl;
        return V*I;
    }
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal dc line count when calling VSC_HVDC::"<<__FUNCTION__<<"()"<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 0.0;
    }
}

double VSC_HVDC::get_dc_line_current_in_kA(DC_DEVICE_ID dc_did, unsigned int meter_side) const
{
    unsigned int index = dc_line_device_id2index(dc_did);
    return get_dc_line_current_in_kA(index, meter_side);
}

double VSC_HVDC::get_dc_line_power_in_MW(DC_DEVICE_ID dc_did, unsigned int meter_side) const
{
    unsigned int index = dc_line_device_id2index(dc_did);
    return get_dc_line_power_in_MW(index, meter_side);
}

double VSC_HVDC::get_dc_line_loss_in_MW(unsigned int index) const
{
    if(index<get_dc_line_count())
    {
        double Pi = get_dc_line_power_in_MW(index, SENDING_SIDE);
        double Pj = get_dc_line_power_in_MW(index, RECEIVING_SIDE);
        return Pi+Pj;
    }
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal dc line count when calling VSC_HVDC::"<<__FUNCTION__<<"()"<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 0.0;
    }
}

bool VSC_HVDC::is_connected_to_bus(unsigned int bus) const
{
    unsigned int n_converter = converters.size();
    for(unsigned int i=0; i!=n_converter; ++i)
    {
        if (get_converter_ac_bus(i)==bus) return true;
    }
    return false;
}

bool VSC_HVDC::is_connected_to_dc_bus(unsigned int bus) const
{
    unsigned int n_dc_bus = dc_buses.size();
    for(unsigned int i=0; i!=n_dc_bus; ++i)
    {
        if (get_dc_bus_number(i)==bus) return true;
    }
    return false;
}

bool VSC_HVDC::is_in_area(unsigned int area) const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    unsigned int n_converter = converters.size();
    bool in_area = false;
    for(unsigned int i=0; i!=n_converter; ++i)
    {
        BUS* busptr = psdb.get_bus(get_converter_ac_bus(i));
        if(busptr!=NULL)
            in_area = in_area or busptr->is_in_area(area);
        if(in_area)
            break;
    }
    return in_area;
}

bool VSC_HVDC::is_in_zone(unsigned int zone) const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    unsigned int n_converter = converters.size();
    bool in_zone = false;
    for(unsigned int i=0; i!=n_converter; ++i)
    {
        BUS* busptr = psdb.get_bus(get_converter_ac_bus(i));
        if(busptr!=NULL)
            in_zone = in_zone or busptr->is_in_zone(zone);
        if(in_zone)
            break;
    }
    return in_zone;
}

bool VSC_HVDC::is_valid() const
{
    unsigned int n_converter = converters.size();
    for(unsigned int i=0; i!=n_converter; ++i)
    {
        if (get_converter_ac_bus(i)==0) return false;
    }
    return true;
}

void VSC_HVDC::check()
{
    ;
}

void VSC_HVDC::report() const
{
    cout<<"get_name():"<<get_name()<<endl;
    cout<<"get_converter_count():"<<get_converter_count()<<endl;
    cout<<"get_dc_bus_count():"<<get_dc_bus_count()<<endl;
    cout<<"get_dc_line_count():"<<get_dc_line_count()<<endl;
    cout<<"get_project_status():"<<get_status()<<endl;
    unsigned int ncon=get_converter_count();
    unsigned int nbus=get_dc_bus_count();
    unsigned int ndc_line=get_dc_line_count();
    for(unsigned int i=0;i!=ncon;++i)
    {
        cout<<"get_converter_ac_bus("<<i<<"): "<<get_converter_ac_bus(i)<<endl;
        cout<<"get_converter_status("<<i<<"): "<<get_converter_status(i)<<endl;
        cout<<"get_converter_active_power_operation_mode("<<i<<"): "<<get_converter_active_power_operation_mode(i)<<endl;
        cout<<"get_converter_dc_voltage_control_priority("<<i<<"): "<<get_converter_dc_voltage_control_priority(i)<<endl;
        cout<<"get_converter_reactive_power_operation_mode("<<i<<"): "<<get_converter_reactive_power_operation_mode(i)<<endl;
        cout<<"get_converter_loss_factor_A_in_kW("<<i<<"): "<<get_converter_loss_factor_A_in_kW(i)<<endl;
        cout<<"get_converter_loss_factor_B_in_kW_per_amp("<<i<<"): "<<get_converter_loss_factor_B_in_kW_per_amp(i)<<endl;
        cout<<"get_converter_loss_factor_C_in_kW_per_amp_squard("<<i<<"): "<<get_converter_loss_factor_C_in_kW_per_amp_squard(i)<<endl;
        cout<<"get_converter_minimum_loss_in_kW("<<i<<"): "<<get_converter_minimum_loss_in_kW(i)<<endl;
        cout<<"get_converter_rated_current_in_A("<<i<<"): "<<get_converter_rated_current_in_A(i)<<endl;
        cout<<"get_converter_transformer_capacity_in_MVA("<<i<<"): "<<get_converter_transformer_capacity_in_MVA(i)<<endl;
        cout<<"get_converter_transformer_AC_side_base_voltage_in_kV("<<i<<"): "<<get_converter_transformer_AC_side_base_voltage_in_kV(i)<<endl;
        cout<<"get_converter_transformer_converter_side_base_voltage_in_kV("<<i<<"): "<<get_converter_transformer_converter_side_base_voltage_in_kV(i)<<endl;
        cout<<"get_converter_transformer_off_nominal_turn_ratio("<<i<<"): "<<get_converter_transformer_off_nominal_turn_ratio(i)<<endl;
        cout<<"get_converter_transformer_impedance_in_pu("<<i<<"): "<<get_converter_transformer_impedance_in_pu(i)<<endl;
        cout<<"get_converter_commutating_impedance_in_ohm("<<i<<"): "<<get_converter_commutating_impedance_in_ohm(i)<<endl;
        cout<<"get_converter_filter_admittance_in_siemens("<<i<<"): "<<get_converter_filter_admittance_in_siemens(i)<<endl;
        cout<<"get_converter_P_to_AC_bus_in_MW("<<i<<"): "<<get_converter_P_to_AC_bus_in_MW(i)<<endl;
        cout<<"get_converter_Q_to_AC_bus_in_MVar("<<i<<"): "<<get_converter_Q_to_AC_bus_in_MVar(i)<<endl;
        cout<<"get_converter_Pmax_in_MW("<<i<<"): "<<get_converter_Pmax_in_MW(i)<<endl;
        cout<<"get_converter_Pmin_in_MW("<<i<<"): "<<get_converter_Pmin_in_MW(i)<<endl;
        cout<<"get_converter_Qmax_in_MVar("<<i<<"): "<<get_converter_Qmax_in_MVar(i)<<endl;
        cout<<"get_converter_Qmin_in_MVar("<<i<<"): "<<get_converter_Qmin_in_MVar(i)<<endl;
        cout<<"get_converter_Udmax_in_kV("<<i<<"): "<<get_converter_Udmax_in_kV(i)<<endl;
        cout<<"get_converter_Udmin_in_kV("<<i<<"): "<<get_converter_Udmin_in_kV(i)<<endl;
        cout<<"get_converter_remote_bus_to_regulate("<<i<<"): "<<get_converter_remote_bus_to_regulate(i)<<endl;
        cout<<"get_converter_remote_regulation_percent("<<i<<"): "<<get_converter_remote_regulation_percent(i)<<endl;
    }
    for(unsigned int i=0;i!=nbus;++i)
    {
        cout<<"get_dc_bus_number("<<i<<"): "<<get_dc_bus_number(i)<<endl;
        cout<<"get_dc_bus_Vdc_in_kV("<<i<<"): "<<get_dc_bus_Vdc_in_kV(i)<<endl;
        cout<<"get_dc_bus_name("<<i<<"): "<<get_dc_bus_name(i)<<endl;
        cout<<"get_dc_bus_area("<<i<<"): "<<get_dc_bus_area(i)<<endl;
        cout<<"get_dc_bus_zone("<<i<<"): "<<get_dc_bus_zone(i)<<endl;
        cout<<"get_dc_bus_owner_number("<<i<<"): "<<get_dc_bus_owner_number(i)<<endl;
        cout<<"get_dc_bus_ground_resistance_in_ohm("<<i<<"): "<<get_dc_bus_ground_resistance_in_ohm(i)<<endl;
        cout<<"get_dc_bus_generation_power_in_MW("<<i<<"): "<<get_dc_bus_generation_power_in_MW(i)<<endl;
        cout<<"get_dc_bus_load_power_in_MW("<<i<<"): "<<get_dc_bus_load_power_in_MW(i)<<endl;
    }
    for(unsigned int i=0;i!=ndc_line;++i)
    {
        cout<<"get_dc_line_sending_side_bus("<<i<<"): "<<get_dc_line_sending_side_bus(i)<<endl;
        cout<<"get_dc_line_receiving_side_bus("<<i<<"): "<<get_dc_line_receiving_side_bus(i)<<endl;
        cout<<"get_dc_line_identifier("<<i<<"): "<<get_dc_line_identifier(i)<<endl;
        cout<<"get_dc_line_meter_end_bus("<<i<<"): "<<get_dc_line_meter_end_bus(i)<<endl;
        cout<<"get_dc_line_resistance_in_ohm("<<i<<"): "<<get_dc_line_resistance_in_ohm(i)<<endl;
        cout<<"get_dc_line_inductance_in_mH("<<i<<"): "<<get_dc_line_inductance_in_mH(i)<<endl;
    }
}

void VSC_HVDC::save() const
{
    ;
}

DEVICE_ID VSC_HVDC::get_device_id() const
{
    DEVICE_ID did;
    did.set_device_type(STEPS_VSC_HVDC);
    TERMINAL terminal;
    unsigned int n_cov = get_converter_count();
    for(unsigned int i=0; i!=n_cov; ++i)
        terminal.append_bus(get_converter_ac_bus(i));
    did.set_device_terminal(terminal);
    did.set_device_identifier_index(get_identifier_index());

    return did;
}

void VSC_HVDC::set_model(MODEL* model)
{
    if(model != NULL and model->has_allowed_device_type(STEPS_VSC_HVDC))
    {
        model->set_device_id(get_device_id());
        if(model->get_model_type()=="VSC HVDC")
        {
            set_vsc_hvdc_model((VSC_HVDC_MODEL*) model);
            model->allocate_model_variables();
            model->prepare_model_data_table();
            model->prepare_model_internal_variable_table();
            return;
        }
        ostringstream osstream;
        osstream<<"Waring. VSC HVDC model is not given to set dynamic model for "<<get_compound_device_name();
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

MODEL* VSC_HVDC::get_model_of_type(string model_type)
{
    model_type = string2upper(model_type);
    if(model_type=="VSC HVDC")
    {
        return get_vsc_hvdc_model();
    }
    return NULL;
}

void VSC_HVDC::set_vsc_hvdc_model(VSC_HVDC_MODEL* model)
{
    if(model!=NULL)
    {
        delete_vsc_hvdc_model();
        vsc_hvdc_model = model;
    }
}

VSC_HVDC_MODEL* VSC_HVDC::get_vsc_hvdc_model() const
{
    return vsc_hvdc_model;
}

void VSC_HVDC::delete_vsc_hvdc_model()
{
    if(get_vsc_hvdc_model()!=NULL)
    {
        delete vsc_hvdc_model;
        vsc_hvdc_model =  NULL;
    }
}

void VSC_HVDC::solve_steady_state()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    unsigned int iter_count_of_dc_bus_control_mode_changed = 0;

    initialize_steady_state_solver();

    optimize_network_ordering();
    build_dc_network_matrix();
    update_P_equation_internal_buses();
    update_current_dc_slack_bus();
    //export_dc_bus_voltage_with_network_ordering();
    //show_inphno_bus_number();
    //while(true)
    for(unsigned int i=0;i!=1;++i)
    {
        //try to restore initial VDC control bus. reset initial P commands
        //cout<<"ieteration_count: "<<iteration_count<<endl;
        unsigned int inner_iteration_count = 0;
        while(true)
        {
            calculate_raw_bus_power_mismatch();
            max_P_mismatch_in_MW = get_maximum_active_power_mismatch_in_MW();
            //cout<<"max_P_mismatch_in_MW:"<<max_P_mismatch_in_MW<<endl;
            //cout<<"allowed_max_active_power_imbalance_in_MW:"<<get_allowed_max_active_power_imbalance_in_MW()<<endl;
            if(max_P_mismatch_in_MW<get_allowed_max_active_power_imbalance_in_MW())
            {
                set_convergence_flag(true);
                osstream<<"vsc powerflow inner iteration converged within ("<<inner_iteration_count<<") iterations."<<endl;
                toolkit.show_information_with_leading_time_stamp(osstream);
                break;
            }
            if(inner_iteration_count>=get_max_iteration())
            {
                osstream<<"Vsc hvdc inner powerflow failed to converge within ("<<get_max_iteration()<<") iterations."<<endl;
                toolkit.show_information_with_leading_time_stamp(osstream);
                break;
            }
            build_dc_bus_power_mismatch_vector();
            build_jacobian();
            //show_jacobian_matrix();
            Udc_mismatch = P_mismatch/jacobian;
            /*for(unsigned int m=0;m!=Udc_mismatch.size();++m)
                cout<<"Udc_mismatch:"<<Udc_mismatch[m]<<endl;
                */
            if(jacobian.is_lu_factorization_successful())
            {
                update_dc_bus_voltage();
                ++inner_iteration_count;
            }
            else
            {
                osstream<<"Fatal Error. Vsc hvdc inner powerflow solution failed since LU factorization of N-R Jacobian matrix is failed."<<endl;
                toolkit.show_information_with_leading_time_stamp(osstream);
                break;
            }
        }
        if(get_convergence_flag()==true)
        {
            osstream<<"vsc powerflow iteration converged within ("<<get_iteration_count()<<") iterations."<<endl;
            toolkit.show_information_with_leading_time_stamp(osstream);
            break;
        }
        else
        {
            if(jacobian.is_lu_factorization_successful())
            {
                bool dc_bus_control_mode_changed=false;
                calculate_raw_bus_power_mismatch();
                dc_bus_control_mode_changed = check_dc_slack_converter_constraint();  //should check if VDC bus should be replaced
                if(dc_bus_control_mode_changed)
                {
                    cout<<"dc_bus_control_mode_changed!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
                    update_P_equation_internal_buses();
                    ++iter_count_of_dc_bus_control_mode_changed;
                }
            }
            else
            {
                osstream<<"Fatal Error. Vsc hvdc outer powerflow solution failed since LU factorization of N-R Jacobian matrix is failed."<<endl;
                toolkit.show_information_with_leading_time_stamp(osstream);
                break;
            }
        }
        ++iteration_count;
        if(get_iteration_count()>=get_max_iteration())
        {
            osstream<<"Vsc hvdc powerflow failed to converge within ("<<get_max_iteration()<<") iterations."<<endl;
            toolkit.show_information_with_leading_time_stamp(osstream);
            break;
        }
        if(iter_count_of_dc_bus_control_mode_changed>=get_converter_count())
        {
            osstream<<"Vsc hvdc powerflow failed to converge within ("<<get_converter_count()<<") dc control mode change."<<endl;
            toolkit.show_information_with_leading_time_stamp(osstream);
            break;
        }
    }
    save_dc_bus_powerflow_result_to_file("Vsc_hvdc_bus_powerflow_result.csv");
    calculate_dc_active_power_of_slack_bus();
    unsigned int n_converter=get_converter_count();
    /*
    for(unsigned int i=0;i!=n_converter;++i)
    {
        cout<<"get_converter_Pdc: "<<get_converter_Pdc_command_in_MW(i)<<endl;
    }
    */
    update_converters_P_and_Q_to_AC_bus();

}

void VSC_HVDC::initialize_steady_state_solver()
{
    iteration_count=0;
    converged = false;

    initialize_alpha_vector();
    initialize_beta_vector();
    initialize_active_and_reactive_power_control_mode_vector();
    initialize_dc_bus_voltage();
}

void VSC_HVDC::initialize_dc_bus_voltage()
{
    unsigned int nbus = get_dc_bus_count();
    for(unsigned int i=0; i!=nbus; ++i)
    {
        double vdc_pu = get_dc_bus_Vdc_in_kV(i)/get_dc_network_base_voltage_in_kV();
        if(vdc_pu<0.5 or vdc_pu>1.5)
        {
            unsigned int index = get_dc_bus_converter_index_with_dc_bus_index(i);
            if(index!=INDEX_NOT_EXIST)
            {
                VSC_HVDC_CONVERTER_ACTIVE_POWER_CONTROL_MODE mode = current_active_power_control_mode[index];
                if(mode == VSC_DC_VOLTAGE_CONTORL or
                   mode == VSC_DC_CURRENT_VOLTAGE_DROOP_CONTROL or
                   mode == VSC_DC_ACTIVE_POWER_VOLTAGE_DROOP_CONTROL)
                    set_dc_bus_Vdc_in_kV(i, get_converter_initial_dc_voltage_reference_in_kV(index));
                else
                    set_dc_bus_Vdc_in_kV(i, get_dc_network_base_voltage_in_kV());
            }
            else
                set_dc_bus_Vdc_in_kV(i, get_dc_network_base_voltage_in_kV());

        }
        //cout<<"get_dc_bus_voltage(i)"<<i<<"  "<<get_dc_bus_Vdc_in_kV(i)<<endl;
    }
}

void VSC_HVDC::optimize_network_ordering()
{
    initialize_physical_internal_bus_pair();
    build_dc_network_matrix();
    reorder_physical_internal_bus_pair();
    /*for(unsigned int i=0; i<1; ++i)
    {
        reorder_physical_internal_bus_pair();
        build_network_Y1_matrix();
    }*/
}

void VSC_HVDC::initialize_physical_internal_bus_pair()
{
    unsigned int n_dcbus = get_dc_bus_count();

    if(n_dcbus!=0)
    {
        inphno.clear();

        unsigned int internal_bus_number = 0;
        for(unsigned int i=0; i!=n_dcbus; ++i)
        {
            unsigned int bus_number = get_dc_bus_number(i);
            inphno.set_physical_internal_bus_number_pair(bus_number, internal_bus_number);
            ++internal_bus_number;
        }
    }
}

void VSC_HVDC::reorder_physical_internal_bus_pair()
{
    STEPS& toolkit = get_toolkit();
    vector<unsigned int> permutation = dc_network_matrix.get_reorder_permutation();
    inphno.update_with_new_internal_bus_permutation(permutation);
    ostringstream osstream;
    //osstream<<"DC network internal bus numbers are optimized for "<<get_compound_device_name();
    //toolkit.show_information_with_leading_time_stamp(osstream);

    /*os<<"After optimizing the network with network optimizer, the internal buses are listed as follows");
    toolkit->show_information_with_leading_time_stamp(osstream);

    osstream<<"internal   physical   storage");
    toolkit->show_information_with_leading_time_stamp(osstream);

    unsigned int bus, index, nbus;

    nbus = get_in_service_bus_count();

    for(unsigned int i=0; i!=nbus; ++i)
    {
        bus = get_physical_bus_number_of_internal_bus(i);
        index = get_bus_index(bus);
        osstream<<"%-10u %-10u %-10u",i, bus, index);
        toolkit->show_information_with_leading_time_stamp(osstream);
    }*/

    /*os<<"Permutation for reorder internal bus:");
    toolkit->show_information_with_leading_time_stamp(osstream);

    unsigned int nperm = permutation.size();
    for(unsigned int i=0; i!=nperm; ++i)
    {
        osstream<<"%u ---> %u",i,permutation[i]);
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
    */
}

void VSC_HVDC::update_P_equation_internal_buses()
{
    STEPS& toolkit = get_toolkit();
    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
    //snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Updating powerflow P equation buses for %s",get_compound_device_name().c_str());
    //toolkit.show_information_with_leading_time_stamp(buffer);

    unsigned int nbus = get_dc_bus_count();

    internal_P_equation_buses.clear();
    internal_P_equation_buses.reserve(nbus);

    for(unsigned int i=0; i!=nbus; ++i)
    {
        unsigned int dc_bus_number = inphno.get_physical_bus_number_of_internal_bus_number(i);
        unsigned int index = get_dc_bus_converter_index_with_dc_bus_number(dc_bus_number);
        if(index!=INDEX_NOT_EXIST)
        {
            if(current_active_power_control_mode[index]!=VSC_DC_VOLTAGE_CONTORL)
                internal_P_equation_buses.push_back(i);
        }
        else
            internal_P_equation_buses.push_back(i);
    }
	/*
    osstream<<"Buses with P equations (physical bus):" << endl;
    toolkit->show_information_with_leading_time_stamp(osstream);
    unsigned int n = internal_P_equation_buses.size();
    for(unsigned int i=0; i!=n; ++i)
    {
        osstream<<network_matrix.get_physical_bus_number_of_internal_bus(internal_P_equation_buses[i]) << endl;
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
    osstream<<"Buses with Q equations (physical bus):"<<endl;
    toolkit->show_information_with_leading_time_stamp(osstream);
    n = internal_Q_equation_buses.size();
    for(unsigned int i=0; i!=n; ++i)
    {
        osstream<< network_matrix.get_physical_bus_number_of_internal_bus(internal_Q_equation_buses[i]) << endl;
        toolkit->show_information_with_leading_time_stamp(osstream);
    }*/

    //snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Done updating powerflow P equation buses for %s",get_compound_device_name().c_str());
    //toolkit.show_information_with_leading_time_stamp(buffer);

}

void VSC_HVDC::build_dc_network_matrix()
{
    if(inphno.get_table_size()<2)
        initialize_physical_internal_bus_pair();

    dc_network_matrix.clear();
    build_initial_zero_matrix();
    add_dc_lines_to_dc_network();
    dc_network_matrix.compress_and_merge_duplicate_entries();
}

void VSC_HVDC::build_initial_zero_matrix()
{
    unsigned int n = get_dc_bus_count();
    for(unsigned int i=0; i!=n; ++i)
        dc_network_matrix.add_entry(i, i, 0);
}

void VSC_HVDC::add_dc_lines_to_dc_network()
{
    unsigned int n = get_dc_line_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        unsigned int ibus = get_dc_line_sending_side_bus(i);
        unsigned int jbus = get_dc_line_receiving_side_bus(i);
        double g = 1.0/get_dc_line_resistance_in_ohm(i);
        unsigned int i_inbus = inphno.get_internal_bus_number_of_physical_bus_number(ibus);
        unsigned int j_inbus = inphno.get_internal_bus_number_of_physical_bus_number(jbus);

        dc_network_matrix.add_entry(i_inbus, i_inbus, g);
        dc_network_matrix.add_entry(j_inbus, j_inbus, g);
        dc_network_matrix.add_entry(i_inbus, j_inbus, -g);
        dc_network_matrix.add_entry(j_inbus, i_inbus, -g);
    }
}

void VSC_HVDC::initialize_alpha_vector()
{
    unsigned int n_converter = get_converter_count();
    alpha.clear();
    alpha.reserve(n_converter);
    for(unsigned int i=0; i<n_converter; ++i)
    {
        if(get_converter_active_power_operation_mode(i) == VSC_DC_CURRENT_VOLTAGE_DROOP_CONTROL)
            alpha.push_back(1);
        else
            alpha.push_back(0);
    }
    //for(unsigned int i=0; i!=n_converter;++i)
    //{
    //    cout<<"alpha[i]:"<<alpha[i]<<endl;
    //}
}

void VSC_HVDC::initialize_beta_vector()
{
    unsigned int n_converter = get_converter_count();
    beta.clear();
    beta.reserve(n_converter);
    for(unsigned int i=0; i<n_converter; ++i)
    {
        if(get_converter_active_power_operation_mode(i) == VSC_DC_ACTIVE_POWER_VOLTAGE_DROOP_CONTROL)
            beta.push_back(1);
        else
            beta.push_back(0);
    }
    /*
    for(unsigned int i=0; i!=n_converter;++i)
    {
        cout<<"beta[i]:"<<beta[i]<<endl;
    }
    */
}

void VSC_HVDC::initialize_active_and_reactive_power_control_mode_vector()
{
    unsigned int n_converter=get_converter_count();
    current_active_power_control_mode.clear();
    current_active_power_control_mode.reserve(n_converter);
    current_reactive_power_control_mode.clear();
    current_reactive_power_control_mode.reserve(n_converter);
    //cout<<"current_active_power_control_mode.reserve(n_converter)"<<endl;
    for(unsigned int index=0; index!=n_converter;++index)
    {
        VSC_HVDC_CONVERTER_ACTIVE_POWER_CONTROL_MODE active_power_control_mode=get_converter_active_power_operation_mode(index);
        VSC_HVDC_CONVERTER_REACTIVE_POWER_CONTROL_MODE reactive_power_control_mode=get_converter_reactive_power_operation_mode(index);
        //cout<<"active_power_control_mode: "<<active_power_control_mode<<endl;
        //cout<<"reactive_power_control_mode: "<<reactive_power_control_mode<<endl;
        current_active_power_control_mode[index]=active_power_control_mode;
        current_reactive_power_control_mode[index]=reactive_power_control_mode;
    }
}

void VSC_HVDC::calculate_raw_bus_power_mismatch()
{
    unsigned int nbus=get_dc_bus_count();
    P_mismatch.clear();
    P_mismatch.reserve(nbus);
    for(unsigned int i=0; i<nbus-1; ++i)
        P_mismatch.push_back(0.0);

    calculate_raw_dc_current_into_dc_network();
    calculate_raw_dc_power_into_dc_network();

    for(unsigned int i=0;i!=nbus;++i)
    {
        bus_power[i] = -bus_power[i];
        //cout<<"bus_power[i]: "<<bus_power[i]<<endl;
    }

    add_generation_power_to_raw_bus_power_mismatch();
    add_load_power_to_raw_bus_power_mismatch();
    add_converter_dc_power_command_to_raw_bus_power_mismatch();
}


void VSC_HVDC::calculate_raw_dc_current_into_dc_network()
{
    unsigned int nbus=get_dc_bus_count();
    bus_current.clear();
    bus_current.reserve(nbus);
    for(unsigned int i=0;i!=nbus;++i)
        bus_current.push_back(0.0);
    int nsize = dc_network_matrix.get_matrix_size();
    int k_start=0;
    for(int column=0; column != nsize; ++column)
    {
        unsigned int bus = inphno.get_physical_bus_number_of_internal_bus_number(column);
        double voltage = get_dc_bus_Vdc_in_kV(dc_bus_no2index(bus));

        int k_end = dc_network_matrix.get_starting_index_of_column(column+1);
        for(int k=k_start; k<k_end; ++k)
        {
            int row=dc_network_matrix.get_row_number_of_entry_index(k);
            double yij = dc_network_matrix.get_real_entry_value(k);
            bus_current[row]+= (yij * voltage);
        }
        k_start = k_end;
    }
}

void VSC_HVDC::calculate_raw_dc_power_into_dc_network()
{
    unsigned int nbus=get_dc_bus_count();
    bus_power.clear();
    bus_power.reserve(nbus);
    for(unsigned int i=0;i!=nbus;++i)
        bus_power.push_back(0.0);

    for(unsigned int i=0;i!=nbus;++i)
    {
        unsigned int bus = inphno.get_physical_bus_number_of_internal_bus_number(i);
        double Udc = get_dc_bus_Vdc_in_kV(dc_bus_no2index(bus));
        bus_power[i] = Udc * bus_current[i];
        //cout<<"bus_power[i]:"<<bus_power[i]<<endl;
    }
}

void VSC_HVDC::add_generation_power_to_raw_bus_power_mismatch()
{
    unsigned int nbus = get_dc_bus_count();
    for (unsigned int index=0; index!=nbus; ++index)
    {
        unsigned int dcbus = get_dc_bus_number(index);
        int i = inphno.get_internal_bus_number_of_physical_bus_number(dcbus);
        bus_power[i] += get_dc_bus_generation_power_in_MW(index);
    }
}

void VSC_HVDC::add_load_power_to_raw_bus_power_mismatch()
{
    unsigned int nbus = get_dc_bus_count();
    for (unsigned int index=0; index!=nbus; ++index)
    {
        unsigned int dcbus = get_dc_bus_number(index);
        int i = inphno.get_internal_bus_number_of_physical_bus_number(dcbus);
        bus_power[i] -= get_dc_bus_load_power_in_MW(index);
    }
}

void VSC_HVDC::add_converter_dc_power_command_to_raw_bus_power_mismatch()
{
    unsigned int nbus = get_dc_bus_count();
    for(unsigned int index=0; index!=nbus; ++index)
    {
        unsigned int bus = get_dc_bus_number(index);
        int i = inphno.get_internal_bus_number_of_physical_bus_number(bus);
        bus_power[i] += get_converter_dc_power_command_at_dc_bus(bus);
    }
}

double VSC_HVDC::get_converter_dc_power_command_at_dc_bus(unsigned int bus)
{
    unsigned int converter_index = get_dc_bus_converter_index_with_dc_bus_number(bus);
    return get_converter_dc_power_command(converter_index);
}

double VSC_HVDC::get_converter_dc_power_command(unsigned int converter_index)
{
    if(converter_index!=INDEX_NOT_EXIST)
    {
        double Pdc_command = 0.0;
        VSC_HVDC_CONVERTER_ACTIVE_POWER_CONTROL_MODE active_power_mode = current_active_power_control_mode[converter_index];
        VSC_HVDC_CONVERTER_REACTIVE_POWER_CONTROL_MODE reactive_power_mode = current_reactive_power_control_mode[converter_index];
        switch(active_power_mode)
        {
            case VSC_AC_ACTIVE_POWER_CONTORL:
                if(reactive_power_mode==VSC_AC_REACTIVE_POWER_CONTROL)
                    Pdc_command = solve_Pdc_with_active_power_control_and_reactive_power_control(converter_index);
                else
                    Pdc_command =  solve_Pdc_with_active_power_control_and_ac_voltage_control(converter_index);
                break;
            case VSC_AC_VOLTAGE_ANGLE_CONTROL:
                if(reactive_power_mode==VSC_AC_REACTIVE_POWER_CONTROL)
                    Pdc_command =  solve_Pdc_with_voltage_angle_and_reactive_power_control(converter_index);
                else
                    Pdc_command =  solve_Pdc_with_voltage_angle_and_voltage_control(converter_index);
                break;
            case VSC_DC_ACTIVE_POWER_VOLTAGE_DROOP_CONTROL:
                Pdc_command =  solve_Pdc_with_dc_active_power_voltage_droop_control(converter_index);
                break;
            case VSC_DC_CURRENT_VOLTAGE_DROOP_CONTROL:
                Pdc_command =  solve_Pdc_with_dc_current_voltage_droop_control(converter_index);
                break;
            case VSC_DC_VOLTAGE_CONTORL:
            default:
                Pdc_command =  0.0;
                break;
        }
        if(active_power_mode != VSC_AC_VOLTAGE_ANGLE_CONTROL)
        {
            double pmax = get_converter_Pmax_in_MW(converter_index);
            double pmin = get_converter_Pmin_in_MW(converter_index);
            if(Pdc_command > pmax)
                Pdc_command = pmax;
            else
            {
                if(Pdc_command < pmin)
                    Pdc_command = pmin;
            }
        }
        set_converter_Pdc_command_in_MW(converter_index, Pdc_command);
        //cout<<"Pdc_command: "<<Pdc_command<<endl;
        return Pdc_command;
    }
    else
        return 0.0;
}

void VSC_HVDC::build_dc_bus_power_mismatch_vector()
{
    unsigned int nP = internal_P_equation_buses.size();

    P_mismatch.resize(nP);

    #ifdef ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
        set_openmp_number_of_threads(toolkit->get_thread_number());
        #pragma omp parallel for schedule(static)
    #endif // ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
    for(unsigned int i=0; i<nP; ++i)
    {
        unsigned int internal_bus = internal_P_equation_buses[i];
        //s_mismatch.push_back(-bus_power[internal_bus].real());
        P_mismatch[i] = -bus_power[internal_bus];
        //cout<<"P_mismatch: "<<P_mismatch[i]<<endl;;
    }
}

double VSC_HVDC::solve_Pdc_with_active_power_control_and_reactive_power_control(unsigned int converter_index) const
{
    complex<double> j(0.0,1.0);
    double Pac_command = get_converter_nominal_ac_active_power_command_in_MW(converter_index);
    double Qac_command = get_converter_nominal_ac_reactive_power_command_in_Mvar(converter_index);
    //cout<<"active_and_reactive_power_mode->"<<"active_power_command: "<<Pac_command<<" reactive_power_command: "<<Qac_command<<endl;
    BUS *bus_pointer = get_converter_ac_bus_pointer(converter_index);
    complex<double> Vac=bus_pointer->get_positive_sequence_complex_voltage_in_kV();
    //cout<<"get_bus_number: "<<bus_pointer->get_bus_number();
    //cout<<"Vac-> "<<Vac<<endl;
    double Vbase_ac=get_converter_transformer_AC_side_base_voltage_in_kV(converter_index);
    double Vbase_converter=get_converter_transformer_converter_side_base_voltage_in_kV(converter_index);
    double k=get_converter_transformer_off_nominal_turn_ratio(converter_index);
    double turn_ration=k*Vbase_ac/Vbase_converter;
    /*
    cout<<"Vbase_ac-> "<<Vbase_ac<<endl;
    cout<<"Vbase_converter-> "<<Vbase_converter<<endl;
    cout<<"k-> "<<k<<endl;
    cout<<"turn_ration-> "<<turn_ration<<endl;
    */
    complex<double> Yf_in_siemens=get_converter_filter_admittance_in_siemens(converter_index);
    complex<double> Zc_in_ohm=get_converter_commutating_impedance_in_ohm(converter_index);
    complex<double> Eac=Vac/turn_ration;
    complex<double> Zt_in_pu=get_converter_transformer_impedance_in_pu(converter_index);
    complex<double> Zt_in_ohm=Zt_in_pu*Vbase_converter*Vbase_converter/get_converter_transformer_capacity_in_MVA(converter_index);
    complex<double> Vac_f=Eac+(Pac_command-j*Qac_command)*Zt_in_ohm/conj(Eac);
    complex<double> Ic=(Pac_command-j*Qac_command)/conj(Eac)+Vac_f*Yf_in_siemens;
    complex<double> Vac_c=Vac_f+Ic*Zc_in_ohm;
    /*
    cout<<"Yf_in_siemens-> "<<Yf_in_siemens<<endl;
    cout<<"Zc_in_ohm-> "<<Zc_in_ohm<<endl;
    cout<<"Eac-> "<<Eac<<endl;
    cout<<"Zt_in_pu-> "<<Zt_in_pu<<endl;
    cout<<"Zt_in_ohm-> "<<Zt_in_ohm<<endl;
    cout<<"Vac_f-> "<<Vac_f<<endl;
    cout<<"Ic-> "<<Ic<<endl;
    cout<<"Vac_c-> "<<Vac_c<<endl;
    */
    double Pc_command=(Vac_c*conj(Ic)).real();
    double c_loss=get_converter_loss_factor_C_in_kW_per_amp_squard(converter_index);
    double b_loss=get_converter_loss_factor_B_in_kW_per_amp(converter_index);
    double a_loss=get_converter_loss_factor_A_in_kW(converter_index);
    double Ic_mag=abs(Ic)*1000;
    double P_converter_loss=(c_loss*Ic_mag*Ic_mag+b_loss*Ic_mag+a_loss)/1000;
    double Pdc=-Pc_command-P_converter_loss;
    /*
    cout<<"Pc_command-> "<<Pc_command<<endl;
    cout<<"c_loss-> "<<c_loss<<endl;
    cout<<"b_loss-> "<<b_loss<<endl;
    cout<<"a_loss-> "<<a_loss<<endl;
    cout<<"Ic_mag-> "<<Ic_mag<<endl;
    cout<<"P_converter_loss-> "<<P_converter_loss<<endl;
    cout<<"Pdc-> "<<Pdc<<endl;
    */
    return Pdc;
}

double VSC_HVDC::solve_Pdc_with_active_power_control_and_ac_voltage_control(unsigned int converter_index) const
{
    complex<double> j(0.0,1.0);
    double Pac_command = get_converter_nominal_ac_active_power_command_in_MW(converter_index);
    double Qac_command = get_converter_Q_to_AC_bus_in_MVar(converter_index);
    BUS *bus_pointer = get_converter_ac_bus_pointer(converter_index);
    complex<double> Vac=bus_pointer->get_positive_sequence_complex_voltage_in_kV();
    double Vbase_ac=get_converter_transformer_AC_side_base_voltage_in_kV(converter_index);
    double Vbase_converter=get_converter_transformer_converter_side_base_voltage_in_kV(converter_index);
    double k=get_converter_transformer_off_nominal_turn_ratio(converter_index);
    double turn_ration=k*Vbase_ac/Vbase_converter;
    complex<double> Yf_in_ohm=get_converter_filter_admittance_in_siemens(converter_index);
    complex<double> Zc_in_ohm=get_converter_commutating_impedance_in_ohm(converter_index);
    complex<double> Eac=Vac/turn_ration;
    complex<double> Zt_in_pu=get_converter_transformer_impedance_in_pu(converter_index);
    complex<double> Zt_in_ohm=Zt_in_pu*Vbase_converter*Vbase_converter/get_converter_transformer_capacity_in_MVA(converter_index);
    complex<double> Vac_f=Eac+(Pac_command-j*Qac_command)*Zt_in_ohm/conj(Eac);
    complex<double> Ic=(Pac_command-j*Qac_command)/conj(Eac)+Vac_f*Yf_in_ohm;
    complex<double> Vac_c=Vac_f+Ic*Zc_in_ohm;
    double Pc_command=(Vac_c*conj(Ic)).real();
    double c_loss=get_converter_loss_factor_C_in_kW_per_amp_squard(converter_index);
    double b_loss=get_converter_loss_factor_B_in_kW_per_amp(converter_index);
    double a_loss=get_converter_loss_factor_A_in_kW(converter_index);
    double Ic_mag=abs(Ic)*1000;
    double P_converter_loss=(c_loss*Ic_mag*Ic_mag+b_loss*Ic_mag+a_loss)/1000;
    double Pdc=-Pc_command-P_converter_loss;
    return Pdc;
}

double VSC_HVDC::solve_Pdc_with_voltage_angle_and_voltage_control(unsigned int converter_index) const
{
    complex<double> j(0.0,1.0);
    double Pac_command = get_converter_P_to_AC_bus_in_MW(converter_index);
    double Qac_command = get_converter_Q_to_AC_bus_in_MVar(converter_index);
    BUS *bus_pointer = get_converter_ac_bus_pointer(converter_index);
    complex<double> Vac=bus_pointer->get_positive_sequence_complex_voltage_in_kV();
    double Vbase_ac=get_converter_transformer_AC_side_base_voltage_in_kV(converter_index);
    double Vbase_converter=get_converter_transformer_converter_side_base_voltage_in_kV(converter_index);
    double k=get_converter_transformer_off_nominal_turn_ratio(converter_index);
    double turn_ration=k*Vbase_ac/Vbase_converter;
    complex<double> Yf_in_ohm=get_converter_filter_admittance_in_siemens(converter_index);
    complex<double> Zc_in_ohm=get_converter_commutating_impedance_in_ohm(converter_index);
    complex<double> Eac=Vac/turn_ration;
    complex<double> Zt_in_pu=get_converter_transformer_impedance_in_pu(converter_index);
    complex<double> Zt_in_ohm=Zt_in_pu*Vbase_converter*Vbase_converter/get_converter_transformer_capacity_in_MVA(converter_index);
    complex<double> Vac_f=Eac+(Pac_command-j*Qac_command)*Zt_in_ohm/conj(Eac);
    complex<double> Ic=(Pac_command-j*Qac_command)/conj(Eac)+Vac_f*Yf_in_ohm;
    complex<double> Vac_c=Vac_f+Ic*Zc_in_ohm;
    double Pc_command=(Vac_c*conj(Ic)).real();
    double c_loss=get_converter_loss_factor_C_in_kW_per_amp_squard(converter_index);
    double b_loss=get_converter_loss_factor_B_in_kW_per_amp(converter_index);
    double a_loss=get_converter_loss_factor_A_in_kW(converter_index);
    double Ic_mag=abs(Ic)*1000;
    double P_converter_loss=(c_loss*Ic_mag*Ic_mag+b_loss*Ic_mag+a_loss)/1000;
    double Pdc=-Pc_command-P_converter_loss;
    return Pdc;
}

double VSC_HVDC::solve_Pdc_with_voltage_angle_and_reactive_power_control(unsigned int converter_index) const
{
    complex<double> j(0.0,1.0);
    double Pac_command = get_converter_P_to_AC_bus_in_MW(converter_index);
    double Qac_command = get_converter_nominal_ac_reactive_power_command_in_Mvar(converter_index);
    BUS *bus_pointer = get_converter_ac_bus_pointer(converter_index);
    complex<double> Vac=bus_pointer->get_positive_sequence_complex_voltage_in_kV();
    double Vbase_ac=get_converter_transformer_AC_side_base_voltage_in_kV(converter_index);
    double Vbase_converter=get_converter_transformer_converter_side_base_voltage_in_kV(converter_index);
    double k=get_converter_transformer_off_nominal_turn_ratio(converter_index);
    double turn_ration=k*Vbase_ac/Vbase_converter;
    complex<double> Yf_in_ohm=get_converter_filter_admittance_in_siemens(converter_index);
    complex<double> Zc_in_ohm=get_converter_commutating_impedance_in_ohm(converter_index);
    complex<double> Eac=Vac/turn_ration;
    complex<double> Zt_in_pu=get_converter_transformer_impedance_in_pu(converter_index);
    complex<double> Zt_in_ohm=Zt_in_pu*Vbase_converter*Vbase_converter/get_converter_transformer_capacity_in_MVA(converter_index);
    complex<double> Vac_f=Eac+(Pac_command-j*Qac_command)*Zt_in_ohm/conj(Eac);
    complex<double> Ic=(Pac_command-j*Qac_command)/conj(Eac)+Vac_f*Yf_in_ohm;
    complex<double> Vac_c=Vac_f+Ic*Zc_in_ohm;
    double Pc_command=(Vac_c*conj(Ic)).real();
    double c_loss=get_converter_loss_factor_C_in_kW_per_amp_squard(converter_index);
    double b_loss=get_converter_loss_factor_B_in_kW_per_amp(converter_index);
    double a_loss=get_converter_loss_factor_A_in_kW(converter_index);
    double Ic_mag=abs(Ic)*1000;
    double P_converter_loss=(c_loss*Ic_mag*Ic_mag+b_loss*Ic_mag+a_loss)/1000;
    double Pdc=-Pc_command-P_converter_loss;
    return Pdc;
}

double VSC_HVDC::solve_Pdc_with_dc_active_power_voltage_droop_control(unsigned int converter_index) const
{
    double initial_Pdc=get_converter_initial_dc_active_power_reference_in_MW(converter_index);
    double Udcref=get_converter_initial_dc_voltage_reference_in_kV(converter_index);
    double kdp=get_converter_initial_droop_coefficient_for_droop_control(converter_index);
    int beta=get_converter_beta(converter_index);
    unsigned int dc_bus_index=get_dc_bus_index_with_converter_index(converter_index);
    double Vdc=get_dc_bus_Vdc_in_kV(dc_bus_index);
    double operating_power=beta*(Vdc-Udcref)/kdp;
    /*
    cout<<"beta: "<<beta<<endl;
    cout<<"Udcref: "<<Udcref<<endl;
    cout<<"Vdc: "<<Vdc<<endl;
    cout<<"operating_power: "<<operating_power<<endl;
    cout<<"initial_Pdc: "<<initial_Pdc<<endl;
    */
    double Pdc=initial_Pdc-operating_power;
    return Pdc;
}

double VSC_HVDC::solve_Pdc_with_dc_current_voltage_droop_control(unsigned int converter_index) const
{
    double initial_Pdc=get_converter_initial_dc_current_reference_in_kA(converter_index)*get_converter_initial_dc_voltage_reference_in_kV(converter_index);
    double Udcref=get_converter_initial_dc_voltage_reference_in_kV(converter_index);
    double Idcref=get_converter_initial_dc_current_reference_in_kA(converter_index);
    double kdi=get_converter_initial_droop_coefficient_for_droop_control(converter_index);
    int alpha=get_converter_alpha(converter_index);
    /*
    cout<<"converter_index: "<<converter_index<<endl;
    cout<<"initial_Pdc: "<<initial_Pdc<<endl;
    cout<<"Udcref: "<<Udcref<<endl;
    cout<<"Idcref: "<<Idcref<<endl;
    cout<<"kdi: "<<kdi<<endl;
    cout<<"alpha: "<<alpha<<endl;
    */
    unsigned int dc_bus_index=get_dc_bus_index_with_converter_index(converter_index);
    double Vdc=get_dc_bus_Vdc_in_kV(dc_bus_index);
    double operating_power=alpha*Vdc*(Idcref-(Vdc-Udcref)/kdi);
    double Pdc=operating_power;
    /*
    cout<<"dc_bus_index: "<<dc_bus_index<<endl;
    cout<<"Vdc: "<<Vdc<<endl;
    cout<<"operating_power: "<<operating_power<<endl;
    */
    return Pdc;
}

void VSC_HVDC::set_converter_Pdc_command_in_MW(const unsigned int converter_index, const double P)
{
    if(converter_index_is_out_of_range_in_function(converter_index, __FUNCTION__))
        return;
    converters[converter_index].Pdc_command_in_MW = P;
}

double VSC_HVDC::get_converter_Pdc_command_in_MW(unsigned int converter_index) const
{
    if(not converter_index_is_out_of_range_in_function(converter_index, __FUNCTION__))
        return converters[converter_index].Pdc_command_in_MW;
    else
        return 0.0;
}

double VSC_HVDC::get_maximum_active_power_mismatch_in_MW()
{
    unsigned int nbus = get_dc_bus_count();
    double max_P_error_in_MW = 0.0;
    unsigned int max_P_error_physical_bus = 0;
    for(unsigned int i=0; i!=nbus; ++i)
    {
        unsigned int bus = inphno.get_physical_bus_number_of_internal_bus_number(i);
        unsigned int index = get_dc_bus_converter_index_with_dc_bus_number(bus);
        if(index==INDEX_NOT_EXIST or
           (index!=INDEX_NOT_EXIST and
            current_active_power_control_mode[index]!=VSC_DC_VOLTAGE_CONTORL))
        {
            if(fabs(bus_power[i])<max_P_error_in_MW)
                continue;
            else
            {
                max_P_error_in_MW = fabs(bus_power[i]);
                max_P_error_physical_bus = bus;
            }
        }
    }
    string maxbusname = get_dc_bus_name(dc_bus_no2index(max_P_error_physical_bus));

    STEPS& toolkit = get_toolkit();
    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s: Maximum power mismatch found: %10.6fMW   at bus %u [%s].",
             get_compound_device_name().c_str(),
             max_P_error_in_MW,
             max_P_error_physical_bus,
             maxbusname.c_str());
    //toolkit.show_information_with_leading_time_stamp(buffer);
    return max_P_error_in_MW;
}

double VSC_HVDC::get_allowed_max_active_power_imbalance_in_MW()
{
    return P_threshold_in_MW;
}

void VSC_HVDC::set_convergence_flag(bool flag)
{
    converged = flag;
}

unsigned int VSC_HVDC::get_iteration_count()
{
    return iteration_count;
}

double VSC_HVDC::get_max_iteration()
{
    return max_iteration;
}

void VSC_HVDC::set_max_iteration(unsigned int iteration)
{
    max_iteration=iteration;
}

void VSC_HVDC::set_allowed_max_P_mismatch_in_MW(double max_mismatch)
{
    P_threshold_in_MW=max_mismatch;
}

void VSC_HVDC::build_jacobian()
{
    if(not is_dc_network_matrix_set())
        build_dc_network_matrix();

    unsigned int nbus = get_dc_bus_count();
    jacobian.clear();
    for(unsigned int i=0; i!=nbus-1; ++i)
        jacobian.add_entry(i, i, 0);

    unsigned int n = dc_network_matrix.get_matrix_entry_count();
    unsigned int row, column;
    unsigned int ibus, jbus;
    ostringstream osstream;

    unsigned int dc_slack_bus_internal_number = inphno.get_internal_bus_number_of_physical_bus_number(current_dc_slack_bus);

    for(unsigned int k=0; k!=n; ++k)
    {
        row = dc_network_matrix.get_row_number_of_entry_index(k);
        column = dc_network_matrix.get_column_number_of_entry_index(k);

        ibus = inphno.get_physical_bus_number_of_internal_bus_number(row);
        jbus = inphno.get_physical_bus_number_of_internal_bus_number(column);

        unsigned int iconverter_index = get_dc_bus_converter_index_with_dc_bus_number(ibus);
        unsigned int jconverter_index = get_dc_bus_converter_index_with_dc_bus_number(jbus);

        if(iconverter_index!=INDEX_NOT_EXIST and
           current_active_power_control_mode[iconverter_index]==VSC_DC_VOLTAGE_CONTORL)
            continue;
        if(jconverter_index!=INDEX_NOT_EXIST and
           current_active_power_control_mode[jconverter_index]==VSC_DC_VOLTAGE_CONTORL)
            continue;
        double der = calculate_jacobian_matrix_entry(k);

        if(row>dc_slack_bus_internal_number) --row;
        if(column>dc_slack_bus_internal_number) --column;

        jacobian.add_entry(row, column, der);
    }
    jacobian.compress_and_merge_duplicate_entries();
}

void VSC_HVDC::update_current_dc_slack_bus()
{
    current_dc_slack_bus = get_dc_slack_bus_number();
}

unsigned int VSC_HVDC::get_dc_slack_bus_number() const
{
    unsigned int nbus = get_dc_bus_count();
    for(unsigned int i=0; i<nbus; ++i)
    {
        unsigned int index = get_dc_bus_converter_index_with_dc_bus_index(i);
        if(index!=INDEX_NOT_EXIST and current_active_power_control_mode[index]==VSC_DC_VOLTAGE_CONTORL)
            return dc_bus_index2no(i);
    }
    // say some thing is error.
    return INDEX_NOT_EXIST;
}

unsigned int VSC_HVDC::get_converter_index_of_dc_voltage_control_priority(unsigned int p) const
{
    unsigned int nconverter = get_converter_count();
    for(unsigned int i=0; i!=nconverter; ++i)
    {
        if(get_converter_dc_voltage_control_priority(i)==p)
            return i;
    }
    return INDEX_NOT_EXIST;
}

double VSC_HVDC::calculate_jacobian_matrix_entry(unsigned int k)
{
    double der = 0.0;
    //cout<<"k: "<<k<<endl;
    int row = dc_network_matrix.get_row_number_of_entry_index(k);
    int column = dc_network_matrix.get_column_number_of_entry_index(k);
    unsigned int ibus=inphno.get_physical_bus_number_of_internal_bus_number(row);
    //cout<<"row: "<<row<<" column:"<<column<<" ibus:"<<ibus<<endl;

    double y, Udi, Udj, Idi_reference, Udi_reference, Kdi, Kpi;
    double alpha, beta;

    Udi = get_dc_voltage_of_dc_bus_number(ibus);
    unsigned int n_bus=get_dc_bus_count();
    unsigned int converter_index = get_dc_bus_converter_index_with_dc_bus_number(ibus);
    if(converter_index!=INDEX_NOT_EXIST)
    {
        Idi_reference = get_converter_initial_dc_current_reference_in_kA(converter_index);
        Udi_reference = get_converter_initial_dc_voltage_reference_in_kV(converter_index);
        Kdi = get_converter_initial_droop_coefficient_for_droop_control(converter_index);
        Kpi = get_converter_initial_droop_coefficient_for_droop_control(converter_index);
        VSC_HVDC_CONVERTER_ACTIVE_POWER_CONTROL_MODE mode = get_converter_active_power_operation_mode(converter_index);
        if(mode!=VSC_DC_ACTIVE_POWER_VOLTAGE_DROOP_CONTROL and mode!=VSC_DC_CURRENT_VOLTAGE_DROOP_CONTROL)
        {
            Kdi=1.0;
            Kpi=1.0;
        }
        alpha = get_converter_alpha(converter_index);
        beta = get_converter_beta(converter_index);

    }
    else
    {
        Idi_reference = 0;
        Udi_reference = 0;
        Kdi = 1;
        Kpi = 1;
        alpha = 0;
        beta = 0;
    }

    if(row!=column)
    {
        y = dc_network_matrix.get_real_entry_value(k);
        der = -y*Udi;
    }
    else
    {
        unsigned int k_start=dc_network_matrix.get_starting_index_of_column(row);
        unsigned int k_end=dc_network_matrix.get_starting_index_of_column(row+1);
        for(unsigned int m=k_start;m!=k_end;++m)
        {
            y = dc_network_matrix.get_real_entry_value(m);
            int temp_col=dc_network_matrix.get_column_number_of_entry_index(m);
            int temp_row=dc_network_matrix.get_row_number_of_entry_index(m);
            unsigned int jbus=inphno.get_physical_bus_number_of_internal_bus_number(temp_row);
            Udj = get_dc_voltage_of_dc_bus_number(jbus);
            if(temp_row!=temp_col)
                der+=(-Udj*y);
            else
                der+=(-2.0*Udi*y);

        }
        /*
        cout<<"der:::"<<der<<endl;
        double variable = alpha*Idi_reference-alpha*(Udi-Udi_reference)/Kdi-alpha*Udi/Kdi-beta/Kpi;
        cout<<"variable:: "<<variable<<endl;
        cout<<"alpha="<<alpha<<", beta="<<beta<<", Kdi="<<Kdi<<",Kpi="<<Kpi<<endl;
        */
        der+=alpha*Idi_reference-alpha*(Udi-Udi_reference)/Kdi-alpha*Udi/Kdi-beta/Kpi;
    }
    if(not std::isnan(der))
        ;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"NAN is detected when building Jacobian matrix for VSC_HVDC at row: "<<row<<"(bus "<<ibus<<"), col: "<<column<<"(bus unknown)";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
    return der;
}


void VSC_HVDC::update_converters_P_and_Q_to_AC_bus()
{
    unsigned int n = get_converter_count();
    /*for(unsigned int i=0; i!=n; ++i)
        cout<<"get_converter_Pdc: "<<get_converter_Pdc_command_in_MW(i)<<endl;
        */
    for(unsigned int index=0; index!=n; ++index)
        update_converter_P_and_Q_to_AC_bus(index);
}
void VSC_HVDC::update_converter_P_and_Q_to_AC_bus(unsigned int index)
{
    if(get_converter_status(index)==true)
    {
        double Pac=0.0;
        VSC_HVDC_CONVERTER_ACTIVE_POWER_CONTROL_MODE mode = current_active_power_control_mode[index];
        if(mode==VSC_DC_VOLTAGE_CONTORL or
           mode==VSC_DC_ACTIVE_POWER_VOLTAGE_DROOP_CONTROL or
           mode==VSC_DC_CURRENT_VOLTAGE_DROOP_CONTROL)
        {
            Pac=solve_converter_Pac_with_Pdc(index);
        }
        else
        {
            Pac=get_converter_nominal_ac_active_power_command_in_MW(index);
        }
        //cout<<"P_to_AC_bus_in_MW: "<<Pac<<endl;
        //cout<<"Q_to_AC_bus_in_MW: "<<get_converter_Q_to_AC_bus_in_MVar(index)<<endl;
        VSC_HVDC_CONVERTER_STRUCT* converter = get_converter(index);
        if(converter!=NULL)
        {
            if(converter->status==true)
                converter->P_to_AC_bus_MW=Pac;
        }
    }
}

void VSC_HVDC::update_dc_bus_voltage()
{
    unsigned int nbus=get_dc_bus_count();
    for(unsigned int index=0; index!=nbus;++index)
    {
        unsigned int dcbus = get_dc_bus_number(index);
        unsigned int current_dc_slack_bus_internal_number=inphno.get_internal_bus_number_of_physical_bus_number(current_dc_slack_bus);
        if(dcbus!=current_dc_slack_bus)
        {
            double Vdc = get_dc_bus_Vdc_in_kV(index);
            int i = inphno.get_internal_bus_number_of_physical_bus_number(dcbus);
            if(i<current_dc_slack_bus_internal_number)
                Vdc += Udc_mismatch[i];
            else
                Vdc += Udc_mismatch[i-1];
            //cout<<"Udc_mismatch[i-1]:"<<Udc_mismatch[i-1]<<endl;
            set_dc_bus_Vdc_in_kV(index,Vdc);
        }
    }
    /*
    for(unsigned int i=0; i!=nbus; ++i)
        cout<<"DC voltage of DC bus @ index "<<i<<": "<<get_dc_bus_Vdc_in_kV(i)<<endl;
    */
}

void VSC_HVDC::calculate_dc_active_power_of_slack_bus()
{
    double U_slack = get_dc_bus_Vdc_in_kV(dc_bus_no2index(current_dc_slack_bus));
    unsigned int islack = inphno.get_internal_bus_number_of_physical_bus_number(current_dc_slack_bus);
    double P_slack_dc_side=0.0;
    int nsize = dc_network_matrix.get_matrix_size();
    int k_start=dc_network_matrix.get_starting_index_of_column(islack);
    int k_end=dc_network_matrix.get_starting_index_of_column(islack+1);
    for(int k=k_start;k!=k_end;++k)
    {
        int row=dc_network_matrix.get_row_number_of_entry_index(k);
        double yij = dc_network_matrix.get_real_entry_value(k);
        unsigned int other_side_bus = inphno.get_physical_bus_number_of_internal_bus_number(row);
        P_slack_dc_side+=U_slack*yij*get_dc_bus_Vdc_in_kV(dc_bus_no2index(other_side_bus));
    }
    unsigned int converter_index=get_dc_bus_converter_index_with_dc_bus_index(dc_bus_no2index(current_dc_slack_bus));
    //cout<<"dc_slack_bus_active_power: "<<P_slack_dc_side<<endl;
    set_converter_Pdc_command_in_MW(converter_index, P_slack_dc_side);
}

bool VSC_HVDC::check_dc_slack_converter_constraint()
{
    unsigned int islack_bus = inphno.get_internal_bus_number_of_physical_bus_number(current_dc_slack_bus);
    double P_slack_dc_side = bus_power[islack_bus];
    unsigned int current_master_converter_index = get_dc_bus_converter_index_with_dc_bus_number(current_dc_slack_bus);
    set_converter_Pdc_command_in_MW(current_master_converter_index, P_slack_dc_side);
    unsigned int current_priority = get_converter_dc_voltage_control_priority(current_master_converter_index);
    double P_max=get_converter_Pmax_in_MW(current_master_converter_index);
    double P_min=get_converter_Pmin_in_MW(current_master_converter_index);
    if(P_slack_dc_side>P_max or P_slack_dc_side<P_min)
    {
        double Vdc_command = get_converter_nominal_dc_voltage_command_in_kV(current_master_converter_index);
        unsigned int reserve_converter_index = get_converter_index_of_dc_voltage_control_priority(current_priority+1);
        if(reserve_converter_index == INDEX_NOT_EXIST)
        {
            cout<<"Fatal Error. No reserver converter for changing DC voltage control mode."<<endl;
            return false;
        }
        else
        {
            current_active_power_control_mode[reserve_converter_index]=VSC_DC_VOLTAGE_CONTORL;
            set_converter_nominal_dc_voltage_command_in_kV(reserve_converter_index,Vdc_command);

            current_active_power_control_mode[current_master_converter_index]=VSC_AC_ACTIVE_POWER_CONTORL;
            if(P_slack_dc_side>P_max)
                set_converter_nominal_ac_active_power_command_in_MW(current_master_converter_index, P_max);
            else
                set_converter_nominal_ac_active_power_command_in_MW(current_master_converter_index, P_min);

            update_current_dc_slack_bus();
            return true;
        }
    }
    return false;
}

double VSC_HVDC::get_dc_voltage_of_dc_bus_number(unsigned int bus)
{
    return get_dc_bus_Vdc_in_kV(dc_bus_no2index(bus));
}

unsigned int VSC_HVDC::get_dc_bus_index_with_ac_bus_number(unsigned int bus)
{
    unsigned int n_dc_bus = get_dc_bus_count();
    unsigned int dc_bus_index=INDEX_NOT_EXIST;
    for(unsigned int i=0; i!=n_dc_bus; ++i)
    {
        ;
    }

}

int VSC_HVDC::get_converter_alpha(unsigned int index) const
{
    if(index!=INDEX_NOT_EXIST)
        return alpha[index];
    else
        return 0;
}

int VSC_HVDC::get_converter_beta(unsigned int index) const
{
    if(index!=INDEX_NOT_EXIST)
        return beta[index];
    else
        return 0;
}


int VSC_HVDC::get_alpha_of_dc_bus_number(unsigned int bus)
{
    unsigned int converter_index = get_dc_bus_converter_index_with_dc_bus_number(bus);
    if(converter_index!=INDEX_NOT_EXIST)
        return alpha[converter_index];
    else
        return 0;
}

int VSC_HVDC::get_beta_of_dc_bus_number(unsigned int bus)
{
    unsigned int converter_index = get_dc_bus_converter_index_with_dc_bus_number(bus);
    if(converter_index!=INDEX_NOT_EXIST)
        return beta[converter_index];
    else
        return 0;
}

unsigned int VSC_HVDC::get_index_of_dc_bus_number(unsigned int bus)
{
    unsigned int index = INDEX_NOT_EXIST;
    unsigned int n_dc_bus = dc_buses.size();
    for(unsigned int i=0; i<n_dc_bus; ++i)
    {
        if(dc_buses[i].dc_bus_number==bus)
        {
            index = i;
            break;
        }
    }
    return index;
}

double VSC_HVDC::solve_converter_Pac_with_Pdc(unsigned int converter_index)
{
    double Pdc_command = get_converter_Pdc_command_in_MW(converter_index);
    double Pdc0=-Pdc_command;
    double Pac_low = Pdc0*0.95;
    double Pac_high = Pdc0*1.05;
    double Qac = get_converter_Q_to_AC_bus_in_MVar(converter_index);
    /*
    cout<<"Pdc0: "<<Pdc0<<endl;
    cout<<"Pac_low: "<<Pac_low<<endl;
    cout<<"Pac_high: "<<Pac_high<<endl;
    cout<<"Qac: "<<Qac<<endl;
    */
    BUS *bus_pointer = get_converter_ac_bus_pointer(converter_index);
    double Pdc_low = calculate_converter_Pdc_with_Pac(converter_index, Pac_low, Qac);
    double Pdc_high = calculate_converter_Pdc_with_Pac(converter_index, Pac_high, Qac);
    //cout<<"Pdc_low: "<<Pdc_low<<endl;
    //cout<<"Pdc_high: "<<Pdc_high<<endl;

    //while(true)
    for(unsigned int i=0; i!=10; ++i)
    {
        //cout<<"i"<<i<<endl;
        double slope = (Pdc_high-Pdc_low)/(Pac_high-Pac_low);
        double delta_Pac = (Pdc_high-Pdc0)/slope;
        double Pac_new = Pac_high-delta_Pac;
        double Pdc_new = calculate_converter_Pdc_with_Pac(converter_index, Pac_new, Qac);
        if(fabs(Pdc_low-Pdc0)>fabs(Pdc_high-Pdc0))
        {
            Pdc_low = Pdc_new;
            Pac_low = Pac_new;
        }
        else
        {
            Pdc_high = Pdc_new;
            Pac_high = Pac_new;
        }
        //if(fabs(Pac_high-Pac_low)<get_allowed_max_active_power_imbalance_in_MW())
        if(fabs(Pdc_high-Pdc0)<get_allowed_max_active_power_imbalance_in_MW())
           return Pac_high;
        if(fabs(Pdc_low-Pdc0)<get_allowed_max_active_power_imbalance_in_MW())
           return Pac_low;
    }
    cout<<"VSC_HVDC::"<<__FUNCTION__<<" failed to converge."<<endl;
    return 0.5*(Pac_high+Pac_low);
}

double VSC_HVDC::calculate_converter_Pdc_with_Pac(unsigned int converter_index, double Pac, double Qac)
{
    complex<double> j(0.0,1.0);
    //cout<<"active_and_reactive_power_mode->"<<"active_power_command: "<<Pac_command<<" reactive_power_command: "<<Qac_command<<endl;
    BUS *bus_pointer = get_converter_ac_bus_pointer(converter_index);
    complex<double> Vac=bus_pointer->get_positive_sequence_complex_voltage_in_kV();
    //cout<<"get_bus_number: "<<bus_pointer->get_bus_number();
    //cout<<"Vac-> "<<Vac<<endl;
    double Vbase_ac=get_converter_transformer_AC_side_base_voltage_in_kV(converter_index);
    double Vbase_converter=get_converter_transformer_converter_side_base_voltage_in_kV(converter_index);
    double k=get_converter_transformer_off_nominal_turn_ratio(converter_index);
    double turn_ration=k*Vbase_ac/Vbase_converter;
    /*
    cout<<"Vbase_ac-> "<<Vbase_ac<<endl;
    cout<<"Vbase_converter-> "<<Vbase_converter<<endl;
    cout<<"k-> "<<k<<endl;
    cout<<"turn_ration-> "<<turn_ration<<endl;
    */
    complex<double> Yf_in_siemens=get_converter_filter_admittance_in_siemens(converter_index);
    complex<double> Zc_in_ohm=get_converter_commutating_impedance_in_ohm(converter_index);
    complex<double> Eac=Vac/turn_ration;
    complex<double> Zt_in_pu=get_converter_transformer_impedance_in_pu(converter_index);
    complex<double> Zt_in_ohm=Zt_in_pu*Vbase_converter*Vbase_converter/get_converter_transformer_capacity_in_MVA(converter_index);
    complex<double> Ic = (Pac-j*Qac)/(SQRT3*conj(Eac));
    complex<double> Vac_f=Eac+Ic*Zt_in_ohm*SQRT3;
                    Ic=Ic+Vac_f/SQRT3*Yf_in_siemens;
    complex<double> Vac_c=Vac_f+Ic*Zc_in_ohm*SQRT3;
    /*
    cout<<"Yf_in_siemens-> "<<Yf_in_siemens<<endl;
    cout<<"Zc_in_ohm-> "<<Zc_in_ohm<<endl;
    cout<<"Eac-> "<<Eac<<endl;
    cout<<"Zt_in_pu-> "<<Zt_in_pu<<endl;
    cout<<"Zt_in_ohm-> "<<Zt_in_ohm<<endl;
    cout<<"Vac_f-> "<<Vac_f<<endl;
    cout<<"Ic-> "<<Ic<<endl;
    cout<<"Vac_c-> "<<Vac_c<<endl;
    */
    double Pc_command=(SQRT3*Vac_c*conj(Ic)).real();
    double c_loss=get_converter_loss_factor_C_in_kW_per_amp_squard(converter_index);
    double b_loss=get_converter_loss_factor_B_in_kW_per_amp(converter_index);
    double a_loss=get_converter_loss_factor_A_in_kW(converter_index);
    double Ic_mag=abs(Ic)*1000;
    /*
    cout<<"c_loss-> "<<c_loss<<endl;
    cout<<"b_loss-> "<<b_loss<<endl;
    cout<<"a_loss-> "<<a_loss<<endl;
    */
    double P_converter_loss=(c_loss*Ic_mag*Ic_mag+b_loss*Ic_mag+a_loss)*0.001;
    //cout<<"P_converter_loss: "<<P_converter_loss<<endl;
    double Pdc=-Pc_command-P_converter_loss;
    return -Pdc;
}

void VSC_HVDC::save_dc_bus_powerflow_result_to_file(const string& filename) const
{
    ostringstream osstream;
    ofstream file(filename);
    if(file.is_open())
    {
        file<<"DC_BUS,AC_BUS,VOLTAGE"<<endl;

        //vector<BUS*> buses = psdb.get_all_buses();
        unsigned int nbus = get_dc_bus_count();
        for(unsigned int i=0; i!=nbus; ++i)
        {
            file<<"Udc["<<i<<"]"<<","
                <<setprecision(6)<<fixed<<get_converter_ac_bus_number_with_dc_bus_index(i)<<","
                <<setprecision(6)<<fixed<<get_dc_bus_Vdc_in_kV(i)<<endl;
        }
        file.close();
    }
    else
    {
        STEPS& toolkit = get_toolkit();
        osstream<<"File '"<<filename<<"' cannot be opened for saving bus powerflow result to file."<<endl
          <<"No bus powerflow result will be exported.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
}

bool VSC_HVDC::is_dc_network_matrix_set()
{
    unsigned int n = dc_network_matrix.get_matrix_size();
    if(n>=2) return true;
    else     return false;
}

bool VSC_HVDC::is_jacobian_matrix_set()
{
    unsigned int n = jacobian.get_matrix_size();
    if(n>=2) return true;
    else     return false;
}


bool VSC_HVDC::get_convergence_flag() const
{
    return converged;
}

void VSC_HVDC::export_dc_network_matrix(string filename)
{
    dc_network_matrix.save_matrix_to_file(filename);
}

void VSC_HVDC::show_dc_network_matrix()
{
    string buffer;
    char cbuffer[1000];

    int i=0, j=0, k=0;

    buffer="matrix entries:\n";
    show_information_with_leading_time_stamp_with_default_toolkit(buffer);

	snprintf(cbuffer, 1000, "row   rowbus     column    colbus     value\n");
    show_information_with_leading_time_stamp_with_default_toolkit(cbuffer);

    int n = dc_network_matrix.get_matrix_size();
    for(j=0;j!=n;++j)
    {
        int k_start = dc_network_matrix.get_starting_index_of_column(j);
        int k_end = dc_network_matrix.get_starting_index_of_column(j+1);
        unsigned int jbus = inphno.get_physical_bus_number_of_internal_bus_number(j);
        for(k=k_start;k<k_end;++k)
        {
            i = dc_network_matrix.get_row_number_of_entry_index(k);
            unsigned int ibus = inphno.get_physical_bus_number_of_internal_bus_number(i);

            snprintf(cbuffer,1000, "%-6d, %-6d, %-6d, %-6d, % 10.6f\n",i,ibus,j,jbus,dc_network_matrix.get_real_entry_value(k));

            buffer = cbuffer;
            show_information_with_leading_time_stamp_with_default_toolkit(buffer);
        }
    }
}


void VSC_HVDC::show_jacobian_matrix()
{
    string buffer;
    char cbuffer[1000];

    int i=0, j=0, k=0;

    buffer="matrix entries:\n";
    show_information_with_leading_time_stamp_with_default_toolkit(buffer);

	snprintf(cbuffer, 1000, "row   rowbus     column    colbus     value\n");
    show_information_with_leading_time_stamp_with_default_toolkit(cbuffer);

    int n = jacobian.get_matrix_size();
    for(j=0;j!=n;++j)
    {
        int k_start = jacobian.get_starting_index_of_column(j);
        int k_end = jacobian.get_starting_index_of_column(j+1);
        unsigned int jbus = inphno.get_physical_bus_number_of_internal_bus_number(j);
        for(k=k_start;k<k_end;++k)
        {
            i = jacobian.get_row_number_of_entry_index(k);
            unsigned int ibus = inphno.get_physical_bus_number_of_internal_bus_number(i);

            snprintf(cbuffer,1000, "%-6d, %-6d, %-6d, %-6d, % 10.6f\n",i,ibus,j,jbus,jacobian.get_real_entry_value(k));

            buffer = cbuffer;
            show_information_with_leading_time_stamp_with_default_toolkit(buffer);
        }
    }
}

double VSC_HVDC::get_dc_network_matrix_entry_between_dc_bus(unsigned int ibus, unsigned int jbus)
{
    if(is_dc_network_matrix_set())
    {
        int i = inphno.get_internal_bus_number_of_physical_bus_number(ibus);
        int j = inphno.get_internal_bus_number_of_physical_bus_number(jbus);
        return dc_network_matrix.get_real_entry_value(i, j);
    }
    else
        return 0.0;
}

double VSC_HVDC::get_jacobian_matrix_entry_between_dc_bus(unsigned int ibus, unsigned int jbus)
{
    if(is_jacobian_matrix_set())
    {
        int i = inphno.get_internal_bus_number_of_physical_bus_number(ibus);
        int j = inphno.get_internal_bus_number_of_physical_bus_number(jbus);
        return jacobian.get_real_entry_value(i, j);
    }
    else
        return 0.0;
}

void VSC_HVDC::show_inphno_bus_number()
{
    unsigned int n_bus=get_dc_bus_count();
    for(unsigned int index=0;index!=n_bus;++index)
        cout<<"inphno_bus_number: "<<index<<" physical_bus_number: "<<inphno.get_physical_bus_number_of_internal_bus_number(index)<<endl;
}

void VSC_HVDC::export_dc_bus_voltage_with_network_ordering()
{
    unsigned int n_bus=get_dc_bus_count();
    for(unsigned int index=0;index!=n_bus;++index)
    {
        unsigned int dc_bus_number=inphno.get_physical_bus_number_of_internal_bus_number(index);
        double Vdc=get_dc_bus_Vdc_in_kV(dc_bus_no2index(dc_bus_number));
        cout<<"dc_bus_voltage:"<<Vdc<<endl;
    }
}

double VSC_HVDC::get_converter_ac_voltage_in_pu_with_ac_bus_number(unsigned int bus)
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    BUS* busptr = psdb.get_bus(bus);
    return busptr->get_positive_sequence_voltage_in_pu();
}

double VSC_HVDC::get_converter_ac_voltage_in_pu_with_converter_index(unsigned int index)
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    unsigned int bus = get_converter_ac_bus(index);
    BUS* busptr = psdb.get_bus(bus);
    return busptr->get_positive_sequence_voltage_in_pu();
}

double VSC_HVDC::get_converter_ac_voltage_in_kV_with_ac_bus_number(unsigned int bus)
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    BUS* busptr = psdb.get_bus(bus);
    return busptr->get_positive_sequence_voltage_in_kV();
}

complex<double> VSC_HVDC::get_converter_ac_current_in_kA_with_ac_bus_number(unsigned int bus)
{
    complex<double> j(0.0,1.0);
    unsigned int converter_index = get_converter_index_with_ac_bus(bus);
    double Pac = get_converter_P_to_AC_bus_in_MW(converter_index);
    double Qac = get_converter_Q_to_AC_bus_in_MVar(converter_index);
    //cout<<"active_and_reactive_power_mode->"<<"active_power_command: "<<Pac_command<<" reactive_power_command: "<<Qac_command<<endl;
    BUS *bus_pointer = get_converter_ac_bus_pointer(converter_index);
    complex<double> Vac=bus_pointer->get_positive_sequence_complex_voltage_in_kV();
    complex<double> Iac=(Pac-j*Qac)/conj(Vac);
    return Iac;
}

double VSC_HVDC::get_converter_dc_voltage_in_kV_with_ac_bus_number(unsigned int bus)
{
    unsigned int dc_bus_index=get_dc_bus_index_with_ac_bus_number(bus);
    double dc_bus_voltage=get_dc_bus_Vdc_in_kV(dc_bus_index);
    return dc_bus_voltage;
}


double VSC_HVDC::get_converter_dc_current_in_kA_with_ac_bus_number(unsigned int bus)
{
    unsigned int converter_index=get_converter_index_with_ac_bus(bus);
    unsigned int dc_bus_index=get_dc_bus_index_with_ac_bus_number(bus);
    double Pdc = get_converter_Pdc_command_in_MW(converter_index);
    double dc_bus_voltage = get_dc_bus_Vdc_in_kV(dc_bus_index);
    double Idc = Pdc/dc_bus_voltage;
    return Idc;
}
double VSC_HVDC::get_converter_dc_power_in_MW_with_ac_bus_number(unsigned int bus)
{
    unsigned int converter_index=get_converter_index_with_ac_bus(bus);
    double Pdc = get_converter_Pdc_command_in_MW(converter_index);
    return Pdc;
}
double VSC_HVDC::get_converter_ac_active_power_in_MW_with_ac_bus_number(unsigned int bus)
{
    unsigned int converter_index=get_converter_index_with_ac_bus(bus);
    double Pac=get_converter_P_to_AC_bus_in_MW(converter_index);
    return Pac;
}
double VSC_HVDC::get_converter_ac_reactive_power_in_MVar_with_ac_bus_number(unsigned int bus)
{
    unsigned int converter_index=get_converter_index_with_ac_bus(bus);
    double Qac=get_converter_Q_to_AC_bus_in_MVar(converter_index);
    return Qac;
}

complex<double> VSC_HVDC::get_converter_dynamic_current_in_pu_based_on_system_base_power(unsigned int converter_index)
{
    if(get_status() == true)
    {
        VSC_HVDC_MODEL* model = get_vsc_hvdc_model();
        if(model!=NULL)
            return model->get_converter_ac_current_in_pu(converter_index);
        else
            return 0.0;
    }
    else
        return 0.0;
}


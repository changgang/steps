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
    clear();
}
VSC_HVDC::VSC_HVDC(const VSC_HVDC& vsc) : NONBUS_DEVICE(vsc.get_toolkit())
{
    clear();

    copy_from_const_vsc(vsc);
}

VSC_HVDC& VSC_HVDC::operator=(const VSC_HVDC& vsc)
{
    if(this==&vsc) return *this; // to avoid self assignment

    clear();

    copy_from_const_vsc(vsc);

    return *this;
}

void VSC_HVDC::copy_from_const_vsc(const VSC_HVDC& vsc)
{
    set_toolkit(vsc.get_toolkit());

    set_name(vsc.get_name());
    set_identifier(vsc.get_identifier());
    set_converter_count(vsc.get_converter_count());
    set_dc_bus_count(vsc.get_dc_bus_count());
    set_dc_line_count(vsc.get_dc_line_count());
    set_status(vsc.get_status());
    set_ac_converter_bus_with_dc_voltage_control(vsc.get_ac_converter_bus_with_dc_voltage_control());
    unsigned int ncon=vsc.get_converter_count();
    unsigned int nbus=vsc.get_dc_bus_count();
    unsigned int ndc_line=vsc.get_dc_line_count();
    for(unsigned int i=0;i!=ncon;++i)
    {
        set_converter_ac_bus(i,vsc.get_converter_ac_bus(i));
        if(vsc.get_converter_active_power_operation_mode(i)==1)
        {
            set_converter_active_power_operation_mode(i,VSC_DC_VOLTAGE_CONTORL);
            set_converter_nominal_dc_voltage_command_in_kV(i,vsc.get_converter_nominal_dc_voltage_command_in_kV(i));
        }

        else
        {
            set_converter_active_power_operation_mode(i,VSC_AC_ACTIVE_POWER_CONTORL);
            set_converter_nominal_ac_active_power_command_in_MW(i,vsc.get_converter_nominal_ac_active_power_command_in_MW(i));
        }

        if(vsc.get_converter_reactive_power_operation_mode(i)==1)
        {
            set_converter_reactive_power_operation_mode(i,VSC_AC_VOLTAGE_CONTROL);
            set_converter_nominal_ac_voltage_command_in_pu(i,vsc.get_converter_nominal_ac_voltage_command_in_pu(i));
        }

        else
        {
            set_converter_reactive_power_operation_mode(i,VSC_AC_REACTIVE_POWER_CONTROL);
            set_converter_nominal_ac_reactive_power_command_in_Mvar(i,vsc.get_converter_nominal_ac_reactive_power_command_in_Mvar(i));
        }
        set_converter_loss_factor_A_in_kW(i,vsc.get_converter_loss_factor_A_in_kW(i));
        set_converter_loss_factor_B_in_kW_per_amp(i,vsc.get_converter_loss_factor_B_in_kW_per_amp(i));
        set_converter_loss_factor_C_in_KW_per_amp_squared(i,vsc.get_converter_loss_factor_C_in_kW_per_amp_squard(i));
        set_converter_minimum_loss_in_kW(i,vsc.get_converter_minimum_loss_in_kW(i));
        set_converter_rated_capacity_in_MVA(i,vsc.get_converter_rated_capacity_in_MVA(i));
        set_converter_rated_current_in_A(i,vsc.get_converter_rated_current_in_A(i));
        set_converter_Qmax_in_MVar(i,vsc.get_converter_Qmax_in_MVar(i));
        set_converter_Qmin_in_MVar(i,vsc.get_converter_Qmin_in_MVar(i));
        set_converter_remote_bus_to_regulate(i,vsc.get_converter_remote_bus_to_regulate(i));
        set_converter_remote_regulation_percent(i,vsc.get_converter_remote_regulation_percent(i));

    }

    for(unsigned int i=0;i!=nbus;++i)
    {
        set_dc_bus_converter_index_with_ac_bus_number(i,vsc.get_dc_bus_number(i));
        set_dc_bus_converter_index_with_ac_bus_number(i,vsc.get_converter_ac_bus_number_with_dc_bus_index(i));
        set_dc_bus_area(i,vsc.get_dc_bus_area(i));
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


}

VSC_HVDC::~VSC_HVDC()
{
    clear();
}

void VSC_HVDC::clear()
{
    set_identifier("");
    set_name("");
    set_status(false);
    ac_converter_bus_with_dc_voltage_control = 0;
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
    for(unsigned int i=0; i<n; ++i)
        dc_buses.push_back(dc_bus);
}

void VSC_HVDC::set_dc_line_count(unsigned int n)
{
    dc_lines.clear();

    if(n<1) n = 1;

    dc_lines.reserve(n);
    VSC_HVDC_DC_LINE_STRUCT dc_line;
    for(unsigned int i=0; i<n; ++i)
        dc_lines.push_back(dc_line);
}

void VSC_HVDC::set_status(const bool status)
{
    this->status = status;
}

void VSC_HVDC::set_reserve_master_converter_ac_bus(unsigned int ac_bus)
{
    ostringstream osstream;
    if(ac_bus!=0)
    {
        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

        if(psdb.is_bus_exist(ac_bus))
        {
            ac_converter_bus_with_reserve_master_control = ac_bus;
        }
        else
        {
            osstream<<"Bus "<<ac_bus<<" does not exist for setting up converter AC bus with reserve master of vsc-hvdc link."<<endl
                    <<"0 will be set to indicate invalid reserve master converter.";
            toolkit.show_information_with_leading_time_stamp(osstream);
            ac_converter_bus_with_reserve_master_control = 0;
        }
    }
    else
    {
        osstream<<"Warning. Zero bus number (0) is set up."<<endl
                <<"0 will be set to indicate no reserve master converter in this VSC_HVDC project.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        ac_converter_bus_with_reserve_master_control = ac_bus;
    }
}

void VSC_HVDC::set_ac_converter_bus_with_dc_voltage_control(const unsigned int bus)
{
    ostringstream osstream;
    if(bus!=0)
    {
        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

        if(psdb.is_bus_exist(bus))
        {
            ac_converter_bus_with_dc_voltage_control = bus;
        }
        else
        {
            osstream<<"Bus "<<bus<<" does not exist for setting up converter AC bus with dc voltage of vsc-hvdc link."<<endl
                    <<"0 will be set to indicate invalid vsc-hvdc link.";
            toolkit.show_information_with_leading_time_stamp(osstream);
            ac_converter_bus_with_dc_voltage_control = 0;
        }
    }
    else
    {
        osstream<<"Warning. Zero bus number (0) is not allowed for setting up converter AC bus with dc voltage of vsc-hvdc link."<<endl
                <<"0 will be set to indicate invalid vsc-hvdc link.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        ac_converter_bus_with_dc_voltage_control = bus;
    }
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
        osstream<<"VSC_HVDC converter index "<<index<<" exceeds converter count"<<get_converter_count()
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
    if(mode == VSC_DC_VOLTAGE_CONTORL and get_ac_converter_bus_with_dc_voltage_control()==0)
    {
        unsigned int acbus = get_converter_ac_bus(index);
        set_ac_converter_bus_with_dc_voltage_control(acbus);
    }
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
void VSC_HVDC::set_converter_initial_power_voltage_droop_coefficient(const unsigned int index, const double droop)
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

void VSC_HVDC::set_converter_initial_current_voltage_droop_coefficient(const unsigned int index, const double droop)
{
    set_converter_initial_power_voltage_droop_coefficient(index, droop);
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

unsigned int VSC_HVDC::get_ac_converter_bus_with_dc_voltage_control() const
{
    return ac_converter_bus_with_dc_voltage_control;
}

unsigned int VSC_HVDC::get_reserve_master_converter_ac_bus() const
{
    return ac_converter_bus_with_reserve_master_control;
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

double VSC_HVDC::get_converter_initial_power_voltage_droop_coefficient(const unsigned int index)const
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

double VSC_HVDC::get_converter_initial_current_voltage_droop_coefficient(const unsigned int index)const
{
    return get_converter_initial_power_voltage_droop_coefficient(index);
    /*
    if(index<get_converter_count())
        return converters[index].initial_power_current_droop_coefficient;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count when calling VSC_HVDC::"<<__FUNCTION__<<"()."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }*/
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
        return INDEX_NOT_EXIST;

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
        osstream<<"Error. index ("<<index<<") is out of  maximal dc line count."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
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
    unsigned int ncon=get_converter_count();
    unsigned int nbus=get_dc_bus_count();
    unsigned int ndc_line=get_dc_line_count();
    for(unsigned int i=0;i!=ncon;++i)
    {
        cout<<"get_converter_ac_bus("<<i<<"): "<<get_converter_ac_bus(i)<<endl;
        cout<<"get_converter_status("<<i<<"): "<<get_converter_status(i)<<endl;
        cout<<"get_converter_active_power_operation_mode("<<i<<"): "<<get_converter_active_power_operation_mode(i)<<endl;
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
        cout<<"get_dc_bus_name("<<i<<"): "<<get_dc_bus_name(i)<<endl;
        cout<<"get_dc_bus_area("<<i<<"): "<<get_dc_bus_area(i)<<endl;
        cout<<"get_dc_bus_zone("<<i<<"): "<<get_dc_bus_zone(i)<<endl;
        cout<<"get_dc_bus_owner_number("<<i<<"): "<<get_dc_bus_owner_number(i)<<endl;
        cout<<"get_dc_bus_ground_resistance_in_ohm("<<i<<"): "<<get_dc_bus_ground_resistance_in_ohm(i)<<endl;
        cout<<"get_dc_bus_generation_power_in_MW("<<i<<"): "<<get_dc_bus_generation_power_in_MW(i)<<endl;
        cout<<"get_dc_bus_load_power_in_MW("<<i<<"): "<<get_dc_bus_load_power_in_MW(i)<<endl;
        cout<<"get_dc_bus_Vdc_in_kV("<<i<<"): "<<get_dc_bus_Vdc_in_kV(i)<<endl;
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
        vsc_hvdc_model = model;
}

VSC_HVDC_MODEL* VSC_HVDC::get_vsc_hvdc_model() const
{
    return vsc_hvdc_model;
}

//****************solve dc network under hybrid control strategy*********************//


void VSC_HVDC::solve_steady_state()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    iteration_count=0;
    build_inphno();
    build_dc_network_matrix();
    initialize_Udc_vector();
    initialize_alpha_vector();
    initialize_beta_vector();
    initialize_active_and_reactive_power_control_mode_vector();
    while(true)
    {
        //try to restore initial VDC control bus. reset initial P commands
        cout<<"ieteration_count: "<<iteration_count<<endl;
        build_dc_bus_power_mismatch_vector();
        max_P_mismatch_in_MW = get_maximum_active_power_mismatch_in_MW();
        cout<<"max_P_mismatch_in_MW:"<<max_P_mismatch_in_MW<<endl;
        //cout<<"allowed_max_active_power_imbalance_in_MW:"<<get_allowed_max_active_power_imbalance_in_MW()<<endl;
        bool dc_bus_control_mode_changed=false;
        dc_bus_control_mode_changed=check_dc_bus_control_mode();
        if(dc_bus_control_mode_changed)
        {
            /*
            cout<<"dc_bus_control_mode_changed!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
            re_build_inphno();
            build_dc_network_matrix();
            re_initialize_Udc_vector();
            re_build_dc_bus_power_mismatch_vector();
            */
        }
        if(max_P_mismatch_in_MW<get_allowed_max_active_power_imbalance_in_MW())
        {
            set_convergence_flag(true);
            osstream<<"vsc powerflow converged within ("<<iteration_count<<") iterations."<<endl;
            toolkit.show_information_with_leading_time_stamp(osstream);
            break;
        }
        else
        {
            set_convergence_flag(false);
        }

        if(get_iteration_count()>=get_max_iteration())
        {
            osstream<<"Vsc hvdc powerflow failed to converge within ("<<get_max_iteration()<<") iterations."<<endl;
            toolkit.show_information_with_leading_time_stamp(osstream);
            break;
        }
        build_jacobian();
        show_jacobian_matrix();
        Udc_mismatch = P_mismatch/jacobian;
        if(jacobian.is_lu_factorization_successful())
        {
            update_dc_bus_voltage();
            //calculate_dc_active_power_of_slack_bus();
            //check_dc_slack_converter_constraint();  //should check if VDC bus should be replaced
            ++iteration_count;
        }
        else
        {
            osstream<<"Vsc hvdc No further powerflow solution will be attempted since LU factorization of N-R Jacobian matrix is failed."<<endl;
            toolkit.show_information_with_leading_time_stamp(osstream);
            break;
        }
    }
    save_dc_bus_powerflow_result_to_file("Vsc_hvdc_bus_powerflow_result.csv");
    //update_converters_P_and_Q_to_AC_bus();

}

void VSC_HVDC::build_dc_network_matrix()
{
    if(inphno.get_table_size()<2)
        build_inphno();

    dc_network_matrix.clear();
    build_initial_zero_matrix();
    add_dc_lines_to_dc_network();
    dc_network_matrix.compress_and_merge_duplicate_entries();
}

void VSC_HVDC::build_inphno()
{
    unsigned int dc_bus_controlling_dc_voltage = 0, dc_bus_index = 0;
    unsigned int n = get_dc_bus_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        unsigned int converter_index = get_dc_bus_converter_index_with_dc_bus_index(i);
        if(converter_index!=INDEX_NOT_EXIST)
        {
            VSC_HVDC_CONVERTER_ACTIVE_POWER_CONTROL_MODE mode = get_converter_active_power_operation_mode(converter_index);
            if(mode == VSC_DC_VOLTAGE_CONTORL)
            {
                dc_bus_controlling_dc_voltage = get_dc_bus_number(i);
                dc_bus_index = i;
                break;
            }
        }
    }
    if(dc_bus_controlling_dc_voltage == 0)
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. no dc bus controlling dc voltage."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        unsigned int bus = get_dc_bus_number(i);
        if(i<dc_bus_index)
            inphno.set_physical_internal_bus_number_pair(bus, i);
        else
        {
            if(i>dc_bus_index)
                inphno.set_physical_internal_bus_number_pair(bus, i-1);
            else
                inphno.set_physical_internal_bus_number_pair(bus, n-1);
        }
    }
}

void VSC_HVDC::re_build_inphno()
{
    unsigned int dc_bus_controlling_dc_voltage = 0, dc_bus_index = 0;
    unsigned int n = get_dc_bus_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        unsigned int converter_index = get_dc_bus_converter_index_with_dc_bus_index(i);
        if(converter_index!=INDEX_NOT_EXIST)
        {
            VSC_HVDC_CONVERTER_ACTIVE_POWER_CONTROL_MODE mode = current_active_power_control_mode[converter_index];
            if(mode == VSC_DC_VOLTAGE_CONTORL)
            {
                dc_bus_controlling_dc_voltage = get_dc_bus_number(i);
                dc_bus_index = i;
                break;
            }
        }
    }
    if(dc_bus_controlling_dc_voltage == 0)
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. no dc bus controlling dc voltage."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        unsigned int bus = get_dc_bus_number(i);
        if(i<dc_bus_index)
            inphno.set_physical_internal_bus_number_pair(bus, i);
        else
        {
            if(i>dc_bus_index)
                inphno.set_physical_internal_bus_number_pair(bus, i-1);
            else
                inphno.set_physical_internal_bus_number_pair(bus, n-1);
        }
    }
}

void VSC_HVDC::build_initial_zero_matrix()
{
    unsigned int n = get_dc_bus_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        dc_network_matrix.add_entry(i, i, 0);
    }
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

void VSC_HVDC::initialize_Udc_vector()
{
    if(inphno.get_table_size()>=2)
    {
        unsigned int nbus = get_dc_bus_count();
        Udc.clear();
        Udc.reserve(nbus);
        for(unsigned int i=0; i<nbus; ++i)
            Udc.push_back(0.0);

        for (unsigned int index=0; index!=nbus; ++index)
        {
            unsigned int dcbus = get_dc_bus_number(index);
            int i = inphno.get_internal_bus_number_of_physical_bus_number(dcbus);
            if(i!=nbus-1)
            {
                double Vdc=get_dc_network_base_voltage_in_kV();
                set_dc_bus_Vdc_in_kV(index, Vdc);
                Udc[i] = Vdc;
            }

            else
            {
                unsigned int converter_index = get_dc_bus_converter_index_with_dc_bus_number(dcbus);
                if(converter_index!=INDEX_NOT_EXIST)
                {
                    double Vdc=get_converter_nominal_dc_voltage_command_in_kV(converter_index);
                    set_dc_bus_Vdc_in_kV(index,Vdc);
                    Udc[i] =Vdc;
                }
                else
                {
                    double Vdc=get_dc_network_base_voltage_in_kV();
                    set_dc_bus_Vdc_in_kV(index,Vdc);
                    Udc[i] =Vdc;
                }
            }
        }
        for(unsigned int i=0;i!=nbus;++i)
            cout<<"Udc[i]"<<Udc[i]<<endl;
    }
}

void VSC_HVDC::re_initialize_Udc_vector()
{
    if(inphno.get_table_size()>=2)
    {
        unsigned int nbus = get_dc_bus_count();
        Udc.clear();
        Udc.reserve(nbus);
        for(unsigned int i=0; i<nbus; ++i)
            Udc.push_back(0.0);

        for (unsigned int index=0; index!=nbus; ++index)
        {
            unsigned int dcbus = get_dc_bus_number(index);
            int i = inphno.get_internal_bus_number_of_physical_bus_number(dcbus);
            if(i!=nbus-1)
            {
                double Vdc=get_dc_network_base_voltage_in_kV();
                set_dc_bus_Vdc_in_kV(index, Vdc);
                Udc[i] = Vdc;
            }

            else
            {
                unsigned int converter_index = get_dc_bus_converter_index_with_dc_bus_number(dcbus);
                if(converter_index!=INDEX_NOT_EXIST)
                {
                    double Vdc=0.0;
                    unsigned int n_converter=get_converter_count();
                    for(unsigned int index=0;index!=n_converter;++i)
                    {
                        if(get_converter_active_power_operation_mode(index)==VSC_DC_VOLTAGE_CONTORL)
                            Vdc=get_converter_nominal_dc_voltage_command_in_kV(index);
                    }
                    set_dc_bus_Vdc_in_kV(index,Vdc);
                    Udc[i] =Vdc;
                }
                else
                {
                    double Vdc=get_dc_network_base_voltage_in_kV();
                    set_dc_bus_Vdc_in_kV(index,Vdc);
                    Udc[i] =Vdc;
                }
            }
        }
        for(unsigned int i=0;i!=nbus;++i)
            cout<<"get_dc_bus_Vdc:"<<get_dc_bus_Vdc_in_kV(i)<<endl;
    }
}

void VSC_HVDC::initialize_alpha_vector()
{
    unsigned int n_converter = get_converter_count();
    alpha.clear();
    alpha.reserve(n_converter);
    for(unsigned int i=0; i<n_converter; ++i)
    {
        cout<<"operation_mode:"<<get_converter_active_power_operation_mode(i)<<endl;
        if(get_converter_active_power_operation_mode(i) == VSC_DC_CURRENT_VOLTAGE_DROOP_CONTROL)
            alpha.push_back(1);
        else
            alpha.push_back(0);
    }
    for(unsigned int i=0; i!=n_converter;++i)
    {
        cout<<"alpha[i]:"<<alpha[i]<<endl;
    }
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
    for(unsigned int i=0; i!=n_converter;++i)
    {
        cout<<"beta[i]:"<<beta[i]<<endl;
    }
}

void VSC_HVDC::initialize_active_and_reactive_power_control_mode_vector()
{
    unsigned int n_converter=get_converter_count();
    current_active_power_control_mode.clear();
    current_active_power_control_mode.reserve(n_converter);
    current_reactive_power_control_mode.clear();
    current_reactive_power_control_mode.reserve(n_converter);
    cout<<"current_active_power_control_mode.reserve(n_converter)"<<endl;
    for(unsigned int index=0; index!=n_converter;++index)
    {
        VSC_HVDC_CONVERTER_ACTIVE_POWER_CONTROL_MODE active_power_control_mode=get_converter_active_power_operation_mode(index);
        VSC_HVDC_CONVERTER_REACTIVE_POWER_CONTROL_MODE reactive_power_control_mode=get_converter_reactive_power_operation_mode(index);
        cout<<"active_power_control_mode: "<<active_power_control_mode<<endl;
        cout<<"reactive_power_control_mode: "<<reactive_power_control_mode<<endl;
        current_active_power_control_mode[index]=active_power_control_mode;
        current_reactive_power_control_mode[index]=reactive_power_control_mode;
    }
}

void VSC_HVDC::build_dc_bus_power_mismatch_vector()
{
    unsigned int nbus=get_dc_bus_count();
    P_mismatch.clear();
    P_mismatch.reserve(nbus);
    for(unsigned int i=0; i<nbus-1; ++i)
        P_mismatch.push_back(0.0);
    build_Pdc_command_vector();
    update_raw_dc_current_into_dc_network();
    calculate_raw_dc_power_into_dc_network();

    add_Pdc_command_to_P_mismatch_vector();
    add_generation_power_to_P_mismatch_vector();
    add_load_power_to_P_mismatch_vector();
    add_raw_dc_power_to_P_mismatch_vector();
}

void VSC_HVDC::re_build_dc_bus_power_mismatch_vector()
{
    unsigned int nbus=get_dc_bus_count();
    P_mismatch.clear();
    P_mismatch.reserve(nbus);
    for(unsigned int i=0; i<nbus-1; ++i)
        P_mismatch.push_back(0.0);
    re_build_Pdc_command_vector();
    update_raw_dc_current_into_dc_network();
    calculate_raw_dc_power_into_dc_network();

    add_Pdc_command_to_P_mismatch_vector();
    add_generation_power_to_P_mismatch_vector();
    add_load_power_to_P_mismatch_vector();
    add_raw_dc_power_to_P_mismatch_vector();
}

void VSC_HVDC::build_Pdc_command_vector()
{
    double Pdc=0.0;
    unsigned int nbus = get_dc_bus_count();
    Pdc_command.clear();
    Pdc_command.reserve(nbus);
    for(unsigned int i=0;i!=nbus;++i)
        Pdc_command.push_back(0.0);
    for(unsigned int index=0; index!=nbus; ++index)
    {
        unsigned int dcbus = get_dc_bus_number(index);
        int i = inphno.get_internal_bus_number_of_physical_bus_number(dcbus);
        unsigned int converter_index = get_dc_bus_converter_index_with_dc_bus_number(dcbus);
        if(converter_index!=INDEX_NOT_EXIST)
        {
            VSC_HVDC_CONVERTER_ACTIVE_POWER_CONTROL_MODE active_power_mode = get_converter_active_power_operation_mode(converter_index);
            VSC_HVDC_CONVERTER_REACTIVE_POWER_CONTROL_MODE reactive_power_mode = get_converter_reactive_power_operation_mode(converter_index);
            switch(active_power_mode)
            {
                case VSC_AC_ACTIVE_POWER_CONTORL:
                    if(reactive_power_mode==VSC_AC_REACTIVE_POWER_CONTROL)
                    {
                        Pdc=solve_Pdc_with_active_power_control_and_reactive_power_control(converter_index);
                        Pdc_command[i]=Pdc;
                        break;
                    }
                    else
                    {
                        Pdc=solve_Pdc_with_active_power_control_and_ac_voltage_control(converter_index);
                        Pdc_command[i]=Pdc;
                        break;
                    }
                case VSC_AC_VOLTAGE_ANGLE_CONTROL:
                    if(reactive_power_mode==VSC_AC_REACTIVE_POWER_CONTROL)
                    {
                        Pdc=solve_Pdc_with_voltage_angle_and_reactive_power_control(converter_index);
                        Pdc_command[i]=Pdc;
                        break;
                    }
                    else
                    {
                        Pdc=solve_Pdc_with_voltage_angle_and_voltage_control(converter_index);
                        Pdc_command[i]=Pdc;
                        break;
                    }
                    /*
                    double Pac_command = 0;
                    if(mode==VSC_AC_ACTIVE_POWER_CONTORL)
                        Pac_command = get_converter_nominal_ac_active_power_command_in_MW(converter_index);
                    else
                        Pac_command = get_converter_P_to_AC_bus_in_MW(converter_index);
                    double Qac_command = get_converter_Q_to_AC_bus_in_MVar(converter_index);
                    BUS* bus_pointer = get_converter_ac_bus_pointer(converter_index);
                    double Vac = bus_pointer->get_positive_sequence_voltage_in_kV();
                    double Vbase_ac = get AC side base voltage
                    double Vbase_converter = get converter side base voltage
                    double k = get off-nominal turn ratio
                    double turn_ratio = k*Vbase_ac/Vbase_converter

                    double Eac = Vac/turn_ratio;

                    if(transformer, filter, commutating inductance are considered)
                    {
                        convert transformer Zt, filter Y, commutating ZL in Y form to Pi form
                        caution: what if one or two of (Zt, Y, ZL) is zero.
                        direct calculate current into converter
                    }
                    else
                    {
                        simplified
                    }
                    calculate loss

                    Pdc_command[i]=
                    break;
                    */
                case VSC_DC_ACTIVE_POWER_VOLTAGE_DROOP_CONTROL:
                    Pdc=solve_Pdc_with_dc_active_power_voltage_droop_control(converter_index);
                    Pdc_command[i]=Pdc;
                    break;
                case VSC_DC_CURRENT_VOLTAGE_DROOP_CONTROL:
                    Pdc=solve_Pdc_with_dc_current_voltage_droop_control(converter_index);
                    Pdc_command[i]=Pdc;
                    break;
                case VSC_DC_VOLTAGE_CONTORL:
                    Pdc_command[i] = 0.0;
                    break;
                default:
                    break;
            }
        }
        else
            Pdc_command[i]=0.0;
    }
    for(unsigned int index=0; index!=nbus;++index)
        cout<<"Pdc_command["<<index<<"]:"<<Pdc_command[index]<<endl;
}

void VSC_HVDC::re_build_Pdc_command_vector()
{
    double Pdc=0.0;
    unsigned int nbus = get_dc_bus_count();
    Pdc_command.clear();
    Pdc_command.reserve(nbus);
    for(unsigned int i=0;i!=nbus;++i)
        Pdc_command.push_back(0.0);
    for(unsigned int index=0; index!=nbus; ++index)
    {
        unsigned int dcbus = get_dc_bus_number(index);
        int i = inphno.get_internal_bus_number_of_physical_bus_number(dcbus);
        unsigned int converter_index = get_dc_bus_converter_index_with_dc_bus_number(dcbus);
        if(converter_index!=INDEX_NOT_EXIST)
        {
            VSC_HVDC_CONVERTER_ACTIVE_POWER_CONTROL_MODE active_power_mode = current_active_power_control_mode[converter_index];
            VSC_HVDC_CONVERTER_REACTIVE_POWER_CONTROL_MODE reactive_power_mode = current_reactive_power_control_mode[converter_index];
            switch(active_power_mode)
            {
                case VSC_AC_ACTIVE_POWER_CONTORL:
                    if(reactive_power_mode==VSC_AC_REACTIVE_POWER_CONTROL)
                    {
                        Pdc=solve_Pdc_with_active_power_control_and_reactive_power_control(converter_index);
                        Pdc_command[i]=Pdc;
                        break;
                    }
                    else
                    {
                        Pdc=solve_Pdc_with_active_power_control_and_ac_voltage_control(converter_index);
                        Pdc_command[i]=Pdc;
                        break;
                    }
                case VSC_AC_VOLTAGE_ANGLE_CONTROL:
                    if(reactive_power_mode==VSC_AC_REACTIVE_POWER_CONTROL)
                    {
                        Pdc=solve_Pdc_with_voltage_angle_and_reactive_power_control(converter_index);
                        Pdc_command[i]=Pdc;
                        break;
                    }
                    else
                    {
                        Pdc=solve_Pdc_with_voltage_angle_and_voltage_control(converter_index);
                        Pdc_command[i]=Pdc;
                        break;
                    }
                    /*
                    double Pac_command = 0;
                    if(mode==VSC_AC_ACTIVE_POWER_CONTORL)
                        Pac_command = get_converter_nominal_ac_active_power_command_in_MW(converter_index);
                    else
                        Pac_command = get_converter_P_to_AC_bus_in_MW(converter_index);
                    double Qac_command = get_converter_Q_to_AC_bus_in_MVar(converter_index);
                    BUS* bus_pointer = get_converter_ac_bus_pointer(converter_index);
                    double Vac = bus_pointer->get_positive_sequence_voltage_in_kV();
                    double Vbase_ac = get AC side base voltage
                    double Vbase_converter = get converter side base voltage
                    double k = get off-nominal turn ratio
                    double turn_ratio = k*Vbase_ac/Vbase_converter

                    double Eac = Vac/turn_ratio;

                    if(transformer, filter, commutating inductance are considered)
                    {
                        convert transformer Zt, filter Y, commutating ZL in Y form to Pi form
                        caution: what if one or two of (Zt, Y, ZL) is zero.
                        direct calculate current into converter
                    }
                    else
                    {
                        simplified
                    }
                    calculate loss

                    Pdc_command[i]=
                    break;
                    */
                case VSC_DC_ACTIVE_POWER_VOLTAGE_DROOP_CONTROL:
                    Pdc=solve_Pdc_with_dc_active_power_voltage_droop_control(converter_index);
                    Pdc_command[i]=Pdc;
                    break;
                case VSC_DC_CURRENT_VOLTAGE_DROOP_CONTROL:
                    Pdc=solve_Pdc_with_dc_current_voltage_droop_control(converter_index);
                    Pdc_command[i]=Pdc;
                    break;
                case VSC_DC_VOLTAGE_CONTORL:
                    Pdc_command[i] = 0.0;
                    break;
                default:
                    break;
            }
        }
        else
            Pdc_command[i]=0.0;
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
    //cout<<"Vbase_ac-> "<<Vbase_ac<<endl;
    //cout<<"Vbase_converter-> "<<Vbase_converter<<endl;
    //cout<<"k-> "<<k<<endl;
    //cout<<"turn_ration-> "<<turn_ration<<endl;
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
    double kdp=get_converter_initial_power_voltage_droop_coefficient(converter_index);
    int beta=get_converter_beta(converter_index);
    unsigned int dc_bus_index=get_dc_bus_index_with_converter_index(converter_index);
    double Vdc=get_dc_bus_Vdc_in_kV(dc_bus_index);
    double operating_power=beta*(Vdc-Udcref)/kdp;
    cout<<"beta: "<<beta<<endl;
    cout<<"Udcref: "<<Udcref<<endl;
    cout<<"Vdc: "<<Vdc<<endl;
    cout<<"operating_power: "<<operating_power<<endl;
    cout<<"initial_Pdc: "<<initial_Pdc<<endl;
    double Pdc=initial_Pdc-operating_power;
    return Pdc;
}

double VSC_HVDC::solve_Pdc_with_dc_current_voltage_droop_control(unsigned int converter_index) const
{
    double initial_Pdc=get_converter_initial_dc_current_reference_in_kA(converter_index)*get_converter_initial_dc_voltage_reference_in_kV(converter_index);
    double Udcref=get_converter_initial_dc_voltage_reference_in_kV(converter_index);
    double Idcref=get_converter_initial_dc_current_reference_in_kA(converter_index);
    double kdi=get_converter_initial_current_voltage_droop_coefficient(converter_index);
    int alpha=get_converter_alpha(converter_index);
    unsigned int dc_bus_index=get_dc_bus_index_with_converter_index(converter_index);
    double Vdc=get_dc_bus_Vdc_in_kV(dc_bus_index);
    double operating_power=alpha*Vdc*(Idcref-(Vdc-Udcref)/kdi);
    double Pdc=operating_power;
    return Pdc;
}

void VSC_HVDC::update_raw_dc_current_into_dc_network()
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
        double voltage = Udc[column];
        int k_end = dc_network_matrix.get_starting_index_of_column(column+1);
        for(int k=k_start; k<k_end; ++k)
        {
            int row=dc_network_matrix.get_row_number_of_entry_index(k);
            double yij = dc_network_matrix.get_real_entry_value(k);
            bus_current[row]+=yij * voltage;
        }
        k_start = k_end;
    }
    for(unsigned int i=0;i!=nbus;++i)
        cout<<"bus_currenr[i]"<<bus_current[i]<<endl;
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
        bus_power[i] = Udc[i] * bus_current[i];
        cout<<"bus_power[i]:"<<bus_power[i]<<endl;
    }
}

void VSC_HVDC::add_Pdc_command_to_P_mismatch_vector()
{
    unsigned int nbus=get_dc_bus_count();

    P_mismatch.clear();
    P_mismatch.reserve(nbus);
    for(unsigned int i=0;i!=nbus-1;++i)
    {
        P_mismatch.push_back(0.0);
    }
    for(unsigned int i=0;i!=nbus-1;++i)
    {
        P_mismatch[i]=P_mismatch[i]+Pdc_command[i];
    }
}

void VSC_HVDC::add_generation_power_to_P_mismatch_vector()
{
    unsigned int nbus = get_dc_bus_count();
    generation_power.clear();
    generation_power.reserve(nbus);
    for(unsigned int i=0; i<nbus; ++i)
        generation_power.push_back(0.0);
    for (unsigned int index=0; index!=nbus; ++index)
    {
        unsigned int dcbus = get_dc_bus_number(index);
        int i = inphno.get_internal_bus_number_of_physical_bus_number(dcbus);
        generation_power[i] = get_dc_bus_generation_power_in_MW(index);
    }
    for(unsigned int i=0;i!=nbus-1;++i)
    {
        P_mismatch[i]=P_mismatch[i]+generation_power[i];
    }
}

void VSC_HVDC::add_load_power_to_P_mismatch_vector()
{
    unsigned int nbus = get_dc_bus_count();
    load_power.clear();
    load_power.reserve(nbus);
    for(unsigned int i=0; i<nbus; ++i)
        load_power.push_back(0.0);
    for (unsigned int index=0; index!=nbus; ++index)
    {
        unsigned int dcbus = get_dc_bus_number(index);
        int i = inphno.get_internal_bus_number_of_physical_bus_number(dcbus);
        load_power[i] = get_dc_bus_load_power_in_MW(index);
    }
    for(unsigned int i=0;i!=nbus-1;++i)
    {
        P_mismatch[i]=P_mismatch[i]-load_power[i];
    }
}

void VSC_HVDC::add_raw_dc_power_to_P_mismatch_vector()
{
    unsigned int nbus=get_dc_bus_count();
    for(unsigned int i=0;i!=nbus-1;++i)
    {
        P_mismatch[i]=P_mismatch[i]-bus_power[i];
        cout<<"add_raw_dc_power_to_P_mismatch_vector: "<<P_mismatch[i]<<endl;
    }
}

double VSC_HVDC::get_maximum_active_power_mismatch_in_MW()
{
    unsigned int nbus=get_dc_bus_count();
    double max_P_error_in_MW=0.0;
    for(unsigned int i=0; i!=nbus-1; ++i)
    {
        if(fabs(P_mismatch[i])<max_P_error_in_MW)
            continue;
        else
        {
            max_P_error_in_MW=fabs(P_mismatch[i]);
        }
    }
    return max_P_error_in_MW;
}

bool VSC_HVDC::check_dc_bus_control_mode()
{
    unsigned int n_converter=get_converter_count();
    for(unsigned int index=0;index!=n_converter;++index)
    {
        if(get_converter_active_power_operation_mode(index)==current_active_power_control_mode[index]
           and get_converter_reactive_power_operation_mode(index)==current_reactive_power_control_mode[index])
            return false;
        else
            return true;
    }
}

double VSC_HVDC::get_allowed_max_active_power_imbalance_in_MW()
{
    return P_threshold_in_MW;
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

double VSC_HVDC::get_converter_P_to_AC_bus_in_MW(unsigned int index) const
{
    if(not converter_index_is_out_of_range_in_function(index, __FUNCTION__))
        converters[index].P_to_AC_bus_MW;
    else
        return 0.0;
}

double VSC_HVDC::get_converter_Q_to_AC_bus_in_MVar(unsigned int index) const
{
    if(not converter_index_is_out_of_range_in_function(index, __FUNCTION__))
        converters[index].Q_to_AC_bus_MVar;
    else
        return 0.0;
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

    STEPS& toolkit = get_toolkit();

    unsigned int nbus = get_dc_bus_count();
    jacobian.clear();
    for(unsigned int i=0; i!=nbus-1; ++i)
        jacobian.add_entry(i, i, 0);

    unsigned int n = dc_network_matrix.get_matrix_entry_count();
    unsigned int row, column;
    unsigned int ibus, jbus;
    double y, Udi, Udj, Idi_reference, Udi_reference, Kdi, Kpi;
    double alpha, beta;
    ostringstream osstream;

    for(unsigned int k=0; k!=n; ++k)
    {
        double der=0;
        row = dc_network_matrix.get_row_number_of_entry_index(k);
        column = dc_network_matrix.get_column_number_of_entry_index(k);
        if(row == nbus-1 or column == nbus-1)
            continue;

        ibus = inphno.get_physical_bus_number_of_internal_bus_number(row);
        jbus = inphno.get_physical_bus_number_of_internal_bus_number(column);

        Udi = get_dc_voltage_of_dc_bus_number(ibus);
        unsigned int converter_index = get_dc_bus_converter_index_with_dc_bus_number(ibus);
        if(converter_index!=INDEX_NOT_EXIST)
        {
            Idi_reference = get_converter_initial_dc_current_reference_in_kA(converter_index);
            Udi_reference = get_converter_initial_dc_voltage_reference_in_kV(converter_index);
            Kdi = get_converter_initial_current_voltage_droop_coefficient(converter_index);
            Kpi = get_converter_initial_power_voltage_droop_coefficient(converter_index);
            VSC_HVDC_CONVERTER_ACTIVE_POWER_CONTROL_MODE mode = get_converter_active_power_operation_mode(converter_index);
            if(mode!=VSC_DC_ACTIVE_POWER_VOLTAGE_DROOP_CONTROL and mode!=VSC_DC_CURRENT_VOLTAGE_DROOP_CONTROL)
            {
                Kdi=1.0;
                Kpi=1.0;
            }
            alpha = get_converter_alpha(converter_index);
            beta = get_converter_beta(converter_index);

            /*Idi_reference = get_dc_current_reference_of_dc_bus_number(ibus);
            Udi_reference = get_dc_voltage_reference_of_dc_bus_number(ibus);
            Kdi = get_current_voltage_dropp_coefficient_of_dc_bus_number(ibus);
            Kpi = get_power_voltage_droop_coefficient_of_dc_bus_number(ibus);
            alpha = get_alpha_of_dc_bus_number(ibus);
            beta = get_beta_of_dc_bus_number(ibus);*/
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
            der = 0.0;
            unsigned int k_start=dc_network_matrix.get_starting_index_of_column(row);
            unsigned int k_end=dc_network_matrix.get_starting_index_of_column(row+1);
            for(unsigned int m=k_start;m!=k_end;++m)
            {
                y = dc_network_matrix.get_real_entry_value(m);
                int temp_col=dc_network_matrix.get_row_number_of_entry_index(m);
                jbus=inphno.get_physical_bus_number_of_internal_bus_number(temp_col);
                Udj = get_dc_voltage_of_dc_bus_number(jbus);
                if(temp_col!=row)
                    der+=(-Udj*y);
                else
                    der+=(-2.0*Udi*y);

            }
            double variable = alpha*Idi_reference-alpha*(Udi-Udi_reference)/Kdi-alpha*Udi/Kdi-beta/Kpi;
            //cout<<"variable:: "<<variable<<endl;
            //cout<<"alpha="<<alpha<<", beta="<<beta<<", Kdi="<<Kdi<<",Kpi="<<Kpi<<endl;
            der+=alpha*Idi_reference-alpha*(Udi-Udi_reference)/Kdi-alpha*Udi/Kdi-beta/Kpi;
        }
        if(not std::isnan(der))
            ;
        else
        {
            osstream<<"NAN is detected when building Jacobian matrix for VSC_HVDC at row: "<<row<<"(bus "<<ibus<<"), col: "<<column<<"(bus "<<jbus<<")";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
        jacobian.add_entry(row, column, der);
    }
    jacobian.compress_and_merge_duplicate_entries();

}


void VSC_HVDC::update_converters_P_and_Q_to_AC_bus()
{
    unsigned int n = get_converter_count();
    for(unsigned int index=0; index!=n; ++index)
        update_converter_P_and_Q_to_AC_bus(index);
}
void VSC_HVDC::update_converter_P_and_Q_to_AC_bus(unsigned int index)
{
    if(get_converter_status(index)==true)
    {
        VSC_HVDC_CONVERTER_ACTIVE_POWER_CONTROL_MODE mode = current_active_power_control_mode[index];
        if(mode==VSC_DC_VOLTAGE_CONTORL or
           mode==VSC_DC_ACTIVE_POWER_VOLTAGE_DROOP_CONTROL or
           mode==VSC_DC_CURRENT_VOLTAGE_DROOP_CONTROL)
        {
            double Pdc = get_Pdc_command(index);

            double Qac = get_converter_Q_to_AC_bus_in_MVar(index);
            BUS* bus_pointer = get_converter_ac_bus_pointer(index);
            double Vac = bus_pointer->get_positive_sequence_voltage_in_kV();
            double Vbase_ac = get_converter_transformer_AC_side_base_voltage_in_kV(index);
            double Vbase_converter = get_converter_transformer_converter_side_base_voltage_in_kV(index);
            double k = get_converter_transformer_off_nominal_turn_ratio(index);
            double turn_ratio = k*Vbase_ac/Vbase_converter;
            double Eac = Vac/turn_ratio;
            /*
            convert transformer Zt, filter Y, commutating Z in Y form to Pi form

            guess Pac0 = Pdc;
            guess Pac1 = Pdc*1.03 or Pdc*0.97. depends on direction of Pdc
            while(true)
            {
                direct calculate current into converter for (Pac0, Qac)
                calculate loss for Pac0
                get Pdc0 for Pac0
                direct calculate current into converter for (Pac1, Qac)
                calculate loss for Pac1
                get Pdc1 for Pac1

                interpolate or extrapolate based on linear guess, get Pac2
                direct calculate current into converter for (Pac2, Qac)
                calculate loss for Pac2
                get Pdc2 for Pac2

                if(fabs(Pdc2-Pdc)<tolerance)
                    converged
                    break

                find the two best solutions that most close to Pdc from(Pdc0, Pdc1, Pdc2)
                and get new corresponding (Pac0 and Pac0)
                */
            }

            //Pdc_command[i]=
        double Pac=0;
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
    int nbus=get_dc_bus_count();
    for(unsigned int index=0; index!=nbus-1;++index)
    {
        unsigned int dcbus = get_dc_bus_number(index);
        int i = inphno.get_internal_bus_number_of_physical_bus_number(dcbus);
        double Vdc=Udc[i]+Udc_mismatch[i];
        set_dc_bus_Vdc_in_kV(index,Vdc);
        Udc[i]=Udc[i]-Udc_mismatch[i];
    }
    for(int i=0; i!=nbus-1; ++i)
    {
        cout<<"Udc[i]: "<<Udc[i]<<endl;
    }
}

void VSC_HVDC::calculate_dc_active_power_of_slack_bus()
{
    P_slack_dc_side=0.0;
    int nsize = dc_network_matrix.get_matrix_size();
    int k_start=dc_network_matrix.get_starting_index_of_column(nsize-1);
    int k_end=dc_network_matrix.get_starting_index_of_column(nsize);
    for(int k=k_start;k!=k_end;++k)
    {
        int row=dc_network_matrix.get_row_number_of_entry_index(k);
        double yij = dc_network_matrix.get_real_entry_value(k);
        P_slack_dc_side+=Udc[nsize-1]*yij*Udc[row];
    }
}

void VSC_HVDC::check_dc_slack_converter_constraint()
{
    //if (P_slack_dc_side>
    unsigned int n_converter=get_converter_count();
    for(unsigned int index=0; index!=n_converter;++index)
    {
        VSC_HVDC_CONVERTER_ACTIVE_POWER_CONTROL_MODE active_power_control_mode=current_active_power_control_mode[index];
        VSC_HVDC_CONVERTER_REACTIVE_POWER_CONTROL_MODE reactive_power_control_mode=current_reactive_power_control_mode[index];
        if(active_power_control_mode==VSC_DC_VOLTAGE_CONTORL)
        {
            double P_max=get_converter_Pmax_in_MW(index);
            double Vdc_control=get_converter_nominal_dc_voltage_command_in_kV(index);
            calculate_dc_active_power_of_slack_bus();
            if(P_slack_dc_side>P_max)
            {
                unsigned int master_converter_index=0;
                unsigned int reserve_bus_number=get_reserve_master_converter_ac_bus();
                unsigned int reserve_converter_index=get_converter_index_with_ac_bus(reserve_bus_number);
                n_converter=get_converter_count();
                for(unsigned int i=0; i!=n_converter;++i)
                {
                    if(get_converter_active_power_operation_mode(index)==VSC_DC_VOLTAGE_CONTORL)
                        master_converter_index=index;
                }
                current_active_power_control_mode[master_converter_index]=VSC_AC_ACTIVE_POWER_CONTORL;
                current_active_power_control_mode[reserve_converter_index]=VSC_DC_VOLTAGE_CONTORL;
                set_converter_nominal_ac_active_power_command_in_MW(master_converter_index, P_max);
                set_converter_nominal_dc_voltage_command_in_kV(reserve_converter_index,Vdc_control);
            }
        }
    }

}

double VSC_HVDC::get_dc_voltage_of_dc_bus_number(unsigned int bus)
{
    unsigned int internal_bus = inphno.get_internal_bus_number_of_physical_bus_number(bus);
    if(internal_bus!=INDEX_NOT_EXIST)
        return Udc[internal_bus];
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. DC bus number "<<bus<<" does not exist in DC buses of "<<get_compound_device_name();
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 0.0;
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

double VSC_HVDC::get_Pdc_command(unsigned int index) const
{
    double Pdc=0.0;
    unsigned int dc_bus_count=get_dc_bus_count();
    unsigned int dc_bus_index=0;
    for(unsigned int i=0; i!=dc_bus_count; ++i)
    {
        if(dc_buses[i].converter_index==index)
            dc_bus_index=i;
    }
    double dc_bus_voltage=get_dc_bus_Vdc_in_kV(dc_bus_index);
    VSC_HVDC_CONVERTER_ACTIVE_POWER_CONTROL_MODE mode=get_converter_active_power_operation_mode(index);
    if(mode==VSC_DC_VOLTAGE_CONTORL)
    {
        int nsize = dc_network_matrix.get_matrix_size();
        int k_start=dc_network_matrix.get_starting_index_of_column(nsize-1);
        int k_end=dc_network_matrix.get_starting_index_of_column(nsize);
        for(int k=k_start;k!=k_end;++k)
        {
            int row=dc_network_matrix.get_row_number_of_entry_index(k);
            double yij = dc_network_matrix.get_real_entry_value(k);
            unsigned int dc_bus_number=inphno.get_physical_bus_number_of_internal_bus_number(row);
            double row_voltage=get_dc_bus_Vdc_in_kV(dc_bus_no2index(dc_bus_number));
            Pdc+=dc_bus_voltage*yij*row_voltage;
            return Pdc;
        }
    }
    if(mode==VSC_DC_ACTIVE_POWER_VOLTAGE_DROOP_CONTROL)
    {
        double Pdref=get_converter_initial_dc_active_power_reference_in_MW(index);
        double droop=get_converter_initial_dc_voltage_reference_in_kV(index);
        double Udref=get_converter_initial_dc_voltage_reference_in_kV(index);
        Pdc=Pdref-(dc_bus_voltage-Udref)/droop;
        return Pdc;
    }
    if(mode==VSC_DC_CURRENT_VOLTAGE_DROOP_CONTROL)
    {
        double Idref=get_converter_initial_dc_current_reference_in_kA(index);
        double Udref=get_converter_initial_dc_voltage_reference_in_kV(index);
        double droop=get_converter_initial_current_voltage_droop_coefficient(index);
        double Idc=Idref-(dc_bus_voltage-Udref)/droop;
        double Pdc=dc_bus_voltage*Idc;
        return Pdc;
    }
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. converter index ("<<index<<") is not Vdc mode or VP mode or VImode when calling VSC_HVDC::"<<__FUNCTION__<<"()."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return NULL;
    }
}

void VSC_HVDC::save_dc_bus_powerflow_result_to_file(const string& filename) const
{
    ostringstream osstream;
    ofstream file(filename);
    if(file.is_open())
    {
        file<<"DC_BUS,AC_BUS,VOLTAGE"<<endl;

        //vector<BUS*> buses = psdb.get_all_buses();
        unsigned int nbus = Udc.size();
        for(unsigned int i=0; i!=nbus; ++i)
        {
            file<<"Udc["<<i<<"]"<<","
                <<setprecision(6)<<fixed<<get_converter_ac_bus_number_with_dc_bus_index(i)<<","
                <<setprecision(6)<<fixed<<Udc[i]<<endl;
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

bool VSC_HVDC::check_slack_limit(double P)
{
    unsigned int dc_control_bus=get_ac_converter_bus_with_dc_voltage_control();
    unsigned int ncon=get_converter_count();
    unsigned int index=0;
    for(unsigned int i=0;i!=ncon;++i)
    {
        if(get_converter_ac_bus(i)==dc_control_bus)
            index=i;
    }
    if(P<get_converter_rated_capacity_in_MVA(index))
        return true;
    else
        return false;
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


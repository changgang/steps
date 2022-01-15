#include "header/device/vsc_hvdc.h"
#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include "header/STEPS.h"
#include "header/steps_namespace.h"
#include "header/model/hvdc_model/hvdc_models.h"
#include <istream>
#include <iostream>
#include <cstdio>

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
        if(vsc.get_converter_dc_operation_mode(i)==1)
        {
            set_converter_dc_operation_mode(i,VSC_DC_VOLTAGE_CONTORL);
            set_converter_nominal_dc_voltage_command_in_kV(i,vsc.get_converter_nominal_dc_voltage_command_in_kV(i));
        }

        else
        {
            set_converter_dc_operation_mode(i,VSC_AC_ACTIVE_POWER_CONTORL);
            set_converter_nominal_ac_active_power_command_in_MW(i,vsc.get_converter_nominal_ac_active_power_command_in_MW(i));
        }

        if(vsc.get_converter_ac_operation_mode(i)==1)
        {
            set_converter_ac_operation_mode(i,VSC_AC_VOLTAGE_CONTROL);
            set_converter_nominal_ac_voltage_command_in_pu(i,vsc.get_converter_nominal_ac_voltage_command_in_pu(i));
        }

        else
        {
            set_converter_ac_operation_mode(i,VSC_AC_REACTIVE_POWER_CONTROL);
            set_converter_nominal_ac_reactive_power_command_in_Mvar(i,vsc.get_converter_nominal_ac_reactive_power_command_in_Mvar(i));
        }
        set_converter_loss_factor_A_in_kW(i,vsc.get_converter_loss_factor_A_in_kW(i));
        set_converter_loss_factor_B_in_kW_per_amp(i,vsc.get_converter_loss_factor_B_in_kW_per_amp(i));
        set_converter_loss_factor_C_in_KW_per_amp_squared(i,vsc.get_converter_loss_factor_C_in_kW_per_amp_squard(i));
        set_converter_minimum_loss_in_kW(i,vsc.get_converter_minimum_loss_in_kW(i));
        set_converter_rated_capacity_in_MVA(i,vsc.get_converter_rated_capacity_in_MVA(i));
        set_converter_rated_current_in_A(i,vsc.get_converter_current_rating_in_amp(i));
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
    active_power.clear();
    reactive_power.clear();
    iteration_count=0;
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

bool VSC_HVDC::converter_index_is_out_of_range_in_function(const unsigned int index, const string& func)
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
        }
        else
        {
            ostringstream osstream;
            STEPS& toolkit = get_toolkit();
            osstream<<"Bus "<<bus<<" does not exist for setting up converter AC side bus of vsc-hvdc link."<<endl
                    <<"0 will be set to indicate invalid vsc-hvdc link.";
            toolkit.show_information_with_leading_time_stamp(osstream);
            converters[index].converter_bus = 0;
        }
    }
    else
    {
        ostringstream osstream;
        STEPS& toolkit = get_toolkit();
        osstream<<"Warning. Zero bus number (0) is not allowed for setting up converter AC bus of vsc-hvdc link."<<endl
                <<"0 will be set to indicate invalid vsc-hvdc link.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        converters[index].converter_bus = bus;
    }
}

void VSC_HVDC::set_converter_dc_operation_mode(const unsigned int index, const VSC_HVDC_DC_CONTROL_MODE mode)
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

void VSC_HVDC::set_converter_ac_operation_mode(const unsigned int index, const VSC_HVDC_AC_CONTROL_MODE mode)
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
    converters[index].initial_power_voltage_droop_coefficient = droop;
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

void VSC_HVDC::set_converter_transformer_impedance_in_ohm(unsigned int index, const complex<double> z)
{
    if(converter_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    converters[index].converter_transformer_impedance_in_ohm = z;
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

bool VSC_HVDC::dc_bus_index_is_out_of_range_in_function(const unsigned int index, const string& func)
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

bool VSC_HVDC::dc_line_index_is_out_of_range_in_function(const unsigned int index, const string& func)
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

double VSC_HVDC::get_dc_network_base_voltage_in_kV() const
{
    return dc_base_voltage_in_kV;
}

unsigned int VSC_HVDC::get_converter_ac_bus(unsigned int index) const
{
    if(index<get_converter_count())
        return converters[index].converter_bus;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 0;
    }
}

VSC_HVDC_DC_CONTROL_MODE VSC_HVDC::get_converter_dc_operation_mode(unsigned int index) const
{
    if(index<get_converter_count())
        return converters[index].dc_control_mode;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return VSC_INVALID_DC_CONTORL;
    }

}

VSC_HVDC_AC_CONTROL_MODE VSC_HVDC::get_converter_ac_operation_mode(unsigned int index) const
{
    if(index<get_converter_count())
        return converters[index].ac_control_mode;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
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
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
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
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
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
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
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
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
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
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
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
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
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
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
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
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

double VSC_HVDC::get_converter_initial_power_voltage_droop_coefficient(const unsigned int index)const
{
    if(index<get_converter_count())
        return converters[index].initial_power_voltage_droop_coefficient;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
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
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
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
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
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
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
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
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
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
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
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
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
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
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
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
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
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
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

double VSC_HVDC::get_converter_current_rating_in_amp(unsigned int index) const
{
    if(index<get_converter_count())
        return converters[index].converter_rated_current_in_amp;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

complex<double>VSC_HVDC::get_converter_transformer_impedance_in_ohm(unsigned int index) const
{
    if(index<get_converter_count())
        return converters[index].converter_transformer_impedance_in_ohm;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
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
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
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
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        complex<double> j(0.0,1.0);
        return 9999.9+j*9999.9;
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
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
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
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
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
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
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
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
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
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
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
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
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
    cout<<"get_converter_ac_bus(1):"<<get_converter_ac_bus(1)<<endl;
    cout<<"get_converter_dc_operation_mode(1):"<<get_converter_dc_operation_mode(1)<<endl;
    cout<<"get_converter_Qmax_in_MVar(1):"<<get_converter_Qmax_in_MVar(1)<<endl;
    cout<<"get_converter_Qmin_in_MVar(1):"<<get_converter_Qmin_in_MVar(1)<<endl;
    cout<<"get_dc_bus_area(1):"<<get_dc_bus_area(1)<<endl;
    cout<<"get_dc_bus_zone(1):"<<get_dc_bus_zone(1)<<endl;
    cout<<"get_dc_bus_owner_number(1):"<<get_dc_bus_owner_number(1)<<endl;
    cout<<"get_dc_bus_ground_resistance_in_ohm(1):"<<get_dc_bus_ground_resistance_in_ohm(1)<<endl;
    cout<<"get_dc_line_sending_side_bus(1):"<<get_dc_line_sending_side_bus(1)<<endl;
    cout<<"get_dc_line_receiving_side_bus(1):"<<get_dc_line_receiving_side_bus(1)<<endl;
    cout<<"get_dc_line_identifier(1):"<<get_dc_line_identifier(1)<<endl;
    cout<<"get_dc_line_meter_end_bus(1):"<<get_dc_line_meter_end_bus(1)<<endl;
    cout<<"get_dc_line_resistance_in_ohm(1):"<<get_dc_line_resistance_in_ohm(1)<<endl;
    cout<<"get_dc_line_inductance_in_mH(1):"<<get_dc_line_inductance_in_mH(1)<<endl;
    cout<<"get_dc_bus_generation_power_in_MW(0):"<<get_dc_bus_generation_power_in_MW(0)<<endl;
    cout<<"get_dc_bus_load_power_in_MW(0):"<<get_dc_bus_load_power_in_MW(0)<<endl;
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
    initialize_P_converter_loss_vector();
    initialize_alpha_vector();
    initialize_beta_vector();

    while(true)
    {
        cout<<"ieteration_count: "<<iteration_count<<endl;
        build_dc_bus_power_mismatch_vector();
        max_P_mismatch_in_MW = get_maximum_active_power_mismatch_in_MW();
        cout<<"max_P_mismatch_in_MW:"<<max_P_mismatch_in_MW<<endl;
        //cout<<"allowed_max_active_power_imbalance_in_MW:"<<get_allowed_max_active_power_imbalance_in_MW()<<endl;
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
        Udc_mismatch = P_mismatch/jacobian;
        if(jacobian.is_lu_factorization_successful())
        {
            update_dc_bus_voltage();
            ++iteration_count;
        }
        else
        {
            osstream<<"Vsc hvdc No further powerflow solution will be attempted since LU factorization of N-R Jacobian matrix is failed."<<endl;
            toolkit.show_information_with_leading_time_stamp(osstream);
            break;
        }
        update_P_converter_loss_vector();
    }
    save_dc_bus_powerflow_result_to_file("Vsc_hvdc_bus_powerflow_result.csv");
    calculate_dc_active_power_of_slack_bus();
    unsigned int nbus=get_dc_bus_count();
    P_slack_ac_side=P_slack_dc_side+P_converter_loss[nbus-1];

    if(check_slack_limit(P_slack_ac_side)==true)
    {
        osstream<<"dc voltage control converter within the capacity limit"<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
    else
    {
        osstream<<"dc voltage control converter is not within the capacity limit"<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }

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
            VSC_HVDC_DC_CONTROL_MODE mode = get_converter_dc_operation_mode(converter_index);
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
                Udc[i] = get_dc_network_base_voltage_in_kV();
            else
            {
                unsigned int converter_index = get_dc_bus_converter_index_with_dc_bus_number(dcbus);
                if(converter_index!=INDEX_NOT_EXIST)
                    Udc[i] = get_converter_nominal_dc_voltage_command_in_kV(converter_index);
                else
                    Udc[i] = get_dc_network_base_voltage_in_kV();
            }
        }
        for(unsigned int i=0;i!=nbus;++i)
            cout<<"Udc[i]"<<Udc[i]<<endl;
    }
}

void VSC_HVDC::initialize_P_converter_loss_vector()
{
    unsigned int nbus = get_dc_bus_count();
    P_converter_loss.clear();
    P_converter_loss.reserve(nbus);
    for(unsigned int i=0; i<nbus; ++i)
        P_converter_loss.push_back(0.0);

    for (unsigned int index=0; index!=nbus; ++index)
    {
        unsigned int dcbus = get_dc_bus_number(index);
        int i = inphno.get_internal_bus_number_of_physical_bus_number(dcbus);
        unsigned int converter_index = get_dc_bus_converter_index_with_dc_bus_number(dcbus);
        if(converter_index==INDEX_NOT_EXIST)
            P_converter_loss[i] = 0.0;
        else
        {
            VSC_HVDC_DC_CONTROL_MODE mode = get_converter_dc_operation_mode(converter_index);
            if(mode==VSC_AC_ACTIVE_POWER_CONTORL)
                P_converter_loss[i] = 0.01*get_converter_nominal_ac_active_power_command_in_MW(converter_index);
            if(mode==VSC_DC_ACTIVE_POWER_VOLTAGE_DROOP_CONTROL)
                P_converter_loss[i] = 0.01*get_converter_initial_dc_active_power_reference_in_MW(converter_index);
            if(mode==VSC_DC_CURRENT_VOLTAGE_DROOP_CONTROL)
                P_converter_loss[i] = 0.01*get_converter_initial_dc_current_reference_in_kA(converter_index)*get_converter_initial_dc_voltage_reference_in_kV(converter_index);
            if(mode==VSC_DC_VOLTAGE_CONTORL)
                P_converter_loss[i] = 0.0;
        }
    }
    /*
    for(unsigned int i=0; i!=ncon; ++i)
    {
        unsigned int con_number=get_converter_ac_bus(i);
        if(dc_voltage_control_bus==con_number)
        {
            P_converter_loss[nbus-1]=0;
        }
        else
        {
            for(unsigned int j=0; j!=nbus; ++j)
            {
                unsigned int ac_bus_number=get_dc_bus_ac_bus_number(j);
                if(con_number==ac_bus_number)
                {
                    if(j<dc_voltage_control_index)
                    {
                        P_converter_loss[j]=Aloss+Bloss*fabs(bus_current[j]);
                    }
                    if(j>dc_voltage_control_index)
                    {
                        P_converter_loss[j-1]=Aloss+Bloss*fabs(bus_current[j-1]);
                    }
                }
            }
        }

    }
    */
    /*
    for(unsigned int i=0; i!=ncon; ++i)
    {
        double Aloss=get_converter_loss_factor_A_in_kW(i);
        double Bloss=get_converter_loss_factor_B_in_kW_per_amp(i);
        double Closs=get_converter_loss_factor_C_in_kW_per_amp_squard(i);
        unsigned int con_number=get_converter_ac_bus(i);
        if(dc_voltage_control_bus==con_number)
        {
            P_converter_loss[nbus-1]=Aloss+Bloss*fabs(bus_current[nbus-1]);
        }
        else
        {
            for(unsigned int j=0; j!=nbus; ++j)
            {
                unsigned int ac_bus_number=get_dc_bus_ac_bus_number(j);
                if(con_number==ac_bus_number)
                {
                    if(j<dc_voltage_control_index)
                    {
                        P_converter_loss[j]=Aloss+Bloss*fabs(bus_current[j]);
                    }
                    if(j>dc_voltage_control_index)
                    {
                        P_converter_loss[j-1]=Aloss+Bloss*fabs(bus_current[j-1]);
                    }
                }
            }
        }
    }
    */
}

void VSC_HVDC::initialize_alpha_vector()
{
    unsigned int n_converter = get_converter_count();
    alpha.clear();
    alpha.reserve(n_converter);
    for(unsigned int i=0; i<n_converter; ++i)
    {
        cout<<"operation_mode:"<<get_converter_dc_operation_mode(i)<<endl;
        if(get_converter_dc_operation_mode(i) == VSC_DC_CURRENT_VOLTAGE_DROOP_CONTROL)
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
        if(get_converter_dc_operation_mode(i) == VSC_DC_ACTIVE_POWER_VOLTAGE_DROOP_CONTROL)
            beta.push_back(1);
        else
            beta.push_back(0);
    }
    for(unsigned int i=0; i!=n_converter;++i)
    {
        cout<<"beta[i]:"<<beta[i]<<endl;
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
    add_P_converter_loss_to_P_mismatch_vector();
}

void VSC_HVDC::build_Pdc_command_vector()
{
    unsigned int nbus = get_dc_bus_count();
    Pdc_command.clear();
    Pdc_command.reserve(nbus);
    for(unsigned int i=0;i!=nbus;++i)
        Pdc_command.push_back(0.0);
    for (unsigned int index=0; index!=nbus; ++index)
    {
        unsigned int dcbus = get_dc_bus_number(index);
        int i = inphno.get_internal_bus_number_of_physical_bus_number(dcbus);
        unsigned int converter_index = get_dc_bus_converter_index_with_dc_bus_number(dcbus);
        if(converter_index==INDEX_NOT_EXIST)
            Pdc_command[i]=0.0;
        else
        {
            VSC_HVDC_DC_CONTROL_MODE mode = get_converter_dc_operation_mode(converter_index);
            if(mode==VSC_AC_ACTIVE_POWER_CONTORL)
            {
                Pdc_command[i]=get_converter_nominal_ac_active_power_command_in_MW(converter_index);
            }
            if(mode==VSC_DC_ACTIVE_POWER_VOLTAGE_DROOP_CONTROL)
            {
                double initial_Pdc=get_converter_initial_dc_active_power_reference_in_MW(converter_index);
                double Udcref=get_converter_initial_dc_voltage_reference_in_kV(converter_index);
                double kdp=get_converter_initial_power_voltage_droop_coefficient(converter_index);
                int beta=get_converter_beta(converter_index);
                double operating_power=beta*(Udc[i]-Udcref)/kdp;
                Pdc_command[i]=initial_Pdc+operating_power;
            }
            if(mode==VSC_DC_CURRENT_VOLTAGE_DROOP_CONTROL)
            {
                double initial_Pdc=get_converter_initial_dc_current_reference_in_kA(converter_index)*get_converter_initial_dc_voltage_reference_in_kV(converter_index);
                double Udcref=get_converter_initial_dc_voltage_reference_in_kV(converter_index);
                double Idcref=get_converter_initial_dc_current_reference_in_kA(converter_index);
                double kdi=get_converter_initial_current_voltage_droop_coefficient(converter_index);
                int alpha=get_converter_alpha(converter_index);
                double operating_power=alpha*Udc[i]*(Idcref-(Udc[i]-Udcref)/kdi);
                Pdc_command[i]=initial_Pdc-operating_power;
            }
            if(mode==VSC_DC_VOLTAGE_CONTORL)
                Pdc_command[i] = 0.0;
        }
    }
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
    //for(unsigned int i=0;i!=nbus;++i)
    //    cout<<bus_current[i]<<endl;
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
        //cout<<"bus_power[i]:"<<bus_power[i]<<endl;
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
        P_mismatch[i]=P_mismatch[i]-Pdc_command[i];
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
        P_mismatch[i]=P_mismatch[i]-generation_power[i];
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
    }
}

void VSC_HVDC::add_P_converter_loss_to_P_mismatch_vector()
{
    unsigned int nbus=get_dc_bus_count();
    for(unsigned int i=0;i!=nbus-1;++i)
    {
        //P_mismatch[i]=P_mismatch[i]-P_converter_loss[i]/1000;  //not calculate P_converter_loss, not need to divide 1000
        P_mismatch[i]=P_mismatch[i]-P_converter_loss[i];
        cout<<"P_mismatch["<<i+1<<"]: "<<P_mismatch[i]<<endl;
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
            VSC_HVDC_DC_CONTROL_MODE mode = get_converter_dc_operation_mode(converter_index);
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

void VSC_HVDC::update_dc_bus_voltage()
{
    int nbus=get_dc_bus_count();
    for(int i=0; i!=nbus-1; ++i)
    {
        Udc[i]=Udc[i]-Udc_mismatch[i];
        cout<<"Udc[i]: "<<Udc[i]<<endl;
    }
}

void VSC_HVDC::update_P_converter_loss_vector()
{
    unsigned int nbus = get_dc_bus_count();
    P_converter_loss.clear();
    P_converter_loss.reserve(nbus);
    for(unsigned int i=0; i<nbus; ++i)
        P_converter_loss.push_back(0.0);

    for (unsigned int index=0; index!=nbus; ++index)
    {
        unsigned int dcbus = get_dc_bus_number(index);
        int i = inphno.get_internal_bus_number_of_physical_bus_number(dcbus);
        unsigned int converter_index = get_dc_bus_converter_index_with_dc_bus_number(dcbus);
        if(converter_index==INDEX_NOT_EXIST)
            P_converter_loss[i] = 0.0;
        else
        {
            VSC_HVDC_DC_CONTROL_MODE mode = get_converter_dc_operation_mode(converter_index);
            if(mode==VSC_AC_ACTIVE_POWER_CONTORL)
                P_converter_loss[i] = 0.01*get_converter_nominal_ac_active_power_command_in_MW(converter_index);
            if(mode==VSC_DC_ACTIVE_POWER_VOLTAGE_DROOP_CONTROL)
                P_converter_loss[i] = 0.01*get_converter_initial_dc_active_power_reference_in_MW(converter_index);
            if(mode==VSC_DC_CURRENT_VOLTAGE_DROOP_CONTROL)
                P_converter_loss[i] = 0.01*get_converter_initial_dc_current_reference_in_kA(converter_index)*get_converter_initial_dc_voltage_reference_in_kV(converter_index);
            if(mode==VSC_DC_VOLTAGE_CONTORL)
                P_converter_loss[i] = 0.0;
        }
    }
    /*
    for(unsigned int i=0; i!=ncon; ++i)
    {
        double Aloss=get_converter_loss_factor_A_in_kW(i);
        double Bloss=get_converter_loss_factor_B_in_kW_per_amp(i);
        unsigned int con_number=get_converter_ac_bus(i);
        if(dc_voltage_control_bus==con_number)
        {
            Pdc_loss[nbus-1]=Aloss+Bloss*fabs(bus_current[nbus-1]);
        }
        else
        {
            for(unsigned int j=0; j!=nbus; ++j)
            {
                unsigned int ac_bus_number=get_dc_bus_ac_bus_number(j);
                if(con_number==ac_bus_number)
                {
                    if(j<dc_voltage_control_index)
                    {
                        Pdc_loss[j]=Aloss+Bloss*fabs(bus_current[j]);
                    }
                    if(j>dc_voltage_control_index)
                    {
                        Pdc_loss[j-1]=Aloss+Bloss*fabs(bus_current[j-1]);
                    }
                }
            }
        }
    }*/
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

double VSC_HVDC::get_dc_voltage_of_dc_bus_number(unsigned int bus)
{
    unsigned int index = inphno.get_internal_bus_number_of_physical_bus_number(bus);
    if(index!=INDEX_NOT_EXIST)
        return Udc[index];
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. DC bus number "<<bus<<" does not exist in DC buses of "<<get_compound_device_name();
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 0.0;
    }
}


int VSC_HVDC::get_converter_alpha(unsigned int index)
{
    if(index!=INDEX_NOT_EXIST)
        return alpha[index];
    else
        return 0;
}

int VSC_HVDC::get_converter_beta(unsigned int index)
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


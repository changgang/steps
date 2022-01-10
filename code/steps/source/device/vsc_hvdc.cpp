#include "header/device/vsc_hvdc.h"
#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include "header/STEPS.h"
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
        set_converter_bus(i,vsc.get_converter_bus(i));
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
            set_converter_nominal_ac_reactive_power_command_in_Mvar(i,vsc.get_converter_nominal_ac_power_command_in_Mvar(i));
        }
        set_converter_loss_factor_A_in_kW(i,vsc.get_converter_loss_factor_A_in_kW(i));
        set_converter_loss_factor_B_in_kW_per_amp(i,vsc.get_converter_loss_factor_B_in_kW_per_amp(i));
        set_converter_minimum_loss_in_kW(i,vsc.get_converter_minimum_loss_in_kW(i));
        set_converter_rated_capacity_in_MVA(i,vsc.get_converter_rated_capacity_in_MVA(i));
        set_converter_rated_current_in_A(i,vsc.get_converter_current_rating_in_amp(i));
        set_converter_power_weighting_factor(i,vsc.get_converter_power_weighting_factor(i));
        set_converter_Qmax_in_MVar(i,vsc.get_converter_Qmax_in_MVar(i));
        set_converter_Qmin_in_MVar(i,vsc.get_converter_Qmin_in_MVar(i));
        set_converter_remote_bus_to_regulate(i,vsc.get_converter_remote_bus_to_regulate(i));
        set_converter_remote_regulation_percent(i,vsc.get_converter_remote_regulation_percent(i));

    }

    for(unsigned int i=0;i!=nbus;++i)
    {
        set_dc_bus_number(i,vsc.get_dc_bus_number(i));
        set_ac_bus_number_of_dc_bus(i,vsc.get_ac_bus_number_of_dc_bus(i));
        set_dc_bus_area(i,vsc.get_dc_bus_area(i));
        set_dc_bus_zone(i,vsc.get_dc_bus_zone(i));
        set_dc_bus_name(i,vsc.get_dc_bus_name(i));
        set_ground_resistance_in_ohm(i,vsc.get_ground_resistance_in_ohm(i));
        set_owner_number(i,vsc.get_owner_number(i));
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

    converters.clear();
    dc_buses.clear();
    dc_lines.clear();
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
            osstream<<"Bus "<<bus<<" does not exist for setting up "<<"converter_name"<<" side bus of multi_vsc-hvdc link."<<endl
                    <<"0 will be set to indicate invalid vsc-hvdc link.";
            toolkit.show_information_with_leading_time_stamp(osstream);
            ac_converter_bus_with_dc_voltage_control = 0;
        }
    }
    else
    {
        osstream<<"Warning. Zero bus number (0) is not allowed for setting up "<<"converter_name"<<" bus of vsc-hvdc link."<<endl
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
void VSC_HVDC::set_converter_bus(const unsigned int index, const unsigned int bus)
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
            osstream<<"Bus "<<bus<<" does not exist for setting up "<<"converter_name"<<" side bus of multi_vsc-hvdc link."<<endl
                    <<"0 will be set to indicate invalid vsc-hvdc link.";
            toolkit.show_information_with_leading_time_stamp(osstream);
            converters[index].converter_bus = 0;
        }
    }
    else
    {
        ostringstream osstream;
        STEPS& toolkit = get_toolkit();
        osstream<<"Warning. Zero bus number (0) is not allowed for setting up "<<"converter_name"<<" bus of vsc-hvdc link."<<endl
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
    converters[index].nominal_dc_power_in_MW = P;
}

void VSC_HVDC::set_converter_nominal_dc_voltage_command_in_kV(const unsigned int index, const double V)
{
    if(converter_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    converters[index].nominal_dc_voltage_in_kV = V;
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

void VSC_HVDC::set_converter_power_weighting_factor(const unsigned int index, const double pwf)
{
    if(converter_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    converters[index].power_weighting_factor = pwf;
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

void VSC_HVDC::set_ac_bus_number_of_dc_bus(const unsigned int index, const unsigned int bus)
{
    if(dc_bus_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    dc_buses[index].ac_bus_number = bus;
}

void VSC_HVDC::set_owner_number(const unsigned int index, const unsigned int n)
{
    if(dc_bus_index_is_out_of_range_in_function(index, __FUNCTION__))
        return;
    dc_buses[index].owner = n;
}

void VSC_HVDC::set_ground_resistance_in_ohm(const unsigned int index, const double R)
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

unsigned int VSC_HVDC::get_converter_bus(unsigned int index) const
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
        return converters[index].nominal_dc_power_in_MW;
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
        return converters[index].nominal_dc_power_in_MW/sbase;
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

double VSC_HVDC::get_converter_nominal_ac_power_command_in_Mvar(unsigned int index) const
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

double VSC_HVDC::get_converter_power_weighting_factor(unsigned int index) const
{
    if(index<get_converter_count())
        return converters[index].power_weighting_factor;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
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
        osstream<<"Error. index ("<<index<<") is out of  maximal dc bus count."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999;
    }
}

unsigned int VSC_HVDC::get_ac_bus_number_of_dc_bus(unsigned int index) const
{
    if(index<get_dc_bus_count())
        return dc_buses[index].ac_bus_number;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal dc bus count."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999;
    }
}

unsigned int VSC_HVDC::get_dc_bus_area(unsigned int index) const
{
    if(index<get_dc_bus_count())
        return dc_buses[index].dc_bus_area;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal dc bus count."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999;
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
        osstream<<"Error. index ("<<index<<") is out of  maximal dc bus count."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999;
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
        osstream<<"Error. index ("<<index<<") is out of  maximal dc bus count."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return "9999.9";
    }
}

double VSC_HVDC::get_ground_resistance_in_ohm(unsigned int index) const
{
    if(index<get_dc_bus_count())
        return dc_buses[index].converter_ground_resistance_in_ohm;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal dc bus count."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

unsigned int VSC_HVDC::get_owner_number(unsigned int index) const
{
    if(index<get_dc_bus_count())
        return dc_buses[index].owner;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal dc bus count."<<endl;
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
        osstream<<"Error. index ("<<index<<") is out of  maximal dc bus count."<<endl;
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
        osstream<<"Error. index ("<<index<<") is out of  maximal dc bus count."<<endl;
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
    unsigned int m=0;
    unsigned int n=0;
    unsigned int n_converter = converters.size();
    for(unsigned int i=0; i!=n_converter; ++i)
    {
        if (get_converter_bus(i)==bus) ++m;
        else                           ++n;
    }
    if(m>0)                            return true;
    else                               return false;
}

bool VSC_HVDC::is_in_area(unsigned int area) const
{
    unsigned int m=0;
    unsigned int n=0;
    unsigned int n_converter = converters.size();
    for(unsigned int i=0; i!=n_converter; ++i)
    {
        if (get_dc_bus_area(i)==area) ++m;
        else                          ++n;
    }
    if(m>0)                            return true;
    else                               return false;
}

bool VSC_HVDC::is_in_zone(unsigned int zone) const
{
    unsigned int m=0;
    unsigned int n=0;
    unsigned int n_converter = converters.size();
    for(unsigned int i=0; i!=n_converter; ++i)
    {
        if (get_dc_bus_zone(i)==zone) ++m;
        else                          ++n;
    }
    if(m>0)                            return true;
    else                               return false;
}

bool VSC_HVDC::is_valid() const
{
    unsigned int m=0;
    unsigned int n=0;
    unsigned int n_converter = converters.size();
    for(unsigned int i=0; i!=n_converter; ++i)
    {
        if (get_converter_bus(i)!=0)   ++m;
        else                           ++n;
    }
    if(m>0)                            return true;
    else                               return false;
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
    cout<<"get_converter_bus(1):"<<get_converter_bus(1)<<endl;
    cout<<"get_converter_dc_operation_mode(1):"<<get_converter_dc_operation_mode(1)<<endl;
    cout<<"get_converter_power_weighting_factor(1):"<<get_converter_power_weighting_factor(1)<<endl;
    cout<<"get_converter_Qmax_in_MVar(1):"<<get_converter_Qmax_in_MVar(1)<<endl;
    cout<<"get_converter_Qmin_in_MVar(1):"<<get_converter_Qmin_in_MVar(1)<<endl;
    cout<<"get_dc_bus_area(1):"<<get_dc_bus_area(1)<<endl;
    cout<<"get_dc_bus_zone(1):"<<get_dc_bus_zone(1)<<endl;
    cout<<"get_owner_number(1):"<<get_owner_number(1)<<endl;
    cout<<"get_ground_resistance_in_ohm(1):"<<get_ground_resistance_in_ohm(1)<<endl;
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
    unsigned int nbus = get_converter_count();
    for(unsigned int i=0; i<nbus; ++i)
        terminal.append_bus(get_converter_bus(i));
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

void VSC_HVDC::build_dc_network_matrix()
{

    build_inphno();

    dc_network_matrix.clear();

    build_initial_zero_matrix();

    add_dc_lines_to_dc_network();

    dc_network_matrix.compress_and_merge_duplicate_entries();
}

void VSC_HVDC::export_dc_network_matrix(string filename)
{
    dc_network_matrix.save_matrix_to_file(filename);
}

void VSC_HVDC::show_dc_network_matrix()
{
    dc_network_matrix.report_brief();
}

void VSC_HVDC::build_inphno()
{
    unsigned int ac_bus = get_ac_converter_bus_with_dc_voltage_control();
    unsigned int dc_bus_controlling_dc_voltage = 0, dc_bus_index = 0;
    unsigned int n = get_dc_bus_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        unsigned int ac_bus_temp = get_ac_bus_number_of_dc_bus(i);
        if(ac_bus_temp == ac_bus)
        {
            dc_bus_controlling_dc_voltage = get_dc_bus_number(i);
            dc_bus_index = i;
            break;
        }
    }
    if(dc_bus_controlling_dc_voltage == 0)
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. no dc bus controlling dc voltage."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);

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

bool VSC_HVDC::is_dc_network_matrix_set()
{
    unsigned int n = dc_network_matrix.get_matrix_size();
    if(n>=2) return true;
    else     return false;
}

void VSC_HVDC::build_jacobian()
{
    if(not is_dc_network_matrix_set())
        build_dc_network_matrix();

    jacobian.clear();
    STEPS& toolkit = get_toolkit();

    unsigned int nbus = get_dc_bus_count();
    for(unsigned int i=0; i!=nbus-1; ++i)
    {
        jacobian.add_entry(i, i, 0);
    }

    unsigned int n = dc_network_matrix.get_matrix_entry_count();
    unsigned int row, column;
    unsigned int ibus, jbus;
    double y;
    double Ui;
    double Uj;
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

        if(row!=column)
        {
            y = dc_network_matrix.get_real_entry_value(k);
            Ui = get_dc_voltage_of_dc_bus_number(ibus);
            der = y*Ui;
            jacobian.add_entry(row, column, der);
        }
        else
        {
            unsigned int k_start=dc_network_matrix.get_starting_index_of_column(row);
            unsigned int k_end=dc_network_matrix.get_starting_index_of_column(row+1);
            Ui=get_dc_voltage_of_dc_bus_number(ibus);
            for(unsigned int m=k_start;m!=k_end;++m)
            {
                column=dc_network_matrix.get_row_number_of_entry_index(m);
                jbus=inphno.get_physical_bus_number_of_internal_bus_number(column);
                y = dc_network_matrix.get_real_entry_value(m);
                if(y!=0)
                {
                    Uj = get_dc_voltage_of_dc_bus_number(jbus);
                    if(row!=column)
                    {
                        der+=Uj*y;
                    }
                    else
                    {
                        der+=2.0*Ui*y;
                    }
                }

            }
            jacobian.add_entry(row, row, der);
        }
        if(not std::isnan(der))
            ;
        else
        {
            osstream<<"NAN is detected when building Jacobian matrix for VSC_HVDC at row: "<<row<<"(bus "<<ibus<<"), col: "<<column<<"(bus "<<jbus<<")";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
        der = 0.0;
    }
    jacobian.compress_and_merge_duplicate_entries();
}

void VSC_HVDC::show_jacobian()
{
    jacobian.report_brief();
}

double VSC_HVDC::get_dc_voltage_of_dc_bus_number(unsigned int bus)
{
    unsigned int index = get_index_of_dc_bus_number(bus);
    //cout<<"index"<<index<<endl;
    if(index!=INDEX_NOT_EXIST)
    {
        unsigned int ac_bus = get_ac_converter_bus_with_dc_voltage_control();
        unsigned int  dc_bus_index = 0;
        unsigned int n = get_dc_bus_count();
        for(unsigned int i=0; i!=n; ++i)
        {
            unsigned int ac_bus_temp = get_ac_bus_number_of_dc_bus(i);
            if(ac_bus_temp == ac_bus)
            {
                dc_bus_index = i;
                break;
            }
        }
        if(index<dc_bus_index)
            return Udc[index];
        else
        {
            if(index>dc_bus_index)
                return Udc[index-1];
            else
                return Udc[n-1];
        }
    }
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. DC bus number "<<bus<<" does not exist in DC buses of VSC HVDC.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 0.0;
    }
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


void VSC_HVDC::set_allowed_max_active_power_in_MW(double P)
{
    if(P>0.0)
        P_threshold_in_MW = P;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<< "Non-positive "<<P<<"MW is not allowed for setting convergence threshold of"
                 "active power imbalance for powerflow solution.\n0.01MW will be set automatically.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        P_threshold_in_MW = 0.0001;
    }
}

void VSC_HVDC::solve_steady_state()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    iteration_count=0;
    build_dc_network_matrix();
    initialize_Udc_vector();
    initialize_Pdc_loss_vector();

    while(true)
    {
        update_Pdc_command_with_converter_capacity_limit();
        build_delta_p_vector();
        max_P_mismatch_in_MW = get_maximum_active_power_mismatch_in_MW();
        cout<<"max_P_mismatch_in_MW:"<<max_P_mismatch_in_MW<<endl;
        cout<<"allowed_max_active_power_imbalance_in_MW:"<<get_allowed_max_active_power_imbalance_in_MW()<<endl;
        if(max_P_mismatch_in_MW<get_allowed_max_active_power_imbalance_in_MW())
        {
            set_convergence_flag(true);
            osstream<<"multi-vsc powerflow converged within ("<<iteration_count<<") iterations."<<endl;
            toolkit.show_information_with_leading_time_stamp(osstream);
            break;
        }
        else
        {
            set_convergence_flag(false);
        }

        if(get_iteration_count()>=get_max_iteration())
        {
            osstream<<"multi-vsc powerflow failed to converge within ("<<get_max_iteration()<<") iterations."<<endl;
            toolkit.show_information_with_leading_time_stamp(osstream);
            break;
        }
        build_jacobian();
        delta_Udc = delta_power/jacobian;
        if(jacobian.is_lu_factorization_successful())
        {
            update_dc_bus_voltage();
            ++iteration_count;
        }
        else
        {
            osstream<<"multi-vsc No further powerflow solution will be attempted since LU factorization of N-R Jacobian matrix is failed."<<endl;
            toolkit.show_information_with_leading_time_stamp(osstream);
            break;
        }
        update_Pdc_loss();
    }
    save_dc_bus_powerflow_result_to_file("dc_bus_powerflow_result.csv");
    P_slack_dc_side = calculate_dc_active_power_of_slack_bus();
    unsigned int nbus=get_dc_bus_count();
    P_slack_ac_side=P_slack_dc_side+Pdc_loss[nbus-1]/1000;

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

void VSC_HVDC::initialize_Udc_vector()
{
    unsigned int dc_voltage_index = 0;
    unsigned int ncon = get_converter_count();
    unsigned int nbus = get_dc_bus_count();
    unsigned int voltage_control_ac_number = get_ac_converter_bus_with_dc_voltage_control();

    if(Udc.size()==0)
    {
        Udc.reserve(nbus);
        for(unsigned int i=0; i<nbus; ++i)
            Udc.push_back(0.0);
    }

    for(unsigned int i=0; i!=ncon; ++i)
    {
        unsigned int acbus_temp=get_converter_bus(i);
        if(voltage_control_ac_number==acbus_temp)
        {
            dc_voltage_index = i;
        }
    }

    for (unsigned int j=0; j!=nbus; ++j)
    {
        if(j==nbus-1)
        {
            Udc[j] = get_converter_nominal_dc_voltage_command_in_kV(dc_voltage_index);
        }
        else
        {
            Udc[j] = get_dc_network_base_voltage_in_kV();
        }
    }
}

void VSC_HVDC::initialize_Pdc_loss_vector()
{
    unsigned int nbus = get_dc_bus_count();
    if(Pdc_loss.size()==0)
    {
        Pdc_loss.reserve(nbus);
        for(unsigned int i=0; i<nbus; ++i)
            Pdc_loss.push_back(0.0);
    }
}


void VSC_HVDC::update_Pdc_command_with_converter_capacity_limit()
{
    ;

}

void VSC_HVDC::build_delta_p_vector()
{
    unsigned int nbus=get_dc_bus_count();

    delta_power.reserve(nbus);
    for(unsigned int i=0; i<nbus-1; ++i)
        delta_power.push_back(0.0);
    get_Pdc_command_vector();
    update_raw_dc_current_into_dc_network();
    calculate_raw_dc_power_into_dc_network();

    add_Pdc_command_to_delta_p_vector();
    add_supply_power_to_delta_p_vector();
    add_consumed_power_to_delta_p_vector();
    add_raw_dc_power_to_delta_p_vector();
    add_Pdc_loss_to_delta_p_vector();
}


void VSC_HVDC::get_Pdc_command_vector()
{
    unsigned int ncon = get_converter_count();
    unsigned int nbus = get_dc_bus_count();
    unsigned int dc_voltage_control_of_ac_number = get_ac_converter_bus_with_dc_voltage_control();
    unsigned int dc_voltage_control_index = 0;
    unsigned int ac_bus_number = 0;
    if(Pdc_command.size()==0)
    {
        Pdc_command.reserve(nbus);
        for(unsigned int i=0; i<nbus; ++i)
            Pdc_command.push_back(0.0);
    }
    for(unsigned int i=0; i!=nbus; ++i)
    {
        ac_bus_number = get_ac_bus_number_of_dc_bus(i);
        if(ac_bus_number == dc_voltage_control_of_ac_number)
        {
            dc_voltage_control_index = i;
        }
    }
    for (unsigned int i=0; i!=nbus; ++i)
    {
        ac_bus_number = get_ac_bus_number_of_dc_bus(i);
        if(i < dc_voltage_control_index)
        {
            if(ac_bus_number != 0)
            {
                for(unsigned int j=0; j!=ncon; ++j)
                {
                    if(ac_bus_number == get_converter_bus(j))
                    {
                        Pdc_command[i] = get_converter_nominal_ac_active_power_command_in_MW(j); //sbase;
                    }
                }
            }
            else
            {
                Pdc_command[i] = 0.0;
            }
        }

        if((dc_voltage_control_index < i) && (i < nbus-1))
        {
            if(ac_bus_number != 0)
            {
                for(unsigned int j=0; j!=ncon; ++j)
                {
                    if(ac_bus_number == get_converter_bus(j))
                    {
                        Pdc_command[i-1] = get_converter_nominal_ac_active_power_command_in_MW(j); //sbase;
                    }
                }
            }
            else
            {
                Pdc_command[i-1] = 0.0;
            }
        }
        else
        {
            Pdc_command[nbus-1] = 0.0;
        }

    }
}


void VSC_HVDC::calculate_raw_dc_current_into_dc_network()
{
    unsigned int nbus=get_dc_bus_count();
    if(bus_current.size()==0)
    {
        bus_current.clear();
        bus_current.reserve(nbus);
        for(unsigned int i=0;i!=nbus;++i)
        {
            bus_current.push_back(0.0);
        }
    }

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

}

void VSC_HVDC::update_raw_dc_current_into_dc_network()
{
    unsigned int nbus=get_dc_bus_count();

    bus_current.clear();
    bus_current.reserve(nbus);
    for(unsigned int i=0;i!=nbus;++i)
    {
        bus_current.push_back(0.0);
    }

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

}


void VSC_HVDC::calculate_raw_dc_power_into_dc_network()
{
    unsigned int nbus=get_dc_bus_count();
    if(bus_power.size()==0)
    {
        bus_power.clear();
        bus_power.reserve(nbus);
        for(unsigned int i=0;i!=nbus;++i)
        {
            bus_power.push_back(0.0);
        }
    }

    for(unsigned int i=0;i!=nbus;++i)
    {
        double voltage = Udc[i];
        double I = bus_current[i];
        bus_power[i] = voltage * I;
    }
}

void VSC_HVDC::add_Pdc_command_to_delta_p_vector()
{
    unsigned int nbus=get_dc_bus_count();

    delta_power.clear();
    delta_power.reserve(nbus);
    for(unsigned int i=0;i!=nbus-1;++i)
    {
        delta_power.push_back(0.0);
    }
    for(unsigned int i=0;i!=nbus-1;++i)
    {
        delta_power[i]=delta_power[i]+Pdc_command[i];
    }
}

void VSC_HVDC::add_supply_power_to_delta_p_vector()
{
    unsigned int nbus = get_dc_bus_count();
    unsigned int dc_voltage_control_of_ac_number = get_ac_converter_bus_with_dc_voltage_control();
    unsigned int dc_voltage_control_index = 0;
    unsigned int ac_bus_number = 0;
    if(supply_power.size()==0)
    {
        supply_power.reserve(nbus);
        for(unsigned int i=0; i<nbus; ++i)
            supply_power.push_back(0.0);
    }
    for(unsigned int i=0; i!=nbus; ++i)
    {
        ac_bus_number = get_ac_bus_number_of_dc_bus(i);
        if(ac_bus_number == dc_voltage_control_of_ac_number)
        {
            dc_voltage_control_index = i;
        }
    }
    for (unsigned int i=0; i!=nbus; ++i)
    {
        if(i < dc_voltage_control_index)
        {
            supply_power[i]=get_dc_bus_generation_power_in_MW(i);
        }
        if(i > dc_voltage_control_index)
        {
            supply_power[i-1]=get_dc_bus_generation_power_in_MW(i);
        }
        if(i == dc_voltage_control_index)
        {
            supply_power[nbus-1]=get_dc_bus_generation_power_in_MW(dc_voltage_control_index);
        }
    }
    for(unsigned int i=0;i!=nbus-1;++i)
    {
        delta_power[i]=delta_power[i]+supply_power[i];
    }
}

void VSC_HVDC::add_consumed_power_to_delta_p_vector()
{
    unsigned int nbus = get_dc_bus_count();
    unsigned int dc_voltage_control_of_ac_number = get_ac_converter_bus_with_dc_voltage_control();
    unsigned int dc_voltage_control_index = 0;
    unsigned int ac_bus_number = 0;
    if(consumed_power.size()==0)
    {
        consumed_power.reserve(nbus);
        for(unsigned int i=0; i<nbus; ++i)
            consumed_power.push_back(0.0);
    }
    for(unsigned int i=0; i!=nbus; ++i)
    {
        ac_bus_number = get_ac_bus_number_of_dc_bus(i);
        if(ac_bus_number == dc_voltage_control_of_ac_number)
        {
            dc_voltage_control_index = i;
        }
    }
    for (unsigned int i=0; i!=nbus; ++i)
    {
        if(i < dc_voltage_control_index)
        {
            consumed_power[i]=get_dc_bus_load_power_in_MW(i);
        }
        if(i > dc_voltage_control_index)
        {
            consumed_power[i-1]=get_dc_bus_load_power_in_MW(i);
        }
        if(i == dc_voltage_control_index)
        {
            consumed_power[nbus-1]=get_dc_bus_load_power_in_MW(dc_voltage_control_index);
        }

    }

    for(unsigned int i=0;i!=nbus-1;++i)
    {
        delta_power[i]=delta_power[i]-consumed_power[i];
    }

}

void VSC_HVDC::add_raw_dc_power_to_delta_p_vector()
{
    unsigned int nbus=get_dc_bus_count();
    for(unsigned int i=0;i!=nbus-1;++i)
    {
        delta_power[i]=delta_power[i]-bus_power[i];
    }
}

void VSC_HVDC::add_Pdc_loss_to_delta_p_vector()
{
    unsigned int nbus=get_dc_bus_count();
    for(unsigned int i=0;i!=nbus-1;++i)
    {
        delta_power[i]=delta_power[i]+Pdc_loss[i]/1000;
    }
}

void VSC_HVDC::set_convergence_flag(bool flag)
{
    converged = flag;
}

unsigned int VSC_HVDC::get_iteration_count()
{
    return iteration_count;
}

bool VSC_HVDC::check_slack_limit(double P)
{
    unsigned int dc_control_bus=get_ac_converter_bus_with_dc_voltage_control();
    unsigned int ncon=get_converter_count();
    unsigned int index=0;
    for(unsigned int i=0;i!=ncon;++i)
    {
        if(get_converter_bus(i)==dc_control_bus)
            index=i;
    }
    if(P<get_converter_rated_capacity_in_MVA(index))
        return true;
    else
        return false;
}

double VSC_HVDC::calculate_dc_active_power_of_slack_bus()
{
    double P_slack=0;
    int nsize = dc_network_matrix.get_matrix_size();
    int k_start=dc_network_matrix.get_starting_index_of_column(nsize-1);
    int k_end=dc_network_matrix.get_starting_index_of_column(nsize);
    for(int k=k_start;k!=k_end;++k)
    {
        int row=dc_network_matrix.get_row_number_of_entry_index(k);
        double yij = dc_network_matrix.get_real_entry_value(k);
        P_slack=P_slack+Udc[nsize-1]*yij*Udc[row];
    }
    return P_slack;
}

double VSC_HVDC::get_max_iteration()
{
    return max_iteration;
}

void VSC_HVDC::update_dc_bus_voltage()
{
    int nbus=get_dc_bus_count();
    for(int i=0; i!=nbus-1; ++i)
    {
        Udc[i]=Udc[i]+delta_Udc[i];
    }
}

void VSC_HVDC::update_Pdc_loss()
{
    unsigned int ncon=get_converter_count();
    unsigned int nbus=get_dc_bus_count();
    unsigned int dc_voltage_control_bus=get_ac_converter_bus_with_dc_voltage_control();
    unsigned int dc_voltage_control_index;
    for(unsigned int i=0; i!=nbus; ++i)
    {
        unsigned int ac_bus_number = get_ac_bus_number_of_dc_bus(i);
        if(ac_bus_number == dc_voltage_control_bus)
        {
            dc_voltage_control_index = i;
        }
    }
    for(unsigned int i=0; i!=ncon; ++i)
    {
        double Aloss=get_converter_loss_factor_A_in_kW(i);
        double Bloss=get_converter_loss_factor_B_in_kW_per_amp(i);
        unsigned int con_number=get_converter_bus(i);
        if(dc_voltage_control_bus==con_number)
        {
            Pdc_loss[nbus-1]=Aloss+Bloss*fabs(bus_current[nbus-1]);
        }
        else
        {
            for(unsigned int j=0; j!=nbus; ++j)
            {
                unsigned int ac_bus_number=get_ac_bus_number_of_dc_bus(j);
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

    }

}

double VSC_HVDC::calculate_dc_power_mismatch()
{
    return 0;
}

double VSC_HVDC::get_allowed_max_active_power_imbalance_in_MW()
{
    return P_threshold_in_MW;
}

double VSC_HVDC::get_maximum_active_power_mismatch_in_MW()
{
    unsigned int nbus=get_dc_bus_count();
    double max_P_error_in_MW=0.0;
    for(unsigned int i=0; i!=nbus-1; ++i)
    {
        if(fabs(delta_power[i])<max_P_error_in_MW)
            continue;
        else
        {
            max_P_error_in_MW=fabs(delta_power[i]);
        }
    }
    return max_P_error_in_MW;
}

double VSC_HVDC::get_dc_network_base_voltage_in_kV() const
{
    return dc_base_voltage_in_kV;
}

void VSC_HVDC::set_max_iteration(unsigned int iteration)
{
    max_iteration=iteration;
}

void VSC_HVDC::set_allowed_max_P_mismatch_in_MW(double max_mismatch)
{
    P_threshold_in_MW=max_mismatch;
}

double VSC_HVDC::get_allowed_max_P_mistmach_in_MW()
{
    return P_threshold_in_MW;
}


vector<double> VSC_HVDC::generate_active_power()
{
    unsigned int ncon=get_converter_count();
    if(active_power.size()==0)
    {
        active_power.reserve(ncon);
        for(unsigned int i=0; i<ncon; ++i)
            active_power.push_back(0.0);
    }

    for(unsigned int i=0;i!=ncon;++i)
    {
        if(get_converter_dc_operation_mode(i)==1)
            active_power[i]=P_slack_ac_side;
        if(get_converter_dc_operation_mode(i)==2)
            active_power[i]=get_converter_nominal_ac_active_power_command_in_MW(i);
    }
    //for(unsigned int i=0;i!=ncon;++i)
    return active_power;
}

vector<double> VSC_HVDC::generate_reactive_power()
{
    //STEPS& toolkit = get_toolkit();
    //POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    unsigned int ncon=get_converter_count();
    if(reactive_power.size()==0)
    {
        reactive_power.reserve(ncon);
        for(unsigned int i=0; i<ncon; ++i)
            reactive_power.push_back(0.0);
    }
    for(unsigned int i=0;i!=ncon;++i)
    {
        if(get_converter_ac_operation_mode(i)==1)
            reactive_power[i]=0;
        if(get_converter_ac_operation_mode(i)==2)
            reactive_power[i]=get_converter_nominal_ac_power_command_in_Mvar(i);
    }
    //for(unsigned int i=0;i!=ncon;++i)
    return reactive_power;
}

vector<double> VSC_HVDC::generate_ac_side_voltage()
{
    unsigned int ncon=get_converter_count();
    unsigned int nbus=get_dc_bus_count();
    unsigned int dc_base_voltage=get_dc_network_base_voltage_in_kV();
    if(Us.size()==0)
    {
        Us.reserve(ncon);
        for(unsigned int i=0; i<ncon; ++i)
            Us.push_back(0.0);
    }
    for(unsigned int i=0;i!=ncon;++i)
    {
        Us[i]=(Udc[i]+Pdc_loss[i]/bus_current[i]/1000)/dc_base_voltage;
    }
    for(unsigned int i=0;i!=ncon;++i)
    {
        if(i==ncon-1)
            Us[i]=(Udc[nbus-1]+Pdc_loss[nbus-1]/bus_current[nbus-1]/1000)/dc_base_voltage;
        else
            Us[i]=(Udc[i]+Pdc_loss[i]/bus_current[i]/1000)/dc_base_voltage;
    }
    return Us;
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
                <<setprecision(6)<<fixed<<get_ac_bus_number_of_dc_bus(i)<<","
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

double VSC_HVDC::get_converter_ac_bus_current(unsigned int index)
{
    //unsigned int ac_bus = get_converter_bus(index);
    return 0;
}

double VSC_HVDC::get_converter_ac_bus_power_factor(unsigned int index)
{
    return 0;
}

double VSC_HVDC::get_converter_dc_bus_current(unsigned int index)
{
    return 0;
}

double VSC_HVDC::get_converter_dc_voltage(unsigned int index)
{
    return 0;
}

double VSC_HVDC::get_converter_dc_power(unsigned int index)
{
    return 0;
}

double VSC_HVDC::get_converter_ac_active_power(unsigned int index)
{
    return 0;
}

double VSC_HVDC::get_converter_ac_reactive_power(unsigned int index)
{
    return 0;
}

double VSC_HVDC::get_converter_ac_bus_voltage(unsigned int index)
{
    return 0;
}

double VSC_HVDC::get_converter_ac_bus_angle(unsigned int index)
{
    return 0;
}

double VSC_HVDC::get_converter_ac_bus_complex_voltage(unsigned int index)
{
    return 0;
}

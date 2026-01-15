#include "header/device/mt_hvdc.h"
#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include "header/STEPS.h"
#include <istream>
#include <iostream>
#include <cstdio>

using namespace std;

MT_HVDC::MT_HVDC(STEPS& toolkit) : NONBUS_DEVICE(toolkit)
{
  clear();
}


MT_HVDC::~MT_HVDC()
{
    ;
}



bool MT_HVDC::is_connected_to_bus(unsigned int bus) const
{

    unsigned int n_lcc = Hlcc.size();
    for(unsigned int i=0;i<n_lcc;i++)
    {
        unsigned int ac_bus = Hlcc[i].get_converter_ac_bus();
        if(ac_bus==bus)
        {
            return true;
        }
    }

    unsigned int n_vsc = Hvsc.size();
    for(unsigned int i=0;i<n_vsc;i++)
    {
        unsigned int ac_bus = Hvsc[i].get_converter_ac_bus();
        if(ac_bus==bus)
        {
            return true;
        }
    }
    return false;
}

bool MT_HVDC::is_lccs_connected_to_bus(unsigned int bus) const
{
    unsigned int n_lcc = Hlcc.size();
    for(unsigned int i=0;i<n_lcc;i++)
    {
        unsigned int ac_bus = Hlcc[i].get_converter_ac_bus();
        if(ac_bus==bus)
        {
            return true;
        }
    }
    return false;
}
bool MT_HVDC::is_vscs_connected_to_bus(unsigned int bus) const
{
    unsigned int n_vsc = Hvsc.size();
    for(unsigned int i=0;i<n_vsc;i++)
    {
        unsigned int ac_bus = Hvsc[i].get_converter_ac_bus();
        if(ac_bus==bus)
        {
            return true;
        }
    }
    return false;
}

bool MT_HVDC::is_connected_to_dcbus(unsigned int dcbus) const
{
    unsigned int n = Dcbus.size();
    for(unsigned int i=0;i<n;i++)
    {
        unsigned int dcbus_number = Dcbus[i].get_dc_bus_number();
        if(dcbus_number==dcbus)
        {
            return true;
        }
    }
    return false;
}

bool MT_HVDC::is_in_area(unsigned int area) const
{
    return false;
}

bool MT_HVDC::is_in_zone(unsigned int zone) const
{
    return false;
}

bool MT_HVDC::is_valid() const
{
    unsigned int n_lcc = Hlcc.size();
    for(unsigned int i=0;i<n_lcc;i++)
    {
        if(Hlcc[i].get_converter_ac_bus()==0)
            return false;
    }

    unsigned int n_vsc = Hvsc.size();
    for(unsigned int i=0;i<n_vsc;i++)
    {
        if(Hvsc[i].get_converter_ac_bus()==0)
            return false;
    }

    unsigned int n_dcbus = Dcbus.size();
    for(unsigned int i=0;i<n_dcbus;i++)
    {
        if(Dcbus[i].get_dc_bus_number()==0)
            return false;
    }
    return true;
}

void MT_HVDC::check()
{
    ;
}

void MT_HVDC::clear()
{
    set_lcc_converter_count(0);
    set_dc_bus_count(0);
    set_dc_line_count(0);

    inphno.clear();
    dc_network_matrix.clear();
    jacobian.clear();

    max_iteration = 10;
    allowed_power_mismatch = 0.001;

    mtdc_network_model = NULL;
}

void MT_HVDC::report() const
{
    cout<<"get_name():"<<get_name()<<endl;
    cout<<"get_dc_terminal_count():"<<get_dc_terminal_count()<<endl;
    cout<<"get_dc_station_count():"<<get_dc_station_count()<<endl;
    cout<<"get_lcc_converter_count():"<<get_lcc_converter_count()<<endl;
    cout<<"get_vsc_converter_count():"<<get_vsc_converter_count()<<endl;
    cout<<"get_dc__count():"<<get_dc_bus_count()<<endl;
    cout<<"get_dc_line_count():"<<get_dc_line_count()<<endl;
    cout<<"get_project_status():"<<get_status()<<endl;
    unsigned int nter=get_dc_terminal_count();
    unsigned int nsta=get_dc_station_count();
    unsigned int nlcc=get_lcc_converter_count();
    unsigned int nvsc=get_vsc_converter_count();
    unsigned int ndcbus=get_dc_bus_count();
    unsigned int ndcline=get_dc_line_count();

    for(unsigned int i=0;i!=nter;++i)
    {
        cout<<"get_dc_terminal_number()"<<dc_terminals[i].get_terminal_number()<<endl;
        cout<<"get_dc_terminal_dc_bus(): "<<dc_terminals[i].get_terminal_dc_bus_number()<<endl;
        switch(dc_terminals[i].get_terminal_mode())
        {
            case DC_P_TERMIANL:
                cout<<"get_dc_terminal_mode(): "<<"P mode"<<endl;
                break;
            case DC_I_TERMINAL:
                cout<<"get_dc_terminal_mode(): "<<"I mode"<<endl;
                break;
            case DC_U_TERMINAL:
                cout<<"get_dc_terminal_mode(): "<<"U mode"<<endl;
                break;
            case DC_ANGLE_TERMINAL:
                cout<<"get_dc_terminal_mode(): "<<"Angle mode"<<endl;
                break;
            default:
                cout<<"get_dc_terminal_mode(): "<<"Invalid mode"<<endl;
                break;
        }
    }
    for(unsigned int i=0;i!=nsta;++i)
    {
        cout<<"get_dc_station_number()"<<dc_stations[i].get_station_number()<<endl;
        cout<<"get_dc_station_terminal(): "<<dc_stations[i].get_terminal_number()<<endl;
        switch(dc_stations[i].get_station_mode())
        {
            case DC_P_STATION:
                cout<<"get_dc_station_mode(): "<<"P mode"<<endl;
                break;
            case DC_I_STATION:
                cout<<"get_dc_station_mode(): "<<"I mode"<<endl;
                break;
            case DC_U_STATION:
                cout<<"get_dc_station_mode(): "<<"U mode"<<endl;
                break;
            case DC_ANGLE_STATION:
                cout<<"get_dc_station_mode(): "<<"Angle mode"<<endl;
                break;
            default:
                cout<<"get_dc_station_mode(): "<<"Invalid mode"<<endl;
                break;
        }

        switch(dc_stations[i].get_station_structure())
        {
            case LCC_CASCADE:
                cout<<"get_dc_station_type(): "<<"LCC_CASCADE"<<endl;
                break;
            case VSC_PARALLEL:
                cout<<"get_dc_station_type(): "<<"VSC_PARALLEL"<<endl;
                break;
            case LCC_HCM:
                cout<<"get_dc_station_type(): "<<"LCC_HCM"<<endl;
                break;
            default:
                cout<<"get_dc_station_type(): "<<"Invalid type"<<endl;
                break;
        }
    }

    for(unsigned int i=0;i!=nlcc;++i)
    {
        cout<<"get_lcc_ac_bus()"<<Hlcc[i].get_converter_ac_bus()<<endl;
        cout<<"get_lcc_name()"<<Hlcc[i].get_converter_name()<<endl;
        cout<<"get_lcc_station_number(): "<<Hlcc[i].get_converter_station_number()<<endl;
        switch(Hlcc[i].get_converter_operating_mode())
        {
            case LCC_RECTIFIER:
                cout<<"get_lcc_operation_mode(): "<<"RECTIFIER"<<endl;
                break;
            case LCC_INVERTER:
                cout<<"get_lcc_operation_mode(): "<<"INVERTER"<<endl;
                break;
            default:
                cout<<"get_lcc_operation_mode(): "<<"Invalid mode"<<endl;
                break;
        }
        switch(Hlcc[i].get_control_mode())
        {
            case LCC_CONSTANT_POWER_MODE:
                cout<<"get_lcc_control_mode(): "<<"P mode"<<endl;
                break;
            case LCC_CONSTANT_CURRENT_MODE:
                cout<<"get_lcc_control_mode(): "<<"I mode"<<endl;
                break;
            case LCC_CONSTANT_VOLTAGE_MODE:
                cout<<"get_lcc_control_mode(): "<<"U mode"<<endl;
                break;
            case LCC_CONSTANR_FIRING_ANGLE_MODE:
                cout<<"get_lcc_control_mode(): "<<"Angle mode"<<endl;
                break;
            default:
                cout<<"get_lcc_control_mode(): "<<"Invalid mode"<<endl;
                break;
        }
        cout<<"get_lcc_pdc_command(): "<<Hlcc[i].get_command_pdc_in_MW()<<endl;
        cout<<"get_lcc_idc_command(): "<<Hlcc[i].get_command_idc_in_kA()<<endl;
        cout<<"get_lcc_udc_command(): "<<Hlcc[i].get_command_udc_in_kV()<<endl;
        cout<<"get_lcc_angle_command(): "<<Hlcc[i].get_command_firing_angle()<<endl;

    }

    for(unsigned int i=0;i!=nvsc;++i)
    {
        cout<<"get_vsc_ac_bus()"<<Hvsc[i].get_converter_ac_bus()<<endl;
        cout<<"get_vsc_name()"<<Hvsc[i].get_converter_name()<<endl;
        cout<<"get_vsc_station_number(): "<<Hvsc[i].get_converter_station_number()<<endl;
        switch(Hvsc[i].get_converter_d_axis_control_mode())
        {
            case VSC_CONSTANT_AC_ACTIVE_POWER_MODE:
                cout<<"get_vsc_d_axis_control_mode(): "<<"PAC mode"<<endl;
                break;
            case  VSC_CONSTANT_DC_VOLTAGE_MODE:
                cout<<"get_vsc_d_axis_control_mode(): "<<"UDC mode"<<endl;
                break;
            default:
                cout<<"get_vsc_d_axis_control_mode(): "<<"Invalid mode"<<endl;
                break;
        }
        switch(Hvsc[i].get_converter_q_axis_control_mode())
        {
            case VSC_CONSTANT_AC_REACTIVE_POWER_MODE:
                cout<<"get_vsc_q_axis_control_mode(): "<<"QAC mode"<<endl;
                break;
            case VSC_CONSTANT_AC_VOLTAGE_MODE:
                cout<<"get_vsc_q_axis_control_mode(): "<<"UAC mode"<<endl;
                break;
            default:
                cout<<"get_vsc_q_axis_control_mode(): "<<"Invalid mode"<<endl;
                break;
        }
        cout<<"get_vsc_pac_command(): "<<Hvsc[i].get_converter_Pac_command()<<endl;
        cout<<"get_vsc_qac_command(): "<<Hvsc[i].get_converter_Qac_command()<<endl;
        cout<<"get_vsc_udc_command(): "<<Hvsc[i].get_converter_Udc_command()<<endl;
        cout<<"get_vsc_uac_command(): "<<Hvsc[i].get_converter_Us_command()<<endl;
    }

    for(unsigned int i=0;i!=ndcbus;++i)
    {
        cout<<"get_dc_bus_number()"<<Dcbus[i].get_dc_bus_number()<<endl;
        cout<<"get_dc_bus_name(): "<<Dcbus[i].get_name()<<endl;
    }
    for(unsigned int i=0;i!=ndcline;++i)
    {
        cout<<"get_dc_line_sending_side_bus(): "<<Dcline[i].get_sending_side_bus_number()<<endl;
        cout<<"get_dc_line_receiving_side_bus(): "<<Dcline[i].get_receiving_side_bus_number()<<endl;
        cout<<"get_dc_line_name(): "<<Dcline[i].get_dc_line_name()<<endl;
        cout<<"get_dc_line_resistance_in_ohm(): "<<Dcline[i].get_line_R_in_ohm()<<endl;
        cout<<"get_dc_line_inductance_in_H(): "<<Dcline[i].get_line_L_in_H()<<endl;
        cout<<"get_dc_line_capacitor_in_uF(): "<<Dcline[i].get_line_C_in_uF()<<endl;
    }

}

void MT_HVDC::save() const
{
    ;
}

 DEVICE_ID MT_HVDC::get_device_id() const
{
    DEVICE_ID did;
    did.set_device_type(STEPS_HYBRID_DC);
    /*

    TERMINAL terminal;
    unsigned int n_lcc = get_lcc_converter_count();
    for(unsigned int i=0;i<n_lcc;i++)
    {
        unsigned int bus = Hlcc[i].get_converter_ac_bus();
        if(not terminal.has_bus(bus))
            terminal.append_bus(bus);
    }
    unsigned int n_vsc = get_vsc_converter_count();
    for(unsigned int i=0;i<n_vsc;i++)
    {
        unsigned int bus = Hvsc[i].get_converter_ac_bus();
        if(not terminal.has_bus(bus))
            terminal.append_bus(bus);
    }
    */
    did.set_device_name_index(get_name_index());
    did.set_device_identifier_index(get_identifier_index());
    return did;
}

void MT_HVDC::set_model(MODEL* model_ptr)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    if(model_ptr != NULL)
    {
        MODEL& model = *model_ptr;
        if(model.has_allowed_device_type(STEPS_HYBRID_DC))
        {
            model.set_device_id(get_device_id());
            if(model.get_model_type()=="MT HVDC LCC MODEL")
            {
                set_lcc_converter_model((HLCC_MODEL*) model_ptr);
                model.allocate_model_variables();
                model.prepare_model_data_table();
                model.prepare_model_internal_variable_table();
                return;
            }
            if(model.get_model_type()=="MT HVDC VSC MODEL")
            {
                set_vsc_converter_model((HVSC_MODEL*) model_ptr);
                model.allocate_model_variables();
                model.prepare_model_data_table();
                model.prepare_model_internal_variable_table();
                return;
            }
            if(model.get_model_type()=="MT DC GRID MODEL")
            {
                set_dc_network_model((MT_DC_GRID_MODEL*) model_ptr);
                model.allocate_model_variables();
                model.prepare_model_data_table();
                model.prepare_model_internal_variable_table();
                return;
            }
            osstream<<"Waring. Multi-terminal HVDC model does not support device type of "<<device_type2string(STEPS_HYBRID_DC)
                    <<" when trying to set dynamic model for "<<get_compound_device_name();
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"Waring. Multi-terminal HVDC model is not given to set dynamic model for "<<get_compound_device_name();
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void MT_HVDC::set_lcc_converter_model(HLCC_MODEL* model_ptr)
{
    if(model_ptr!=NULL)
    {
        HLCC_MODEL& model = *model_ptr;
        string converter_name = model.get_model_converter_name();
        HLCC* lcc_ptr = get_lcc_converter_with_name(converter_name);
        model.set_hlcc_pointer(lcc_ptr);
        HLCC& lcc = *lcc_ptr;
        lcc.set_model(model_ptr);
    }
    else
    {
        ostringstream osstream;
        osstream<<"Waring. LCC model is not given to set dynamic model for LCC converter";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void MT_HVDC::set_vsc_converter_model(HVSC_MODEL* model_ptr)
{
    if(model_ptr!=NULL)
    {
        HVSC_MODEL& model = *model_ptr;
        string converter_name = model.get_model_converter_name();
        HVSC* vsc_ptr = get_vsc_converter_with_name(converter_name);
        model.set_hvsc_pointer(vsc_ptr);
        HVSC& vsc = *vsc_ptr;
        vsc.set_model(model_ptr);
    }
    else
    {
        ostringstream osstream;
        osstream<<"Waring. VSC model is not given to set dynamic model for VSC converter";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}
void MT_HVDC::set_dc_network_model(MT_DC_GRID_MODEL* model_ptr)
{
    if(model_ptr!=NULL)
        mtdc_network_model = model_ptr;
}

MODEL* MT_HVDC::get_model_of_type(string model_type, unsigned int index)
{
    model_type = string2upper(model_type);
    if(model_type=="MT HVDC LCC MODEL")
        return get_lcc_model_with_index(index);
    if(model_type=="MT HVDC VSC MODEL")
        return get_vsc_model_with_index(index);
    if(model_type=="MT DC GRID MODEL")
        return get_mtdc_network_model();

    return NULL;
}

MT_DC_GRID_MODEL* MT_HVDC::get_mtdc_network_model()
{
    return mtdc_network_model;
}

void MT_HVDC::set_status(const bool status)
{
    this->status = status;
}

bool MT_HVDC::get_status() const
{
    return status;
}

void MT_HVDC::set_dc_terminal_count(unsigned int n)
{
    STEPS& toolkit = get_toolkit();
    dc_terminals.clear();
    DC_TERMINAL dc_terminal(toolkit);
    for(unsigned int i=0;i<n;i++)
    {
        dc_terminals.push_back(dc_terminal);
    }
}

void MT_HVDC::set_dc_station_count(unsigned int n)
{
    STEPS& toolkit = get_toolkit();
    dc_stations.clear();
    DC_STATION dc_station(toolkit);
    for(unsigned int i=0;i<n;i++)
    {
        dc_stations.push_back(dc_station);
    }
}

void MT_HVDC::set_lcc_converter_count(unsigned int n)
{
    Hlcc.clear();
    Hlcc.reserve(n);
    STEPS& toolkit = get_toolkit();
    HLCC hlcc(toolkit);
    for(unsigned int i=0;i<n;i++)
    {
        Hlcc.push_back(hlcc);
    }
}

void MT_HVDC::set_vsc_converter_count(unsigned int n)
{
    STEPS& toolkit = get_toolkit();
    Hvsc.clear();
    Hvsc.reserve(n);
    HVSC hvsc(toolkit);
    for(unsigned int i=0;i<n;i++)
    {
        Hvsc.push_back(hvsc);
    }
}

void MT_HVDC::set_dc_bus_count(unsigned int n)
{
    Dcbus.clear();
    Dcbus.reserve(n);
    STEPS& toolkit = get_toolkit();
    DC_BUS dcbus(toolkit);
    for(unsigned int i=0;i<n;i++)
    {
        Dcbus.push_back(dcbus);
    }
}

void MT_HVDC::set_dc_line_count(unsigned int n)
{
    Dcline.clear();
    Dcline.reserve(n);
    STEPS& toolkit = get_toolkit();
    DC_LINE dcline(toolkit);
    for(unsigned int i=0;i<n;i++)
    {
        Dcline.push_back(dcline);
    }
}

void MT_HVDC::set_dc_base_voltage(const double vbase)
{
    dc_base_voltage = vbase;
}

void MT_HVDC::set_dc_system_max_iteration_in_powerflow(const unsigned int n)
{
    max_iteration = n;
    unsigned int n_vsc = Hvsc.size();
    for(unsigned int i=0;i!=n_vsc;i++)
    {
        Hvsc[i].set_max_iteration_of_voltage_station_solution(n);
    }
}

void MT_HVDC::set_dc_system_allow_power_mismatch_in_powerflow(const double pmis)
{
    allowed_power_mismatch = pmis;
    unsigned int n_vsc = Hvsc.size();
    for(unsigned int i=0;i!=n_vsc;i++)
    {
        Hvsc[i].set_allowed_max_p_in_MW_of_voltage_station_solution(pmis);
    }
}

unsigned int MT_HVDC::get_dc_terminal_count() const
{
    return dc_terminals.size();
}

unsigned int MT_HVDC::get_dc_station_count() const
{
    return dc_stations.size();
}

unsigned int MT_HVDC::get_lcc_converter_count() const
{
    return Hlcc.size();
}
 unsigned int MT_HVDC::get_vsc_converter_count() const
 {
     return Hvsc.size();
 }
unsigned int MT_HVDC::get_dc_bus_count() const
{
    return Dcbus.size();
}

unsigned int MT_HVDC::get_dc_line_count() const
{
    return Dcline.size();
}

double MT_HVDC::get_dc_base_voltage() const
{
    return dc_base_voltage;
}

unsigned int MT_HVDC::get_dc_system_max_iteration_in_powerflow() const
{
    return max_iteration;
}

double MT_HVDC::get_dc_system_allow_power_mismatch_in_powerflow() const
{
    return allowed_power_mismatch;
}

bool MT_HVDC::dc_terminal_index_is_out_of_range_in_function(const unsigned int index, const string& func) const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    if(index>=get_dc_terminal_count())
    {
        osstream<<"MULTI-TERMINAL DC project:"<<get_name()<<" DC TERMINAL index "<<index<<" exceeds terminal count "<<get_dc_terminal_count()
                <<" when calling MT_HVDC::"<<func<<"()"<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return true;
    }
    else
        return false;
}

bool MT_HVDC::dc_station_index_is_out_of_range_in_function(const unsigned int index, const string& func) const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    if(index>=get_dc_station_count())
    {
        osstream<<"MULTI-TERMINAL DC project:"<<get_name()<<" DC STATION index "<<index<<" exceeds station count "<<get_dc_station_count()
                <<" when calling MT_HVDC::"<<func<<"()"<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return true;
    }
    else
        return false;
}


bool MT_HVDC::lcc_index_is_out_of_range_in_function(const unsigned int index, const string& func) const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    if(index>=get_lcc_converter_count())
    {
        osstream<<"MULTI-TERMINAL DC project:"<<get_name()<<" LCC index "<<index<<" exceeds lcc count "<<get_lcc_converter_count()
                <<" when calling MT_HVDC::"<<func<<"()"<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return true;
    }
    else
        return false;
}

bool MT_HVDC::vsc_index_is_out_of_range_in_function(const unsigned int index, const string& func) const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    if(index>=get_vsc_converter_count())
    {
        osstream<<"MULTI-TERMINAL DC project:"<<get_name()<<" VSC index "<<index<<" exceeds vsc count "<<get_vsc_converter_count()
                <<" when calling MT_HVDC::"<<func<<"()"<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return true;
    }
    else
        return false;
}

bool MT_HVDC::dc_bus_index_is_out_of_range_in_function(const unsigned int index, const string& func) const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    if(index>=get_dc_bus_count())
    {
        osstream<<"MULTI-TERMINAL DC project:"<<get_name()<<" DC BUS index "<<index<<" exceeds dc bus count "<<get_dc_bus_count()
                <<" when calling MT_HVDC::"<<func<<"()"<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return true;
    }
    else
        return false;;
}

bool MT_HVDC::dc_line_index_is_out_of_range_in_function(const unsigned int index, const string& func) const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    if(index>=get_dc_line_count())
    {
        osstream<<"MULTI-TERMINAL DC project:"<<get_name()<<" DC LINE index "<<index<<" exceeds dc line count"<<get_dc_line_count()
                <<" when calling MT_HVDC::"<<func<<"()"<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return true;
    }
    else
        return false;
}

string MT_HVDC::get_identifier() const
{
    return get_string_of_index(identifier_index);
}

unsigned int MT_HVDC::get_identifier_index() const
{
    return identifier_index;
}

unsigned int MT_HVDC::get_name_index() const
{
    return name_index;
}

void MT_HVDC::set_identifier(const string hdc_id)
{
    string hdc_id_temp = trim_string(hdc_id);
    identifier_index = get_index_of_string(hdc_id_temp);
}

void MT_HVDC::set_name(const string name)
{
    string new_name = trim_string(name);
    name_index = get_index_of_string(name);
}

string MT_HVDC::get_name() const
{
    return get_string_of_index(name_index);
}

//set terminal
void MT_HVDC::set_terminal_name(const unsigned int index,const string name)
{
    if(dc_terminal_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    dc_terminals[index].set_terminal_name(name);
}

void MT_HVDC::set_terminal_number(const unsigned int index,const unsigned int terminal_number)
{
    if(dc_terminal_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    dc_terminals[index].set_terminal_number(terminal_number);
}

void MT_HVDC::set_terminal_dc_bus_number(const unsigned int index,const unsigned int dcbus)
{
    if(dc_terminal_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    dc_terminals[index].set_terminal_dc_bus_number(dcbus);
}

void MT_HVDC::set_termianl_status(const unsigned int index,const bool status)
{
    if(dc_terminal_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    dc_terminals[index].set_status(status);
}

void MT_HVDC::set_terminal_mode(const unsigned int index,const DC_TERMINAL_MODE mode)
{
    if(dc_terminal_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    dc_terminals[index].set_terminal_mode(mode);
}

//get terminal
vector<DC_TERMINAL*> MT_HVDC::get_all_dc_terminals()
{
    vector<DC_TERMINAL*> all_terminals;
    unsigned int n = get_dc_terminal_count();
    all_terminals.reserve(n);
    for(unsigned int i=0;i!=n;i++)
    {
        all_terminals.push_back(&(dc_terminals[i]));
    }
    return all_terminals;
}

DC_TERMINAL* MT_HVDC::get_dc_terminal_with_name(string name)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    vector<DC_TERMINAL*> terminals = get_all_dc_terminals();
    unsigned int n = terminals.size();
    for(unsigned int i=0;i!=n;i++)
    {
        DC_TERMINAL& terminal = *(terminals[i]);
        string tname= terminal.get_terminal_name();
        if(tname==name)
            return terminals[i];
    }
    osstream<<"No terminal with terminal name: "<<name;
    toolkit.show_information_with_leading_time_stamp(osstream);
    return NULL;;
}

DC_TERMINAL* MT_HVDC::get_dc_terminal_with_index(unsigned int index)
{
    if(not dc_terminal_index_is_out_of_range_in_function(index,__FUNCTION__))
        return &(dc_terminals[index]);
    else
        return NULL;
}

DC_TERMINAL* MT_HVDC::get_dc_terminal_with_terminal_number(unsigned int terminal_number)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    vector<DC_TERMINAL*> terminals = get_all_dc_terminals();
    unsigned int n = terminals.size();
    for(unsigned int i=0;i!=n;i++)
    {
        DC_TERMINAL& terminal = *(terminals[i]);
        unsigned int terminal_num = terminal.get_terminal_number();
        if(terminal_num==terminal_number)
            return terminals[i];
    }
    osstream<<"NO TERMIANL with TERMINAL NUMBER: "<<terminal_number;
    toolkit.show_information_with_leading_time_stamp(osstream);
    return NULL;
}

DC_TERMINAL* MT_HVDC::get_dc_terminal_with_dc_bus_number(unsigned int dc_bus_number)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    vector<DC_TERMINAL*> terminals = get_all_dc_terminals();
    unsigned int n = terminals.size();
    for(unsigned int i=0;i!=n;i++)
    {
        DC_TERMINAL& terminal = *(terminals[i]);
        unsigned int dcbus_num = terminal.get_terminal_dc_bus_number();
        if(dcbus_num==dc_bus_number)
            return terminals[i];
    }
    osstream<<"NO TERMIANL CONNECTED TO DC BUS: "<<dc_bus_number;
    toolkit.show_information_with_leading_time_stamp(osstream);
    return NULL;
}

string MT_HVDC::get_terminal_name(unsigned int index) const
{
    if(dc_terminal_index_is_out_of_range_in_function(index,__FUNCTION__))
        return "";
    return dc_terminals[index].get_terminal_name();
}

unsigned int MT_HVDC::get_terminal_index_with_name(string name)
{
    unsigned int n_terminal = dc_terminals.size();
    for(unsigned int i=0;i!=n_terminal;i++)
    {
        string terminal_name = dc_terminals[i].get_terminal_name();
        if(terminal_name==name)
            return i;
    }
    return INDEX_NOT_EXIST;
}

unsigned int MT_HVDC::get_terminal_number(unsigned int index) const
{
    if(dc_terminal_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0;
    return dc_terminals[index].get_terminal_number();
}

unsigned int MT_HVDC::get_terminal_dc_bus_number(unsigned int index) const
{
    if(dc_terminal_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0;
    return dc_terminals[index].get_terminal_dc_bus_number();
}

bool MT_HVDC::get_terminal_status(unsigned int index) const
{
    if(dc_terminal_index_is_out_of_range_in_function(index,__FUNCTION__))
        return false;
    return dc_terminals[index].get_status();
}

DC_TERMINAL_MODE MT_HVDC::get_terminal_mode(unsigned int index) const
{
    if(dc_terminal_index_is_out_of_range_in_function(index,__FUNCTION__))
        return DC_INVALID_TERMINAL;
    return dc_terminals[index].get_terminal_mode();
}

//set station
 void MT_HVDC::set_station_name(unsigned int index,const string name)
 {
     if(dc_station_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
     dc_stations[index].set_station_name(name);
 }
void MT_HVDC::set_station_number(unsigned int index,const unsigned int station_number)
{
    if(dc_station_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    dc_stations[index].set_station_number(station_number);
}

void MT_HVDC::set_station_terminal_number(unsigned int index,const unsigned int terminal_number)
{
    if(dc_station_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    dc_stations[index].set_terminal_number(terminal_number);
}

void MT_HVDC::set_station_status(unsigned int index,const bool status)
{
    if(dc_station_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    dc_stations[index].set_status(status);
}

void MT_HVDC::set_station_structure(unsigned int index,const DC_STATION_STRUCTURE structure)
{
    if(dc_station_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    dc_stations[index].set_station_structure(structure);
}

void MT_HVDC::set_station_mode(unsigned int index,const DC_STATION_MODE mode)
{
    if(dc_station_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    dc_stations[index].set_station_mode(mode);
}

void MT_HVDC::set_station_cascade_lcc_number(unsigned int index,const unsigned int n_lcc)
{
    if(dc_station_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    dc_stations[index].set_station_cascade_lcc_number(n_lcc);
}

void MT_HVDC::set_station_parallel_vsc_number(unsigned int index,const unsigned int n_vsc)
{
    if(dc_station_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    dc_stations[index].set_station_parallel_vsc_number(n_vsc);
}

//get station
vector<DC_STATION*> MT_HVDC::get_all_dc_stations()
{
    vector<DC_STATION*> all_stations;
    unsigned int n = get_dc_station_count();
    all_stations.reserve(n);
    for(unsigned int i=0;i!=n;i++)
    {
        all_stations.push_back(&(dc_stations[i]));
    }
    return all_stations;
}

vector<DC_STATION*> MT_HVDC::get_dc_stations_connect_to_terminal(unsigned int terminal_number)
{
    vector<DC_STATION*> stations;
    stations.clear();

    unsigned int n_station = dc_stations.size();
    for(unsigned int i=0;i!=n_station;i++)
    {
        unsigned int station_terminal = dc_stations[i].get_terminal_number();
        if(station_terminal==terminal_number)
        {
            stations.push_back(&(dc_stations[i]));
        }
    }
    return stations;
}

DC_STATION* MT_HVDC::get_dc_station_with_station_number(unsigned int station_number)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    vector<DC_STATION*> all_stations = get_all_dc_stations();
    unsigned int n = all_stations.size();
    for(unsigned int i=0;i!=n;i++)
    {
        DC_STATION& station = *(all_stations[i]);
        unsigned int number = station.get_station_number();
        if(number==station_number)
            return all_stations[i];
    }
    osstream<<"NO DC STATION with STATION NUMBER "<<station_number;
    toolkit.show_information_with_leading_time_stamp(osstream);
    return NULL;
}

DC_STATION* MT_HVDC::get_dc_station_with_name(string name)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    vector<DC_STATION*> all_stations = get_all_dc_stations();
    unsigned int n = all_stations.size();
    for(unsigned int i=0;i!=n;i++)
    {
        DC_STATION& station = *(all_stations[i]);
        string station_name = station.get_station_name();
        if(station_name==name)
            return all_stations[i];
    }
    osstream<<"NO DC STATION with STATION NAME "<<name;
    toolkit.show_information_with_leading_time_stamp(osstream);
    return NULL;
}

DC_STATION* MT_HVDC::get_dc_station_with_index(unsigned int index)
{
    if(not dc_station_index_is_out_of_range_in_function(index,__FUNCTION__))
        return &(dc_stations[index]);
    else
        return NULL;
}

string MT_HVDC::get_station_name(unsigned int index) const
{
    if(dc_station_index_is_out_of_range_in_function(index,__FUNCTION__))
        return "";
    return dc_stations[index].get_station_name();
}

unsigned int MT_HVDC::get_station_index_with_name(string name) const
{
    unsigned int n_station = dc_stations.size();
    for(unsigned int i=0;i!=n_station;i++)
    {
        if(dc_stations[i].get_station_name()==name)
            return i;
    }
    return INDEX_NOT_EXIST;
}

unsigned int MT_HVDC::get_station_number(unsigned int index) const
{
    if(dc_station_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0;
    return dc_stations[index].get_station_number();
}

unsigned int MT_HVDC::get_station_terminal_number(unsigned int index) const
{
    if(dc_station_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0;
    return dc_stations[index].get_terminal_number();
}

bool MT_HVDC::get_station_status(unsigned int index) const
{
    if(dc_station_index_is_out_of_range_in_function(index,__FUNCTION__))
        return false;
    return dc_stations[index].get_status();
}

DC_STATION_STRUCTURE MT_HVDC::get_station_structure(unsigned int index) const
{
    if(dc_station_index_is_out_of_range_in_function(index,__FUNCTION__))
        return INVALID_DC_STATION_STRUCTURE;
    return dc_stations[index].get_station_structure();
}

DC_STATION_MODE MT_HVDC::get_station_mode(unsigned int index) const
{
    if(dc_station_index_is_out_of_range_in_function(index,__FUNCTION__))
        return DC_INVALID_STATION;
    return dc_stations[index].get_station_mode();
}

unsigned int MT_HVDC::get_station_cascade_lcc_number(unsigned int index) const
{
    if(dc_station_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0;
    return dc_stations[index].get_station_cascade_lcc_number();
}

unsigned int MT_HVDC::get_station_parallel_vsc_number(unsigned int index) const
{
    if(dc_station_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0;
    return dc_stations[index].get_station_parallel_vsc_number();
}

//set lcc
void MT_HVDC::set_lcc_converter_status(const unsigned int index,const bool status)
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hlcc[index].set_status(status);
}

void  MT_HVDC::set_lcc_converter_name(const unsigned int index,const string name)
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hlcc[index].set_converter_name(name);
}

void MT_HVDC::set_lcc_converter_ac_bus(const unsigned int index,const unsigned int bus)
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hlcc[index].set_converter_ac_bus(bus);
}

void MT_HVDC::set_lcc_converter_station_number(const unsigned int index,const unsigned int station)
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hlcc[index].set_converter_station_number(station);
}

void MT_HVDC::set_lcc_converter_dc_bus(const unsigned int index,const unsigned int bus)
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hlcc[index].set_converter_dc_bus(bus);
}

void MT_HVDC::set_lcc_converter_operating_mode(const unsigned int index,const HLCC_OPERATION_MODE omode)
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hlcc[index].set_operating_mode(omode);
}

void MT_HVDC::set_lcc_converter_brideg_number(const unsigned int index,const unsigned int nbridge)
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hlcc[index].set_brideg_number(nbridge);
}

void MT_HVDC::set_lcc_converter_voltage_drop_per_bridge_in_kV(const unsigned int index,const double vdrop)
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hlcc[index].set_voltage_drop_per_bridge_in_kV(vdrop);
}

void MT_HVDC::set_lcc_converter_line_smoothing_L_in_H(const unsigned int index,const double lsmooth)
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hlcc[index].set_line_smoothing_L_in_H(lsmooth);
}

void MT_HVDC::set_lcc_converter_line_smoothing_R_in_ohm(const unsigned int index,const double rsmooth)
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hlcc[index].set_line_smoothing_R_in_ohm(rsmooth);
}

void MT_HVDC::set_lcc_converter_transformer_grid_side_base_voltage_in_kV(const unsigned int index,const double vbase)
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hlcc[index].set_converter_transformer_grid_side_base_voltage_in_kV(vbase);
}

void MT_HVDC::set_lcc_converter_transformer_converter_side_base_voltage_in_kV(const unsigned int index,const double vbase)
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hlcc[index].set_converter_transformer_converter_side_base_voltage_in_kV(vbase);
}

void MT_HVDC::set_lcc_converter_transformer_R_in_ohm(const unsigned int index,const double rt)
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hlcc[index].set_converter_transformer_R_in_ohm(rt);
}

void MT_HVDC::set_lcc_converter_transformer_X_in_ohm(const unsigned int index,const double xt)
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hlcc[index].set_converter_transformer_X_in_ohm(xt);
}

void MT_HVDC::set_lcc_converter_transformer_Z_in_ohm(const unsigned int index,const complex<double> zt)
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hlcc[index].set_converter_transformer_Z_in_ohm(zt);
}

void MT_HVDC::set_lcc_converter_transformer_tap_in_pu(const unsigned int index,const double tap)
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hlcc[index].set_converter_transformer_tap_in_pu(tap);
}

void MT_HVDC::set_lcc_converter_transformer_max_tap_in_pu(const unsigned int index,const double tapmax)
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hlcc[index].set_converter_transformer_max_tap_in_pu(tapmax);
}

void MT_HVDC::set_lcc_converter_transformer_min_tap_in_pu(const unsigned int index,const double tapmin)
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hlcc[index].set_converter_transformer_min_tap_in_pu(tapmin);
}

void MT_HVDC::set_lcc_converter_transformer_number_of_taps(const unsigned int index,const unsigned int ntap)
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hlcc[index].set_converter_transformer_number_of_taps(ntap);
}

void MT_HVDC::set_lcc_converter_firing_angle_in_deg(const unsigned int index,const double fangle)
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hlcc[index].set_firing_angle_in_deg(fangle);
}

void MT_HVDC::set_lcc_converter_max_firing_angle_in_deg(const unsigned int index,const double maxangle)
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hlcc[index].set_max_firing_angle_in_deg(maxangle);
}

void MT_HVDC::set_lcc_converter_min_firing_angle_in_deg(const unsigned int index,const double minangle)
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hlcc[index].set_min_firing_angle_in_deg(minangle);
}

void MT_HVDC::set_lcc_converter_udc_in_kV(const unsigned int index,const double udc)
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hlcc[index].set_udc_in_kV(udc);
}

void MT_HVDC::set_lcc_converter_idc_in_kA(const unsigned int index,const double idc)
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hlcc[index].set_idc_in_kA(idc);
}

void MT_HVDC::set_lcc_converter_pdc_in_MW(const unsigned int index,const double pdc)
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hlcc[index].set_pdc_in_MW(pdc);
}

void MT_HVDC::set_lcc_converter_command_udc_in_kV(const unsigned int index,const double udc)
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hlcc[index].set_command_udc_in_kV(udc);
}

void MT_HVDC::set_lcc_converter_command_idc_in_kA(const unsigned int index,const double idc)
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hlcc[index].set_command_idc_in_kA(idc);
}

void MT_HVDC::set_lcc_converter_command_pdc_in_MW(const unsigned int index,const double pdc)
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hlcc[index].set_command_pdc_in_MW(pdc);
}

void MT_HVDC::set_lcc_converter_command_firing_angle(const unsigned int index,const double fangle)
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hlcc[index].set_command_firing_angle(fangle);
}

void MT_HVDC::set_lcc_converter_P_2_ac_bus(const unsigned int index,const double Pac)
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hlcc[index].set_converter_P_2_ac_bus(Pac);
}
void MT_HVDC::set_lcc_converter_Q_2_ac_bus(const unsigned int index,const double Qac)
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hlcc[index].set_converter_Q_2_ac_bus(Qac);
}

void MT_HVDC::set_lcc_converter_rated_capacity_in_MVA(const unsigned int index,const double Sbase)
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hlcc[index].set_converter_rated_capacity_in_MVA(Sbase);
}

void MT_HVDC::set_lcc_converter_base_voltage_in_kV(const unsigned int index,const double Vbase)
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hlcc[index].set_converter_base_voltage_in_kV(Vbase);
}

void MT_HVDC::set_lcc_converter_current_margin_in_pu(const unsigned int index,const double magrin)
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hlcc[index].set_converter_current_margin_in_pu(magrin);
}

void MT_HVDC::set_lcc_converter_control_mode(const unsigned int index,const HLCC_CONTROL_MODE mode)
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hlcc[index].set_control_mode(mode);
}

//get lcc
vector<HLCC*> MT_HVDC::get_all_lcc_converters()
{
    vector<HLCC*> all_lcc;
    unsigned int n = get_lcc_converter_count();
    all_lcc.clear();
    all_lcc.reserve(n);
    for(unsigned int i=0;i!=n;i++)
    {
        all_lcc.push_back(&(Hlcc[i]));
    }
    return all_lcc;
}

vector<HLCC*> MT_HVDC::get_lcc_converters_connect_to_ac_bus(unsigned int ac_bus)
{
    vector<HLCC*> lccs;
    unsigned int n = get_lcc_converter_count();
    lccs.clear();
    for(unsigned int i=0;i!=n;i++)
    {
        if(Hlcc[i].get_converter_ac_bus()==ac_bus)
            lccs.push_back(&(Hlcc[i]));
    }
    return lccs;;
}

HLCC* MT_HVDC::get_lcc_converter_with_name(string name)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    unsigned int n = get_lcc_converter_count();
    for(unsigned int i=0;i!=n;i++)
    {
        string lcc_name = Hlcc[i].get_converter_name();
        if(lcc_name==name)
        {
            return &(Hlcc[i]);
        }
    }
    osstream<<"NO LCC CONVERTER WITH NAME: "<<name;
    toolkit.show_information_with_leading_time_stamp(osstream);
    return NULL;
}

vector<HLCC*> MT_HVDC::get_angle_control_lcc_converters()
{
    vector<HLCC*> angle_lcc;
    unsigned int n_angle = get_number_of_angle_node();
    angle_lcc.clear();
    angle_lcc.reserve(n_angle);
    unsigned int n = get_lcc_converter_count();
    for(unsigned int i=0;i!=n;i++)
    {
        HLCC_CONTROL_MODE mode = Hlcc[i].get_control_mode();
        if(mode==LCC_CONSTANR_FIRING_ANGLE_MODE)
        {
            angle_lcc.push_back(&(Hlcc[i]));
        }
    }
    return angle_lcc;
}

vector<HLCC*> MT_HVDC::get_lcc_converters_connect_to_station(unsigned int station_number)
{
    vector<HLCC*> lccs;
    lccs.clear();
    unsigned int n = get_lcc_converter_count();
    for(unsigned int i=0;i!=n;i++)
    {
        unsigned int station = Hlcc[i].get_converter_station_number();
        if(station==station_number)
        {
            lccs.push_back(&(Hlcc[i]));
        }
    }
    return lccs;
}

HLCC* MT_HVDC::get_lcc_converter_with_index(unsigned int index)
{
    if(not lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return &(Hlcc[index]);
    else
        return NULL;
}

HLCC* MT_HVDC::get_lcc_converter_with_dc_bus_number(unsigned int dc_bus)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    vector<HLCC*> lccs = get_all_lcc_converters();
    unsigned int nlcc = get_lcc_converter_count();

    for(unsigned int i=0;i!=nlcc;i++)
    {
        HLCC* lcc_ptr = lccs[i];
        HLCC& lcc = *lcc_ptr;
        unsigned int dc_number = lcc.get_converter_dc_bus();
        if(dc_number == dc_bus)
            return lcc_ptr;
    }
    osstream << "no lcc converter connected to dc bus"<<dc_bus;
    toolkit.show_information_with_leading_time_stamp(osstream);
    return NULL;
}

unsigned int MT_HVDC::get_lcc_angle_control_number_of_hcm(unsigned int station_number)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    DC_STATION* station_ptr = get_dc_station_with_station_number(station_number);
    DC_STATION& station = *station_ptr;
    DC_STATION_STRUCTURE structure = station.get_station_structure();
    unsigned int n_angle = 0;
    if(structure==LCC_HCM)
    {
        vector<HLCC*> lccs = get_lcc_converters_connect_to_station(station_number);
        unsigned int n_lcc = lccs.size();
        if(n_lcc>2)
        {
            osstream<<"ONLY 2 LCC CONVERTERS ARE SUPPORTED IN STATION"<<station.get_station_name()<<" IN LCC HCM MODE ";
            toolkit.show_information_with_leading_time_stamp(osstream);
            return 0;
        }

        for(unsigned int i=0;i!=n_lcc;i++)
        {
            HLCC& lcc = *(lccs[i]);
            HLCC_CONTROL_MODE mode = lcc.get_control_mode();
            if(mode==LCC_CONSTANR_FIRING_ANGLE_MODE)
                n_angle++;
        }
        return n_angle;
    }
    else
    {
        osstream<<"STATION: "<<station.get_station_name()<<" is not in the HCM mode";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 0;
    }
 }


HLCC* MT_HVDC::get_lcc_converter_angle_fixed_of_HCM_station(unsigned int station_number)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    DC_STATION* station_ptr = get_dc_station_with_station_number(station_number);
    DC_STATION& station = *station_ptr;
    DC_STATION_STRUCTURE structure = station.get_station_structure();
    if(structure==LCC_HCM)
    {
        vector<HLCC*> lccs = get_lcc_converters_connect_to_station(station_number);
        unsigned int n_lcc = lccs.size();
        if(n_lcc>2)
        {
            osstream<<"ONLY 2 LCC CONVERTERS ARE SUPPORTED IN STATION"<<station.get_station_name()<<" IN LCC HCM MODE ";
            toolkit.show_information_with_leading_time_stamp(osstream);

        }
        for(unsigned int i=0;i!=n_lcc;i++)
        {
            HLCC& lcc = *(lccs[i]);
            HLCC_CONTROL_MODE mode = lcc.get_control_mode();
            if(mode==LCC_CONSTANR_FIRING_ANGLE_MODE)
            {
                return lccs[i];
            }
        }
    }
    return NULL;
}

HLCC* MT_HVDC::get_lcc_converter_angle_slack_of_HCM_station(unsigned int station_number)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    DC_STATION* station_ptr = get_dc_station_with_station_number(station_number);
    DC_STATION& station = *station_ptr;
    DC_STATION_STRUCTURE structure = station.get_station_structure();
    if(structure==LCC_HCM)
    {
        vector<HLCC*> lccs = get_lcc_converters_connect_to_station(station_number);
        unsigned int n_lcc = lccs.size();
        if(n_lcc>2)
        {
            osstream<<"ONLY 2 LCC CONVERTERS ARE SUPPORTED IN STATION"<<station.get_station_name()<<" IN LCC HCM MODE ";
            toolkit.show_information_with_leading_time_stamp(osstream);

        }
        for(unsigned int i=0;i!=n_lcc;i++)
        {
            HLCC& lcc = *(lccs[i]);
            HLCC_CONTROL_MODE mode = lcc.get_control_mode();
            if(mode==LCC_CONSTANT_VOLTAGE_MODE)
            {
                return lccs[i];
            }
        }
    }
    return NULL;
}

string MT_HVDC::get_lcc_converter_name(unsigned int index) const
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return "";
    return Hlcc[index].get_converter_name();
}

unsigned int MT_HVDC::get_lcc_converter_index_with_name(string name) const
{
    unsigned int n_lcc = Hlcc.size();
    for(unsigned int i=0;i!=n_lcc;i++)
    {
        if(Hlcc[i].get_converter_name()==name)
            return i;
    }
    return INDEX_NOT_EXIST;
}


bool MT_HVDC::get_lcc_converter_status(unsigned int index) const
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return false;
    return Hlcc[index].get_status();
}

unsigned int MT_HVDC::get_lcc_converter_ac_bus(unsigned int index) const
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0;
    return Hlcc[index].get_converter_ac_bus();
}

unsigned int MT_HVDC::get_lcc_converter_station_number(unsigned int index) const
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0;
    return Hlcc[index].get_converter_station_number();
}

unsigned int MT_HVDC::get_lcc_converter_dc_bus(unsigned int index) const
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0;
    return Hlcc[index].get_converter_dc_bus();
}

HLCC_OPERATION_MODE MT_HVDC::get_lcc_converter_operating_mode(unsigned int index)
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return INVALID_HLCC_OPERATING_MODE;
    return Hlcc[index].get_converter_operating_mode();
}

unsigned int MT_HVDC::get_lcc_converter_brideg_number(unsigned int index) const
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0;
    return Hlcc[index].get_brideg_number();
}

double MT_HVDC::get_lcc_converter_voltage_drop_per_bridge_in_kV(unsigned int index) const
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0.0;
    return Hlcc[index].get_voltage_drop_per_bridge_in_kV();
}

double MT_HVDC::get_lcc_converter_line_smoothing_L_in_H(unsigned int index) const
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0.0;
    return Hlcc[index].get_line_smoothing_L_in_H();
}

double MT_HVDC::get_lcc_converter_line_smoothing_R_in_ohm(unsigned int index) const
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0.0;
    return Hlcc[index].get_line_smoothing_R_in_ohm();
}

double MT_HVDC::get_lcc_converter_ac_bus_vlotage(unsigned int index) const
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0.0;
    return Hlcc[index].get_ac_bus_vlotage_in_kV();
}

complex<double> MT_HVDC::get_lcc_converter_complex_vac_in_kV(unsigned int index) const
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return complex<double>(0,0);
    return Hlcc[index].get_complex_vac_in_kV();
}

double MT_HVDC::get_lcc_converter_transformer_grid_side_base_voltage_in_kV(unsigned int index) const
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0.0;
    return Hlcc[index].get_converter_transformer_grid_side_base_voltage_in_kV();
}

double MT_HVDC::get_lcc_converter_transformer_converter_side_base_voltage_in_kV(unsigned int index) const
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0.0;
    return Hlcc[index].get_converter_transformer_converter_side_base_voltage_in_kV();
}

double MT_HVDC::get_lcc_converter_transformer_R_in_ohm(unsigned int index) const
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0.0;
    return Hlcc[index].get_converter_transformer_R_in_ohm();
}

double MT_HVDC::get_lcc_converter_transformer_X_in_ohm(unsigned int index) const
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0.0;
    return Hlcc[index].get_converter_transformer_X_in_ohm();
}

complex<double> MT_HVDC::get_lcc_converter_transformer_Z_in_ohm(unsigned int index) const
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return complex<double>(0,0);
    return Hlcc[index].get_converter_transformer_Z_in_ohm();
}

double MT_HVDC::get_lcc_converter_transformer_tap_in_pu(unsigned int index) const
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0.0;
    return Hlcc[index].get_converter_transformer_tap_in_pu();
}

double MT_HVDC::get_lcc_converter_transformer_max_tap_in_pu(unsigned int index) const
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0.0;
    return Hlcc[index].get_converter_transformer_max_tap_in_pu();
}

double MT_HVDC::get_lcc_converter_transformer_min_tap_in_pu(unsigned int index) const
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0.0;
    return Hlcc[index].get_converter_transformer_min_tap_in_pu();
}

unsigned int MT_HVDC::get_lcc_converter_transformer_number_of_taps(unsigned int index) const
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0;
    return Hlcc[index].get_converter_transformer_number_of_taps();
}

double MT_HVDC::get_lcc_converter_firing_angle_in_deg(unsigned int index) const
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0;
    return Hlcc[index].get_firing_angle_in_deg();
}

double MT_HVDC::get_lcc_converter_cos_firing_angle(unsigned int index) const
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0.0;
    return Hlcc[index].get_cos_firing_angle();
}

double MT_HVDC::get_lcc_converter_max_firing_angle_in_deg(unsigned int index) const
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0.0;
    return Hlcc[index].get_max_firing_angle_in_deg();
}

double MT_HVDC::get_lcc_converter_min_firing_angle_in_deg(unsigned int index) const
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0.0;
    return Hlcc[index].get_min_firing_angle_in_deg();
}

double MT_HVDC::get_lcc_converter_udc_in_kV(unsigned int index) const
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0.0;
    return Hlcc[index].get_udc_in_kV();
}

double MT_HVDC::get_lcc_converter_idc_in_kA(unsigned int index) const
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0.0;
    return Hlcc[index].get_idc_in_kA();
}

double MT_HVDC::get_lcc_converter_pdc_in_MW(unsigned int index) const
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0.0;
    return Hlcc[index].get_pdc_in_MW();
}
double MT_HVDC::get_lcc_converter_command_udc_in_kV(unsigned int index) const
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0.0;
    return Hlcc[index].get_command_udc_in_kV();
}

double MT_HVDC::get_lcc_converter_command_idc_in_kA(unsigned int index) const
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0.0;
    return Hlcc[index].get_command_idc_in_kA();
}

double MT_HVDC::get_lcc_converter_command_pdc_in_MW(unsigned int index) const
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0.0;
    return Hlcc[index].get_command_pdc_in_MW();
}

double MT_HVDC::get_lcc_converter_command_firing_angle(unsigned int index) const
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0.0;
    return Hlcc[index].get_command_firing_angle();
}

double MT_HVDC::get_lcc_converter_P_2_ac_bus(unsigned int index) const
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0.0;
    return Hlcc[index].get_converter_P_2_ac_bus();
}

double MT_HVDC::get_lcc_converter_Q_2_ac_bus(unsigned int index) const
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0.0;
    return Hlcc[index].get_converter_Q_2_ac_bus();
}

HLCC_CONTROL_MODE MT_HVDC::get_lcc_converter_control_mode(unsigned int index) const
{
    if(lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return LCC_INVALID_CONTROL_MODE;
    return Hlcc[index].get_control_mode();
}

//set vsc
void MT_HVDC::set_vsc_converter_ac_bus(const unsigned int index,const unsigned int acbus)
{
    if(vsc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hvsc[index].set_converter_ac_bus(acbus);
}

void MT_HVDC::set_vsc_converter_station_number(const unsigned int index,const unsigned int station)
{
    if(vsc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hvsc[index].set_converter_station_number(station);
}

void MT_HVDC::set_vsc_converter_dc_bus(const unsigned int index,const unsigned int dcbus)
{
    if(vsc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hvsc[index].set_converter_dc_bus(dcbus);
}

void MT_HVDC::set_vsc_converter_name(const unsigned int index,const string name)
{
    if(vsc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hvsc[index].set_converter_name(name);
}

void MT_HVDC::set_vsc_converter_status(const unsigned int index, const bool status)
{
    if(vsc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hvsc[index].set_converter_status(status);
}

void MT_HVDC::set_vsc_converter_priority(const unsigned int index,unsigned int priority)
{
    if(vsc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hvsc[index].set_priority(priority);
}

void MT_HVDC::set_vsc_converter_d_axis_control_mode(const unsigned int index,const HVSC_D_CONTROL_MODE dmode)
{
    if(vsc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hvsc[index].set_converter_d_axis_control_mode(dmode);
}

void MT_HVDC::set_vsc_converter_q_axis_control_mode(const unsigned int index,const HVSC_Q_CONTROL_MODE qmode)
{
    if(vsc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hvsc[index].set_converter_q_axis_control_mode(qmode);
}

void MT_HVDC::set_vsc_converter_pac_command(const unsigned int index,const double pac)
{
    if(vsc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hvsc[index].set_converter_Pac_command(pac);
}

void MT_HVDC::set_vsc_converter_qac_command(const unsigned int index,const double qac)
{
    if(vsc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hvsc[index].set_converter_Qac_command(qac);
}

void MT_HVDC::set_vsc_converter_udc_command(const unsigned int index,const double udc)
{
    if(vsc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hvsc[index].set_converter_Udc_command(udc);
}

void MT_HVDC::set_vsc_converter_us_command(const unsigned int index,const double us)
{
    if(vsc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hvsc[index].set_converter_Us_command(us);
}

void MT_HVDC::set_vsc_converter_loss_coefficient_A(const unsigned int index,const double A)
{
    if(vsc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hvsc[index].set_converter_loss_coefficient_A_in_kW(A);
}

void MT_HVDC::set_vsc_converter_loss_coefficient_B(const unsigned int index,const double B)
{
    if(vsc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hvsc[index].set_converter_loss_coefficient_B_in_kW_per_amp(B);
}

void MT_HVDC::set_vsc_converter_loss_coefficient_C(const unsigned int index,const double C)
{
    if(vsc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hvsc[index].set_converter_loss_coefficient_C_in_kW_per_amp_squard(C);
}

void MT_HVDC::set_vsc_converter_min_loss_in_kW(const unsigned int index,const double min_loss)
{
    if(vsc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hvsc[index].set_converter_min_loss_in_kW(min_loss);
}

void MT_HVDC::set_vsc_converter_Z_in_ohm(const unsigned int index,const complex<double> Z)
{
    if(vsc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hvsc[index].set_converter_commutating_impedance(Z);
}

void  MT_HVDC::set_vsc_converter_P_to_ac_bus_in_MW(const unsigned int index,const double P2ac)
{
    if(vsc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hvsc[index].set_converter_P_to_AC_bus_MW(P2ac);
}

void  MT_HVDC::set_vsc_converter_Q_to_ac_bus_in_Mvar(const unsigned int index,const double Q2ac)
{
    if(vsc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hvsc[index].set_converter_Q_to_AC_bus_Mvar(Q2ac);
}

void MT_HVDC::set_vsc_converter_rated_capacity_in_MVA(const unsigned int index,const double Sbase)
{
    if(vsc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hvsc[index].set_converter_rated_capacity_in_MVA(Sbase);
}

void MT_HVDC::set_vsc_converter_base_voltage_in_kV(const unsigned int index,const double vbase)
{
    if(vsc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Hvsc[index].set_converter_base_voltage_in_kV(vbase);
}

//get vsc

string MT_HVDC::get_vsc_converter_name(unsigned int index) const
{
    if(vsc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return "";
    return Hvsc[index].get_converter_name();
}


vector<HVSC*> MT_HVDC::get_all_vsc_converters()
{
    vector<HVSC*> all_vsc;
    unsigned int n = get_vsc_converter_count();
    all_vsc.clear();
    all_vsc.reserve(n);
    for(unsigned int i=0;i!=n;i++)
        all_vsc.push_back(&(Hvsc[i]));
    return all_vsc;
}

vector<HVSC*> MT_HVDC::get_vsc_converters_connect_to_ac_bus(unsigned int ac_bus)
{
    vector<HVSC*> vscs;
    unsigned int n = get_vsc_converter_count();
    vscs.clear();
    for(unsigned int i=0;i!=n;i++)
    {
        if(Hvsc[i].get_converter_ac_bus()==ac_bus)
            vscs.push_back(&(Hvsc[i]));
    }
    return vscs;
}

HVSC* MT_HVDC::get_vsc_converter_with_name(string name)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    unsigned int n = get_vsc_converter_count();
    for(unsigned int i=0;i!=n;i++)
    {
        string vsc_name = Hvsc[i].get_converter_name();
        if(vsc_name==name)
            return &(Hvsc[i]);
    }
    osstream<<"NO VSC CONVERTER WITH NAME: "<<name;
    toolkit.show_information_with_leading_time_stamp(osstream);
    return NULL;
}

 HVSC* MT_HVDC::get_vsc_converter_with_index(unsigned int index)
 {
     if(not vsc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return &(Hvsc[index]);
     else
        return NULL;
 }

vector<HVSC*> MT_HVDC::get_vsc_converters_connect_to_station(unsigned int station_number)
{
    vector<HVSC*> vscs;
    vscs.clear();
    unsigned int n = get_vsc_converter_count();
    for(unsigned int i=0;i!=n;i++)
    {
        unsigned int station = Hvsc[i].get_converter_station_number();
        if(station==station_number)
            vscs.push_back(&(Hvsc[i]));
    }
    return vscs;
}

HVSC* MT_HVDC::get_current_vsc_converter_with_top_priority(unsigned int station_number)
{
    vector<HVSC*> vscs;
    vscs.clear();

    vscs = get_vsc_converters_connect_to_station(station_number);
    unsigned int n = vscs.size();

    HVSC* voltage_converter = vscs[0];
    if(n==1)
        return voltage_converter;
    else
    {
        voltage_converter = vscs[0];
        for(unsigned int i=0;i!=n-1;i++)
        {
            HVSC& vsc1 = *(vscs[i]);
            HVSC& vsc2 = *(vscs[i+1]);
            unsigned int priority1 = vsc1.get_priority();
            unsigned int priority2 = vsc2.get_priority();
            if(priority1>priority2)
                voltage_converter = &vsc2;
        }
        return voltage_converter;
    }
}

vector<HVSC*> MT_HVDC::get_vsc_connected_to_ac_bus(unsigned int bus)
{
    vector<HVSC*> vscs;
    unsigned int n_vsc = Hvsc.size();
    for(unsigned int i=0;i!=n_vsc;i++)
    {
        unsigned int acbus = Hvsc[i].get_converter_ac_bus();
        if(acbus==bus)
        {
            vscs.push_back(&Hvsc[i]);
        }
    }
    return vscs;
}

unsigned int MT_HVDC::get_vsc_converter_ac_bus(unsigned int index) const
{
    if(vsc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0;
    return Hvsc[index].get_converter_ac_bus();
}

//set dcbus
void MT_HVDC::set_dc_bus_status(const unsigned int index,const bool status)
{
    if(dc_bus_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Dcbus[index].set_status(status);
}
void MT_HVDC::set_dc_bus_name(const unsigned int index,const string name)
{
    if(dc_bus_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Dcbus[index].set_name(name);
}
void MT_HVDC::set_dc_bus_nominal_voltage(const unsigned int index,const double vn)
{
    if(dc_bus_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Dcbus[index].set_nominal_voltage(vn);
}
void MT_HVDC::set_dc_bus_number(const unsigned int index,const unsigned int number)
{
    if(dc_bus_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Dcbus[index].set_dc_bus_number(number);
}

void MT_HVDC::set_dc_bus_voltage_in_kV(const unsigned int index,const double vdc)
{
    if(dc_bus_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Dcbus[index].set_dc_bus_voltage_in_kV(vdc);
}

void MT_HVDC::set_dc_bus_current_in_kA(const unsigned int index, double idc)
{
    if(dc_bus_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Dcbus[index].set_dc_bus_current_in_kA(idc);
}
void MT_HVDC::set_dc_bus_power_in_MW(const unsigned int index,const double pdc)
{
    if(dc_bus_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Dcbus[index].set_dc_bus_power_in_MW(pdc);
}
void MT_HVDC::set_dc_bus_area(const unsigned int index,const unsigned int area)
{
    if(dc_bus_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
   Dcbus[index].set_dc_bus_area(area);
}

void MT_HVDC::set_dc_bus_zone(const unsigned int index,const unsigned int zone)
{
    if(dc_bus_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
   Dcbus[index].set_dc_bus_zone(zone);
}

//get dcbus
vector<DC_BUS*> MT_HVDC::get_all_dc_buses()
{
    vector<DC_BUS*> all_dc_buses;
    unsigned int n = get_dc_bus_count();
    all_dc_buses.reserve(n);
    for(unsigned int i=0;i!=n;i++)
        all_dc_buses.push_back(&(Dcbus[i]));
    return all_dc_buses;

}

DC_BUS* MT_HVDC::get_dc_bus_with_index(unsigned int index)
{
    if(not dc_bus_index_is_out_of_range_in_function(index,__FUNCTION__))
        return &(Dcbus[index]);
    else
        return NULL;
}

DC_BUS* MT_HVDC::get_dc_bus_with_name(string name)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    unsigned int n_dcbus = Dcbus.size();
    for(unsigned int i=0;i!=n_dcbus;i++)
    {
        if(Dcbus[i].get_name()==name)
            return &(Dcbus[i]);
    }
    osstream<<"No dc bus with bus name: "<<name;
    toolkit.show_information_with_leading_time_stamp(osstream);
    return NULL;
}

DC_BUS* MT_HVDC::get_dc_bus_with_bus_number(unsigned int number)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    unsigned int n = get_dc_bus_count();
    vector<DC_BUS*> all_dc_buses = get_all_dc_buses();
    for(unsigned int i=0;i!=n;i++)
    {
        DC_BUS& dcbus = *(all_dc_buses[i]);
        unsigned int bus_number = dcbus.get_dc_bus_number();
        if (bus_number == number)
            return &dcbus;
    }
    osstream<<"No dc bus with bus number: "<<number;
    toolkit.show_information_with_leading_time_stamp(osstream);
    return NULL;
}

DC_BUS* MT_HVDC::get_dc_bus_with_terminal_number(unsigned int terminal_number)
{
    DC_TERMINAL* terminal_ptr = get_dc_terminal_with_terminal_number(terminal_number);
    DC_TERMINAL& terminal = *terminal_ptr;
    unsigned int dc_bus_number = terminal.get_terminal_dc_bus_number();
    DC_BUS* dcbus = get_dc_bus_with_bus_number(dc_bus_number);
    return dcbus;
}

unsigned int MT_HVDC::get_number_of_P_node()
{
    unsigned nbus = get_dc_bus_count();
    vector<DC_BUS*> dc_buses = get_all_dc_buses();
    unsigned int n_p = 0;
    for(unsigned int i=0;i!=nbus;i++)
    {
        DC_BUS& dc_bus = *(dc_buses[i]);
        unsigned int bus_number = dc_bus.get_dc_bus_number();
        HLCC* lcc_ptr = get_lcc_converter_with_dc_bus_number(bus_number);
        HLCC& lcc = *lcc_ptr;
        HLCC_CONTROL_MODE mode = lcc.get_control_mode();
        if(mode == LCC_CONSTANT_POWER_MODE)
        {
            n_p++;
        }
    }
    return n_p;
}

unsigned int MT_HVDC::get_number_of_I_node()
{
    unsigned nbus = get_dc_bus_count();
    vector<DC_BUS*> dc_buses = get_all_dc_buses();
    unsigned int n_i = 0;
    for(unsigned int i=0;i!=nbus;i++)
    {
        DC_BUS& dc_bus = *(dc_buses[i]);
        unsigned int bus_number = dc_bus.get_dc_bus_number();
        HLCC* lcc_ptr = get_lcc_converter_with_dc_bus_number(bus_number);
        HLCC& lcc = *lcc_ptr;
        HLCC_CONTROL_MODE mode = lcc.get_control_mode();
        if(mode == LCC_CONSTANT_CURRENT_MODE)
        {
            n_i++;
        }
    }
    return n_i;
}

unsigned int MT_HVDC::get_number_of_P_and_I_node()
{
    unsigned nbus = get_dc_bus_count();
    vector<DC_BUS*> dc_buses = get_all_dc_buses();
    unsigned int n_pi = 0;
    for(unsigned int i=0;i!=nbus;i++)
    {
        DC_BUS& dc_bus = *(dc_buses[i]);
        unsigned int bus_number = dc_bus.get_dc_bus_number();
        HLCC* lcc_ptr = get_lcc_converter_with_dc_bus_number(bus_number);
        HLCC& lcc = *lcc_ptr;
        HLCC_CONTROL_MODE mode = lcc.get_control_mode();
        if((mode == LCC_CONSTANT_POWER_MODE) or (mode == LCC_CONSTANT_CURRENT_MODE))
        {
            n_pi++;
        }
    }
    return n_pi;
}

unsigned int MT_HVDC::get_number_of_angle_node()
{
    unsigned nbus = get_dc_bus_count();
    vector<DC_BUS*> dc_buses = get_all_dc_buses();
    unsigned int n_angle = 0;
    for(unsigned int i=0;i!=nbus;i++)
    {
        DC_BUS& dc_bus = *(dc_buses[i]);
        unsigned int bus_number = dc_bus.get_dc_bus_number();
        HLCC* lcc_ptr = get_lcc_converter_with_dc_bus_number(bus_number);
        HLCC& lcc = *lcc_ptr;
        HLCC_CONTROL_MODE mode = lcc.get_control_mode();
        if(mode == LCC_CONSTANR_FIRING_ANGLE_MODE)
        {
            n_angle++;
        }
    }
    return n_angle;

}

/*
unsigned int MT_HVDC::get_number_of_vsc_P_node()
{
    unsigned int nbus = get_dc_bus_count();
    vector<DC_BUS*> dc_buses = get_all_dc_buses();
    unsigned int n_vsc_p = 0;
    for(unsigned int i=0;i!=nbus;i++)
    {
        DC_BUS& dc_bus = *(dc_buses[i]);
        DC_BUS_TYPE type = dc_bus.get_dc_bus_type();
        if(type == DC_VSC_P_NODE)
        {
            n_vsc_p++;
        }
    }
    return n_vsc_p;
}*/

bool MT_HVDC::get_dc_bus_status(unsigned int index) const
{
    if(dc_bus_index_is_out_of_range_in_function(index,__FUNCTION__))
        return false;
    return Dcbus[index].get_status();
}

string MT_HVDC::get_dc_bus_name(unsigned int index) const
{
    if(dc_bus_index_is_out_of_range_in_function(index,__FUNCTION__))
        return "";
    return Dcbus[index].get_name();
}

unsigned int MT_HVDC::get_dc_bus_index_with_name(string name) const
{
    unsigned int n_dcbus = Dcbus.size();
    for(unsigned int i=0;i!=n_dcbus;i++)
    {
        if(Dcbus[i].get_name()==name)
            return i;
    }
    return INDEX_NOT_EXIST;
}

unsigned int MT_HVDC::get_dc_bus_number(unsigned int index) const
{
    if(dc_bus_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0;
    return Dcbus[index].get_dc_bus_number();
}

double MT_HVDC::get_dc_bus_nominal_voltage(unsigned int index) const
{
    if(dc_bus_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0.0;
    return Dcbus[index].get_nominal_voltage();
}

double MT_HVDC::get_dc_bus_voltage_in_kV(unsigned int index) const
{
    if(dc_bus_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0.0;
    return Dcbus[index].get_dc_bus_voltage_in_kV();
}

double MT_HVDC::get_dc_bus_current_in_kA(unsigned int index) const
{
    if(dc_bus_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0.0;
    return Dcbus[index].get_dc_bus_current_in_kA();
}

double MT_HVDC::get_dc_bus_power_in_MW(unsigned int index) const
{
    if(dc_bus_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0.0;
    return Dcbus[index].get_dc_bus_power_in_MW();
}

unsigned int MT_HVDC::get_dc_bus_area(unsigned int index) const
{
    if(dc_bus_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0;
    return Dcbus[index].get_dc_bus_area();
}

unsigned int MT_HVDC::get_dc_bus_zone(unsigned int index) const
{
    if(dc_bus_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0;
    return Dcbus[index].get_dc_bus_zone();
}

//set dcline
void MT_HVDC::set_dc_line_sending_side_bus(const unsigned int index,const unsigned int bus)
{
    if(dc_line_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Dcline[index].set_sending_side_bus(bus);
}

void MT_HVDC::set_dc_line_receiving_side_bus(const unsigned int index,const unsigned int bus)
{
    if(dc_line_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Dcline[index].set_receiving_side_bus(bus);
}

void MT_HVDC::set_dc_line_name(const unsigned int index,const string name)
{
    if(dc_line_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Dcline[index].set_dc_line_name(name);
}

void MT_HVDC::set_dc_line_status(const unsigned int index,const bool status)
{
    if(dc_line_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Dcline[index].set_status(status);
}

void MT_HVDC::set_dc_line_R_in_ohm(const unsigned int index,const double r)
{
    if(dc_line_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Dcline[index].set_line_R_in_ohm(r);
}

void MT_HVDC::set_dc_line_L_in_H(const unsigned int index,const double l)
{
    if(dc_line_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Dcline[index].set_line_L_in_H(l);
}

void MT_HVDC::set_dc_line_C_in_uF(const unsigned int index,const double c)
{
    if(dc_line_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Dcline[index].set_line_C_in_uF(c);
}

void MT_HVDC::set_dc_line_current(const unsigned int index,const double idc)
{
    if(dc_line_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Dcline[index].set_dc_line_current(idc);
}
void MT_HVDC::set_dc_line_sengding_side_p(const unsigned int index,const double psend)
{
    if(dc_line_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Dcline[index].set_sending_side_power(psend);
}

void MT_HVDC::set_dc_line_receiving_side_p(const unsigned int index,const double prec)
{
    if(dc_line_index_is_out_of_range_in_function(index,__FUNCTION__))
        return;
    Dcline[index].set_receiving_side_power(prec);
}

//get dcline
vector<DC_LINE*> MT_HVDC::get_all_dc_lines()
{
    vector<DC_LINE*> all_dc_lines;
    unsigned int n = get_dc_line_count();
    all_dc_lines.reserve(n);
    for(unsigned int i=0;i!=n;i++)
    {
        all_dc_lines.push_back(&(Dcline[i]));
    }
    return all_dc_lines;
}

DC_LINE* MT_HVDC::get_dc_line_with_line_name(string name)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    unsigned int n_dcline = get_dc_line_count();
    for(unsigned int i=0;i!=n_dcline;i++)
    {
        if(Dcline[i].get_dc_line_name()==name)
        {
            return &(Dcline[i]);
        }
    }
    osstream<<"No dc line with name: "<<name;
    toolkit.show_information_with_leading_time_stamp(osstream);
    return NULL;
}

DC_LINE* MT_HVDC::get_dc_line_with_index(unsigned int index)
{
    if(not dc_line_index_is_out_of_range_in_function(index,__FUNCTION__))
        return &(Dcline[index]);
    else
        return NULL;
}

DC_LINE* MT_HVDC::get_dc_line_with_ibus_and_jbus(unsigned int ibus,unsigned int jbus)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    unsigned int n = get_dc_line_count();
    vector<DC_LINE*> lines = get_all_dc_lines();
    for(unsigned int i=0;i!=n;i++)
    {
        DC_LINE* line_ptr = lines[i];
        DC_LINE& line = *line_ptr;
        unsigned int sending_bus = line.get_sending_side_bus_number();
        unsigned int receiving_bus = line.get_receiving_side_bus_number();
        if( (ibus == sending_bus) && (jbus == receiving_bus) )
            return line_ptr;
    }
    osstream<<"No dc line with ibus: "<<ibus<<"  and jbus: "<<jbus;
    toolkit.show_information_with_leading_time_stamp(osstream);
    return NULL;

}

vector<DC_LINE*> MT_HVDC::get_dc_lines_with_ibus(unsigned int ibus)
{
    unsigned int n = get_dc_line_count();
    vector<DC_LINE*> lines = get_all_dc_lines();
    vector<DC_LINE*> lines_with_ibus;
    lines_with_ibus.clear();
    for(unsigned int i=0;i!=n;i++)
    {
        DC_LINE* line_ptr = lines[i];
        DC_LINE& line = *line_ptr;
        unsigned int sending_bus = line.get_sending_side_bus_number();
        if(ibus == sending_bus)
            lines_with_ibus.push_back(line_ptr);

    }
    return lines_with_ibus;
}

vector<DC_LINE*> MT_HVDC::get_dc_lines_with_jbus(unsigned int jbus)
{
    unsigned int n = get_dc_line_count();
    vector<DC_LINE*> lines = get_all_dc_lines();
    vector<DC_LINE*> lines_with_jbus;
    lines_with_jbus.clear();
    for(unsigned int i=0;i!=n;i++)
    {
        DC_LINE* line_ptr = lines[i];
        DC_LINE& line = *line_ptr;
        unsigned int receiving_bus = line.get_receiving_side_bus_number();
        if(jbus == receiving_bus)
            lines_with_jbus.push_back(line_ptr);

    }
    return lines_with_jbus;
}

unsigned int MT_HVDC::get_dc_line_sending_side_bus_number(unsigned int index) const
{
    if(dc_line_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0;
    return Dcline[index].get_sending_side_bus_number();
}

unsigned int MT_HVDC::get_dc_line_receiving_side_bus_number(unsigned int index) const
{
    if(dc_line_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0;
    return Dcline[index].get_receiving_side_bus_number();
}

string MT_HVDC::get_dc_line_name(unsigned int index) const
{
    if(dc_line_index_is_out_of_range_in_function(index,__FUNCTION__))
        return "";
    return Dcline[index].get_dc_line_name();
}

unsigned int MT_HVDC::get_dc_line_index_with_name(string name)
{
    unsigned int n_dcline = Dcline.size();
    for(unsigned int i=0;i!=n_dcline;i++)
    {
        if(Dcline[i].get_dc_line_name()==name)
            return i;
    }
    return INDEX_NOT_EXIST;
}

bool MT_HVDC::get_dc_line_status(unsigned int index) const
{
    if(dc_line_index_is_out_of_range_in_function(index,__FUNCTION__))
        return false;
    return Dcline[index].get_status();
}

double MT_HVDC::get_dc_line_R_in_ohm(unsigned int index) const
{
    if(dc_line_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0.0;
    return Dcline[index].get_line_R_in_ohm();
}

double MT_HVDC::get_dc_line_L_in_H(unsigned int index) const
{
    if(dc_line_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0.0;
    return Dcline[index].get_line_L_in_H();
}

double MT_HVDC::get_dc_line_current(unsigned int index) const
{
    if(dc_line_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0.0;
    return Dcline[index].get_dc_line_current();
}

double MT_HVDC::get_dc_line_sending_side_p(unsigned int index) const
{
    if(dc_line_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0.0;
    return Dcline[index].get_sending_side_power();
}

double MT_HVDC::get_dc_line_receiving_side_p(unsigned int index) const
{
    if(dc_line_index_is_out_of_range_in_function(index,__FUNCTION__))
        return 0.0;
    return Dcline[index].get_receiving_side_power();
}

bool MT_HVDC::check_control_mode_change_flag()
{
    bool is_control_mode_change = false;
    vector<HLCC*> lccs = get_all_lcc_converters();
    unsigned int n_lcc = lccs.size();
    for(unsigned int i=0;i!=n_lcc;i++)
    {
        if(lccs[i]->get_control_mode_changed_flag()==true)
        {
            HLCC& lcc = *(lccs[i]);
            is_control_mode_change = true;
            unsigned int station_number = lcc.get_converter_station_number();
            DC_STATION* station_ptr = get_dc_station_with_station_number(station_number);
            DC_STATION& station = *station_ptr;
            station.set_station_mode(DC_ANGLE_STATION);

            DC_TERMINAL* terminal_ptr = get_dc_terminal_with_terminal_number(station.get_station_number());
            DC_TERMINAL& terminal = *terminal_ptr;
            terminal.set_terminal_mode(DC_ANGLE_TERMINAL);

            ostringstream osstream;
            osstream<<"The CONTROL MODE OF STATION: "<<station_number<<"  IS CHANGED TO :" <<station.get_station_mode();
            show_information_with_leading_time_stamp_with_default_toolkit(osstream);

            osstream<<"The CONTROL MODE OF TERMINAL: "<<terminal.get_terminal_number()<<"  IS CHANGED TO :" <<terminal.get_terminal_mode();
            show_information_with_leading_time_stamp_with_default_toolkit(osstream);
        }
    }

    return is_control_mode_change;
}

STEPS_SPARSE_MATRIX MT_HVDC::get_dc_network_matrix_without_dynamic()
{
    return dc_network_matrix;
}

INPHNO MT_HVDC::get_dc_network_internal_physical_number_inphno()
{
    return inphno;
}

void MT_HVDC::solve_steady_state()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    while(true)
    {
        initialize_vsc_converters();
        initialize_lcc_converters();
        initialize_dc_power_flow();

        build_dc_network_matrix();
        bool is_dc_network_converged = solve_dc_network();

        if(is_dc_network_converged)
        {
            solve_converters();
            bool is_control_mode_change = check_control_mode_change_flag();
            if(not is_control_mode_change)
            {
                return;
            }
        }
        else
        {
            osstream<<"The power flow calculation of MTDC :"<<get_name()<<" is solved unsuccessfully";
            toolkit.show_information_with_leading_time_stamp(osstream);
            return;
        }
    }
}

bool MT_HVDC::solve_dc_network()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    vector<double> u_bus;
    unsigned int n_bus = get_dc_bus_count();
    unsigned int n_ubus = get_number_of_dc_u_bus();
    unsigned int n_umis = n_bus-n_ubus;
    u_bus.reserve(n_umis);
    for(unsigned int i=0;i!=n_umis;i++)
    {
        unsigned int bus_number = inphno.get_physical_bus_number_of_internal_bus_number(i);
        DC_BUS* bus_ptr = get_dc_bus_with_bus_number(bus_number);
        DC_BUS& bus = *bus_ptr;
        double voltage = bus.get_dc_bus_voltage_in_kV();
        u_bus[i] = voltage;
    }

    double max_iter= get_dc_system_max_iteration_in_powerflow();
    double allowed_max_pmis = get_dc_system_allow_power_mismatch_in_powerflow();
    iteration_count = 0;

    while(true)
    {
        //osstream<<"DC powerflow iteration count: "<<iteration_count;
        //toolkit.show_information_with_leading_time_stamp(osstream);

        calculate_raw_power_mismatch();
        double max_pmis = get_max_active_power_mismatch_in_MW();

        //osstream<<"The max power mismatch: "<<max_pmis;
        //toolkit.show_information_with_leading_time_stamp(osstream);

        if(max_pmis<allowed_max_pmis)
        {
            //osstream<<"DC power flow converged in: "<<iteration_count<<" itreation";
            //toolkit.show_information_with_leading_time_stamp(osstream);
            //show_bus_voltage();
            update_bus_current();
            update_bus_power();
            return true;
        }

        build_jacobian();
        u_mismactch.clear();
        u_mismactch = p_mismatch/jacobian;
        if(not jacobian.is_lu_factorization_failed())
        {
            for(unsigned int i=0;i!=n_umis;i++)
            {
                unsigned int bus_number = inphno.get_physical_bus_number_of_internal_bus_number(i);
                DC_BUS* dcbus_ptr = get_dc_bus_with_bus_number(bus_number);
                DC_BUS& dcbus = *dcbus_ptr;
                u_bus[i]-=u_mismactch[i];
                dcbus.set_dc_bus_voltage_in_kV(u_bus[i]);
            }
            iteration_count++;
        }
        else
        {
            osstream<<"Fatal Error. "<<"MT HVDC "<<get_name()<<" inner powerflow solution failed since LU factorization of N-R Jacobian matrix is failed."<<endl;
            toolkit.show_information_with_leading_time_stamp(osstream);
            break;

        }

        if(iteration_count>max_iter)
        {
            osstream<<"DC power flow failed to converged in: "<<max_iter<<" iteration and the power flow result is not reliable";
            toolkit.show_information_with_leading_time_stamp(osstream);
            //show_bus_voltage();
            return false;
        }
    }
    return false;
}

void MT_HVDC::solve_converters()
{
    vector<DC_TERMINAL*> terminals = get_all_dc_terminals();
    unsigned int n_terminal = terminals.size();
    for(unsigned int i=0;i!=n_terminal;i++)
    {
        DC_TERMINAL& terminal = *(terminals[i]);
        if(terminal.get_status()==false)
            continue;

        unsigned int terminal_number = terminal.get_terminal_number();
        DC_TERMINAL_MODE tmode = terminal.get_terminal_mode();
        if(tmode==DC_P_TERMIANL)
        {
            solve_converters_of_P_terminal(terminal_number);
        }

        if(tmode==DC_U_TERMINAL)
        {
            solve_converters_of_U_terminal(terminal_number);
        }

        if(tmode==DC_I_TERMINAL)
        {
            solve_converters_of_I_terminal(terminal_number);
        }

        if(tmode==DC_ANGLE_TERMINAL)
        {
            solve_converters_of_ANGLE_terminal(terminal_number);
        }

        else
        {
            ;
        }

    }
}

void MT_HVDC::solve_converters_of_P_terminal(unsigned int terminal_number)
{
    DC_TERMINAL* terminal_ptr = get_dc_terminal_with_terminal_number(terminal_number);
    DC_TERMINAL& terminal = *terminal_ptr;
    unsigned int dcbus_number = terminal.get_terminal_dc_bus_number();
    DC_BUS* dcbus_ptr = get_dc_bus_with_bus_number(dcbus_number);
    DC_BUS& dcbus = *dcbus_ptr;
    double Udc = dcbus.get_dc_bus_voltage_in_kV();
    double Idc = dcbus.get_dc_bus_current_in_kA();

    double P_terminal = dcbus.get_dc_bus_power_in_MW();


    vector<DC_STATION*> stations = get_dc_stations_connect_to_terminal(terminal_number);
    unsigned int n_station = stations.size();
    for(unsigned int j=0;j!=n_station;j++)
    {
        DC_STATION& station = *(stations[j]);
        if(station.get_status()==false)
            continue;

        unsigned int station_number = station.get_station_number();
        double P_station = get_active_power_of_power_station(station_number);
        double U_station = Udc*P_station/P_terminal;

        DC_STATION_STRUCTURE structure = station.get_station_structure();
        if(structure==LCC_CASCADE)
        {
            //vector<HLCC*> lccs = station.get_lcc_converters_in_dc_station_with_lcc_cascade();
            vector<HLCC*> lccs = get_lcc_converters_connect_to_station(station_number);
            unsigned int n_lcc = lccs.size();
            for(unsigned int i=0;i!=n_lcc;i++)
            {
                HLCC& lcc = *(lccs[i]);
                if(lcc.get_status()==true)
                {
                    double P_lcc = lcc.get_command_pdc_in_MW();
                    double U_lcc = U_station*P_lcc/P_station;

                    //osstream<<"Voltage of LCC is: "<< U_lcc;
                    //toolkit.show_information_with_leading_time_stamp(osstream);
                    lcc.set_idc_in_kA(Idc);
                    lcc.set_udc_in_kV(U_lcc);
                    lcc.solve_converter_to_AC(U_lcc,Idc);
                }
            }
        }

        if(structure==VSC_PARALLEL)
        {
            vector<HVSC*> vscs = get_vsc_converters_connect_to_station(station_number);
            unsigned int n_vsc = vscs.size();
            for(unsigned int i=0;i!=n_vsc;i++)
            {
                HVSC& vsc = *(vscs[i]);
                if(vsc.get_status())
                {
                    double P_vsc = vsc.get_converter_P_to_DC_bus_MW();
                    double I_vsc = Idc*P_vsc/P_station;
                    vsc.set_udc_in_kV(U_station);
                    vsc.set_idc_in_kA(I_vsc);
                }
            }
        }
    }
}

void MT_HVDC::solve_converters_of_I_terminal(unsigned int terminal_number)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    DC_TERMINAL* terminal_ptr = get_dc_terminal_with_terminal_number(terminal_number);
    DC_TERMINAL& terminal = *terminal_ptr;
    unsigned int dcbus_number = terminal.get_terminal_dc_bus_number();
    DC_BUS* dcbus_ptr = get_dc_bus_with_bus_number(dcbus_number);
    DC_BUS& dcbus = *dcbus_ptr;
    double Udc = dcbus.get_dc_bus_voltage_in_kV();
    double Idc = dcbus.get_dc_bus_current_in_kA();

    double U_terminal = Udc;

    vector<DC_STATION*> stations = get_dc_stations_connect_to_terminal(terminal_number);
    unsigned int n_station = stations.size();

    double U_station = U_terminal/n_station;
    for(unsigned int j=0;j!=n_station;j++)
    {
        DC_STATION& station = *(stations[j]);
        if(station.get_status()==false)
            continue;
        unsigned int station_number = station.get_station_number();
        DC_STATION_STRUCTURE structure = station.get_station_structure();
        if(structure==LCC_CASCADE)
        {
            //vector<HLCC*> lccs = station.get_lcc_converters_in_dc_station_with_lcc_cascade();
            vector<HLCC*> lccs = get_lcc_converters_connect_to_station(station_number);
            unsigned int n_lcc = lccs.size();

            for(unsigned int i=0;i!=n_lcc;i++)
            {
                double U_lcc = U_station/n_lcc;
                double I_lcc = Idc;
                HLCC& lcc = *(lccs[i]);
                lcc.set_udc_in_kV(U_lcc);
                lcc.set_idc_in_kA(I_lcc);
                lcc.solve_converter_to_AC(U_lcc,I_lcc);

                //lcc.solve_lcc_with_tap_and_angle(U_lcc,I_lcc);

                //osstream<<"LCC:"<<lcc.get_converter_name()<<"solve with udc:"<<U_lcc<<"and idc:"<<I_lcc;
                //toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }

        if(structure==VSC_PARALLEL)
        {
            osstream<<"CURRENT CONTROL IS NOT SUPPORTED FOR VSC CONVERTER";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void MT_HVDC::solve_converters_of_U_terminal(unsigned int terminal_number)
{
    ostringstream osstream;

    DC_TERMINAL* terminal_ptr = get_dc_terminal_with_terminal_number(terminal_number);
    DC_TERMINAL& terminal = *terminal_ptr;
    unsigned int dcbus_number = terminal.get_terminal_dc_bus_number();
    DC_BUS* dcbus_ptr = get_dc_bus_with_bus_number(dcbus_number);
    DC_BUS& dcbus = *dcbus_ptr;
    double Udc = dcbus.get_dc_bus_voltage_in_kV();
    double Idc = dcbus.get_dc_bus_current_in_kA();

    double U_terminal= Udc;
    //vector<DC_STATION*> stations = terminal.get_dc_stations_in_terminal();
    vector<DC_STATION*> stations = get_dc_stations_connect_to_terminal(terminal_number);
    unsigned int n_station = stations.size();
    for(unsigned int j=0;j!=n_station;j++)
    {
        DC_STATION& station = *(stations[j]);
        if(station.get_status()==false)
            continue;
        unsigned int station_number = station.get_station_number();
        //double U_station = get_voltage_of_voltage_station(station_number);

        DC_STATION_STRUCTURE structure = station.get_station_structure();
        if((structure==LCC_CASCADE) or (structure==LCC_HCM))
        {
            //vector<HLCC*> lccs = station.get_lcc_converters_in_dc_station_with_lcc_cascade();
            vector<HLCC*> lccs = get_lcc_converters_connect_to_station(station_number);
            unsigned int n_lcc = lccs.size();

            for(unsigned int i=0;i!=n_lcc;i++)
            {
                HLCC& lcc = *(lccs[i]);
                double U_lcc = lcc.get_command_udc_in_kV();
                double I_lcc = Idc;
                lcc.set_udc_in_kV(U_lcc);
                lcc.set_idc_in_kA(I_lcc);
                lcc.solve_converter_to_AC(U_lcc,I_lcc);

                //lcc.solve_lcc_with_tap_and_angle(U_lcc,I_lcc);

                //osstream<<"LCC:"<<lcc.get_converter_name()<<"solve with udc:"<<U_lcc<<"and idc:"<<I_lcc;
                //toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }

        if(structure==VSC_PARALLEL)
        {
            HVSC* vsc_converter_ptr = get_current_vsc_converter_with_top_priority(station_number);
            HVSC& vsc_converter = *vsc_converter_ptr;
            double U_station = get_voltage_of_voltage_station(station_number);
            double P_power_station = get_active_power_of_voltage_vsc_stations(station_number);
            double P_voltage_converter = U_station*Idc - P_power_station;
            vsc_converter.set_converter_P_to_DC_bus_MW(P_voltage_converter);
            vector<HVSC*> vscs = get_vsc_converters_connect_to_station(station_number);
            unsigned int n_vsc = vscs.size();
            for(unsigned int i=0;i!=n_vsc;i++)
            {
                HVSC& vsc = *(vscs[i]);
                vsc.set_udc_in_kV(U_station);
                double Pdc = vsc.get_converter_P_to_DC_bus_MW();
                vsc.set_idc_in_kA(Pdc/U_station);
            }
            vsc_converter.solve_ac_active_power_of_udc_control(P_voltage_converter);

        }
    }
}

void MT_HVDC::solve_converters_of_ANGLE_terminal(unsigned int terminal_number)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    ostringstream osstream;

    DC_TERMINAL* terminal_ptr = get_dc_terminal_with_terminal_number(terminal_number);
    DC_TERMINAL& terminal = *terminal_ptr;
    unsigned int dcbus_number = terminal.get_terminal_dc_bus_number();
    DC_BUS* dcbus_ptr = get_dc_bus_with_bus_number(dcbus_number);
    DC_BUS& dcbus = *dcbus_ptr;
    double Udc = dcbus.get_dc_bus_voltage_in_kV();
    double Idc = dcbus.get_dc_bus_current_in_kA();

    double U_terminal = Udc;

    vector<DC_STATION*> stations = get_dc_stations_connect_to_terminal(terminal_number);
    unsigned int n_station = stations.size();

    double U_station = U_terminal/n_station;
    for(unsigned int j=0;j!=n_station;j++)
    {
        DC_STATION& station = *(stations[j]);
        if(station.get_status()==false)
            continue;
        unsigned int station_number = station.get_station_number();
        DC_STATION_STRUCTURE structure = station.get_station_structure();
        if(structure==LCC_CASCADE)
        {
            //vector<HLCC*> lccs = station.get_lcc_converters_in_dc_station_with_lcc_cascade();
            vector<HLCC*> lccs = get_lcc_converters_connect_to_station(station_number);
            unsigned int n_lcc = lccs.size();

            for(unsigned int i=0;i!=n_lcc;i++)
            {
                double U_lcc = U_station/n_lcc;
                double I_lcc = Idc;
                HLCC& lcc = *(lccs[i]);
                lcc.set_udc_in_kV(U_lcc);
                lcc.set_idc_in_kA(I_lcc);
                lcc.solve_lcc_with_tap_and_angle(U_lcc,I_lcc);
            }
        }
        if(structure==LCC_HCM)
        {
            unsigned int n_angle = get_lcc_angle_control_number_of_hcm(station_number);
            if(n_angle==1)
            {
                HLCC* lcc_angle_ptr = get_lcc_converter_angle_fixed_of_HCM_station(station_number);
                HLCC* lcc_voltage_ptr = get_lcc_converter_angle_slack_of_HCM_station(station_number);
                HLCC& lcc_angle = *lcc_angle_ptr;
                HLCC& lcc_voltage = *lcc_voltage_ptr;

                double v_voltage = lcc_voltage.get_command_udc_in_kV();
                double v_angle = Udc-v_voltage;

                lcc_angle.set_udc_in_kV(v_angle);
                lcc_angle.set_idc_in_kA(Idc);
                lcc_angle.solve_lcc_with_tap_and_angle(v_angle,Idc);

                lcc_voltage.set_udc_in_kV(v_voltage);
                lcc_voltage.set_idc_in_kA(Idc);
                lcc_voltage.solve_converter_to_AC(v_voltage,Idc);
            }
            else if(n_angle==2)
            {
                vector<HLCC*> lccs = get_lcc_converters_connect_to_station(station_number);
                unsigned int n_lcc = lccs.size();

                for(unsigned int i=0;i!=n_lcc;i++)
                {
                    double U_lcc = 0.0;
                    double I_lcc = Idc;
                    HLCC& lcc = *(lccs[i]);
                    double n_bridge = lcc.get_brideg_number();
                    double vbase_grid = lcc.get_converter_transformer_grid_side_base_voltage_in_kV();
                    double vbase_converter = lcc.get_converter_transformer_converter_side_base_voltage_in_kV();
                    double k = vbase_grid/vbase_converter;

                    double tap = lcc.get_converter_transformer_tap_in_pu();
                    double ang_deg = lcc.get_command_firing_angle();
                    double ang_rad = deg2rad(ang_deg);
                    double cos_ang = steps_cos(ang_rad);
                    double vdrop = lcc.get_voltage_drop_per_bridge_in_kV();
                    complex<double> z = lcc.get_converter_transformer_Z_in_ohm();
                    double r = z.real();
                    double x = z.imag();
                    double vac = psdb.get_bus_positive_sequence_voltage_in_kV(lcc.get_converter_ac_bus());

                    double req = THREE_OVER_PI*x + 2*r;
                    U_lcc = n_bridge*(THREE_SQRT2_OVER_PI*vac*cos_ang/k/tap-I_lcc*req-vdrop);


                    lcc.set_idc_in_kA(I_lcc);
                    lcc.set_udc_in_kV(U_lcc);
                    lcc.solve_lcc_with_tap_and_angle(U_lcc,I_lcc);
                }
            }
            else
            {
                osstream<<n_angle<<" LCC CONVERTERS IS NOT SUPPORTED FOR HCM MODE";
                toolkit.show_information_with_leading_time_stamp(osstream);

            }
        }

        if(structure==VSC_PARALLEL)
        {
            osstream<<"ANGLE CONTROL IS NOT SUPPORTED FOR VSC CONVERTER";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void MT_HVDC::initialize_dc_power_flow()
{
    initialize_dc_bus_type();
    initialize_dc_voltage();
    initialize_dc_power();
}

void MT_HVDC::initialize_dc_bus_type()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    vector<DC_BUS*> dc_buses = get_all_dc_buses();
    unsigned int n_bus = dc_buses.size();
    for(unsigned int i=0;i!=n_bus;i++)
    {
        DC_BUS& dc_bus = *(dc_buses[i]);
        unsigned int dc_bus_number = dc_bus.get_dc_bus_number();
        DC_TERMINAL* dc_terminal_ptr = get_dc_terminal_with_dc_bus_number(dc_bus_number);
        if(dc_terminal_ptr==NULL)
        {
            dc_bus.set_dc_bus_type(DC_NOCON_BUS);//BUS WITH NO CONVERTER
        }
        else
        {
            DC_TERMINAL& dc_terminal = *dc_terminal_ptr;
            DC_TERMINAL_MODE tmode = dc_terminal.get_terminal_mode();
            if(tmode == DC_P_TERMIANL)
            {
                dc_bus.set_dc_bus_type(DC_P_NODE);
            }
            else if(tmode==DC_I_TERMINAL)
            {
                dc_bus.set_dc_bus_type(DC_I_NODE);
            }
            else if(tmode==DC_U_TERMINAL)
            {
                dc_bus.set_dc_bus_type(DC_U_NODE);
            }
            else if(tmode==DC_ANGLE_TERMINAL)
            {
                dc_bus.set_dc_bus_type(DC_ANGLE_NODE);
            }
            else
            {
                osstream<<"Control mode of DC terminal: "<<dc_terminal.get_terminal_number()<<" is not supported in STEPS";
                toolkit.show_information_with_leading_time_stamp(osstream);
                dc_bus.set_dc_bus_type(DC_NOCON_BUS);
            }
        }
    }
}

void MT_HVDC::initialize_dc_power()
{
    vector<DC_BUS*> dc_buses = get_all_dc_buses();
    unsigned int n_bus = dc_buses.size();
    for(unsigned int i=0;i!=n_bus;i++)
    {
        DC_BUS& dc_bus = *(dc_buses[i]);
        unsigned int dc_bus_number = dc_bus.get_dc_bus_number();
        DC_BUS_TYPE type = dc_bus.get_dc_bus_type();

        if(type == DC_NOCON_BUS)
            dc_bus.set_dc_bus_power_in_MW(0.0);
        else
        {
            DC_TERMINAL* dc_terminal_ptr = get_dc_terminal_with_dc_bus_number(dc_bus_number);
            DC_TERMINAL& dc_terminal = *dc_terminal_ptr;
            unsigned int terminal_number = dc_terminal.get_terminal_number();
            if(type==DC_P_NODE)
            {
                double Pref = get_active_power_of_power_terminal(terminal_number);
                dc_bus.set_dc_bus_power_in_MW(Pref);
            }
            else if(type==DC_I_NODE)
            {
                double Idc = get_current_of_current_terminal(terminal_number);
                double Udc = dc_bus.get_dc_bus_voltage_in_kV();
                double Pref = Idc*Udc;
                dc_bus.set_dc_bus_power_in_MW(Pref);
            }
            else if(type==DC_ANGLE_NODE)
            {
                double Pref = get_active_power_of_angle_terminal(terminal_number);
                dc_bus.set_dc_bus_power_in_MW(Pref);; ;
            }
            else if(type==DC_U_NODE)
            {
                dc_bus.set_dc_bus_power_in_MW(0.0);
            }
            else
            {
                dc_bus.set_dc_bus_power_in_MW(0.0);
            }
        }
    }
}

void MT_HVDC::initialize_dc_voltage()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    vector<DC_BUS*> dc_buses = get_all_dc_buses();
    unsigned int n_bus = dc_buses.size();
    for(unsigned int i=0;i!=n_bus;i++)
    {
        DC_BUS& dc_bus = *(dc_buses[i]);
        unsigned int dc_bus_number = dc_bus.get_dc_bus_number();

        DC_BUS_TYPE type = dc_bus.get_dc_bus_type();
        if(type==DC_U_NODE)
        {
            DC_TERMINAL* dc_terminal_ptr = get_dc_terminal_with_dc_bus_number(dc_bus_number);
            if(dc_terminal_ptr==NULL)
            {
                osstream<<"Erro for powerflow initialization of DC bus:"<<dc_bus_number<<" with U mode bu no termianl is connected";
                toolkit.show_information_with_leading_time_stamp(osstream);
                return;
            }
            DC_TERMINAL& dc_terminal = *dc_terminal_ptr;
            unsigned int terminal_number = dc_terminal.get_terminal_number();

            double Uref = get_voltage_of_voltage_terminal(terminal_number);
            dc_bus.set_dc_bus_voltage_in_kV(Uref);
        }
        else
        {
            double Uref = get_dc_base_voltage();
            dc_bus.set_dc_bus_voltage_in_kV(Uref);
        }

    }
}

void MT_HVDC::initialize_vsc_converters()
{
    vector<HVSC*> vscs = get_all_vsc_converters();
    unsigned int n = vscs.size();
    for(unsigned int i=0;i!=n;i++)
    {
        HVSC& vsc = *(vscs[i]);
        vsc.initialize_power_stations();
    }
}

void MT_HVDC::initialize_lcc_converters()
{
    vector<HLCC*> lccs = get_all_lcc_converters();
    unsigned int n = lccs.size();
    for(unsigned int i=0;i!=n;i++)
    {
        HLCC& lcc = *(lccs[i]);
        lcc.set_control_mode_change_flag(false);
        HLCC_CONTROL_MODE mode = lcc.get_control_mode();
        if(mode==LCC_CONSTANR_FIRING_ANGLE_MODE)
        {
            double min_tap = lcc.get_converter_transformer_min_tap_in_pu();
            double angle = lcc.get_command_firing_angle();
            lcc.set_converter_transformer_tap_in_pu(min_tap);
            lcc.set_firing_angle_in_deg(angle);
        }
    }
}

void MT_HVDC::initialize_physical_internal_bus_pair()
{
    vector<DC_BUS*> dc_buses = get_all_dc_buses();
    unsigned int n_dcbus = get_dc_bus_count();
    if(n_dcbus!=0)
    {
        inphno.clear();
        unsigned int internal_bus_number = 0;
        for(unsigned int i=0;i!=n_dcbus;i++)
        {
            DC_BUS& dc_bus = *(dc_buses[i]);
            DC_BUS_TYPE type = dc_bus.get_dc_bus_type();
            if(type==DC_P_NODE)
            {
                unsigned int bus_number = dc_bus.get_dc_bus_number();
                inphno.set_physical_internal_bus_number_pair(bus_number, internal_bus_number);
                ++internal_bus_number;
            }
        }

        for(unsigned int i=0;i!=n_dcbus;i++)
        {
            DC_BUS& dc_bus = *(dc_buses[i]);
            DC_BUS_TYPE type = dc_bus.get_dc_bus_type();
            if(type==DC_I_NODE)
            {
                unsigned int bus_number = dc_bus.get_dc_bus_number();
                inphno.set_physical_internal_bus_number_pair(bus_number, internal_bus_number);
                ++internal_bus_number;
            }
        }

        for(unsigned int i=0;i!=n_dcbus;i++)
        {
            DC_BUS& dc_bus = *(dc_buses[i]);
            DC_BUS_TYPE type = dc_bus.get_dc_bus_type();
            if(type==DC_ANGLE_NODE)
            {
                unsigned int bus_number = dc_bus.get_dc_bus_number();
                inphno.set_physical_internal_bus_number_pair(bus_number, internal_bus_number);
                ++internal_bus_number;
            }
        }

        for(unsigned int i=0;i!=n_dcbus;i++)
        {
            DC_BUS& dc_bus = *(dc_buses[i]);
            DC_BUS_TYPE type = dc_bus.get_dc_bus_type();
            if(type==DC_NOCON_BUS)
            {
                unsigned int bus_number = dc_bus.get_dc_bus_number();
                inphno.set_physical_internal_bus_number_pair(bus_number, internal_bus_number);
                ++internal_bus_number;
            }
        }

        for(unsigned int i=0;i!=n_dcbus;i++)
        {
            DC_BUS& dc_bus = *(dc_buses[i]);
            DC_BUS_TYPE type = dc_bus.get_dc_bus_type();
            if(type==DC_U_NODE)
            {
                unsigned int bus_number = dc_bus.get_dc_bus_number();
                inphno.set_physical_internal_bus_number_pair(bus_number, internal_bus_number);
                ++internal_bus_number;
            }
        }
    }
}


void MT_HVDC::build_dc_network_matrix()
{
    if(inphno.get_table_size()<2)
    {
        initialize_physical_internal_bus_pair();
    }

    dc_network_matrix.clear();
    build_dc_network_initial_zero_matrix();
    add_dc_line_to_dc_network_matrix();
    dc_network_matrix.compress_and_merge_duplicate_entries();
    //show_dc_network_matrix();
}


void MT_HVDC::build_dc_network_initial_zero_matrix()
{
    unsigned int n = get_dc_bus_count();
    for(unsigned int i=0;i!=n;i++)
    {
        dc_network_matrix.add_entry(i,i,0);
    }
}

void MT_HVDC::add_dc_line_to_dc_network_matrix()
{
    vector<DC_LINE*> dclines = get_all_dc_lines();
    unsigned int n = get_dc_line_count();
    for(unsigned int i=0;i!=n;i++)
    {
        DC_LINE& dcline = *(dclines[i]);
        if(dcline.get_status()==true)
        {
            unsigned int sending_bus = dcline.get_sending_side_bus_number();
            unsigned int receiving_bus = dcline.get_receiving_side_bus_number();
            double r = dcline.get_line_R_in_ohm();
            double g = 1/r;

            unsigned int ibus = inphno.get_internal_bus_number_of_physical_bus_number(sending_bus);
            unsigned int jbus = inphno.get_internal_bus_number_of_physical_bus_number(receiving_bus);
            dc_network_matrix.add_entry(ibus,ibus, g);
            dc_network_matrix.add_entry(ibus,jbus,-g);
            dc_network_matrix.add_entry(jbus,ibus,-g);
            dc_network_matrix.add_entry(jbus,jbus, g);
        }
    }
}

void MT_HVDC::show_dc_network_matrix()
{
    string buffer;
    char cbuffer[1000];

    int i=0, j=0, k=0;

    buffer="dc network matrix entries:\n";
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

double MT_HVDC::get_active_power_of_power_terminal(unsigned int terminal_number)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    double P_terminal = 0;
    DC_TERMINAL* terminal_ptr = get_dc_terminal_with_terminal_number(terminal_number);
    DC_TERMINAL& terminal = *terminal_ptr;

    if(terminal.get_status()==false)
        return 0.0;

    if(terminal.get_terminal_mode()==DC_P_TERMIANL)
    {
        vector<DC_STATION*> stations = get_dc_stations_connect_to_terminal(terminal_number);
        unsigned int n = stations.size();
        for(unsigned int i=0;i!=n;i++)
        {
            DC_STATION& station = *(stations[i]);
            DC_STATION_MODE smode = station.get_station_mode();
            unsigned int station_number = station.get_station_number();
            if(smode==DC_P_STATION)
            {
                double P_station = get_active_power_of_power_station(station_number);
                P_terminal = P_terminal+P_station;

            }
            else
            {
                osstream<<"Erro! Conflict modes between station: "<<station_number<<" in power terminal:"<<terminal.get_terminal_name();
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
        return P_terminal;;
    }
    else
    {
        osstream<<"Erro! DC terminal "<<terminal.get_terminal_name()<<" is not in power mode";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 0.0;
    }

}

double MT_HVDC::get_active_power_of_power_station(unsigned int station_number)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    double P_station = 0.0;
    DC_STATION* station_ptr = get_dc_station_with_station_number(station_number);
    DC_STATION& station = *station_ptr;
    if(station.get_status()==false)
        return 0.0;

    DC_STATION_STRUCTURE type = station.get_station_structure();
    if(type==LCC_CASCADE)
    {
        vector<HLCC*> lccs = get_lcc_converters_connect_to_station(station_number);
        unsigned int n_lcc = lccs.size();
        for(unsigned int i=0;i!=n_lcc;i++)
        {
            HLCC& lcc = *(lccs[i]);
            if(lcc.get_control_mode()!=LCC_CONSTANT_POWER_MODE)
            {
                osstream<<"Erro! Control mode of LCC:"<<lcc.get_converter_name()<<" in station: "<<station_number<<" conflict with station power mode";
                toolkit.show_information_with_leading_time_stamp(osstream);
                return 0.0;
            }
            double P_lcc = lcc.get_command_pdc_in_MW();
            P_station = P_station+P_lcc;
        }
        return P_station;
    }

    if(type==VSC_PARALLEL)
    {
        vector<HVSC*> vscs = get_vsc_converters_connect_to_station(station_number);
        unsigned n_vsc = vscs.size();
        for(unsigned int i=0;i!=n_vsc;i++)
        {
            HVSC& vsc = *(vscs[i]);
            if(vsc.get_converter_d_axis_control_mode()!=VSC_CONSTANT_AC_ACTIVE_POWER_MODE)
            {
                osstream<<"Erro! Control mode of VSC:"<<vsc.get_converter_name()<<" in station: "<<station_number<<" conflict with station power mode";
                toolkit.show_information_with_leading_time_stamp(osstream);
                return 0.0;
            }
            double Pac_vsc = vsc.get_converter_P_to_AC_bus_MW();
            double Qac_vsc = vsc.get_converter_Q_to_AC_bus_Mvar();
            double P_vsc = vsc.solve_dc_power_of_p_control(Pac_vsc, Qac_vsc);
            //osstream<<"POWER OF "<<vsc.get_converter_name()<<"Pac_vsc= "<<Pac_vsc<<" Qac_vsc="<<Qac_vsc;
            //toolkit.show_information_with_leading_time_stamp(osstream);
            P_station = P_station+P_vsc;
        }
        return P_station;
    }

    if(type==LCC_HCM)
    {
        vector<HLCC*> lccs = get_lcc_converters_connect_to_station(station_number);
        unsigned int n_lcc = lccs.size();
        for(unsigned int i=0;i!=n_lcc;i++)
        {
            HLCC& lcc = *(lccs[i]);
            if(lcc.get_control_mode()!=LCC_CONSTANT_POWER_MODE)
            {
                osstream<<"Erro! Control mode of LCC:"<<lcc.get_converter_name()<<" in station: "<<station_number<<" conflict with station power mode";
                toolkit.show_information_with_leading_time_stamp(osstream);
                return 0.0;
            }
            double P_lcc = lcc.get_command_pdc_in_MW();
            P_station = P_station+P_lcc;
        }
        return P_station;
    }
    return P_station;
}

double MT_HVDC::get_current_of_current_terminal(unsigned int terminal_number)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    double I_terminal = 0.0;
    double min_I_terminal = 0.0;
    DC_TERMINAL* terminal_ptr = get_dc_terminal_with_terminal_number(terminal_number);
    DC_TERMINAL& terminal = *terminal_ptr;

    if(terminal.get_status()==false)
        return 0.0;

    if(terminal.get_terminal_mode()==DC_I_TERMINAL)
    {
        vector<DC_STATION*> stations = get_dc_stations_connect_to_terminal(terminal_number);
        unsigned int n = stations.size();
        if(n==1)
        {
            DC_STATION& station_first = *(stations[0]);
            I_terminal = get_current_of_current_station(station_first.get_station_number());
        }
        else
        {
            DC_STATION& station_first = *(stations[0]);
            min_I_terminal = get_current_of_current_station(station_first.get_station_number());
            for(unsigned int i=0;i!=n;i++)
            {

                DC_STATION& station = *(stations[i]);
                double idc_station = get_current_of_current_station(station.get_station_number());
                if((abs(idc_station)<abs(min_I_terminal))&&(idc_station!=0))
                {
                    osstream<<"Warning! Station command current in terminal"<< terminal.get_terminal_name()<<"is inconsistent and the station current is set:"<<idc_station;
                    toolkit.show_information_with_leading_time_stamp(osstream);
                    min_I_terminal = idc_station;
                }
            }
            I_terminal = min_I_terminal;
        }
        return I_terminal;
    }
    else
    {
        osstream<<"Erro! DC terminal "<<terminal.get_terminal_name()<<" is not in current mode";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 0.0;
    }
}

double MT_HVDC::get_current_of_current_station(unsigned int station_number)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    double I_station = 0.0;
    DC_STATION* station_ptr = get_dc_station_with_station_number(station_number);
    DC_STATION& station = *station_ptr;
    DC_STATION_STRUCTURE type = station.get_station_structure();
    if(type==LCC_CASCADE)
    {
        vector<HLCC*> lccs = get_lcc_converters_connect_to_station(station_number);
        unsigned int n_lcc = lccs.size();
        for(unsigned int i=0;i!=n_lcc;i++)
        {
            HLCC& lcc = *(lccs[i]);
            if(lcc.get_control_mode()!=LCC_CONSTANT_CURRENT_MODE)
            {
                osstream<<"Control mode of LCC: "<<lcc.get_converter_name()<<"in station: "<<station.get_station_name()<<" conflict with station current mode";
                toolkit.show_information_with_leading_time_stamp(osstream);
                return 0.0;
            }
        }

        if(n_lcc==1)
        {
            HLCC& lcc_first = *(lccs[0]);
            I_station=lcc_first.get_command_idc_in_kA();
            return I_station;
        }
        else
        {
            HLCC& lcc_first = *(lccs[0]);
            double min_idc = lcc_first.get_command_idc_in_kA();
            for(unsigned int i=0;i!=n_lcc-1;i++)
            {
                HLCC& lcc = *(lccs[i]);
                double idc_lcc = lcc.get_command_idc_in_kA();
                if((abs(idc_lcc)<abs(min_idc)) && (idc_lcc!=0))
                {
                    osstream<<"The dc current command of LCC in station "<<station.get_station_name()<<"is inconsistent and the station current is set:"<<idc_lcc;
                    toolkit.show_information_with_leading_time_stamp(osstream);
                    min_idc = idc_lcc;
                }
            }
            I_station = min_idc;
            return I_station;
        }

    }
    if(type==VSC_PARALLEL)
    {
        osstream<<"Current control is not supported in VSC STATION"<<station_number;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 0.0;
    }

    if(type==LCC_HCM)
    {
        osstream<<"Current control is not supported in LCC HCM STATION"<<station_number;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 0.0;
    }
    return 0.0;

}

double MT_HVDC::get_voltage_of_voltage_terminal(unsigned int terminal_number)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    double U_terminal = 0;
    DC_TERMINAL* terminal_ptr = get_dc_terminal_with_terminal_number(terminal_number);
    DC_TERMINAL& terminal = *terminal_ptr;

    if(terminal.get_status()==false)
        return 0.0;

    if(terminal.get_terminal_mode()==DC_U_TERMINAL)
    {
        vector<DC_STATION*> stations = get_dc_stations_connect_to_terminal(terminal_number);
        unsigned int n = stations.size();
        for(unsigned int i=0;i!=n;i++)
        {
            DC_STATION& station = *(stations[i]);
            DC_STATION_MODE smode = station.get_station_mode();
            unsigned int station_number = station.get_station_number();
            if(smode==DC_U_STATION)
            {
                double U_station = get_voltage_of_voltage_station(station_number);
                U_terminal = U_terminal+U_station;

            }
            else
            {
                osstream<<"Erro! Conflict modes between station: "<<station_number<<" and terminal:"<<terminal_number;
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
        return U_terminal;
    }
    else
    {
        osstream<<"Erro! DC terminal "<<terminal.get_terminal_name()<<" is not in voltage mode";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 0.0;;
    }

}

double MT_HVDC::get_voltage_of_voltage_station(unsigned int station_number)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    DC_STATION* station_ptr = get_dc_station_with_station_number(station_number);
    DC_STATION& station = *station_ptr;

    if(station.get_status()==false)
        return 0.0;
    double U_station = 0.0;
    DC_STATION_STRUCTURE type = station.get_station_structure();
    if((type==LCC_CASCADE) or (type==LCC_HCM))
    {
        vector<HLCC*> lccs = get_lcc_converters_connect_to_station(station_number);
        unsigned int n_lcc = lccs.size();
        for(unsigned int i=0;i!=n_lcc;i++)
        {
            HLCC& lcc = *(lccs[i]);
            if(lcc.get_control_mode()!=LCC_CONSTANT_VOLTAGE_MODE)
            {
                osstream<<"Erro! Control mode of LCC:"<<lcc.get_converter_name()<<" in station: "<<station_number<<" conflict with station power mode";
                toolkit.show_information_with_leading_time_stamp(osstream);
                return 0.0;
            }
            double U_lcc = lcc.get_command_udc_in_kV();
            U_station = U_station+U_lcc;
        }
        return U_station;
    }

    if(type==VSC_PARALLEL)
    {

        HVSC* vsc_ptr = get_current_vsc_converter_with_top_priority(station_number);
        HVSC& vsc = *vsc_ptr;
        double udc = vsc.get_converter_Udc_command();
        return udc;
    }
    return 0.0;
}

double MT_HVDC::get_active_power_of_voltage_vsc_stations(unsigned int station_number)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    DC_STATION* station_ptr = get_dc_station_with_station_number(station_number);
    DC_STATION& station = *station_ptr;
    if(station.get_station_structure()!=VSC_PARALLEL)
    {
        osstream<<"DC station: "<<station.get_station_name()<<"is not VSC station";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 0.0;
    }

    vector<HVSC*> vscs = get_vsc_converters_connect_to_station(station_number);
    unsigned int n_vsc = vscs.size();
    double P = 0.0;
    for(unsigned int i=0;i!=n_vsc;i++)
    {
        double P_vsc = 0.0;
        HVSC& vsc = *(vscs[i]);
        HVSC_D_CONTROL_MODE dmode = vsc.get_converter_d_axis_control_mode();
        if(dmode==VSC_CONSTANT_AC_ACTIVE_POWER_MODE)
        {
            double Pac = vsc.get_converter_P_to_AC_bus_MW();
            double Qac = vsc.get_converter_Q_to_AC_bus_Mvar();
            P_vsc = vsc.solve_dc_power_of_p_control(Pac,Qac);
            P+=P_vsc;

        }
        else if(dmode==VSC_CONSTANT_DC_VOLTAGE_MODE)
        {
            P_vsc = 0.0;
        }

        else
        {
            P_vsc = 0.0;
        }
    }
    return P;
}


double MT_HVDC::get_active_power_of_angle_terminal(unsigned int terminal_number)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    ostringstream osstream;
    DC_TERMINAL* terminal_ptr = get_dc_terminal_with_terminal_number(terminal_number);
    DC_TERMINAL& terminal = *terminal_ptr;
    if(terminal.get_status()==false)
        return 0.0;

    double P_terminal = 0.0;
    vector<DC_STATION*> stations = get_dc_stations_connect_to_terminal(terminal_number);
    unsigned int n_station = stations.size();
    if(n_station==1)
    {
        DC_STATION& station = *(stations[0]);
        unsigned int station_number = station.get_station_number();
        DC_STATION_STRUCTURE structure = station.get_station_structure();
        if(structure==LCC_CASCADE)
        {
            vector<HLCC*> lccs = get_lcc_converters_connect_to_station(station_number);
            HLCC& lcc = *(lccs[0]);
            unsigned int n_lcc = lccs.size();
            DC_BUS* dcbus_ptr = get_dc_bus_with_terminal_number(terminal_number);
            DC_BUS& dcbus = *dcbus_ptr;
            double vdc = dcbus.get_dc_bus_voltage_in_kV();
            double vac = psdb.get_bus_positive_sequence_voltage_in_kV(lcc.get_converter_ac_bus());

            unsigned int n_bridge = lcc.get_brideg_number();
            double vbase_grid = lcc.get_converter_transformer_grid_side_base_voltage_in_kV();
            double vbase_converter = lcc.get_converter_transformer_converter_side_base_voltage_in_kV();
            double k = vbase_grid/vbase_converter;
            double tap = lcc.get_converter_transformer_tap_in_pu();

            double ang_in_deg = lcc.get_command_firing_angle();
            double ang_in_rad = deg2rad(ang_in_deg);
            double cos_ang = steps_cos(ang_in_rad);

            double vdrop = lcc.get_voltage_drop_per_bridge_in_kV();

            complex<double> Z = lcc.get_converter_transformer_Z_in_ohm();
            double r = Z.real();
            double x = Z.imag();

            double req = THREE_OVER_PI*x + 2*r;
            double ueq = -vdc*vdc/n_lcc/n_bridge+(THREE_SQRT2_OVER_PI*vac*cos_ang/k/tap-vdrop)*vdc;

            P_terminal = ueq/req;
            return P_terminal;

        }

        if(structure==LCC_HCM)
        {
            vector<HLCC*> lccs = get_lcc_converters_connect_to_station(station_number);
            unsigned int n_lcc = lccs.size();
            if(n_lcc!=2)
            {
                osstream<<"Erro! Only 2 converters with HCM mode is not supported in terminal "<<terminal.get_terminal_name();
                toolkit.show_information_with_leading_time_stamp(osstream);
                return 0.0;
            }
            unsigned int n_angle = get_lcc_angle_control_number_of_hcm(station_number);
            if(n_angle==1)
            {
                P_terminal=get_active_power_of_HCM_terminal_with_1_angle_fixed(station_number);
                return P_terminal;
            }
            if(n_angle==2)
            {
                P_terminal=get_active_power_of_HCM_terminal_with_2_angle_fixed(station_number);
                return P_terminal;
            }
        }
        return 0.0;
    }
    else
    {
        osstream<<"ANGLE control is only supported with one station in terminal"<<terminal_number;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 0.0;
    }
}

double MT_HVDC::get_active_power_of_HCM_terminal_with_1_angle_fixed(unsigned int station_number)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    ostringstream osstream;

    double P_station =0.0;
    DC_STATION* station_ptr = get_dc_station_with_station_number(station_number);
    DC_STATION& station = *station_ptr;
    unsigned int terminal_number = station.get_terminal_number();
    DC_BUS* dcbus_ptr = get_dc_bus_with_terminal_number(terminal_number);
    DC_BUS& dcbus = *dcbus_ptr;
    double vdc = dcbus.get_dc_bus_voltage_in_kV();

    HLCC* lcc_angle_ptr = get_lcc_converter_angle_fixed_of_HCM_station(station_number);
    HLCC* lcc_voltage_ptr = get_lcc_converter_angle_slack_of_HCM_station(station_number);
    HLCC& lcc_angle = *lcc_angle_ptr;
    HLCC& lcc_voltage = *lcc_voltage_ptr;

    unsigned int n_bridge = lcc_angle.get_brideg_number();
    double vbase_grid = lcc_angle.get_converter_transformer_grid_side_base_voltage_in_kV();
    double vbase_converter = lcc_angle.get_converter_transformer_converter_side_base_voltage_in_kV();
    double k = vbase_grid/vbase_converter;
    double tap = lcc_angle.get_converter_transformer_tap_in_pu();

    double ang_deg = lcc_angle.get_command_firing_angle();
    double ang_rad = deg2rad(ang_deg);
    double cos_ang = steps_cos(ang_rad);

    double vdrop = lcc_angle.get_voltage_drop_per_bridge_in_kV();
    complex<double> z = lcc_angle.get_converter_transformer_Z_in_ohm();
    double r = z.real();
    double x = z.imag();
    double vac = psdb.get_bus_positive_sequence_voltage_in_kV(lcc_angle.get_converter_ac_bus());

    double vdc1 = lcc_voltage.get_command_udc_in_kV();

    double req = THREE_OVER_PI*x + 2*r;
    double ueq = -(vdc-vdc1)/n_bridge+(THREE_SQRT2_OVER_PI*vac*cos_ang/k/tap-vdrop);
    double ieq = ueq/req;

    P_station = vdc*ieq;

    return P_station;

}

double MT_HVDC::get_active_power_of_HCM_terminal_with_2_angle_fixed(unsigned int station_number)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    ostringstream osstream;

    double P_station =0.0;
    DC_STATION* station_ptr = get_dc_station_with_station_number(station_number);
    DC_STATION& station = *station_ptr;
    unsigned int terminal_number = station.get_terminal_number();
    DC_BUS* dcbus_ptr = get_dc_bus_with_terminal_number(terminal_number);
    DC_BUS& dcbus = *dcbus_ptr;
    double vdc = dcbus.get_dc_bus_voltage_in_kV();

    vector<HLCC*> lccs = get_lcc_converters_connect_to_station(station_number);
    HLCC& lcc1 = *(lccs[0]);
    HLCC& lcc2 = *(lccs[1]);

    unsigned int n_bridge1 = lcc1.get_brideg_number();

    double vbase_grid1 = lcc1.get_converter_transformer_grid_side_base_voltage_in_kV();
    double vbase_converter1 = lcc1.get_converter_transformer_converter_side_base_voltage_in_kV();
    double k1 = vbase_grid1/vbase_converter1;

    double tap1 = lcc1.get_converter_transformer_tap_in_pu();

    double ang_in_deg1 = lcc1.get_command_firing_angle();
    double ang_in_rad1 = deg2rad(ang_in_deg1);
    double cos_ang1 = steps_cos(ang_in_rad1);

    double vdrop1 = lcc1.get_voltage_drop_per_bridge_in_kV();

    double vac1 = psdb.get_bus_positive_sequence_voltage_in_kV(lcc1.get_converter_ac_bus());
    complex<double> z1 = lcc1.get_converter_transformer_Z_in_ohm();
    double r1 = z1.real();
    double x1 = z1.imag();

    unsigned int n_bridge2 = lcc2.get_brideg_number();

    double vbase_grid2 = lcc2.get_converter_transformer_grid_side_base_voltage_in_kV();
    double vbase_converter2 = lcc2.get_converter_transformer_converter_side_base_voltage_in_kV();
    double k2 = vbase_grid2/vbase_converter2;

    double tap2 = lcc2.get_converter_transformer_tap_in_pu();

    double ang_in_deg2 = lcc2.get_command_firing_angle();
    double ang_in_rad2 = deg2rad(ang_in_deg2);
    double cos_ang2 = steps_cos(ang_in_rad2);

    double vdrop2 = lcc2.get_voltage_drop_per_bridge_in_kV();

    double vac2 = psdb.get_bus_positive_sequence_voltage_in_kV(lcc2.get_converter_ac_bus());
    complex<double> z2 = lcc2.get_converter_transformer_Z_in_ohm();
    double r2 = z2.real();
    double x2 = z2.imag();

    double req1 = n_bridge1*(THREE_OVER_PI*x1+2*r1);
    double req2 = n_bridge2*(THREE_OVER_PI*x2+2*r2);

    double ueq1 = n_bridge1*THREE_SQRT2_OVER_PI*vac1*cos_ang1/k1/tap1-n_bridge1*vdrop1;
    double ueq2 = n_bridge2*THREE_SQRT2_OVER_PI*vac2*cos_ang2/k2/tap2-n_bridge2*vdrop2;

    double Ieq = (-vdc+ueq1+ueq2)/(req1+req2);
    P_station = vdc*Ieq;

    osstream<<"ACTIVE POWER OF HCM STATION "<<station.get_station_name()<<" WITH 2 LCC ANGLE FIXED IS "<<P_station;
    toolkit.show_information_with_leading_time_stamp(osstream);

    return P_station;

}


double MT_HVDC::get_jacobian_element_of_HCM_terminal_with_1_angle_fixed(unsigned int station_number)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    ostringstream osstream;

    double der =0.0;
    DC_STATION* station_ptr = get_dc_station_with_station_number(station_number);
    DC_STATION& station = *station_ptr;
    unsigned int terminal_number = station.get_terminal_number();
    DC_BUS* dcbus_ptr = get_dc_bus_with_terminal_number(terminal_number);
    DC_BUS& dcbus = *dcbus_ptr;
    double vdc = dcbus.get_dc_bus_voltage_in_kV();

    HLCC* lcc_angle_ptr = get_lcc_converter_angle_fixed_of_HCM_station(station_number);
    HLCC* lcc_voltage_ptr = get_lcc_converter_angle_slack_of_HCM_station(station_number);
    HLCC& lcc_angle = *lcc_angle_ptr;
    HLCC& lcc_voltage = *lcc_voltage_ptr;

    unsigned int n_bridge = lcc_angle.get_brideg_number();
    double vbase_grid = lcc_angle.get_converter_transformer_grid_side_base_voltage_in_kV();
    double vbase_converter = lcc_angle.get_converter_transformer_converter_side_base_voltage_in_kV();
    double k = vbase_grid/vbase_converter;
    double tap = lcc_angle.get_converter_transformer_tap_in_pu();

    double ang_deg = lcc_angle.get_command_firing_angle();
    double ang_rad = deg2rad(ang_deg);
    double cos_ang = steps_cos(ang_rad);

    double vdrop = lcc_angle.get_voltage_drop_per_bridge_in_kV();
    complex<double> z = lcc_angle.get_converter_transformer_Z_in_ohm();
    double r = z.real();
    double x = z.imag();
    double vac = psdb.get_bus_positive_sequence_voltage_in_kV(lcc_angle.get_converter_ac_bus());

    double vdc1 = lcc_voltage.get_command_udc_in_kV();

    double req = THREE_OVER_PI*x + 2*r;
    double ueq = (-2*vdc+vdc1)/n_bridge+(THREE_SQRT2_OVER_PI*vac*cos_ang/k/tap-vdrop);

    der = ueq/req;

    return der;
}

double MT_HVDC::get_jacobian_element_of_HCM_terminal_with_2_angle_fixed(unsigned int station_number)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    ostringstream osstream;

    double der =0.0;
    DC_STATION* station_ptr = get_dc_station_with_station_number(station_number);
    DC_STATION& station = *station_ptr;
    unsigned int terminal_number = station.get_terminal_number();
    DC_BUS* dcbus_ptr = get_dc_bus_with_terminal_number(terminal_number);
    DC_BUS& dcbus = *dcbus_ptr;
    double vdc = dcbus.get_dc_bus_voltage_in_kV();

    vector<HLCC*> lccs = get_lcc_converters_connect_to_station(station_number);
    HLCC& lcc1 = *(lccs[0]);
    HLCC& lcc2 = *(lccs[1]);

    unsigned int n_bridge1 = lcc1.get_brideg_number();

    double vbase_grid1 = lcc1.get_converter_transformer_grid_side_base_voltage_in_kV();
    double vbase_converter1 = lcc1.get_converter_transformer_converter_side_base_voltage_in_kV();
    double k1 = vbase_grid1/vbase_converter1;

    double tap1 = lcc1.get_converter_transformer_tap_in_pu();

    double ang_in_deg1 = lcc1.get_command_firing_angle();
    double ang_in_rad1 = deg2rad(ang_in_deg1);
    double cos_ang1 = steps_cos(ang_in_rad1);

    double vdrop1 = lcc1.get_voltage_drop_per_bridge_in_kV();

    double vac1 = psdb.get_bus_positive_sequence_voltage_in_kV(lcc1.get_converter_ac_bus());
    complex<double> z1 = lcc1.get_converter_transformer_Z_in_ohm();
    double r1 = z1.real();
    double x1 = z1.imag();

    unsigned int n_bridge2 = lcc2.get_brideg_number();

    double vbase_grid2 = lcc2.get_converter_transformer_grid_side_base_voltage_in_kV();
    double vbase_converter2 = lcc2.get_converter_transformer_converter_side_base_voltage_in_kV();
    double k2 = vbase_grid2/vbase_converter2;

    double tap2 = lcc2.get_converter_transformer_tap_in_pu();

    double ang_in_deg2 = lcc2.get_command_firing_angle();
    double ang_in_rad2 = deg2rad(ang_in_deg2);
    double cos_ang2 = steps_cos(ang_in_rad2);

    double vdrop2 = lcc2.get_voltage_drop_per_bridge_in_kV();

    double vac2 = psdb.get_bus_positive_sequence_voltage_in_kV(lcc2.get_converter_ac_bus());
    complex<double> z2 = lcc2.get_converter_transformer_Z_in_ohm();
    double r2 = z2.real();
    double x2 = z2.imag();

    double req1 = n_bridge1*(THREE_OVER_PI*x1+2*r1);
    double req2 = n_bridge2*(THREE_OVER_PI*x2 + r2);

    double ueq1 = n_bridge1*THREE_SQRT2_OVER_PI*vac1*cos_ang1/k1/tap1-n_bridge1*vdrop1;
    double ueq2 = n_bridge2*THREE_SQRT2_OVER_PI*vac2*cos_ang2/k2/tap2-n_bridge2*vdrop2;

    der = (-2*vdc+ueq1+ueq2)/(req1+req2);

    return der;
}

double MT_HVDC::get_jacobian_element_of_angle_terminal(unsigned int terminal_number)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    ostringstream osstream;

    double der = 0.0;
    vector<DC_STATION*> stations = get_dc_stations_connect_to_terminal(terminal_number);
    unsigned int n_station = stations.size();
    if(n_station==1)
    {
        DC_STATION& station = *(stations[0]);
        unsigned int station_number = station.get_station_number();
        //DC_STATION_MODE smode == station.get_station_mode();
        DC_STATION_STRUCTURE structure = station.get_station_structure();
        if(structure==LCC_CASCADE)
        {
            vector<HLCC*> lccs = get_lcc_converters_connect_to_station(station_number);
            HLCC& lcc = *(lccs[0]);
            unsigned int n_lcc = lccs.size();
            DC_BUS* dcbus_ptr = get_dc_bus_with_terminal_number(terminal_number);
            DC_BUS& dcbus = *dcbus_ptr;
            double vdc = dcbus.get_dc_bus_voltage_in_kV();
            double vac = psdb.get_bus_positive_sequence_voltage_in_kV(lcc.get_converter_ac_bus());

            unsigned int n_bridge = lcc.get_brideg_number();
            double vbase_grid = lcc.get_converter_transformer_grid_side_base_voltage_in_kV();
            double vbase_converter = lcc.get_converter_transformer_converter_side_base_voltage_in_kV();
            double k = vbase_grid/vbase_converter;
            double tap = lcc.get_converter_transformer_min_tap_in_pu();

            double ang_in_deg = lcc.get_command_firing_angle();
            double ang_in_rad = deg2rad(ang_in_deg);
            double cos_ang = steps_cos(ang_in_rad);

            double vdrop = lcc.get_voltage_drop_per_bridge_in_kV();

            complex<double> Z = lcc.get_converter_transformer_Z_in_ohm();
            double r = Z.real();
            double x = Z.imag();

            double req = THREE_OVER_PI*x + 2*r;
            double ueq = -2*vdc/n_lcc/n_bridge+(THREE_SQRT2_OVER_PI*vac*cos_ang/k/tap-vdrop);

            der = ueq/req;

            return der;

        }

        if(structure==LCC_HCM)
        {
            unsigned int n_angle = get_lcc_angle_control_number_of_hcm(station_number);
            if(n_angle==1)
            {
                der = get_jacobian_element_of_HCM_terminal_with_1_angle_fixed(station_number);
                return der;
            }

            if(n_angle==2)
            {
                der = get_jacobian_element_of_HCM_terminal_with_2_angle_fixed(station_number);
                return der;
            }
        }
        return 0.0;
    }
    else
    {
        osstream<<"ANGLE control is only supported in one station with 2 LCC";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 0.0;
    }
}

unsigned int MT_HVDC::get_number_of_dc_u_bus()
{
    unsigned int n_ubus = 0;
    vector<DC_BUS*> dc_buses = get_all_dc_buses();
    unsigned int n = dc_buses.size();
    for(unsigned int i=0;i!=n;i++)
    {
        DC_BUS& dc_bus = *(dc_buses[i]);
        DC_BUS_TYPE type = dc_bus.get_dc_bus_type();
        if(type==DC_U_NODE)
        {
            n_ubus++;
        }
    }
    return n_ubus;

}

void MT_HVDC::calculate_raw_power_mismatch()
{
    ostringstream osstream;

    unsigned int n_dcbus = get_dc_bus_count();
    unsigned int n_ubus = get_number_of_dc_u_bus();

    unsigned int n_pmismatch = n_dcbus-n_ubus;
    p_mismatch.clear();
    p_mismatch.reserve(n_pmismatch);
    for(unsigned int i=0;i!=n_pmismatch;i++)
    {
        p_mismatch.push_back(0.0);
    }

    calculate_current_to_dc();//I=YU
    calculate_power_to_dc();  //P=UI
    for(unsigned int i=0;i!=n_pmismatch;i++)
    {
        unsigned int bus_number = inphno.get_physical_bus_number_of_internal_bus_number(i);
        //DC_BUS* dc_bus = get_dc_bus_with_bus_number(bus_number);
        DC_TERMINAL* terminal_ptr = get_dc_terminal_with_dc_bus_number(bus_number);
        if(terminal_ptr==NULL)
        {
            double pmis = -bus_power[i];
            p_mismatch[i] = pmis;;
        }
        else
        {
            DC_TERMINAL& terminal = *terminal_ptr;
            unsigned int terminal_number = terminal.get_terminal_number();
            double Pc = get_dc_power_from_terminal(terminal_number);

            double pmis = Pc-bus_power[i];
            p_mismatch[i] = pmis;
        }

    }

    //add_dc_source_to_power_mismatch();
    //add_dc_load_to_power_mismatch();
}

void MT_HVDC::calculate_current_to_dc()
{
    ostringstream osstream;

    unsigned int nbus=get_dc_bus_count();
    bus_current.clear();
    bus_current.reserve(nbus);
    for(unsigned int i=0;i!=nbus;++i)
        bus_current.push_back(0.0);
    int nsize = dc_network_matrix.get_matrix_size();


    int k_start=0;
    for(int col=0;col!=nsize;col++)
    {
        unsigned int bus_number = inphno.get_physical_bus_number_of_internal_bus_number(col);
        DC_BUS* dcbus_ptr = get_dc_bus_with_bus_number(bus_number);
        DC_BUS& dcbus = *dcbus_ptr;
        double voltage = dcbus.get_dc_bus_voltage_in_kV();
        int k_end = dc_network_matrix.get_starting_index_of_column(col+1);
        for(int k=k_start;k<k_end;k++)
        {
            double yij = dc_network_matrix.get_real_entry_value(k);
            int row = dc_network_matrix.get_row_number_of_entry_index(k);
            bus_current[row]+=yij*voltage;

        }
        k_start = k_end;
    }
}

void MT_HVDC::calculate_power_to_dc()
{
    unsigned int nbus = get_dc_bus_count();
    bus_power.clear();
    for(unsigned int j=0;j!=nbus;j++)
    {
        bus_power.push_back(0.0);
    }
    for(unsigned int i=0;i!=nbus;i++)
    {
        unsigned int bus_no = inphno.get_physical_bus_number_of_internal_bus_number(i);
        DC_BUS* dc_bus_ptr = get_dc_bus_with_bus_number(bus_no);
        DC_BUS& dc_bus = *dc_bus_ptr;
        double voltage = dc_bus.get_dc_bus_voltage_in_kV();
        double i_bus = bus_current[i];
        bus_power[i] = voltage*i_bus;
    }
}

double MT_HVDC::get_dc_power_from_terminal(unsigned int bus_number)
{
    double Pdc = 0.0;
    DC_TERMINAL* terminal_ptr = get_dc_terminal_with_dc_bus_number(bus_number);
    DC_TERMINAL& terminal = *terminal_ptr;
    DC_TERMINAL_MODE tmode = terminal.get_terminal_mode();
    unsigned int terminal_number = terminal.get_terminal_number();

    if(tmode==DC_P_TERMIANL)
    {
        Pdc = get_active_power_of_power_terminal(terminal_number);
        return Pdc;
    }
    else if(tmode==DC_I_TERMINAL)
    {
        double Idc = get_current_of_current_terminal(terminal_number);
        DC_BUS* bus_ptr = get_dc_bus_with_bus_number(bus_number);
        DC_BUS& bus = *bus_ptr;
        double Udc = bus.get_dc_bus_voltage_in_kV();
        Pdc = Idc*Udc;
    }
    else if(tmode==DC_U_TERMINAL)
    {
        Pdc = 0.0;
    }
    else if(tmode==DC_ANGLE_TERMINAL)
    {
        Pdc = get_active_power_of_angle_terminal(terminal_number);
    }
    else
    {
        Pdc = 0.0;
    }

    return Pdc;
}


double MT_HVDC::get_max_active_power_mismatch_in_MW()
{
    unsigned int n = p_mismatch.size();
    unsigned int max_power_mismatch_bus = 0;
    double max_power_mismatch = 0.0;
    for(unsigned int i=0;i!=n;i++)
    {
        if(fabs(p_mismatch[i])<max_power_mismatch)
            continue;
        else
        {
            max_power_mismatch = fabs(p_mismatch[i]);
            max_power_mismatch_bus = inphno.get_physical_bus_number_of_internal_bus_number(i);
        }
    }
    //show max power mismatch
    //STEPS& toolkit = get_toolkit();
    //ostringstream osstream;
    //osstream << "The maximum active power mismatch is: " << max_power_mismatch << "in bus: " << max_power_mismatch_bus;
    //toolkit.show_information_with_leading_time_stamp(osstream);
    return max_power_mismatch;
}

void MT_HVDC::build_jacobian()
{
    ostringstream osstream;

    if(not is_dc_network_matrix_set())
    {
        build_dc_network_matrix();
    }
    jacobian.clear();
    unsigned int n_dcbus = get_dc_bus_count();
    unsigned int n_ubus = get_number_of_dc_u_bus();

    unsigned int n_jacobian = n_dcbus-n_ubus;

    for(unsigned int i=0;i!=n_jacobian;i++)
    {
        jacobian.add_entry(i,i,0);
    }

    unsigned int n = jacobian.get_matrix_size();
    for(unsigned int j=0;j!=n;j++)
    {
        for(unsigned int k=0;k!=n;k++)
        {
            double der = calculate_jacobian_element(j,k);
            jacobian.add_entry(j,k,der);
        }
    }

    jacobian.compress_and_merge_duplicate_entries();
    //show_jacobian_matrix();
}


double MT_HVDC::calculate_jacobian_element(unsigned int row, unsigned int col)
{
    ostringstream osstream;

    double der = 0.0;
    int k = dc_network_matrix.get_entry_index(row,col);
    int n = dc_network_matrix.get_matrix_size();

    unsigned int ibus = inphno.get_physical_bus_number_of_internal_bus_number(row);
    unsigned int jbus = inphno.get_physical_bus_number_of_internal_bus_number(col);
    DC_BUS* i_dcbus_ptr = get_dc_bus_with_bus_number(ibus);
    DC_BUS* j_dcbus = get_dc_bus_with_bus_number(jbus);

    DC_BUS& i_dcbus = *i_dcbus_ptr;
    DC_BUS_TYPE type = i_dcbus.get_dc_bus_type();
    if((type==DC_P_NODE) or (type==DC_NOCON_BUS))
    {
        if(row==col)
        {
            der = 0.0;
            for(unsigned int i=0;i!=n;i++)
            {
                unsigned int temp_col = i;
                int temp_k = dc_network_matrix.get_entry_index(row,temp_col);
                double temp_yij = dc_network_matrix.get_real_entry_value(temp_k);

                int temp_jbus_number = inphno.get_physical_bus_number_of_internal_bus_number(temp_col);
                DC_BUS* temp_jbus_ptr = get_dc_bus_with_bus_number(temp_jbus_number);
                DC_BUS& temp_jbus = *temp_jbus_ptr;
                double temp_voltage = temp_jbus.get_dc_bus_voltage_in_kV();
                //osstream << "The  temp voltage of " <<temp_jbus<<"   is:   " <<temp_voltage << "The  temp yij of is:   " <<temp_yij;
                //toolkit.show_information_with_leading_time_stamp(osstream);
                der-=temp_yij*temp_voltage;

            }
            double yii = dc_network_matrix.get_real_entry_value(k);
            double vii = i_dcbus.get_dc_bus_voltage_in_kV();
            der = der-yii*vii;
            return der;
        }

        else //row != col
        {
            double yij = dc_network_matrix.get_real_entry_value(k);
            double voltage = i_dcbus.get_dc_bus_voltage_in_kV();
            der = -yij*voltage;
            return der;
        }
    }
    else if(type==DC_I_NODE)
    {
        if(row==col)
        {
            der = 0.0;
            for(unsigned int i=0;i!=n;i++)
            {
                int temp_col = i;
                int temp_k = dc_network_matrix.get_entry_index(row,temp_col);
                double temp_yij = dc_network_matrix.get_real_entry_value(temp_k);

                int temp_jbus_number = inphno.get_physical_bus_number_of_internal_bus_number(temp_col);
                DC_BUS* temp_jbus_ptr = get_dc_bus_with_bus_number(temp_jbus_number);
                DC_BUS& temp_jbus = *temp_jbus_ptr;
                double temp_voltage = temp_jbus.get_dc_bus_voltage_in_kV();

                der -= (temp_voltage*temp_yij);
            }

            double yii = dc_network_matrix.get_real_entry_value(k);
            double vii = i_dcbus.get_dc_bus_voltage_in_kV();
            der=der-yii*vii;

            DC_TERMINAL* terminal_ptr = get_dc_terminal_with_dc_bus_number(ibus);
            DC_TERMINAL& terminal = *terminal_ptr;
            unsigned int terminal_number = terminal.get_terminal_number();
            double Iref = get_current_of_current_terminal(terminal_number);
            //osstream <<row<<" and " <<col<< " current of I " << Iref <<" der is "<<der;
            //toolkit.show_information_with_leading_time_stamp(osstream);
            return (Iref+der);
        }
        else //row!=col
        {

            double yij = dc_network_matrix.get_real_entry_value(k);
            double voltage = i_dcbus.get_dc_bus_voltage_in_kV();
            der = -yij*voltage;
            return der;
        }
    }

    else if(type==DC_ANGLE_NODE)
    {
        if(row==col)
        {
            der = 0.0;
            for(unsigned int i=0;i!=n;i++)
            {
                int temp_col = i;
                int temp_k = dc_network_matrix.get_entry_index(row,temp_col);
                double temp_yij = dc_network_matrix.get_real_entry_value(temp_k);

                int temp_jbus_number = inphno.get_physical_bus_number_of_internal_bus_number(temp_col);
                DC_BUS* temp_jbus_ptr = get_dc_bus_with_bus_number(temp_jbus_number);
                DC_BUS& temp_jbus = *temp_jbus_ptr;
                double temp_voltage = temp_jbus.get_dc_bus_voltage_in_kV();

                der -= (temp_voltage*temp_yij);
            }

            double yii = dc_network_matrix.get_real_entry_value(k);
            double vii = i_dcbus.get_dc_bus_voltage_in_kV();
            der=der-yii*vii;

            DC_TERMINAL* terminal_ptr = get_dc_terminal_with_dc_bus_number(ibus);
            DC_TERMINAL& terminal = *terminal_ptr;
            unsigned int terminal_number = terminal.get_terminal_number();
            double Iref = get_jacobian_element_of_angle_terminal(terminal_number);
            //osstream <<row<<" and " <<col<< " current of I " << Iref <<" der is "<<der;
            //toolkit.show_information_with_leading_time_stamp(osstream);
            return (Iref+der);
        }

        else
        {
            double yij = dc_network_matrix.get_real_entry_value(k);
            double voltage = i_dcbus.get_dc_bus_voltage_in_kV();
            der = -yij*voltage;
            return der;;
        }
    }

    else
    {
        der = 0.0;
        return der;
    }
}

bool MT_HVDC::is_dc_network_matrix_set()
{
    unsigned int n = dc_network_matrix.get_matrix_size();
    if(n>=2)
        return true;
    else
        return false;
}

void MT_HVDC::show_jacobian_matrix()
{
    string buffer;
    char cbuffer[1000];

    int i=0, j=0, k=0;

    buffer="jacobian matrix entries:\n";
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

void MT_HVDC::show_bus_voltage()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    vector<DC_BUS*> buses = get_all_dc_buses();
    unsigned int nbus = get_dc_bus_count();
    for(unsigned int i=0;i!=nbus;i++)
    {
        DC_BUS& dcbus = *(buses[i]);
        unsigned int bus_number = dcbus.get_dc_bus_number();
        double vdc = dcbus.get_dc_bus_voltage_in_kV();
        osstream << "Voltage of bus "<< bus_number << " is: "<< vdc;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}
void MT_HVDC::update_bus_current()
{
    ostringstream osstream;

    int nsize = dc_network_matrix.get_matrix_size();
    vector<double> idc_bus;
    idc_bus.reserve(nsize);
    for(unsigned int i=0;i!=nsize;i++)
    {
        idc_bus[i]=0;
    }

    int k_start=0;
    for(int col=0;col!=nsize;col++)
    {
        unsigned int bus_number = inphno.get_physical_bus_number_of_internal_bus_number(col);
        DC_BUS* dcbus_ptr = get_dc_bus_with_bus_number(bus_number);
        DC_BUS& dcbus = *dcbus_ptr;
        double voltage = dcbus.get_dc_bus_voltage_in_kV();

        //osstream << "The voltage of internal " <<col <<"is: "<<voltage;
        //toolkit.show_information_with_leading_time_stamp(osstream);

        int k_end = dc_network_matrix.get_starting_index_of_column(col+1);
        for(int k=k_start;k<k_end;k++)
        {
            double yij = dc_network_matrix.get_real_entry_value(k);
            int row = dc_network_matrix.get_row_number_of_entry_index(k);
            idc_bus[row]+=yij*voltage;
            //osstream << "voltage of bus "<< col << " is: "<< voltage<<" yij of "<<col<<k<<" is: "<<yij<<" bus current "<<row<<" += "<<yij*voltage;
            //toolkit.show_information_with_leading_time_stamp(osstream);

        }
        k_start = k_end;
    }

    for(unsigned int i=0;i!=nsize;i++)
    {
        unsigned int bus_number = inphno.get_physical_bus_number_of_internal_bus_number(i);
        DC_BUS* bus_ptr = get_dc_bus_with_bus_number(bus_number);
        DC_BUS& bus = *bus_ptr;
        bus.set_dc_bus_current_in_kA(idc_bus[i]);
        //osstream << "current of bus "<< bus_number << " is: "<< idc_bus[i];
        //toolkit.show_information_with_leading_time_stamp(osstream);
    }

    vector<DC_LINE*> lines = get_all_dc_lines();
    unsigned int n_line = lines.size();
    for(unsigned int i=0;i!=n_line;i++)
    {
        DC_LINE& line = *(lines[i]);
        unsigned int ibus_number = line.get_sending_side_bus_number();
        unsigned int jbus_number = line.get_receiving_side_bus_number();
        DC_BUS* ibus_ptr = get_dc_bus_with_bus_number(ibus_number);
        DC_BUS* jbus_ptr = get_dc_bus_with_bus_number(jbus_number);
        DC_BUS& ibus = *ibus_ptr;
        DC_BUS& jbus = *jbus_ptr;
        double line_r = line.get_line_R_in_ohm();

        double Ui = ibus.get_dc_bus_voltage_in_kV();
        double Uj = jbus.get_dc_bus_voltage_in_kV();
        double Idc = (Ui-Uj)/line_r;
        double Pi = Ui*Idc;
        double Pj = Uj*Idc;

        line.set_Udc_ibus_in_kV(Ui);
        line.set_Udc_jbus_in_kV(Uj);
        line.set_dc_line_current(Idc);
        line.set_sending_side_power(Pi);
        line.set_receiving_side_power(Pj);
    }
}

void MT_HVDC::update_bus_power()
{
    //STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    vector<DC_BUS*> buses = get_all_dc_buses();
    unsigned int nbus = get_dc_bus_count();
    for(unsigned int i=0;i!=nbus;i++)
    {
        DC_BUS& dcbus = *(buses[i]);
        //unsigned int bus_number = dcbus.get_dc_bus_number();
        double vdc = dcbus.get_dc_bus_voltage_in_kV();
        double idc = dcbus.get_dc_bus_current_in_kA();
        double pc = vdc*idc;
        //osstream << "lcc power of bus "<< bus_number << " is: "<< pc;
        //toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

 void MT_HVDC::show_converter_power()
 {
     STEPS& toolkit = get_toolkit();
     ostringstream osstream;
     vector<HLCC*> lccs = get_all_lcc_converters();
     unsigned int n_lcc = lccs.size();
     for(unsigned int i=0;i!=n_lcc;i++)
     {
         HLCC& lcc = *(lccs[i]);
         double p_lcc = lcc.get_converter_P_2_ac_bus();
         double q_lcc = lcc.get_converter_Q_2_ac_bus();
         osstream<<"POWER TO AC BUS"<<lcc.get_converter_ac_bus()<<" of LCC CONVERTER: "<<lcc.get_converter_name()<<" is: "<<p_lcc<<" +j "<<q_lcc;
         toolkit.show_information_with_leading_time_stamp(osstream);
     }

     vector<HVSC*> vscs = get_all_vsc_converters();
     unsigned int n_vsc = vscs.size();
     for(unsigned int i=0;i!=n_vsc;i++)
     {
         HVSC& vsc = *(vscs[i]);
         double p_vsc = vsc.get_converter_P_to_AC_bus_MW();
         double q_vsc = vsc.get_converter_Q_to_AC_bus_Mvar();
         osstream<<"POWER TO AC BUS"<<vsc.get_converter_ac_bus()<<" of VSC CONVERTER: "<<vsc.get_converter_name()<<" is: "<<p_vsc<<" +j "<<q_vsc;
         toolkit.show_information_with_leading_time_stamp(osstream);
     }
 }


void MT_HVDC::initialize_vsc_station_udc_block(unsigned int station_number)
{
    //STEPS& toolkit = get_toolkit();
    //double ueq = 0.0;

    DC_STATION* station_ptr = get_dc_station_with_station_number(station_number);
    DC_STATION& station = *station_ptr;
    DC_STATION_STRUCTURE structure = station.get_station_structure();
    if(structure == VSC_PARALLEL)
    {
        double ceq = 0.0;
        vector<HVSC*> vscs = get_vsc_converters_connect_to_station(station_number);
        unsigned int n_vsc = vscs.size();
        for(unsigned int i=0;i!=n_vsc;i++)
        {
            HVSC& vsc = *(vscs[i]);
            HVSC_MODEL* model_ptr = vsc.get_hvsc_model();
            HVSC_MODEL& model = *model_ptr;
            ceq = ceq + model.get_dc_voltage_block_ceq_in_uF()*1e-6;
        }

        station.set_udc_block_ceq_in_F(ceq);
        HVSC& vsc_first = *(vscs[0]);
        station.set_udc_block_initial_value(vsc_first.get_udc_in_kV());
    }
}
void MT_HVDC::update_vsc_station_dynamic_ueq_input(unsigned int station_number)
{
    //STEPS& toolkit = get_toolkit();
    DC_STATION* station_ptr = get_dc_station_with_station_number(station_number);
    DC_STATION& station = *station_ptr;
    DC_STATION_STRUCTURE structure = station.get_station_structure();
    if(structure == VSC_PARALLEL)
    {

        double idc_to_network = 0.0;
        double idc_to_ceq = 0.0;
        vector<HVSC*> vscs = get_vsc_converters_connect_to_station(station_number);
        unsigned int n_vsc = vscs.size();
        for(unsigned int i=0;i!=n_vsc;i++)
        {
            HVSC& vsc = *(vscs[i]);
            idc_to_network = idc_to_network + vsc.get_idc_in_kA();
            HVSC_MODEL* model_ptr = vsc.get_hvsc_model();
            HVSC_MODEL& model = *model_ptr;
            idc_to_ceq = idc_to_ceq + model.get_converter_dynamic_current_from_converter_to_ceq_in_kA();
        }
        station.set_udc_block_input(idc_to_ceq-idc_to_network);

        //ostringstream osstream;
        //osstream<<"DC STATION NUMBER: "<<station_number<<" UPDATE WITH idc to ceq: "<<idc_to_ceq<<" and idc_to_network"<<idc_to_network;
        //toolkit.show_information_with_leading_time_stamp(osstream);
    }

}


double MT_HVDC::get_dc_bus_ueq_in_dynamic_simulation(unsigned int dc_bus)
{
    double ueq = 0.0;
    DC_TERMINAL* terminal_ptr = get_dc_terminal_with_dc_bus_number(dc_bus);
    DC_TERMINAL& terminal = *terminal_ptr;
    double terminal_number = terminal.get_terminal_number();
    vector<DC_STATION*> stations = get_dc_stations_connect_to_terminal(terminal_number);
    unsigned int n_station = stations.size();
    for(unsigned int i=0;i!=n_station;i++)
    {
        DC_STATION& station = *(stations[i]);
        DC_STATION_STRUCTURE structure = station.get_station_structure();
        if(structure==LCC_CASCADE)
        {
            vector<HLCC*> lccs = get_lcc_converters_connect_to_station(station.get_station_number());
            unsigned int n_lcc = lccs.size();
            for(unsigned int j=0;j!=n_lcc;j++)
            {
                HLCC& lcc = *(lccs[i]);
                ueq = ueq + lcc.get_lcc_converter_ueq_in_kV();
            }
        }
        if(structure == VSC_PARALLEL)
        {
            double u_station = station.get_vsc_station_dynamic_udc_in_kV();
            ueq = ueq + u_station;
        }
    }
    return ueq;
}

double MT_HVDC::get_dc_bus_req_in_dynamic_simulation(unsigned int dc_bus)
{
    double req = 0.0;
    DC_TERMINAL* terminal_ptr = get_dc_terminal_with_dc_bus_number(dc_bus);
    DC_TERMINAL& terminal = *terminal_ptr;
    double terminal_number = terminal.get_terminal_number();
    vector<DC_STATION*> stations = get_dc_stations_connect_to_terminal(terminal_number);
    unsigned int n_station = stations.size();
    for(unsigned int i=0;i!=n_station;i++)
    {
        DC_STATION& station = *(stations[i]);
        DC_STATION_STRUCTURE structure = station.get_station_structure();
        if(structure==LCC_CASCADE)
        {
            vector<HLCC*> lccs = get_lcc_converters_connect_to_station(station.get_station_number());
            unsigned int n_lcc = lccs.size();
            for(unsigned int j=0;j!=n_lcc;j++)
            {
                HLCC& lcc = *(lccs[i]);
                req = req + lcc.get_lcc_converter_req_in_ohm();
            }
        }
        if(structure == VSC_PARALLEL)
        {
            ;
        }
    }
    return req;
}

vector<HLCC_MODEL*> MT_HVDC::get_all_hlcc_models()
{
    vector<HLCC_MODEL*> lcc_models;
    lcc_models.clear();
    unsigned int n_lcc = get_lcc_converter_count();
    for(unsigned int i=0;i!=n_lcc;i++)
    {
        HLCC_MODEL* model = Hlcc[i].get_hlcc_model();
        lcc_models.push_back(model);
    }
    return lcc_models;
}

vector<HVSC_MODEL*> MT_HVDC::get_all_hvsc_models()
{
    vector<HVSC_MODEL*> vsc_models;
    vsc_models.clear();
    unsigned int n_vsc = get_vsc_converter_count();
    for(unsigned int i=0;i!=n_vsc;i++)
    {
        HVSC_MODEL* model = Hvsc[i].get_hvsc_model();
        vsc_models.push_back(model);
    }
    return vsc_models;
}

HLCC_MODEL* MT_HVDC::get_lcc_model_with_index(unsigned int index)
{
    if(not lcc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return Hlcc[index].get_hlcc_model();
    else
        return NULL;
}

HLCC_MODEL* MT_HVDC::get_lcc_model_with_name(string name)
{
    HLCC* lcc_ptr = get_lcc_converter_with_name(name);
    if(lcc_ptr!=NULL)
    {
        HLCC& lcc = *lcc_ptr;
        return lcc.get_hlcc_model();
    }
    else
        return NULL;
}

HVSC_MODEL* MT_HVDC::get_vsc_model_with_index(unsigned int index)
{
    if(not vsc_index_is_out_of_range_in_function(index,__FUNCTION__))
        return Hvsc[index].get_hvsc_model();
    else
        return NULL;
}

HVSC_MODEL* MT_HVDC::get_vsc_model_with_index(string name)
{
    HVSC* vsc_ptr = get_vsc_converter_with_name(name);
    if(vsc_ptr!=NULL)
    {
        HVSC& vsc = *vsc_ptr;
        return vsc.get_hvsc_model();
    }
    else
        return NULL;
}

void MT_HVDC::run(DYNAMIC_MODE mode)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    unsigned int n_lcc = get_lcc_converter_count();
    unsigned int n_vsc = get_vsc_converter_count();
    vector<DC_STATION*> dc_stations = get_all_dc_stations();
    unsigned int n_station = dc_stations.size();
    vector<HLCC_MODEL*> lcc_models = get_all_hlcc_models();
    vector<HVSC_MODEL*> vsc_models = get_all_hvsc_models();
    MT_DC_GRID_MODEL* mtdc_model_ptr = get_mtdc_network_model();
    if(get_status()==true)
    {
        switch(mode)
        {
            case DYNAMIC_INITIALIZE_MODE:
            {
                for(unsigned int i=0;i!=n_lcc;i++)
                {
                    if(lcc_models[i]!=NULL)
                    {
                        HLCC_MODEL& lcc_model = *(lcc_models[i]);
                        lcc_model.initialize();
                    }
                }
                for(unsigned int i=0;i!=n_vsc;i++)
                {
                    if(vsc_models[i]!=NULL)
                    {
                        HVSC_MODEL& vsc_model = *(vsc_models[i]);
                        vsc_model.initialize();
                    }
                }
                for(unsigned int i=0;i!=n_station;i++)
                {
                    DC_STATION& dc_station = *(dc_stations[i]);
                    if(dc_station.get_station_structure()==VSC_PARALLEL)
                    {
                        initialize_vsc_station_udc_block(dc_station.get_station_number());
                        dc_station.initialize();
                    }
                }
                if(mtdc_model_ptr!=NULL)
                {
                    MT_DC_GRID_MODEL& mtdc_model = *mtdc_model_ptr;
                    mtdc_model.initialize();
                }
                break;
            }

            case DYNAMIC_UPDATE_MODE:
                {
                    for(unsigned int i=0;i!=n_lcc;i++)
                    {
                        if(lcc_models[i]!=NULL)
                        {
                            HLCC_MODEL& lcc_model = *(lcc_models[i]);
                            lcc_model.run(mode);
                        }
                    }

                    for(unsigned int i=0;i!=n_vsc;i++)
                    {
                        if(vsc_models[i]!=NULL)
                        {
                            HVSC_MODEL& vsc_model = *(vsc_models[i]);
                            vsc_model.run(mode);
                        }
                    }

                    for(unsigned int i=0;i!=n_station;i++)
                    {
                        DC_STATION& dc_station = *(dc_stations[i]);
                        update_vsc_station_dynamic_ueq_input(dc_station.get_station_number());
                        dc_station.run(mode);
                    }
                    break;
                }

            default:
            {
                 for(unsigned int i=0;i!=n_lcc;i++)
                {
                    if(lcc_models[i]!=NULL)
                    {
                        HLCC_MODEL& lcc_model = *(lcc_models[i]);
                        lcc_model.run(mode);
                    }
                }

                for(unsigned int i=0;i!=n_vsc;i++)
                {
                    if(vsc_models[i]!=NULL)
                    {
                        HVSC_MODEL& vsc_model = *(vsc_models[i]);
                        vsc_model.run(mode);
                    }
                }

                for(unsigned int i=0;i!=n_station;i++)
                {
                    DC_STATION& dc_station = *(dc_stations[i]);
                    update_vsc_station_dynamic_ueq_input(dc_station.get_station_number());
                    dc_station.run(mode);
                }

                /*
                if(mtdc_model_ptr!=NULL)
                {
                    MT_DC_GRID_MODEL& mtdc_model = *mtdc_model_ptr;
                    mtdc_model.run(mode);
                }

                update_converters_in_dynamic_simulation();
                */
                break;
            }
        }
    }
}

void MT_HVDC::solve_dynamic_dc_systems()
{
    //STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    vector<HLCC_MODEL*> lcc_models = get_all_hlcc_models();
    unsigned int n_lcc = lcc_models.size();
    for(unsigned int i=0;i!=n_lcc;i++)
    {
        HLCC_MODEL& lcc_model = *(lcc_models[i]);
        lcc_model.update_converters();
    }

    MT_DC_GRID_MODEL* mtdc_model_ptr = get_mtdc_network_model();
    if(mtdc_model_ptr!=NULL)
    {
        MT_DC_GRID_MODEL& mtdc_model = *mtdc_model_ptr;
        mtdc_model.run(DYNAMIC_UPDATE_MODE);
    }
    update_converters_in_dynamic_simulation();
}

void MT_HVDC::update_converters_in_dynamic_simulation()
{
    ostringstream osstream;

    vector<DC_BUS*> dc_buses = get_all_dc_buses();
    unsigned int n_dcbus = get_dc_bus_count();
    for(unsigned int i=0;i!=n_dcbus;i++)
    {
        unsigned int bus_number = Dcbus[i].get_dc_bus_number();
        double Ubus = Dcbus[i].get_dc_bus_voltage_in_kV();
        double Ibus = Dcbus[i].get_dc_bus_current_in_kA();
        DC_TERMINAL* terminal_ptr = get_dc_terminal_with_dc_bus_number(bus_number);
        DC_TERMINAL& terminal = *terminal_ptr;
        vector<DC_STATION*> stations = get_dc_stations_connect_to_terminal(terminal.get_terminal_number());
        unsigned int n_station = stations.size();
        for(unsigned int j=0;j!=n_station;j++)
        {
            DC_STATION& station = *(stations[j]);
            DC_STATION_STRUCTURE structure = station.get_station_structure();
            if((structure==LCC_CASCADE) or (structure==LCC_HCM))
            {
                update_lcc_station_in_dynamic_simulation(station.get_station_number(),Ibus);
            }

            else
            {
                update_vsc_station_in_dynamic_simulation(station.get_station_number(),Ibus);
            }

        }
    }
}

void MT_HVDC::update_lcc_station_in_dynamic_simulation(unsigned int station_number,double Idc)
{
    ostringstream osstream;
    double Udc = 0.0;
    DC_STATION* station_ptr = get_dc_station_with_station_number(station_number);
    DC_STATION& station = *station_ptr;
    DC_STATION_STRUCTURE structure = station.get_station_structure();
    if(structure==LCC_CASCADE or structure==LCC_HCM)
    {
        vector<HLCC*> lccs = get_lcc_converters_connect_to_station(station_number);
        unsigned int n_lcc = lccs.size();
        for(unsigned int i=0;i!=n_lcc;i++)
        {
            HLCC& lcc = *(lccs[i]);
            lcc.set_idc_in_kA(Idc);
            double ueq = lcc.get_lcc_converter_ueq_in_kV();
            double req = lcc.get_lcc_converter_req_in_ohm();
            Udc = ueq-Idc*req;
            lcc.set_udc_in_kV(Udc);
            lcc.solve_lcc_with_tap_and_angle(Udc,Idc);

            //osstream<<"LCC: "<<lcc.get_converter_name()<<" UPADATE WITH UDC= "<<Udc<<"and Idc: "<<Idc<<" and angle = "<<lcc.et_firing_angle_in_deg();
            //show_information_with_leading_time_stamp_with_default_toolkit(osstream);
            //cout<< fixed << setprecision(6)<<Udc<<endl;
            //cout<< fixed << setprecision(6)<<Idc<<endl;

        }
    }
}

void MT_HVDC::update_vsc_station_in_dynamic_simulation(unsigned int station_number,double Idc)
{
    ostringstream osstream;

    DC_STATION* station_ptr = get_dc_station_with_station_number(station_number);
    DC_STATION& station = *station_ptr;
    DC_STATION_STRUCTURE structure = station.get_station_structure();
    if(structure == VSC_PARALLEL)
    {
        vector<HVSC*> vscs = get_vsc_converters_connect_to_station(station_number);
        unsigned int n_vsc = vscs.size();
        DC_TERMINAL* terminal_ptr = get_dc_terminal_with_terminal_number(station.get_terminal_number());
        DC_TERMINAL& terminal = *terminal_ptr;
        DC_TERMINAL_MODE tmode = terminal.get_terminal_mode();
        double Udc= station.get_vsc_station_dynamic_udc_in_kV();
        if(tmode==DC_P_TERMIANL)
        {
            double P_station = 0.0;
            for(unsigned int i=0;i!=n_vsc;i++)
            {
                HVSC& vsc = *(vscs[i]);
                P_station += vsc.get_converter_Pdc_command_in_MW();
            }
            for(unsigned int i=0;i!=n_vsc;i++)
            {
                HVSC& vsc = *(vscs[i]);
                vsc.set_udc_in_kV(Udc);
                double P_vsc = vsc.get_converter_Pdc_command_in_MW();
                double I_vsc = Idc*P_vsc/P_station;
                vsc.set_idc_in_kA(I_vsc);
                /*
                osstream<<"updata voltage of vsc: "<<vsc.get_converter_name()<<"  with  udc =  "<<Udc<<"  and Idc = "<<I_vsc;
                show_information_with_leading_time_stamp_with_default_toolkit(osstream);
                */
            }
        }
        if(tmode == DC_U_TERMINAL)
        {
            double ceq = 0.0;
            for(unsigned int i=0;i!=n_vsc;i++)
            {
                HVSC& vsc = *(vscs[i]);
                HVSC_MODEL* model_ptr = vsc.get_hvsc_model();
                HVSC_MODEL& model = *model_ptr;
                ceq = ceq + model.get_dc_voltage_block_ceq_in_uF()*1e-6;
            }

            for(unsigned int i=0;i!=n_vsc;i++)
            {
                HVSC& vsc = *(vscs[i]);
                HVSC_MODEL* model_ptr = vsc.get_hvsc_model();
                HVSC_MODEL& model = *model_ptr;
                double ceq_vsc = model.get_dc_voltage_block_ceq_in_uF()*1e-6;
                vsc.set_udc_in_kV(Udc);
                vsc.set_idc_in_kA(Idc*ceq_vsc/ceq);
            }

        }
    }
}


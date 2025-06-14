#include "header/model/pvu_models/pv_converter_model/pvgu1.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <cstdio>
#include <istream>
#include <iostream>
using namespace std;

PVGU1::PVGU1(STEPS& toolkit) : PV_CONVERTER_MODEL(toolkit),
                               csconv2(toolkit)
{
    clear();
}

PVGU1::~PVGU1()
{
}

void PVGU1::clear()
{
    set_model_float_parameter_count(9);

    set_current_source_flag(true);

    csconv2.clear();
    csconv2.set_KPLL(0.0);
    csconv2.set_KIPLL(0.0);
}

void PVGU1::copy_from_const_model(const PVGU1& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);
    csconv2.set_toolkit(toolkit);

    clear();
    set_converter_activer_current_command_T_in_s(model.get_converter_activer_current_command_T_in_s());
    set_LVPL_max_rate_of_active_current_change(model.get_LVPL_max_rate_of_active_current_change());
    set_LVPL_voltage_sensor_T_in_s(model.get_LVPL_voltage_sensor_T_in_s());

    set_converter_reactiver_voltage_command_T_in_s(model.get_converter_reactiver_voltage_command_T_in_s());
    set_HVRC_voltage_in_pu(model.get_HVRC_voltage_in_pu());
    set_HVRC_current_in_pu(model.get_HVRC_current_in_pu());

}

PVGU1::PVGU1(const PVGU1& model):PV_CONVERTER_MODEL(model.get_toolkit()),
                                csconv2(model.get_toolkit())
{
    copy_from_const_model(model);
}

PVGU1& PVGU1::operator=(const PVGU1& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

void PVGU1::set_converter_activer_current_command_T_in_s(double t)
{
    csconv2.set_converter_activer_current_command_T_in_s(t);
}

void PVGU1::set_converter_reactiver_voltage_command_T_in_s(double t)
{
    csconv2.set_converter_reactiver_voltage_command_T_in_s(t);
}

void PVGU1::set_HVRC_voltage_in_pu(double v)
{
    csconv2.set_HVRC_voltage_in_pu(v);
}

void PVGU1::set_HVRC_current_in_pu(double i)
{
    csconv2.set_HVRC_current_in_pu(i);
}

void PVGU1::set_LVPL_max_rate_of_active_current_change(double rate)
{
    csconv2.set_LVPL_max_rate_of_active_current_change(rate);
}

void PVGU1::set_LVPL_voltage_sensor_T_in_s(double t)
{
    csconv2.set_LVPL_voltage_sensor_T_in_s(t);
}

void PVGU1::set_LVPL_low_voltage_in_pu(double v)
{
    csconv2.set_LVPL_low_voltage_in_pu(v);
}

void PVGU1::set_LVPL_high_voltage_in_pu(double v)
{
    csconv2.set_LVPL_high_voltage_in_pu(v);
}

void PVGU1::set_LVPL_gain_at_high_voltage(double k)
{
    csconv2.set_LVPL_gain_at_high_voltage(k);
}

double PVGU1::get_converter_activer_current_command_T_in_s() const
{
    return csconv2.get_converter_activer_current_command_T_in_s();
}

double PVGU1::get_converter_reactiver_voltage_command_T_in_s() const
{
    return csconv2.get_converter_reactiver_voltage_command_T_in_s();
}

double PVGU1::get_HVRC_voltage_in_pu() const
{
    return csconv2.get_HVRC_voltage_in_pu();
}

double PVGU1::get_HVRC_current_in_pu() const
{
    return csconv2.get_HVRC_current_in_pu();
}

double PVGU1::get_LVPL_max_rate_of_active_current_change() const
{
    return csconv2.get_LVPL_max_rate_of_active_current_change();
}

double PVGU1::get_LVPL_voltage_sensor_T_in_s() const
{
    return csconv2.get_LVPL_voltage_sensor_T_in_s();
}

double PVGU1::get_LVPL_low_voltage_in_pu() const
{
    return csconv2.get_LVPL_low_voltage_in_pu();
}

double PVGU1::get_LVPL_high_voltage_in_pu() const
{
    return csconv2.get_LVPL_high_voltage_in_pu();
}

double PVGU1::get_LVPL_gain_at_high_voltage() const
{
    return csconv2.get_LVPL_gain_at_high_voltage();
}

string PVGU1::get_model_name() const
{
    return "PVGU1";
}

bool PVGU1::setup_model_with_steps_string_vector(vector<string>& data)
{
    ostringstream osstream;

    bool is_successful = false;

    if(data.size()>=12)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            unsigned int ibus;
            string id;
            double t_EQcmd, t_IPcmd, lvpl_v1, lvpl_v2, lvpl_g,
                   hvrc_v, hvrc_i, lvpl_rate, t_lvpl;

            ibus = (unsigned int)(get_integer_data(data[1],"0"));
            id = get_string_data(data[2],"");

            unsigned int i=3;
            t_EQcmd = get_double_data(data[i],"0.0"); i++;
            t_IPcmd = get_double_data(data[i],"0.0"); i++;
            lvpl_v1 = get_double_data(data[i],"0.0"); i++;
            lvpl_v2 = get_double_data(data[i],"0.0"); i++;
            lvpl_g = get_double_data(data[i],"0.0"); i++;
            hvrc_v = get_double_data(data[i],"0.0"); i++;
            hvrc_i = get_double_data(data[i],"0.0"); i++;
            lvpl_rate = get_double_data(data[i],"0.0"); i++;
            t_lvpl = get_double_data(data[i],"0.0");

            DEVICE_ID did = get_pv_unit_device_id(ibus, id);
            STEPS& toolkit = get_toolkit();
            POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
            PV_UNIT* pv = psdb.get_pv_unit(did);
            if(pv!=NULL)
            {
                set_converter_activer_current_command_T_in_s(t_IPcmd);
                set_LVPL_max_rate_of_active_current_change(lvpl_rate);
                set_LVPL_voltage_sensor_T_in_s(t_lvpl);
                set_LVPL_low_voltage_in_pu(lvpl_v1);
                set_LVPL_high_voltage_in_pu(lvpl_v2);
                set_LVPL_gain_at_high_voltage(lvpl_g);

                set_converter_reactiver_voltage_command_T_in_s(t_EQcmd);
                set_HVRC_voltage_in_pu(hvrc_v);
                set_HVRC_current_in_pu(hvrc_i);

                is_successful = true;

                return is_successful;
            }
            else
            {
                osstream<<"Error when loading data to build "<<get_model_name()<<" model for "<<did.get_compound_device_name()<<endl
                       <<"No such pv unit exists in the power system database.";
                toolkit.show_information_with_leading_time_stamp(osstream);
                return is_successful;
            }
        }
        else
        {
            return is_successful;
        }
    }
    else
    {
        return is_successful;
    }
}

bool PVGU1::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool PVGU1::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void PVGU1::setup_block_toolkit_and_parameters()
{
    csconv2.set_bus_pointer(get_bus_pointer());
    csconv2.set_device_pointer(get_device_pointer());
}

void PVGU1::initialize()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    if(not is_model_initialized())
    {

        PV_UNIT* pv_unit = get_pv_unit_pointer();
        setup_block_toolkit_and_parameters();
        double mbase = get_mbase_in_MVA();
        double P = pv_unit->get_p_generation_in_MW();
        double Q = pv_unit->get_q_generation_in_MVar();
        P /= mbase;
        Q /= mbase;
        complex<double> Zsource = get_source_impedance_in_pu_based_on_mbase();
        csconv2.set_initial_P_generation_in_pu(P);
        csconv2.set_initial_Q_generation_in_pu(Q);
        csconv2.set_source_impedance_in_pu_based_on_mbase(Zsource);
        csconv2.initialize();

        set_flag_model_initialized_as_true();

        if(toolkit.is_detailed_log_enabled())
        {
            osstream<<get_model_name()<<" model of "<<get_compound_device_name()<<" is initialized."<<endl
                    <<"(1) Initial active current command = "<<get_initial_active_current_command_in_pu_based_on_mbase()<<endl
                    <<"(2) Initial reactive current command = "<<get_initial_reactive_current_command_in_pu_based_on_mbase()<<endl
                    <<"(3) Initial reactive voltage command = "<<get_initial_reactive_voltage_command_in_pu()<<endl
                    <<"(4) States of blocks"<<endl
                    <<"    active_current_commander block state: "<<csconv2.get_active_current_commander_state()<<endl
                    <<"    reactive_voltage_commander block state: "<<csconv2.get_reactive_voltage_commander_state()<<endl
                    <<"    PLL_frequency_integrator block state: "<<csconv2.get_PLL_frequency_integrator_state()<<endl
                    <<"    PLL_angle_integrator block state: "<<csconv2.get_PLL_angle_integrator_state()<<endl
                    <<"    LVPL_voltage_sensor block state: "<<csconv2.get_LVPL_voltage_sensor_state()<<endl
                    <<"(5) active power generation :"<<get_terminal_active_power_in_MW()<<"MW"<<endl
                    <<"(6) reactive power generation :"<<get_terminal_reactive_power_in_MVar()<<"MVar"<<endl
                    <<"(7) terminal current :"<<get_terminal_current_in_pu_based_on_mbase()<<"pu";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void PVGU1::run(DYNAMIC_MODE mode)
{
    double Ipcmd = get_active_current_command_in_pu_based_on_mbase();
    double Eqcmd = get_reactive_voltage_command_in_pu();
    csconv2.set_Ipcmd_in_pu_based_on_mbase(Ipcmd);
    csconv2.set_Eqcmd_in_pu(Eqcmd);
    csconv2.run(mode);
    if(mode==DYNAMIC_UPDATE_MODE)
        set_flag_model_updated_as_true();
}

complex<double> PVGU1::get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase()
{
    return csconv2.get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase();
}

double PVGU1::get_active_power_generation_including_stator_loss_in_pu_based_on_mbase()
{
    return csconv2.get_active_power_generation_including_stator_loss_in_pu_based_on_mbase();
}

complex<double> PVGU1::get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_mbase()
{
    return csconv2.get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_mbase();
}

complex<double> PVGU1::get_internal_voltage_in_pu_in_xy_axis()
{
    return csconv2.get_internal_voltage_in_pu_in_xy_axis();
}

double PVGU1::get_initial_active_current_command_in_pu_based_on_mbase() const
{
    return csconv2.get_initial_active_current_command_in_pu_based_on_mbase();
}

double PVGU1::get_initial_reactive_current_command_in_pu_based_on_mbase() const
{
    return csconv2.get_initial_reactive_current_command_in_pu_based_on_mbase();
}

double PVGU1::get_initial_reactive_voltage_command_in_pu() const
{
    return csconv2.get_initial_reactive_voltage_command_in_pu();
}

double PVGU1::get_initial_active_power_command_in_pu_based_on_mbase() const
{
    return csconv2.get_initial_active_power_command_in_pu_based_on_mbase();
}

double PVGU1::get_initial_reactive_power_command_in_pu_based_on_mbase() const
{
    return csconv2.get_initial_reactive_power_command_in_pu_based_on_mbase();
}

void PVGU1::check()
{
    ;
}

void PVGU1::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string(false);
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void PVGU1::save()
{
    ;
}

string PVGU1::get_standard_psse_string(bool export_internal_bus_number) const
{
    ostringstream osstream;
    DEVICE_ID did = get_device_id();
    unsigned int bus = did.get_device_terminal().get_buses()[0];
    string identifier = "'"+did.get_device_identifier()+"'";

    string model_name = "'"+get_model_name()+"'";

    STEPS& toolkit = get_toolkit();
    NETWORK_MATRIX& network = toolkit.get_network_matrix();
    if(export_internal_bus_number==true)
        bus = network.get_internal_bus_number_of_physical_bus(bus)+1;

    osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(6)<<identifier<<", "
            <<setw(8)<<setprecision(6)<<get_converter_reactiver_voltage_command_T_in_s()<<", "
            <<setw(8)<<setprecision(6)<<get_converter_activer_current_command_T_in_s()<<", "
            <<setw(8)<<setprecision(6)<<get_LVPL_low_voltage_in_pu()<<", "
            <<setw(8)<<setprecision(6)<<get_LVPL_high_voltage_in_pu()<<", "
            <<setw(8)<<setprecision(6)<<get_LVPL_gain_at_high_voltage()<<", "
            <<setw(8)<<setprecision(6)<<get_HVRC_voltage_in_pu()<<", "
            <<setw(8)<<setprecision(6)<<get_HVRC_current_in_pu()<<"\n "
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<get_LVPL_max_rate_of_active_current_change()<<", "
            <<setw(8)<<setprecision(6)<<get_LVPL_voltage_sensor_T_in_s()<<" /";

    return osstream.str();
}

void PVGU1::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("TV", i); i++;
    add_model_data_name_and_index_pair("TI", i); i++;
    add_model_data_name_and_index_pair("VL LVPL", i); i++;
    add_model_data_name_and_index_pair("VH LVPL", i); i++;
    add_model_data_name_and_index_pair("G LVPL", i); i++;
    add_model_data_name_and_index_pair("V HVRC", i); i++;
    add_model_data_name_and_index_pair("C HVRC", i); i++;
    add_model_data_name_and_index_pair("CRATE LVPL", i); i++;
    add_model_data_name_and_index_pair("TV LVPL", i); i++;
}

double PVGU1::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="TV")
        return get_converter_reactiver_voltage_command_T_in_s();
    if(par_name=="TI")
        return get_converter_activer_current_command_T_in_s();
    if(par_name=="VL LVPL")
        return get_LVPL_low_voltage_in_pu();
    if(par_name=="VH LVPL")
        return get_LVPL_high_voltage_in_pu();
    if(par_name=="G LVPL")
        return get_LVPL_gain_at_high_voltage();
    if(par_name=="V HVRC")
        return get_HVRC_voltage_in_pu();
    if(par_name=="C HVRC")
        return get_HVRC_current_in_pu();
    if(par_name=="CRATE LVPL")
        return get_LVPL_max_rate_of_active_current_change();
    if(par_name=="TV LVPL")
        return get_LVPL_voltage_sensor_T_in_s();

    return 0.0;
}

void PVGU1::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="TV")
        return set_converter_reactiver_voltage_command_T_in_s(value);
    if(par_name=="TI")
        return set_converter_activer_current_command_T_in_s(value);
    if(par_name=="VL LVPL")
        return set_LVPL_low_voltage_in_pu(value);
    if(par_name=="VH LVPL")
        return set_LVPL_high_voltage_in_pu(value);
    if(par_name=="G LVPL")
        return set_LVPL_gain_at_high_voltage(value);
    if(par_name=="V HVRC")
        return set_HVRC_voltage_in_pu(value);
    if(par_name=="C HVRC")
        return set_HVRC_current_in_pu(value);
    if(par_name=="CRATE LVPL")
        return set_LVPL_max_rate_of_active_current_change(value);
    if(par_name=="TV LVPL")
        return set_LVPL_voltage_sensor_T_in_s(value);
    return;

    return;
}

double PVGU1::get_minimum_nonzero_time_constant_in_s()
{
    double mint = INFINITE_THRESHOLD;
    if(get_converter_activer_current_command_T_in_s()!=0.0 and mint>get_converter_activer_current_command_T_in_s())
        mint = get_converter_activer_current_command_T_in_s();
    if(get_converter_reactiver_voltage_command_T_in_s()!=0.0 and mint>get_converter_reactiver_voltage_command_T_in_s())
        mint = get_converter_reactiver_voltage_command_T_in_s();
    if(get_LVPL_voltage_sensor_T_in_s()!=0.0 and mint>get_LVPL_voltage_sensor_T_in_s())
        mint = get_LVPL_voltage_sensor_T_in_s();
    return mint;
}

void PVGU1::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_internal_variable_name_and_index_pair("PLL ANGLE IN DEG", i); i++;
    add_model_internal_variable_name_and_index_pair("PLL ANGLE IN RAD", i); i++;
    add_model_internal_variable_name_and_index_pair("ACTIVE CURRENT IN PU ON MBASE", i); i++;
    add_model_internal_variable_name_and_index_pair("REACTIVE CURRENT IN PU ON MBASE", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@ACTIVE CURRENT COMMAND BLOCK", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@REACTIVE VOLTAGE COMMAND BLOCK", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@LVPL VOLTAGE SENSOR", i); i++;
}

double PVGU1::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name == "PLL ANGLE IN DEG")
        return get_pll_angle_in_deg();
    if(var_name == "PLL ANGLE IN RAD")
        return get_pll_angle_in_rad();
    if(var_name == "ACTIVE CURRENT IN PU ON MBASE")
        return csconv2.get_Ip_in_pu();
    if(var_name == "REACTIVE CURRENT IN PU ON MBASE")
        return csconv2.get_Iq_in_pu();
    if(var_name == "STATE@ACTIVE CURRENT COMMAND BLOCK")
        return csconv2.get_active_current_commander_state();
    if(var_name == "STATE@REACTIVE VOLTAGE COMMAND BLOCK")
        return csconv2.get_reactive_voltage_commander_state();
    if(var_name == "STATE@LVPL VOLTAGE SENSOR")
        return csconv2.get_LVPL_voltage_sensor_state();

    return 0.0;
}

double PVGU1::get_pll_angle_in_rad()
{
    return csconv2.get_pll_angle_in_rad();
}

double PVGU1::get_pll_angle_in_deg()
{
    return csconv2.get_pll_angle_in_deg();
}

double PVGU1::get_pll_frequency_deviation_in_Hz()
{
    return 0.0;
}

double PVGU1::get_pll_frequency_deviation_in_pu()
{
    return 0.0;
}

double PVGU1::get_pll_frequency_in_pu()
{
    return 0.0;
}

double PVGU1::get_pll_frequency_in_Hz()
{
    return 0.0;
}

string PVGU1::get_dynamic_data_in_psse_format() const
{
    return "";
}

string PVGU1::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string PVGU1::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}

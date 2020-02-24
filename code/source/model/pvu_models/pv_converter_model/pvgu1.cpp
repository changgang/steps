#include "header/model/pvu_models/pv_converter_model/pvgu1.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <cstdio>
#include <istream>
#include <iostream>
using namespace std;

PVGU1::PVGU1(STEPS& toolkit) : PV_CONVERTER_MODEL(toolkit),
                               active_current_commander(toolkit),
                               LVPL_voltage_sensor(toolkit),
                               reactive_voltage_commander(toolkit)
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

    active_current_commander.set_limiter_type(NO_LIMITER);

    reactive_voltage_commander.set_limiter_type(NO_LIMITER);
    reactive_voltage_commander.set_K(1.0);

    LVPL_voltage_sensor.set_limiter_type(NO_LIMITER);
    LVPL_voltage_sensor.set_K(1.0);
}

void PVGU1::copy_from_const_model(const PVGU1& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);
    active_current_commander.set_toolkit(toolkit);
    LVPL_voltage_sensor.set_toolkit(toolkit);
    reactive_voltage_commander.set_toolkit(toolkit);

    clear();
    set_converter_activer_current_command_T_in_s(model.get_converter_activer_current_command_T_in_s());
    set_LVPL_max_rate_of_active_current_change(model.get_LVPL_max_rate_of_active_current_change());
    set_LVPL_voltage_sensor_T_in_s(model.get_LVPL_voltage_sensor_T_in_s());
    set_LVPL(model.get_LVPL());

    set_converter_reactiver_voltage_command_T_in_s(model.get_converter_reactiver_voltage_command_T_in_s());
    set_HVRC_voltage_in_pu(model.get_HVRC_voltage_in_pu());
    set_HVRC_current_in_pu(model.get_HVRC_current_in_pu());
}

PVGU1::PVGU1(const PVGU1& model):PV_CONVERTER_MODEL(model.get_toolkit()),
                                 active_current_commander(model.get_toolkit()),
                                 LVPL_voltage_sensor(model.get_toolkit()),
                                 reactive_voltage_commander(model.get_toolkit())
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
    active_current_commander.set_T_in_s(t);
}

void PVGU1::set_converter_reactiver_voltage_command_T_in_s(double t)
{
    reactive_voltage_commander.set_T_in_s(t);
}

void PVGU1::set_LVPL(const LVPL& lvpl)
{
    this->lvpl = lvpl;
}

void PVGU1::set_HVRC_voltage_in_pu(double v)
{
    HVRCR_voltage = v;
}

void PVGU1::set_HVRC_current_in_pu(double i)
{
    HVRCR_current = i;
}

void PVGU1::set_LVPL_max_rate_of_active_current_change(double rate)
{
    LVPL_active_power_change_rate = rate;
}

void PVGU1::set_LVPL_voltage_sensor_T_in_s(double t)
{
    LVPL_voltage_sensor.set_T_in_s(t);
}

double PVGU1::get_converter_activer_current_command_T_in_s() const
{
    return active_current_commander.get_T_in_s();
}

double PVGU1::get_converter_reactiver_voltage_command_T_in_s() const
{
    return reactive_voltage_commander.get_T_in_s();
}

LVPL PVGU1::get_LVPL() const
{
    return lvpl;
}

double PVGU1::get_HVRC_voltage_in_pu() const
{
    return HVRCR_voltage;
}

double PVGU1::get_HVRC_current_in_pu() const
{
    return HVRCR_current;
}

double PVGU1::get_LVPL_max_rate_of_active_current_change() const
{
    return LVPL_active_power_change_rate;
}

double PVGU1::get_LVPL_voltage_sensor_T_in_s() const
{
    return LVPL_voltage_sensor.get_T_in_s();
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
                LVPL lvpl;
                lvpl.set_low_voltage_in_pu(lvpl_v1);
                lvpl.set_high_voltage_in_pu(lvpl_v2);
                lvpl.set_gain_at_high_voltage(lvpl_g);
                set_LVPL(lvpl);

                set_converter_reactiver_voltage_command_T_in_s(t_EQcmd);
                set_HVRC_voltage_in_pu(hvrc_v);
                set_HVRC_current_in_pu(hvrc_i);

                is_successful = true;

                return is_successful;
            }
            else
            {
                osstream<<"Error when loading data to build "<<get_model_name()<<" model for "<<did.get_device_name()<<endl
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
}

void PVGU1::initialize()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    if(not is_model_initialized())
    {
        PV_UNIT* pv_unit = get_pv_unit_pointer();
        setup_block_toolkit_and_parameters();

        double one_over_mbase = get_one_over_mbase_in_one_over_MVA();

        complex<double> Zsource = get_source_impedance_in_pu_based_on_mbase();
        double xeq = Zsource.imag();

        double P = pv_unit->get_p_generation_in_MW();
        double Q = pv_unit->get_q_generation_in_MVar();
        complex<double> S(P*one_over_mbase,Q*one_over_mbase);


        complex<double> Vxy = get_terminal_complex_voltage_in_pu();
        double V = steps_fast_complex_abs(Vxy);
        double angle_in_rad = atan2(Vxy.imag(), Vxy.real());
        // ignore voltage angle
        complex<double> Ixy = conj(S/Vxy);
        double Ix = Ixy.real();
        double Iy = Ixy.imag();

        double IP = Ix*steps_cos(angle_in_rad) + Iy*steps_sin(angle_in_rad);
        double IQ =-Ix*steps_sin(angle_in_rad) + Iy*steps_cos(angle_in_rad);

        double EQ = IQ*(-xeq);

        active_current_commander.set_output(IP);
        active_current_commander.initialize();
        set_initial_active_current_command_in_pu_based_on_mbase(IP);

        reactive_voltage_commander.set_output(EQ);
        reactive_voltage_commander.initialize();

        set_initial_reactive_voltage_command_in_pu(EQ);
        set_initial_reactive_current_command_in_pu_based_on_mbase(IQ);

        LVPL_voltage_sensor.set_output(V);
        LVPL_voltage_sensor.initialize();

        set_flag_model_initialized_as_true();

        if(toolkit.is_detailed_log_enabled())
        {
            osstream<<get_model_name()<<" model of "<<get_device_name()<<" is initialized."<<endl
                    <<"(1) Initial active current command = "<<get_initial_active_current_command_in_pu_based_on_mbase()<<endl
                    <<"(2) Initial reactive current command = "<<get_initial_reactive_current_command_in_pu_based_on_mbase()<<endl
                    <<"(3) Initial reactive voltage command = "<<get_initial_reactive_voltage_command_in_pu()<<endl
                    <<"(4) States of blocks"<<endl
                    <<"    active_current_commander block state: "<<active_current_commander.get_state()<<endl
                    <<"    reactive_voltage_commander block state: "<<reactive_voltage_commander.get_state()<<endl
                    <<"    LVPL_voltage_sensor block state: "<<LVPL_voltage_sensor.get_state()<<endl
                    <<"(5) active power generation :"<<get_terminal_active_power_in_MW()<<"MW"<<endl
                    <<"(6) reactive power generation :"<<get_terminal_reactive_power_in_MVar()<<"MVar"<<endl
                    <<"(7) terminal current :"<<get_terminal_current_in_pu_based_on_mbase()<<"pu";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void PVGU1::run(DYNAMIC_MODE mode)
{
    complex<double> Vxy = get_terminal_complex_voltage_in_pu();
    double V = steps_fast_complex_abs(Vxy);

    LVPL_voltage_sensor.set_input(V);
    LVPL_voltage_sensor.run(mode);

    double lvpl_order = lvpl.get_LVPL_order(LVPL_voltage_sensor.get_output());

    double IP = get_active_current_command_in_pu_based_on_mbase();

    double input = active_current_commander.get_output();
    if(input>lvpl_order)
        input = lvpl_order;

    input = IP - input;
    double lvpl_rate_max = get_LVPL_max_rate_of_active_current_change();
    if(input>lvpl_rate_max)
        input = lvpl_rate_max;

    active_current_commander.set_input(input);
    active_current_commander.run(mode);

    double EQ = get_reactive_voltage_command_in_pu();

    reactive_voltage_commander.set_input(EQ);
    reactive_voltage_commander.run(mode);

    if(mode==UPDATE_MODE)
        set_flag_model_updated_as_true();
}

complex<double> PVGU1::get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_sbase()
{
    STEPS& toolkit = get_toolkit();
    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
    double mbase = get_mbase_in_MVA();

    complex<double> Vxy = get_terminal_complex_voltage_in_pu();
    double V = steps_fast_complex_abs(Vxy);

    complex<double> Zsource = get_source_impedance_in_pu_based_on_mbase();
    double Xeq = Zsource.imag();

    // low voltage active current logic
    double V_LVACR_high = V;
    double V_LVACR_low = 0.4;
    if(V<=V_LVACR_low)
        V_LVACR_low = V;
    if(V<=0.8)
        V_LVACR_high = 0.8;

    double Ip = active_current_commander.get_output();

    double lvpl_order = lvpl.get_LVPL_order(LVPL_voltage_sensor.get_output());
    if(Ip>lvpl_order)
        Ip =lvpl_order;
    Ip = Ip*(V-V_LVACR_low)/(V_LVACR_high-V_LVACR_low);

    // high voltage reactive current logic
    /*double v_hvrc = get_HVRC_voltage_in_pu();
    double overvoltage_correction_factor = 0.0;
    if(V>=v_hvrc)
        overvoltage_correction_factor=0.7*(V-v_hvrc);


    double Iq = -reactive_voltage_commander.get_output()/Xeq + overvoltage_correction_factor/V;*/
    double Iq = -reactive_voltage_commander.get_output()/Xeq;
    double hvrc_i = get_HVRC_current_in_pu();
    if(Iq<-hvrc_i)
        Iq = -hvrc_i;

    double pll_angle = get_pll_angle_in_rad();

    double Ix = Ip*steps_cos(pll_angle) - Iq*steps_sin(pll_angle);
    double Iy = Ip*steps_sin(pll_angle) + Iq*steps_cos(pll_angle);

    complex<double> Ixy(Ix, Iy);
    //cout<<"Norton Ixy based on mbase = "<<Ixy<<endl;

    return Ixy*(mbase*one_over_sbase);
}

complex<double> PVGU1::get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();
    double one_over_mbase = get_one_over_mbase_in_one_over_MVA();
    complex<double> Ixy = get_terminal_complex_current_in_pu_in_xy_axis_based_on_sbase();
    return Ixy*(sbase*one_over_mbase);
}

complex<double> PVGU1::get_terminal_complex_current_in_pu_in_xy_axis_based_on_sbase()
{
    complex<double> Ixy_norton = get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_sbase();
    complex<double> Ixy_term = Ixy_norton;
    return Ixy_term;
}

double PVGU1::get_terminal_current_in_pu_based_on_mbase()
{
    return steps_fast_complex_abs(get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase());
}

double PVGU1::get_terminal_current_in_pu_based_on_sbase()
{
    return steps_fast_complex_abs(get_terminal_complex_current_in_pu_in_xy_axis_based_on_sbase());
}


void PVGU1::check()
{
    ;
}

void PVGU1::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string();
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void PVGU1::save()
{
    ;
}

string PVGU1::get_standard_psse_string() const
{
    ostringstream osstream;
    DEVICE_ID did = get_device_id();
    unsigned int bus = did.get_device_terminal().get_buses()[0];
    string identifier = "'"+did.get_device_identifier()+"'";

    string model_name = "'"+get_model_name()+"'";

    LVPL lvpl = get_LVPL();

    osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(6)<<identifier<<", "
            <<setw(8)<<setprecision(6)<<get_converter_reactiver_voltage_command_T_in_s()<<", "
            <<setw(8)<<setprecision(6)<<get_converter_activer_current_command_T_in_s()<<", "
            <<setw(8)<<setprecision(6)<<lvpl.get_low_voltage_in_pu()<<", "
            <<setw(8)<<setprecision(6)<<lvpl.get_high_voltage_in_pu()<<", "
            <<setw(8)<<setprecision(6)<<lvpl.get_gain_at_hig_voltage()<<", "
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
    LVPL lvpl = get_LVPL();
    if(par_name=="TV")
        return get_converter_reactiver_voltage_command_T_in_s();
    if(par_name=="TI")
        return get_converter_activer_current_command_T_in_s();
    if(par_name=="VL LVPL")
        return lvpl.get_low_voltage_in_pu();
    if(par_name=="VH LVPL")
        return lvpl.get_high_voltage_in_pu();
    if(par_name=="G LVPL")
        return lvpl.get_gain_at_hig_voltage();
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
        return;
    if(par_name=="VH LVPL")
        return;
    if(par_name=="G LVPL")
        return;
    if(par_name=="V HVRC")
        return set_HVRC_voltage_in_pu(value);
    if(par_name=="C HVRC")
        return set_HVRC_current_in_pu(value);
    if(par_name=="CRATE LVPL")
        return set_LVPL_max_rate_of_active_current_change(value);
    if(par_name=="TV LVPL")
        return set_LVPL_voltage_sensor_T_in_s(value);
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
    add_model_inernal_variable_name_and_index_pair("PLL ANGLE IN DEG", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@ACTIVE CURRENT COMMAND BLOCK", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@REACTIVE VOLTAGE COMMAND BLOCK", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@LVPL VOLTAGE SENSOR", i); i++;
}

double PVGU1::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name == "PLL ANGLE IN DEG")
        return get_pll_angle_in_deg();
    if(var_name == "STATE@ACTIVE CURRENT COMMAND BLOCK")
        return active_current_commander.get_state();
    if(var_name == "STATE@REACTIVE VOLTAGE COMMAND BLOCK")
        return reactive_voltage_commander.get_state();
    if(var_name == "STATE@LVPL VOLTAGE SENSOR")
        return LVPL_voltage_sensor.get_state();

    return 0.0;
}

complex<double> PVGU1::get_terminal_complex_power_in_pu_based_on_mbase()
{
    complex<double> Vxy = get_terminal_complex_voltage_in_pu();
    complex<double> Ixy = get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase();

    complex<double> S = Vxy*conj(Ixy);
    return S;
}

complex<double> PVGU1::get_terminal_complex_power_in_MVA()
{
    return get_terminal_complex_power_in_pu_based_on_mbase()*get_mbase_in_MVA();
}

double PVGU1::get_terminal_active_power_in_pu_based_on_mbase()
{
    return get_terminal_complex_power_in_pu_based_on_mbase().real();
}

double PVGU1::get_terminal_active_power_in_MW()
{
    return get_terminal_complex_power_in_MVA().real();
}

double PVGU1::get_terminal_reactive_power_in_pu_based_on_mbase()
{
    return get_terminal_complex_power_in_pu_based_on_mbase().imag();
}

double PVGU1::get_terminal_reactive_power_in_MVar()
{
    return get_terminal_complex_power_in_MVA().imag();
}

double PVGU1::get_active_power_generation_including_stator_loss_in_pu_based_on_mbase()
{
    return get_active_power_generation_including_stator_loss_in_MW()/get_mbase_in_MVA();
}

double PVGU1::get_active_power_generation_including_stator_loss_in_MW()
{
    double pterm = get_terminal_active_power_in_MW();
    double rsource = get_source_impedance_in_pu_based_on_mbase().real();
    double iterm = steps_fast_complex_abs(get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase());
    double mbase = get_mbase_in_MVA();

    return pterm+rsource*iterm*iterm*mbase;
}

double PVGU1::get_pll_angle_in_rad()
{
    complex<double> Vxy = get_terminal_complex_voltage_in_pu();
    double angle = atan2(Vxy.imag(), Vxy.real());
    return angle;
}

double PVGU1::get_pll_angle_in_deg()
{
    return rad2deg(get_pll_angle_in_rad());
}

double PVGU1::get_pll_frequency_deviation_in_pu()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    PV_UNIT* pv = (PV_UNIT*) get_device_pointer();
    unsigned int bus = pv->get_unit_bus();
    return psdb.get_bus_frequency_deviation_in_pu(bus);
}

double PVGU1::get_pll_frequency_deviation_in_Hz()
{
    double fbase = get_bus_base_frequency_in_Hz();

    return fbase*get_pll_frequency_deviation_in_pu();
}

double PVGU1::get_pll_frequency_in_pu()
{
    return 1.0+get_pll_frequency_deviation_in_pu();
}

double PVGU1::get_pll_frequency_in_Hz()
{
    double fbase = get_bus_base_frequency_in_Hz();

    return fbase*get_pll_frequency_in_pu();
}

complex<double> PVGU1::get_internal_voltage_in_pu_in_xy_axis()
{
    complex<double> Ixy = get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_sbase();
    complex<double> Z = get_source_impedance_in_pu_based_on_mbase();

    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();
    double one_over_mbase = get_one_over_mbase_in_one_over_MVA();

    Z *= (one_over_mbase*sbase);

    return Ixy*Z;
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

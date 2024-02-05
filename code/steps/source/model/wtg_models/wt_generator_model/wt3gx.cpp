#include "header/model/wtg_models/wt_generator_model/wt3gx.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <cstdio>
#include <istream>
#include <iostream>
using namespace std;

WT3GX::WT3GX(STEPS& toolkit) : WT_GENERATOR_MODEL(toolkit),
                               active_current_commander(toolkit),
                               LVPL_voltage_sensor(toolkit),
                               reactive_voltage_commander(toolkit),
                               vsg(toolkit)
{
    clear();
}

WT3GX::~WT3GX()
{
}

void WT3GX::clear()
{
    set_model_float_parameter_count(14);

    set_current_source_flag(false);

    active_current_commander.set_limiter_type(NO_LIMITER);

    reactive_voltage_commander.set_limiter_type(NO_LIMITER);
    reactive_voltage_commander.set_K(1.0);

    vsg.clear();

    LVPL_voltage_sensor.set_limiter_type(NO_LIMITER);
    LVPL_voltage_sensor.set_K(1.0);
}

void WT3GX::copy_from_const_model(const WT3GX& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);
    active_current_commander.set_toolkit(toolkit);
    LVPL_voltage_sensor.set_toolkit(toolkit);
    reactive_voltage_commander.set_toolkit(toolkit);
    vsg.set_toolkit(toolkit);

    clear();
    set_converter_activer_current_command_T_in_s(model.get_converter_activer_current_command_T_in_s());
    set_LVPL_max_rate_of_active_current_change(model.get_LVPL_max_rate_of_active_current_change());
    set_LVPL_voltage_sensor_T_in_s(model.get_LVPL_voltage_sensor_T_in_s());
    set_LVPL(model.get_LVPL());

    set_converter_reactiver_voltage_command_T_in_s(model.get_converter_reactiver_voltage_command_T_in_s());
    set_HVRC_voltage_in_pu(model.get_HVRC_voltage_in_pu());
    set_HVRC_current_in_pu(model.get_HVRC_current_in_pu());

    set_Tj_in_s(model.get_Tj_in_s());
    set_D(model.get_D());
    set_Ku(model.get_Ku());
    set_Te_in_s(model.get_Te_in_s());
}

WT3GX::WT3GX(const WT3GX& model):WT_GENERATOR_MODEL(model.get_toolkit()),
                                 active_current_commander(model.get_toolkit()),
                                 LVPL_voltage_sensor(model.get_toolkit()),
                                 reactive_voltage_commander(model.get_toolkit()),
                                 vsg(model.get_toolkit())
{
    copy_from_const_model(model);
}

WT3GX& WT3GX::operator=(const WT3GX& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

void WT3GX::set_converter_activer_current_command_T_in_s(double t)
{
    active_current_commander.set_T_in_s(t);
}

void WT3GX::set_converter_reactiver_voltage_command_T_in_s(double t)
{
    reactive_voltage_commander.set_T_in_s(t);
}

void WT3GX::set_LVPL(const LVPL& lvpl)
{
    this->lvpl = lvpl;
}

void WT3GX::set_HVRC_voltage_in_pu(double v)
{
    HVRCR_voltage = v;
}

void WT3GX::set_HVRC_current_in_pu(double i)
{
    HVRCR_current = i;
}

void WT3GX::set_LVPL_max_rate_of_active_current_change(double rate)
{
    LVPL_active_power_change_rate = rate;
}

void WT3GX::set_LVPL_voltage_sensor_T_in_s(double t)
{
    LVPL_voltage_sensor.set_T_in_s(t);
}

void WT3GX::set_Tj_in_s(double T)
{
    vsg.set_Tj_in_s(T);
}

void WT3GX::set_D(double D)
{
    vsg.set_D(D);
}

void WT3GX::set_Ku(double Ku)
{
    vsg.set_Ku(Ku);
}

void WT3GX::set_Te_in_s(double T)
{
    vsg.set_Te_in_s(T);
}

double WT3GX::get_converter_activer_current_command_T_in_s() const
{
    return active_current_commander.get_T_in_s();
}

double WT3GX::get_converter_reactiver_voltage_command_T_in_s() const
{
    return reactive_voltage_commander.get_T_in_s();
}

LVPL WT3GX::get_LVPL() const
{
    return lvpl;
}

double WT3GX::get_HVRC_voltage_in_pu() const
{
    return HVRCR_voltage;
}

double WT3GX::get_HVRC_current_in_pu() const
{
    return HVRCR_current;
}

double WT3GX::get_LVPL_max_rate_of_active_current_change() const
{
    return LVPL_active_power_change_rate;
}

double WT3GX::get_LVPL_voltage_sensor_T_in_s() const
{
    return LVPL_voltage_sensor.get_T_in_s();
}

double WT3GX::get_Tj_in_s() const
{
    return vsg.get_Tj_in_s();
}

double WT3GX::get_D() const
{
    return vsg.get_D();
}

double WT3GX::get_Ku() const
{
    return vsg.get_Ku();
}

double WT3GX::get_Te_in_s() const
{
    return vsg.get_Te_in_s();
}

string WT3GX::get_model_name() const
{
    return "WT3GX";
}

bool WT3GX::setup_model_with_steps_string_vector(vector<string>& data)
{
    ostringstream osstream;

    bool is_successful = false;

    if(data.size()>=18)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            unsigned int ibus;
            string id;
            unsigned int n_lumped_turbine;
            double t_EQcmd, t_IPcmd, tj, d, ku, te, prate, lvpl_v1, lvpl_v2, lvpl_g,
                   hvrc_v, hvrc_i, lvpl_rate, t_lvpl;

            ibus = (unsigned int)(get_integer_data(data[1],"0"));
            id = get_string_data(data[2],"");

            unsigned int i=3;
            n_lumped_turbine = (unsigned int)(get_integer_data(data[i],"1")); i++;
            prate = get_double_data(data[i],"0.0"); i++;
            t_IPcmd = get_double_data(data[i],"0.0"); i++;
            lvpl_rate = get_double_data(data[i],"0.0"); i++;
            t_lvpl = get_double_data(data[i],"0.0"); i++;
            lvpl_v1 = get_double_data(data[i],"0.0"); i++;
            lvpl_v2 = get_double_data(data[i],"0.0"); i++;
            lvpl_g = get_double_data(data[i],"0.0"); i++;
            t_EQcmd = get_double_data(data[i],"0.0"); i++;
            hvrc_v = get_double_data(data[i],"0.0"); i++;
            hvrc_i = get_double_data(data[i],"0.0"); i++;
            tj = get_double_data(data[i],"0.0"); i++;
            d = get_double_data(data[i],"0.0"); i++;
            ku = get_double_data(data[i],"0.0"); i++;
            te = get_double_data(data[i],"0.0");

            DEVICE_ID did = get_wt_generator_device_id(ibus, id);
            STEPS& toolkit = get_toolkit();
            POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
            WT_GENERATOR* gen = psdb.get_wt_generator(did);
            if(gen!=NULL)
            {
                gen->set_number_of_lumped_wt_generators(n_lumped_turbine);
                gen->set_rated_power_per_wt_generator_in_MW(prate);
                gen->set_mbase_in_MVA(prate*n_lumped_turbine);

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
                set_Tj_in_s(tj);
                set_D(d);
                set_Ku(ku);
                set_Te_in_s(te);

                is_successful = true;

                return is_successful;
            }
            else
            {
                osstream<<"Error when loading data to build "<<get_model_name()<<" model for "<<did.get_compound_device_name()<<endl
                       <<"No such wt generator exists in the power system database.";
                toolkit.show_information_with_leading_time_stamp(osstream);
                return is_successful;
            }
        }
        else
            return is_successful;
    }
    else
        return is_successful;
}

bool WT3GX::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool WT3GX::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void WT3GX::setup_block_toolkit_and_parameters()
{
}

void WT3GX::initialize()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    if(not is_model_initialized())
    {
        WT_GENERATOR* wt_generator = get_wt_generator_pointer();
        setup_block_toolkit_and_parameters();

        unsigned int n_lumped = get_number_of_lumped_wt_generators();

        double mbase = get_mbase_in_MVA();
        mbase /= n_lumped;

        complex<double> Zsource = get_source_impedance_in_pu_based_on_mbase();
        double xeq = Zsource.imag();

        double P = wt_generator->get_p_generation_in_MW()/n_lumped;
        double Q = wt_generator->get_q_generation_in_MVar()/n_lumped;
        complex<double> S(P/mbase,Q/mbase);


        complex<double> Vxy = get_terminal_complex_voltage_in_pu();
        double V = get_terminal_voltage_in_pu();
        double angle_in_rad = get_terminal_voltage_angle_in_rad();
        // ignore voltage angle
        complex<double> Ixy = conj(S/Vxy);
        double Ix = Ixy.real();
        double Iy = Ixy.imag();

        complex<double> Z = wt_generator->get_source_impedance_in_pu();
        complex<double> E =  Vxy + Ixy*Z;

        double sine = steps_sin(angle_in_rad), cosin = steps_cos(angle_in_rad);
        double IP = Ix*cosin + Iy*sine;
        double IQ =-Ix*sine + Iy*cosin;

        double EQ = IQ*(-xeq);

        active_current_commander.set_output(IP);
        active_current_commander.initialize();
        set_initial_active_current_command_in_pu_based_on_mbase(IP);

        reactive_voltage_commander.set_output(EQ);
        reactive_voltage_commander.initialize();
        set_initial_reactive_voltage_command_in_pu(EQ);
        set_initial_reactive_current_command_in_pu_based_on_mbase(IQ);

        vsg.set_Pref_in_pu_based_on_mbase(P/mbase);
        vsg.set_Qref_in_pu_based_on_mbase(Q/mbase);
        vsg.set_Vref_in_pu(V);
        vsg.set_P_in_pu_based_on_mbase(P/mbase);
        vsg.set_Q_in_pu_based_on_mbase(Q/mbase);
        vsg.set_V_in_pu(V);
        vsg.set_initial_virtual_voltage_in_pu(steps_fast_complex_abs(E));
        vsg.set_initial_virtual_angle_in_rad(steps_fast_complex_arg(E));

        vsg.initialize();

        LVPL_voltage_sensor.set_output(V);
        LVPL_voltage_sensor.initialize();

        set_flag_model_initialized_as_true();

        if(toolkit.is_detailed_log_enabled())
        {
            osstream<<get_model_name()<<" model of "<<get_compound_device_name()<<" is initialized."<<endl
                    <<"(1) Initial active current command = "<<get_initial_active_current_command_in_pu_based_on_mbase()<<endl
                    <<"(2) Initial reactive current command = "<<get_initial_reactive_current_command_in_pu_based_on_mbase()<<endl
                    <<"(3) Initial reactive voltage command = "<<get_initial_reactive_voltage_command_in_pu()<<endl
                    <<"(4) States of blocks"<<endl
                    <<"    active_current_commander block state: "<<active_current_commander.get_state()<<endl
                    <<"    reactive_voltage_commander block state: "<<reactive_voltage_commander.get_state()<<endl
                    <<"    PLL_frequency_integrator block state: "<<0<<endl
                    <<"    PLL_angle_integrator block state: "<<0<<endl
                    <<"    LVPL_voltage_sensor block state: "<<LVPL_voltage_sensor.get_state()<<endl
                    <<"(5) active power generation :"<<get_terminal_active_power_in_MW()<<"MW"<<endl
                    <<"(6) reactive power generation :"<<get_terminal_reactive_power_in_MVar()<<"MVar"<<endl
                    <<"(7) terminal current :"<<get_terminal_current_in_pu_based_on_mbase()<<"pu";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void WT3GX::run(DYNAMIC_MODE mode)
{
    double V = get_terminal_voltage_in_pu();

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

    vsg.set_P_in_pu_based_on_mbase(get_terminal_active_power_in_pu_based_on_mbase());
    vsg.set_Q_in_pu_based_on_mbase(get_terminal_reactive_power_in_pu_based_on_mbase());
    vsg.set_V_in_pu(get_terminal_voltage_in_pu());
    vsg.run(mode);

    if(mode==UPDATE_MODE)
        set_flag_model_updated_as_true();
}

complex<double> WT3GX::get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_sbase()
{
    STEPS& toolkit = get_toolkit();
    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
    double mbase = get_mbase_in_MVA();

    double V = get_terminal_voltage_in_pu();

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
    //cout<<get_compound_device_name()<<" model "<<get_model_name()<<": Ip= "<<Ip<<", Iq="<<Iq<<", Pllangle = "<<pll_angle<<endl;

    double sine = steps_sin(pll_angle), cosine = steps_cos(pll_angle);
    double Ix = Ip*cosine - Iq*sine;
    double Iy = Ip*sine + Iq*cosine;

    complex<double> Ixy(Ix, Iy);
    //cout<<"Norton Ixy based on mbase = "<<Ixy<<endl;

    return Ixy*(mbase*one_over_sbase);
}

complex<double> WT3GX::get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();
    double one_over_mbase = get_one_over_mbase_in_one_over_MVA();
    complex<double> Ixy = get_terminal_complex_current_in_pu_in_xy_axis_based_on_sbase();
    return Ixy*(sbase*one_over_mbase);
}

complex<double> WT3GX::get_terminal_complex_current_in_pu_in_xy_axis_based_on_sbase()
{
    complex<double> Ixy_norton = get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_sbase();
    complex<double> Ixy_term = Ixy_norton;
    return Ixy_term;
}

double WT3GX::get_terminal_current_in_pu_based_on_mbase()
{
    return steps_fast_complex_abs(get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase());
}

double WT3GX::get_terminal_current_in_pu_based_on_sbase()
{
    return steps_fast_complex_abs(get_terminal_complex_current_in_pu_in_xy_axis_based_on_sbase());
}


void WT3GX::check()
{
    ;
}

void WT3GX::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string(false);
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void WT3GX::save()
{
    ;
}

string WT3GX::get_standard_psse_string(bool export_internal_bus_number) const
{
    ostringstream osstream;
    DEVICE_ID did = get_device_id();
    unsigned int bus = did.get_device_terminal().get_buses()[0];
    string identifier = "'"+did.get_device_identifier()+"'";

    string model_name = "'"+get_model_name()+"'";

    LVPL lvpl = get_LVPL();

    STEPS& toolkit = get_toolkit();
    NETWORK_MATRIX& network = toolkit.get_network_matrix();
    if(export_internal_bus_number==true)
        bus = network.get_internal_bus_number_of_physical_bus(bus)+1;

    osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(6)<<identifier<<", "
            <<setw(8)<<get_number_of_lumped_wt_generators()<<", "
            <<setw(8)<<setprecision(6)<<get_rated_power_per_wt_generator_in_MW()<<", "
            <<setw(8)<<setprecision(6)<<get_converter_activer_current_command_T_in_s()<<", "
            <<setw(8)<<setprecision(6)<<get_LVPL_max_rate_of_active_current_change()<<", "
            <<setw(8)<<setprecision(6)<<get_LVPL_voltage_sensor_T_in_s()<<", "
            <<setw(8)<<setprecision(6)<<lvpl.get_low_voltage_in_pu()<<", "
            <<setw(8)<<setprecision(6)<<lvpl.get_high_voltage_in_pu()<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<lvpl.get_gain_at_hig_voltage()<<", "
            <<setw(8)<<setprecision(6)<<get_converter_reactiver_voltage_command_T_in_s()<<", "
            <<setw(8)<<setprecision(6)<<get_HVRC_voltage_in_pu()<<", "
            <<setw(8)<<setprecision(6)<<get_HVRC_current_in_pu()<<", "
            <<setw(8)<<setprecision(6)<<get_Tj_in_s()<<", "
            <<setw(8)<<setprecision(6)<<get_D()<<", "
            <<setw(8)<<setprecision(6)<<get_Ku()<<", "
            <<setw(8)<<setprecision(6)<<get_Te_in_s()<<" /";

    return osstream.str();
}

void WT3GX::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    LVPL lvpl = get_LVPL();
    add_model_data_name_and_index_pair("N", i); i++;
    add_model_data_name_and_index_pair("PN", i); i++;
    add_model_data_name_and_index_pair("TI", i); i++;
    add_model_data_name_and_index_pair("CRATE LVPL", i); i++;
    add_model_data_name_and_index_pair("TV LVPL", i); i++;
    add_model_data_name_and_index_pair("VL LVPL", i); i++;
    add_model_data_name_and_index_pair("VH LVPL", i); i++;
    add_model_data_name_and_index_pair("G LVPL", i); i++;
    add_model_data_name_and_index_pair("TV", i); i++;
    add_model_data_name_and_index_pair("V HVRC", i); i++;
    add_model_data_name_and_index_pair("C HVRC", i); i++;
    add_model_data_name_and_index_pair("KPLL", i); i++;
    add_model_data_name_and_index_pair("KIPLL", i); i++;
    add_model_data_name_and_index_pair("PLLMAX", i); i++;
    add_model_data_name_and_index_pair("PLLMIN", i); i++;
}

double WT3GX::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    LVPL lvpl = get_LVPL();
    if(par_name=="N")
        return get_number_of_lumped_wt_generators();
    if(par_name=="PN")
        return get_rated_power_per_wt_generator_in_MW();
    if(par_name=="TI")
        return get_converter_activer_current_command_T_in_s();
    if(par_name=="CRATE LVPL")
        return get_LVPL_max_rate_of_active_current_change();
    if(par_name=="TV LVPL")
        return get_LVPL_voltage_sensor_T_in_s();
    if(par_name=="VL LVPL")
        return lvpl.get_low_voltage_in_pu();
    if(par_name=="VH LVPL")
        return lvpl.get_high_voltage_in_pu();
    if(par_name=="G LVPL")
        return lvpl.get_gain_at_hig_voltage();
    if(par_name=="TV")
        return get_converter_reactiver_voltage_command_T_in_s();
    if(par_name=="V HVRC")
        return get_HVRC_voltage_in_pu();
    if(par_name=="C HVRC")
        return get_HVRC_current_in_pu();
    if(par_name=="TJ")
        return get_Tj_in_s();
    if(par_name=="D")
        return get_D();
    if(par_name=="KU")
        return get_Ku();
    if(par_name=="TE")
        return get_Te_in_s();

    return 0.0;
}

void WT3GX::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="N")
    {
        WT_GENERATOR* gen = get_wt_generator_pointer();
        return gen->set_number_of_lumped_wt_generators((unsigned int)(value));
    }
    if(par_name=="PN")
    {
        WT_GENERATOR* gen = get_wt_generator_pointer();
        return gen->set_rated_power_per_wt_generator_in_MW(value);
    }
    if(par_name=="TI")
        return set_converter_activer_current_command_T_in_s(value);
    if(par_name=="CRATE LVPL")
        return set_LVPL_max_rate_of_active_current_change(value);
    if(par_name=="TV LVPL")
        return set_LVPL_voltage_sensor_T_in_s(value);
    if(par_name=="VL LVPL")
        return;
    if(par_name=="VH LVPL")
        return;
    if(par_name=="G LVPL")
        return;
    if(par_name=="TV")
        return set_converter_reactiver_voltage_command_T_in_s(value);
    if(par_name=="V HVRC")
        return set_HVRC_voltage_in_pu(value);
    if(par_name=="C HVRC")
        return set_HVRC_current_in_pu(value);
    if(par_name=="TJ")
        return set_Tj_in_s(value);
    if(par_name=="D")
        return set_D(value);
    if(par_name=="KU")
        return set_Ku(value);
    if(par_name=="TE")
        return set_Te_in_s(value);
    return;
}

double WT3GX::get_minimum_nonzero_time_constant_in_s()
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

void WT3GX::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_internal_variable_name_and_index_pair("PLL ANGLE IN DEG", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@ACTIVE CURRENT COMMAND BLOCK", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@REACTIVE VOLTAGE COMMAND BLOCK", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@PLL FREQUENCY BLOCK", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@PLL ANGLE BLOCK", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@LVPL VOLTAGE SENSOR", i); i++;
}

double WT3GX::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name == "PLL ANGLE IN DEG")
        return get_pll_angle_in_deg();
    if(var_name == "STATE@ACTIVE CURRENT COMMAND BLOCK")
        return active_current_commander.get_state();
    if(var_name == "STATE@REACTIVE VOLTAGE COMMAND BLOCK")
        return reactive_voltage_commander.get_state();
    if(var_name == "STATE@VIRTUAL FREQUENCY BLOCK")
        return vsg.get_virtual_frequency_deviation_block_state();
    if(var_name == "STATE@VIRTUAL ANGLE BLOCK")
        return vsg.get_virtual_angle_block_state();
    if(var_name == "STATE@VIRTUAL VOLTAGE BLOCK")
        return vsg.get_virtual_voltage_block_state();
    if(var_name == "STATE@LVPL VOLTAGE SENSOR")
        return LVPL_voltage_sensor.get_state();

    return 0.0;
}

complex<double> WT3GX::get_terminal_complex_power_in_pu_based_on_mbase()
{
    complex<double> Vxy = get_terminal_complex_voltage_in_pu();
    complex<double> Ixy = get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase();

    complex<double> S = Vxy*conj(Ixy);
    return S;
}

complex<double> WT3GX::get_terminal_complex_power_in_MVA()
{
    return get_terminal_complex_power_in_pu_based_on_mbase()*get_mbase_in_MVA();
}

double WT3GX::get_terminal_active_power_in_pu_based_on_mbase()
{
    return get_terminal_complex_power_in_pu_based_on_mbase().real();
}

double WT3GX::get_terminal_active_power_in_MW()
{
    return get_terminal_complex_power_in_MVA().real();
}

double WT3GX::get_terminal_reactive_power_in_pu_based_on_mbase()
{
    return get_terminal_complex_power_in_pu_based_on_mbase().imag();
}

double WT3GX::get_terminal_reactive_power_in_MVar()
{
    return get_terminal_complex_power_in_MVA().imag();
}

double WT3GX::get_active_power_generation_including_stator_loss_in_pu_based_on_mbase()
{
    return get_active_power_generation_including_stator_loss_in_MW()/get_mbase_in_MVA();
}

double WT3GX::get_active_power_generation_including_stator_loss_in_MW()
{
    double pterm = get_terminal_active_power_in_MW();
    double rsource = get_source_impedance_in_pu_based_on_mbase().real();
    double iterm = steps_fast_complex_abs(get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase());
    double mbase = get_mbase_in_MVA();

    return pterm+rsource*iterm*iterm*mbase;
}

double WT3GX::get_pll_angle_in_rad()
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    osstream<<"WARNING. "<<get_model_name()<<"::"<<__FUNCTION__<<"() is called without implementation. ZERO is returned with no meaning.";
    toolkit.show_information_with_leading_time_stamp(osstream);

    return 0;
}

double WT3GX::get_pll_angle_in_deg()
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    osstream<<"WARNING. "<<get_model_name()<<"::"<<__FUNCTION__<<"() is called without implementation. ZERO is returned with no meaning.";
    toolkit.show_information_with_leading_time_stamp(osstream);

    return 0;
}

double WT3GX::get_pll_frequency_deviation_in_pu()
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    osstream<<"WARNING. "<<get_model_name()<<"::"<<__FUNCTION__<<"() is called without implementation. ZERO is returned with no meaning.";
    toolkit.show_information_with_leading_time_stamp(osstream);

    return 0;
}

double WT3GX::get_pll_frequency_deviation_in_Hz()
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    osstream<<"WARNING. "<<get_model_name()<<"::"<<__FUNCTION__<<"() is called without implementation. ZERO is returned with no meaning.";
    toolkit.show_information_with_leading_time_stamp(osstream);

    return 0;
}

double WT3GX::get_pll_frequency_in_pu()
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    osstream<<"WARNING. "<<get_model_name()<<"::"<<__FUNCTION__<<"() is called without implementation. ZERO is returned with no meaning.";
    toolkit.show_information_with_leading_time_stamp(osstream);

    return 0;
}

double WT3GX::get_pll_frequency_in_Hz()
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    osstream<<"WARNING. "<<get_model_name()<<"::"<<__FUNCTION__<<"() is called without implementation. ZERO is returned with no meaning.";
    toolkit.show_information_with_leading_time_stamp(osstream);

    return 0;
}

complex<double> WT3GX::get_internal_voltage_in_pu_in_xy_axis()
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

string WT3GX::get_dynamic_data_in_psse_format() const
{
    return "";
}

string WT3GX::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string WT3GX::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}

#include "header/model/wtg_models/wt_generator_model/wt3g1.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <cstdio>
#include <istream>
#include <iostream>
using namespace std;

WT3G1::WT3G1()
{
    clear();
}

WT3G1::~WT3G1()
{
    clear();
}

void WT3G1::clear()
{
    prepare_model_data_table();
    prepare_model_internal_variable_table();

    set_current_source_flag(false);

    set_Xeq_in_pu(0.0);

    active_current_commander.set_limiter_type(NO_LIMITER);
    active_current_commander.set_K(1.0);
    active_current_commander.set_T_in_s(0.02);

    reactive_voltage_commander.set_limiter_type(NO_LIMITER);
    reactive_voltage_commander.set_K(1.0);
    reactive_voltage_commander.set_T_in_s(0.02);

    PLL_frequency_integrator.set_limiter_type(NON_WINDUP_LIMITER);

    PLL_angle_integrator.set_limiter_type(NO_LIMITER);
}

void WT3G1::copy_from_const_model(const WT3G1& model)
{
    clear();
    set_current_source_flag(model.get_current_source_flag());
    set_Xeq_in_pu(model.get_Xeq_in_pu());
    set_KPLL(model.get_KPLL());
    set_KIPLL(model.get_KIPLL());
    set_PLLmax(model.get_PLLmax());
}

WT3G1::WT3G1(const WT3G1& model):WT_GENERATOR_MODEL()
{
    copy_from_const_model(model);
}

WT3G1& WT3G1::operator=(const WT3G1& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

void WT3G1::set_Xeq_in_pu(double xeq)
{
    Xeq = xeq;
}

void WT3G1::set_KPLL(double K)
{
    KPLL = K;
}

void WT3G1::set_KIPLL(double K)
{
    PLL_frequency_integrator.set_T_in_s(1.0/K);
}

void WT3G1::set_PLLmax(double pmax)
{
    PLL_frequency_integrator.set_upper_limit(pmax);
}

double WT3G1::get_Xeq_in_pu() const
{
    return Xeq;
}

double WT3G1::get_KPLL() const
{
    return KPLL;
}

double WT3G1::get_KIPLL() const
{
    return 1.0/PLL_frequency_integrator.get_T_in_s();
}

double WT3G1::get_PLLmax() const
{
    return PLL_frequency_integrator.get_upper_limit();
}

string WT3G1::get_model_name() const
{
    return "WT3G1";
}

bool WT3G1::setup_model_with_steps_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    show_information_with_leading_time_stamp(osstream);
    return false;
}

bool WT3G1::setup_model_with_psse_string(string data)
{
    ostringstream osstream;

    bool is_successful = false;
    vector<string> dyrdata = split_string(data,",");

    if(dyrdata.size()<8)
        return is_successful;

    string model_name = get_string_data(dyrdata[1],"");
    if(model_name!=get_model_name())
        return is_successful;

    size_t ibus;
    string id;
    size_t n_lumped_turbine;
    double xeq, kpll, kipll, pllmax, prate;

    ibus = size_t(get_integer_data(dyrdata[0],"0"));
    id = get_string_data(dyrdata[2],"");

    size_t i=3;
    xeq = get_double_data(dyrdata[i],"0.0"); i++;
    kpll = get_double_data(dyrdata[i],"0.0"); i++;
    kipll = get_double_data(dyrdata[i],"0.0"); i++;
    pllmax = get_double_data(dyrdata[i],"0.0"); i++;
    prate = get_double_data(dyrdata[i],"0.0");

    DEVICE_ID did = get_wt_generator_device_id(ibus, id);
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    WT_GENERATOR* gen = psdb.get_wt_generator(did);
    if(gen==NULL)
    {
        osstream<<"Error when loading data to build "<<get_model_name()<<" model for "<<did.get_device_name()<<endl
               <<"No such wt generator exists in the power system database.";
        show_information_with_leading_time_stamp(osstream);
        return is_successful;
    }

    double mbase = gen->get_mbase_in_MVA();
    n_lumped_turbine = round(mbase/prate);
    if(fabs(mbase-n_lumped_turbine*prate)>1e-2)
    {
        osstream<<"Warning. The MBASE of "<<did.get_device_name()<<" is far way from n times of the Prate of "<<get_model_name()<<" model."<<endl
               <<"MBASE = "<<mbase<<" MVA and Prate = "<<prate<<" MW."<<endl
               <<"Machine MBASE will be updated as "<<n_lumped_turbine*prate<<" MVA.";
        show_information_with_leading_time_stamp(osstream);
        gen->set_mbase_in_MVA(n_lumped_turbine*prate);
    }

    gen->set_number_of_lumped_wt_generators(n_lumped_turbine);
    gen->set_rated_power_per_wt_generator_in_MW(prate);

    set_Xeq_in_pu(xeq);
    set_KPLL(kpll);
    set_KIPLL(kipll);
    set_PLLmax(pllmax);

    complex<double> Z = gen->get_source_impedance_in_pu();
    if(fabs(Z.imag()-xeq)>FLOAT_EPSILON)
    {
        osstream<<"Warning. The Xeq of "<<get_model_name()<<" model is different from imaginary part of ZSOURCE of "<<did.get_device_name()<<endl
               <<"Xeq = "<<xeq<<" and XSource = "<<Z.imag()<<endl
               <<"XSource will be updated as "<<xeq;
        show_information_with_leading_time_stamp(osstream);
        Z = complex<double>(Z.real(), xeq);
        gen->set_source_impedance_in_pu(Z);
    }

    is_successful = true;

    return is_successful;
}

bool WT3G1::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    show_information_with_leading_time_stamp(osstream);
    return false;
}

void WT3G1::initialize()
{
    ostringstream oosstream;
    if(is_model_initialized())
        return;

    WT_GENERATOR* wt_generator = get_wt_generator_pointer();
    if(wt_generator==NULL)
        return;

    size_t n_lumped = get_number_of_lumped_wt_generators();
    double fbase = get_bus_base_frequency_in_Hz();
    double wbase = 2.0*PI*fbase;

    double kipll = get_KIPLL();
    if(kipll!=0.0)
    {
        PLL_frequency_integrator.set_T_in_s(1.0/kipll);
        double pllmax = get_PLLmax();
        PLL_frequency_integrator.set_upper_limit(pllmax);
        PLL_frequency_integrator.set_lower_limit(-pllmax);
    }

    PLL_angle_integrator.set_T_in_s(1.0/wbase);

    double mbase = get_mbase_in_MVA();
    mbase /= n_lumped;

    double xeq = get_Xeq_in_pu();
    complex<double>Zsource(0.0, xeq);

    double P = wt_generator->get_p_generation_in_MW()/n_lumped;
    double Q = wt_generator->get_q_generation_in_MVar()/n_lumped;
    complex<double> S(P/mbase,Q/mbase);


    complex<double> Vxy = get_terminal_complex_voltage_in_pu();
    double angle_in_rad = atan2(Vxy.imag(), Vxy.real());
    // ignore voltage angle
    complex<double> Ixy = conj(S/Vxy);
    complex<double> Isource = Ixy + Vxy/Zsource;

    double Ix = Isource.real();
    double Iy = Isource.imag();

    double IP = Ix*cos(angle_in_rad) + Iy*sin(angle_in_rad);
    double IQ =-Ix*sin(angle_in_rad) + Iy*cos(angle_in_rad);

    double EQ = IQ*(-xeq);

    active_current_commander.set_output(IP);
    active_current_commander.initialize();

    reactive_voltage_commander.set_output(EQ);
    reactive_voltage_commander.initialize();

    if(kipll!=0.0)
    {
        PLL_frequency_integrator.set_output(0.0);
        PLL_frequency_integrator.initialize();
    }

    PLL_angle_integrator.set_output(angle_in_rad);
    PLL_angle_integrator.initialize();

    set_initial_active_current_command_in_pu_based_on_mbase(IP);
    set_initial_reactive_current_command_in_pu_based_on_mbase(IQ);
    set_initial_reactive_voltage_command_in_pu(EQ);

    set_flag_model_initialized_as_true();

    oosstream<<get_model_name()<<" model of "<<get_device_name()<<" is initialized."<<endl
            <<"(1) Initial active current command = "<<get_initial_active_current_command_in_pu_based_on_mbase()<<endl
            <<"(2) Initial reactive current command = "<<get_initial_reactive_current_command_in_pu_based_on_mbase()<<endl
            <<"(3) States of blocks"<<endl
            <<"    active_current_commander block state: "<<active_current_commander.get_state()<<endl
            <<"    reactive_voltage_commander block state: "<<reactive_voltage_commander.get_state()<<endl
            <<"    PLL_frequency_integrator block state: "<<PLL_frequency_integrator.get_state()<<endl
            <<"    PLL_angle_integrator block state: "<<PLL_angle_integrator.get_state()<<endl
            <<"(4) active power generation :"<<get_terminal_active_power_in_MW()<<"MW"<<endl
            <<"(5) reactive power generation :"<<get_terminal_reactive_power_in_MVar()<<"MVar"<<endl
            <<"(6) terminal current :"<<get_terminal_current_in_pu_based_on_mbase()<<"pu";
    show_information_with_leading_time_stamp(oosstream);
}

void WT3G1::run(DYNAMIC_MODE mode)
{
    WT_GENERATOR* wt_generator = get_wt_generator_pointer();
    if(wt_generator==NULL)
        return;

    double fbase = get_bus_base_frequency_in_Hz();
    double wbase = 2.0*PI*fbase;

    complex<double> Vxy = get_terminal_complex_voltage_in_pu();
    double angle_in_rad = atan2(Vxy.imag(), Vxy.real());
    double angle_in_deg = rad2deg(angle_in_rad);

    double IP = get_active_current_command_in_pu_based_on_mbase();

    active_current_commander.set_input(IP);
    active_current_commander.run(mode);

    double EQ = get_reactive_voltage_command_in_pu();

    reactive_voltage_commander.set_input(EQ);
    reactive_voltage_commander.run(mode);

    double kpll = get_KPLL();
    double kipll = get_KIPLL();
    if(kpll==0.0 and kipll==0.0)
    {
        set_pll_angle_in_deg(angle_in_deg);
    }
    else
    {
        double Vr = Vxy.real();
        double Vi = Vxy.imag();

        double angle = get_pll_angle_in_rad();
        double Vy = -Vr*sin(angle)+Vi*cos(angle);

        double input = Vy*kpll/wbase;
        PLL_frequency_integrator.set_input(input);
        PLL_frequency_integrator.run(mode);

        double output = PLL_frequency_integrator.get_output();
        input += output;

        double pllmax = get_PLLmax();
        if(input>pllmax)
            input = pllmax;
        if(input<-pllmax)
            input = -pllmax;

        PLL_angle_integrator.set_input(input);
        PLL_angle_integrator.run(mode);
    }
    if(mode==UPDATE_MODE)
        set_flag_model_updated_as_true();
}

complex<double> WT3G1::get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_sbase()
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();
    double mbase = get_mbase_in_MVA();

    double Xeq = get_Xeq_in_pu();

    double Ip = active_current_commander.get_output();
    double Iq = -reactive_voltage_commander.get_output()/Xeq;

    double pll_angle = get_pll_angle_in_rad();

    double Ix = Ip*cos(pll_angle) - Iq*sin(pll_angle);
    double Iy = Ip*sin(pll_angle) + Iq*cos(pll_angle);

    complex<double> Ixy(Ix, Iy);
    //cout<<"Norton Ixy based on mbase = "<<Ixy<<endl;

    return Ixy*mbase/sbase;
}

complex<double> WT3G1::get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase()
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();
    double mbase = get_mbase_in_MVA();
    complex<double> Ixy = get_terminal_complex_current_in_pu_in_xy_axis_based_on_sbase();
    return Ixy*sbase/mbase;
}

complex<double> WT3G1::get_terminal_complex_current_in_pu_in_xy_axis_based_on_sbase()
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();
    double mbase = get_mbase_in_MVA();

    complex<double> Zsource (0.0, get_Xeq_in_pu());
    Zsource /= mbase;
    Zsource *= sbase;

    complex<double> Ixy_norton = get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_sbase();
    complex<double> Vxy = get_terminal_complex_voltage_in_pu();
    complex<double> Ixy_term = Ixy_norton - Vxy/Zsource;
    return Ixy_term;
}

double WT3G1::get_terminal_current_in_pu_based_on_mbase()
{
    return steps_fast_complex_abs(get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase());
}

double WT3G1::get_terminal_current_in_pu_based_on_sbase()
{
    return steps_fast_complex_abs(get_terminal_complex_current_in_pu_in_xy_axis_based_on_sbase());
}


void WT3G1::check()
{
    ;
}

void WT3G1::report()
{
    ostringstream osstream;
    osstream<<get_standard_model_string();
    show_information_with_leading_time_stamp(osstream);
}

void WT3G1::save()
{
    ;
}

string WT3G1::get_standard_model_string() const
{
    ostringstream osstream;
    DEVICE_ID did = get_device_id();
    size_t bus = did.get_device_terminal().get_buses()[0];
    string identifier = did.get_device_identifier();

    osstream<<setw(8)<<bus<<", "
      <<"'"<<get_model_name()<<"', "
      <<"'"<<identifier<<"', "
      <<setw(8)<<setprecision(6)<<get_Xeq_in_pu()<<", "
      <<setw(8)<<setprecision(6)<<get_KPLL()<<", "
      <<setw(8)<<setprecision(6)<<get_KIPLL()<<", "
      <<setw(8)<<setprecision(6)<<get_PLLmax()<<", "
      <<setw(8)<<setprecision(6)<<get_rated_power_per_wt_generator_in_MW()<<" /";

    return osstream.str();
}

void WT3G1::prepare_model_data_table()
{
    clear_model_data_table();
    size_t i=0;
    add_model_data_name_and_index_pair("A", i); i++;
}

double WT3G1::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="A")
        return 0.0;

    return 0.0;
}

void WT3G1::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="A")
        return;

    return;
}

void WT3G1::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    size_t i=0;
    add_model_inernal_variable_name_and_index_pair("PLL ANGLE IN DEG", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@ACTIVE CURRENT COMMAND BLOCK", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@REACTIVE VOLTAGE COMMAND BLOCK", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@PLL FREQUENCY BLOCK", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@PLL ANGLE BLOCK", i); i++;
}

double WT3G1::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name == "PLL ANGLE IN DEG")
        return get_pll_angle_in_deg();
    if(var_name == "STATE@ACTIVE CURRENT COMMAND BLOCK")
        return active_current_commander.get_state();
    if(var_name == "STATE@REACTIVE VOLTAGE COMMAND BLOCK")
        return reactive_voltage_commander.get_state();
    if(var_name == "STATE@PLL FREQUENCY BLOCK")
        return PLL_frequency_integrator.get_state();
    if(var_name == "STATE@PLL ANGLE BLOCK")
        return PLL_angle_integrator.get_state();

    return 0.0;
}

complex<double> WT3G1::get_terminal_complex_power_in_pu_based_on_mbase()
{
    complex<double> Vxy = get_terminal_complex_voltage_in_pu();
    complex<double> Ixy = get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase();

    complex<double> S = Vxy*conj(Ixy);
    return S;
}

complex<double> WT3G1::get_terminal_complex_power_in_MVA()
{
    return get_terminal_complex_power_in_pu_based_on_mbase()*get_mbase_in_MVA();
}

double WT3G1::get_terminal_active_power_in_pu_based_on_mbase()
{
    return get_terminal_complex_power_in_pu_based_on_mbase().real();
}

double WT3G1::get_terminal_active_power_in_MW()
{
    return get_terminal_complex_power_in_MVA().real();
}

double WT3G1::get_terminal_reactive_power_in_pu_based_on_mbase()
{
    return get_terminal_complex_power_in_pu_based_on_mbase().imag();
}

double WT3G1::get_terminal_reactive_power_in_MVar()
{
    return get_terminal_complex_power_in_MVA().imag();
}

double WT3G1::get_active_power_generation_including_stator_loss_in_pu_based_on_mbase()
{
    return get_active_power_generation_including_stator_loss_in_MW()/get_mbase_in_MVA();
}

double WT3G1::get_active_power_generation_including_stator_loss_in_MW()
{
    double pterm = get_terminal_active_power_in_MW();
    return pterm;

    /*double rsource = get_source_impedance_in_pu_based_on_mbase().real();
    double iterm = steps_fast_complex_abs(get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase());
    double mbase = get_mbase_in_MVA();

    return pterm+rsource*iterm*iterm*mbase;*/
}

double WT3G1::get_pll_angle_in_rad()
{
    double kpll = get_KPLL();
    double kipll = get_KIPLL();
    if(kpll==0.0 and kipll==0.0)
    {
        complex<double> Vxy = get_terminal_complex_voltage_in_pu();
        double angle = atan2(Vxy.imag(), Vxy.real());
        return angle;
    }
    else
        return PLL_angle_integrator.get_output();
}

double WT3G1::get_pll_angle_in_deg()
{
    return rad2deg(get_pll_angle_in_rad());
}

double WT3G1::get_pll_frequency_deviation_in_pu()
{
    double fbase = get_bus_base_frequency_in_Hz();
    double wbase = 2.0*PI*fbase;

    complex<double> Vxy = get_terminal_complex_voltage_in_pu();

    double kpll = get_KPLL();
    double kipll = get_KIPLL();
    if(kpll==0.0 and kipll==0.0)
        return 0.0;
    else
    {
        double Vr = Vxy.real();
        double Vi = Vxy.imag();

        double angle = get_pll_angle_in_rad();
        double Vy = -Vr*sin(angle)+Vi*cos(angle);

        double input = Vy*kpll/wbase;

        double output = PLL_frequency_integrator.get_output();

        return input+output;
    }
}

double WT3G1::get_pll_frequency_deviation_in_Hz()
{
    double fbase = get_bus_base_frequency_in_Hz();

    return fbase*get_pll_frequency_deviation_in_pu();
}

double WT3G1::get_pll_frequency_in_pu()
{
    return 1.0+get_pll_frequency_deviation_in_pu();
}

double WT3G1::get_pll_frequency_in_Hz()
{
    double fbase = get_bus_base_frequency_in_Hz();

    return fbase*get_pll_frequency_in_pu();
}

complex<double> WT3G1::get_internal_voltage_in_pu_in_xy_axis()
{
    complex<double> Ixy = get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_sbase();
    complex<double> Z(0.0, get_Xeq_in_pu());

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();
    double mbase = get_mbase_in_MVA();

    Z = Z/mbase*sbase;

    return Ixy*Z;
}



void WT3G1::set_pll_angle_in_deg(double angle)
{
    PLL_angle_integrator.set_output(deg2rad(angle));
    PLL_angle_integrator.initialize();// the initialize function is used to update STORE
}


string WT3G1::get_dynamic_data_in_psse_format() const
{
    return "";
}

string WT3G1::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string WT3G1::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}

#include "header/model/wtg_models/wt_generator_model/wt3gx.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <cstdio>
#include <istream>
#include <iostream>
using namespace std;

WT3GX::WT3GX(STEPS& toolkit) : WT_GENERATOR_MODEL(toolkit),
                               vsg(toolkit),
                               convcol(toolkit)
{
    clear();
}

WT3GX::~WT3GX()
{
}

void WT3GX::clear()
{
    set_model_float_parameter_count(6);

    set_current_source_flag(false);

    vsg.clear();
}

void WT3GX::copy_from_const_model(const WT3GX& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);

    vsg.set_toolkit(toolkit);
    convcol.set_toolkit(toolkit);

    clear();

    set_Tj_in_s(model.get_Tj_in_s());
    set_D(model.get_D());
    set_Ku(model.get_Ku());
    set_Te_in_s(model.get_Te_in_s());

    set_pq_priority_flag(model.get_pq_priority_flag());
    set_Imax(model.get_Imax());
}

WT3GX::WT3GX(const WT3GX& model):WT_GENERATOR_MODEL(model.get_toolkit()),
                                 vsg(model.get_toolkit()),
                                 convcol(model.get_toolkit())
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

void WT3GX::set_pq_priority_flag(COL_PQ_PRIORITY flag)
{
    convcol.set_PQ_priority_flag(flag);
}

void WT3GX::set_Imax(double Imax)
{
    convcol.set_Imax(Imax);
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

COL_PQ_PRIORITY WT3GX::get_pq_priority_flag() const
{
    return convcol.get_PQ_priority_flag();
}

double WT3GX::get_Imax() const
{
    return convcol.get_Imax();
}

double WT3GX::get_virtual_voltage_in_pu()
{
    return vsg.get_virtual_voltage_in_pu();
}

double WT3GX::get_virtual_angle_in_rad()
{
    return vsg.get_virtual_angle_in_rad();
}

complex<double> WT3GX::get_virtual_voltage_in_pu_in_xy_axis()
{
    double E = get_virtual_voltage_in_pu();
    double theta = get_virtual_angle_in_rad();
    double Ex = E * steps_cos(theta);
    double Ey = E * steps_sin(theta);
    complex<double> Exy(Ex, Ey);
    return Exy;
}

complex<double> WT3GX::get_limited_PQ_current_order()
{
    complex<double> Exy = get_virtual_voltage_in_pu_in_xy_axis();
    complex<double> Vxy = get_terminal_complex_voltage_in_pu();
    complex<double> Zsource = get_source_impedance_in_pu_based_on_mbase();
    complex<double> Ixy_guess = (Exy - Vxy) / Zsource;

    double Ix_guess = Ixy_guess.real();
    double Iy_guess = Ixy_guess.imag();

    double angle_in_rad = get_terminal_voltage_angle_in_rad();
    double sine = steps_sin(angle_in_rad), cosine = steps_cos(angle_in_rad);
    double Ip_guess = Ix_guess*cosine + Iy_guess*sine;
    double Iq_guess =-Ix_guess*sine + Iy_guess*cosine;
    complex<double> I_guess (Ip_guess,Iq_guess);
    return convcol.get_limited_PQ_current_order(I_guess);
}

string WT3GX::get_model_name() const
{
    return "WT3GX";
}

bool WT3GX::setup_model_with_steps_string_vector(vector<string>& data)
{
    ostringstream osstream;

    bool is_successful = false;

    if(data.size()>=10)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            unsigned int ibus;
            string id;
            double tj, d, ku, te, prate, imax;
            unsigned int pqflag;

            ibus = (unsigned int)(get_integer_data(data[1],"0"));
            id = get_string_data(data[2],"");

            unsigned int i=3;

            prate = get_double_data(data[i],"0.0"); i++;
            tj = get_double_data(data[i],"0.0"); i++;
            d = get_double_data(data[i],"0.0"); i++;
            ku = get_double_data(data[i],"0.0"); i++;
            te = get_double_data(data[i],"0.0"); i++;
            pqflag = (unsigned int)(get_integer_data(data[i],"0")); i++;
            imax = get_double_data(data[i],"0.0");

            DEVICE_ID did = get_wt_generator_device_id(ibus, id);
            STEPS& toolkit = get_toolkit();
            POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
            WT_GENERATOR* gen = psdb.get_wt_generator(did);

            if(gen==NULL)
            {
                osstream<<"Error when loading data to build "<<get_model_name()<<" model for "<<did.get_compound_device_name()<<endl
                       <<"No such wt generator exists in the power system database.";
                toolkit.show_information_with_leading_time_stamp(osstream);
                return is_successful;
            }
            double n_lumped_turbine;
            double mbase = gen->get_mbase_in_MVA();
            n_lumped_turbine = mbase/prate;
            if(fabs(n_lumped_turbine-round(n_lumped_turbine))>0.1)
            {
                osstream<<"Warning. The MBASE of "<<did.get_compound_device_name()<<" deviates from n(integer) times of the Prate in "<<get_model_name()<<" model."<<endl
                        <<"MBASE = "<<mbase<<" MVA, Prate = "<<prate<<" MW, and N(float) = "<<n_lumped_turbine<<"."<<endl
                        <<"User should confirm if MBASE or Prate needs to be corrected.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }

            gen->set_number_of_lumped_wt_generators(n_lumped_turbine);
            gen->set_rated_power_per_wt_generator_in_MW(prate);


            set_Tj_in_s(tj);
            set_D(d);
            set_Ku(ku);
            set_Te_in_s(te);
            switch(pqflag)
            {
            case 1:
                set_pq_priority_flag(Q_FIRST);
                break;
            case 0:
            default:
                set_pq_priority_flag(P_FIRST);
                break;
            }
            set_Imax(imax);

            is_successful = true;

            return is_successful;
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
    vsg.set_bus_pointer(get_bus_pointer());
    vsg.set_device_pointer(get_device_pointer());
}

void WT3GX::initialize()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    if(not is_model_initialized())
    {
        WT_GENERATOR* wt_generator = get_wt_generator_pointer();
        setup_block_toolkit_and_parameters();

        double n_lumped = get_number_of_lumped_wt_generators();
        double P = wt_generator->get_p_generation_in_MW()/n_lumped;
        double Q = wt_generator->get_q_generation_in_MVar()/n_lumped;

        double prate = get_rated_power_per_wt_generator_in_MW();
        P /= prate;
        Q /= prate;
        complex<double> S(P, Q);

        complex<double> Zsource = get_source_impedance_in_pu_based_on_mbase();
        double Xeq = Zsource.imag();

        complex<double> Vxy = get_terminal_complex_voltage_in_pu();
        complex<double> Ixy = conj(S/Vxy);
        double Ix = Ixy.real();
        double Iy = Ixy.imag();

        double angle_in_rad = get_terminal_voltage_angle_in_rad();
        double sine = steps_sin(angle_in_rad), cosine = steps_cos(angle_in_rad);

        double Ip = Ix*cosine + Iy*sine;
        double Iq =-Ix*sine + Iy*cosine;


        double Eqcmd = Iq*(-Xeq);

        vsg.set_Pref_in_pu_based_on_mbase(P);
        vsg.set_P_in_pu_based_on_mbase(P);

        vsg.set_Qref_in_pu_based_on_mbase(Q);
        vsg.set_Q_in_pu_based_on_mbase(Q);

        vsg.set_Vref_in_pu(Eqcmd);
        vsg.set_V_in_pu(Eqcmd);

        complex<double> Esource = Vxy + Ixy * Zsource;
        double Emag = steps_fast_complex_abs(Esource);
        double Earg = steps_fast_complex_arg(Esource);

        vsg.set_initial_virtual_voltage_in_pu(Emag);
        vsg.set_initial_virtual_angle_in_rad(Earg);

        vsg.initialize();

        //set internal value
        set_initial_active_power_command_in_pu_based_on_mbase(P);
        set_initial_reactive_power_command_in_pu_based_on_mbase(Q);

        set_initial_active_current_command_in_pu_based_on_mbase(Ip);
        set_initial_reactive_current_command_in_pu_based_on_mbase(Iq);

        set_initial_reactive_voltage_command_in_pu(Eqcmd);

        set_flag_model_initialized_as_true();

        if(toolkit.is_detailed_log_enabled())
        {
            osstream<<get_model_name()<<" model of "<<get_compound_device_name()<<" is initialized."<<endl
                    <<"(1) Initial active power command = "<<get_initial_active_power_command_in_pu_based_on_mbase()<<endl
                    <<"(2) Initial reactive power command = "<<get_initial_reactive_power_command_in_pu_based_on_mbase()<<endl
                    <<"(3) Initial reactive voltage command = "<<get_initial_reactive_voltage_command_in_pu()<<endl
                    <<"(4) States of blocks"<<endl
                    <<"    virtual_frequency_deviation block state: "<<vsg.get_virtual_frequency_deviation_block_state()<<endl
                    <<"    virtual_angle block state: "<<vsg.get_virtual_angle_block_state()<<endl
                    <<"    virtual_voltage block state: "<<vsg.get_virtual_voltage_block_state()<<endl
                    <<"(5) active power generation :"<<get_terminal_active_power_in_MW()<<"MW"<<endl
                    <<"(6) reactive power generation :"<<get_terminal_reactive_power_in_MVar()<<"MVar"<<endl
                    <<"(7) terminal current :"<<get_terminal_current_in_pu_based_on_mbase()<<"pu"<<endl
                    <<"(8) terminal voltage :"<<get_terminal_voltage_in_pu()<<"pu";///////////////////////////////////
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void WT3GX::run(DYNAMIC_MODE mode)
{
    complex<double> S = get_terminal_complex_power_in_pu_based_on_mbase();
    double P = S.real(), Q = S.imag();

    complex<double> Vxy = get_terminal_complex_voltage_in_pu();
    complex<double> Ixy = conj(S/Vxy);
    double Ix = Ixy.real(), Iy = Ixy.imag();

    complex<double> Zsource = get_source_impedance_in_pu_based_on_mbase();
    double Xeq = Zsource.imag();

    double angle_in_rad = get_terminal_voltage_angle_in_rad();
    double sine = steps_sin(angle_in_rad), cosine = steps_cos(angle_in_rad);
    double Iq = -Ix*sine + Iy*cosine;
    double Eq = Iq*(-Xeq);

    vsg.set_Pref_in_pu_based_on_mbase(get_active_power_command_in_pu_based_on_mbase());
    vsg.set_P_in_pu_based_on_mbase(P);

    vsg.set_Qref_in_pu_based_on_mbase(get_reactive_power_command_in_pu_based_on_mbase());
    vsg.set_Q_in_pu_based_on_mbase(Q);

    vsg.set_Vref_in_pu(get_reactive_voltage_command_in_pu());
    vsg.set_V_in_pu(Eq);

    vsg.run(mode);

    if(mode==DYNAMIC_UPDATE_MODE)
        set_flag_model_updated_as_true();
}

complex<double> WT3GX::get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_sbase()
{
    STEPS& toolkit = get_toolkit();
    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
    double mbase = get_mbase_in_MVA();
    complex<double> Exy = get_internal_voltage_in_pu_in_xy_axis();
    complex<double> Zsource = get_source_impedance_in_pu_based_on_mbase();
    complex<double> Ixy = Exy / Zsource;
    return Ixy*(mbase*one_over_sbase);
}

complex<double> WT3GX::get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase()
{
    complex<double> Exy = get_internal_voltage_in_pu_in_xy_axis();
    complex<double> Vxy = get_terminal_complex_voltage_in_pu();
    complex<double> Zsource = get_source_impedance_in_pu_based_on_mbase();
    complex<double> Ixy = (Exy - Vxy) / Zsource;
    return Ixy;
}

complex<double> WT3GX::get_terminal_complex_current_in_pu_in_xy_axis_based_on_sbase()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    double one_over_sbase = psdb.get_one_over_system_base_power_in_one_over_MVA();
    double mbase = get_mbase_in_MVA();
    complex<double> Ixy_term = get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase() * mbase * one_over_sbase;
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

    STEPS& toolkit = get_toolkit();
    NETWORK_MATRIX& network = toolkit.get_network_matrix();
    if(export_internal_bus_number==true)
        bus = network.get_internal_bus_number_of_physical_bus(bus)+1;

    osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(6)<<identifier<<", "
            <<setw(8)<<setprecision(6)<<get_rated_power_per_wt_generator_in_MW()<<", "
            <<setw(8)<<setprecision(6)<<get_Tj_in_s()<<", "
            <<setw(8)<<setprecision(6)<<get_D()<<", "
            <<setw(8)<<setprecision(6)<<get_Ku()<<", "
            <<setw(8)<<setprecision(6)<<get_Te_in_s()<<", "
            <<setw(8)<<setprecision(6)<<get_pq_priority_flag()<<", "
            <<setw(8)<<setprecision(6)<<get_Imax()<<" /";

    return osstream.str();
}

void WT3GX::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("PN", i); i++;
    add_model_data_name_and_index_pair("TJ", i); i++;
    add_model_data_name_and_index_pair("D", i); i++;
    add_model_data_name_and_index_pair("KU", i); i++;
    add_model_data_name_and_index_pair("TE", i); i++;
    add_model_data_name_and_index_pair("PQ PRIORITY FLAG", i); i++;
    add_model_data_name_and_index_pair("IMAX", i);
}

double WT3GX::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="PN")
        return get_rated_power_per_wt_generator_in_MW();
    if(par_name=="TJ")
        return get_Tj_in_s();
    if(par_name=="D")
        return get_D();
    if(par_name=="KU")
        return get_Ku();
    if(par_name=="TE")
        return get_Te_in_s();
    if(par_name=="PQ PRIORITY FLAG")
        return get_pq_priority_flag();
    if(par_name=="IMAX")
        return get_Imax();

    return 0.0;
}

void WT3GX::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="PN")
    {
        WT_GENERATOR* gen = get_wt_generator_pointer();
        return gen->set_rated_power_per_wt_generator_in_MW(value);
    }
    if(par_name=="TJ")
        return set_Tj_in_s(value);
    if(par_name=="D")
        return set_D(value);
    if(par_name=="KU")
        return set_Ku(value);
    if(par_name=="TE")
        return set_Te_in_s(value);
    if(par_name=="PQ PRIORITY FLAG")
    {
        int ivalue = int(value);
        COL_PQ_PRIORITY flag = P_FIRST;
        switch(ivalue)
        {
            case 1:
                flag = Q_FIRST;
                break;
            case 0:
            default:
                flag = P_FIRST;
                break;
        }
        return set_pq_priority_flag(flag);
    }
    if(par_name=="IMAX")
        return set_Imax(value);

    return;
}

double WT3GX::get_minimum_nonzero_time_constant_in_s()
{
    double mint = INFINITE_THRESHOLD;
    if(get_Te_in_s()!=0.0 and mint>get_Te_in_s())
        mint = get_Te_in_s();
    if(get_Tj_in_s()!=0.0 and mint>get_Tj_in_s())
        mint = get_Tj_in_s();
    return mint;
}

void WT3GX::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_internal_variable_name_and_index_pair("STATE@VIRTUAL FREQUENCY BLOCK", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@VIRTUAL ANGLE BLOCK", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@VIRTUAL VOLTAGE BLOCK", i); i++;
}

double WT3GX::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name == "STATE@VIRTUAL FREQUENCY BLOCK")
        return vsg.get_virtual_frequency_deviation_block_state();
    if(var_name == "STATE@VIRTUAL ANGLE BLOCK")
        return vsg.get_virtual_angle_block_state();
    if(var_name == "STATE@VIRTUAL VOLTAGE BLOCK")
        return vsg.get_virtual_voltage_block_state();
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
    complex<double> Vxy = get_terminal_complex_voltage_in_pu();
    double angle_in_rad = get_terminal_voltage_angle_in_rad();
    complex<double> Zsource = get_source_impedance_in_pu_based_on_mbase();
    complex<double> I = get_limited_PQ_current_order();
    double Ip = I.real();
    double Iq = I.imag();
    double sine = steps_sin(angle_in_rad), cosine = steps_cos(angle_in_rad);
    double Ix = Ip*cosine - Iq*sine;
    double Iy = Ip*sine + Iq*cosine;
    complex<double> Ixy(Ix,Iy);
    complex<double> Exy = Vxy + Ixy * Zsource;
    return Exy;
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

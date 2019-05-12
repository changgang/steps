#include "header/model/energy_storage_model/estr0.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <cstdio>
#include <istream>
#include <iostream>
using namespace std;
ESTR0::ESTR0() : ENERGY_STORAGE_MODEL()
{
    clear();
}

ESTR0::~ESTR0()
{
    clear();
}

void ESTR0::clear()
{
    prepare_model_data_table();
    prepare_model_internal_variable_table();

    active_power_filter.set_limiter_type(WINDUP_LIMITER);
    reactive_integral_block.set_limiter_type(WINDUP_LIMITER);
    energy_state_block.set_limiter_type(WINDUP_LIMITER);
}

void ESTR0::copy_from_const_model(const ESTR0& model)
{
    clear();

    //this->set_power_system_database(model.toolkit.get_power_system_database());
    //this->set_device_id(model.get_device_id());

    this->set_Tp1_in_s(model.get_Tp1_in_s());
    this->set_Tp2_in_s(model.get_Tp2_in_s());
    this->set_Tp3_in_s(model.get_Tp3_in_s());
    this->set_Tp4_in_s(model.get_Tp4_in_s());
    this->set_Kpp(model.get_Kpp());
    this->set_Kip(model.get_Kip());
    this->set_Kdp(model.get_Kdp());
    this->set_Tdp_in_s(model.get_Tdp_in_s());
    this->set_Pmax_in_pu(model.get_Pmax_in_pu());
    this->set_Tr_in_s(model.get_Tr_in_s());
    this->set_Iacmax_in_pu(model.get_Iacmax_in_pu());
    this->set_Kin_in_pu(model.get_Kin_in_pu());
    this->set_Kout_in_pu(model.get_Kout_in_pu());
    this->set_E0_in_pu(model.get_E0_in_pu());
    this->set_En_in_MWh(model.get_En_in_MWh());

    this->set_Tq1_in_s(model.get_Tq1_in_s());
    this->set_Tq2_in_s(model.get_Tq2_in_s());
    this->set_Tq3_in_s(model.get_Tq3_in_s());
    this->set_Tq4_in_s(model.get_Tq4_in_s());
    this->set_Kq(model.get_Kq());
    this->set_Dq(model.get_Dq());
}

ESTR0::ESTR0(const ESTR0& model):ENERGY_STORAGE_MODEL()
{
    copy_from_const_model(model);
}

ESTR0& ESTR0::operator=(const ESTR0& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

string ESTR0::get_model_name() const
{
    return "ESTR0";
}


void ESTR0::set_Tp1_in_s(double t)
{
    active_lead_lag_1.set_T1_in_s(t);
}

void ESTR0::set_Tp2_in_s(double t)
{
    active_lead_lag_1.set_T2_in_s(t);
}

void ESTR0::set_Tp3_in_s(double t)
{
    active_lead_lag_2.set_T1_in_s(t);
}

void ESTR0::set_Tp4_in_s(double t)
{
    active_lead_lag_2.set_T2_in_s(t);
}

void ESTR0::set_Kpp(double k)
{
    active_pid_block.set_Kp(k);
}

void ESTR0::set_Kip(double k)
{
    active_pid_block.set_Ki(k);
}

void ESTR0::set_Kdp(double k)
{
    active_pid_block.set_Kd(k);
}

void ESTR0::set_Tdp_in_s(double t)
{
    active_pid_block.set_Td_in_s(t);
}

void ESTR0::set_Tr_in_s(double t)
{
    active_power_filter.set_T_in_s(t);
}

void ESTR0::set_Iacmax_in_pu(double imax)
{
    active_power_filter.set_upper_limit(imax);
    active_power_filter.set_lower_limit(-imax);
}


void ESTR0::set_Kin_in_pu(double k)
{
    Effin = k;
}

void ESTR0::set_Kout_in_pu(double k)
{
    Effout = k;
}

void ESTR0::set_Tq1_in_s(double t)
{
    reactive_lead_lag_1.set_T1_in_s(t);
}

void ESTR0::set_Tq2_in_s(double t)
{
    reactive_lead_lag_1.set_T2_in_s(t);
}

void ESTR0::set_Tq3_in_s(double t)
{
    reactive_lead_lag_2.set_T1_in_s(t);
}

void ESTR0::set_Tq4_in_s(double t)
{
    reactive_lead_lag_2.set_T2_in_s(t);
}

void ESTR0::set_Kq(double k)
{
    reactive_integral_block.set_T_in_s(1.0/k);
}

void ESTR0::set_Dq(double d)
{
    this->Dq = d;
}

double ESTR0::get_Tp1_in_s() const
{
    return active_lead_lag_1.get_T1_in_s();
}

double ESTR0::get_Tp2_in_s() const
{
    return active_lead_lag_1.get_T2_in_s();
}

double ESTR0::get_Tp3_in_s() const
{
    return active_lead_lag_2.get_T1_in_s();
}

double ESTR0::get_Tp4_in_s() const
{
    return active_lead_lag_2.get_T2_in_s();
}

double ESTR0::get_Kpp() const
{
    return active_pid_block.get_Kp();
}

double ESTR0::get_Kip() const
{
    return active_pid_block.get_Ki();
}

double ESTR0::get_Kdp() const
{
    return active_pid_block.get_Kd();
}

double ESTR0::get_Tdp_in_s() const
{
    return active_pid_block.get_Td_in_s();
}

double ESTR0::get_Tr_in_s() const
{
    return active_power_filter.get_T_in_s();
}

double ESTR0::get_Iacmax_in_pu() const
{
    return active_power_filter.get_upper_limit();
}


double ESTR0::get_Kin_in_pu() const
{
    return Effin;
}

double ESTR0::get_Kout_in_pu() const
{
    return Effout;
}

double ESTR0::get_Tq1_in_s() const
{
    return reactive_lead_lag_1.get_T1_in_s();
}

double ESTR0::get_Tq2_in_s() const
{
    return reactive_lead_lag_1.get_T2_in_s();
}

double ESTR0::get_Tq3_in_s() const
{
    return reactive_lead_lag_2.get_T1_in_s();
}

double ESTR0::get_Tq4_in_s() const
{
    return reactive_lead_lag_2.get_T2_in_s();
}

double ESTR0::get_Kq() const
{
    return 1.0/reactive_integral_block.get_T_in_s();
}

double ESTR0::get_Dq() const
{
    return this->Dq;
}

bool ESTR0::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()<24)
        return is_successful;

    string model_name = get_string_data(data[0],"");
    if(model_name!=get_model_name())
        return is_successful;


    double tp1, tp2, tp3, tp4, kpp, kip, kdp, tdp, pmax, tr, iacmax, kin, kout, e0, en;
    double tq1, tq2, tq3, tq4, kq, dq;

    size_t i=3;
    tp1 = get_double_data(data[i],"0.0"); i++;
    tp2 = get_double_data(data[i],"0.0"); i++;
    tp3 = get_double_data(data[i],"0.0"); i++;
    tp4 = get_double_data(data[i],"0.0"); i++;
    kpp = get_double_data(data[i],"0.0"); i++;
    kip = get_double_data(data[i],"0.0"); i++;
    kdp = get_double_data(data[i],"0.0"); i++;
    tdp = get_double_data(data[i],"0.0"); i++;
    pmax = get_double_data(data[i],"0.0"); i++;
    tr = get_double_data(data[i],"0.0"); i++;
    iacmax = get_double_data(data[i],"0.0"); i++;
    kin = get_double_data(data[i],"0.0"); i++;
    kout = get_double_data(data[i],"0.0"); i++;
    e0 = get_double_data(data[i],"0.0"); i++;
    en = get_double_data(data[i],"0.0"); i++;

    tq1 = get_double_data(data[i],"0.0"); i++;
    tq2 = get_double_data(data[i],"0.0"); i++;
    tq3 = get_double_data(data[i],"0.0"); i++;
    tq4 = get_double_data(data[i],"0.0"); i++;
    kq = get_double_data(data[i],"0.0"); i++;
    dq = get_double_data(data[i],"0.0");

    set_Tp1_in_s(tp1);
    set_Tp2_in_s(tp2);
    set_Tp3_in_s(tp3);
    set_Tp4_in_s(tp4);
    set_Kpp(kpp);
    set_Kip(kip);
    set_Kdp(kdp);
    set_Tdp_in_s(tdp);
    set_Pmax_in_pu(pmax);
    set_Tr_in_s(tr);
    set_Iacmax_in_pu(iacmax);
    set_Kin_in_pu(kin);
    set_Kout_in_pu(kout);
    set_E0_in_pu(e0);
    set_En_in_MWh(en);

    set_Tq1_in_s(tq1);
    set_Tq2_in_s(tq2);
    set_Tq3_in_s(tq3);
    set_Tq4_in_s(tq4);
    set_Kq(kq);
    set_Dq(dq);

    is_successful = true;

    return is_successful;
}

bool ESTR0::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool ESTR0::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void ESTR0::set_block_toolkit()
{
    STEPS& toolkit = get_toolkit();

    active_lead_lag_1.set_toolkit(toolkit);
    active_lead_lag_2.set_toolkit(toolkit);
    active_pid_block.set_toolkit(toolkit);
    active_power_filter.set_toolkit(toolkit);
    energy_state_block.set_toolkit(toolkit);
    reactive_lead_lag_1.set_toolkit(toolkit);
    reactive_lead_lag_2.set_toolkit(toolkit);
    reactive_integral_block.set_toolkit(toolkit);
}

void ESTR0::initialize()
{
    set_block_toolkit();

    double V = get_terminal_bus_voltage_in_pu();
    double iacmax = get_Iacmax_in_pu();

    double vamax = iacmax*V;
    active_power_filter.set_upper_limit(vamax);
    active_power_filter.set_lower_limit(-vamax);

    ENERGY_STORAGE* estorage = get_energy_storage_pointer();
    double mbase = get_mbase_in_MVA();

    double P = estorage->get_p_generation_in_MW()/mbase;
    double Q = estorage->get_q_generation_in_MVar()/mbase;

    double iP = P/V;
    double iQ = Q/V;

    double Iqmax = sqrt(iacmax*iacmax-iP*iP);
    reactive_integral_block.set_upper_limit(Iqmax);
    reactive_integral_block.set_lower_limit(-Iqmax);

    energy_state_block.set_output(get_E0_in_pu());
    energy_state_block.initialize();
    double p = P;
    if(P>0.0) p /= get_Kout_in_pu();
    else      p *= get_Kin_in_pu();
    energy_state_block.set_input(-p);

    active_power_filter.set_output(P);
    active_power_filter.initialize();

    set_Pref_in_pu(P);

    active_pid_block.set_output(0.0);
    active_pid_block.initialize();

    active_lead_lag_2.set_output(0.0);
    active_lead_lag_2.initialize();

    active_lead_lag_1.set_output(0.0);
    active_lead_lag_1.initialize();

    reactive_integral_block.set_output(iQ);
    reactive_integral_block.initialize();

    reactive_lead_lag_2.set_output(0.0);
    reactive_lead_lag_2.initialize();

    reactive_lead_lag_1.set_output(0.0);
    reactive_lead_lag_1.initialize();

    double vref = iQ*get_Dq()+V;
    set_Vref_in_pu(vref);


    set_flag_model_initialized_as_true();
}

void ESTR0::run(DYNAMIC_MODE mode)
{
    ENERGY_STORAGE* estorage = get_energy_storage_pointer();
    double mbase = get_mbase_in_MVA();

    double P = estorage->get_p_generation_in_MW()/mbase;

    double df = get_terminal_bus_frequency_deviation_in_pu();

    double input = df;
    active_lead_lag_1.set_input(df);
    active_lead_lag_1.run(mode);

    input = active_lead_lag_1.get_output();
    active_lead_lag_2.set_input(input);
    active_lead_lag_2.run(mode);

    input = active_lead_lag_2.get_output();
    active_pid_block.set_input(input);
    active_pid_block.run(mode);

    double pref = get_Pref_in_pu();

    input = active_pid_block.get_output()+pref;

    double pmax = get_Pmax_in_pu();
    if(input>pmax)
        input = pmax;
    else
    {
        if(input<-pmax)
            input = -pmax;
    }

    double V = get_terminal_bus_voltage_in_pu();
    double iacmax = get_Iacmax_in_pu();

    double vamax = iacmax*V;
    active_power_filter.set_upper_limit(vamax);
    active_power_filter.set_lower_limit(-vamax);

    active_power_filter.set_input(input);
    active_power_filter.run(mode);


    input = active_power_filter.get_output();
    if(input>0.0)
        input /= get_Kout_in_pu();
    else
        input *= get_Kin_in_pu();
    energy_state_block.set_input(-input);
    energy_state_block.run(mode);


    double vref = get_Vref_in_pu();
    input = vref-V-get_Dq()*reactive_integral_block.get_output();

    reactive_lead_lag_1.set_input(input);
    reactive_lead_lag_1.run(mode);

    input = reactive_lead_lag_1.get_output();
    reactive_lead_lag_2.set_input(input);
    reactive_lead_lag_2.run(mode);

    double iP = P/V;
    double Iqmax = sqrt(iacmax*iacmax-iP*iP);
    reactive_integral_block.set_upper_limit(Iqmax);
    reactive_integral_block.set_lower_limit(-Iqmax);

    input = reactive_lead_lag_2.get_output();
    reactive_integral_block.set_input(input);
    reactive_integral_block.run(mode);


    if(mode==UPDATE_MODE)
        set_flag_model_updated_as_true();
}


complex<double> ESTR0::get_terminal_complex_power_in_pu_based_on_mbase() const
{
    double P = get_terminal_active_power_in_pu_based_on_mbase();
    double Q = get_terminal_reactive_power_in_pu_based_on_mbase();

    return complex<double>(P,Q);
}

complex<double> ESTR0::get_terminal_complex_power_in_MVA() const
{
    return get_terminal_complex_power_in_pu_based_on_mbase()*get_mbase_in_MVA();
}

double ESTR0::get_terminal_active_power_in_pu_based_on_mbase() const
{
    double P = active_power_filter.get_output();
    double E = energy_state_block.get_output();
    if(P>0.0 and E<=0.0)
        P = 0.0;
    else
    {
        if(P<0.0 and E>=1.0)
            P = 0.0;
    }
    return P;
}

double ESTR0::get_terminal_active_power_in_MW() const
{
    return get_terminal_active_power_in_pu_based_on_mbase()*get_mbase_in_MVA();
}

double ESTR0::get_terminal_reactive_power_in_pu_based_on_mbase() const
{
    double Q = reactive_integral_block.get_output();
    return Q;
}

double ESTR0::get_terminal_reactive_power_in_MVar() const
{
    return get_terminal_reactive_power_in_pu_based_on_mbase()*get_mbase_in_MVA();
}


complex<double> ESTR0::get_terminal_complex_current_in_pu_based_on_mbase() const
{
    complex<double> S = get_terminal_complex_power_in_pu_based_on_mbase();
    complex<double> V = get_terminal_bus_complex_voltage_in_pu();

    complex<double> I = conj(S/V);

    return I;
}

complex<double> ESTR0::get_terminal_complex_current_in_pu_based_on_sbase() const
{
    complex<double> I = get_terminal_complex_current_in_pu_based_on_mbase();
    double mbase = get_mbase_in_MVA();

    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();

    return I*mbase/sbase;
}



complex<double> ESTR0::get_terminal_complex_current_in_kA() const
{
    complex<double> I = get_terminal_complex_current_in_pu_based_on_mbase();
    double mbase = get_mbase_in_MVA();

    ENERGY_STORAGE* estorage = (ENERGY_STORAGE*) get_device_pointer();
    size_t bus = estorage->get_energy_storage_bus();

    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    double vbase = psdb.get_bus_base_voltage_in_kV(bus);

    double ibase = mbase/(sqrt(3.0)*vbase);

    return I*ibase;
}

double ESTR0::get_terminal_current_in_kA() const
{
    return steps_fast_complex_abs(get_terminal_complex_current_in_kA());
}


double ESTR0::get_terminal_current_in_pu_based_on_mbase() const
{
    return steps_fast_complex_abs(get_terminal_complex_current_in_pu_based_on_mbase());
}

double ESTR0::get_terminal_current_in_pu_based_on_sbase() const
{
    return steps_fast_complex_abs(get_terminal_complex_current_in_pu_based_on_sbase());
}

double ESTR0::get_energy_state_in_pu() const
{
    return energy_state_block.get_output();
}


void ESTR0::check()
{
    ;
}

void ESTR0::report()
{
    ostringstream osstream;
    osstream<<get_standard_model_string();
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void ESTR0::save()
{
    ;
}

string ESTR0::get_standard_model_string() const
{
    ostringstream osstream;
    DEVICE_ID did = get_device_id();
    size_t bus = did.get_device_terminal().get_buses()[0];
    string identifier = did.get_device_identifier();

    osstream<<setw(8)<<bus<<", "
      <<"'"<<get_model_name()<<"', "
      <<"'"<<identifier<<"', "
      <<setw(8)<<setprecision(6)<<get_Tp1_in_s()<<", "
      <<setw(8)<<setprecision(6)<<get_Tp2_in_s()<<", "
      <<setw(8)<<setprecision(6)<<get_Tp3_in_s()<<", "
      <<setw(8)<<setprecision(6)<<get_Tp4_in_s()<<", "
      <<setw(8)<<setprecision(6)<<get_Kpp()<<", "
      <<setw(8)<<setprecision(6)<<get_Kip()<<", "
      <<setw(8)<<setprecision(6)<<get_Kdp()<<", "
      <<setw(8)<<setprecision(6)<<get_Tdp_in_s()<<", "
      <<setw(8)<<setprecision(6)<<get_Pmax_in_pu()<<", "
      <<setw(8)<<setprecision(6)<<get_Tr_in_s()<<", "
      <<setw(8)<<setprecision(6)<<get_Iacmax_in_pu()<<", "
      <<setw(8)<<setprecision(6)<<get_Kin_in_pu()<<", "
      <<setw(8)<<setprecision(6)<<get_Kout_in_pu()<<", "
      <<setw(8)<<setprecision(6)<<get_E0_in_pu()<<", "
      <<setw(8)<<setprecision(6)<<get_En_in_MWh()<<", "
      <<setw(8)<<setprecision(6)<<get_Tq1_in_s()<<", "
      <<setw(8)<<setprecision(6)<<get_Tq2_in_s()<<", "
      <<setw(8)<<setprecision(6)<<get_Tq3_in_s()<<", "
      <<setw(8)<<setprecision(6)<<get_Tq4_in_s()<<", "
      <<setw(8)<<setprecision(6)<<get_Kq()<<", "
      <<setw(8)<<setprecision(6)<<get_Dq()<<"  /";

    return osstream.str();
}

void ESTR0::prepare_model_data_table()
{
    clear_model_data_table();
    size_t i=0;
    add_model_data_name_and_index_pair("PMAX", i); i++;
    add_model_data_name_and_index_pair("ENOMINAL", i); i++;
    add_model_data_name_and_index_pair("E0", i); i++;
}

double ESTR0::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="PMAX")
        return get_Pmax_in_pu();
    if(par_name=="ENOMINAL")
        return get_En_in_MWh();
    if(par_name=="E0")
        return get_E0_in_pu();

    return 0.0;
}

void ESTR0::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="PMAX")
        return set_Pmax_in_pu(value);
    if(par_name=="ENOMINAL")
        return set_En_in_MWh(value);
    if(par_name=="E0")
        return set_E0_in_pu(value);
    return;
}


void ESTR0::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    size_t i=0;
    add_model_inernal_variable_name_and_index_pair("TERMINAL P IN PU", i); i++;
    add_model_inernal_variable_name_and_index_pair("TERMINAL P IN MW", i); i++;
    add_model_inernal_variable_name_and_index_pair("TERMINAL Q IN PU", i); i++;
    add_model_inernal_variable_name_and_index_pair("TERMINAL Q IN MVAR", i); i++;
    add_model_inernal_variable_name_and_index_pair("TERMINAL S IN PU", i); i++;
    add_model_inernal_variable_name_and_index_pair("TERMINAL S IN MVA", i); i++;
    add_model_inernal_variable_name_and_index_pair("TERMINAL CURRENT IN PU", i); i++;
    add_model_inernal_variable_name_and_index_pair("TERMINAL CURRENT IN KA", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE OF ENERGY", i); i++;
}

double ESTR0::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name == "TERMINAL P IN MW")
        return get_terminal_active_power_in_MW();
    if(var_name == "TERMINAL Q IN MVAR")
        return get_terminal_reactive_power_in_MVar();
    if(var_name == "STATE OF ENERGY")
        return get_energy_state_in_pu();

    return 0.0;
}


string ESTR0::get_dynamic_data_in_psse_format() const
{
    return get_standard_model_string();
}

string ESTR0::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string ESTR0::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}

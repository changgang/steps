#include "header/model/exciter_model/PSASPE1.h"
#include "header/basic/utility.h"
#include <cstdio>
#include <iostream>

using namespace std;

static vector<string> MODEL_VARIABLE_TABLE{ "EXCITATION VOLTAGE",      //0
                                            "VOLTAGE REFERENCE",      //1
                                            "COMPENSATED VOLTAGE",      //2
                                            "STABILIZING SIGNAL",      //3
                                            "STATE@SENSOR",     //4
                                            "STATE@REGULATOR",     //5
                                            "STATE@FEEDBACKER",     //6
                                            "STATE@EXCITER"     //7
                                            };

PSASPE1::PSASPE1()
{
    clear();
}

PSASPE1::~PSASPE1()
{
    ;
}
void PSASPE1::clear()
{
    sensor.set_limiter_type(NO_LIMITER);
    regulator.set_limiter_type(NO_LIMITER);
    exciter.set_limiter_type(WINDUP_LIMITER);
    feedbacker.set_limiter_type(NO_LIMITER);
}
void PSASPE1::copy_from_const_model(const PSASPE1& model)
{
    clear();
    this->set_KR(model.get_KR());
    this->set_TR_in_s(model.get_TR_in_s());
    this->set_KA(model.get_KA());
    this->set_TA_in_s(model.get_TA_in_s());
    this->set_TE_in_s(model.get_TE_in_s());
    this->set_Efdmax_in_pu(model.get_Efdmax_in_pu());
    this->set_Efdmin_in_pu(model.get_Efdmin_in_pu());
    this->set_KF(model.get_KF());
    this->set_TF_in_s(model.get_TF_in_s());
}

PSASPE1::PSASPE1(const PSASPE1& model) : EXCITER_MODEL()
{
    copy_from_const_model(model);
}

PSASPE1& PSASPE1::operator=(const PSASPE1& model)
{
    if(this==(&model)) return *this;

    copy_from_const_model(model);

    return (*this);
}

string PSASPE1::get_model_name() const
{
    return "PSASPE1";
}


void PSASPE1::set_KR(double K)
{
    sensor.set_K(K);
}

void PSASPE1::set_TR_in_s(double T)
{
    sensor.set_T_in_s(T);
}

void PSASPE1::set_KA(double K)
{
    regulator.set_K(K);
}

void PSASPE1::set_TA_in_s(double T)
{
    regulator.set_T_in_s(T);
}

void PSASPE1::set_VAmax_in_pu(double vmax)
{
    this->VAmax = vmax;
}

void PSASPE1::set_VAmin_in_pu(double vmin)
{
    this->VAmin = vmin;
}

void PSASPE1::set_TE_in_s(double T)
{
    exciter.set_T_in_s(T);
}
void PSASPE1::set_Efdmax_in_pu(double emax)
{
    exciter.set_upper_limit(emax);
    set_VAmax_in_pu(emax);
    set_VAmin_in_pu(-emax);
}

void PSASPE1::set_Efdmin_in_pu(double emin)
{
    exciter.set_lower_limit(emin);
}

void PSASPE1::set_KF(double K)
{
    feedbacker.set_K(K);
}

void PSASPE1::set_TF_in_s(double T)
{
    feedbacker.set_T_in_s(T);
}

double PSASPE1::get_KR() const
{
    return sensor.get_K();
}

double PSASPE1::get_TR_in_s() const
{
    return sensor.get_T_in_s();
}

double PSASPE1::get_KA() const
{
    return regulator.get_K();
}

double PSASPE1::get_TA_in_s() const
{
    return regulator.get_T_in_s();
}

double PSASPE1::get_VAmax_in_pu() const
{
    return get_Efdmax_in_pu();
}

double PSASPE1::get_VAmin_in_pu() const
{
    return -get_Efdmax_in_pu();
}

double PSASPE1::get_TE_in_s() const
{
    return exciter.get_T_in_s();
}

double PSASPE1::get_Efdmax_in_pu() const
{
    return exciter.get_upper_limit();
}

double PSASPE1::get_Efdmin_in_pu() const
{
    return exciter.get_lower_limit();
}

double PSASPE1::get_KF() const
{
    return feedbacker.get_K();
}

double PSASPE1::get_TF_in_s() const
{
    return feedbacker.get_T_in_s();
}

bool PSASPE1::setup_model_with_steps_string(string data)
{
    return false;
}

bool PSASPE1::setup_model_with_psse_string(string data)
{
    bool is_successful = false;
    vector<string> dyrdata = split_string(data,",");
    if(dyrdata.size()<12)
        return is_successful;

    string model_name = get_string_data(dyrdata[1],"");
    if(model_name!="PSASPE1")
        return is_successful;


    double kr, tr, ka, ta, te, kf, tf, efdmax, efdmin;

    size_t i=3;
    kr = get_double_data(dyrdata[i],"1.0"); i++;
    tr = get_double_data(dyrdata[i],"0.0"); i++;
    ka = get_double_data(dyrdata[i],"0.0"); i++;
    ta = get_double_data(dyrdata[i],"0.0"); i++;
    te = get_double_data(dyrdata[i],"0.0"); i++;
    kf = get_double_data(dyrdata[i],"0.0"); i++;
    tf = get_double_data(dyrdata[i],"0.0"); i++;
    efdmax = get_double_data(dyrdata[i],"0.0"); i++;
    efdmin = get_double_data(dyrdata[i],"0.0"); i++;

    set_KR(kr);
    set_TR_in_s(tr);
    set_KA(ka);
    set_TA_in_s(ta);
    set_TE_in_s(te);
    set_KF(kf);
    set_TF_in_s(tf);
    set_Efdmax_in_pu(efdmax);
    set_Efdmax_in_pu(efdmin);

    is_successful = true;

    return is_successful;
}

bool PSASPE1::setup_model_with_bpa_string(string data)
{
    return false;
}


void PSASPE1::initialize()
{
    if(is_model_initialized())
        return;

    GENERATOR* generator = (GENERATOR*) get_device_pointer();
    if(generator==NULL)
        return;

    SYNC_GENERATOR_MODEL* gen_model = generator->get_sync_generator_model();
    if(gen_model==NULL)
        return;

    if(not gen_model->is_model_initialized())
        gen_model->initialize();

    double Ecomp = get_compensated_voltage_in_pu();

    set_voltage_reference_in_pu(Ecomp);

    sensor.set_output(0.0);
    sensor.initialize();

    double Efd =  get_initial_excitation_voltage_in_pu_from_sync_generator_model();
    Efd0 = Efd;

    exciter.set_output(Efd);
    exciter.initialize();

    feedbacker.set_input(Efd);
    feedbacker.initialize();

    regulator.set_output(0.0);
    regulator.initialize();

    set_flag_model_initialized_as_true();
}

void PSASPE1::run(DYNAMIC_MODE mode)
{
    GENERATOR* generator = (GENERATOR*) get_device_pointer();
    if(generator==NULL)
        return;

    double Ecomp = get_compensated_voltage_in_pu();
    double Vref = get_voltage_reference_in_pu();
    double Vs = get_stabilizing_signal_in_pu();

    double input = Vref-Ecomp;
    sensor.set_input(input);
    sensor.run(mode);

    input = sensor.get_output()+Vs-feedbacker.get_output();
    regulator.set_input(input);
    regulator.run(mode);

    input = Efd0+regulator.get_output();
    double VAmax = get_VAmax_in_pu();
    double VAmin = get_VAmin_in_pu();
    if(input>VAmax) input = VAmax;
    if(input<VAmin) input = VAmin;

    exciter.set_input(input);
    exciter.run(mode);

    double Efd = get_excitation_voltage_in_pu();

    feedbacker.set_input(Efd);
    feedbacker.run(mode);
    //cout<<"Ecomp="<<Ecomp<<", Vref="<<Vref<<", Vs="<<Vs<<", Efd="<<exciter.get_output()<<endl;

    if(mode == UPDATE_MODE)
        set_flag_model_updated_as_true();
}

double PSASPE1::get_excitation_voltage_in_pu() const
{
    double Efd = exciter.get_output();
    double Efdmax = get_Efdmax_in_pu();
    double Efdmin = get_Efdmin_in_pu();
    if(Efd>Efdmax) Efd = Efdmax;
    if(Efd<Efdmin) Efd = Efdmin;

    return Efd;
}
void PSASPE1::check()
{
    ;
}

void PSASPE1::report()
{
    ostringstream sstream;
    sstream<<get_standard_model_string();
    show_information_with_leading_time_stamp(sstream);
}

void PSASPE1::save()
{
    ;
}

string PSASPE1::get_standard_model_string() const
{
    ostringstream sstream;
    GENERATOR* gen = (GENERATOR*) get_device_pointer();
    size_t bus = gen->get_generator_bus();
    string identifier= gen->get_identifier();
    double KR = get_KR();
    double TR = get_TR_in_s();
    double KA = get_KA();
    double TA = get_TA_in_s();
    double Efdmax = get_Efdmax_in_pu();
    double Efdmin = get_Efdmin_in_pu();
    double TE = get_TE_in_s();
    double KF = get_KF();
    double TF = get_TF_in_s();

    sstream<<setw(8)<<bus<<", "
      <<"'"<<get_model_name()<<"', "
      <<"'"<<identifier<<"', "
      <<setw(8)<<setprecision(6)<<KR<<", "
      <<setw(8)<<setprecision(6)<<TR<<", "
      <<setw(8)<<setprecision(6)<<KA<<", "
      <<setw(8)<<setprecision(6)<<TA<<", "
      <<setw(8)<<setprecision(6)<<TE<<", "
      <<setw(8)<<setprecision(6)<<Efdmax<<", "
      <<setw(8)<<setprecision(6)<<Efdmin<<", "
      <<setw(8)<<setprecision(6)<<KF<<", "
      <<setw(8)<<setprecision(6)<<TF<<"  /";
    return sstream.str();
}


size_t PSASPE1::get_variable_index_from_variable_name(string var_name)
{
    return MODEL::get_variable_index_from_variable_name(var_name, MODEL_VARIABLE_TABLE);
}

string PSASPE1::get_variable_name_from_variable_index(size_t var_index)
{
    return MODEL::get_variable_name_from_variable_index(var_index, MODEL_VARIABLE_TABLE);
}

double PSASPE1::get_variable_with_index(size_t var_index)
{
    string var_name = get_variable_name_from_variable_index(var_index);
    return get_variable_with_name(var_name);
}

double PSASPE1::get_variable_with_name(string var_name)
{
    if(var_name == "EXCITATION VOLTAGE")
        return exciter.get_output();

    if(var_name == "VOLTAGE REFERENCE")
        return get_voltage_reference_in_pu();

    if(var_name == "COMPENSATED VOLTAGE")
        return get_compensated_voltage_in_pu();

    if(var_name == "STABILIZING SIGNAL")
        return get_stabilizing_signal_in_pu();

    if(var_name == "STATE@SENSOR")
        return sensor.get_state();

    if(var_name == "STATE@REGULATOR")
        return regulator.get_state();

    if(var_name == "STATE@FEEDBACKER")
        return feedbacker.get_state();

    if(var_name == "STATE@EXCITER")
        return exciter.get_state();

    return 0.0;
}


string PSASPE1::get_dynamic_data_in_psse_format() const
{
    return "";
}

string PSASPE1::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string PSASPE1::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}

#include "header/model/sg_models/exciter_model/PSASPE13.h"
#include "header/basic/utility.h"
#include <cstdio>
#include <istream>
#include <iostream>

using namespace std;

static vector<string> MODEL_VARIABLE_TABLE{ "EXCITATION VOLTAGE",      //0
                                            "VOLTAGE REFERENCE",      //1
                                            "PSASPE13ENSATED VOLTAGE",      //2
                                            "STABILIZING SIGNAL",      //3
                                            "STATE@SENSOR",     //4
                                            "STATE@TUNER",     //4
                                            "STATE@REGULATOR",     //5
                                            "STATE@FEEDBACKER",     //6
                                            };

PSASPE13::PSASPE13()
{
    clear();
}

PSASPE13::~PSASPE13()
{
    ;
}
void PSASPE13::clear()
{
    sensor.set_limiter_type(NO_LIMITER);
    sensor.set_K(1.0);
    tuner.set_limiter_type(NO_LIMITER);
    regulator.set_limiter_type(NON_WINDUP_LIMITER);
    feedbacker.set_limiter_type(NO_LIMITER);
}
void PSASPE13::copy_from_const_model(const PSASPE13& model)
{
    clear();
    this->set_TR_in_s(model.get_TR_in_s());
    this->set_VImax_in_pu(model.get_VImax_in_pu());
    this->set_VImin_in_pu(model.get_VImin_in_pu());
    this->set_TC_in_s(model.get_TC_in_s());
    this->set_TB_in_s(model.get_TB_in_s());
    this->set_KA(model.get_KA());
    this->set_TA_in_s(model.get_TA_in_s());
    this->set_VRmax_in_pu(model.get_VRmax_in_pu());
    this->set_VRmin_in_pu(model.get_VRmin_in_pu());
    this->set_KF(model.get_KF());
    this->set_TF_in_s(model.get_TF_in_s());
    this->set_Efdmax_in_pu(model.get_Efdmax_in_pu());
    this->set_Efdmin_in_pu(model.get_Efdmin_in_pu());
    this->set_KC(model.get_KC());
}

PSASPE13::PSASPE13(const PSASPE13& model) : EXCITER_MODEL()
{
    copy_from_const_model(model);
}

PSASPE13& PSASPE13::operator=(const PSASPE13& model)
{
    if(this==(&model)) return *this;

    copy_from_const_model(model);

    return (*this);
}

string PSASPE13::get_model_name() const
{
    return "PSASPE13";
}

double PSASPE13::get_double_data_with_index(size_t index) const
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input index is provided: "<<index;
    show_information_with_leading_time_stamp(osstream);
    return 0.0;
}

double PSASPE13::get_double_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="")
        return 0.0;

    return 0.0;
}

void PSASPE13::set_double_data_with_index(size_t index, double value)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input (index, value) is provided: ("<<index<<", "<<value<<").";
    show_information_with_leading_time_stamp(osstream);
    return;
}

void PSASPE13::set_double_data_with_name(string par_name, double value)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input (par_name, value) is provided: ("<<par_name<<", "<<value<<").";
    show_information_with_leading_time_stamp(osstream);
    return;
}

void PSASPE13::set_TR_in_s(double T)
{
    sensor.set_T_in_s(T);
}

void PSASPE13::set_VImax_in_pu(double vmax)
{
    this->VImax = vmax;
}

void PSASPE13::set_VImin_in_pu(double vmin)
{
    this->VImin = vmin;
}

void PSASPE13::set_TC_in_s(double T)
{
    tuner.set_T1_in_s(T);
}

void PSASPE13::set_TB_in_s(double T)
{
    tuner.set_T2_in_s(T);
}

void PSASPE13::set_KA(double K)
{
    regulator.set_K(K);
}

void PSASPE13::set_TA_in_s(double T)
{
    regulator.set_T_in_s(T);
}

void PSASPE13::set_VRmax_in_pu(double vmax)
{
    regulator.set_upper_limit(vmax);
}

void PSASPE13::set_VRmin_in_pu(double vmin)
{
    regulator.set_lower_limit(vmin);
}

void PSASPE13::set_KF(double K)
{
    feedbacker.set_K(K);
}

void PSASPE13::set_TF_in_s(double T)
{
    feedbacker.set_T_in_s(T);
}

void PSASPE13::set_Efdmax_in_pu(double emax)
{
    this->Efdmax = emax;
}

void PSASPE13::set_Efdmin_in_pu(double emin)
{
    this->Efdmin = emin;
}

void PSASPE13::set_KC(double K)
{
    this->KC = K;
}

double PSASPE13::get_TR_in_s() const
{
    return sensor.get_T_in_s();
}

double PSASPE13::get_VImax_in_pu() const
{
    return VImax;
}

double PSASPE13::get_VImin_in_pu() const
{
    return VImin;
}

double PSASPE13::get_TC_in_s() const
{
    return tuner.get_T1_in_s();
}

double PSASPE13::get_TB_in_s() const
{
    return tuner.get_T2_in_s();
}

double PSASPE13::get_KA() const
{
    return regulator.get_K();
}

double PSASPE13::get_TA_in_s() const
{
    return regulator.get_T_in_s();
}

double PSASPE13::get_VRmax_in_pu() const
{
    return regulator.get_upper_limit();
}

double PSASPE13::get_VRmin_in_pu() const
{
    return regulator.get_lower_limit();
}

double PSASPE13::get_KF() const
{
    return feedbacker.get_K();
}

double PSASPE13::get_TF_in_s() const
{
    return feedbacker.get_T_in_s();
}

double PSASPE13::get_Efdmax_in_pu() const
{
    return Efdmax;
}

double PSASPE13::get_Efdmin_in_pu() const
{
    return Efdmin;
}

double PSASPE13::get_KC() const
{
    return KC;
}


bool PSASPE13::setup_model_with_steps_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    show_information_with_leading_time_stamp(osstream);
    return false;
}

bool PSASPE13::setup_model_with_psse_string(string data)
{
    bool is_successful = false;
    vector<string> dyrdata = split_string(data,",");
    if(dyrdata.size()<17)
        return is_successful;

    string model_name = get_string_data(dyrdata[1],"");
    if(model_name!=get_model_name())
        return is_successful;

    size_t i=3;
    double tr = get_double_data(dyrdata[i],"0.0"); i++;
    double vimax = get_double_data(dyrdata[i],"0.0"); i++;
    double vimin = get_double_data(dyrdata[i],"0.0"); i++;
    double tc = get_double_data(dyrdata[i],"0.0"); i++;
    double tb = get_double_data(dyrdata[i],"0.0"); i++;
    double ka = get_double_data(dyrdata[i],"0.0"); i++;
    double ta = get_double_data(dyrdata[i],"0.0"); i++;
    double vrmax = get_double_data(dyrdata[i],"0.0"); i++;
    double vrmin = get_double_data(dyrdata[i],"0.0"); i++;
    double kf = get_double_data(dyrdata[i],"0.0"); i++;
    double tf = get_double_data(dyrdata[i],"0.0"); i++;
    double efdmax = get_double_data(dyrdata[i],"0.0"); i++;
    double efdmin = get_double_data(dyrdata[i],"0.0"); i++;
    double kc = get_double_data(dyrdata[i],"0.0"); i++;

    set_TR_in_s(tr);
    set_VImax_in_pu(vimax);
    set_VImin_in_pu(vimin);
    set_TC_in_s(tc);
    set_TB_in_s(tb);
    set_KA(ka);
    set_TA_in_s(ta);
    set_VRmax_in_pu(vrmax);
    set_VRmin_in_pu(vrmin);
    set_KF(kf);
    set_TF_in_s(tf);
    set_Efdmax_in_pu(efdmax);
    set_Efdmax_in_pu(efdmin);
    set_KC(kc);

    is_successful = true;

    return is_successful;
}

bool PSASPE13::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    show_information_with_leading_time_stamp(osstream);
    return false;
}

void PSASPE13::initialize()
{
    if(is_model_initialized())
        return;

    GENERATOR* generator = get_generator_pointer();
    if(generator==NULL)
        return;

    SYNC_GENERATOR_MODEL* gen_model = generator->get_sync_generator_model();
    if(gen_model==NULL)
        return;

    if(not gen_model->is_model_initialized())
        gen_model->initialize();

    double Ecomp = get_compensated_voltage_in_pu();
    double Efd =  get_initial_excitation_voltage_in_pu_from_sync_generator_model();

    regulator.set_output(Efd);
    regulator.initialize();
    double output = regulator.get_input();

    feedbacker.set_input(Efd);
    feedbacker.initialize();

    tuner.set_output(output);
    tuner.initialize();

    sensor.set_output(Ecomp);
    sensor.initialize();

    double Vref = Ecomp+tuner.get_input();

    set_voltage_reference_in_pu(Vref);

    set_flag_model_initialized_as_true();
}

void PSASPE13::run(DYNAMIC_MODE mode)
{
    GENERATOR* generator = get_generator_pointer();
    if(generator==NULL)
        return;

    double Ecomp = get_compensated_voltage_in_pu();
    double Vref = get_voltage_reference_in_pu();
    double Vs = get_stabilizing_signal_in_pu();

    sensor.set_input(Ecomp);
    sensor.run(mode);

    double input = Vref-sensor.get_output()+Vs+feedbacker.get_output();
    double VImax = get_VImax_in_pu();
    double VImin = get_VImin_in_pu();
    if(input>VImax) input = VImax;
    if(input<VImin) input = VImin;

    tuner.set_input(input);
    tuner.run(mode);

    regulator.set_input(tuner.get_output());
    regulator.run(mode);

    feedbacker.set_input(regulator.get_output());
    feedbacker.run(mode);
    //cout<<"Ecomp="<<Ecomp<<", Vref="<<Vref<<", Vs="<<Vs<<", Efd="<<exciter.get_output()<<endl;

    if(mode == UPDATE_MODE)
        set_flag_model_updated_as_true();
}

double PSASPE13::get_excitation_voltage_in_pu() const
{
    GENERATOR* generator = get_generator_pointer();
    if(generator==NULL)
        return 0.0;

    SYNC_GENERATOR_MODEL* gen_model = generator->get_sync_generator_model();
    if(gen_model==NULL)
        return 0.0;

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    size_t bus = generator->get_generator_bus();
    double Vt = psdb->get_bus_voltage_in_pu(bus);
    double Ifd = gen_model->get_field_current_in_pu_based_on_mbase();

    double Efd = regulator.get_output();
    double Efdmax = get_Efdmax_in_pu();
    double Efdmin = get_Efdmin_in_pu();
    double KC = get_KC();

    Efdmax = Vt*Efdmax-KC*Ifd;
    Efdmin= Vt*Efdmin-KC*Ifd;

    if(Efd>Efdmax) Efd = Efdmax;
    if(Efd<Efdmin) Efd = Efdmin;

    return Efd;
}
void PSASPE13::check()
{
    ;
}

void PSASPE13::report()
{
    ostringstream osstream;
    osstream<<get_standard_model_string();
    show_information_with_leading_time_stamp(osstream);
}

void PSASPE13::save()
{
    ;
}

string PSASPE13::get_standard_model_string() const
{
    ostringstream osstream;
    GENERATOR* gen = get_generator_pointer();
    size_t bus = gen->get_generator_bus();
    string identifier= gen->get_identifier();

    double TR = get_TR_in_s();
    double VImax = get_VImax_in_pu();
    double VImin = get_VImin_in_pu();
    double KA = get_KA();
    double TA = get_TA_in_s();
    double VRmax = get_VRmax_in_pu();
    double VRmin = get_VRmin_in_pu();
    double KF = get_KF();
    double TF = get_TF_in_s();
    double Efdmax = get_Efdmax_in_pu();
    double Efdmin = get_Efdmin_in_pu();
    double KC = get_KC();

    osstream<<setw(8)<<bus<<", "
      <<"'"<<get_model_name()<<"', "
      <<"'"<<identifier<<"', "
      <<setw(8)<<setprecision(6)<<TR<<", "
      <<setw(8)<<setprecision(6)<<VImax<<", "
      <<setw(8)<<setprecision(6)<<VImin<<", "
      <<setw(8)<<setprecision(6)<<KA<<", "
      <<setw(8)<<setprecision(6)<<TA<<", "
      <<setw(8)<<setprecision(6)<<VRmax<<", "
      <<setw(8)<<setprecision(6)<<VRmin<<", "
      <<setw(8)<<setprecision(6)<<KF<<", "
      <<setw(8)<<setprecision(6)<<TF<<", "
      <<setw(8)<<setprecision(6)<<Efdmax<<", "
      <<setw(8)<<setprecision(6)<<Efdmin<<", "
      <<setw(8)<<setprecision(6)<<KC<<"  /";

    return osstream.str();
}


size_t PSASPE13::get_variable_index_from_variable_name(string var_name)
{
    return MODEL::get_variable_index_from_variable_name(var_name, MODEL_VARIABLE_TABLE);
}

string PSASPE13::get_variable_name_from_variable_index(size_t var_index)
{
    return MODEL::get_variable_name_from_variable_index(var_index, MODEL_VARIABLE_TABLE);
}

double PSASPE13::get_variable_with_index(size_t var_index)
{
    string var_name = get_variable_name_from_variable_index(var_index);
    return get_variable_with_name(var_name);
}

double PSASPE13::get_variable_with_name(string var_name)
{
    if(var_name == "EXCITATION VOLTAGE")
        return get_excitation_voltage_in_pu();

    if(var_name == "VOLTAGE REFERENCE")
        return get_voltage_reference_in_pu();

    if(var_name == "PSASPE13ENSATED VOLTAGE")
        return get_compensated_voltage_in_pu();

    if(var_name == "STABILIZING SIGNAL")
        return get_stabilizing_signal_in_pu();

    if(var_name == "STATE@SENSOR")
        return sensor.get_state();

    if(var_name == "STATE@TUNER")
        return tuner.get_state();

    if(var_name == "STATE@REGULATOR")
        return regulator.get_state();

    if(var_name == "STATE@FEEDBACKER")
        return feedbacker.get_state();

    return 0.0;
}


string PSASPE13::get_dynamic_data_in_psse_format() const
{
    return "";
}

string PSASPE13::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string PSASPE13::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}

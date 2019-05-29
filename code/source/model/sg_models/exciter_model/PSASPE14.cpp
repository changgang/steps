#include "header/model/sg_models/exciter_model/PSASPE14.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <cstdio>
#include <istream>
#include <iostream>

using namespace std;
PSASPE14::PSASPE14()
{
    clear();
}

PSASPE14::~PSASPE14()
{
    ;
}
void PSASPE14::clear()
{
    prepare_model_data_table();
    prepare_model_internal_variable_table();

    sensor.set_K(1.0);
    voltage_pi.set_limiter_type(NON_WINDUP_LIMITER);
    current_pi.set_limiter_type(NON_WINDUP_LIMITER);
}
void PSASPE14::copy_from_const_model(const PSASPE14& model)
{
    clear();

    set_Xc_in_s(model.get_Xc_in_s());
    set_Tr_in_s(model.get_Tr_in_s());
    set_Ka(model.get_Ka());
    set_Ta_in_s(model.get_Ta_in_s());
    set_Kp(model.get_Kp());
    set_Ki(model.get_Ki());
    set_Vrmax_in_pu(model.get_Vrmax_in_pu());
    set_Vrmin_in_pu(model.get_Vrmin_in_pu());
    set_IKp(model.get_IKp());
    set_IKi(model.get_IKi());
    set_Vfmax_in_pu(model.get_Vfmax_in_pu());
    set_Vfmin_in_pu(model.get_Vfmin_in_pu());
    set_Kt(model.get_Kt());
    set_Tt_in_s(model.get_Tt_in_s());
    set_Kifd(model.get_Kifd());
    set_Tifd_in_s(model.get_Tifd_in_s());
    set_Efdmax_in_pu(model.get_Efdmax_in_pu());
    set_Efdmin_in_pu(model.get_Efdmin_in_pu());
    set_Kc(model.get_Kc());
}

PSASPE14::PSASPE14(const PSASPE14& model) : EXCITER_MODEL()
{
    copy_from_const_model(model);
}

PSASPE14& PSASPE14::operator=(const PSASPE14& model)
{
    if(this==(&model)) return *this;

    copy_from_const_model(model);

    return (*this);
}

string PSASPE14::get_model_name() const
{
    return "PSASPE14";
}

void PSASPE14::set_Xc_in_s(double X)
{
    Xc = X;
}

void PSASPE14::set_Tr_in_s(double T)
{
    sensor.set_T_in_s(T);
}

void PSASPE14::set_Ka(double K)
{
    regulator.set_K(K);
}

void PSASPE14::set_Ta_in_s(double T)
{
    regulator.set_T_in_s(T);
}

void PSASPE14::set_Kp(double K)
{
    voltage_pi.set_Kp(K);
}

void PSASPE14::set_Ki(double K)
{
    voltage_pi.set_Ki(K);
}

void PSASPE14::set_Vrmax_in_pu(double vmax)
{
    voltage_pi.set_upper_limit(vmax);
}

void PSASPE14::set_Vrmin_in_pu(double vmin)
{
    voltage_pi.set_lower_limit(vmin);
}

void PSASPE14::set_IKp(double K)
{
    current_pi.set_Kp(K);
}

void PSASPE14::set_IKi(double K)
{
    current_pi.set_Ki(K);
}

void PSASPE14::set_Vfmax_in_pu(double vmax)
{
    current_pi.set_upper_limit(vmax);
}

void PSASPE14::set_Vfmin_in_pu(double vmin)
{
    current_pi.set_lower_limit(vmin);
}

void PSASPE14::set_Kt(double K)
{
    rectifier.set_K(K);
}

void PSASPE14::set_Tt_in_s(double T)
{
    rectifier.set_T_in_s(T);
}

void PSASPE14::set_Kifd(double K)
{
    ifd_feedback.set_K(K);
}

void PSASPE14::set_Tifd_in_s(double T)
{
    ifd_feedback.set_T_in_s(T);
}

void PSASPE14::set_Efdmax_in_pu(double emax)
{
    Efdmax = emax;
}

void PSASPE14::set_Efdmin_in_pu(double emin)
{
    Efdmin = emin;
}

void PSASPE14::set_Kc(double K)
{
    Kc = K;
}

double PSASPE14::get_Xc_in_s() const
{
    return Xc;
}

double PSASPE14::get_Tr_in_s() const
{
    return sensor.get_T_in_s();
}

double PSASPE14::get_Ka() const
{
    return regulator.get_K();
}

double PSASPE14::get_Ta_in_s() const
{
    return regulator.get_T_in_s();
}

double PSASPE14::get_Kp() const
{
    return voltage_pi.get_Kp();
}

double PSASPE14::get_Ki() const
{
    return voltage_pi.get_Ki();
}

double PSASPE14::get_Vrmax_in_pu() const
{
    return voltage_pi.get_upper_limit();
}

double PSASPE14::get_Vrmin_in_pu() const
{
    return voltage_pi.get_lower_limit();
}

double PSASPE14::get_IKp() const
{
    return current_pi.get_Kp();
}

double PSASPE14::get_IKi() const
{
    return current_pi.get_Ki();
}

double PSASPE14::get_Vfmax_in_pu() const
{
    return current_pi.get_upper_limit();
}

double PSASPE14::get_Vfmin_in_pu() const
{
    return current_pi.get_lower_limit();
}

double PSASPE14::get_Kt() const
{
    return rectifier.get_K();
}

double PSASPE14::get_Tt_in_s() const
{
    return rectifier.get_T_in_s();
}

double PSASPE14::get_Kifd() const
{
    return ifd_feedback.get_K();
}

double PSASPE14::get_Tifd_in_s() const
{
    return ifd_feedback.get_T_in_s();
}

double PSASPE14::get_Efdmax_in_pu() const
{
    return Efdmax;
}

double PSASPE14::get_Efdmin_in_pu() const
{
    return Efdmin;
}

double PSASPE14::get_Kc() const
{
    return Kc;
}

bool PSASPE14::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()<17)
        return is_successful;

    string model_name = get_string_data(data[0],"");
    if(model_name!=get_model_name())
        return is_successful;

    size_t i=3;
    double tr = get_double_data(data[i],"0.0"); i++;
    double vimax = get_double_data(data[i],"0.0"); i++;
    double vimin = get_double_data(data[i],"0.0"); i++;
    double tc = get_double_data(data[i],"0.0"); i++;
    double tb = get_double_data(data[i],"0.0"); i++;
    double ka = get_double_data(data[i],"0.0"); i++;
    double ta = get_double_data(data[i],"0.0"); i++;
    double vrmax = get_double_data(data[i],"0.0"); i++;
    double vrmin = get_double_data(data[i],"0.0"); i++;
    double kf = get_double_data(data[i],"0.0"); i++;
    double tf = get_double_data(data[i],"0.0"); i++;
    double efdmax = get_double_data(data[i],"0.0"); i++;
    double efdmin = get_double_data(data[i],"0.0"); i++;
    double kc = get_double_data(data[i],"0.0"); i++;


    is_successful = true;

    return is_successful;
}

bool PSASPE14::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool PSASPE14::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void PSASPE14::set_block_toolkit()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    sensor.set_toolkit(toolkit);
    regulator.set_toolkit(toolkit);
    rectifier.set_toolkit(toolkit);
    ifd_feedback.set_toolkit(toolkit);
    voltage_pi.set_toolkit(toolkit);
    current_pi.set_toolkit(toolkit);
}

void PSASPE14::initialize()
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

    set_block_toolkit();

    double Ecomp = get_compensated_voltage_in_pu();
    double Efd =  get_initial_excitation_voltage_in_pu_from_sync_generator_model();

    regulator.set_output(Efd);
    regulator.initialize();
    double output = regulator.get_input();

    set_flag_model_initialized_as_true();
}

void PSASPE14::run(DYNAMIC_MODE mode)
{
    GENERATOR* generator = get_generator_pointer();
    if(generator==NULL)
        return;

    double Ecomp = get_compensated_voltage_in_pu();
    double Vref = get_voltage_reference_in_pu();
    double Vs = get_stabilizing_signal_in_pu();

    //cout<<"Ecomp="<<Ecomp<<", Vref="<<Vref<<", Vs="<<Vs<<", Efd="<<exciter.get_output()<<endl;

    if(mode == UPDATE_MODE)
        set_flag_model_updated_as_true();
}

double PSASPE14::get_excitation_voltage_in_pu() const
{
    GENERATOR* generator = get_generator_pointer();
    if(generator==NULL)
        return 0.0;

    SYNC_GENERATOR_MODEL* gen_model = generator->get_sync_generator_model();
    if(gen_model==NULL)
        return 0.0;

    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    size_t bus = generator->get_generator_bus();
    double Vt = psdb.get_bus_voltage_in_pu(bus);
    double Ifd = gen_model->get_field_current_in_pu_based_on_mbase();


    return 0.0;
}
void PSASPE14::check()
{
    ;
}

void PSASPE14::report()
{
    ostringstream osstream;
    osstream<<get_standard_model_string();
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void PSASPE14::save()
{
    ;
}

string PSASPE14::get_standard_model_string() const
{
    ostringstream osstream;
    GENERATOR* gen = get_generator_pointer();
    size_t bus = gen->get_generator_bus();
    string identifier= gen->get_identifier();


    return osstream.str();
}

void PSASPE14::prepare_model_data_table()
{
    clear_model_data_table();
    size_t i=0;
    add_model_data_name_and_index_pair("A", i); i++;
}

double PSASPE14::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="A")
        return 0.0;

    return 0.0;
}

void PSASPE14::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="A")
        return;

    return;
}


void PSASPE14::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    size_t i=0;
    add_model_inernal_variable_name_and_index_pair("STATE@SENSOR", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@TUNER", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@REGULATOR", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@FEEDBACKER", i); i++;
}

double PSASPE14::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name == "STATE@SENSOR")
        return sensor.get_state();

    if(var_name == "STATE@TUNER")
        return 0.0;

    if(var_name == "STATE@REGULATOR")
        return regulator.get_state();

    if(var_name == "STATE@FEEDBACKER")
        return 0.0;

    return 0.0;
}


string PSASPE14::get_dynamic_data_in_psse_format() const
{
    return "";
}

string PSASPE14::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string PSASPE14::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}

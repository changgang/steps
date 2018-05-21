#include "header/model/sg_models/turbine_governor_model/IEEEG3.h"
#include "header/basic/utility.h"
#include <iostream>
using namespace std;
static vector<string> MODEL_VARIABLE_TABLE{ "GENERATOR MECHANICAL POWER IN PU", //0
                                            "GENERATOR MECHANICAL POWER IN MW",      //1
                                            "MECHANICAL POWER REFERENCE IN PU",      //2
                                            "GENERATOR ROTOR SPEED DEVIATION IN PU",     //3
                                            "STATE@GOVERNOR",     //4
                                            "STATE@SERVO MOTOR",       //5
                                            "STATE@FEEDBACKER",       //6
                                            "STATE@WATER HAMMER",       //7
                                            };//8

IEEEG3::IEEEG3()
{
    clear();
}

IEEEG3::~IEEEG3()
{
    clear();
}
void IEEEG3::clear()
{
    governor.set_limiter_type(NON_WINDUP_LIMITER);
    servo_motor.set_limiter_type(NON_WINDUP_LIMITER);
    servo_motor.set_T_in_s(1.0);
    feedbacker.set_limiter_type(NO_LIMITER);
    water_hammer.set_limiter_type(NO_LIMITER);
    set_a11(0.5);
    set_a13(1.5);
    set_a21(1.0);
    set_a23(1.0);
}
void IEEEG3::copy_from_const_model(const IEEEG3& model)
{
    clear();

    this->set_TG_in_s(model.get_TG_in_s());
    this->set_TP_in_s(model.get_TP_in_s());
    this->set_Uo_in_pu(model.get_Uo_in_pu());
    this->set_Uc_in_pu(model.get_Uc_in_pu());
    this->set_Pmax_in_pu(model.get_Pmax_in_pu());
    this->set_Pmin_in_pu(model.get_Pmin_in_pu());
    this->set_sigma(model.get_sigma());
    this->set_delta(model.get_delta());
    this->set_TR_in_s(model.get_TR_in_s());
    this->set_TW_in_s(model.get_TW_in_s());
    this->set_a11(model.get_a11());
    this->set_a13(model.get_a13());
    this->set_a21(model.get_a21());
    this->set_a23(model.get_a23());
}

IEEEG3::IEEEG3(const IEEEG3&model) : TURBINE_GOVERNOR_MODEL()
{
    copy_from_const_model(model);
}

IEEEG3& IEEEG3::operator=(const IEEEG3& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

string IEEEG3::get_model_name() const
{
    return "IEEEG3";
}

double IEEEG3::get_double_data_with_index(size_t index) const
{
    return 0.0;
}

double IEEEG3::get_double_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="PMAX")
        return get_Pmax_in_pu();
    if(par_name=="PMIN")
        return get_Pmin_in_pu();

    return 0.0;
}

void IEEEG3::set_double_data_with_index(size_t index, double value)
{
    if(index==0)
        return;
}

void IEEEG3::set_double_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="PMAX")
        return set_Pmax_in_pu(value);
    if(par_name=="PMIN")
        return set_Pmin_in_pu(value);
}

void IEEEG3::set_TG_in_s(double T)
{
    governor.set_K(1.0/T);
}

void IEEEG3::set_TP_in_s(double T)
{
    governor.set_T_in_s(T);
}

void IEEEG3::set_Uo_in_pu(double U)
{
    governor.set_upper_limit(U);
}

void IEEEG3::set_Uc_in_pu(double U)
{
    governor.set_lower_limit(U);
}

void IEEEG3::set_Pmax_in_pu(double P)
{
    servo_motor.set_upper_limit(P);
}

void IEEEG3::set_Pmin_in_pu(double P)
{
    servo_motor.set_lower_limit(P);
}

void IEEEG3::set_sigma(double sigma)
{
    this->sigma = sigma;
}

void IEEEG3::set_delta(double delta)
{
    this->delta = delta;
}

void IEEEG3::set_TR_in_s(double T)
{
    feedbacker.set_T_in_s(T);
}

void IEEEG3::set_TW_in_s(double T)
{
    this->TW = T;
}

void IEEEG3::set_a11(double a)
{
    this->a11 = a;
}

void IEEEG3::set_a13(double a)
{
    this->a13 = a;
}

void IEEEG3::set_a21(double a)
{
    this->a21 = a;
}

void IEEEG3::set_a23(double a)
{
    this->a23 = a;
}

double IEEEG3::get_TG_in_s() const
{
    return 1.0/governor.get_K();
}

double IEEEG3::get_TP_in_s() const
{
    return governor.get_T_in_s();
}

double IEEEG3::get_Uo_in_pu() const
{
    return governor.get_upper_limit();
}

double IEEEG3::get_Uc_in_pu() const
{
    return governor.get_lower_limit();
}

double IEEEG3::get_Pmax_in_pu() const
{
    return servo_motor.get_upper_limit();
}

double IEEEG3::get_Pmin_in_pu() const
{
    return servo_motor.get_lower_limit();
}

double IEEEG3::get_sigma() const
{
    return sigma;
}

double IEEEG3::get_delta() const
{
    return delta;
}

double IEEEG3::get_TR_in_s() const
{
    return feedbacker.get_T_in_s();
}

double IEEEG3::get_TW_in_s() const
{
    return TW;
}

double IEEEG3::get_a11() const
{
    return a11;
}

double IEEEG3::get_a13() const
{
    return a13;
}

double IEEEG3::get_a21() const
{
    return a21;
}

double IEEEG3::get_a23() const
{
    return a23;
}


bool IEEEG3::setup_model_with_steps_string(string data)
{
    return false;
}

bool IEEEG3::setup_model_with_psse_string(string data)
{
    bool is_successful = false;
    vector<string> dyrdata = split_string(data,",");

    if(dyrdata.size()<17)
        return is_successful;

    string model_name = get_string_data(dyrdata[1],"");
    if(model_name!=get_model_name())
        return is_successful;

    double tg, tp, uo, uc, pmax, pmin, sigma, delta, tr, tw, a11, a13, a21, a23;

    size_t i=3;
    tg = get_double_data(dyrdata[i],"0.0"); i++;
    tp = get_double_data(dyrdata[i],"0.0"); i++;
    uo = get_double_data(dyrdata[i],"0.0"); i++;
    uc = get_double_data(dyrdata[i],"0.0"); i++;
    pmax = get_double_data(dyrdata[i],"0.0"); i++;
    pmin = get_double_data(dyrdata[i],"0.0"); i++;
    sigma = get_double_data(dyrdata[i],"0.0"); i++;
    delta = get_double_data(dyrdata[i],"0.0"); i++;
    tr = get_double_data(dyrdata[i],"0.0"); i++;
    tw = get_double_data(dyrdata[i],"0.0"); i++;
    a11 = get_double_data(dyrdata[i],"0.0"); i++;
    a13 = get_double_data(dyrdata[i],"0.0"); i++;
    a21 = get_double_data(dyrdata[i],"0.0"); i++;
    a23 = get_double_data(dyrdata[i],"0.0"); i++;

    set_TG_in_s(tg);
    set_TP_in_s(tp);
    set_Uo_in_pu(uo);
    set_Uc_in_pu(uc);
    set_Pmax_in_pu(pmax);
    set_Pmin_in_pu(pmin);
    set_sigma(sigma);
    set_delta(delta);
    set_TR_in_s(tr);
    set_TW_in_s(tw);
    set_a11(a11);
    set_a13(a13);
    set_a21(a21);
    set_a23(a23);

    is_successful = true;

    return is_successful;
}

bool IEEEG3::setup_model_with_bpa_string(string data)
{
    return false;
}

void IEEEG3::initialize()
{
    ostringstream sstream;
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

    double delta = get_delta();
    double TR = get_TR_in_s();
    feedbacker.set_K(delta*TR);

    double a11 = get_a11();
    double a13 = get_a13();
    double a21 = get_a21();
    double a23 = get_a23();
    double TW = get_TW_in_s();

    water_hammer.set_K(a23);
    water_hammer.set_T1_in_s((a11-a13*a21/a23)*TW);
    water_hammer.set_T2_in_s(a11*TW);


    double pmech0 = get_initial_mechanical_power_in_pu_based_on_mbase_from_sync_generator_model();

    water_hammer.set_output(pmech0);
    water_hammer.initialize();

    double valve = water_hammer.get_input();
    feedbacker.set_input(valve);
    feedbacker.initialize();


    if(valve>get_Pmax_in_pu())
    {
        sstream<<"Initialization error. Valve of '"<<get_model_name()<<"' model of "<<get_device_name()<<" exceeds upper limit."
          <<"Valve is "<<valve<<", and Pmax is "<<get_Pmax_in_pu()<<".";
        show_information_with_leading_time_stamp(sstream);
    }
    if(valve<get_Pmin_in_pu())
    {
        sstream<<"Initialization error. Valve of '"<<get_model_name()<<"' model of "<<get_device_name()<<" exceeds lower limit."
          <<"Valve is "<<valve<<", and Pmin is "<<get_Pmin_in_pu()<<".";
        show_information_with_leading_time_stamp(sstream);
    }

    servo_motor.set_output(valve);
    servo_motor.initialize();

    governor.set_output(0.0);
    governor.initialize();

    double Pref = get_sigma()*valve;

    set_mechanical_power_reference_in_pu_based_on_mbase(Pref);

    set_flag_model_initialized_as_true();
}

void IEEEG3::run(DYNAMIC_MODE mode)
{
    double Pref = get_mechanical_power_reference_in_pu_based_on_mbase();
    double speed = get_rotor_speed_deviation_in_pu_from_sync_generator_model();

    double input = Pref-speed-get_sigma()*servo_motor.get_output()-feedbacker.get_output();

    governor.set_input(input);
    governor.run(mode);

    input = governor.get_output();
    servo_motor.set_input(input);
    servo_motor.run(mode);

    double valve = servo_motor.get_output();
    feedbacker.set_input(valve);
    feedbacker.run(mode);

    water_hammer.set_input(valve);
    water_hammer.run(mode);

    if(mode==UPDATE_MODE)
        set_flag_model_updated_as_true();
}

double IEEEG3::get_mechanical_power_in_pu_based_on_mbase() const
{
    double Pmech = water_hammer.get_output();

    return Pmech;
}

double IEEEG3::get_mechanical_power_upper_limit_in_pu_based_on_mbase() const
{
    return get_Pmax_in_pu();
}

double IEEEG3::get_mechanical_power_lower_limit_in_pu_based_on_mbase() const
{
    return get_Pmin_in_pu();
}

void IEEEG3::check()
{
    ;
}

void IEEEG3::report()
{
    ostringstream sstream;
    sstream<<get_standard_model_string();
    show_information_with_leading_time_stamp(sstream);
}
void IEEEG3::save()
{
    ;
}
string IEEEG3::get_standard_model_string() const
{
    ostringstream sstream;

    double TG = get_TG_in_s();
    double TP = get_TP_in_s();
    double Uo = get_Uo_in_pu();
    double Uc = get_Uc_in_pu();
    double Pmax = get_Pmax_in_pu();
    double Pmin = get_Pmin_in_pu();
    double sigma = get_sigma();
    double delta = get_delta();
    double TR = get_TR_in_s();
    double TW = get_TW_in_s();
    double a11 = get_a11();
    double a13 = get_a13();
    double a21 = get_a21();
    double a23 = get_a23();

    DEVICE_ID did = get_device_id();
    size_t bus = did.get_device_terminal().get_buses()[0];
    string identifier = did.get_device_identifier();

    sstream<<setw(8)<<bus<<", "
      <<"'"<<get_model_name()<<"', "
      <<"'"<<identifier<<"', "
      <<setw(8)<<setprecision(6)<<TG<<", "
      <<setw(8)<<setprecision(6)<<TP<<", "
      <<setw(8)<<setprecision(6)<<Uo<<", "
      <<setw(8)<<setprecision(6)<<Uc<<", "
      <<setw(8)<<setprecision(6)<<Pmax<<", "
      <<setw(8)<<setprecision(6)<<Pmin<<", "
      <<setw(8)<<setprecision(6)<<sigma<<", "
      <<setw(8)<<setprecision(6)<<delta<<", "
      <<setw(8)<<setprecision(6)<<TR<<", "
      <<setw(8)<<setprecision(6)<<TW<<", "
      <<setw(8)<<setprecision(6)<<a11<<", "
      <<setw(8)<<setprecision(6)<<a13<<", "
      <<setw(8)<<setprecision(6)<<a21<<", "
      <<setw(8)<<setprecision(6)<<a23<<"  /";
    return sstream.str();
}

size_t IEEEG3::get_variable_index_from_variable_name(string var_name)
{
    return MODEL::get_variable_index_from_variable_name(var_name, MODEL_VARIABLE_TABLE);
}
string IEEEG3::get_variable_name_from_variable_index(size_t var_index)
{
    return MODEL::get_variable_name_from_variable_index(var_index, MODEL_VARIABLE_TABLE);
}
double IEEEG3::get_variable_with_index(size_t var_index)
{
    string var_name = get_variable_name_from_variable_index(var_index);
    return get_variable_with_name(var_name);
}
double IEEEG3::get_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);

    if(var_name == "GENERATOR MECHANICAL POWER IN PU")
        return get_mechanical_power_in_pu_based_on_mbase();

    if(var_name == "GENERATOR MECHANICAL POWER IN MW")
    {
        GENERATOR* generator = get_generator_pointer();
        return get_mechanical_power_in_pu_based_on_mbase()*generator->get_mbase_in_MVA();
    }

    if(var_name == "MECHANICAL POWER REFERENCE IN PU")
        return get_mechanical_power_reference_in_pu_based_on_mbase();

    if(var_name == "GENERATOR ROTOR SPEED DEVIATION IN PU")
        return get_rotor_speed_deviation_in_pu_from_sync_generator_model();

    if(var_name == "STATE@GOVERNOR")
        return governor.get_state();

    if(var_name == "STATE@SERVO MOTOR")
        return servo_motor.get_state();

    if(var_name == "STATE@FEEDBACKER")
        return feedbacker.get_state();

    if(var_name == "STATE@WATER HAMMER")
        return water_hammer.get_state();


    return 0.0;
}

string IEEEG3::get_dynamic_data_in_psse_format() const
{
    return "";
}

string IEEEG3::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string IEEEG3::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}

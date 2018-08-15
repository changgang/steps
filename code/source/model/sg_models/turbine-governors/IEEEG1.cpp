#include "header/model/sg_models/turbine_governor_model/IEEEG1.h"
#include "header/basic/utility.h"
#include <istream>
#include <iostream>
using namespace std;
static vector<string> MODEL_VARIABLE_TABLE{ "GENERATOR MECHANICAL POWER IN PU", //0
                                            "GENERATOR MECHANICAL POWER IN MW",      //1
                                            "MECHANICAL POWER REFERENCE IN PU",      //2
                                            "GENERATOR ROTOR SPEED DEVIATION IN PU",     //3
                                            "STATE@DROOP",     //4
                                            "STATE@SERVO MOTOR",       //5
                                            "STATE@DELAYER 1",       //6
                                            "STATE@DELAYER 2",       //7
                                            "STATE@DELAYER 3",       //8
                                            "STATE@DELAYER 4",       //9
                                            };//8

IEEEG1::IEEEG1()
{
    clear();
}

IEEEG1::~IEEEG1()
{
    clear();
}
void IEEEG1::clear()
{
    servo_motor.set_limiter_type(NON_WINDUP_LIMITER);
    servo_motor.set_T_in_s(1.0);
    set_T5_in_s(0.0);
    set_T6_in_s(0.0);
    set_T7_in_s(0.0);
    set_K1(0.0);
    set_K2(0.0);
    set_K3(0.0);
    set_K4(0.0);
    set_K5(0.0);
    set_K6(0.0);
    set_K7(0.0);
    set_K8(0.0);
}
void IEEEG1::copy_from_const_model(const IEEEG1& model)
{
    clear();

    this->set_K(model.get_K());
    this->set_T1_in_s(model.get_T1_in_s());
    this->set_T2_in_s(model.get_T2_in_s());
    this->set_T3_in_s(model.get_T3_in_s());
    this->set_Uo_in_pu(model.get_Uo_in_pu());
    this->set_Uc_in_pu(model.get_Uc_in_pu());
    this->set_Pmax_in_pu(model.get_Pmax_in_pu());
    this->set_Pmin_in_pu(model.get_Pmin_in_pu());
    this->set_T4_in_s(model.get_T4_in_s());
    this->set_T5_in_s(model.get_T5_in_s());
    this->set_T6_in_s(model.get_T6_in_s());
    this->set_T7_in_s(model.get_T7_in_s());
    this->set_K1(model.get_K1());
    this->set_K2(model.get_K2());
    this->set_K3(model.get_K3());
    this->set_K4(model.get_K4());
    this->set_K5(model.get_K5());
    this->set_K6(model.get_K6());
    this->set_K7(model.get_K7());
    this->set_K8(model.get_K8());
}

IEEEG1::IEEEG1(const IEEEG1&model) : TURBINE_GOVERNOR_MODEL()
{
    copy_from_const_model(model);
}

IEEEG1& IEEEG1::operator=(const IEEEG1& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

string IEEEG1::get_model_name() const
{
    return "IEEEG1";
}

double IEEEG1::get_double_data_with_index(size_t index) const
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input index is provided: "<<index;
    show_information_with_leading_time_stamp(osstream);
    return 0.0;
}

double IEEEG1::get_double_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="PMAX")
        return get_Pmax_in_pu();
    if(par_name=="PMIN")
        return get_Pmin_in_pu();

    return 0.0;
}

void IEEEG1::set_double_data_with_index(size_t index, double value)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input (index, value) is provided: ("<<index<<", "<<value<<").";
    show_information_with_leading_time_stamp(osstream);
    return;
}

void IEEEG1::set_double_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="PMAX")
        return set_Pmax_in_pu(value);
    if(par_name=="PMIN")
        return set_Pmin_in_pu(value);
}

void IEEEG1::set_K(double K)
{
    droop.set_K(K);
}

void IEEEG1::set_T1_in_s(double T)
{
    droop.set_T1_in_s(T);
}

void IEEEG1::set_T2_in_s(double T)
{
    droop.set_T2_in_s(T);
}

void IEEEG1::set_T3_in_s(double T)
{
    this->Tservo = T;
}

void IEEEG1::set_Uo_in_pu(double U)
{
    this->Uo = U;
}

void IEEEG1::set_Uc_in_pu(double U)
{
    this->Uc = U;
}

void IEEEG1::set_Pmax_in_pu(double P)
{
    servo_motor.set_upper_limit(P);
}

void IEEEG1::set_Pmin_in_pu(double P)
{
    servo_motor.set_lower_limit(P);
}

void IEEEG1::set_T4_in_s(double T)
{
    delayer1.set_T_in_s(T);
}

void IEEEG1::set_T5_in_s(double T)
{
    delayer2.set_T_in_s(T);
}

void IEEEG1::set_T6_in_s(double T)
{
    delayer3.set_T_in_s(T);
}

void IEEEG1::set_T7_in_s(double T)
{
    delayer4.set_T_in_s(T);
}

void IEEEG1::set_K1(double K)
{
    this->K1 = K;
}

void IEEEG1::set_K2(double K)
{
    this->K2 = K;
}

void IEEEG1::set_K3(double K)
{
    this->K3 = K;
}

void IEEEG1::set_K4(double K)
{
    this->K4 = K;
}

void IEEEG1::set_K5(double K)
{
    this->K5 = K;
}

void IEEEG1::set_K6(double K)
{
    this->K6 = K;
}

void IEEEG1::set_K7(double K)
{
    this->K7 = K;
}

void IEEEG1::set_K8(double K)
{
    this->K8 = K;
}


double IEEEG1::get_K() const
{
    return droop.get_K();
}

double IEEEG1::get_T1_in_s() const
{
    return droop.get_T1_in_s();
}

double IEEEG1::get_T2_in_s() const
{
    return droop.get_T2_in_s();
}

double IEEEG1::get_T3_in_s() const
{
    return Tservo;
}

double IEEEG1::get_Uo_in_pu() const
{
    return Uo;
}

double IEEEG1::get_Uc_in_pu() const
{
    return Uc;
}

double IEEEG1::get_Pmax_in_pu() const
{
    return servo_motor.get_upper_limit();
}

double IEEEG1::get_Pmin_in_pu() const
{
    return servo_motor.get_lower_limit();
}

double IEEEG1::get_T4_in_s() const
{
    return delayer1.get_T_in_s();
}

double IEEEG1::get_T5_in_s() const
{
    return delayer2.get_T_in_s();
}

double IEEEG1::get_T6_in_s() const
{
    return delayer3.get_T_in_s();
}

double IEEEG1::get_T7_in_s() const
{
    return delayer4.get_T_in_s();
}

double IEEEG1::get_K1() const
{
    return K1;
}

double IEEEG1::get_K2() const
{
    return K2;
}

double IEEEG1::get_K3() const
{
    return K3;
}

double IEEEG1::get_K4() const
{
    return K4;
}

double IEEEG1::get_K5() const
{
    return K5;
}

double IEEEG1::get_K6() const
{
    return K6;
}

double IEEEG1::get_K7() const
{
    return K7;
}

double IEEEG1::get_K8() const
{
    return K8;
}


bool IEEEG1::setup_model_with_steps_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    show_information_with_leading_time_stamp(osstream);
    return false;
}

bool IEEEG1::setup_model_with_psse_string(string data)
{
    bool is_successful = false;
    vector<string> dyrdata = split_string(data,",");

    if(dyrdata.size()<25)
        return is_successful;

    string model_name = get_string_data(dyrdata[1],"");
    if(model_name!=get_model_name())
        return is_successful;


    double k, t1, t2, t3, uo, uc, pmax, pmin, t4, k1, k2, t5, k3, k4, t6, k5, k6, t7, k7, k8;

    size_t i=5;
    k = get_double_data(dyrdata[i],"0.0"); i++;
    t1 = get_double_data(dyrdata[i],"0.0"); i++;
    t2 = get_double_data(dyrdata[i],"0.0"); i++;
    t3 = get_double_data(dyrdata[i],"0.0"); i++;
    uo = get_double_data(dyrdata[i],"0.0"); i++;
    uc = get_double_data(dyrdata[i],"0.0"); i++;
    pmax = get_double_data(dyrdata[i],"0.0"); i++;
    pmin = get_double_data(dyrdata[i],"0.0"); i++;
    t4 = get_double_data(dyrdata[i],"0.0"); i++;
    k1 = get_double_data(dyrdata[i],"0.0"); i++;
    k2 = get_double_data(dyrdata[i],"0.0"); i++;
    t5 = get_double_data(dyrdata[i],"0.0"); i++;
    k3 = get_double_data(dyrdata[i],"0.0"); i++;
    k4 = get_double_data(dyrdata[i],"0.0"); i++;
    t6 = get_double_data(dyrdata[i],"0.0"); i++;
    k5 = get_double_data(dyrdata[i],"0.0"); i++;
    k6 = get_double_data(dyrdata[i],"0.0"); i++;
    t7 = get_double_data(dyrdata[i],"0.0"); i++;
    k7 = get_double_data(dyrdata[i],"0.0"); i++;
    k8 = get_double_data(dyrdata[i],"0.0");

    set_K(k);
    set_T1_in_s(t1);
    set_T2_in_s(t2);
    set_T3_in_s(t3);
    set_Uo_in_pu(uo);
    set_Uc_in_pu(uc);
    set_Pmax_in_pu(pmax);
    set_Pmin_in_pu(pmin);
    set_T4_in_s(t4);
    set_K1(k1);
    set_K2(k2);
    set_T5_in_s(t5);
    set_K3(k3);
    set_K4(k4);
    set_T6_in_s(t6);
    set_K5(k5);
    set_K6(k6);
    set_T7_in_s(t7);
    set_K7(k7);
    set_K8(k8);

    is_successful = true;

    return is_successful;
}

bool IEEEG1::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    show_information_with_leading_time_stamp(osstream);
    return false;
}


void IEEEG1::initialize()
{
    ostringstream osstream;
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

    double pmech0 = get_initial_mechanical_power_in_pu_based_on_mbase_from_sync_generator_model();

    droop.set_output(0.0);
    droop.initialize();

    double sumK=get_K1()+get_K3()+get_K5()+get_K7();

    double valve = pmech0/sumK;

    if(valve>get_Pmax_in_pu())
    {
        osstream<<"Initialization error. Valve of '"<<get_model_name()<<"' model of "<<get_device_name()<<" exceeds upper limit."
          <<"Valve is "<<valve<<", and Pmax is "<<get_Pmax_in_pu()<<".";
        show_information_with_leading_time_stamp(osstream);
    }
    if(valve<get_Pmin_in_pu())
    {
        osstream<<"Initialization error. Valve of '"<<get_model_name()<<"' model of "<<get_device_name()<<" exceeds lower limit."
          <<"Valve is "<<valve<<", and Pmin is "<<get_Pmin_in_pu()<<".";
        show_information_with_leading_time_stamp(osstream);
    }

    servo_motor.set_output(valve);
    delayer1.set_output(valve);
    delayer2.set_output(valve);
    delayer3.set_output(valve);
    delayer4.set_output(valve);

    servo_motor.initialize();
    delayer1.initialize();
    delayer2.initialize();
    delayer3.initialize();
    delayer4.initialize();

    set_mechanical_power_reference_in_pu_based_on_mbase(valve);

    set_flag_model_initialized_as_true();
}

void IEEEG1::run(DYNAMIC_MODE mode)
{
    double Pref = get_mechanical_power_reference_in_pu_based_on_mbase();
    double speed = get_rotor_speed_deviation_in_pu_from_sync_generator_model();

    droop.set_input(speed);
    droop.run(mode);

    double input = Pref-droop.get_output()-servo_motor.get_output();

    input = input/Tservo;

    if(input>Uo)
        input = Uo;
    if(input<Uc)
        input = Uc;

    servo_motor.set_input(input);
    servo_motor.run(mode);

    input = servo_motor.get_output();
    delayer1.set_input(input);
    delayer1.run(mode);

    input = delayer1.get_output();
    delayer2.set_input(input);
    delayer2.run(mode);

    input = delayer2.get_output();
    delayer3.set_input(input);
    delayer3.run(mode);

    input = delayer3.get_output();
    delayer4.set_input(input);
    delayer4.run(mode);

    if(mode==UPDATE_MODE)
        set_flag_model_updated_as_true();
}

double IEEEG1::get_mechanical_power_in_pu_based_on_mbase() const
{
    double Pmech = delayer1.get_output()*get_K1()+
                   delayer2.get_output()*get_K3()+
                   delayer3.get_output()*get_K5()+
                   delayer4.get_output()*get_K7();

    return Pmech;
}

double IEEEG1::get_mechanical_power_upper_limit_in_pu_based_on_mbase() const
{
    return get_Pmax_in_pu();
}

double IEEEG1::get_mechanical_power_lower_limit_in_pu_based_on_mbase() const
{
    return get_Pmin_in_pu();
}

void IEEEG1::check()
{
    ;
}

void IEEEG1::report()
{
    ostringstream osstream;
    osstream<<get_standard_model_string();
    show_information_with_leading_time_stamp(osstream);
}
void IEEEG1::save()
{
    ;
}
string IEEEG1::get_standard_model_string() const
{
    ostringstream osstream;

    double K = get_K();
    double T1 = get_T1_in_s();
    double T2 = get_T2_in_s();
    double T3 = get_T3_in_s();
    double Uo = get_Uo_in_pu();
    double Uc = get_Uc_in_pu();
    double Pmax = get_Pmax_in_pu();
    double Pmin = get_Pmin_in_pu();
    double T4 = get_T4_in_s();
    double T5 = get_T5_in_s();
    double T6 = get_T6_in_s();
    double T7 = get_T7_in_s();
    double K1 = get_K1();
    double K2 = get_K2();
    double K3 = get_K3();
    double K4 = get_K4();
    double K5 = get_K5();
    double K6 = get_K6();
    double K7 = get_K7();
    double K8 = get_K8();

    DEVICE_ID did = get_device_id();
    size_t bus = did.get_device_terminal().get_buses()[0];
    string identifier = did.get_device_identifier();

    osstream<<setw(8)<<bus<<", "
      <<"'"<<get_model_name()<<"', "
      <<"'"<<identifier<<"', 0, '', "
      <<setw(8)<<setprecision(6)<<K<<", "
      <<setw(8)<<setprecision(6)<<T1<<", "
      <<setw(8)<<setprecision(6)<<T2<<", "
      <<setw(8)<<setprecision(6)<<T3<<", "
      <<setw(8)<<setprecision(6)<<Uo<<", "
      <<setw(8)<<setprecision(6)<<Uc<<", "
      <<setw(8)<<setprecision(6)<<Pmax<<", "
      <<setw(8)<<setprecision(6)<<Pmin<<", "
      <<setw(8)<<setprecision(6)<<T4<<", "
      <<setw(8)<<setprecision(6)<<K1<<", "
      <<setw(8)<<setprecision(6)<<K2<<", "
      <<setw(8)<<setprecision(6)<<T5<<", "
      <<setw(8)<<setprecision(6)<<K3<<", "
      <<setw(8)<<setprecision(6)<<K4<<", "
      <<setw(8)<<setprecision(6)<<T6<<", "
      <<setw(8)<<setprecision(6)<<K5<<", "
      <<setw(8)<<setprecision(6)<<K6<<", "
      <<setw(8)<<setprecision(6)<<T7<<", "
      <<setw(8)<<setprecision(6)<<K7<<", "
      <<setw(8)<<setprecision(6)<<K8<<"  /";
    return osstream.str();
}

size_t IEEEG1::get_variable_index_from_variable_name(string var_name)
{
    return MODEL::get_variable_index_from_variable_name(var_name, MODEL_VARIABLE_TABLE);
}
string IEEEG1::get_variable_name_from_variable_index(size_t var_index)
{
    return MODEL::get_variable_name_from_variable_index(var_index, MODEL_VARIABLE_TABLE);
}
double IEEEG1::get_variable_with_index(size_t var_index)
{
    string var_name = get_variable_name_from_variable_index(var_index);
    return get_variable_with_name(var_name);
}
double IEEEG1::get_variable_with_name(string var_name)
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

    if(var_name == "STATE@DROOP")
        return droop.get_state();

    if(var_name == "STATE@SERVO MOTOR")
        return servo_motor.get_state();

    if(var_name == "STATE@DELAYER 1")
        return delayer1.get_state();

    if(var_name == "STATE@DELAYER 2")
        return delayer2.get_state();

    if(var_name == "STATE@DELAYER 3")
        return delayer3.get_state();

    if(var_name == "STATE@DELAYER 4")
        return delayer4.get_state();

    return 0.0;
}


string IEEEG1::get_dynamic_data_in_psse_format() const
{
    return "";
}

string IEEEG1::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string IEEEG1::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}

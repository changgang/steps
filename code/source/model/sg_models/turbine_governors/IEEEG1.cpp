#include "header/model/sg_models/turbine_governor_model/IEEEG1.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <istream>
#include <iostream>
using namespace std;
IEEEG1::IEEEG1()
{
    clear();
}

IEEEG1::~IEEEG1()
{
}
void IEEEG1::clear()
{
    set_model_float_parameter_count(20);

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


bool IEEEG1::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=25)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            size_t i=5;

            set_K(get_double_data(data[i],"0.0")); i++;
            set_T1_in_s(get_double_data(data[i],"0.0")); i++;
            set_T2_in_s(get_double_data(data[i],"0.0")); i++;
            set_T3_in_s(get_double_data(data[i],"0.0")); i++;
            set_Uo_in_pu(get_double_data(data[i],"0.0")); i++;
            set_Uc_in_pu(get_double_data(data[i],"0.0")); i++;
            set_Pmax_in_pu(get_double_data(data[i],"0.0")); i++;
            set_Pmin_in_pu(get_double_data(data[i],"0.0")); i++;
            set_T4_in_s(get_double_data(data[i],"0.0")); i++;
            set_K1(get_double_data(data[i],"0.0")); i++;
            set_K2(get_double_data(data[i],"0.0")); i++;
            set_T5_in_s(get_double_data(data[i],"0.0")); i++;
            set_K3(get_double_data(data[i],"0.0")); i++;
            set_K4(get_double_data(data[i],"0.0")); i++;
            set_T6_in_s(get_double_data(data[i],"0.0")); i++;
            set_K5(get_double_data(data[i],"0.0")); i++;
            set_K6(get_double_data(data[i],"0.0")); i++;
            set_T7_in_s(get_double_data(data[i],"0.0")); i++;
            set_K7(get_double_data(data[i],"0.0")); i++;
            set_K8(get_double_data(data[i],"0.0"));

            is_successful = true;

            return is_successful;
        }
        else
            return is_successful;
    }
    else
        return is_successful;
}

bool IEEEG1::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool IEEEG1::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void IEEEG1::setup_block_toolkit_and_parameters()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    droop.set_toolkit(toolkit);
    servo_motor.set_toolkit(toolkit);
    delayer1.set_toolkit(toolkit);
    delayer2.set_toolkit(toolkit);
    delayer3.set_toolkit(toolkit);
    delayer4.set_toolkit(toolkit);
}

void IEEEG1::initialize()
{
    ostringstream osstream;
    if(not is_model_initialized())
    {
        GENERATOR* generator = get_generator_pointer();
        if(generator!=NULL)
        {
            SYNC_GENERATOR_MODEL* gen_model = generator->get_sync_generator_model();
            if(gen_model!=NULL)
            {
                if(not gen_model->is_model_initialized())
                    gen_model->initialize();

                setup_block_toolkit_and_parameters();

                STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);

                double pmech0 = get_initial_mechanical_power_in_pu_based_on_mbase_from_sync_generator_model();

                droop.set_output(0.0);
                droop.initialize();

                double sumK=get_K1()+get_K3()+get_K5()+get_K7();

                double valve = pmech0/sumK;

                if(valve>get_Pmax_in_pu())
                {
                    osstream<<"Initialization error. Valve of '"<<get_model_name()<<"' model of "<<get_device_name()<<" exceeds upper limit."
                      <<"Valve is "<<valve<<", and Pmax is "<<get_Pmax_in_pu()<<".";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
                if(valve<get_Pmin_in_pu())
                {
                    osstream<<"Initialization error. Valve of '"<<get_model_name()<<"' model of "<<get_device_name()<<" exceeds lower limit."
                      <<"Valve is "<<valve<<", and Pmin is "<<get_Pmin_in_pu()<<".";
                    toolkit.show_information_with_leading_time_stamp(osstream);
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

                set_initial_mechanical_power_reference_in_pu_based_on_mbase(valve);

                set_flag_model_initialized_as_true();
            }
        }
    }
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
    osstream<<get_standard_psse_string();
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
}
void IEEEG1::save()
{
    ;
}
string IEEEG1::get_standard_psse_string() const
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
    string identifier = "'"+did.get_device_identifier()+"'";

    string model_name = "'"+get_model_name()+"'";

    osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(6)<<identifier<<", "
            <<setw(10)<<0<<", "
            <<setw(6)<<"''"<<", "
            <<setw(8)<<setprecision(6)<<K<<", "
            <<setw(8)<<setprecision(6)<<T1<<", "
            <<setw(8)<<setprecision(6)<<T2<<", "
            <<setw(8)<<setprecision(6)<<T3<<", "
            <<setw(8)<<setprecision(6)<<Uo<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<Uc<<", "
            <<setw(8)<<setprecision(6)<<Pmax<<", "
            <<setw(8)<<setprecision(6)<<Pmin<<", "
            <<setw(8)<<setprecision(6)<<T4<<", "
            <<setw(8)<<setprecision(6)<<K1<<", "
            <<setw(8)<<setprecision(6)<<K2<<", "
            <<setw(8)<<setprecision(6)<<T5<<", "
            <<setw(8)<<setprecision(6)<<K3<<", "
            <<setw(8)<<setprecision(6)<<K4<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<T6<<", "
            <<setw(8)<<setprecision(6)<<K5<<", "
            <<setw(8)<<setprecision(6)<<K6<<", "
            <<setw(8)<<setprecision(6)<<T7<<", "
            <<setw(8)<<setprecision(6)<<K7<<", "
            <<setw(8)<<setprecision(6)<<K8<<" /";
    return osstream.str();
}

void IEEEG1::prepare_model_data_table()
{
    clear_model_data_table();
    size_t i=0;
    add_model_data_name_and_index_pair("K", i); i++;
    add_model_data_name_and_index_pair("T1", i); i++;
    add_model_data_name_and_index_pair("T2", i); i++;
    add_model_data_name_and_index_pair("T3", i); i++;
    add_model_data_name_and_index_pair("UO", i); i++;
    add_model_data_name_and_index_pair("UC", i); i++;
    add_model_data_name_and_index_pair("PMAX", i); i++;
    add_model_data_name_and_index_pair("PMIN", i); i++;
    add_model_data_name_and_index_pair("T4", i); i++;
    add_model_data_name_and_index_pair("K1", i); i++;
    add_model_data_name_and_index_pair("K2", i); i++;
    add_model_data_name_and_index_pair("T5", i); i++;
    add_model_data_name_and_index_pair("K3", i); i++;
    add_model_data_name_and_index_pair("K4", i); i++;
    add_model_data_name_and_index_pair("T6", i); i++;
    add_model_data_name_and_index_pair("K5", i); i++;
    add_model_data_name_and_index_pair("K6", i); i++;
    add_model_data_name_and_index_pair("T7", i); i++;
    add_model_data_name_and_index_pair("K7", i); i++;
    add_model_data_name_and_index_pair("K8", i); i++;
}

double IEEEG1::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="K")
        return get_K();
    if(par_name=="T1")
        return get_T1_in_s();
    if(par_name=="T2")
        return get_T2_in_s();
    if(par_name=="T3")
        return get_T3_in_s();
    if(par_name=="UO")
        return get_Uo_in_pu();
    if(par_name=="UC")
        return get_Uc_in_pu();
    if(par_name=="PMAX")
        return get_Pmax_in_pu();
    if(par_name=="PMIN")
        return get_Pmin_in_pu();
    if(par_name=="T4")
        return get_T4_in_s();
    if(par_name=="K1")
        return get_K1();
    if(par_name=="K2")
        return get_K2();
    if(par_name=="T5")
        return get_T5_in_s();
    if(par_name=="K3")
        return get_K3();
    if(par_name=="K4")
        return get_K4();
    if(par_name=="T6")
        return get_T6_in_s();
    if(par_name=="K5")
        return get_K5();
    if(par_name=="K6")
        return get_K6();
    if(par_name=="T7")
        return get_T7_in_s();
    if(par_name=="K7")
        return get_K7();
    if(par_name=="K8")
        return get_K8();

    return 0.0;
}

void IEEEG1::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="K")
        return set_K(value);
    if(par_name=="T1")
        return set_T1_in_s(value);
    if(par_name=="T2")
        return set_T2_in_s(value);
    if(par_name=="T3")
        return set_T3_in_s(value);
    if(par_name=="UO")
        return set_Uo_in_pu(value);
    if(par_name=="UC")
        return set_Uc_in_pu(value);
    if(par_name=="PMAX")
        return set_Pmax_in_pu(value);
    if(par_name=="PMIN")
        return set_Pmin_in_pu(value);
    if(par_name=="T4")
        return set_T4_in_s(value);
    if(par_name=="K1")
        return set_K1(value);
    if(par_name=="K2")
        return set_K2(value);
    if(par_name=="T5")
        return set_T5_in_s(value);
    if(par_name=="K3")
        return set_K3(value);
    if(par_name=="K4")
        return set_K4(value);
    if(par_name=="T6")
        return set_T6_in_s(value);
    if(par_name=="K5")
        return set_K5(value);
    if(par_name=="K6")
        return set_K6(value);
    if(par_name=="T7")
        return set_T7_in_s(value);
    if(par_name=="K7")
        return set_K7(value);
    if(par_name=="K8")
        return set_K8(value);
}

void IEEEG1::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    size_t i=0;
    add_model_inernal_variable_name_and_index_pair("STATE@DROOP", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@SERVO MOTOR", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@DELAYER 1", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@DELAYER 2", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@DELAYER 3", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@DELAYER 4", i); i++;
}

double IEEEG1::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
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

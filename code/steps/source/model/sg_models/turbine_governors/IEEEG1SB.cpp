#include "header/model/sg_models/turbine_governor_model/IEEEG1SB.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <istream>
#include <iostream>
using namespace std;
IEEEG1SB::IEEEG1SB(STEPS& toolkit) : TURBINE_GOVERNOR_MODEL(toolkit),
                                     droop(toolkit),
                                     servo_motor(toolkit),
                                     delayer1(toolkit),
                                     delayer2(toolkit),
                                     delayer3(toolkit),
                                     delayer4(toolkit),
                                     boiler_PI(toolkit),
                                     fuel_producer(toolkit),
                                     fuel_delayer1(toolkit),
                                     fuel_delayer2(toolkit),
                                     drum(toolkit)
{
    clear();
}

IEEEG1SB::~IEEEG1SB()
{
}
void IEEEG1SB::clear()
{
    set_model_float_parameter_count(27);

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
    set_KP(0.0);
    set_KI(1.0);
    set_Tfuel_in_s(30.0);
    set_TFD1_in_s(5.0);
    set_TFD2_in_s(5.0);
    set_Cb_in_s(60.0);
    set_Kb(0.2);
}
void IEEEG1SB::copy_from_const_model(const IEEEG1SB& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);
    droop.set_toolkit(toolkit);
    servo_motor.set_toolkit(toolkit);
    delayer1.set_toolkit(toolkit);
    delayer2.set_toolkit(toolkit);
    delayer3.set_toolkit(toolkit);
    delayer4.set_toolkit(toolkit);
    boiler_PI.set_toolkit(toolkit);
    fuel_producer.set_toolkit(toolkit);
    fuel_delayer1.set_toolkit(toolkit);
    fuel_delayer2.set_toolkit(toolkit);
    drum.set_toolkit(toolkit);

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
    this->set_KP(model.get_KP());
    this->set_KI(model.get_KI());
    this->set_Tfuel_in_s(model.get_Tfuel_in_s());
    this->set_TFD1_in_s(model.get_TFD1_in_s());
    this->set_TFD2_in_s(model.get_TFD2_in_s());
    this->set_Cb_in_s(model.get_Cb_in_s());
    this->set_Kb(model.get_Kb());
}

IEEEG1SB::IEEEG1SB(const IEEEG1SB&model) : TURBINE_GOVERNOR_MODEL(model.get_toolkit()),
                                           droop(model.get_toolkit()),
                                           servo_motor(model.get_toolkit()),
                                           delayer1(model.get_toolkit()),
                                           delayer2(model.get_toolkit()),
                                           delayer3(model.get_toolkit()),
                                           delayer4(model.get_toolkit()),
                                           boiler_PI(model.get_toolkit()),
                                           fuel_producer(model.get_toolkit()),
                                           fuel_delayer1(model.get_toolkit()),
                                           fuel_delayer2(model.get_toolkit()),
                                           drum(model.get_toolkit())
{
    copy_from_const_model(model);
}

IEEEG1SB& IEEEG1SB::operator=(const IEEEG1SB& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

string IEEEG1SB::get_model_name() const
{
    return "IEEEG1SB";
}


void IEEEG1SB::set_K(double K)
{
    droop.set_K(K);
}

void IEEEG1SB::set_T1_in_s(double T)
{
    droop.set_T1_in_s(T);
}

void IEEEG1SB::set_T2_in_s(double T)
{
    droop.set_T2_in_s(T);
}

void IEEEG1SB::set_T3_in_s(double T)
{
    this->Tservo = T;
}

void IEEEG1SB::set_Uo_in_pu(double U)
{
    this->Uo = U;
}

void IEEEG1SB::set_Uc_in_pu(double U)
{
    this->Uc = U;
}

void IEEEG1SB::set_Pmax_in_pu(double P)
{
    servo_motor.set_upper_limit(P);
}

void IEEEG1SB::set_Pmin_in_pu(double P)
{
    servo_motor.set_lower_limit(P);
}

void IEEEG1SB::set_T4_in_s(double T)
{
    delayer1.set_T_in_s(T);
}

void IEEEG1SB::set_T5_in_s(double T)
{
    delayer2.set_T_in_s(T);
}

void IEEEG1SB::set_T6_in_s(double T)
{
    delayer3.set_T_in_s(T);
}

void IEEEG1SB::set_T7_in_s(double T)
{
    delayer4.set_T_in_s(T);
}

void IEEEG1SB::set_K1(double K)
{
    this->K1 = K;
}

void IEEEG1SB::set_K2(double K)
{
    this->K2 = K;
}

void IEEEG1SB::set_K3(double K)
{
    this->K3 = K;
}

void IEEEG1SB::set_K4(double K)
{
    this->K4 = K;
}

void IEEEG1SB::set_K5(double K)
{
    this->K5 = K;
}

void IEEEG1SB::set_K6(double K)
{
    this->K6 = K;
}

void IEEEG1SB::set_K7(double K)
{
    this->K7 = K;
}

void IEEEG1SB::set_K8(double K)
{
    this->K8 = K;
}

void IEEEG1SB::set_KP(double K)
{
    boiler_PI.set_Kp(K);
}

void IEEEG1SB::set_KI(double K)
{
    boiler_PI.set_Ki(K);
}

void IEEEG1SB::set_Tfuel_in_s(double T)
{
    fuel_producer.set_T_in_s(T);
}

void IEEEG1SB::set_TFD1_in_s(double T)
{
    fuel_delayer1.set_T_in_s(T);
}

void IEEEG1SB::set_TFD2_in_s(double T)
{
    fuel_delayer2.set_T_in_s(T);
}

void IEEEG1SB::set_Cb_in_s(double T)
{
    drum.set_T_in_s(T);
}

void IEEEG1SB::set_Kb(double K)
{
    Kb = K;
}

double IEEEG1SB::get_K() const
{
    return droop.get_K();
}

double IEEEG1SB::get_T1_in_s() const
{
    return droop.get_T1_in_s();
}

double IEEEG1SB::get_T2_in_s() const
{
    return droop.get_T2_in_s();
}

double IEEEG1SB::get_T3_in_s() const
{
    return Tservo;
}

double IEEEG1SB::get_Uo_in_pu() const
{
    return Uo;
}

double IEEEG1SB::get_Uc_in_pu() const
{
    return Uc;
}

double IEEEG1SB::get_Pmax_in_pu() const
{
    return servo_motor.get_upper_limit();
}

double IEEEG1SB::get_Pmin_in_pu() const
{
    return servo_motor.get_lower_limit();
}

double IEEEG1SB::get_T4_in_s() const
{
    return delayer1.get_T_in_s();
}

double IEEEG1SB::get_T5_in_s() const
{
    return delayer2.get_T_in_s();
}

double IEEEG1SB::get_T6_in_s() const
{
    return delayer3.get_T_in_s();
}

double IEEEG1SB::get_T7_in_s() const
{
    return delayer4.get_T_in_s();
}

double IEEEG1SB::get_K1() const
{
    return K1;
}

double IEEEG1SB::get_K2() const
{
    return K2;
}

double IEEEG1SB::get_K3() const
{
    return K3;
}

double IEEEG1SB::get_K4() const
{
    return K4;
}

double IEEEG1SB::get_K5() const
{
    return K5;
}

double IEEEG1SB::get_K6() const
{
    return K6;
}

double IEEEG1SB::get_K7() const
{
    return K7;
}

double IEEEG1SB::get_K8() const
{
    return K8;
}

double IEEEG1SB::get_KP() const
{
    return boiler_PI.get_Kp();
}

double IEEEG1SB::get_KI() const
{
    return boiler_PI.get_Ki();
}

double IEEEG1SB::get_Tfuel_in_s() const
{
    return fuel_producer.get_T_in_s();
}

double IEEEG1SB::get_TFD1_in_s() const
{
    return fuel_delayer1.get_T_in_s();
}

double IEEEG1SB::get_TFD2_in_s() const
{
    return fuel_delayer2.get_T_in_s();
}

double IEEEG1SB::get_Cb_in_s() const
{
    return drum.get_T_in_s();
}

double IEEEG1SB::get_Kb() const
{
    return Kb;
}

bool IEEEG1SB::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=32)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            double k, t1, t2, t3, uo, uc, pmax, pmin, t4, k1, k2, t5, k3, k4, t6, k5, k6, t7, k7, k8,
                   kp, ki, tfuel, tfd1, tfd2, cb, kb;

            unsigned int i=5;
            k = get_double_data(data[i],"0.0"); i++;
            t1 = get_double_data(data[i],"0.0"); i++;
            t2 = get_double_data(data[i],"0.0"); i++;
            t3 = get_double_data(data[i],"0.0"); i++;
            uo = get_double_data(data[i],"0.0"); i++;
            uc = get_double_data(data[i],"0.0"); i++;
            pmax = get_double_data(data[i],"0.0"); i++;
            pmin = get_double_data(data[i],"0.0"); i++;
            t4 = get_double_data(data[i],"0.0"); i++;
            k1 = get_double_data(data[i],"0.0"); i++;
            k2 = get_double_data(data[i],"0.0"); i++;
            t5 = get_double_data(data[i],"0.0"); i++;
            k3 = get_double_data(data[i],"0.0"); i++;
            k4 = get_double_data(data[i],"0.0"); i++;
            t6 = get_double_data(data[i],"0.0"); i++;
            k5 = get_double_data(data[i],"0.0"); i++;
            k6 = get_double_data(data[i],"0.0"); i++;
            t7 = get_double_data(data[i],"0.0"); i++;
            k7 = get_double_data(data[i],"0.0"); i++;
            k8 = get_double_data(data[i],"0.0"); i++;
            kp = get_double_data(data[i],"0.0"); i++;
            ki = get_double_data(data[i],"0.0"); i++;
            tfuel = get_double_data(data[i],"0.0"); i++;
            tfd1 = get_double_data(data[i],"0.0"); i++;
            tfd2 = get_double_data(data[i],"0.0"); i++;
            cb = get_double_data(data[i],"0.0"); i++;
            kb = get_double_data(data[i],"0.0");

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
            set_KP(kp);
            set_KI(ki);
            set_Tfuel_in_s(tfuel);
            set_TFD1_in_s(tfd1);
            set_TFD2_in_s(tfd2);
            set_Cb_in_s(cb);
            set_Kb(kb);

            is_successful = true;

            return is_successful;
        }
        else
            return is_successful;
    }
    else
        return is_successful;
}

bool IEEEG1SB::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool IEEEG1SB::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void IEEEG1SB::setup_block_toolkit_and_parameters()
{
}

void IEEEG1SB::initialize()
{
    ostringstream osstream;
    if(not is_model_initialized())
    {
        STEPS& toolkit = get_toolkit();

        GENERATOR* generator = get_generator_pointer();
        SYNC_GENERATOR_MODEL* gen_model = generator->get_sync_generator_model();
        if(gen_model!=NULL)
        {
            if(not gen_model->is_model_initialized())
                gen_model->initialize();

            setup_block_toolkit_and_parameters();

            double pmech0 = get_initial_mechanical_power_in_pu_based_on_mbase_from_sync_generator_model();

            droop.set_output(0.0);
            droop.initialize();

            double sumK=get_K1()+get_K3()+get_K5()+get_K7();

            double valve = pmech0/sumK;

            if(valve>get_Pmax_in_pu())
            {
                osstream<<"Initialization error. Valve of '"<<get_model_name()<<"' model of "<<get_compound_device_name()<<" exceeds upper limit."
                  <<"Valve is "<<valve<<", and Pmax is "<<get_Pmax_in_pu()<<".";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
            if(valve<get_Pmin_in_pu())
            {
                osstream<<"Initialization error. Valve of '"<<get_model_name()<<"' model of "<<get_compound_device_name()<<" exceeds lower limit."
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

            drum.set_output(1.0+get_Kb()*valve*valve);
            drum.initialize();

            fuel_delayer2.set_output(valve);
            fuel_delayer2.initialize();

            fuel_delayer1.set_output(valve);
            fuel_delayer1.initialize();

            fuel_producer.set_output(valve);
            fuel_producer.initialize();

            boiler_PI.set_output(valve);
            boiler_PI.initialize();

            set_flag_model_initialized_as_true();
        }
    }
}

void IEEEG1SB::run(DYNAMIC_MODE mode)
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

    input = delayer1.get_output();
    double pressure = drum.get_output()-input*input*get_Kb();

    input = servo_motor.get_output()*pressure;
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

    input = pressure-1.0;
    boiler_PI.set_input(input);
    boiler_PI.run(mode);

    fuel_producer.set_input(boiler_PI.get_output());
    fuel_producer.run(mode);

    fuel_delayer1.set_input(fuel_producer.get_output());
    fuel_delayer1.run(mode);

    fuel_delayer2.set_input(fuel_delayer1.get_output());
    fuel_delayer2.run(mode);

    drum.set_input(fuel_delayer2.get_output()-delayer1.get_output());
    drum.run(mode);

    if(mode==DYNAMIC_UPDATE_MODE)
        set_flag_model_updated_as_true();
}

double IEEEG1SB::get_mechanical_power_in_pu_based_on_mbase() const
{
    return delayer1.get_output()*get_K1()+
           delayer2.get_output()*get_K3()+
           delayer3.get_output()*get_K5()+
           delayer4.get_output()*get_K7();
}

double IEEEG1SB::get_mechanical_power_upper_limit_in_pu_based_on_mbase() const
{
    return get_Pmax_in_pu();
}

double IEEEG1SB::get_mechanical_power_lower_limit_in_pu_based_on_mbase() const
{
    return get_Pmin_in_pu();
}

void IEEEG1SB::check()
{
    ;
}

void IEEEG1SB::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string(false);
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}
void IEEEG1SB::save()
{
    ;
}
string IEEEG1SB::get_standard_psse_string(bool export_internal_bus_number) const
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
    double KP = get_KP();
    double KI = get_KI();
    double Tfuel = get_Tfuel_in_s();
    double TFD1 = get_TFD1_in_s();
    double TFD2 = get_TFD2_in_s();
    double Cb = get_Cb_in_s();
    double Kb = get_Kb();


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
            <<setw(8)<<setprecision(6)<<K8<<", "
            <<setw(8)<<setprecision(6)<<KP<<", "
            <<setw(8)<<setprecision(6)<<KI<<", "
            <<setw(8)<<setprecision(6)<<Tfuel<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<TFD1<<", "
            <<setw(8)<<setprecision(6)<<TFD2<<", "
            <<setw(8)<<setprecision(6)<<Cb<<", "
            <<setw(8)<<setprecision(6)<<Kb<<" /";
    return osstream.str();
}

void IEEEG1SB::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
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
    add_model_data_name_and_index_pair("KP", i); i++;
    add_model_data_name_and_index_pair("KI", i); i++;
    add_model_data_name_and_index_pair("TFUEL", i); i++;
    add_model_data_name_and_index_pair("TFD1", i); i++;
    add_model_data_name_and_index_pair("TFD2", i); i++;
    add_model_data_name_and_index_pair("CB", i); i++;
    add_model_data_name_and_index_pair("KB", i); i++;
}

double IEEEG1SB::get_model_data_with_name(string par_name) const
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
    if(par_name=="KP")
        return get_KP();
    if(par_name=="KI")
        return get_KI();
    if(par_name=="TFUEL")
        return get_Tfuel_in_s();
    if(par_name=="TFD1")
        return get_TFD1_in_s();
    if(par_name=="TFD2")
        return get_TFD2_in_s();
    if(par_name=="CB")
        return get_Cb_in_s();
    if(par_name=="KB")
        return get_Kb();

    return 0.0;
}

void IEEEG1SB::set_model_data_with_name(string par_name, double value)
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
    if(par_name=="KP")
        return set_KP(value);
    if(par_name=="KI")
        return set_KI(value);
    if(par_name=="TFUEL")
        return set_Tfuel_in_s(value);
    if(par_name=="TFD1")
        return set_TFD1_in_s(value);
    if(par_name=="TFD2")
        return set_TFD2_in_s(value);
    if(par_name=="CB")
        return set_Cb_in_s(value);
    if(par_name=="KB")
        return set_Kb(value);
}

double IEEEG1SB::get_minimum_nonzero_time_constant_in_s()
{
    return INFINITE_THRESHOLD;
}

void IEEEG1SB::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_internal_variable_name_and_index_pair("STATE@DROOP", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@SERVO MOTOR", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@DELAYER 1", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@DELAYER 2", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@DELAYER 3", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@DELAYER 4", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@BOILER INTEGRATOR", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@FUEL PRODUCER", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@FUEL DELAYER 1", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@FUEL DELAYER 2", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@DRUM", i); i++;
    add_model_internal_variable_name_and_index_pair("STEAM PRESSURE", i); i++;
}

double IEEEG1SB::get_model_internal_variable_with_name(string var_name)
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

    if(var_name == "STATE@BOILER INTEGRATOR")
        return boiler_PI.get_state();

    if(var_name == "STATE@FUEL PRODUCER")
        return fuel_producer.get_state();

    if(var_name == "STATE@FUEL DELAYER 1")
        return fuel_delayer1.get_state();

    if(var_name == "STATE@FUEL DELAYER 2")
        return fuel_delayer2.get_state();

    if(var_name == "STATE@DRUM")
        return drum.get_state();

    if(var_name == "STEAM PRESSURE")
        return drum.get_state()-get_Kb()*delayer1.get_output()*delayer1.get_output();

    return 0.0;
}


string IEEEG1SB::get_dynamic_data_in_psse_format() const
{
    return "";
}

string IEEEG1SB::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string IEEEG1SB::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}

void IEEEG1SB::linearize()
{
    STEPS_SPARSE_MATRIX* matrix = new STEPS_SPARSE_MATRIX;
    set_linearized_matrix("GOV", matrix);
    // do linearization
    matrix = new STEPS_SPARSE_MATRIX;
    set_linearized_matrix("GOV-GEN", matrix);
    // do linearization
    matrix = new STEPS_SPARSE_MATRIX;
    set_linearized_matrix("GOV-TLC", matrix);
    // do linearization
}

void IEEEG1SB::build_linearized_matrix_ABCD()
{
    return;
}

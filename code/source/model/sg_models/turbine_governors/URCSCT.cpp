#include "header/model/sg_models/turbine_governor_model/URCSCT.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <istream>
#include <iostream>
using namespace std;
URCSCT::URCSCT()
{
    clear();
}

URCSCT::~URCSCT()
{
    clear();
}
void URCSCT::clear()
{
    prepare_model_data_table();
    prepare_model_internal_variable_table();
    //URCSCT
    set_gas_W(0.0);
    set_gas_X_in_s(0.0);
    set_gas_Y_in_s(0.1);
    set_gas_Z(1);
    set_gas_ETD_in_s(5.0);
    set_gas_TCD_in_s(0.5);
    set_gas_Prate_in_MW(100.0);
    set_gas_T_in_s(3.0);
    set_gas_max_in_pu(1.0);
    set_gas_min_in_pu(0.0);
    set_gas_ECR_in_s(5.0);
    set_gas_K3(1.0);
    set_gas_a(1.0);
    set_gas_b_in_s(0.1);
    set_gas_c(1.0);
    set_gas_Tf_in_s(0.1);
    set_gas_Kf(1.0);
    set_gas_K5(1.0);
    set_gas_K4(1.0);
    set_gas_T3_in_s(1.0);
    set_gas_T4_in_s(1.0);
    set_gas_Tt_in_s(1.0);
    set_gas_T5_in_s(1.0);
    set_gas_af1(1.0);
    set_gas_bf1(1.0);
    set_gas_af2(1.0);
    set_gas_bf2(1.0);
    set_gas_cf2(1.0);
    set_gas_TR_in_deg(200.0);
    set_gas_K6(0.1);
    set_gas_TC_in_deg(250.0);
    //IEEEG1
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
    //URCSCT
    set_ST_in_MW(100.0);
    set_PoutA_in_MW(100.0);
    set_SToutA_in_MW(100.0);
    set_PoutB_in_MW(100.0);
    set_SToutB_in_MW(100.0);
    set_PoutC_in_MW(100.0);
    set_SToutC_in_MW(100.0);
}
void URCSCT::copy_from_const_model(const URCSCT& model)
{
    clear();

    set_gas_W(model.get_gas_W());
    set_gas_X_in_s(model.get_gas_X_in_s());
    set_gas_Y_in_s(model.get_gas_Y_in_s());
    set_gas_Z(model.get_gas_Z());
    set_gas_ETD_in_s(model.get_gas_ETD_in_s());
    set_gas_TCD_in_s(model.get_gas_TCD_in_s());
    set_gas_Prate_in_MW(model.get_gas_Prate_in_MW());
    set_gas_T_in_s(model.get_gas_T_in_s());
    set_gas_max_in_pu(model.get_gas_max_in_pu());
    set_gas_min_in_pu(model.get_gas_min_in_pu());
    set_gas_ECR_in_s(model.get_gas_ECR_in_s());
    set_gas_K3(model.get_gas_K3());
    set_gas_a(model.get_gas_a());
    set_gas_b_in_s(model.get_gas_b_in_s());
    set_gas_c(model.get_gas_c());
    set_gas_Tf_in_s(model.get_gas_Tf_in_s());
    set_gas_Kf(model.get_gas_Kf());
    set_gas_K5(model.get_gas_K5());
    set_gas_K4(model.get_gas_K4());
    set_gas_T3_in_s(model.get_gas_T3_in_s());
    set_gas_T4_in_s(model.get_gas_T4_in_s());
    set_gas_Tt_in_s(model.get_gas_Tt_in_s());
    set_gas_T5_in_s(model.get_gas_T5_in_s());
    set_gas_af1(model.get_gas_af1());
    set_gas_bf1(model.get_gas_bf1());
    set_gas_af2(model.get_gas_af2());
    set_gas_bf2(model.get_gas_bf2());
    set_gas_cf2(model.get_gas_cf2());
    set_gas_TR_in_deg(model.get_gas_TR_in_deg());
    set_gas_K6(model.get_gas_K6());
    set_gas_TC_in_deg(model.get_gas_TC_in_deg());

    set_K(model.get_K());
    set_T1_in_s(model.get_T1_in_s());
    set_T2_in_s(model.get_T2_in_s());
    set_T3_in_s(model.get_T3_in_s());
    set_Uo_in_pu(model.get_Uo_in_pu());
    set_Uc_in_pu(model.get_Uc_in_pu());
    set_Pmax_in_pu(model.get_Pmax_in_pu());
    set_Pmin_in_pu(model.get_Pmin_in_pu());
    set_T4_in_s(model.get_T4_in_s());
    set_T5_in_s(model.get_T5_in_s());
    set_T6_in_s(model.get_T6_in_s());
    set_T7_in_s(model.get_T7_in_s());
    set_K1(model.get_K1());
    set_K2(model.get_K2());
    set_K3(model.get_K3());
    set_K4(model.get_K4());
    set_K5(model.get_K5());
    set_K6(model.get_K6());
    set_K7(model.get_K7());
    set_K8(model.get_K8());

    set_ST_in_MW(model.get_ST_in_MW());
    set_PoutA_in_MW(model.get_PoutA_in_MW());
    set_SToutA_in_MW(model.get_SToutA_in_MW());
    set_PoutB_in_MW(model.get_PoutB_in_MW());
    set_SToutB_in_MW(model.get_SToutB_in_MW());
    set_PoutC_in_MW(model.get_PoutC_in_MW());
    set_SToutC_in_MW(model.get_SToutC_in_MW());
}

URCSCT::URCSCT(const URCSCT&model) : TURBINE_GOVERNOR_MODEL()
{
    copy_from_const_model(model);
}

URCSCT& URCSCT::operator=(const URCSCT& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

string URCSCT::get_model_name() const
{
    return "URCSCT";
}


void URCSCT::set_gas_W(double W)
{
    gas_governor_W = W;
}

void URCSCT::set_gas_X_in_s(double T)
{
    gas_governor_X = T;
}

void URCSCT::set_gas_Y_in_s(double T)
{
    gas_governor_Y = T;
}

void URCSCT::set_gas_Z(size_t Z)
{
    gas_governor_Z = Z;
}

void URCSCT::set_gas_ETD_in_s(double T)
{
    gas_ETD = T;
}

void URCSCT::set_gas_TCD_in_s(double T)
{
    gas_turbine_dynamic.set_T_in_s(T);
}

void URCSCT::set_gas_Prate_in_MW(double P)
{
    gas_turbine_rate_MW = P;
}

void URCSCT::set_gas_T_in_s(double T)
{
    gas_T = T;
}

void URCSCT::set_gas_max_in_pu(double U)
{
    gas_governor_MAX = U;
}

void URCSCT::set_gas_min_in_pu(double U)
{
    gas_governor_MIN = U;
}

void URCSCT::set_gas_ECR_in_s(double T)
{
    gas_ECR = T;
}

void URCSCT::set_gas_K3(double K)
{
    gas_K3 = K;
}

void URCSCT::set_gas_a(double a)
{
    gas_a = a;
}

void URCSCT::set_gas_b_in_s(double T)
{
    gas_b = T;
}

void URCSCT::set_gas_c(double K)
{
    gas_c = K;
}

void URCSCT::set_gas_Tf_in_s(double T)
{
    gas_fuel_system.set_T_in_s(T);
}

void URCSCT::set_gas_Kf(double K)
{
    gas_Kf = K;
}

void URCSCT::set_gas_K5(double K)
{
    gas_radiation_shield.set_K(K);
}

void URCSCT::set_gas_K4(double K)
{
    gas_K4 = K;
}

void URCSCT::set_gas_T3_in_s(double T)
{
    gas_radiation_shield.set_T_in_s(T);
}

void URCSCT::set_gas_T4_in_s(double T)
{
    gas_thermocouple.set_T_in_s(T);
}

void URCSCT::set_gas_Tt_in_s(double T)
{
    gas_Tt = T;
}

void URCSCT::set_gas_T5_in_s(double T)
{
    gas_T5 = T;
}

void URCSCT::set_gas_af1(double K)
{
    gas_af1 = K;
}

void URCSCT::set_gas_bf1(double K)
{
    gas_bf1 = K;
}

void URCSCT::set_gas_af2(double K)
{
    gas_af2 = K;
}

void URCSCT::set_gas_bf2(double K)
{
    gas_bf2 = K;
}

void URCSCT::set_gas_cf2(double K)
{
    gas_cf2 = K;
}

void URCSCT::set_gas_TR_in_deg(double T)
{
    gas_TR = T;
}

void URCSCT::set_gas_K6(double K)
{
    gas_K6 = K;
}

void URCSCT::set_gas_TC_in_deg(double T)
{
    gas_TC = T;
}

void URCSCT::set_K(double K)
{
    droop.set_K(K);
}

void URCSCT::set_T1_in_s(double T)
{
    droop.set_T1_in_s(T);
}

void URCSCT::set_T2_in_s(double T)
{
    droop.set_T2_in_s(T);
}

void URCSCT::set_T3_in_s(double T)
{
    this->Tservo = T;
}

void URCSCT::set_Uo_in_pu(double U)
{
    this->Uo = U;
}

void URCSCT::set_Uc_in_pu(double U)
{
    this->Uc = U;
}

void URCSCT::set_Pmax_in_pu(double P)
{
    servo_motor.set_upper_limit(P);
}

void URCSCT::set_Pmin_in_pu(double P)
{
    servo_motor.set_lower_limit(P);
}

void URCSCT::set_T4_in_s(double T)
{
    delayer1.set_T_in_s(T);
}

void URCSCT::set_T5_in_s(double T)
{
    delayer2.set_T_in_s(T);
}

void URCSCT::set_T6_in_s(double T)
{
    delayer3.set_T_in_s(T);
}

void URCSCT::set_T7_in_s(double T)
{
    delayer4.set_T_in_s(T);
}

void URCSCT::set_K1(double K)
{
    this->K1 = K;
}

void URCSCT::set_K2(double K)
{
    this->K2 = K;
}

void URCSCT::set_K3(double K)
{
    this->K3 = K;
}

void URCSCT::set_K4(double K)
{
    this->K4 = K;
}

void URCSCT::set_K5(double K)
{
    this->K5 = K;
}

void URCSCT::set_K6(double K)
{
    this->K6 = K;
}

void URCSCT::set_K7(double K)
{
    this->K7 = K;
}

void URCSCT::set_K8(double K)
{
    this->K8 = K;
}


void URCSCT::set_ST_in_MW(double S)
{
    ST = S;
}

void URCSCT::set_PoutA_in_MW(double P)
{
    PoutA = P;
}

void URCSCT::set_SToutA_in_MW(double S)
{
    SToutA = S;
}

void URCSCT::set_PoutB_in_MW(double P)
{
    PoutB = P;
}

void URCSCT::set_SToutB_in_MW(double S)
{
    SToutB = S;
}

void URCSCT::set_PoutC_in_MW(double P)
{
    PoutC = P;
}

void URCSCT::set_SToutC_in_MW(double S)
{
    SToutC = S;
}


double URCSCT::get_gas_W() const
{
    return gas_governor_W;
}

double URCSCT::get_gas_X_in_s() const
{
    return gas_governor_X;
}

double URCSCT::get_gas_Y_in_s() const
{
    return gas_governor_Y;
}

size_t URCSCT::get_gas_Z() const
{
    return gas_governor_Z;
}

double URCSCT::get_gas_ETD_in_s() const
{
    return gas_ETD;
}

double URCSCT::get_gas_TCD_in_s() const
{
    return gas_turbine_dynamic.get_T_in_s();
}

double URCSCT::get_gas_Prate_in_MW() const
{
    return gas_turbine_rate_MW;
}

double URCSCT::get_gas_T_in_s() const
{
    return gas_T;
}

double URCSCT::get_gas_max_in_pu() const
{
    return gas_governor_MAX;
}

double URCSCT::get_gas_min_in_pu() const
{
    return gas_governor_MIN;
}

double URCSCT::get_gas_ECR_in_s() const
{
    return gas_ECR;
}

double URCSCT::get_gas_K3() const
{
    return gas_K3;
}

double URCSCT::get_gas_a() const
{
    return gas_a;
}

double URCSCT::get_gas_b_in_s() const
{
    return gas_b;
}

double URCSCT::get_gas_c() const
{
    return gas_c;
}

double URCSCT::get_gas_Tf_in_s() const
{
    return gas_fuel_system.get_T_in_s();
}

double URCSCT::get_gas_Kf() const
{
    return gas_Kf;
}

double URCSCT::get_gas_K5() const
{
    return gas_radiation_shield.get_K();
}

double URCSCT::get_gas_K4() const
{
    return gas_K4;
}

double URCSCT::get_gas_T3_in_s() const
{
    return gas_fuel_system.get_T_in_s();
}

double URCSCT::get_gas_T4_in_s() const
{
    return gas_thermocouple.get_T_in_s();
}

double URCSCT::get_gas_Tt_in_s() const
{
    return gas_Tt;
}

double URCSCT::get_gas_T5_in_s() const
{
    return gas_T5;
}

double URCSCT::get_gas_af1() const
{
    return gas_af1;
}

double URCSCT::get_gas_bf1() const
{
    return gas_bf1;
}

double URCSCT::get_gas_af2() const
{
    return gas_af2;
}

double URCSCT::get_gas_bf2() const
{
    return gas_bf2;
}

double URCSCT::get_gas_cf2() const
{
    return gas_cf2;
}

double URCSCT::get_gas_TR_in_deg() const
{
    return gas_TR;
}

double URCSCT::get_gas_K6() const
{
    return gas_K6;
}

double URCSCT::get_gas_TC_in_deg() const
{
    return gas_TC;
}

double URCSCT::get_K() const
{
    return droop.get_K();
}

double URCSCT::get_T1_in_s() const
{
    return droop.get_T1_in_s();
}

double URCSCT::get_T2_in_s() const
{
    return droop.get_T2_in_s();
}

double URCSCT::get_T3_in_s() const
{
    return Tservo;
}

double URCSCT::get_Uo_in_pu() const
{
    return Uo;
}

double URCSCT::get_Uc_in_pu() const
{
    return Uc;
}

double URCSCT::get_Pmax_in_pu() const
{
    return servo_motor.get_upper_limit();
}

double URCSCT::get_Pmin_in_pu() const
{
    return servo_motor.get_lower_limit();
}

double URCSCT::get_T4_in_s() const
{
    return delayer1.get_T_in_s();
}

double URCSCT::get_T5_in_s() const
{
    return delayer2.get_T_in_s();
}

double URCSCT::get_T6_in_s() const
{
    return delayer3.get_T_in_s();
}

double URCSCT::get_T7_in_s() const
{
    return delayer4.get_T_in_s();
}

double URCSCT::get_K1() const
{
    return K1;
}

double URCSCT::get_K2() const
{
    return K2;
}

double URCSCT::get_K3() const
{
    return K3;
}

double URCSCT::get_K4() const
{
    return K4;
}

double URCSCT::get_K5() const
{
    return K5;
}

double URCSCT::get_K6() const
{
    return K6;
}

double URCSCT::get_K7() const
{
    return K7;
}

double URCSCT::get_K8() const
{
    return K8;
}

double URCSCT::get_ST_in_MW() const
{
    return ST;
}

double URCSCT::get_PoutA_in_MW() const
{
    return PoutA;
}

double URCSCT::get_SToutA_in_MW() const
{
    return SToutA;
}

double URCSCT::get_PoutB_in_MW() const
{
    return PoutB;
}

double URCSCT::get_SToutB_in_MW() const
{
    return SToutB;
}

double URCSCT::get_PoutC_in_MW() const
{
    return PoutC;
}

double URCSCT::get_SToutC_in_MW() const
{
    return SToutC;
}

bool URCSCT::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=34+20+7)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            double k, t1, t2, t3, uo, uc, pmax, pmin, t4, k1, k2, t5, k3, k4, t6, k5, k6, t7, k7, k8;

            size_t i=3;
            set_gas_W(get_double_data(data[i],"0.0")); i++;
            set_gas_X_in_s(get_double_data(data[i],"0.0")); i++;
            set_gas_Y_in_s(get_double_data(data[i],"0.0")); i++;
            set_gas_Z(get_integer_data(data[i],"0.0")); i++;
            set_gas_ETD_in_s(get_double_data(data[i],"0.0")); i++;
            set_gas_TCD_in_s(get_double_data(data[i],"0.0")); i++;
            set_gas_Prate_in_MW(get_double_data(data[i],"0.0")); i++;
            set_gas_T_in_s(get_double_data(data[i],"0.0")); i++;
            set_gas_max_in_pu(get_double_data(data[i],"0.0")); i++;
            set_gas_min_in_pu(get_double_data(data[i],"0.0")); i++;
            set_gas_ECR_in_s(get_double_data(data[i],"0.0")); i++;
            set_gas_K3(get_double_data(data[i],"0.0")); i++;
            set_gas_a(get_double_data(data[i],"0.0")); i++;
            set_gas_b_in_s(get_double_data(data[i],"0.0")); i++;
            set_gas_c(get_double_data(data[i],"0.0")); i++;
            set_gas_Tf_in_s(get_double_data(data[i],"0.0")); i++;
            set_gas_Kf(get_double_data(data[i],"0.0")); i++;
            set_gas_K5(get_double_data(data[i],"0.0")); i++;
            set_gas_K4(get_double_data(data[i],"0.0")); i++;
            set_gas_T3_in_s(get_double_data(data[i],"0.0")); i++;
            set_gas_T4_in_s(get_double_data(data[i],"0.0")); i++;
            set_gas_Tt_in_s(get_double_data(data[i],"0.0")); i++;
            set_gas_T5_in_s(get_double_data(data[i],"0.0")); i++;
            set_gas_af1(get_double_data(data[i],"0.0")); i++;
            set_gas_bf1(get_double_data(data[i],"0.0")); i++;
            set_gas_af2(get_double_data(data[i],"0.0")); i++;
            set_gas_bf2(get_double_data(data[i],"0.0")); i++;
            set_gas_cf2(get_double_data(data[i],"0.0")); i++;
            set_gas_TR_in_deg(get_double_data(data[i],"0.0")); i++;
            set_gas_K6(get_double_data(data[i],"0.0")); i++;
            set_gas_TC_in_deg(get_double_data(data[i],"0.0")); i++;

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
            set_K8(get_double_data(data[i],"0.0")); i++;

            set_ST_in_MW(get_double_data(data[i],"0.0")); i++;
            set_PoutA_in_MW(get_double_data(data[i],"0.0")); i++;
            set_SToutA_in_MW(get_double_data(data[i],"0.0")); i++;
            set_PoutB_in_MW(get_double_data(data[i],"0.0")); i++;
            set_SToutB_in_MW(get_double_data(data[i],"0.0")); i++;
            set_PoutC_in_MW(get_double_data(data[i],"0.0")); i++;
            set_SToutC_in_MW(get_double_data(data[i],"0.0"));

            is_successful = true;

            return is_successful;
        }
        else
            return is_successful;
    }
    else
        return is_successful;
}

bool URCSCT::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool URCSCT::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void URCSCT::setup_block_toolkit_and_parameters()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    double delt = toolkit.get_dynamic_simulation_time_step_in_s();

    gas_governor_droop.set_toolkit(toolkit);
    gas_governor_droop.set_K(get_gas_W());
    gas_governor_droop.set_T1_in_s(get_gas_X_in_s());
    gas_governor_droop.set_T2_in_s(get_gas_Y_in_s());
    gas_governor_droop.set_limiter_type(NON_WINDUP_LIMITER);
    gas_governor_droop.set_upper_limit(get_gas_max_in_pu());
    gas_governor_droop.set_lower_limit(get_gas_min_in_pu());


    gas_governor_iso.set_toolkit(toolkit);
    gas_governor_iso.set_Kp(get_gas_W()*get_gas_X_in_s()/get_gas_Y_in_s());
    gas_governor_iso.set_Ki(get_gas_W()/get_gas_Y_in_s());
    gas_governor_iso.set_limiter_type(NON_WINDUP_LIMITER);
    gas_governor_iso.set_upper_limit(get_gas_max_in_pu());
    gas_governor_iso.set_lower_limit(get_gas_min_in_pu());

    gas_fuel_control.set_toolkit(toolkit);
    gas_fuel_control.set_buffer_size(round(get_gas_T_in_s()/delt));

    gas_valve_positioner.set_toolkit(toolkit);
    gas_valve_positioner.set_limiter_type(NO_LIMITER);
    gas_valve_positioner.set_K(get_gas_a()/get_gas_c());
    gas_valve_positioner.set_K(get_gas_b_in_s()/get_gas_c());

    gas_fuel_system.set_toolkit(toolkit);
    gas_fuel_system.set_limiter_type(NO_LIMITER);
    gas_fuel_system.set_K(1.0);
    gas_fuel_system.set_T_in_s(get_gas_Tf_in_s());

    gas_combustor.set_toolkit(toolkit);
    gas_combustor.set_buffer_size(round(get_gas_ECR_in_s()/delt));

    gas_turbine_exhaust.set_toolkit(toolkit);
    gas_turbine_exhaust.set_buffer_size(round(get_gas_ETD_in_s()/delt));

    gas_radiation_shield.set_toolkit(toolkit);
    gas_radiation_shield.set_limiter_type(NO_LIMITER);
    gas_radiation_shield.set_K(get_gas_K5());
    gas_radiation_shield.set_T_in_s(get_gas_T3_in_s());

    gas_thermocouple.set_toolkit(toolkit);
    gas_thermocouple.set_limiter_type(NO_LIMITER);
    gas_thermocouple.set_T_in_s(get_gas_T4_in_s());

    gas_temperature_control.set_toolkit(toolkit);
    gas_temperature_control.set_limiter_type(NON_WINDUP_LIMITER);
    gas_temperature_control.set_upper_limit(get_gas_max_in_pu());
    gas_temperature_control.set_lower_limit(-INFINITE_THRESHOLD);
    gas_temperature_control.set_Kp(get_gas_T5_in_s()/get_gas_Tt_in_s());
    gas_temperature_control.set_Ki(1.0/get_gas_Tt_in_s());

    gas_turbine_dynamic.set_toolkit(toolkit);
    gas_turbine_dynamic.set_limiter_type(NO_LIMITER);
    gas_turbine_dynamic.set_T_in_s(get_gas_TCD_in_s());


    droop.set_toolkit(toolkit);
    servo_motor.set_toolkit(toolkit);
    servo_motor.set_limiter_type(NON_WINDUP_LIMITER);
    delayer1.set_toolkit(toolkit);
    delayer2.set_toolkit(toolkit);
    delayer3.set_toolkit(toolkit);
    delayer4.set_toolkit(toolkit);
}

void URCSCT::initialize()
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
                double time = toolkit.get_dynamic_simulation_time_in_s();

                double pmech0 = get_initial_mechanical_power_in_pu_based_on_mbase_from_sync_generator_model();
                double mbase = get_mbase_in_MVA();

                double pplant = pmech0*mbase;
                double slope=0.0, psteam = 0.0, pgas = 0.0;
                if(pplant<=PoutB)
                {
                    slope = (PoutB-PoutA)/(SToutB-SToutA);
                    double deltaS = 0.0;
                    deltaS = (pplant-PoutA)/slope;
                    psteam = SToutA + deltaS;
                    if(psteam<0.0)
                        psteam = 0.0;
                }
                else
                {
                    if(pplant<=PoutC)
                    {
                        slope = (PoutC-PoutB)/(SToutC-SToutB);
                        double deltaS = 0.0;
                        deltaS = (pplant-PoutB)/slope;
                        psteam = SToutB + deltaS;
                    }
                    else
                    {
                        double ptotal = ST+get_gas_Prate_in_MW();
                        if(pplant<=ptotal)
                        {
                            slope = (ptotal-PoutC)/(ST-SToutC);
                            double deltaS = 0.0;
                            deltaS = (pplant-PoutC)/slope;
                            psteam = SToutC + deltaS;
                        }
                        else
                        {
                            psteam = ST;
                            osstream<<"Initialization error. Total mechanical power of URCSCT model of "<<get_device_name()<<" exceeds sum of steam and gas turbine nominal power. \n"
                                    <<"Mechanical power is "<<pplant<<"MW. Gas turbine nominal power is "<<get_gas_Prate_in_MW()<<"MW. Steam turbine nominal power is "<<ST<<"MW.";
                            toolkit.show_information_with_leading_time_stamp(osstream);
                        }
                    }
                }
                pgas = pplant-psteam;//MW

                double prate = get_gas_Prate_in_MW();

                double f2 = pgas/prate;
                double wf2 = (f2-get_gas_af2())/get_gas_bf2();

                gas_turbine_dynamic.set_output(wf2);
                gas_turbine_dynamic.initialize();

                gas_combustor.initialize_buffer(time, wf2);

                gas_turbine_exhaust.initialize_buffer(time, wf2);

                double wf1 = wf2;

                double f1 = get_gas_TR_in_deg() - get_gas_af1()*(1.0-wf1);

                gas_radiation_shield.set_output(f1*get_gas_K5());
                gas_radiation_shield.initialize();

                gas_thermocouple.set_output((get_gas_K4()+get_gas_K5())*f1);
                gas_thermocouple.initialize();

                gas_temperature_control.set_output(get_gas_max_in_pu());
                gas_temperature_control.initialize();

                gas_fuel_system.set_output(wf2);
                gas_fuel_system.initialize();

                gas_valve_positioner.set_output(wf2);
                gas_valve_positioner.initialize();

                double gfc = gas_valve_positioner.get_input()+get_gas_Kf()*wf2-get_gas_K6();
                gas_fuel_control.initialize_buffer(time, gfc);

                double output = gfc/get_gas_K3();
                if(output>get_gas_max_in_pu())
                {
                    osstream<<"Initialization error. Governor of '"<<get_model_name()<<"' model of "<<get_device_name()<<" exceeds upper limit."
                      <<"Governor is "<<output<<", and max is "<<get_gas_max_in_pu()<<".";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
                if(output<get_gas_min_in_pu())
                {
                    osstream<<"Initialization error. Governor of '"<<get_model_name()<<"' model of "<<get_device_name()<<" exceeds lower limit."
                      <<"Governor is "<<output<<", and min is "<<get_gas_min_in_pu()<<".";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }

                double input = 0.0;
                if(get_gas_Z()!=0.0)
                {
                    gas_governor_droop.set_output(output);
                    gas_governor_droop.initialize();
                    input = gas_governor_droop.get_input();
                }
                else
                {
                    gas_governor_iso.set_output(output);
                    gas_governor_iso.initialize();
                    input = gas_governor_iso.get_input();
                }
                set_initial_mechanical_power_reference_in_pu_based_on_mbase(input);


                psteam = psteam/ST;

                droop.set_output(0.0);
                droop.initialize();

                double sumK=get_K1()+get_K3()+get_K5()+get_K7();

                double valve = psteam/sumK;

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

                steam_pmech_ref = valve;

                set_flag_model_initialized_as_true();
            }
        }
    }
}

void URCSCT::run(DYNAMIC_MODE mode)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    double time = toolkit.get_dynamic_simulation_time_in_s();

    double Pref = get_mechanical_power_reference_in_pu_based_on_mbase();

    double speed = get_rotor_speed_deviation_in_pu_from_sync_generator_model();

    double input = Pref-speed;
    double output = 0.0;
    if(get_gas_Z()!=0.0)
    {
        gas_governor_droop.set_input(input);
        gas_governor_droop.run(mode);
        output = gas_governor_droop.get_output();
    }
    else
    {
        gas_governor_iso.set_input(input);
        gas_governor_iso.run(mode);
        output = gas_governor_iso.get_output();
    }

    double output2 = gas_temperature_control.get_output();
    if(get_gas_TC_in_deg()-gas_thermocouple.get_output()<0.0)
        output2 = output;

    output = (output<=output2)?output:output2;

    input = output*(1.0+speed)*get_gas_K3();

    gas_fuel_control.append_data(time, input);

    output = gas_fuel_control.get_buffer_value_at_head();

    input = output + get_gas_K6() - get_gas_Kf()*gas_fuel_system.get_output();

    gas_valve_positioner.set_input(input);
    gas_valve_positioner.run(mode);

    gas_fuel_system.set_input(gas_valve_positioner.get_output());
    gas_fuel_system.run(mode);

    gas_combustor.append_data(time, gas_fuel_system.get_output());

    double wf = gas_combustor.get_buffer_value_at_head();

    gas_turbine_exhaust.append_data(time, wf);

    double wf1 = gas_turbine_exhaust.get_buffer_time_at_head();
    double f1 = get_gas_TR_in_deg()-get_gas_af1()*(1.0-wf1)-get_gas_bf1()*speed;

    gas_radiation_shield.set_input(f1);
    gas_radiation_shield.run(mode);

    input = get_gas_K4()*f1+gas_radiation_shield.get_output();

    gas_thermocouple.set_input(input);
    gas_thermocouple.run(mode);

    gas_temperature_control.set_input(get_gas_TC_in_deg()-gas_thermocouple.get_output());
    gas_temperature_control.run(mode);

    gas_turbine_dynamic.set_input(wf);
    gas_turbine_dynamic.run(mode);


    Pref = steam_pmech_ref;

    droop.set_input(speed);
    droop.run(mode);

    input = Pref-droop.get_output()-servo_motor.get_output();

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

double URCSCT::get_mechanical_power_in_pu_based_on_mbase() const
{
    double speed = get_rotor_speed_deviation_in_pu_from_sync_generator_model();

    double wf2 = gas_turbine_dynamic.get_output();
    double f2 = get_gas_af2()+get_gas_bf2()*wf2-get_gas_cf2()*speed;

    double pmech = f2*(1.0+speed);

    double pmech_gas = pmech*get_gas_Prate_in_MW()/get_mbase_in_MVA();

    pmech = delayer1.get_output()*get_K1()+
            delayer2.get_output()*get_K3()+
            delayer3.get_output()*get_K5()+
            delayer4.get_output()*get_K7();
    double pmech_steam = pmech*ST/get_mbase_in_MVA();

    return pmech_gas+pmech_steam;
}

double URCSCT::get_mechanical_power_upper_limit_in_pu_based_on_mbase() const
{
    return get_Pmax_in_pu();
}

double URCSCT::get_mechanical_power_lower_limit_in_pu_based_on_mbase() const
{
    return get_Pmin_in_pu();
}

void URCSCT::check()
{
    ;
}

void URCSCT::report()
{
    ostringstream osstream;
    osstream<<get_standard_model_string();
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
}
void URCSCT::save()
{
    ;
}
string URCSCT::get_standard_model_string() const
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
      <<"'"<<identifier<<"', "
      <<setw(8)<<setprecision(6)<<get_gas_W()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_X_in_s()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_Y_in_s()<<", "
      <<setw(8)<<setprecision(0)<<get_gas_Z()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_ETD_in_s()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_TCD_in_s()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_Prate_in_MW()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_T_in_s()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_max_in_pu()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_min_in_pu()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_ECR_in_s()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_K3()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_a()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_b_in_s()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_c()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_Tf_in_s()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_Kf()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_K5()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_K4()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_T3_in_s()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_T4_in_s()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_Tt_in_s()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_T5_in_s()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_af1()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_bf1()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_af2()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_bf2()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_cf2()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_TR_in_deg()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_K6()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_TC_in_deg()<<", "
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
      <<setw(8)<<setprecision(6)<<K8<<", "
      <<setw(8)<<setprecision(6)<<get_ST_in_MW()<<", "
      <<setw(8)<<setprecision(6)<<get_PoutA_in_MW()<<", "
      <<setw(8)<<setprecision(6)<<get_SToutA_in_MW()<<", "
      <<setw(8)<<setprecision(6)<<get_PoutB_in_MW()<<", "
      <<setw(8)<<setprecision(6)<<get_SToutB_in_MW()<<", "
      <<setw(8)<<setprecision(6)<<get_PoutC_in_MW()<<", "
      <<setw(8)<<setprecision(6)<<get_SToutC_in_MW()<<" /";

    return osstream.str();
}

void URCSCT::prepare_model_data_table()
{
    clear_model_data_table();
    size_t i=0;
    add_model_data_name_and_index_pair("PMAX", i); i++;
    add_model_data_name_and_index_pair("PMIN", i); i++;
}

double URCSCT::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="PMAX")
        return get_Pmax_in_pu();
    if(par_name=="PMIN")
        return get_Pmin_in_pu();

    return 0.0;
}

void URCSCT::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="PMAX")
        return set_Pmax_in_pu(value);
    if(par_name=="PMIN")
        return set_Pmin_in_pu(value);
}

void URCSCT::prepare_model_internal_variable_table()
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

double URCSCT::get_model_internal_variable_with_name(string var_name)
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


string URCSCT::get_dynamic_data_in_psse_format() const
{
    return "";
}

string URCSCT::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string URCSCT::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}

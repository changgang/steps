#include "header/model/load_model/MOTOR.h"
#include "header/basic/constants.h"
#include "header/power_system_database.h"
#include "header/basic/utility.h"

MOTOR::MOTOR(POWER_SYSTEM_DATABASE* db, DEVICE_ID did) : LOAD_MODEL(db, did)
{
    motor_double_cage_type = PARALLEL_TYPE;
    Ra = 0.0;
    Xa = 0.0;
    Xm = 0.0;
    R1 = 0.0;
    X1 = 0.0;
    R2 = 0.0;
    X2 = 0.0;
    mbase = 0.0;
    multiplier = 0.0;
    H = 0.0;
    A = 0.0;
    B = 0.0;
    C = 0.0;
    D = 0.0;
    E = 0.0;
    torque_for_starting = 0.0;
    V_interrupt = 0.0;
    T_interrupt = 0.0;
    T_breaker = 0.0;

    slip = 1.0;

    const_shunt_in_MVA = 0.0;
}

MOTOR::~MOTOR()
{
    ;
}

string MOTOR::get_model_name() const
{
    return "MOTOR";
}

void MOTOR::set_motor_double_cage_type(DOUBLE_CAGE_MOTOR_TYPE mtype)
{
    motor_double_cage_type = mtype;
}

void MOTOR::set_Ra_in_pu(double ra)
{
    this->Ra = ra;
}

void MOTOR::set_Xa_in_pu(double xa)
{
    this->Xa = xa;
}

void MOTOR::set_Xm_in_pu(double xm)
{
    this->Xm = xm;
}

void MOTOR::set_R1_in_pu(double r1)
{
    this->R1 = r1;
}

void MOTOR::set_X1_in_pu(double x1)
{
    this->X1 = x1;
}

void MOTOR::set_R2_in_pu(double r2)
{
    this->R2 = r2;
}

void MOTOR::set_X2_in_pu(double x2)
{
    this->X2 = x2;
}

void MOTOR::set_X_in_pu(double x)
{
    this->X = x;
}

void MOTOR::set_X_leakage_in_pu(double x)
{
    this->Xleakage = x;
}

void MOTOR::set_Xp_in_pu(double x)
{
    this->Xp = x;
}

void MOTOR::set_Xpp_in_pu(double x)
{
    this->Xpp = x;
}

void MOTOR::set_T0p_in_s(double T)
{
    this->T0p = T;
}

void MOTOR::set_T0pp_in_s(double T)
{
    this->T0pp = T;
}

void MOTOR::set_motor_mbase_in_MVA(double mbase)
{
    this->mbase = mbase;
}

void MOTOR::set_motor_mbase_multiplier_in_pu(double mult)
{
    this->multiplier = mult;
}

void MOTOR::set_H_in_s(double h)
{
    this->H = h;
}

void MOTOR::set_damping_A(double a)
{
    this->A = a;
}

void MOTOR::set_damping_B(double b)
{
    this->B = b;
}

void MOTOR::set_damping_C(double c)
{
    this->C = c;
}

void MOTOR::set_damping_D(double d)
{
    this->D = d;
}

void MOTOR::set_damping_E(double e)
{
    this->E = e;
}

void MOTOR::set_nominal_torque_for_motor_starting_in_pu(double Tnom)
{
    this->torque_for_starting = Tnom;
}

void MOTOR::set_voltage_threshold_of_under_voltage_relay_in_pu(double V)
{
    this->V_interrupt = V;
}

void MOTOR::set_timer_of_under_voltage_relay_in_s(double T)
{
    this->T_interrupt = T;
}

void MOTOR::set_timer_of_breaker_in_s(double T)
{
    this->T_breaker = T;
}

void MOTOR::set_slip_in_pu(double slip)
{
    this->slip = slip;
}

DOUBLE_CAGE_MOTOR_TYPE MOTOR::get_motor_double_cage_type() const
{
    return motor_double_cage_type;
}

double MOTOR::get_Ra_in_pu() const
{
    return Ra;
}

double MOTOR::get_Xa_in_pu() const
{
    return Xa;
}

double MOTOR::get_Xm_in_pu() const
{
    return Xm;
}

double MOTOR::get_R1_in_pu() const
{
    return R1;
}

double MOTOR::get_X1_in_pu() const
{
    return X1;
}

double MOTOR::get_R2_in_pu() const
{
    return R2;
}

double MOTOR::get_X2_in_pu() const
{
    return X2;
}

double MOTOR::get_X_in_pu() const
{
    return X;
}

double MOTOR::get_X_leakage_in_pu() const
{
    return Xleakage;
}

double MOTOR::get_Xp_in_pu() const
{
    return Xp;
}

double MOTOR::get_Xpp_in_pu() const
{
    return Xpp;
}

double MOTOR::get_T0p_in_s() const
{
    return T0p;
}

double MOTOR::get_T0pp_in_s() const
{
    return T0pp;
}

double MOTOR::get_motor_mbase_in_MVA() const
{
    return mbase;
}

double MOTOR::get_motor_mbase_multiplier_in_pu() const
{
    return multiplier;
}

double MOTOR::get_H_in_s() const
{
    return H;
}

double MOTOR::get_damping_A() const
{
    return A;
}

double MOTOR::get_damping_B() const
{
    return B;
}

double MOTOR::get_damping_C() const
{
    return C;
}

double MOTOR::get_damping_D() const
{
    return D;
}

double MOTOR::get_damping_E() const
{
    return E;
}

double MOTOR::get_nominal_torque_for_motor_starting_in_pu() const
{
    return torque_for_starting;
}

double MOTOR::get_voltage_threshold_of_under_voltage_relay_in_pu() const
{
    return V_interrupt;
}

double MOTOR::get_timer_of_under_voltage_relay_in_s() const
{
    return T_interrupt;
}

double MOTOR::get_timer_of_breaker_in_s() const
{
    return T_breaker;
}

double MOTOR::get_slip_in_pu() const
{
    return slip;
}


void MOTOR::convert_to_flux_linkage_model()
{
    ostringstream sstream;
    if(is_in_flux_linkage_model_type())
        return;

    POWER_SYSTEM_DATABASE* db = get_power_system_database();
    if(db == NULL)
    {
        sstream<<"MOTOR model is not assigned to any power system database.\n"
                      "Conversion into flux linkage model failed");
        show_information_with_leading_time_stamp(sstream);
        return;
    }
    double fbase = db->get_system_base_frequency_in_Hz();

    set_X_in_pu(Xa+Xm);
    set_X_leakage_in_pu(Xa);
    set_Xp_in_pu(Xa+1.0/(1.0/Xm+1.0/X1));
    set_T0p_in_s((Xm+X1)/(2*PI*fbase*R1));
    if(X2==0.0)
    {
        set_Xpp_in_pu(0.0);
        set_T0pp_in_s(0.0);
    }
    else
    {
        set_Xpp_in_pu(Xa+1.0/((1.0/Xm+1.0/X1+1.0/X2)));
        set_T0pp_in_s((X2+Xm*X1/(Xm+X1))/(2*PI*fbase*R2));
    }
}
void MOTOR::convert_to_equivalent_circuit_model()
{
    ostringstream sstream;
    if(is_in_equivalent_circuit_model_type())
        return;

    POWER_SYSTEM_DATABASE* db = get_power_system_database();
    if(db == NULL)
    {
        sstream<<"MOTOR model is not assigned to any power system database.\n"
                      "Conversion into equivalent circuit model failed");
        show_information_with_leading_time_stamp(sstream);
        return;
    }
    double fbase = db->get_system_base_frequency_in_Hz();

}

bool MOTOR::is_in_flux_linkage_model_type() const
{
    if(X!=0.0)
        return true;
    else
        return false;
}
bool MOTOR::is_in_equivalent_circuit_model_type() const
{
    if(Xa!=0.0)
        return true;
    else
        return false;
}
complex<double> MOTOR::get_motor_power_with_slip(double slip)
{
    double V = get_bus_voltage_in_pu();
    complex<double> Za, Z1, Z2;
    Za = complex<double>(Ra, Xa);
    Z1 = complex<double>(R1/slip, X1);
    if(R2==0 and X2==0)
        Z2 = 0.0;
    else
        Z2 = complex<double>(R2/slip, X2);

    complex<double> Y, Z;
    if(motor_double_cage_type==PARALLEL_TYPE)
    {
        Y = 1.0/Z1;
        if(Z2!=0.0)
            Y += 1.0/Z2;
        Z = 1.0/Y;
    }
    else
    {
        if(Z2==0.0)
            Z = Z1;
        else
        {
            Y = 1.0/Z2;
            Y += 1.0/(R1/slip);
            Z = 1.0/Y;
            Z += complex<double>(0.0, X1);
        }
    }
    Y = 1.0/Z;
    Y += 1.0/(complex<double>(0.0, Xm));
    Z = 1.0/Y;
    Z += Za;

    return V*V/Z;
}

void MOTOR::initialize()
{
    complex<double> S0 = get_load_initial_power_in_MVA();
    double P0 = S0.real();

    double V = get_load_bus_voltage_in_pu();

    double s1 = 0.02, s2 = 0.1;

    complex<double> S1, S2;
    S1 = get_motor_power_with_slip(s1);
    S2 = get_motor_power_with_slip(s2);

    double P1 = S1.real(), P2 = S2.real();

    double s3;
    s3 = s1-(P1-P0)/((P2-P1)/(s2-s1));





}
void MOTOR::run(DYNAMIC_MODE mode)
{
    ;
}

void MOTOR::check()
{
    ;
}

void MOTOR::report()
{
    ;
}

void MOTOR::save()
{
    ;
}

double MOTOR::get_variable_with_index(size_t var_index)
{
    switch(var_index)
    {
        case 0:
            return 0.0;
        case 1:
            return 0.0;
        default:
            return 0.0;
    }
}

double MOTOR::get_variable_with_name(string var_name)
{
    size_t index = get_variable_index_from_variable_name(var_name);
    return get_variable_with_index(index);
}

size_t MOTOR::get_variable_index_from_variable_name(string var_name)
{
    return INDEX_NOT_EXIST;
}

complex<double> MOTOR::get_load_actual_power_in_MVA() const
{
    return 0.0;
}

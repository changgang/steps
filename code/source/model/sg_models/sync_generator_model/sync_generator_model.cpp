#include "header/model/sg_models/sync_generator_model/sync_generator_model.h"
#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include "header/steps_namespace.h"
#include <cstdio>
#include <iostream>

SYNC_GENERATOR_MODEL::SYNC_GENERATOR_MODEL()
{
    common_constructor();
}

void SYNC_GENERATOR_MODEL::common_constructor()
{
    set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR("GENERATOR");

    set_Rs(0.0);
    set_Xd(0.0);
    set_Xq(0.0);
    set_Xdp(0.0);
    set_Xqp(0.0);
    set_Xdpp(0.0);
    set_Xqpp(0.0);
    set_Xl(0.0);
    set_D(0.0);
    set_saturation_at_1(0.0);
    set_saturation_at_1p2(0.0);
    set_saturation_type(QUADRATIC_SATURATION_TYPE);

    rotor_angle_block.set_T_in_s(1.0);

    set_initial_excitation_voltage_in_pu(0.0);
    set_initial_mechanical_power_in_pu_based_on_mbase(0.0);
}
SYNC_GENERATOR_MODEL::~SYNC_GENERATOR_MODEL()
{
    ;
}

string SYNC_GENERATOR_MODEL::get_model_type() const
{
    return "SYNC GENERATOR";
}

void SYNC_GENERATOR_MODEL::set_Rs(double rs)
{
    this->Rs = rs;
}

void SYNC_GENERATOR_MODEL::set_Xd(double xd)
{
    this->Xd = xd;
}

void SYNC_GENERATOR_MODEL::set_Xq(double xq)
{
    this->Xq = xq;
}

void SYNC_GENERATOR_MODEL::set_Xdp(double xdp)
{
    this->Xdp = xdp;
}

void SYNC_GENERATOR_MODEL::set_Xqp(double xqp)
{
    this->Xqp = xqp;
}

void SYNC_GENERATOR_MODEL::set_Xdpp(double xdpp)
{
    this->Xdpp = xdpp;
}

void SYNC_GENERATOR_MODEL::set_Xqpp(double xqpp)
{
    this->Xqpp = xqpp;
}

void SYNC_GENERATOR_MODEL::set_Xpp(double xpp)
{
    set_Xdpp(xpp);
    set_Xqpp(xpp);
}

void SYNC_GENERATOR_MODEL::set_Xl(double xl)
{
    this->Xl = xl;
}

void SYNC_GENERATOR_MODEL::set_Td0p_in_s(double T)
{
    transient_block_d_axis.set_T_in_s(T);
}

void SYNC_GENERATOR_MODEL::set_Tq0p_in_s(double T)
{
    transient_block_q_axis.set_T_in_s(T);
}

void SYNC_GENERATOR_MODEL::set_Td0pp_in_s(double T)
{
    subtransient_block_d_axis.set_T_in_s(T);
}

void SYNC_GENERATOR_MODEL::set_Tq0pp_in_s(double T)
{
    subtransient_block_q_axis.set_T_in_s(T);
}

void SYNC_GENERATOR_MODEL::set_H_in_s(double H)
{
    set_Tj_in_s(2.0*H);
}

void SYNC_GENERATOR_MODEL::set_Tj_in_s(double tj)
{
    rotor_speed_block.set_T_in_s(tj);
}

void SYNC_GENERATOR_MODEL::set_D(double d)
{
    this->D = d;
}

void SYNC_GENERATOR_MODEL::set_saturation_at_1(double s)
{
    saturation_block.set_V1(1.0);
    saturation_block.set_S1(s);
    //S1 = s;
}

void SYNC_GENERATOR_MODEL::set_saturation_at_1p2(double s)
{
    saturation_block.set_V2(1.2);
    saturation_block.set_S2(s);
    //S2 = s;
}

void SYNC_GENERATOR_MODEL::set_saturation_type(SATURATION_TYPE sat_type)
{
    saturation_block.set_saturation_type(sat_type);
    //saturation_type = sat_type;
}


double SYNC_GENERATOR_MODEL::get_Rs() const
{
    return Rs;
}

double SYNC_GENERATOR_MODEL::get_Xd() const
{
    return Xd;
}

double SYNC_GENERATOR_MODEL::get_Xq() const
{
    return Xq;
}

double SYNC_GENERATOR_MODEL::get_Xdp() const
{
    return Xdp;
}

double SYNC_GENERATOR_MODEL::get_Xqp() const
{
    return Xqp;
}

double SYNC_GENERATOR_MODEL::get_Xdpp() const
{
    return Xdpp;
}

double SYNC_GENERATOR_MODEL::get_Xqpp() const
{
    return Xqpp;
}

double SYNC_GENERATOR_MODEL::get_Xpp() const
{
    if(Xdpp!=Xqpp)
    {
        ostringstream osstream;
        osstream<<"Warning. Subtransient Xd\"("<<Xdpp<<") is not equal to Xq\"("<<Xqpp<<"). Xd\" will be returned.";
        show_information_with_leading_time_stamp(osstream);
    }
    return Xdpp;
}

double SYNC_GENERATOR_MODEL::get_Xl() const
{
    return Xl;
}

double SYNC_GENERATOR_MODEL::get_Td0p_in_s() const
{
    return transient_block_d_axis.get_T_in_s();
}

double SYNC_GENERATOR_MODEL::get_Tq0p_in_s() const
{
    return transient_block_q_axis.get_T_in_s();
}

double SYNC_GENERATOR_MODEL::get_Td0pp_in_s() const
{
    return subtransient_block_d_axis.get_T_in_s();
}

double SYNC_GENERATOR_MODEL::get_Tq0pp_in_s() const
{
    return subtransient_block_q_axis.get_T_in_s();
}

double SYNC_GENERATOR_MODEL::get_H_in_s() const
{
    return get_Tj_in_s()*0.5;
}

double SYNC_GENERATOR_MODEL::get_Tj_in_s() const
{
    return rotor_speed_block.get_T_in_s();
}

double SYNC_GENERATOR_MODEL::get_D() const
{
    return D;
}

double SYNC_GENERATOR_MODEL::get_saturation_at_1() const
{
    return saturation_block.get_S1();
    //return S1;
}

double SYNC_GENERATOR_MODEL::get_saturation_at_1p2() const
{
    return saturation_block.get_S2();
    //return S2;
}


SATURATION_TYPE SYNC_GENERATOR_MODEL::get_saturation_type() const
{
    return saturation_block.get_saturation_type();
    //return saturation_type;
}

INTEGRAL_BLOCK* SYNC_GENERATOR_MODEL::get_rotor_speed_block()
{
    return &rotor_speed_block;
}

INTEGRAL_BLOCK* SYNC_GENERATOR_MODEL::get_rotor_angle_block()
{
    return &rotor_angle_block;
}

INTEGRAL_BLOCK* SYNC_GENERATOR_MODEL::get_d_axis_transient_block()
{
    return &transient_block_d_axis;
}

INTEGRAL_BLOCK* SYNC_GENERATOR_MODEL::get_d_axis_subtransient_block()
{
    return &subtransient_block_d_axis;
}

INTEGRAL_BLOCK* SYNC_GENERATOR_MODEL::get_q_axis_transient_block()
{
    return &transient_block_q_axis;
}

INTEGRAL_BLOCK* SYNC_GENERATOR_MODEL::get_q_axis_subtransient_block()
{
    return &subtransient_block_q_axis;
}

bool SYNC_GENERATOR_MODEL::is_saturation_considered() const
{
    return saturation_block.is_saturation_considered();
}

double SYNC_GENERATOR_MODEL::get_saturation_with_flux(double flux) const
{
    if(not is_saturation_considered())
        return 0.0;

    return saturation_block.get_saturation(flux);
    /*
    switch(get_saturation_type())
    {
        case EXPONENTIAL_SATURATION_TYPE:
            return get_exponential_saturation_with_flux(flux);
        case QUADRATIC_SATURATION_TYPE:
            return get_quadratic_saturation_with_flux(flux);
        default:
            return 0.0;
    }
    */
}
/*
double SYNC_GENERATOR_MODEL::get_exponential_saturation_with_flux(double flux) const
{
    double s1 = get_saturation_at_1();
    double s2 = get_saturation_at_1p2();

    double x = s1/s2;
    x = log(x)/log(1.2);

    return s1*pow(flux, x);
}

double SYNC_GENERATOR_MODEL::get_quadratic_saturation_with_flux(double flux) const
{
    double s1 = get_saturation_at_1();
    double s2 = get_saturation_at_1p2();

    double x = sqrt(s1/s2); // another version in PSS/E is x = sqrt(S1/(1.2*S2));
    double A = (1.0-1.2*x)/(1.0-x);
    double B = s1/((1.0-A)*(1.0-A));

    return B*(flux-A)*(flux-A);
}
*/
void SYNC_GENERATOR_MODEL::set_initial_mechanical_power_in_pu_based_on_mbase(double pmech)
{
    Pmech0 = pmech;
}

double SYNC_GENERATOR_MODEL::get_initial_mechanical_power_in_pu_based_on_mbase() const
{
    return Pmech0;
}

void SYNC_GENERATOR_MODEL::set_initial_excitation_voltage_in_pu(double efd)
{
    Efd0 = efd;
}

double SYNC_GENERATOR_MODEL::get_initial_excitation_voltage_in_pu() const
{
    return Efd0;
}

double SYNC_GENERATOR_MODEL::get_mechanical_power_in_pu_based_on_mbase() const
{
    GENERATOR* generator = get_generator_pointer();
    if(generator != NULL)
    {
        TURBINE_GOVERNOR_MODEL* turbine_governor = generator->get_turbine_governor_model();
        if(turbine_governor != NULL)
        {
            if(turbine_governor->is_model_initialized())
                return turbine_governor->get_mechanical_power_in_pu_based_on_mbase();
            else
                return get_initial_mechanical_power_in_pu_based_on_mbase();
        }
        else
            return get_initial_mechanical_power_in_pu_based_on_mbase();
    }
    else
        return 0.0;
}

double SYNC_GENERATOR_MODEL::get_mechanical_power_in_MW() const
{
    return get_mechanical_power_in_pu_based_on_mbase()*get_mbase_in_MVA();
}

double SYNC_GENERATOR_MODEL::get_excitation_voltage_in_pu() const
{
    GENERATOR* generator = get_generator_pointer();
    if(generator != NULL)
    {
        EXCITER_MODEL* exciter = generator->get_exciter_model();
        if(exciter != NULL)
        {
            if(exciter->is_model_initialized())
                return exciter->get_excitation_voltage_in_pu();
            else
                return get_initial_excitation_voltage_in_pu();
        }
        else
            return get_initial_excitation_voltage_in_pu();
    }
    else
        return 0.0;
}

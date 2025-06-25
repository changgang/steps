#include "header/model/common_model/converter_common_model/current_order_limiter_model/convcol1.h"

CONVCOL1::CONVCOL1(STEPS& toolkit):COMMON_MODEL(toolkit)
{
    set_current_limit_mode(SPECIFIED_MAXIMUN_DQ_COMPONENT_OF_I_LIMITED);
    set_dq_oriented_flag(ACL_ORIENTED);
    set_phi_i_in_deg(0.0);
    set_Imax(INFINITE_THRESHOLD);
}

CONVCOL1::~CONVCOL1()
{
}

string CONVCOL1::get_model_name() const
{
    return "CONVCOL1";
}

void CONVCOL1::set_current_limit_mode(CURRENT_LIMIT_MODE current_limit_mode)
{
    this->current_limit_mode = current_limit_mode;
}

void CONVCOL1::set_dq_oriented_flag(DQ_ORIENTED_FLAG dq_oriented_flag)
{
    this->dq_oriented_flag = dq_oriented_flag;
}

void CONVCOL1::set_phi_i_in_deg(double phi_i_in_deg)
{
    this->phi_i_in_rad = deg2rad(phi_i_in_deg);
}

void CONVCOL1::set_phi_i_in_rad(double phi_i_in_rad)
{
    this->phi_i_in_rad = phi_i_in_rad;
}

void CONVCOL1::set_Imax(double I)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    if(I<0.0)
    {
        osstream<<"Warning. Negative Imax ("<<I<<") is given to set "<<get_model_name()<<" of "<<get_compound_device_name()<<". Automatically changed to positive value ("<<-I<<").";
        toolkit.show_information_with_leading_time_stamp(osstream);
        I = -I;
    }
    Imax = I;
}

CURRENT_LIMIT_MODE CONVCOL1::get_current_limit_mode() const
{
    return current_limit_mode;
}

DQ_ORIENTED_FLAG CONVCOL1::get_dq_oriented_flag() const
{
    return dq_oriented_flag;
}

double CONVCOL1::get_phi_i_in_deg() const
{
    return rad2deg(phi_i_in_rad);
}

double CONVCOL1::get_phi_i_in_rad() const
{
    return phi_i_in_rad;
}

double CONVCOL1::get_Imax() const
{
    return Imax;
}

complex<double> CONVCOL1::get_limited_xy_current_order(complex<double> Ixy_guess, double virtual_angle_in_rad, double pll_angle_in_rad) const
{
    //Ixy_guess->Ixy_limit
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    double Imax = get_Imax();
    if(Imax > 0)
    {
        if(steps_fast_complex_abs(Ixy_guess) > Imax)
        {
            double angle_in_rad, Id, Iq, Ix, Iy;

            if(dq_oriented_flag == ACL_ORIENTED)
                angle_in_rad = virtual_angle_in_rad;
            else
                angle_in_rad = pll_angle_in_rad;

            double Ix_guess = Ixy_guess.real();
            double Iy_guess = Ixy_guess.imag();
            double sine = steps_sin(angle_in_rad + phi_i_in_rad);
            double cosine = steps_cos(angle_in_rad + phi_i_in_rad);
            double Id_guess = Ix_guess*cosine + Iy_guess*sine;
            double Iq_guess =-Ix_guess*sine + Iy_guess*cosine;

            switch(current_limit_mode)
            {
                case DQ_PROPORTION_LIMIT:
                {
                    Ix = Imax * Ix_guess / steps_fast_complex_abs(Ixy_guess);
                    Iy = Imax * Iy_guess / steps_fast_complex_abs(Ixy_guess);
                    break;
                }
                case SPECIFIED_MAXIMUN_DQ_COMPONENT_OF_I_LIMITED:
                {
                    double Iqmax = 0.0;
                    double Iqmin = 0.0;
                    if(fabs(Id_guess) < Imax)
                    {
                        Iqmax = steps_sqrt(Imax*Imax - Id_guess*Id_guess);
                        Iqmin = -Iqmax;
                    }
                    if(Id_guess > Imax) Id = Imax;
                    if(Id_guess < -Imax) Id = -Imax;
                    if(Iq_guess > Iqmax) Iq = Iqmax;
                    if(Iq_guess < -Iqmax) Iq = Iqmin;
                    Ix = Id*cosine - Iq*sine;
                    Iy = Id*sine + Iq*cosine;
                    break;
                }
                case SPECIFIED_I_LIMITED:
                default:
                {
                    //osstream<<"Ix_guess = "<<Ix_guess<<" Iy_guess = "<<Iy_guess<<" Ixy = "<<steps_fast_complex_abs(Ixy_guess);
                    //toolkit.show_information_with_leading_time_stamp(osstream);
                    Ix = Imax * cosine;
                    Iy = Imax * sine;
                    break;
                }
            }
            return complex<double>(Ix, Iy);
        }
        else
        {
            return Ixy_guess;
        }
    }
    else
    {
        osstream<<"Warning. CONVCOL1 has ZERO Imax. No current order will be limited. Maybe need data check.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return Ixy_guess;
    }
}

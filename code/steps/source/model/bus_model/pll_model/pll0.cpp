#include "header/model/bus_model/pll_model/pll0.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
#include "header/basic/utility.h"
#include <istream>
#include <iostream>

using namespace std;

PLL0::PLL0(STEPS& toolkit) : PLL_MODEL(toolkit),
                             frequency_deviation_in_Hz_block(toolkit),
                             angle_in_rad_block(toolkit)
{
    clear();
}

PLL0::~PLL0()
{
    ;
}

PLL0::PLL0(const PLL0& model) : PLL_MODEL(model.get_toolkit()),
                                frequency_deviation_in_Hz_block(model.get_toolkit()),
                                angle_in_rad_block(model.get_toolkit())
{
    copy_from_const_model(model);
}

PLL0& PLL0::operator=(const PLL0& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

void PLL0::clear()
{
    frequency_deviation_in_Hz_block.set_limiter_type(NON_WINDUP_LIMITER);
    angle_in_rad_block.set_T_in_s(INFINITE_THRESHOLD);
    set_Kp(0.0);
    set_Ki(0.0);
    set_Pllmax(0.0);
}

void PLL0::copy_from_const_model(const PLL0& model)
{
    clear();
    set_Kp(model.get_Kp());
    set_Ki(model.get_Ki());
    set_Pllmax(model.get_Pllmax());
}

void PLL0::set_Kp(double K)
{
    this->Kp = K;
}

void PLL0::set_Ki(double K)
{
    if(fabs(K)>FLOAT_EPSILON)
        frequency_deviation_in_Hz_block.set_T_in_s(1.0/K);
    else
        frequency_deviation_in_Hz_block.set_T_in_s(INFINITE_THRESHOLD);
}

void PLL0::set_Pllmax(double P)
{
    frequency_deviation_in_Hz_block.set_upper_limit(P);
}

void PLL0::set_Pllmin(double P)
{
    frequency_deviation_in_Hz_block.set_lower_limit(P);
}

double PLL0::get_Kp() const
{
    return this->Kp;
}

double PLL0::get_Ki() const
{
    double T = frequency_deviation_in_Hz_block.get_T_in_s();
    if(fabs(T-INFINITE_THRESHOLD)>FLOAT_EPSILON)
        return 1.0/frequency_deviation_in_Hz_block.get_T_in_s();
    else
        return 0.0;
}
double PLL0::get_Pllmax() const
{
    return frequency_deviation_in_Hz_block.get_upper_limit();
}
double PLL0::get_Pllmin() const
{
    return frequency_deviation_in_Hz_block.get_lower_limit();
}

double PLL0::get_frquency_deviation_block_state() const
{
    return frequency_deviation_in_Hz_block.get_state();
}

double PLL0::get_angle_block_state() const
{
    return angle_in_rad_block.get_state();
}
void PLL0::initialize()
{
    initialize_base_frequency_and_base_period();

    angle_in_rad_block.set_output(get_terminal_voltage_angle_in_rad());
    angle_in_rad_block.initialize();

    frequency_deviation_in_Hz_block.set_output(0.0);
    frequency_deviation_in_Hz_block.initialize();
}

void PLL0::run(DYNAMIC_MODE mode)
{
    if(mode==INTEGRATE_MODE or mode==UPDATE_MODE)
    {
        double kp = get_Kp();
        double ki = get_Ki();
        if(kp!=0.0 or ki!=0.0)
        {
            complex<double> Vxy = get_terminal_complex_voltage_in_pu();
            double Vr = Vxy.real();
            double Vi = Vxy.imag();

            double angle = get_pll_angle_in_rad();
            double Vy = -Vr*steps_sin(angle)+Vi*steps_cos(angle);

            double wbase = get_base_angle_speed_in_radps();
            double input = Vy*kp/wbase;
            if(ki!=0)
            {
                frequency_deviation_in_Hz_block.set_input(input);
                frequency_deviation_in_Hz_block.run(mode);

                double output = frequency_deviation_in_Hz_block.get_output();
                input += output;
            }

            double pllmax = get_Pllmax();
            double pllmin = get_Pllmin();
            if(input>=pllmin and input<=pllmax)
                ;
            else
            {
                if(input>pllmax)
                    input = pllmax;
                else
                    input = pllmin;
            }

            angle_in_rad_block.set_input(input);
            angle_in_rad_block.run(mode);
        }
    }
    else
    {
        if(mode==INITIALIZE_MODE)
            initialize();
    }
}

double PLL0::get_pll_frequency_deviation_in_Hz() const
{
    //double V = get_terminal_voltage_in_pu();
    double kp = get_Kp();
    double ki = get_Ki();
    if(kp!=0.0 or ki!=0.0)
    {
        complex<double> Vxy = get_terminal_complex_voltage_in_pu();
        double Vr = Vxy.real();
        double Vi = Vxy.imag();

        double angle = get_pll_angle_in_rad();
        double Vy = -Vr*steps_sin(angle)+Vi*steps_cos(angle);

        double wbase = get_base_angle_speed_in_radps();
        double input = Vy*kp/wbase;
        if(ki!=0)
        {
            double output = frequency_deviation_in_Hz_block.get_output();
            input += output;
        }

        double pllmax = get_Pllmax();
        double pllmin = get_Pllmin();
        if(input>=pllmin and input<=pllmax)
            ;
        else
        {
            if(input>pllmax)
                input = pllmax;
            else
                input = pllmin;
        }
        return input;
    }
    else
        return 0.0;
}

double PLL0::get_pll_angle_in_rad() const
{
    double kp = get_Kp();
    double ki = get_Ki();
    if(kp!=0.0 or ki!=0.0)
        return angle_in_rad_block.get_output();
    else
        return get_terminal_voltage_angle_in_rad();
}

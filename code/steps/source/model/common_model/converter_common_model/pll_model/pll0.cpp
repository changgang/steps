#include "header/model/common_model/converter_common_model/pll_model/pll0.h"

PLL0::PLL0(STEPS& toolkit) : PLL_MODEL(toolkit),
                             frequency_deviation_block(toolkit),
                             angle_in_rad_block(toolkit)
{
    clear();
}

PLL0::~PLL0()
{
    ;
}

PLL0::PLL0(const PLL0& model) : PLL_MODEL(model.get_toolkit()),
                                frequency_deviation_block(model.get_toolkit()),
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
    frequency_deviation_block.set_limiter_type(NON_WINDUP_LIMITER);
    angle_in_rad_block.set_limiter_type(NO_LIMITER);
    angle_in_rad_block.set_T_in_s(INFINITE_THRESHOLD);
    set_Kp(0.0);
    set_Ki(0.0);
    set_Pllmax(INFINITE_THRESHOLD);
    set_Pllmin(-INFINITE_THRESHOLD);
    ideal_pll_flag = true;
}

void PLL0::copy_from_const_model(const PLL0& model)
{
    clear();
    set_Kp(model.get_Kp());
    set_Ki(model.get_Ki());
    set_Pllmax(model.get_Pllmax());
}

string PLL0::get_model_name() const
{
    return "PLL0";
}

void PLL0::set_Kp(double K)
{
    this->Kp = K;
}

void PLL0::set_Ki(double K)
{
    Ki = K;
    if(fabs(K)>FLOAT_EPSILON)
        frequency_deviation_block.set_T_in_s(1.0/K);
    else
        frequency_deviation_block.set_T_in_s(INFINITE_THRESHOLD);
}

void PLL0::set_Pllmax(double P)
{
    frequency_deviation_block.set_upper_limit(P);
}

void PLL0::set_Pllmin(double P)
{
    frequency_deviation_block.set_lower_limit(P);
}

double PLL0::get_Kp() const
{
    return this->Kp;
}

double PLL0::get_Ki() const
{
    double T = frequency_deviation_block.get_T_in_s();
    if(fabs(T-INFINITE_THRESHOLD)>FLOAT_EPSILON)
        return 1.0/frequency_deviation_block.get_T_in_s();
    else
        return 0.0;
}
double PLL0::get_Pllmax() const
{
    return frequency_deviation_block.get_upper_limit();
}
double PLL0::get_Pllmin() const
{
    return frequency_deviation_block.get_lower_limit();
}

double PLL0::get_frquency_deviation_block_state() const
{
    return frequency_deviation_block.get_state();
}

double PLL0::get_angle_block_state() const
{
    return angle_in_rad_block.get_state();
}

void PLL0::initialize()
{
    angle_in_rad_block.set_output(get_bus_voltage_angle_in_rad());
    angle_in_rad_block.set_T_in_s(1.0/get_base_angle_speed_in_radps());
    angle_in_rad_block.initialize();

    frequency_deviation_block.set_output(0.0);
    frequency_deviation_block.initialize();

    Ki = get_Ki();
    if(Kp==0.0 and Ki==0.0)
        ideal_pll_flag = true;
    else
        ideal_pll_flag = false;
}

void PLL0::run(DYNAMIC_MODE mode)
{
    if(ideal_pll_flag == false)
    {
        complex<double> Vxy = get_bus_complex_voltage_in_pu();
        double Vr = Vxy.real();
        double Vi = Vxy.imag();

        double angle = get_pll_angle_in_rad();
        double Vy = -Vr*steps_sin(angle)+Vi*steps_cos(angle);

        double wbase = get_base_angle_speed_in_radps();
        double input = Vy*Kp/wbase;
        if(Ki!=0)
        {
            frequency_deviation_block.set_input(input);
            frequency_deviation_block.run(mode);

            double output = frequency_deviation_block.get_output();
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

double PLL0::get_pll_frequency_deviation_in_pu() const
{
    if(ideal_pll_flag == false)
    {
        complex<double> Vxy = get_bus_complex_voltage_in_pu();
        double Vr = Vxy.real();
        double Vi = Vxy.imag();

        double angle = get_pll_angle_in_rad();
        double Vy = -Vr*steps_sin(angle)+Vi*steps_cos(angle);

        double wbase = get_base_angle_speed_in_radps();
        double input = Vy*Kp/wbase;
        if(Ki!=0)
        {
            double output = frequency_deviation_block.get_output();
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
    if(ideal_pll_flag == false)
        return angle_in_rad_block.get_output();
    else
        return get_bus_voltage_angle_in_rad();
}

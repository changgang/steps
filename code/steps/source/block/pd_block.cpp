#include "header/block/pd_block.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <istream>
#include <iostream>

using namespace std;

PD_BLOCK::PD_BLOCK(STEPS& toolkit) : BLOCK(toolkit), p_block(toolkit), d_block(toolkit)
{
    set_Kp(1.0);
    set_Kd(1.0);
    set_Td_in_s(999.0);
}

PD_BLOCK::~PD_BLOCK()
{
    ;
}

void PD_BLOCK::set_Kp(double K)
{
    /*if(K==0)
    {
        ostringstream osstream;
        osstream<<"Error. Zero amplifier Kp of PROPORTIONAL_BLOCK part is not allowed for PD_BLOCK.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }*/
    p_block.set_K(K);
}

double PD_BLOCK::get_Kp() const
{
    return p_block.get_K();
}

void PD_BLOCK::set_Kd(double K)
{
    d_block.set_K(K);
}

double PD_BLOCK::get_Kd() const
{
    return d_block.get_K();
}

void PD_BLOCK::set_Td_in_s(double T)
{
    if(T<=0.0)
    {
        ostringstream osstream;
        osstream<<"Error. Non-positive time constant Td is not allowed for PD_BLOCK.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
    d_block.set_T_in_s(T);
}

double PD_BLOCK::get_Td_in_s() const
{
    return d_block.get_T_in_s();
}

double PD_BLOCK::get_differentiator_state() const
{
    return d_block.get_state();
}

double PD_BLOCK::get_differentiator_store() const
{
    return d_block.get_store();
}

void PD_BLOCK::initialize()
{
    double y = get_output();
    p_block.set_output(y);
    d_block.set_input(0.0);

    p_block.initialize();
    d_block.initialize();

    set_input(p_block.get_input());
}

void PD_BLOCK::run(DYNAMIC_MODE mode)
{
    double x = get_input();
    p_block.set_input(x);
    d_block.set_input(x);

    if(mode==INTEGRATE_MODE)
        integrate();
    if(mode==UPDATE_MODE)
        update();

    double y = p_block.get_output() + d_block.get_output();
    if(get_limiter_type() == NO_LIMITER)
        set_output(y);
    else
    {
        double vmax = get_upper_limit();
        double vmin = get_lower_limit();
        if(y>vmax)
            y = vmax;
        else
        {
            if(y<vmin)
                y = vmin;
        }
        set_output(y);
    }
}

void PD_BLOCK::integrate()
{
    p_block.run(INTEGRATE_MODE);
    d_block.run(INTEGRATE_MODE);
}

void PD_BLOCK::update()
{
    p_block.run(UPDATE_MODE);
    d_block.run(UPDATE_MODE);
}

double PD_BLOCK::get_linearized_system_variable(char var) const
{
    var = toupper(var);
    switch(var)
    {
        case 'A':
            return get_linearized_system_A();
        case 'B':
            return get_linearized_system_B();
        case 'C':
            return get_linearized_system_C();
        case 'D':
            return get_linearized_system_D();
        case 'E':
            return get_linearized_system_E();
        case 'F':
            return get_linearized_system_F();
        case 'G':
            return get_linearized_system_G();
        case 'H':
            return get_linearized_system_H();
        default:
            return 0.0;
    }
}

double PD_BLOCK::get_linearized_system_A() const
{
    return 0.0;
}

double PD_BLOCK::get_linearized_system_B() const
{
    return 0.0;
}

double PD_BLOCK::get_linearized_system_C() const
{
    return 0.0;
}

double PD_BLOCK::get_linearized_system_D() const
{
    return 0.0;
}

double PD_BLOCK::get_linearized_system_E() const
{
    return 0.0;
}

double PD_BLOCK::get_linearized_system_F() const
{
    return 0.0;
}

double PD_BLOCK::get_linearized_system_G() const
{
    return 0.0;
}

double PD_BLOCK::get_linearized_system_H() const
{
    return 0.0;
}

void PD_BLOCK::check()
{
    check_limiter();
}

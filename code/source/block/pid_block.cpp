#include "header/block/pid_block.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <iostream>

using namespace std;

PID_BLOCK::PID_BLOCK()
{
    set_Kp(1.0);
    set_Ki(1.0);
    set_Kd(1.0);
}

PID_BLOCK::~PID_BLOCK()
{
    ;
}

void PID_BLOCK::set_Kp(double K)
{
    /*if(K==0)
    {
        ostringstream sstream;
        sstream<<"Error. Zero amplifier Kp of PROPORTIONAL_BLOCK part is not allowed for PID_BLOCK.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }*/
    p_block.set_K(K);
}

double PID_BLOCK::get_Kp() const
{
    return p_block.get_K();
}

void PID_BLOCK::set_Ki(double K)
{
    /*if(K==0)
    {
        ostringstream sstream;
        sstream<<"Error. Zero amplifier Ki of INTEGRAL_BLOCK part is not allowed for PID_BLOCK.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }*/
    if(K==0.0)
        i_block.set_T_in_s(INFINITE_THRESHOLD);
    else
        i_block.set_T_in_s(1.0/K);
}

double PID_BLOCK::get_Ki() const
{
    if(fabs(i_block.get_T_in_s()-INFINITE_THRESHOLD)<FLOAT_EPSILON)
        return 0.0;
    else
        return 1.0/i_block.get_T_in_s();
}

void PID_BLOCK::set_Kd(double K)
{
    d_block.set_K(K);
}

double PID_BLOCK::get_Kd() const
{
    return d_block.get_K();
}

void PID_BLOCK::set_Td_in_s(double T)
{
    if(T<=0.0)
    {
        ostringstream sstream;
        sstream<<"Error. Non-positive time constant Td is not allowed for PID_BLOCK.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }
    d_block.set_T_in_s(T);
}

double PID_BLOCK::get_Td_in_s() const
{
    return d_block.get_T_in_s();
}


void PID_BLOCK::set_input(double x)
{
    p_block.set_input(x);
    i_block.set_input(x);
    d_block.set_input(x);
}

void PID_BLOCK::set_output(double y)
{
    p_block.set_output(0.0);
    i_block.set_output(y);
    d_block.set_input(0.0);
}

double PID_BLOCK::get_input() const
{
    return p_block.get_input();
}

double PID_BLOCK::get_output() const
{
    LIMITER_TYPE limiter = get_limiter_type();
    double vmax = get_upper_limit();
    double vmin = get_lower_limit();

    double y = p_block.get_output() + i_block.get_output() + d_block.get_output();
    if(limiter != NO_LIMITER)
    {
        if(y>vmax)
            y = vmax;
        else
        {
            if(y<vmin)
                y = vmin;
        }
    }
    return y;
}


double PID_BLOCK::get_integrator_state() const
{
    return i_block.get_state();
}

double PID_BLOCK::get_differentiator_state() const
{
    return d_block.get_state();
}

double PID_BLOCK::get_integrator_new_state() const
{
    return i_block.get_new_state();
}

double PID_BLOCK::get_differentiator_new_state() const
{
    return d_block.get_new_state();
}

double PID_BLOCK::get_integrator_dstate() const
{
    return i_block.get_dstate();
}

double PID_BLOCK::get_differentiator_dstate() const
{
    return d_block.get_dstate();
}

double PID_BLOCK::get_integrator_store() const
{
    return i_block.get_store();
}

double PID_BLOCK::get_differentiator_store() const
{
    return d_block.get_store();
}

double PID_BLOCK::get_state() const
{
    return get_integrator_state();
}

double PID_BLOCK::get_new_state() const
{
    return get_integrator_new_state();
}

double PID_BLOCK::get_dstate() const
{
    return get_integrator_dstate();
}

double PID_BLOCK::get_store() const
{
    return get_integrator_store();
}



void PID_BLOCK::initialize()
{
    ostringstream sstream;

    p_block.initialize();
    i_block.initialize();
    d_block.initialize();

    LIMITER_TYPE limiter = get_limiter_type();
    double vmax = get_upper_limit();
    double vmin = get_lower_limit();

    double s = get_integrator_state();

    if(limiter != NO_LIMITER)
    {
        if(s>vmax)
        {
            sstream<<"Initialization Error. State ("<<s<<") exceeds upper limit bound ("<<vmax<<").";
            show_information_with_leading_time_stamp(sstream);
        }
        else
        {
            if(s<vmin)
            {
                sstream<<"Initialization Error. State ("<<s<<") exceeds lower limit bound ("<<vmin<<").";
                show_information_with_leading_time_stamp(sstream);
            }
        }
    }
}

void PID_BLOCK::run(DYNAMIC_MODE mode)
{
    if(mode==INTEGRATE_MODE)
        integrate();
    else
        update();
}

void PID_BLOCK::integrate()
{
    p_block.run(INTEGRATE_MODE);
    i_block.run(INTEGRATE_MODE);
    d_block.run(INTEGRATE_MODE);
}

void PID_BLOCK::update()
{
    p_block.run(UPDATE_MODE);
    i_block.run(UPDATE_MODE);
    d_block.run(UPDATE_MODE);
}

void PID_BLOCK::check()
{
    check_limiter();
}

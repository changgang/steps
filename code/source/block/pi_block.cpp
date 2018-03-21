#include "header/block/pi_block.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <iostream>

using namespace std;

PI_BLOCK::PI_BLOCK()
{
    pid_block.set_Kp(1.0);
    pid_block.set_Ki(1.0);
    pid_block.set_Kd(0.0);
    pid_block.set_Td_in_s(1.0);
}

PI_BLOCK::~PI_BLOCK()
{
    ;
}

void PI_BLOCK::set_Kp(double K)
{
    if(K==0)
    {
        ostringstream sstream;
        sstream<<"Error. Zero amplifier Kp of PROPORTIONAL_BLOCK part is not allowed for PI_BLOCK.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }
    pid_block.set_Kp(K);
}

double PI_BLOCK::get_Kp() const
{
    return pid_block.get_Kp();
}

void PI_BLOCK::set_Ki(double K)
{
    if(K==0)
    {
        ostringstream sstream;
        sstream<<"Error. Zero amplifier Ki of INTEGRAL_BLOCK part is not allowed for PI_BLOCK.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }
    pid_block.set_Ki(K);
}

double PI_BLOCK::get_Ki() const
{
    return pid_block.get_Ki();
}


void PI_BLOCK::set_input(double x)
{
    pid_block.set_input(x);
}

void PI_BLOCK::set_output(double y)
{
    pid_block.set_output(y);
}

double PI_BLOCK::get_input() const
{
    return pid_block.get_input();
}

double PI_BLOCK::get_output() const
{
    LIMITER_TYPE limiter = get_limiter_type();
    double vmax = get_upper_limit();
    double vmin = get_lower_limit();

    double y = pid_block.get_output();
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


double PI_BLOCK::get_state() const
{
    return pid_block.get_integrator_state();
}

double PI_BLOCK::get_new_state() const
{
    return pid_block.get_integrator_new_state();
}

double PI_BLOCK::get_dstate() const
{
    return pid_block.get_integrator_dstate();
}

double PI_BLOCK::get_store() const
{
    return pid_block.get_integrator_store();
}



void PI_BLOCK::initialize()
{
    ostringstream sstream;

    pid_block.initialize();

    LIMITER_TYPE limiter = get_limiter_type();
    double vmax = get_upper_limit();
    double vmin = get_lower_limit();

    double s = get_state();

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
void PI_BLOCK::run(DYNAMIC_MODE mode)
{
    if(mode==INTEGRATE_MODE)
        integrate();
    else
        update();
}

void PI_BLOCK::integrate()
{
    pid_block.run(INTEGRATE_MODE);
}

void PI_BLOCK::update()
{
    pid_block.run(UPDATE_MODE);
}

void PI_BLOCK::check()
{
    check_limiter();
}

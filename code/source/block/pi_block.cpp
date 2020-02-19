#include "header/block/pi_block.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <istream>
#include <iostream>

using namespace std;

PI_BLOCK::PI_BLOCK(STEPS& toolkit) : BLOCK(toolkit), pid_block(toolkit)
{
    pid_block.set_Kp(1.0);
    pid_block.set_Ki(1.0);
    pid_block.set_Kd(0.0);
    pid_block.set_Td_in_s(999.0);
}

PI_BLOCK::~PI_BLOCK()
{
    ;
}

void PI_BLOCK::set_Kp(double K)
{
    /*if(K==0)
    {
        ostringstream osstream;
        osstream<<"Error. Zero amplifier Kp of PROPORTIONAL_BLOCK part is not allowed for PI_BLOCK.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }*/
    pid_block.set_Kp(K);
}

double PI_BLOCK::get_Kp() const
{
    return pid_block.get_Kp();
}

void PI_BLOCK::set_Ki(double K)
{
    /*if(K==0)
    {
        ostringstream osstream;
        osstream<<"Error. Zero amplifier Ki of INTEGRAL_BLOCK part is not allowed for PI_BLOCK.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }*/
    pid_block.set_Ki(K);
}

double PI_BLOCK::get_Ki() const
{
    return pid_block.get_Ki();
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
    ostringstream osstream;

    pid_block.set_output(get_output());

    pid_block.initialize();

    set_input(pid_block.get_input());

    LIMITER_TYPE limiter = get_limiter_type();
    double vmax = get_upper_limit();
    double vmin = get_lower_limit();

    double s = get_state();

    if(limiter != NO_LIMITER)
    {
        if(s>vmax)
        {
            osstream<<"Initialization Error. State ("<<s<<") exceeds upper limit bound ("<<vmax<<").";
            STEPS& toolkit = get_toolkit();
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
        else
        {
            if(s<vmin)
            {
                osstream<<"Initialization Error. State ("<<s<<") exceeds lower limit bound ("<<vmin<<").";
                STEPS& toolkit = get_toolkit();
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
    }
}
void PI_BLOCK::run(DYNAMIC_MODE mode)
{
    pid_block.set_input(get_input());

    if(mode==INTEGRATE_MODE)
        integrate();
    if(mode==UPDATE_MODE)
        update();

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
    set_output(y);
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

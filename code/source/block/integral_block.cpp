#include "header/block/integral_block.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <istream>
#include <iostream>

using namespace std;

INTEGRAL_BLOCK::INTEGRAL_BLOCK()
{
    set_T_in_s(999.0);
}

INTEGRAL_BLOCK::~INTEGRAL_BLOCK()
{
    ;
}

void INTEGRAL_BLOCK::set_T_in_s(double t)
{
    this->T = t;
}

double INTEGRAL_BLOCK::get_T_in_s() const
{
    return T;
}


void INTEGRAL_BLOCK::initialize()
{
    ostringstream osstream;

    double t = get_T_in_s();
    double y = get_output();

    if(fabs(t)<FLOAT_EPSILON or fabs(t-INFINITE_THRESHOLD)<FLOAT_EPSILON)
    {
        set_state(y);
        set_store(0.0);
        set_dstate(0.0);
        set_input(0.0);
        return;
    }

    LIMITER_TYPE limiter = get_limiter_type();
    double vmax = get_upper_limit();
    double vmin = get_lower_limit();

    double h = get_dynamic_simulation_time_step_in_s();

    double s = y;
    double ds = 0.0;
    double x = 0.0;
    double z = s+0.5*h/t*x;

    set_state(s);
    set_store(z);
    set_dstate(ds);
    set_input(x);

    if(limiter != NO_LIMITER)
    {
        if(s>vmax)
        {
            osstream<<"Initialization Error. State ("<<s<<") exceeds upper limit bound ("<<vmax<<").";
            show_information_with_leading_time_stamp(osstream);
        }
        if(s<vmin)
        {
            osstream<<"Initialization Error. State ("<<s<<") exceeds lower limit bound ("<<vmin<<").";
            show_information_with_leading_time_stamp(osstream);
        }
    }
}

void INTEGRAL_BLOCK::run(DYNAMIC_MODE mode)
{
    ostringstream osstream;
    double t = get_T_in_s();
    if(fabs(t)<FLOAT_EPSILON or fabs(t-INFINITE_THRESHOLD)<FLOAT_EPSILON)
        return;

    if(mode==INTEGRATE_MODE)
        integrate();
    if(mode==UPDATE_MODE)
        update();
}

void INTEGRAL_BLOCK::integrate()
{
    double h = get_dynamic_simulation_time_step_in_s();

    double t = get_T_in_s();
    if(fabs(t)<FLOAT_EPSILON or fabs(t-INFINITE_THRESHOLD)<FLOAT_EPSILON)
        return;

    LIMITER_TYPE limiter = get_limiter_type();
    double vmax = get_upper_limit();
    double vmin = get_lower_limit();

    double x = get_input();

    double ds = x/t;
    if(fabs(ds)<FLOAT_EPSILON)
        return;

    double s, z, y;

    z = get_store();
    s = z + 0.5*h/t*x;
    y = s;
    if(limiter == WINDUP_LIMITER)
    {
        if(y>vmax)
            y = vmax;
        else
        {
            if(y<vmin)
                y = vmin;
        }
    }
    else
    {
        if(limiter == NON_WINDUP_LIMITER)
        {
            if(s>vmax)
            {
                s = vmax;
                y = vmax;
            }
            else
            {
                if(y<vmin)
                {
                    s = vmin;
                    y = vmin;
                }
            }
        }
    }
    set_state(s);
    set_output(y);
}

void INTEGRAL_BLOCK::update()
{
    double h = get_dynamic_simulation_time_step_in_s();

    double t = get_T_in_s();
    if(fabs(t)<FLOAT_EPSILON or fabs(t-INFINITE_THRESHOLD)<FLOAT_EPSILON)
        return;

    LIMITER_TYPE limiter = get_limiter_type();
    double vmax = get_upper_limit();
    double vmin = get_lower_limit();

    double x = get_input();

    double s, z, ds, y;

    ds = x/t;
    s = get_state();
    y = s;

    if(limiter == WINDUP_LIMITER)
    {
        if(y>vmax)
            y = vmax;
        else
        {
            if(y<vmin)
                y = vmin;
        }
    }
    else
    {
        if(limiter == NON_WINDUP_LIMITER)
        {
            if(s>=vmax and ds>0.0)
            {
                y = vmax;
                ds = 0.0;
            }
            else
            {
                if(s<=vmin and ds<0.0)
                {
                    y = vmin;
                    ds = 0.0;
                }
            }
        }
    }
    z = s+0.5*h/t*x;
    set_store(z);
    set_dstate(ds);
    set_output(y);
}

void INTEGRAL_BLOCK::check()
{
    check_limiter();
}

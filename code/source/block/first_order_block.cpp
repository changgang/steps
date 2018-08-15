#include "header/block/first_order_block.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <istream>
#include <iostream>

using namespace std;

FIRST_ORDER_BLOCK::FIRST_ORDER_BLOCK()
{
    set_K(1.0);
}

FIRST_ORDER_BLOCK::~FIRST_ORDER_BLOCK()
{
    ;
}

void FIRST_ORDER_BLOCK::set_K(double k)
{
    /*if(k==0)
    {
        ostringstream osstream;
        osstream<<"Error. Zero amplifier K is not allowed for FIRST_ORDER_BLOCK.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }*/
    this->K = k;
}

double FIRST_ORDER_BLOCK::get_K() const
{
    return K;
}

void FIRST_ORDER_BLOCK::set_T_in_s(double t)
{
    this->T = t;
}

double FIRST_ORDER_BLOCK::get_T_in_s() const
{
    return T;
}

void FIRST_ORDER_BLOCK::initialize()
{
    double t = get_T_in_s();
    if(t==0.0)
    {
        proportional_block.set_limiter_type(this->get_limiter_type());
        proportional_block.set_upper_limit(this->get_upper_limit());
        proportional_block.set_lower_limit(this->get_lower_limit());
        proportional_block.set_K(this->get_K());

        proportional_block.set_output(this->get_output());
        proportional_block.initialize();

        this->set_state(0.0);
        this->set_store(0.0);
        this->set_dstate(0.0);
        this->set_input(proportional_block.get_input());
        return;
    }

    ostringstream osstream;

    double h = get_dynamic_simulation_time_step_in_s();

    double k = get_K();

    LIMITER_TYPE limiter_type = get_limiter_type();
    double vmax = get_upper_limit();
    double vmin = get_lower_limit();

    double y = get_output();
    if(k==0.0)
    {
        y=0.0;
        set_output(0.0);
    }

    double s, ds, z, x;

    if(t!=0.0)
    {
        s = y;
        //z = y-(1.0-2.0*t/h)*s;
        z =2.0*t*s/h;
    }
    else
    {
        s = 0.0;
        z = 0.0;
    }
    ds = 0.0;
    if(k==0)
        x = 0.0;
    else
        x = y/k;

    set_state(s);
    set_store(z);
    set_dstate(ds);
    set_input(x);

    if(limiter_type != NO_LIMITER)
    {
        if(s>vmax)
        {
            osstream<<"Initialization Error. State ("<<s<<") exceeds upper limit bound ("<<vmax<<").";
            show_information_with_leading_time_stamp(osstream);
        }
        else
        {
            if(s<vmin)
            {
                osstream<<"Initialization Error. State ("<<s<<") exceeds lower limit bound ("<<vmin<<").";
                show_information_with_leading_time_stamp(osstream);
            }
        }
    }
}

void FIRST_ORDER_BLOCK::run(DYNAMIC_MODE mode)
{
    double t = get_T_in_s();
    if(t==0.0)
    {
        proportional_block.set_input(this->get_input());
        proportional_block.run(mode);

        this->set_output(proportional_block.get_output());
    }
    else
    {
        if(mode==INTEGRATE_MODE)
            integrate();
        else
            update();
    }
}

void FIRST_ORDER_BLOCK::integrate()
{
    double k = get_K();
    if(k==0.0)
        return;

    double h = get_dynamic_simulation_time_step_in_s();

    double t = get_T_in_s();

    LIMITER_TYPE limiter_type = get_limiter_type();
    double vmax = get_upper_limit();
    double vmin = get_lower_limit();

    double x = get_input();

    double s, z, y;

    if(t!=0.0)
    {
        z = get_store();
        //s = (z+k*x)/(1.0+2.0*t/h);
        s = h*(z+k*x)/(h+2.0*t);
        y = s;
        if(limiter_type != NO_LIMITER)
        {
            if(limiter_type == WINDUP_LIMITER)
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
                if(s>vmax)
                {
                    s = vmax;
                    y = vmax;
                }
                else
                {
                    if(s<vmin)
                    {
                        s = vmin;
                        y = vmin;
                    }
                }
            }
        }
        set_state(s);
    }
    else
    {
        y = k*x;
    }
    set_output(y);
}

void FIRST_ORDER_BLOCK::update()
{
    double k = get_K();
    if(k==0.0)
        return;

    double h = get_dynamic_simulation_time_step_in_s();

    double t = get_T_in_s();

    LIMITER_TYPE limiter_type = get_limiter_type();
    double vmax = get_upper_limit();
    double vmin = get_lower_limit();

    double x = get_input();

    double s, ds, z, y;

    if(t!=0.0)
    {
        ds = x/t;
        s = get_state();
        y = s;
        if(limiter_type != NO_LIMITER)
        {
            if(limiter_type == WINDUP_LIMITER)
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
                if(s>=vmax and ds>0.0)
                {
                    ds = 0.0;
                    y = vmax;
                }
                else
                {
                    if(s<=vmin and ds<0.0)
                    {
                        ds = 0.0;
                        y = vmin;
                    }
                }
            }
        }
        //z = k*x-(1.0-2.0*t/h)*s;
        z = k*x-(h-2.0*t)*s/h;
        set_dstate(ds);
        set_store(z);
    }
    else
    {
        y = k*x;
    }
    set_output(y);
}

void FIRST_ORDER_BLOCK::check()
{
    check_limiter();
}

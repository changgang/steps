#include "header/block/first_order_block.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <istream>
#include <iostream>

using namespace std;

FIRST_ORDER_BLOCK::FIRST_ORDER_BLOCK()
{
    set_K(1.0);
    set_T_in_s(999.0);
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
        toolkit.show_information_with_leading_time_stamp(osstream);
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
    double k = get_K();
    double t = get_T_in_s();
    if(k!=0.0)
        one_over_k = 1.0/k;
    if(t!=0.0)
    {
        ostringstream osstream;

        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        double h = toolkit.get_dynamic_simulation_time_step_in_s();

        one_over_t = 1.0/t;
        one_over_h = 1.0/h;
        t_over_h = t*one_over_h;
        h_plus_2t = h+2.0*t;
        one_over_h_plus_2t = 1.0/h_plus_2t;
        h_minus_2t = h-2.0*t;


        LIMITER_TYPE limiter_type = get_limiter_type();
        double vmax = get_upper_limit();
        double vmin = get_lower_limit();

        double y = get_output();
        if(k!=0.0)
            ;
        else
        {
            y=0.0;
            set_output(0.0);
        }

        double s, ds, z, x;

        s = y;
        //z = y-(1.0-2.0*t/h)*s;
        //z = s-(1.0-2.0*t/h)*s;
        //z =2.0*t/h*s;
        z =2.0*t_over_h*s;

        ds = 0.0;
        if(k!=0)
        {
            //x = y/k;
            x = y*one_over_k;
        }
        else
            x = 0.0;

        set_state(s);
        set_store(z);
        set_dstate(ds);
        set_input(x);

        if(limiter_type != NO_LIMITER)
        {
            if(s>vmax)
            {
                osstream<<"Initialization Error. State ("<<s<<") exceeds upper limit bound ("<<vmax<<").";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
            else
            {
                if(s<vmin)
                {
                    osstream<<"Initialization Error. State ("<<s<<") exceeds lower limit bound ("<<vmin<<").";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
            }
        }
    }
    else
    {
        set_state(0.0);
        set_store(0.0);
        set_dstate(0.0);
        if(k!=0.0)
        {
            //set_input(get_output()/get_K());
            set_input(get_output()*one_over_k);
        }
        else
        {
            set_output(0.0);
            set_input(0.0);
        }
    }
}

void FIRST_ORDER_BLOCK::run(DYNAMIC_MODE mode)
{
    if(mode==INTEGRATE_MODE)
        integrate();
    if(mode==UPDATE_MODE)
        update();
}

void FIRST_ORDER_BLOCK::integrate()
{
    double k = get_K();
    if(k!=0.0)
    {
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        double h = toolkit.get_dynamic_simulation_time_step_in_s();

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
            //s = h*(z+k*x)/(h+2.0*t);
            s = h*(z+k*x)*one_over_h_plus_2t;
            y = s;

            //double ds = (k*x-s)/t;
            //double ds = (k*x-s)*one_over_t;
            //if(fabs(ds)<FLOAT_EPSILON)
            //    return;
            //if(fabs(ds)>DSTATE_THRESHOLD)
            //    cout<<"Derivative of state is changed dramatically in FIRST_ORDER_BLOCK\n";

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
}

void FIRST_ORDER_BLOCK::update()
{
    double k = get_K();
    if(k!=0.0)
    {
        //STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        //double h = toolkit.get_dynamic_simulation_time_step_in_s();

        double t = get_T_in_s();

        LIMITER_TYPE limiter_type = get_limiter_type();
        double vmax = get_upper_limit();
        double vmin = get_lower_limit();

        double x = get_input();

        double s, ds, z, y;

        if(t!=0.0)
        {
            s = get_state();
            //ds = (k*x-s)/t;
            ds = (k*x-s)*one_over_t;
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
            //z = k*x-(h-2.0*t)*s/h;
            z = k*x-h_minus_2t*s*one_over_h;
            set_dstate(ds);
            set_store(z);
        }
        else
        {
            y = k*x;
        }
        set_output(y);
    }
}

void FIRST_ORDER_BLOCK::check()
{
    check_limiter();
}

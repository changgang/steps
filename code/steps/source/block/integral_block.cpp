#include "header/block/integral_block.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <istream>
#include <iostream>

using namespace std;

INTEGRAL_BLOCK::INTEGRAL_BLOCK(STEPS& toolkit) : BLOCK(toolkit)
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

    if(fabs(t)>DOUBLE_EPSILON and fabs(t-INFINITE_THRESHOLD)>DOUBLE_EPSILON)
    {
        STEPS& toolkit = get_toolkit();
        double h = toolkit.get_dynamic_simulation_time_step_in_s();

        one_over_t = 1.0/t;
        h_over_t = h*one_over_t;

        double s = y;
        double x = 0.0;
        //double z = s+0.5*h/t*x;
        double z = s+0.5*h_over_t*x;

        set_state(s);
        set_store(z);
        set_input(x);

        if(get_limiter_type() != NO_LIMITER)
        {
            double vmax = get_upper_limit();
            double vmin = get_lower_limit();
            if(s>vmax)
            {
                osstream<<"Initialization Error. State ("<<s<<") exceeds upper limit bound ("<<vmax<<").";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
            if(s<vmin)
            {
                osstream<<"Initialization Error. State ("<<s<<") exceeds lower limit bound ("<<vmin<<").";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
    }
    else
    {
        set_state(y);
        set_store(0.0);
        set_input(0.0);
    }
}

void INTEGRAL_BLOCK::run(DYNAMIC_MODE mode)
{
    ostringstream osstream;
    double t = get_T_in_s();
    if(fabs(t)>DOUBLE_EPSILON and fabs(t-INFINITE_THRESHOLD)>DOUBLE_EPSILON)
    {
        if(mode==INTEGRATE_MODE)
            integrate();
        if(mode==UPDATE_MODE)
            update();
    }
}

void INTEGRAL_BLOCK::integrate()
{
    double t = get_T_in_s();
    if(fabs(t)>DOUBLE_EPSILON and fabs(t-INFINITE_THRESHOLD)>DOUBLE_EPSILON)
    {
        double x = get_input();

        //double ds = x/t;
        //double ds = x*one_over_t;

        //if(fabs(ds)>DOUBLE_EPSILON)
        //{
            double s, z, y;

            z = get_store();
            //s = z + 0.5*h/t*x;
            s = z + 0.5*h_over_t*x;
            y = s;

            switch(get_limiter_type())
            {
                case WINDUP_LIMITER:
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
                    break;
                }
                case NON_WINDUP_LIMITER:
                {
                    double vmax = get_upper_limit();
                    double vmin = get_lower_limit();
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
                    break;
                }
                case NO_LIMITER:
                default:
                    break;
            }
            set_state(s);
            set_output(y);
        //}
        //if(fabs(ds)>DSTATE_THRESHOLD)
        //    cout<<"Derivative of state is changed dramatically in INTEGRAL_BLOCK\n";
    }
}

void INTEGRAL_BLOCK::update()
{
    double t = get_T_in_s();
    if(fabs(t)>DOUBLE_EPSILON and fabs(t-INFINITE_THRESHOLD)>DOUBLE_EPSILON)
    {
        double x = get_input();

        double s, z, ds, y;

        //ds = x/t;
        ds = x*one_over_t;
        s = get_state();
        y = s;

        switch(get_limiter_type())
        {
            case WINDUP_LIMITER:
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
                break;
            }
            case NON_WINDUP_LIMITER:
            {
                double vmax = get_upper_limit();
                double vmin = get_lower_limit();
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
                break;
            }
            case NO_LIMITER:
            default:
                break;
        }
        //z = s+0.5*h/t*x;
        z = s+0.5*h_over_t*x;
        set_store(z);
        set_output(y);
    }
}

void INTEGRAL_BLOCK::check()
{
    check_limiter();
}

#include "header/block/proportional_block.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <cstdio>

PROPORTIONAL_BLOCK::PROPORTIONAL_BLOCK(STEPS& toolkit) : BLOCK(toolkit)
{
    set_K(1.0);
}

PROPORTIONAL_BLOCK::~PROPORTIONAL_BLOCK()
{
    ;
}

void PROPORTIONAL_BLOCK::set_K(double k)
{
    /*if(k==0.0)
    {
        ostringstream osstream;
        osstream<<"Error. Zero amplifier K is not allowed for PROPORTIONAL_BLOCK.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }*/

    this->K = k;
}

double PROPORTIONAL_BLOCK::get_K() const
{
    return K;
}

void PROPORTIONAL_BLOCK::initialize()
{
    double k = get_K();
    if(fabs(k)>FLOAT_EPSILON)
    {
        double y = get_output();
        double x;
        if(k==0.0)
        {
            y = 0.0;
            set_output(y);
            x = 0.0;
        }
        else
            x = y/k;

        set_input(x);

        if(get_limiter_type()!=NO_LIMITER)
        {
            double vmax = get_upper_limit();
            double vmin = get_lower_limit();
            if(y>=vmin and y<=vmax)
                return;

            ostringstream osstream;

            if(y>vmax)
            {
                osstream<<"Initialization Error. Proportional output ("<<y<<") exceeds upper limit bound ("<<vmax<<").";
                STEPS& toolkit = get_toolkit();
                toolkit.show_information_with_leading_time_stamp(osstream);
                return;
            }
            if(y<vmin)
            {
                osstream<<"Initialization Error. Proportional output ("<<y<<") exceeds lower limit bound ("<<vmin<<").";
                STEPS& toolkit = get_toolkit();
                toolkit.show_information_with_leading_time_stamp(osstream);
                return;
            }
        }
    }
    else
    {
        set_input(0.0);
    }
}

void PROPORTIONAL_BLOCK::run(DYNAMIC_MODE mode)
{
    double k = get_K();
    if(fabs(k)>FLOAT_EPSILON)
    {
        LIMITER_TYPE limiter = get_limiter_type();
        double vmax = get_upper_limit();
        double vmin = get_lower_limit();

        double x = get_input();

        double y = k*x;

        if(limiter!=NO_LIMITER)
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
}

void PROPORTIONAL_BLOCK::integrate()
{
    ;
}

void PROPORTIONAL_BLOCK::update()
{
    ;
}

void PROPORTIONAL_BLOCK::check()
{
    check_limiter();
}


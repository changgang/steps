#include "header/block/proportional_block.h"
#include "header/basic/utility.h"
#include <cstdio>

PROPORTIONAL_BLOCK::PROPORTIONAL_BLOCK()
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
        ostringstream sstream;
        sstream<<"Error. Zero amplifier K is not allowed for PROPORTIONAL_BLOCK.";
        show_information_with_leading_time_stamp(sstream);
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
    if(fabs(k)<FLOAT_EPSILON)
    {
        set_input(0.0);
        return;
    }

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

        ostringstream sstream;

        if(y>vmax)
        {
            sstream<<"Initialization Error. Proportional output ("<<y<<") exceeds upper limit bound ("<<vmax<<").";
            show_information_with_leading_time_stamp(sstream);
            return;
        }
        if(y<vmin)
        {
            sstream<<"Initialization Error. Proportional output ("<<y<<") exceeds lower limit bound ("<<vmin<<").";
            show_information_with_leading_time_stamp(sstream);
            return;
        }
    }
}

void PROPORTIONAL_BLOCK::run(DYNAMIC_MODE mode)
{
    if(mode==INTEGRATE_MODE)
        integrate();
    else
        update();
}

void PROPORTIONAL_BLOCK::integrate()
{
    double k = get_K();
    if(fabs(k)<FLOAT_EPSILON)
        return;

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

void PROPORTIONAL_BLOCK::update()
{
    double k = get_K();
    if(fabs(k)<FLOAT_EPSILON)
        return;

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

void PROPORTIONAL_BLOCK::check()
{
    check_limiter();
}


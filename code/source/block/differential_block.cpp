#include "header/block/differential_block.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <istream>
#include <iostream>

using namespace std;

DIFFERENTIAL_BLOCK::DIFFERENTIAL_BLOCK()
{
    set_K(1.0);
}

DIFFERENTIAL_BLOCK::~DIFFERENTIAL_BLOCK()
{
    ;
}

void DIFFERENTIAL_BLOCK::set_K(double k)
{
    this->K = k;
}

double DIFFERENTIAL_BLOCK::get_K() const
{
    return K;
}

void DIFFERENTIAL_BLOCK::set_T_in_s(double t)
{
    if(t==0.0)
    {
        ostringstream osstream;
        osstream<<"Error. Zero time constant T is not allowed for DIFFERENTIAL_BLOCK.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }
    this->T = t;
}

double DIFFERENTIAL_BLOCK::get_T_in_s() const
{
    return T;
}

void DIFFERENTIAL_BLOCK::initialize()
{
    double h = get_dynamic_simulation_time_step_in_s();

    double k = get_K();
    double t = get_T_in_s();

    double x = get_input();

    double y = 0.0;

    double s, ds, z;

    s = x*k/t;
    z = k/t*x-(1.0-2.0*t/h)*s;
    ds = 0.0;

    set_state(s);
    set_store(z);
    set_dstate(ds);
    set_output(y);
}
void DIFFERENTIAL_BLOCK::run(DYNAMIC_MODE mode)
{
    if(mode==INTEGRATE_MODE)
        integrate();
    else
        update();
}

void DIFFERENTIAL_BLOCK::integrate()
{
    double h = get_dynamic_simulation_time_step_in_s();

    double k = get_K();
    double t = get_T_in_s();

    double x = get_input();

    double s, z, y;
    z = get_store();

    s = (z+k/t*x)/(1.0+2.0*t/h);
    y = k/t*x-s;

    set_state(s);
    set_output(y);
}

void DIFFERENTIAL_BLOCK::update()
{
    double h = get_dynamic_simulation_time_step_in_s();

    double k = get_K();
    double t = get_T_in_s();

    double x = get_input();

    double s, ds, z, y;

    s = get_state();
    y = k/t*x-s;
    z = k/t*x-(1.0-2.0*t/h)*s;
    ds = y/t;

    set_dstate(ds);
    set_store(z);
    set_output(y);
}

void DIFFERENTIAL_BLOCK::check()
{
    if(get_limiter_type() != NO_LIMITER)
    {
        ostringstream osstream;
        osstream<<"Warning. Limiter is not allowed for DIFFERENTIAL_BLOCK, and will be disabled.";
        show_information_with_leading_time_stamp(osstream);;
    }
}

#include "header/block/differential_block.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <istream>
#include <iostream>

using namespace std;

DIFFERENTIAL_BLOCK::DIFFERENTIAL_BLOCK()
{
    set_K(1.0);
    set_T_in_s(999.0);
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
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        if(&toolkit!=NULL)
            toolkit.show_information_with_leading_time_stamp(osstream);
        else
            show_information_with_leading_time_stamp_with_default_toolkit(osstream);
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
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    double h = toolkit.get_dynamic_simulation_time_step_in_s();

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
    if(mode==UPDATE_MODE)
        update();
}

void DIFFERENTIAL_BLOCK::integrate()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    double h = toolkit.get_dynamic_simulation_time_step_in_s();

    double k = get_K();
    double t = get_T_in_s();

    double x = get_input();

    double s, z, y;
    z = get_store();

    s = (z+k/t*x)/(1.0+2.0*t/h);
    y = k/t*x-s;

    double ds = (k/t*x-s)/t;
    //if(fabs(ds)>FLOAT_EPSILON)
    //{
        set_state(s);
        set_output(y);
    //}
    //if(fabs(ds)>DSTATE_THRESHOLD)
    //    cout<<"Derivative of state is changed dramatically in DIFFERENTIAL_BLOCK\n";

}

void DIFFERENTIAL_BLOCK::update()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    double h = toolkit.get_dynamic_simulation_time_step_in_s();

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
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        toolkit.show_information_with_leading_time_stamp(osstream);;
    }
}

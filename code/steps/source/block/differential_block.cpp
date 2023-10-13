#include "header/block/differential_block.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <istream>
#include <iostream>

using namespace std;

DIFFERENTIAL_BLOCK::DIFFERENTIAL_BLOCK(STEPS& toolkit) : BLOCK(toolkit)
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
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
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
    determine_block_integration_time_step_mode();
    determine_block_integration_time_step();
    determine_block_temp_variables();

    switch(get_integration_time_step_mode())
    {
        case NORMAL_INTEGRATION_TIME_STEP_MODE:
            initialize_normal_time_step_mode();
            break;
        case SMALL_INTEGRATION_TIME_STEP_MODE:
            initialize_small_time_step_mode();
            break;
        case LARGE_INTEGRATION_TIME_STEP_MODE:
        default:
            initialize_large_time_step_mode();
            break;
    }
}
void DIFFERENTIAL_BLOCK::initialize_normal_time_step_mode()
{
    double k = get_K();
    if(k!=0.0)
    {
        STEPS& toolkit = get_toolkit();
        double h = toolkit.get_dynamic_simulation_time_step_in_s();
        double t = get_T_in_s();

        one_over_t = 1.0/t;
        k_over_t = k*one_over_t;
        t_over_h = t/h;

        double x = get_input();

        double y = 0.0;

        double s, z;

        //s = x*k/t;
        //z = k/t*x-(1.0-2.0*t/h)*s;
        s = x*k_over_t;
        z = k_over_t*x-(1.0-2.0*t_over_h)*s;

        set_state(s);
        set_store(z);
        set_output(y);
    }
    else
    {
        set_state(0.0);
        set_store(0.0);
        set_output(0.0);
    }
}
void DIFFERENTIAL_BLOCK::run(DYNAMIC_MODE mode)
{
    if(get_K()!=0.0)
    {
        if(mode==INTEGRATE_MODE)
            integrate();
        if(mode==UPDATE_MODE)
            update();
    }
}

void DIFFERENTIAL_BLOCK::integrate()
{
    //double k = get_K();
    //double t = get_T_in_s();

    double x = get_input();

    double s, z, y;
    z = get_store();

    //s = (z+k/t*x)/(1.0+2.0*t/h);
    //y = k/t*x-s;
    //double ds = (k/t*x-s)/t;
    s = (z+k_over_t*x)/(1.0+2.0*t_over_h);
    y = k_over_t*x-s;
    //double ds = (k_over_t*x-s)*one_over_t;

    //if(fabs(ds)>DOUBLE_EPSILON)
    //{
        set_state(s);
        set_output(y);
    //}
    //if(fabs(ds)>DSTATE_THRESHOLD)
    //    cout<<"Derivative of state is changed dramatically in DIFFERENTIAL_BLOCK\n";

}

void DIFFERENTIAL_BLOCK::update()
{
    //double k = get_K();
    //double t = get_T_in_s();

    double x = get_input();

    double s, z, y;

    s = get_state();
    //y = k/t*x-s;
    //z = k/t*x-(1.0-2.0*t/h)*s;
    //ds = y/t;
    y = k_over_t*x-s;
    z = k_over_t*x-(1.0-2.0*t_over_h)*s;

    set_store(z);
    set_output(y);
}

void DIFFERENTIAL_BLOCK::check()
{
    if(get_limiter_type() != NO_LIMITER)
    {
        ostringstream osstream;
        osstream<<"Warning. Limiter is not allowed for DIFFERENTIAL_BLOCK, and will be disabled.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);;
    }
}

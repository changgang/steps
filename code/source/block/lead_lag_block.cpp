#include "header/block/lead_lag_block.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <istream>
#include <iostream>

using namespace std;

LEAD_LAG_BLOCK::LEAD_LAG_BLOCK()
{
    set_K(1.0);
    set_T1_in_s(999.0);
    set_T2_in_s(999.0);
}

LEAD_LAG_BLOCK::~LEAD_LAG_BLOCK()
{
    ;
}

void LEAD_LAG_BLOCK::set_K(double k)
{
    if(k==0)
    {
        ostringstream osstream;
        osstream<<"Error. Zero amplifier K is not allowed for LEAD_LAG_BLOCK.";
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
    this->K = k;
}

double LEAD_LAG_BLOCK::get_K() const
{
    return K;
}

void LEAD_LAG_BLOCK::set_T1_in_s(double T)
{
    this->T1 = T;
}

double LEAD_LAG_BLOCK::get_T1_in_s() const
{
    return T1;
}

void LEAD_LAG_BLOCK::set_T2_in_s(double T)
{
    this->T2 = T;
}

double LEAD_LAG_BLOCK::get_T2_in_s() const
{
    return T2;
}

void LEAD_LAG_BLOCK::initialize()
{
    double t1 = get_T1_in_s();
    if(t1!=0.0)
    {
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        double h = toolkit.get_dynamic_simulation_time_step_in_s();

        double k = get_K();

        double y = get_output();

        double s, ds, z, x;

        s = y;
        x = y/k;
        z = (1.0-h/(2.0*t1))*s+h/(2.0*t1)*y;
        ds = 0.0;

        set_state(s);
        set_store(z);
        set_dstate(ds);
        set_input(x);
    }
    else
    {
        first_order_block.set_toolkit(get_toolkit(__PRETTY_FUNCTION__));
        first_order_block.set_limiter_type(this->get_limiter_type());
        first_order_block.set_upper_limit(this->get_upper_limit());
        first_order_block.set_lower_limit(this->get_lower_limit());
        first_order_block.set_K(this->get_K());
        first_order_block.set_T_in_s(this->get_T2_in_s());

        first_order_block.set_output(this->get_output());
        first_order_block.initialize();

        this->set_state(first_order_block.get_state());
        this->set_dstate(first_order_block.get_dstate());
        this->set_store(first_order_block.get_store());
        this->set_input(first_order_block.get_input());
    }
}

void LEAD_LAG_BLOCK::run(DYNAMIC_MODE mode)
{
    double t1 = get_T1_in_s();
    if(t1!=0.0)
    {
        if(mode==INTEGRATE_MODE)
            integrate();
        if(mode==UPDATE_MODE)
            update();
    }
    else
    {
        first_order_block.set_input(this->get_input());
        first_order_block.run(mode);

        this->set_state(first_order_block.get_state());
        this->set_dstate(first_order_block.get_dstate());
        this->set_store(first_order_block.get_store());
        this->set_output(first_order_block.get_output());
    }
}

void LEAD_LAG_BLOCK::integrate()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    double h = toolkit.get_dynamic_simulation_time_step_in_s();

    double k = get_K();
    double t1 = get_T1_in_s();
    double t2 = get_T2_in_s();

    double x = get_input();

    double s, z, y;

    s = get_state();
    z = get_store();
    // with equations:
    // y = t1/t2*(k*x+(t2/t1-1.0)*s);
    // s = (z+h/(2.0*t1)*y)/(1.0+h/(2.0*t1));
    // we may solve y and s as
    // (1+h/(2*t2))*s = z+h/(2.0*t2)*k*x
    s = (z+h/(2.0*t2)*k*x)/(1.0+h/(2.0*t2));
    y = t1/t2*(k*x+(t2/t1-1.0)*s);

    double ds = (k*x-s)/t2;
    if(fabs(ds)>FLOAT_EPSILON)
    {
        set_state(s);
        set_output(y);
    }
    //if(fabs(ds)>DSTATE_THRESHOLD)
    //    cout<<"Derivative of state is changed dramatically in LEAD_LAG_BLOCK\n";

}

void LEAD_LAG_BLOCK::update()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    double h = toolkit.get_dynamic_simulation_time_step_in_s();

    double k = get_K();
    double t1 = get_T1_in_s();
    double t2 = get_T2_in_s();

    double x = get_input();

    double s, ds, z, y;


    s = get_state();
    y = t1/t2*(k*x+(t2/t1-1.0)*s);

    ds = (y-s)/t1;
    z = (1.0-h/(2.0*t1))*s+h/(2.0*t1)*y;

    set_dstate(ds);
    set_store(z);
    set_output(y);
}

void LEAD_LAG_BLOCK::check()
{
    ;
}

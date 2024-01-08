#include "header/block/pi_block.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <istream>
#include <iostream>

using namespace std;

PI_BLOCK::PI_BLOCK(STEPS& toolkit) : BLOCK(toolkit), pid_block(toolkit)
{
    pid_block.set_Kp(1.0);
    pid_block.set_Ki(1.0);
    pid_block.set_Kd(0.0);
    pid_block.set_Td_in_s(999.0);
}

PI_BLOCK::~PI_BLOCK()
{
    ;
}

void PI_BLOCK::set_Kp(double K)
{
    /*if(K==0)
    {
        ostringstream osstream;
        osstream<<"Error. Zero amplifier Kp of PROPORTIONAL_BLOCK part is not allowed for PI_BLOCK.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }*/
    pid_block.set_Kp(K);
}

double PI_BLOCK::get_Kp() const
{
    return pid_block.get_Kp();
}

void PI_BLOCK::set_Ki(double K)
{
    /*if(K==0)
    {
        ostringstream osstream;
        osstream<<"Error. Zero amplifier Ki of INTEGRAL_BLOCK part is not allowed for PI_BLOCK.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }*/
    pid_block.set_Ki(K);
}

double PI_BLOCK::get_Ki() const
{
    return pid_block.get_Ki();
}

double PI_BLOCK::get_state() const
{
    return pid_block.get_integrator_state();
}

double PI_BLOCK::get_store() const
{
    return pid_block.get_integrator_store();
}



void PI_BLOCK::initialize()
{
    ostringstream osstream;

    pid_block.set_output(get_output());

    pid_block.initialize();

    set_input(pid_block.get_input());

    if(get_limiter_type() != NO_LIMITER)
    {
        double s = get_state();
        double vmax = get_upper_limit();
        double vmin = get_lower_limit();
        if(s>vmax)
        {
            osstream<<"Initialization Error. State ("<<s<<") exceeds upper limit bound ("<<vmax<<").";
            STEPS& toolkit = get_toolkit();
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
        else
        {
            if(s<vmin)
            {
                osstream<<"Initialization Error. State ("<<s<<") exceeds lower limit bound ("<<vmin<<").";
                STEPS& toolkit = get_toolkit();
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
    }
}
void PI_BLOCK::run(DYNAMIC_MODE mode)
{
    pid_block.set_input(get_input());

    if(mode==INTEGRATE_MODE)
        integrate();
    if(mode==UPDATE_MODE)
        update();

    double y = pid_block.get_output();
    if(get_limiter_type() == NO_LIMITER)
        set_output(y);
    else
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
        set_output(y);
    }
}

void PI_BLOCK::integrate()
{
    pid_block.run(INTEGRATE_MODE);
}

void PI_BLOCK::update()
{
    pid_block.run(UPDATE_MODE);
}


STEPS_SPARSE_MATRIX PI_BLOCK::get_linearized_system_variable(char var) const
{
    var = toupper(var);
    switch(var)
    {
        case 'A':
            return get_linearized_system_A();
        case 'B':
            return get_linearized_system_B();
        case 'C':
            return get_linearized_system_C();
        case 'D':
            return get_linearized_system_D();
        default:
            STEPS_SPARSE_MATRIX matrix;
            return matrix;
    }
}

STEPS_SPARSE_MATRIX PI_BLOCK::get_linearized_system_A() const
{
    /*
    x: state; u: input; y: output
    dx/dt = A*x + B*u
    y     = C*x + D*u
    */
    STEPS_SPARSE_MATRIX matrix;

    double a = 0;
    /*
    here define a
    */
    matrix.add_entry(0,0, a);

    return matrix;
}

STEPS_SPARSE_MATRIX PI_BLOCK::get_linearized_system_B() const
{
    STEPS_SPARSE_MATRIX matrix;

    double b = 0;
    /*
    here define b
    */
    matrix.add_entry(0,0, b);

    return matrix;
}

STEPS_SPARSE_MATRIX PI_BLOCK::get_linearized_system_C() const
{
    STEPS_SPARSE_MATRIX matrix;

    double c = 0;
    /*
    here define c
    */
    matrix.add_entry(0,0, c);

    return matrix;
}

STEPS_SPARSE_MATRIX PI_BLOCK::get_linearized_system_D() const
{
    STEPS_SPARSE_MATRIX matrix;

    double d = 0;
    /*
    here define d
    */
    matrix.add_entry(0,0, d);

    return matrix;
}

void PI_BLOCK::check()
{
    check_limiter();
}

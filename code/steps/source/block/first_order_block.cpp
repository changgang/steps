#include "header/block/first_order_block.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <istream>
#include <iostream>

using namespace std;

FIRST_ORDER_BLOCK::FIRST_ORDER_BLOCK(STEPS& toolkit) : BLOCK(toolkit)
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

void FIRST_ORDER_BLOCK::determine_block_integration_time_step_mode()
{
    STEPS& toolkit = get_toolkit();
    double global_h = toolkit.get_dynamic_simulation_time_step_in_s();
    BLOCK_INTEGRATION_TIME_STEP_MODE mode = NORMAL_INTEGRATION_TIME_STEP_MODE;

    bool is_automatic_large_step_logic_enabled = get_automatic_large_time_step_logic();
    if(is_automatic_large_step_logic_enabled)
    {
        double t = get_T_in_s();
        if(global_h<6.91*t) // magic number is NOT allowed. must be removed.
        {
            if(global_h>t/4)
                mode = SMALL_INTEGRATION_TIME_STEP_MODE;
        }
        else
            mode = LARGE_INTEGRATION_TIME_STEP_MODE;
    }
    set_integration_time_step_mode(mode);
}

void FIRST_ORDER_BLOCK::determine_block_integration_time_step()
{
    STEPS& toolkit = get_toolkit();
    h = toolkit.get_dynamic_simulation_time_step_in_s();

    BLOCK_INTEGRATION_TIME_STEP_MODE mode = get_integration_time_step_mode();
    if(mode == SMALL_INTEGRATION_TIME_STEP_MODE)
    {
        double t = get_T_in_s();
        count_of_time_slice_when_in_small_integration_time_step_mode = ceil(4.0*h/t);
        h = h/count_of_time_slice_when_in_small_integration_time_step_mode;
    }
}

void FIRST_ORDER_BLOCK::determine_block_temp_variables()
{
    double k = get_K();
    if(k!=0.0)
        one_over_k = 1.0/k;

    double t = get_T_in_s();
    if(t!=0.0)
    {
        one_over_t = 1.0/t;
        one_over_h = 1.0/h;
        t_over_h = t*one_over_h;
        h_plus_2t = h+2.0*t;
        one_over_h_plus_2t = 1.0/h_plus_2t;
        h_minus_2t = h-2.0*t;
    }
}

void FIRST_ORDER_BLOCK::initialize_normal_time_step_mode()
{
    double t = get_T_in_s();
    if(t!=0.0)
        initialize_small_time_step_mode();
    else
        initialize_large_time_step_mode();
}

void FIRST_ORDER_BLOCK::initialize_small_time_step_mode()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    double k = get_K();

    double y = get_output();
    double s, z, x;
    if(k!=0.0)
    {
        s = y;
        //z = y-(1.0-2.0*t/h)*s;
        //z = s-(1.0-2.0*t/h)*s;
        //z =2.0*t/h*s;
        z = 2.0*t_over_h*s;

        //x = y/k;
        x = y*one_over_k;
    }
    else
    {
        y=0.0;
        set_output(0.0);
        s = y;
        //z = y-(1.0-2.0*t/h)*s;
        //z = s-(1.0-2.0*t/h)*s;
        //z =2.0*t/h*s;
        z = 2.0*t_over_h*s;
        x = 0.0;
    }

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
        else
        {
            if(s<vmin)
            {
                osstream<<"Initialization Error. State ("<<s<<") exceeds lower limit bound ("<<vmin<<").";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
    }
    copy_current_input_to_old_input_in_last_time_step();
}

void FIRST_ORDER_BLOCK::initialize_large_time_step_mode()
{
    double k = get_K();
    set_state(0.0);
    set_store(0.0);
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

void FIRST_ORDER_BLOCK::run(DYNAMIC_MODE mode)
{
    if(mode==INTEGRATE_MODE)
        integrate();
    if(mode==UPDATE_MODE)
        update();
}

void FIRST_ORDER_BLOCK::integrate()
{
    switch(get_integration_time_step_mode())
    {
        case NORMAL_INTEGRATION_TIME_STEP_MODE:
            integrate_normal_time_step_mode();
            break;
        case SMALL_INTEGRATION_TIME_STEP_MODE:
            integrate_small_time_step_mode();
            break;
        case LARGE_INTEGRATION_TIME_STEP_MODE:
        default:
            integrate_large_time_step_mode();
            break;
    }
}

void FIRST_ORDER_BLOCK::integrate_normal_time_step_mode()
{
    double k = get_K();
    if(k!=0.0)
    {
        double t = get_T_in_s();

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
            //if(fabs(ds)<DOUBLE_EPSILON)
            //    return;
            //if(fabs(ds)>DSTATE_THRESHOLD)
            //    cout<<"Derivative of state is changed dramatically in FIRST_ORDER_BLOCK\n";

            LIMITER_TYPE limiter_type = get_limiter_type();
            if(limiter_type != NO_LIMITER)
            {
                double vmax = get_upper_limit();
                double vmin = get_lower_limit();
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
            set_output(y);
        }
        else
        {
            integrate_large_time_step_mode();
        }
    }
}

void FIRST_ORDER_BLOCK::integrate_small_time_step_mode()
{
    double k = get_K();
    if(k!=0.0)
    {
        double x = get_input();
        double x0 = get_old_input_in_last_time_step();
        double xi = x0;
        double deltaX = (x-x0)/count_of_time_slice_when_in_small_integration_time_step_mode;

        double s=0, z=0, y=0;

        z = get_store();
        LIMITER_TYPE limiter_type = get_limiter_type();
        double vmax = get_upper_limit();
        double vmin = get_lower_limit();
        for(unsigned int i=0; i<count_of_time_slice_when_in_small_integration_time_step_mode; ++i)
        {
            xi += deltaX;
            //s = (z+k*x)/(1.0+2.0*t/h);
            //s = h*(z+k*x)/(h+2.0*t);
            s = h*(z+k*xi)*one_over_h_plus_2t;
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
            z = k*xi-h_minus_2t*s*one_over_h;
        }
        set_state(s);
        set_output(y);
    }
}

void FIRST_ORDER_BLOCK::integrate_large_time_step_mode()
{
    double k = get_K();
    if(k!=0.0)
    {
        double x = get_input();
        double y = k*x;
        set_output(y);
    }
}

void FIRST_ORDER_BLOCK::update()
{
    switch(get_integration_time_step_mode())
    {
        case NORMAL_INTEGRATION_TIME_STEP_MODE:
            update_normal_time_step_mode();
            break;
        case SMALL_INTEGRATION_TIME_STEP_MODE:
            update_small_time_step_mode();
            break;
        case LARGE_INTEGRATION_TIME_STEP_MODE:
        default:
            update_large_time_step_mode();
            break;
    }
}

void FIRST_ORDER_BLOCK::update_normal_time_step_mode()
{
    double k = get_K();
    if(k!=0.0)
    {
        double t = get_T_in_s();

        double x = get_input();

        double s, ds, z, y;

        if(t!=0.0)
        {
            s = get_state();
            //ds = (k*x-s)/t;
            ds = (k*x-s)*one_over_t;
            y = s;

            LIMITER_TYPE limiter_type = get_limiter_type();
            if(limiter_type != NO_LIMITER)
            {
                double vmax = get_upper_limit();
                double vmin = get_lower_limit();
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
            set_store(z);
            set_output(y);
        }
        else
        {
            update_large_time_step_mode();
        }
    }
}

void FIRST_ORDER_BLOCK::update_small_time_step_mode()
{
    update_normal_time_step_mode();
    copy_current_input_to_old_input_in_last_time_step();
}

void FIRST_ORDER_BLOCK::update_large_time_step_mode()
{
    double k = get_K();
    if(k!=0.0)
    {
        double x = get_input();

        double y = k*x;

        set_output(y);
    }
}

STEPS_SPARSE_MATRIX FIRST_ORDER_BLOCK::get_linearized_matrix_variable(char var) const
{
    var = toupper(var);
    switch(var)
    {
        case 'A':
            return get_linearized_matrix_A();
        case 'B':
            return get_linearized_matrix_B();
        case 'C':
            return get_linearized_matrix_C();
        case 'D':
            return get_linearized_matrix_D();
        default:
            STEPS_SPARSE_MATRIX matrix;
            return matrix;
    }
}

STEPS_SPARSE_MATRIX FIRST_ORDER_BLOCK::get_linearized_matrix_A() const
{
    /*
    x: state; u: input; y: output
    dx/dt = A*x + B*u
    y     = C*x + D*u
    */
    STEPS_SPARSE_MATRIX matrix;
    if(get_T_in_s()!=0.0)
    {
        double a = -1/get_T_in_s();

        matrix.add_entry(0,0, a);
    }
    matrix.compress_and_merge_duplicate_entries();
    return matrix;
}

STEPS_SPARSE_MATRIX FIRST_ORDER_BLOCK::get_linearized_matrix_B() const
{
    STEPS_SPARSE_MATRIX matrix;
    if(get_T_in_s()!=0.0)
    {
        double b = get_K()/get_T_in_s();

        matrix.add_entry(0,0, b);
    }
    matrix.compress_and_merge_duplicate_entries();
    return matrix;
}

STEPS_SPARSE_MATRIX FIRST_ORDER_BLOCK::get_linearized_matrix_C() const
{
    STEPS_SPARSE_MATRIX matrix;
    if(get_T_in_s()!=0.0)
    {
        double c = 1.0;

        matrix.add_entry(0,0, c);
    }
    matrix.compress_and_merge_duplicate_entries();
    return matrix;
}

STEPS_SPARSE_MATRIX FIRST_ORDER_BLOCK::get_linearized_matrix_D() const
{
    STEPS_SPARSE_MATRIX matrix;
    if(get_T_in_s()!=0.0)
    {
        double d = 0.0;

        matrix.add_entry(0,0, d);
    }
    matrix.compress_and_merge_duplicate_entries();

    return matrix;
}

void FIRST_ORDER_BLOCK::check()
{
    check_limiter();
}

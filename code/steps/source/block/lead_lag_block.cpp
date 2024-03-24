#include "header/block/lead_lag_block.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <istream>
#include <iostream>

using namespace std;

LEAD_LAG_BLOCK::LEAD_LAG_BLOCK(STEPS& toolkit) : BLOCK(toolkit), first_order_block(toolkit)
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
        STEPS& toolkit = get_toolkit();
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

void LEAD_LAG_BLOCK::determine_block_integration_time_step_mode()
{
    STEPS& toolkit = get_toolkit();
    double global_h = toolkit.get_dynamic_simulation_time_step_in_s();
    BLOCK_INTEGRATION_TIME_STEP_MODE mode = NORMAL_INTEGRATION_TIME_STEP_MODE;

    bool is_automatic_large_step_logic_enabled = is_automatic_large_time_step_logic_enabled();
    if(is_automatic_large_step_logic_enabled)
    {
        double t2 = get_T2_in_s();
        if(t2!=0)
        {
            if(global_h<7.68*t2) // magic number is NOT allowed. must be removed.
            {
                if(global_h>t2/4)
                    mode = SMALL_INTEGRATION_TIME_STEP_MODE;
            }
            else
                mode = LARGE_INTEGRATION_TIME_STEP_MODE;
        }
    }
    set_integration_time_step_mode(mode);
}

void LEAD_LAG_BLOCK::determine_block_integration_time_step()
{
    STEPS& toolkit = get_toolkit();
    h = toolkit.get_dynamic_simulation_time_step_in_s();

    BLOCK_INTEGRATION_TIME_STEP_MODE mode = get_integration_time_step_mode();
    if(mode == SMALL_INTEGRATION_TIME_STEP_MODE)
    {
        double t2 = get_T2_in_s();
        count_of_time_slice_when_in_small_integration_time_step_mode = ceil(4.0*h/t2);
        h = h/count_of_time_slice_when_in_small_integration_time_step_mode;
    }
}

void LEAD_LAG_BLOCK::determine_block_temp_variables()
{
    double t1 = get_T1_in_s();
    double t2 = get_T2_in_s();
    if(t1!=0.0)
    {
        STEPS& toolkit = get_toolkit();
        double h = toolkit.get_dynamic_simulation_time_step_in_s();

        one_over_t1 = 1.0/t1;
        h_over_2t1 = 0.5*h*one_over_t1;
        t2_over_t1 = t2*one_over_t1;
        if(t2!=0.0)
        {
            one_over_t2 = 1.0/t2;
            h_over_2t2 = 0.5*h*one_over_t2;
            one_over_1_plus_h_over_2t2 = 1.0/(1.0+h_over_2t2);
            t1_over_t2 = t1*one_over_t2;
        }
    }
}

void LEAD_LAG_BLOCK::initialize_normal_time_step_mode()
{
    double t1 = get_T1_in_s();
    if(t1!=0.0)
    {
        double k = get_K();

        double y = get_output();

        double s, z, x;

        s = y;
        x = y/k;
        //z = (1.0-h/(2.0*t1))*s+h/(2.0*t1)*y;
        z = (1.0-h_over_2t1)*s+h_over_2t1*y;

        set_state(s);
        set_store(z);
        set_input(x);
    }
    else
    {
        first_order_block.set_limiter_type(this->get_limiter_type());
        first_order_block.set_upper_limit(this->get_upper_limit());
        first_order_block.set_lower_limit(this->get_lower_limit());
        first_order_block.set_K(this->get_K());
        first_order_block.set_T_in_s(this->get_T2_in_s());

        first_order_block.set_output(this->get_output());
        first_order_block.initialize();

        this->set_state(first_order_block.get_state());
        this->set_store(first_order_block.get_store());
        this->set_input(first_order_block.get_input());
    }
}

void LEAD_LAG_BLOCK::initialize_small_time_step_mode()
{
    initialize_normal_time_step_mode();
    copy_current_input_to_old_input_in_last_time_step();
}

void LEAD_LAG_BLOCK::initialize_large_time_step_mode()
{
    initialize_normal_time_step_mode();
}

void LEAD_LAG_BLOCK::run(DYNAMIC_MODE mode)
{
    double t1 = get_T1_in_s();
    if(t1!=0.0)
    {
        if(mode==DYNAMIC_INTEGRATE_MODE)
            integrate();
        if(mode==DYNAMIC_UPDATE_MODE)
            update();
        if(mode==DYNAMIC_UPDATE_TIME_STEP_MODE)
            update_simulation_time_step();
    }
    else
    {
        first_order_block.set_input(this->get_input());
        first_order_block.run(mode);

        this->set_state(first_order_block.get_state());
        this->set_store(first_order_block.get_store());
        this->set_output(first_order_block.get_output());
    }
}

void LEAD_LAG_BLOCK::integrate()
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

void LEAD_LAG_BLOCK::integrate_normal_time_step_mode()
{
    double k = get_K();
    //double t1 = get_T1_in_s();
    //double t2 = get_T2_in_s();

    double x = get_input();

    double s, z, y;

    z = get_store();
    // with equations:
    // y = t1/t2*(k*x+(t2/t1-1.0)*s);
    // s = (z+h/(2.0*t1)*y)/(1.0+h/(2.0*t1));
    // we may solve y and s as
    // (1+h/(2*t2))*s = z+h/(2.0*t2)*k*x

    //s = (z+h/(2.0*t2)*k*x)/(1.0+h/(2.0*t2));
    //y = t1/t2*(k*x+(t2/t1-1.0)*s);
    //double ds = (k*x-s)/t2;

    s = (z+h_over_2t2*k*x)*one_over_1_plus_h_over_2t2;
    y = t1_over_t2*(k*x+(t2_over_t1-1.0)*s);
    //double ds = (k*x-s)*one_over_t2;

    //if(fabs(ds)>DOUBLE_EPSILON)
    //{
        set_state(s);
        set_output(y);
    //}
    //if(fabs(ds)>DSTATE_THRESHOLD)
    //    cout<<"Derivative of state is changed dramatically in LEAD_LAG_BLOCK\n";

}

void LEAD_LAG_BLOCK::integrate_small_time_step_mode()
{
    double x = get_input();
    double x0 = get_old_input_in_last_time_step();
    double xi = x0;
    double deltaX = (x-x0)/count_of_time_slice_when_in_small_integration_time_step_mode;

    double k = get_K();
    double s=0, z, y=0;

    z = get_store();
    for(unsigned int i=0; i<count_of_time_slice_when_in_small_integration_time_step_mode; ++i)
    {
        xi += deltaX;
        s = (z+h_over_2t2*k*xi)*one_over_1_plus_h_over_2t2;
        y = t1_over_t2*(k*xi+(t2_over_t1-1.0)*s);
        z = s+h_over_2t1*(y-s);
    }
    set_state(s);
    set_output(y);
}

void LEAD_LAG_BLOCK::integrate_large_time_step_mode()
{
    double k = get_K();
    double x = get_input();
    double y = k*x;
    double s = y;

    set_state(s);
    set_output(y);
}

void LEAD_LAG_BLOCK::update()
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

void LEAD_LAG_BLOCK::update_normal_time_step_mode()
{
    double k = get_K();
    //double t1 = get_T1_in_s();
    //double t2 = get_T2_in_s();

    double x = get_input();

    double s, z, y;


    s = get_state();
    //y = t1/t2*(k*x+(t2/t1-1.0)*s);
    //ds = (y-s)/t1;
    //z = (1.0-h/(2.0*t1))*s+h/(2.0*t1)*y;

    y = t1_over_t2*(k*x+(t2_over_t1-1.0)*s);
    //z = (1.0-h_over_2t1)*s+h_over_2t1*y;
    z = s+h_over_2t1*(y-s);

    set_store(z);
    set_output(y);
}

void LEAD_LAG_BLOCK::update_small_time_step_mode()
{
    update_normal_time_step_mode();
    copy_current_input_to_old_input_in_last_time_step();
}

void LEAD_LAG_BLOCK::update_large_time_step_mode()
{
    update_normal_time_step_mode();
}

void LEAD_LAG_BLOCK::update_simulation_time_step()
{
    determine_block_integration_time_step_mode();
    determine_block_integration_time_step();
    determine_block_temp_variables();

    double k = get_K();
    double x = get_input();
    double y = get_output();
    double s = (y/t1_over_t2 - k*x)/(t2_over_t1-1.0);
    set_state(s);
    double z = s+h_over_2t1*(y-s);
    set_store(z);
    copy_current_input_to_old_input_in_last_time_step();
}

STEPS_SPARSE_MATRIX LEAD_LAG_BLOCK::get_linearized_matrix_variable(char var) const
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

STEPS_SPARSE_MATRIX LEAD_LAG_BLOCK::get_linearized_matrix_A() const
{
    /*
    x: state; u: input; y: output
    dx/dt = A*x + B*u
    y     = C*x + D*u
    */
    STEPS_SPARSE_MATRIX matrix;

    double a = -1/get_T2_in_s();

    matrix.add_entry(0,0, a);
    matrix.compress_and_merge_duplicate_entries();

    return matrix;
}

STEPS_SPARSE_MATRIX LEAD_LAG_BLOCK::get_linearized_matrix_B() const
{
    STEPS_SPARSE_MATRIX matrix;

    double b = get_K()/get_T2_in_s();

    matrix.add_entry(0,0, b);
    matrix.compress_and_merge_duplicate_entries();

    return matrix;
}

STEPS_SPARSE_MATRIX LEAD_LAG_BLOCK::get_linearized_matrix_C() const
{
    STEPS_SPARSE_MATRIX matrix;

    double c = 1-get_T1_in_s()/get_T2_in_s();

    matrix.add_entry(0,0, c);
    matrix.compress_and_merge_duplicate_entries();

    return matrix;
}

STEPS_SPARSE_MATRIX LEAD_LAG_BLOCK::get_linearized_matrix_D() const
{
    STEPS_SPARSE_MATRIX matrix;

    double d = get_K()*get_T1_in_s()/get_T2_in_s();

    matrix.add_entry(0,0, d);
    matrix.compress_and_merge_duplicate_entries();

    return matrix;
}

void LEAD_LAG_BLOCK::check()
{
    ;
}

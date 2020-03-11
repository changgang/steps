#include "header/basic/test_macro.h"
#include "header/block/first_order_block_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/basic/constants.h"

#include <cstdlib>
#include <cstring>
#include <cstdio>

#ifdef ENABLE_STEPS_TEST
using namespace std;

FIRST_ORDER_BLOCK_TEST::FIRST_ORDER_BLOCK_TEST() : block(default_toolkit)
{
    TEST_ADD(FIRST_ORDER_BLOCK_TEST::test_constructor);
    TEST_ADD(FIRST_ORDER_BLOCK_TEST::test_set_get_K);
    TEST_ADD(FIRST_ORDER_BLOCK_TEST::test_set_get_T);
    TEST_ADD(FIRST_ORDER_BLOCK_TEST::test_step_response_without_limiter);
    TEST_ADD(FIRST_ORDER_BLOCK_TEST::test_step_response_with_limiter);
    TEST_ADD(FIRST_ORDER_BLOCK_TEST::test_step_response_without_limiter_when_T_is_zero);
}

void FIRST_ORDER_BLOCK_TEST::setup()
{
    default_toolkit.set_dynamic_simulation_time_step_in_s(0.01);
}

void FIRST_ORDER_BLOCK_TEST::tear_down()
{
    show_test_end_information();
}

void FIRST_ORDER_BLOCK_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"FIRST_ORDER_BLOCK_TEST");

    TEST_ASSERT(block.get_limiter_type()==NO_LIMITER);
}

void FIRST_ORDER_BLOCK_TEST::test_set_get_K()
{
    show_test_information_for_function_of_class(__FUNCTION__,"FIRST_ORDER_BLOCK_TEST");

    block.set_K(2.0);
    TEST_ASSERT(block.get_K()==2.0);

    block.set_K(5.0);
    TEST_ASSERT(block.get_K()==5.0);
}

void FIRST_ORDER_BLOCK_TEST::test_set_get_T()
{
    show_test_information_for_function_of_class(__FUNCTION__,"FIRST_ORDER_BLOCK_TEST");

    block.set_T_in_s(0.2);
    TEST_ASSERT(block.get_T_in_s()==0.2);

    block.set_T_in_s(0.5);
    TEST_ASSERT(block.get_T_in_s()==0.5);
}

void FIRST_ORDER_BLOCK_TEST::test_step_response_without_limiter()
{
    show_test_information_for_function_of_class(__FUNCTION__,"FIRST_ORDER_BLOCK_TEST");

    default_toolkit.set_dynamic_simulation_time_step_in_s(0.001);
    double h = default_toolkit.get_dynamic_simulation_time_step_in_s();

    double K = 2.0;
    double T = 5.0;
    block.set_K(K);
    block.set_T_in_s(T);

    double t = 5.0*h;


    // solution to unit step response is
    // 1/(s(1+sT)) or (1/T)/(s(s+1/T)) is
    // 1-e^(-t/T)
    // with initial value 10.0, solution will be
    // y = 10+K*(1-e^(-t/T))

    double output = 10.0;
    block.set_output(output);
    block.initialize();
    double y;

    double input = block.get_input();
    block.set_input(input+1.0);
    block.run(UPDATE_MODE);
    for(t=h; t<=10.0000001; t +=h)
    {
        block.run(INTEGRATE_MODE);
        block.run(UPDATE_MODE);
        y = 10.0+K*(1.0-exp(-t/T));
        TEST_ASSERT(fabs(block.get_output()-y)<1e-8);
    }
}
void FIRST_ORDER_BLOCK_TEST::test_step_response_with_limiter()
{
    show_test_information_for_function_of_class(__FUNCTION__,"FIRST_ORDER_BLOCK_TEST");

    default_toolkit.set_dynamic_simulation_time_step_in_s(0.001);
    double h = default_toolkit.get_dynamic_simulation_time_step_in_s();

    double K = 2.0;
    double T = 5.0;
    block.set_K(K);
    block.set_T_in_s(T);
    block.set_limiter_type(WINDUP_LIMITER);
    block.set_upper_limit(11.0);
    block.set_lower_limit(0.0);


    double t = 5.0*h;


    // solution to unit step response is
    // 1/(s(1+sT)) or (1/T)/(s(s+1/T)) is
    // 1-e^(-t/T)
    // with initial value 10.0, solution will be
    // y = 10+K*(1-e^(-t/T))

    double output = 10.0;
    block.set_output(output);
    block.initialize();
    double y;

    double input = block.get_input();
    block.set_input(input+1.0);
    block.run(UPDATE_MODE);
    for(t=h; t<=10.0000001; t +=h)
    {
        block.run(INTEGRATE_MODE);
        block.run(UPDATE_MODE);
        y = 10.0+K*(1.0-exp(-t/T));
        if(y>11.0)
            y = 11.0;
        TEST_ASSERT(fabs(block.get_output()-y)<1e-8);
    }

}


void FIRST_ORDER_BLOCK_TEST::test_step_response_without_limiter_when_T_is_zero()
{
    show_test_information_for_function_of_class(__FUNCTION__,"FIRST_ORDER_BLOCK_TEST");

    default_toolkit.set_dynamic_simulation_time_step_in_s(0.001);
    double h = default_toolkit.get_dynamic_simulation_time_step_in_s();

    double K = 2.0;
    double T = 0.0;
    block.set_K(K);
    block.set_T_in_s(T);

    double t = 5.0*h;


    // solution to unit step response is
    // 1/(s(1+sT)) or (1/T)/(s(s+1/T)) is
    // 1-e^(-t/T)
    // with initial value 10.0, solution will be
    // y = 10+K*(1-e^(-t/T))

    double output = 10.0;
    block.set_output(output);
    block.initialize();
    double y;

    double input = block.get_input();
    block.set_input(input+1.0);
    block.run(UPDATE_MODE);
    for(t=h; t<=10.0000001; t +=h)
    {
        block.run(INTEGRATE_MODE);
        block.run(UPDATE_MODE);
        y = 10.0+K*1.0;
        TEST_ASSERT(fabs(block.get_output()-y)<1e-8);
    }
}

#endif

#include "header/basic/test_macro.h"
#include "header/block/lead_lag_block_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/basic/constants.h"

#include <cstdlib>
#include <cstring>
#include <cstdio>

#ifdef ENABLE_STEPS_TEST
using namespace std;

LEAD_LAG_BLOCK_TEST::LEAD_LAG_BLOCK_TEST() : block(default_toolkit)
{
    TEST_ADD(LEAD_LAG_BLOCK_TEST::test_constructor);
    TEST_ADD(LEAD_LAG_BLOCK_TEST::test_set_get_K);
    TEST_ADD(LEAD_LAG_BLOCK_TEST::test_set_get_T1);
    TEST_ADD(LEAD_LAG_BLOCK_TEST::test_set_get_T2);
    TEST_ADD(LEAD_LAG_BLOCK_TEST::test_step_response_without_limiter);
    TEST_ADD(LEAD_LAG_BLOCK_TEST::test_step_response_without_limiter_when_T1_is_zero);
    TEST_ADD(LEAD_LAG_BLOCK_TEST::test_step_response_without_limiter_when_T1_and_T2_are_zero);
}

void LEAD_LAG_BLOCK_TEST::setup()
{
    default_toolkit.set_dynamic_simulation_time_step_in_s(0.01);
}

void LEAD_LAG_BLOCK_TEST::tear_down()
{
    show_test_end_information();
}

void LEAD_LAG_BLOCK_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LEAD_LAG_BLOCK_TEST");

    TEST_ASSERT(block.get_limiter_type()==NO_LIMITER);
}

void LEAD_LAG_BLOCK_TEST::test_set_get_K()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LEAD_LAG_BLOCK_TEST");

    block.set_K(1.5);
    TEST_ASSERT(block.get_K()==1.5);

    block.set_K(-1.5);
    TEST_ASSERT(block.get_K()==-1.5);
}

void LEAD_LAG_BLOCK_TEST::test_set_get_T1()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LEAD_LAG_BLOCK_TEST");

    block.set_T1_in_s(0.2);
    TEST_ASSERT(block.get_T1_in_s()==0.2);

    block.set_T1_in_s(0.5);
    TEST_ASSERT(block.get_T1_in_s()==0.5);
}

void LEAD_LAG_BLOCK_TEST::test_set_get_T2()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LEAD_LAG_BLOCK_TEST");

    block.set_T2_in_s(0.2);
    TEST_ASSERT(block.get_T2_in_s()==0.2);

    block.set_T2_in_s(0.5);
    TEST_ASSERT(block.get_T2_in_s()==0.5);
}

void LEAD_LAG_BLOCK_TEST::test_step_response_without_limiter()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LEAD_LAG_BLOCK_TEST");

    default_toolkit.set_dynamic_simulation_time_step_in_s(0.001);
    double h = default_toolkit.get_dynamic_simulation_time_step_in_s();

    double K = 2.0;
    double T1 = 5.0;
    double T2 = 10.0;
    block.set_K(K);
    block.set_T1_in_s(T1);
    block.set_T2_in_s(T2);

    double t = 5.0*h;
    // transfer function is
    // K*(1+sT1)/(1+sT2)  ==
    // K*T1/T2+K*(1-T1/T2)*(1/T2)/(s+1/T2)
    // the step response of the first part K*T1/T2 is : K*T1/T2
    // the step response of the second part is:
    // K*(1-T1/T2)*(1-exp(-t/T2);
    // the total response is
    // K*T1/T2 + K*(1-T1/T2)*(1-exp(-t/T2));


    double output = 10.0;
    block.set_output(output);
    block.initialize();
    TEST_ASSERT(block.get_state()==output);
    TEST_ASSERT(block.get_input()==output/K);

    double input = block.get_input();

    double y;

    input = input+1.0;
    block.set_input(input);
    block.run(DYNAMIC_UPDATE_MODE);
    for(t=h; t<=10.0000001; t +=h)
    {
        block.run(DYNAMIC_INTEGRATE_MODE);
        block.run(DYNAMIC_UPDATE_MODE);
        y =10.0+ K*T1/T2 + K*(1.0-T1/T2)*(1.0-exp(-t/T2));
        TEST_ASSERT(fabs(block.get_output()-y)<1e-8);
    }
}

void LEAD_LAG_BLOCK_TEST::test_step_response_without_limiter_when_T1_is_zero()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LEAD_LAG_BLOCK_TEST");

    default_toolkit.set_dynamic_simulation_time_step_in_s(0.001);
    double h = default_toolkit.get_dynamic_simulation_time_step_in_s();

    double K = 2.0;
    double T1 = 0.0;
    double T2 = 5.0;
    block.set_K(K);
    block.set_T1_in_s(T1);
    block.set_T2_in_s(T2);

    double t = 5.0*h;

    double output = 10.0;
    block.set_output(output);
    block.initialize();
    double y;

    double input = block.get_input();
    block.set_input(input+1.0);
    block.run(DYNAMIC_UPDATE_MODE);
    for(t=h; t<=10.0000001; t +=h)
    {
        block.run(DYNAMIC_INTEGRATE_MODE);
        block.run(DYNAMIC_UPDATE_MODE);
        y = 10.0+K*(1.0-exp(-t/T2));
        TEST_ASSERT(fabs(block.get_output()-y)<1e-8);
    }
}
void LEAD_LAG_BLOCK_TEST::test_step_response_without_limiter_when_T1_and_T2_are_zero()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LEAD_LAG_BLOCK_TEST");

    default_toolkit.set_dynamic_simulation_time_step_in_s(0.001);
    double h = default_toolkit.get_dynamic_simulation_time_step_in_s();

    double K = 2.0;
    double T1 = 0.0;
    double T2 = 0.0;
    block.set_K(K);
    block.set_T1_in_s(T1);
    block.set_T2_in_s(T2);

    double t = 5.0*h;

    double output = 10.0;
    block.set_output(output);
    block.initialize();
    double y;

    double input = block.get_input();
    block.set_input(input+1.0);
    block.run(DYNAMIC_UPDATE_MODE);
    for(t=h; t<=10.0000001; t +=h)
    {
        block.run(DYNAMIC_INTEGRATE_MODE);
        block.run(DYNAMIC_UPDATE_MODE);
        y = 10.0+K*1.0;
        TEST_ASSERT(fabs(block.get_output()-y)<1e-8);
    }
}

#endif

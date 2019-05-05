#include "header/block/proportional_block_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/basic/constants.h"

#include <cstdlib>
#include <cstring>
#include <cstdio>

using namespace std;

PROPORTIONAL_BLOCK_TEST::PROPORTIONAL_BLOCK_TEST()
{
    TEST_ADD(PROPORTIONAL_BLOCK_TEST::test_constructor);
    TEST_ADD(PROPORTIONAL_BLOCK_TEST::test_set_get_K);
    TEST_ADD(PROPORTIONAL_BLOCK_TEST::test_initialize_integrate_update_once);
    TEST_ADD(PROPORTIONAL_BLOCK_TEST::test_step_response_without_limiter);
    TEST_ADD(PROPORTIONAL_BLOCK_TEST::test_step_response_with_limiter);
}

void PROPORTIONAL_BLOCK_TEST::setup()
{
    default_toolkit.set_dynamic_simulation_time_step_in_s(0.01);
}

void PROPORTIONAL_BLOCK_TEST::tear_down()
{
    show_test_end_information();
}

void PROPORTIONAL_BLOCK_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PROPORTIONAL_BLOCK_TEST");

    TEST_ASSERT(block.get_limiter_type()==NO_LIMITER);
}

void PROPORTIONAL_BLOCK_TEST::test_set_get_K()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PROPORTIONAL_BLOCK_TEST");

    block.set_K(1.5);
    TEST_ASSERT(block.get_K()==1.5);
    block.set_K(-1.5);
    TEST_ASSERT(block.get_K()==-1.5);
}

void PROPORTIONAL_BLOCK_TEST::test_initialize_integrate_update_once()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PROPORTIONAL_BLOCK_TEST");

    block.set_K(2.0);
    double output = 10.0;

    block.set_output(output);

    block.initialize();

    TEST_ASSERT(block.get_input()==5.0);
    TEST_ASSERT(block.get_state()==0.0);
    TEST_ASSERT(block.get_store()==0.0);
    TEST_ASSERT(block.get_store()==0.0);

    block.set_input(6.0);
    block.run(UPDATE_MODE);
    TEST_ASSERT(fabs(block.get_output()-12.0)<FLOAT_EPSILON);

    block.run(INTEGRATE_MODE);
    TEST_ASSERT(fabs(block.get_output()-12.0)<FLOAT_EPSILON);

    block.run(UPDATE_MODE);
    TEST_ASSERT(fabs(block.get_output()-12.0)<FLOAT_EPSILON);
}
void PROPORTIONAL_BLOCK_TEST::test_step_response_without_limiter()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PROPORTIONAL_BLOCK_TEST");

    double h = default_toolkit.get_dynamic_simulation_time_step_in_s();
    double t = 5.0*h;

    block.set_K(2.0);
    double output = 10.0;

    block.set_output(output);

    block.initialize();

    block.set_input(6);
    block.run(UPDATE_MODE);
    TEST_ASSERT(fabs(block.get_output()-12.0)<FLOAT_EPSILON);

    for(t=h; t<=10.00001; t+=h)
    {
        block.run(INTEGRATE_MODE);
        block.run(UPDATE_MODE);
        TEST_ASSERT(fabs(block.get_output()-12.0)<FLOAT_EPSILON);
    }
}
void PROPORTIONAL_BLOCK_TEST::test_step_response_with_limiter()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PROPORTIONAL_BLOCK_TEST");

    double h = default_toolkit.get_dynamic_simulation_time_step_in_s();
    double t = 5.0*h;

    block.set_K(2.0);
    block.set_limiter_type(NON_WINDUP_LIMITER);
    block.set_upper_limit(12.0);
    block.set_lower_limit(8.0);

    double output = 10.0;

    block.set_output(output);
    block.initialize();

    for(t=h; t<=0.000001; t+=h)
    {
        block.run(INTEGRATE_MODE);
        block.run(UPDATE_MODE);
        TEST_ASSERT(fabs(block.get_output()-10.0)<FLOAT_EPSILON);
    }
    t -= h;

    block.set_input(5.5);
    block.run(UPDATE_MODE);

    for(t+=h; t<=10.000001; t+=h)
    {
        block.run(INTEGRATE_MODE);
        block.run(UPDATE_MODE);
        TEST_ASSERT(fabs(block.get_output()-11.0)<FLOAT_EPSILON);
    }
    t -= h;

    block.set_input(7.5);
    block.run(UPDATE_MODE);

    for(t+=h; t<=20.000001; t+=h)
    {
        block.run(INTEGRATE_MODE);
        block.run(UPDATE_MODE);
        TEST_ASSERT(fabs(block.get_output()-12.0)<FLOAT_EPSILON);
    }

    t -= h;

    block.set_input(3);
    block.run(UPDATE_MODE);

    for(t+=h; t<=30.000001; t+=h)
    {
        block.run(INTEGRATE_MODE);
        block.run(UPDATE_MODE);
        TEST_ASSERT(fabs(block.get_output()-8.0)<FLOAT_EPSILON);
    }
}

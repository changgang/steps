#include "header/block/integral_block_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/basic/constants.h"

#include <cstdlib>
#include <cstring>
#include <cstdio>

using namespace std;

INTEGRAL_BLOCK_TEST::INTEGRAL_BLOCK_TEST()
{
    TEST_ADD(INTEGRAL_BLOCK_TEST::test_constructor);
    TEST_ADD(INTEGRAL_BLOCK_TEST::test_set_get_T);
    TEST_ADD(INTEGRAL_BLOCK_TEST::test_initialize_integrate_update_once);
    //TEST_ADD(INTEGRAL_BLOCK_TEST::test_initialize_integrate_update_with_limiter);
    TEST_ADD(INTEGRAL_BLOCK_TEST::test_step_response_without_limiter);
    TEST_ADD(INTEGRAL_BLOCK_TEST::test_step_response_with_limiter);
}

void INTEGRAL_BLOCK_TEST::setup()
{
    set_dynamic_simulation_time_step_in_s(0.01);
}

void INTEGRAL_BLOCK_TEST::tear_down()
{
    show_test_end_information();
}

void INTEGRAL_BLOCK_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"INTEGRAL_BLOCK_TEST");

    TEST_ASSERT(block.get_limiter_type()==NO_LIMITER);
}

void INTEGRAL_BLOCK_TEST::test_set_get_T()
{
    show_test_information_for_function_of_class(__FUNCTION__,"INTEGRAL_BLOCK_TEST");

    block.set_T_in_s(0.2);
    TEST_ASSERT(block.get_T_in_s()==0.2);

    block.set_T_in_s(0.5);
    TEST_ASSERT(block.get_T_in_s()==0.5);
}

void INTEGRAL_BLOCK_TEST::test_initialize_integrate_update_once()
{
    show_test_information_for_function_of_class(__FUNCTION__,"INTEGRAL_BLOCK_TEST");

    double h = get_dynamic_simulation_time_step_in_s();

    block.set_T_in_s(0.5);
    double output = 10.0;

    block.set_output(output);

    block.initialize();

    TEST_ASSERT(block.get_input()==0.0);
    TEST_ASSERT(block.get_state()==10.0);
    TEST_ASSERT(block.get_store()==10.0);
    TEST_ASSERT(block.get_dstate()==0.0);

    block.set_input(0.2);
    block.run(UPDATE_MODE);
    TEST_ASSERT(fabs(block.get_output()-10.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(block.get_state()-10.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(block.get_store()-(10.0+0.5*h/0.5*0.2))<FLOAT_EPSILON);
    block.run(INTEGRATE_MODE);

    TEST_ASSERT(fabs(block.get_output()-(10.0+h*0.2/0.5))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(block.get_state()-(10.0+h*0.2/0.5))<FLOAT_EPSILON);

    block.run(UPDATE_MODE);

    TEST_ASSERT(fabs(block.get_output()-(10.0+0.2/0.5*h))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(block.get_state()-(10.0+h*0.2/0.5))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(block.get_store()-(10.0+0.2/0.5*h+0.5*0.2/0.5*h))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(block.get_dstate()-(0.2/0.5))<FLOAT_EPSILON);
}
void INTEGRAL_BLOCK_TEST::test_step_response_without_limiter()
{
    show_test_information_for_function_of_class(__FUNCTION__,"INTEGRAL_BLOCK_TEST");

    double h = get_dynamic_simulation_time_step_in_s();
    double t = -5.0*h;

    block.set_T_in_s(0.5);
    double output = 10.0;

    block.set_output(output);
    block.initialize();

    for(; t<=0.0000001; t +=h)
    {
        block.run(INTEGRATE_MODE);
        block.run(UPDATE_MODE);
        TEST_ASSERT(fabs(block.get_output()-10.0)<FLOAT_EPSILON);
        TEST_ASSERT(fabs(block.get_state()-10.0)<FLOAT_EPSILON);
    }
    t -=h;
    block.set_input(0.2);
    block.run(UPDATE_MODE);

    for(t+=h; t<=10.0; t+=h)
    {
        block.run(INTEGRATE_MODE);
        block.run(UPDATE_MODE);
        TEST_ASSERT(fabs(block.get_output()-(10.0+t*0.2/0.5))<FLOAT_EPSILON);
        TEST_ASSERT(fabs(block.get_state()-(10.0+t*0.2/0.5))<FLOAT_EPSILON);
    }
}
void INTEGRAL_BLOCK_TEST::test_step_response_with_limiter()
{
    show_test_information_for_function_of_class(__FUNCTION__,"INTEGRAL_BLOCK_TEST");

    double h = get_dynamic_simulation_time_step_in_s();
    double t = -5.0*h;

    block.set_T_in_s(0.5);
    block.set_limiter_type(NON_WINDUP_LIMITER);
    block.set_upper_limit(12.0);
    block.set_lower_limit(8.0);

    double output = 10.0;

    block.set_output(output);

    block.initialize();
    for(; t<=0.000001; t +=h)
    {
        block.run(INTEGRATE_MODE);
        block.run(UPDATE_MODE);
        TEST_ASSERT(fabs(block.get_output()-10.0)<FLOAT_EPSILON);
        TEST_ASSERT(fabs(block.get_state()-10.0)<FLOAT_EPSILON);
    }
    t -=h;
    block.set_input(0.2);
    block.run(UPDATE_MODE);

    for(t+=h; t<=10.0+0.0001; t+=h)
    {
        block.run(INTEGRATE_MODE);
        block.run(UPDATE_MODE);
        if(t<=5.0000001)
        {
            TEST_ASSERT(fabs(block.get_output()-(10.0+t*0.2/0.5))<FLOAT_EPSILON);
            TEST_ASSERT(fabs(block.get_state()-(10.0+t*0.2/0.5))<FLOAT_EPSILON);
        }
        else
        {
            TEST_ASSERT(fabs(block.get_output()-12.0)<FLOAT_EPSILON);
            TEST_ASSERT(fabs(block.get_state()-12.0)<FLOAT_EPSILON);
        }
    }
    t -=h;
    block.set_input(-1.0);
    block.run(UPDATE_MODE);

    for(t+=h; t<=15.000001; t+=h)
    {
        block.run(INTEGRATE_MODE);
        block.run(UPDATE_MODE);
        if(t<=12.0000001)
        {
            TEST_ASSERT(fabs(block.get_output()-(12.0-(t-10.0)*1.0/0.5))<FLOAT_EPSILON);
            TEST_ASSERT(fabs(block.get_state()-(12.0-(t-10.0)*1.0/0.5))<FLOAT_EPSILON);
        }
        else
        {
            TEST_ASSERT(fabs(block.get_output()-8.0)<FLOAT_EPSILON);
            TEST_ASSERT(fabs(block.get_state()-8.0)<FLOAT_EPSILON);
        }
    }
}

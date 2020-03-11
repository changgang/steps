#include "header/basic/test_macro.h"
#include "header/block/pi_block_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/basic/constants.h"

#include <cstdlib>
#include <cstring>
#include <cstdio>

#ifdef ENABLE_STEPS_TEST
using namespace std;

PI_BLOCK_TEST::PI_BLOCK_TEST() : block(default_toolkit)
{
    TEST_ADD(PI_BLOCK_TEST::test_constructor);
    TEST_ADD(PI_BLOCK_TEST::test_set_get_Kp);
    TEST_ADD(PI_BLOCK_TEST::test_set_get_Ki);
    TEST_ADD(PI_BLOCK_TEST::test_step_response_without_limiter);
}

void PI_BLOCK_TEST::setup()
{
    default_toolkit.set_dynamic_simulation_time_step_in_s(0.01);
}

void PI_BLOCK_TEST::tear_down()
{
    show_test_end_information();
}

void PI_BLOCK_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PI_BLOCK_TEST");

    TEST_ASSERT(block.get_limiter_type()==NO_LIMITER);
}

void PI_BLOCK_TEST::test_set_get_Kp()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PI_BLOCK_TEST");

    block.set_Kp(1.5);
    TEST_ASSERT(block.get_Kp()==1.5);

    block.set_Kp(-1.5);
    TEST_ASSERT(block.get_Kp()==-1.5);
}

void PI_BLOCK_TEST::test_set_get_Ki()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PI_BLOCK_TEST");

    block.set_Ki(1.5);
    TEST_ASSERT(fabs(block.get_Ki()-1.5)<FLOAT_EPSILON);

    block.set_Ki(-1.5);
    TEST_ASSERT(fabs(block.get_Ki()-(-1.5))<FLOAT_EPSILON);
}

void PI_BLOCK_TEST::test_step_response_without_limiter()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PI_BLOCK_TEST");

    default_toolkit.set_dynamic_simulation_time_step_in_s(0.001);
    double h = default_toolkit.get_dynamic_simulation_time_step_in_s();

    double Kp = 10.0;
    double Ki = 2.0;
    block.set_Kp(Kp);
    block.set_Ki(Ki);

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

    double input = block.get_input();

    TEST_ASSERT(input == 0.0);


    double y;

    input = input+1.0;
    block.set_input(input);
    block.run(UPDATE_MODE);
    y = 1.0*Kp+10.0;
    TEST_ASSERT(fabs(block.get_output()-y)<FLOAT_EPSILON);
    for(t=h; t<=10.0000001; t +=h)
    {
        block.run(INTEGRATE_MODE);
        block.run(UPDATE_MODE);
        y = 10.0;
        y += Kp*1.0;
        y += Ki*1.0*t;
        TEST_ASSERT(fabs(block.get_output()-y)<FLOAT_EPSILON);
    }
}

#endif

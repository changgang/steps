#include "header/block/pd_block_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/basic/constants.h"

#include <cstdlib>
#include <cstring>
#include <cstdio>

using namespace std;

PD_BLOCK_TEST::PD_BLOCK_TEST()
{
    TEST_ADD(PD_BLOCK_TEST::test_constructor);
    TEST_ADD(PD_BLOCK_TEST::test_set_get_Kp);
    TEST_ADD(PD_BLOCK_TEST::test_set_get_Kd);
    TEST_ADD(PD_BLOCK_TEST::test_set_get_Td);
    TEST_ADD(PD_BLOCK_TEST::test_step_response_without_limiter);
}

void PD_BLOCK_TEST::setup()
{
    default_toolkit.set_dynamic_simulation_time_step_in_s(0.01);
}

void PD_BLOCK_TEST::tear_down()
{
    show_test_end_information();
}

void PD_BLOCK_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PD_BLOCK_TEST");

    TEST_ASSERT(block.get_limiter_type()==NO_LIMITER);
}

void PD_BLOCK_TEST::test_set_get_Kp()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PD_BLOCK_TEST");

    block.set_Kp(1.5);
    TEST_ASSERT(block.get_Kp()==1.5);

    block.set_Kp(-1.5);
    TEST_ASSERT(block.get_Kp()==-1.5);
}

void PD_BLOCK_TEST::test_set_get_Kd()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PD_BLOCK_TEST");

    block.set_Kd(1.5);
    TEST_ASSERT(block.get_Kd()==1.5);

    block.set_Kd(-1.5);
    TEST_ASSERT(block.get_Kd()==-1.5);
}

void PD_BLOCK_TEST::test_set_get_Td()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PD_BLOCK_TEST");

    block.set_Td_in_s(0.2);
    TEST_ASSERT(block.get_Td_in_s()==0.2);

    block.set_Td_in_s(0.5);
    TEST_ASSERT(block.get_Td_in_s()==0.5);
}

void PD_BLOCK_TEST::test_step_response_without_limiter()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PD_BLOCK_TEST");

    default_toolkit.set_dynamic_simulation_time_step_in_s(0.001);
    double h = default_toolkit.get_dynamic_simulation_time_step_in_s();

    double Kp = 10.0;
    double Kd = 2.0;
    double Td = 5.0;
    block.set_Kp(Kp);
    block.set_Kd(Kd);
    block.set_Td_in_s(Td);

    double t = 5.0*h;
    // transfer function is
    // Kp+Kd/Td*s
    // step: t, in s-domain is: 1/s
    // the step response of the first part is : Kp/s, in time domain: Kp
    // the step response of the second part is: (Kd/Td)/(s+1/Td) , in time domain: Kd/Td*e^(-t/Td)
    // the total response is
    // Kp+Kd/Td*e^(-t/Td)

    double output=0.0;
    block.set_output(output);
    block.initialize();
    TEST_ASSERT(block.get_differentiator_state()==0.0);
    TEST_ASSERT(block.get_input()==0.0);

    double input = block.get_input();

    TEST_ASSERT(input == 0.0);

    input = 1.0;
    block.set_input(input);
    block.run(UPDATE_MODE);

    double y;
    y=Kp;
    y += Kd/Td;
    TEST_ASSERT(fabs(block.get_output()-y)<FLOAT_EPSILON);
    for(t=h; t<=10.0000001; t +=h)
    {
        block.run(INTEGRATE_MODE);
        block.run(UPDATE_MODE);
        y = Kp;
        y += Kd/Td*exp(-t/Td);
        TEST_ASSERT(fabs(block.get_output()-y)<1e-8);
    }
}

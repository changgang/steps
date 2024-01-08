#include "header/basic/test_macro.h"
#include "header/block/pid_block_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/basic/constants.h"

#include <cstdlib>
#include <cstring>
#include <cstdio>

#ifdef ENABLE_STEPS_TEST
using namespace std;

PID_BLOCK_TEST::PID_BLOCK_TEST() : block(default_toolkit)
{
    TEST_ADD(PID_BLOCK_TEST::test_constructor);
    TEST_ADD(PID_BLOCK_TEST::test_set_get_Kp);
    TEST_ADD(PID_BLOCK_TEST::test_set_get_Ki);
    TEST_ADD(PID_BLOCK_TEST::test_set_get_Kd);
    TEST_ADD(PID_BLOCK_TEST::test_set_get_Td);
    TEST_ADD(PID_BLOCK_TEST::test_step_response_without_limiter);
}

void PID_BLOCK_TEST::setup()
{
    default_toolkit.set_dynamic_simulation_time_step_in_s(0.01);
}

void PID_BLOCK_TEST::tear_down()
{
    show_test_end_information();
}

void PID_BLOCK_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PID_BLOCK_TEST");

    TEST_ASSERT(block.get_limiter_type()==NO_LIMITER);
}

void PID_BLOCK_TEST::test_set_get_Kp()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PID_BLOCK_TEST");

    block.set_Kp(1.5);
    TEST_ASSERT(block.get_Kp()==1.5);

    block.set_Kp(-1.5);
    TEST_ASSERT(block.get_Kp()==-1.5);
}

void PID_BLOCK_TEST::test_set_get_Ki()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PID_BLOCK_TEST");

    block.set_Ki(1.5);
    TEST_ASSERT(fabs(block.get_Ki()-1.5)<FLOAT_EPSILON);

    block.set_Ki(-1.5);
    TEST_ASSERT(fabs(block.get_Ki()-(-1.5))<FLOAT_EPSILON);
}

void PID_BLOCK_TEST::test_set_get_Kd()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PID_BLOCK_TEST");

    block.set_Kd(1.5);
    TEST_ASSERT(block.get_Kd()==1.5);

    block.set_Kd(-1.5);
    TEST_ASSERT(block.get_Kd()==-1.5);
}

void PID_BLOCK_TEST::test_set_get_Td()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PID_BLOCK_TEST");

    block.set_Td_in_s(0.2);
    TEST_ASSERT(block.get_Td_in_s()==0.2);

    block.set_Td_in_s(0.5);
    TEST_ASSERT(block.get_Td_in_s()==0.5);
}

void PID_BLOCK_TEST::test_step_response_without_limiter()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PID_BLOCK_TEST");

    default_toolkit.set_dynamic_simulation_time_step_in_s(0.001);
    double h = default_toolkit.get_dynamic_simulation_time_step_in_s();

    double Kp = 10.0;
    double Ki = 2.0;
    double Kd = 2.0;
    double Td = 5.0;
    block.set_Kp(Kp);
    block.set_Ki(Ki);
    block.set_Kd(Kd);
    block.set_Td_in_s(Td);

    double t = 5.0*h;
    // transfer function is
    // Kp+Ki/s+Kd*s/(1+s*Td)
    // the step response of the first part is : Kp/s, in time domain: Kp
    // the step response of the second part is: Ki/s^2, in time domain: Ki*t
    // the step response of the third part is: Kd/Td, in time domain: 0
    // the total response is
    // Kp+Ki*t+0


    double output = 10.0;
    block.set_output(output);
    block.initialize();
    TEST_ASSERT(block.get_integrator_state()==output);
    TEST_ASSERT(block.get_differentiator_state()==0.0);
    TEST_ASSERT(block.get_input()==0.0);

    double input = block.get_input();

    TEST_ASSERT(input == 0.0);


    double y;

    input = input+1.0;
    block.set_input(input);

    block.run(UPDATE_MODE);
    y = 1.0*Kp+10.0+1.0*Kd/Td;
    TEST_ASSERT(fabs(block.get_output()-y)<FLOAT_EPSILON);
    for(t=h; t<=10.0000001; t +=h)
    {
        block.run(INTEGRATE_MODE);
        block.run(UPDATE_MODE);
        y = 10.0;
        y += Kp*1.0;
        y += Ki*1.0*t;
        y += (1.0*Kd/Td*exp(-t/Td));
        TEST_ASSERT(fabs(block.get_output()-y)<1e-8);
    }
}

void PID_BLOCK_TEST::test_linearized_ABCD()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PID_BLOCK_TEST");
    default_toolkit.set_dynamic_simulation_time_step_in_s(0.001);
    double h = default_toolkit.get_dynamic_simulation_time_step_in_s();

    double kp = 2.0, ki = 5.0, kd = 1.0, td=1.5;
    block.set_Kp(kp);
    block.set_Ki(ki);
    block.set_Kd(kd);
    block.set_Td_in_s(td);

    double u = 1.0, y = 2.0;
    block.set_input(u);
    block.set_output(y);
    block.initialize();
    double si = block.get_integrator_state();
    double sd = block.get_differentiator_state();

    TEST_ASSERT(fabs(si-1.2)<FLOAT_EPSILON);

    STEPS_SPARSE_MATRIX A = block.get_linearized_system_A();
    STEPS_SPARSE_MATRIX B = block.get_linearized_system_B();
    STEPS_SPARSE_MATRIX C = block.get_linearized_system_C();
    STEPS_SPARSE_MATRIX D = block.get_linearized_system_D();

    TEST_ASSERT(fabs(A.get_entry_value(0,0)-(kd/td*u-si))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(A.get_entry_value(0,1)-(kd/td*u-si))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(A.get_entry_value(1,0)-(kd/td*u-si))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(A.get_entry_value(1,1)-(kd/td*u-si))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(B.get_entry_value(0,0)-(kd/td*u-si))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(C.get_entry_value(0,0)-(kd/td*u-si))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(D.get_entry_value(0,0)-(kd/td*u-si))<FLOAT_EPSILON);
}

#endif

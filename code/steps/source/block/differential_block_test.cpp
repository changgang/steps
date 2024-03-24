#include "header/basic/test_macro.h"
#include "header/block/differential_block_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/basic/constants.h"

#include <cstdlib>
#include <cstring>
#include <cstdio>

#ifdef ENABLE_STEPS_TEST
using namespace std;

DIFFERENTIAL_BLOCK_TEST::DIFFERENTIAL_BLOCK_TEST() : block(default_toolkit)
{
    TEST_ADD(DIFFERENTIAL_BLOCK_TEST::test_constructor);
    TEST_ADD(DIFFERENTIAL_BLOCK_TEST::test_set_get_K);
    TEST_ADD(DIFFERENTIAL_BLOCK_TEST::test_set_get_T);
    TEST_ADD(DIFFERENTIAL_BLOCK_TEST::test_step_response_without_limiter);
}

void DIFFERENTIAL_BLOCK_TEST::setup()
{
    default_toolkit.set_dynamic_simulation_time_step_in_s(0.01);
}

void DIFFERENTIAL_BLOCK_TEST::tear_down()
{
    show_test_end_information();
}

void DIFFERENTIAL_BLOCK_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DIFFERENTIAL_BLOCK_TEST");

    TEST_ASSERT(block.get_limiter_type()==NO_LIMITER);
}

void DIFFERENTIAL_BLOCK_TEST::test_set_get_K()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DIFFERENTIAL_BLOCK_TEST");

    block.set_K(1.5);
    TEST_ASSERT(fabs(block.get_K()-1.5)<FLOAT_EPSILON);

    block.set_K(-1.5);
    TEST_ASSERT(fabs(block.get_K()-(-1.5))<FLOAT_EPSILON);
}

void DIFFERENTIAL_BLOCK_TEST::test_set_get_T()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DIFFERENTIAL_BLOCK_TEST");

    block.set_T_in_s(0.2);
    TEST_ASSERT(fabs(block.get_T_in_s()-0.2)<FLOAT_EPSILON);

    block.set_T_in_s(0.5);
    TEST_ASSERT(fabs(block.get_T_in_s()-0.5)<FLOAT_EPSILON);
}

void DIFFERENTIAL_BLOCK_TEST::test_step_response_without_limiter()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DIFFERENTIAL_BLOCK_TEST");
    default_toolkit.set_dynamic_simulation_time_step_in_s(0.001);
    double h = default_toolkit.get_dynamic_simulation_time_step_in_s();

    double K = 2.0;
    double T = 5.0;
    block.set_K(K);
    block.set_T_in_s(T);


    double t = 5.0*h;
    // solution to unit step response is
    // sK/(s(1+sT)) or K/(1+sT)), or (K/T)/(s+1/T) is
    // (K/T)*e^(-t/T)
    // no matter which input is set, solution will be
    // y = K/T*e^(-t/T)

    double input = 10.0;
    block.set_input(input);
    block.initialize();
    TEST_ASSERT(fabs(block.get_state()-(K/T*input))<FLOAT_EPSILON);

    double y;

    input = input+1.0;
    block.set_input(input);
    block.run(DYNAMIC_UPDATE_MODE);
    y = 1.0*K/T;
    TEST_ASSERT(fabs(block.get_output()-y)<FLOAT_EPSILON);
    for(t=h; t<=10.0000001; t +=h)
    {
        block.run(DYNAMIC_INTEGRATE_MODE);
        block.run(DYNAMIC_UPDATE_MODE);
        y =1.0*K/T*exp(-t/T);
        TEST_ASSERT(fabs(block.get_output()-y)<1e-8);
    }
}


void DIFFERENTIAL_BLOCK_TEST::test_linearized_ABCD()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DIFFERENTIAL_BLOCK_TEST");
    default_toolkit.set_dynamic_simulation_time_step_in_s(0.001);
    double h = default_toolkit.get_dynamic_simulation_time_step_in_s();

    double K = 2.0;
    double T = 5.0;
    block.set_K(K);
    block.set_T_in_s(T);

    double u = 1.0;
    block.set_input(u);
    block.initialize();
    double s = block.get_state();
    double y = block.get_output();

    TEST_ASSERT(fabs(s-1.2)<FLOAT_EPSILON);

    STEPS_SPARSE_MATRIX A = block.get_linearized_matrix_A();
    STEPS_SPARSE_MATRIX B = block.get_linearized_matrix_B();
    STEPS_SPARSE_MATRIX C = block.get_linearized_matrix_C();
    STEPS_SPARSE_MATRIX D = block.get_linearized_matrix_D();

    TEST_ASSERT(fabs(A.get_entry_value(0,0)-(K/T*u-s))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(B.get_entry_value(0,0)-(K/T*u-s))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(C.get_entry_value(0,0)-(K/T*u-s))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(D.get_entry_value(0,0)-(K/T*u-s))<FLOAT_EPSILON);
}

#endif

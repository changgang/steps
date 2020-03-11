#include "header/basic/test_macro.h"
#include "header/block/second_order_block_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/basic/constants.h"

#include <cstdlib>
#include <cstring>
#include <cstdio>

#ifdef ENABLE_STEPS_TEST
using namespace std;

SECOND_ORDER_BLOCK_TEST::SECOND_ORDER_BLOCK_TEST() : block(default_toolkit)
{
    TEST_ADD(SECOND_ORDER_BLOCK_TEST::test_constructor);
    TEST_ADD(SECOND_ORDER_BLOCK_TEST::test_set_get_a);
    TEST_ADD(SECOND_ORDER_BLOCK_TEST::test_set_get_b);
    TEST_ADD(SECOND_ORDER_BLOCK_TEST::test_set_get_c);
    TEST_ADD(SECOND_ORDER_BLOCK_TEST::test_set_get_d);
    TEST_ADD(SECOND_ORDER_BLOCK_TEST::test_set_get_e);
    TEST_ADD(SECOND_ORDER_BLOCK_TEST::test_step_response_when_d_is_zero);
    TEST_ADD(SECOND_ORDER_BLOCK_TEST::test_step_response_when_d_is_nonzero);
}

void SECOND_ORDER_BLOCK_TEST::setup()
{
    default_toolkit.set_dynamic_simulation_time_step_in_s(0.01);
}

void SECOND_ORDER_BLOCK_TEST::tear_down()
{
    show_test_end_information();
}

void SECOND_ORDER_BLOCK_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SECOND_ORDER_BLOCK_TEST");

    TEST_ASSERT(block.get_limiter_type()==NO_LIMITER);
}

void SECOND_ORDER_BLOCK_TEST::test_set_get_a()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SECOND_ORDER_BLOCK_TEST");

    block.set_a(1.0);
    TEST_ASSERT(block.get_a()==1.0);

    block.set_a(0.1);
    TEST_ASSERT(block.get_a()==0.1);
}

void SECOND_ORDER_BLOCK_TEST::test_set_get_b()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SECOND_ORDER_BLOCK_TEST");

    block.set_b(2.0);
    TEST_ASSERT(block.get_b()==2.0);

    block.set_b(0.2);
    TEST_ASSERT(block.get_b()==0.2);
}

void SECOND_ORDER_BLOCK_TEST::test_set_get_c()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SECOND_ORDER_BLOCK_TEST");

    block.set_c(3.0);
    TEST_ASSERT(block.get_c()==3.0);

    block.set_c(0.3);
    TEST_ASSERT(block.get_c()==0.3);
}

void SECOND_ORDER_BLOCK_TEST::test_set_get_d()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SECOND_ORDER_BLOCK_TEST");

    block.set_d(4.0);
    TEST_ASSERT(block.get_d()==4.0);

    block.set_d(0.4);
    TEST_ASSERT(block.get_d()==0.4);
}

void SECOND_ORDER_BLOCK_TEST::test_set_get_e()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SECOND_ORDER_BLOCK_TEST");

    block.set_e(5.0);
    TEST_ASSERT(block.get_e()==5.0);

    block.set_e(0.5);
    TEST_ASSERT(block.get_e()==0.5);
}

void SECOND_ORDER_BLOCK_TEST::test_step_response_when_d_is_zero()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SECOND_ORDER_BLOCK_TEST");

}

void SECOND_ORDER_BLOCK_TEST::test_step_response_when_d_is_nonzero()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SECOND_ORDER_BLOCK_TEST");

}

#endif

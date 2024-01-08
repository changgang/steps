#ifndef FIRST_ORDER_BLOCK_TEST_H
#define FIRST_ORDER_BLOCK_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/block/first_order_block.h"
#include "header/STEPS.h"
using namespace std;

class FIRST_ORDER_BLOCK_TEST : public Test::Suite
{
    public:
        FIRST_ORDER_BLOCK_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();
        void test_set_get_K();
        void test_set_get_T();
        void test_initialize_integrate_update_once();
        void test_step_response_without_limiter();
        void test_step_response_with_limiter();
        void test_step_response_without_limiter_when_T_is_zero();
        void test_linearized_ABCD();
    private:
        FIRST_ORDER_BLOCK block;
};
#endif // FIRST_ORDER_BLOCK_TEST_H

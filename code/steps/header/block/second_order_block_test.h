#ifndef SECOND_ORDER_BLOCK_TEST_H
#define SECOND_ORDER_BLOCK_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/block/second_order_block.h"
#include "header/STEPS.h"
using namespace std;

class SECOND_ORDER_BLOCK_TEST : public Test::Suite
{
    public:
        SECOND_ORDER_BLOCK_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();
        void test_set_get_a();
        void test_set_get_b();
        void test_set_get_c();
        void test_set_get_d();
        void test_set_get_e();
        void test_initialize_integrate_update_once();
        void test_step_response_when_d_is_zero();
        void test_step_response_when_d_is_nonzero();
        void test_linearized_ABCD();
    private:
        SECOND_ORDER_BLOCK block;
};
#endif // SECOND_ORDER_BLOCK_TEST_H

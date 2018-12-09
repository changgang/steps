#ifndef LEAD_LAG_BLOCK_TEST_H
#define LEAD_LAG_BLOCK_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/block/lead_lag_block.h"

using namespace std;

class LEAD_LAG_BLOCK_TEST : public Test::Suite
{
    public:
        LEAD_LAG_BLOCK_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();
        void test_set_get_K();
        void test_set_get_T1();
        void test_set_get_T2();
        void test_step_response_without_limiter();
        void test_step_response_without_limiter_when_T1_is_zero();
        void test_step_response_without_limiter_when_T1_and_T2_are_zero();
    private:
        LEAD_LAG_BLOCK block;
};
#endif // LEAD_LAG_BLOCK_TEST_H

#ifndef INTEGRAL_BLOCK_TEST_H
#define INTEGRAL_BLOCK_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "thirdparty/cpptest/cpptest.h"

#include "header/block/integral_block.h"

using namespace std;

class INTEGRAL_BLOCK_TEST : public Test::Suite
{
    public:
        INTEGRAL_BLOCK_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();
        void test_set_get_T();
        void test_initialize_integrate_update_once();
        void test_step_response_without_limiter();
        void test_step_response_with_limiter();
    private:
        INTEGRAL_BLOCK block;
};
#endif // INTEGRAL_BLOCK_TEST_H

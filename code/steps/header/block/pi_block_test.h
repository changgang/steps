#ifndef PI_BLOCK_TEST_H
#define PI_BLOCK_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/block/pi_block.h"
#include "header/STEPS.h"
using namespace std;

class PI_BLOCK_TEST : public Test::Suite
{
    public:
        PI_BLOCK_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();
        void test_set_get_Kp();
        void test_set_get_Ki();
        void test_step_response_without_limiter();
    private:
        PI_BLOCK block;
};
#endif // PI_BLOCK_TEST_H

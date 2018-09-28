#ifndef PID_BLOCK_TEST_H
#define PID_BLOCK_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "thirdparty/cpptest/cpptest.h"

#include "header/block/pid_block.h"

using namespace std;

class PID_BLOCK_TEST : public Test::Suite
{
    public:
        PID_BLOCK_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();
        void test_set_get_Kp();
        void test_set_get_Ki();
        void test_set_get_Kd();
        void test_set_get_Td();
        void test_step_response_without_limiter();
    private:
        PID_BLOCK block;
};
#endif // PID_BLOCK_TEST_H

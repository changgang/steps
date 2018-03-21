#ifndef TIMER_TEST_H
#define TIMER_TEST_H

#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "thirdparty/cpptest/cpptest.h"
#include "header/toolkit/dynamic_simulator/dynamic_simulator.h"
#include "header/basic/timer.h"
using namespace std;

class TIMER_TEST : public Test::Suite
{
    public:
        TIMER_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();
        void test_set_get_time_interval();
        void test_start_reset_timer();
        void test_is_timed_out();
        void test_is_valid();
        void test_copy_with_operator_equal();
    private:
        TIMER* timer;
        POWER_SYSTEM_DATABASE* psdb;
        DYNAMICS_SIMULATOR* simulator;
};
#endif // TIMER_TEST_H

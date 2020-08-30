#ifndef LCC_TEST_H
#define LCC_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/device/lcc.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"

using namespace std;

class LCC_TEST : public Test::Suite
{
    public:
        LCC_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();
        void test_set_get_number_of_bridge();
        void test_set_get_voltage_drop_per_bridge();
        void test_set_get_max_angle();
        void test_set_get_min_angle();
        void test_set_get_angle();
        void test_clear();
        void test_copy_with_operator_equal();
        void test_solve_with_desired_dc_voltage_and_current();
    private:
        LCC converter;
};

#endif

#ifndef WT_GENERATOR_TEST_H
#define WT_GENERATOR_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/device/wt_generator.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
using namespace std;

class WT_GENERATOR_TEST : public Test::Suite
{
    public:
        WT_GENERATOR_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();
        void test_set_get_generator_bus();
        void test_set_get_source_impedance();
        void test_copy_with_operator_equal();
        void test_get_device_id();

    private:
        WT_GENERATOR wt_generator;
};

#endif //WT_GENERATOR_TEST_H

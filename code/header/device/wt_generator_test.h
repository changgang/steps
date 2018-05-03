#ifndef WT_GENERATOR_TEST_H
#define WT_GENERATOR_TEST_H

#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "thirdparty/cpptest/cpptest.h"

#include "header/device/wt_generator.h"
#include "header/power_system_database.h"
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
        POWER_SYSTEM_DATABASE* db;
        WT_GENERATOR* wt_generator;
};

#endif //WT_GENERATOR_TEST_H

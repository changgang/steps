#ifndef FIXED_SHUNT_TEST_H
#define FIXED_SHUNT_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/device/fixed_shunt.h"
#include "header/power_system_database.h"

using namespace std;

class FIXED_SHUNT_TEST : public Test::Suite
{
    public:
        FIXED_SHUNT_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();
        void test_set_get_shunt_bus();
        void test_set_get_identifier();
        void test_set_get_status();
        void test_set_get_nominal_impedance_shunt();

        void test_is_valid();
        void test_clear();
        void test_copy_with_operator_equal();
        void test_is_connected_to_bus();

        void test_get_device_id();

        void test_get_actual_impedance_shunt();
    private:
        FIXED_SHUNT fixed_shunt;
};

#endif

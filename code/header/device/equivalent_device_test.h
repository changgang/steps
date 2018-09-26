#ifndef EQUIVALENT_DEVICE_TEST_H
#define EQUIVALENT_DEVICE_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "thirdparty/cpptest/cpptest.h"

#include "header/device/equivalent_device.h"
#include "header/power_system_database.h"

using namespace std;

class EQUIVALENT_DEVICE_TEST : public Test::Suite
{
    public:
        EQUIVALENT_DEVICE_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();
        void test_set_get_equivalent_device_bus();
        void test_set_get_identifier();
        void test_set_get_status();
        void test_set_get_equivalent_voltage_source_status();
        void test_set_get_equivalent_load_status();
        void test_set_get_equivalent_voltage_source_voltage();
        void test_set_get_equivalent_voltage_source_impedance();
        void test_set_get_equivalent_nominal_constant_power_load();
        void test_set_get_equivalent_nominal_constant_current_load();
        void test_set_get_equivalent_nominal_constant_impedance_load();

        void test_is_valid();
        void test_clear();
        void test_copy_with_operator_equal();
        void test_is_connected_to_bus();

        void test_get_device_id();
    private:
        EQUIVALENT_DEVICE edevice;
};

#endif

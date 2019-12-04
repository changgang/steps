#ifndef BUS_TEST_H
#define BUS_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/device/bus.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"

using namespace std;

class BUS_TEST : public Test::Suite
{
    public:
        BUS_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();
        void test_set_get_bus_number();
        void test_set_get_bus_name();
        void test_set_get_base_voltage();
        void test_set_get_bus_type();
        void test_set_get_area_number();
        void test_set_get_zone_number();
        void test_set_get_owner_number();
        void test_set_get_positive_sequence_voltage_with_nonzero_base_voltage();
        void test_set_get_positive_sequence_voltage_with_zero_base_voltage();
        void test_set_get_positive_sequence_angle();
        void test_set_get_negative_sequence_voltage_with_zero_base_voltage();
        void test_set_get_negative_sequence_angle();
        void test_set_get_zero_sequence_voltage_with_zero_base_voltage();
        void test_set_get_zero_sequence_angle();
        void test_set_get_normal_voltage_upper_limit();
        void test_set_get_normal_voltage_lower_limit();
        void test_set_get_emergency_voltage_upper_limit();
        void test_set_get_emergency_voltage_lower_limit();
        void test_set_get_voltage_upper_limit();
        void test_set_get_voltage_lower_limit();
        void test_set_get_equivalent_bus_number();
        void test_is_bus_overshadowed();
        void test_set_get_base_frequency_period();
        void test_set_get_voltage_to_regulate();
        void test_is_valid();
        void test_clear();
        void test_copy_with_operator_equal();
        void test_is_connected_to_bus();

        void test_get_device_id();

        void test_set_get_fault();
        void test_clear_fault();
        void test_is_faulted();

        void test_set_get_frequency_deviation();
    private:
        BUS bus;
};

#endif

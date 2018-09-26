#ifndef LINE_TEST_H
#define LINE_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "thirdparty/cpptest/cpptest.h"

#include "header/device/line.h"
#include "header/power_system_database.h"

using namespace std;

class LINE_TEST : public Test::Suite
{
    public:
        LINE_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();
        void test_set_get_sending_receiving_side_bus();
        void test_set_get_identifier();
        void test_set_get_breaker_status();
        void test_set_get_line_z120();
        void test_set_get_line_y120();
        void test_set_get_shunt_y120_at_sending_side();
        void test_set_get_shunt_y120_at_receiving_side();
        void test_set_get_rating();
        void test_set_get_meter_end_bus();
        void test_set_get_length();
        void test_set_get_ownership();
        void test_set_get_fault();
        void test_clear_fault();
        void test_get_fault_count();
        void test_is_faulted();
        void test_is_valid();
        void test_clear();
        void test_copy_with_operator_equal();
        void test_is_connected_to_bus();

        void test_get_device_id();

        void test_get_line_base_voltage();
        void test_get_line_voltage_at_two_sides();
        void test_get_line_current_at_two_sides();
        void test_get_line_power_at_two_sides();
        void test_get_line_apparent_impedance_at_two_sides();
    private:
        LINE line;
};

#endif //LINE_TEST_H

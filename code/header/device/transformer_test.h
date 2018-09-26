#ifndef TRANSFORMER_TEST_H
#define TRANSFORMER_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "thirdparty/cpptest/cpptest.h"

#include "header/device/transformer.h"
#include "header/power_system_database.h"
using namespace std;

class TRANSFORMER_TEST : public Test::Suite
{
    public:
        TRANSFORMER_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();
        void test_set_get_winding_buses();
        void test_set_get_identifier();
        void test_set_get_name();
        void test_set_get_winding_breaker_status();
        void test_set_get_non_metered_end_bus();
        void test_set_get_winding_nominal_capacity();
        void test_set_get_winding_nominal_voltage();
        void test_set_get_winding_leakage_impedance();
        void test_set_get_magnetizing_admittance();
        void test_set_get_winding_turn_ratio();
        void test_set_get_winding_angle_shift();
        void test_set_get_winding_rating();
        void test_set_get_winding_max_min_turn_ratio();
        void test_set_get_winding_max_min_angle_shift();
        void test_set_get_winding_number_of_taps();
        void test_set_get_winding_controlled_bus();
        void test_set_get_winding_controlled_max_min_voltage();
        void test_set_get_controlled_reactive_power_into_windings();
        void test_set_get_controlled_active_power_into_windings();

        void test_is_valid();
        void test_clear();
        void test_copy_with_operator_equal();
        void test_is_connected_to_bus();

        void test_get_device_id();

        void test_check_transformer_type();
        void test_get_winding_complex_current_power_2_winding_trans();
        void test_get_winding_complex_current_power_3_winding_trans();
    private:
        void prepare_two_winding_transformer_bus_and_identifier();
        void prepare_three_winding_transformer_bus_and_identifier();
        TRANSFORMER transformer;
};

#endif //TRANSFORMER_TEST_H


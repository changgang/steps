#ifndef CONVERTER_TEST_H
#define CONVERTER_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/device/converter.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"

using namespace std;

class CONVERTER_TEST : public Test::Suite
{
    public:
        CONVERTER_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();
        void test_set_get_side();
        void test_set_get_bus();
        void test_set_get_valve_side_bus_name();
        void test_set_get_name();
        void test_set_get_power_percent();
        void test_set_get_dc_voltage();
        void test_set_get_dc_current();
        void test_set_get_transformer_grid_side_base_voltage();
        void test_set_get_transformer_valve_side_base_voltage();
        void test_set_get_transformer_impedance();
        void test_set_get_transformer_admittance();
        void test_set_get_transformer_max_tap();
        void test_set_get_transformer_min_tap();
        void test_set_get_transformer_number_of_taps();
        void test_set_get_transformer_tap();
        void test_clear();
        void test_copy_with_operator_equal();
        void test_get_bus_voltage();
        void test_get_bus_complex_voltage();
        void test_get_bus_base_voltage();
        void test_is_connected_to_bus();
        void test_is_in_area();
        void test_is_in_zone();
    private:
        CONVERTER converter;
};

#endif

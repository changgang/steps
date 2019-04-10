#ifndef LOAD_TEST_H
#define LOAD_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/device/load.h"
#include "header/power_system_database.h"

using namespace std;

class LOAD_TEST : public Test::Suite
{
    public:
        LOAD_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();
        void test_set_get_load_bus();
        void test_set_get_identifier();
        void test_set_get_status();
        void test_set_get_nominal_constant_power_load();
        void test_set_get_nominal_constant_current_load();
        void test_set_get_nominal_constant_impedance_load();
        void test_set_get_area_number();
        void test_set_get_zone_number();
        void test_set_get_owner_number();
        void test_set_get_flag_interruptable();
        void test_set_get_load_manually_scale();

        void test_is_valid();
        void test_clear();
        void test_copy_with_operator_equal();
        void test_is_connected_to_bus();

        void test_get_device_id();

        void test_get_nominal_total_load();
        void test_get_actual_total_load();
        void test_get_actual_constant_power_load();
        void test_get_actual_constant_current_load();
        void test_get_actual_constant_impedance_load();


        void test_set_get_load_model();
        void test_set_get_load_frequency_shedding_model();
        void test_set_get_load_voltage_shedding_model();
    private:
        LOAD load;
};

#endif

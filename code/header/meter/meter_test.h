#ifndef METER_TEST_H
#define METER_TEST_H

#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "thirdparty/cpptest/cpptest.h"

#include "header/meter/meter.h"
#include "header/power_system_database.h"

using namespace std;

class METER_TEST : public Test::Suite
{
    public:
        METER_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();
        void test_set_get_device_id_and_type();
        void test_set_get_buffer_size();
        void test_set_get_bus_meter_type();
        void test_set_get_line_meter_type();
        void test_set_get_transformer_meter_type();
        void test_set_get_generator_meter_type();
        void test_set_get_load_meter_type();
        void test_set_get_hvdc_meter_type();
        void test_set_get_wt_generator_meter_type();
        void test_set_get_equivalent_device_meter_type();
        void test_set_get_meter_internal_variable_index();
        void test_set_get_device_pointer();
        void test_clear();
        void test_is_valid();
        void test_copy_with_operator_equal();
        void test_equal();
        void test_get_bus_meter_value();
        void test_get_line_meter_value();
        void test_get_transformer_meter_value();
        void test_get_load_meter_value();
        void test_get_generator_meter_value();
        void test_get_wt_generator_meter_value();
        void test_get_hvdc_meter_value();
        void test_get_equivalent_device_meter_value();

    private:
        POWER_SYSTEM_DATABASE* db;
        METER* meter;
};

#endif

#ifndef WT_ELECTRICAL_MODEL_TEST_H
#define WT_ELECTRICAL_MODEL_TEST_H


#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "thirdparty/cpptest/cpptest.h"

#include "header/power_system_database.h"
using namespace std;

class EXCITER_MODEL_TEST : public Test::Suite
{
    public:
        WT_ELECTRICAL_MODEL_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        WT_GENERATOR* get_wt_generator();

        void test_get_model_type();
        void test_set_get_voltage_reference();
        void test_set_get_frequency_reference();
        void test_set_get_active_power_reference();
        void test_set_get_reactive_power_reference();
        void test_set_get_power_factor_reference();
        void test_set_get_turbine_speed_reference();

        void test_step_response();
        void test_get_standard_model_string();

        void run_step_response_of_exciter_model();
        void apply_voltage_drop_of_10_percent();

        virtual void test_get_model_name() = 0;
        virtual void test_set_get_parameters() = 0;
        virtual void test_initialize() = 0;
    private:
        void export_meter_title();
        void export_meter_values(double time);
        WT_GENERATOR* genptr;
        POWER_SYSTEM_DATABASE* db;
};

#endif//WT_ELECTRICAL_MODEL_TEST_H

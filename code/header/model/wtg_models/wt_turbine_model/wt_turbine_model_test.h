#ifndef WT_TURBINE_MODEL_TEST_H
#define WT_TURBINE_MODEL_TEST_H


#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "header/model/wtg_models/wtg_model_test.h"
#include "header/power_system_database.h"
using namespace std;

class WT_TURBINE_MODEL_TEST : public WTG_MODEL_TEST
{
    public:
        WT_TURBINE_MODEL_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();

    protected:
        void test_get_model_type();
        void test_set_get_damping();
        void test_get_standard_model_string();
        void test_step_response_of_wt_turbine_model_with_pitch_angle_increase();
        void test_step_response_of_wt_turbine_model_with_generator_power_order_drop();

    protected:
        virtual void test_get_model_name() = 0;
        virtual void test_set_get_parameters() = 0;
        virtual void test_initialize() = 0;
    private:
        void run_step_response_of_wt_turbine_model_with_pitch_angle_increase();
        void apply_1deg_pitch_angle_increase();
        void run_step_response_of_wt_turbine_model_with_generator_power_order_drop();
        void apply_10_percent_power_order_drop();
        void export_meter_title();
        void export_meter_values();

        POWER_SYSTEM_DATABASE* db;
};

#endif//WT_TURBINE_MODEL_TEST_H

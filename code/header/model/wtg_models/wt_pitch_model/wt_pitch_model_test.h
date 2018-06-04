#ifndef WT_PITCH_MODEL_TEST_H
#define WT_PITCH_MODEL_TEST_H


#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "header/model/wtg_models/wtg_model_test.h"
#include "header/power_system_database.h"
using namespace std;

class WT_PITCH_MODEL_TEST : public WTG_MODEL_TEST
{
    public:
        WT_PITCH_MODEL_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        void test_get_model_type();
        void test_get_wt_generator_speed();
        void test_get_wt_generator_reference_speed();
        void test_get_bus_frequency();
        void test_get_initial_pitch_angle_in_deg_from_wt_aerodynamic_model();

        void test_set_get_frequency_upper_deadband();
        void test_set_get_frequency_lower_deadband();

        void test_get_standard_model_string();

        void test_step_response_with_active_power_order_drop();
        void test_step_response_with_frequency_drop();


        virtual void test_get_model_name() = 0;
        virtual void test_set_get_parameters() = 0;
        virtual void test_initialize() = 0;
    private:
        void apply_active_power_order_drop_of_5_percent();
        void apply_frequency_drop_of_5_percent();

        void initialize_models();
        void update_models();
        void run_to_time(double tend);

        void export_meter_title();
        void export_meter_values();
};

#endif//WT_PITCH_MODEL_TEST_H

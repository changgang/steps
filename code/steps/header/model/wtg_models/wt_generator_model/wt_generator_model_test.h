#ifndef WT_GENERATOR_MODEL_TEST_H
#define WT_GENERATOR_MODEL_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "header/model/wtg_models/wtg_model_test.h"

#include "header/model/wtg_models/wt_generator_model/wt3g0.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"

using namespace std;

class WT_GENERATOR_MODEL_TEST : public WTG_MODEL_TEST
{
    public:
        WT_GENERATOR_MODEL_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        void test_get_mbase_in_MVA();
        void test_get_terminal_complex_voltage_in_pu();
        void test_get_number_of_lumped_wt_generators();
        void test_get_rated_power_per_wt_generator_in_MW();
        void test_get_nominal_frequency_in_Hz();
        void test_get_source_impedance_in_pu_based_on_mbase();

        void test_get_model_type();
        void test_set_get_initial_active_current_command();
        void test_set_get_initial_reactive_current_command();
        void test_get_standard_psse_string();
        void test_current_source_flag();

        void test_active_current_step_response_of_wt_generator_model();
        void test_reactive_voltage_step_response_of_wt_generator_model();
        void test_reactive_current_step_response_of_wt_generator_model();
        void test_bus_magnitude_step_response_of_wt_generator_model();
        void test_bus_angle_step_response_of_wt_generator_model();
        void test_variable_step_simulation_with_active_current_step_response();

        void run_a_step();
        void export_meter_title();
        void export_meter_values(double time);

        virtual void test_get_model_name() = 0;
        virtual void test_set_get_parameters() = 0;
        virtual void test_initialize_and_get_initialized_inputs() = 0;
};

#endif//WT_GENERATOR_MODEL_TEST_H

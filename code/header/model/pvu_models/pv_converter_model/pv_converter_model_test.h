#ifndef PV_CONVERTER_MODEL_TEST_H
#define PV_CONVERTER_MODEL_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "header/model/pvu_models/pvu_model_test.h"

#include "header/power_system_database.h"

using namespace std;

class PV_CONVERTER_MODEL_TEST : public PVU_MODEL_TEST
{
    public:
        PV_CONVERTER_MODEL_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        void test_get_mbase_in_MVA();
        void test_get_terminal_complex_voltage_in_pu();
        void test_get_number_of_lumped_pv_units();
        void test_get_rated_power_per_pv_unit_in_MW();
        void test_get_nominal_frequency_in_Hz();
        void test_get_source_impedance_in_pu_based_on_mbase();

        void test_get_model_type();
        void test_set_get_initial_active_current_command();
        void test_set_get_initial_reactive_current_command();
        void test_get_standard_model_string();

        void test_active_current_step_response_of_pv_converter_model();
        void test_reactive_current_step_response_of_pv_converter_model();
        void test_bus_magnitude_step_response_of_pv_converter_model();
        void test_bus_angle_step_response_of_pv_converter_model();
        void test_variable_step_simulation_with_active_current_step_response();

        void run_a_step();
        void export_meter_title();
        void export_meter_values(double time);

        virtual void test_get_model_name() = 0;
        virtual void test_set_get_parameters() = 0;
        virtual void test_initialize_and_get_initialized_inputs() = 0;
};

#endif//PV_CONVERTER_MODEL_TEST_H

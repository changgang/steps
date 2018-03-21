#ifndef PE_SOURCE_MODEL_TEST_H
#define PE_SOURCE_MODEL_TEST_H


#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "thirdparty/cpptest/cpptest.h"

#include "header/model/source_model/pe_source_model/wt3g2.h"
#include "header/power_system_database.h"

using namespace std;

class PE_SOURCE_MODEL_TEST : public Test::Suite
{
    public:
        PE_SOURCE_MODEL_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        PE_SOURCE* get_pe_source();

        void test_get_model_type();
        void test_update_source_impedance();
        void test_set_get_initial_active_current_command();
        void test_set_get_initial_reactive_current_command();
        void test_set_get_initial_reactive_voltage_command();
        void test_active_current_step_response_of_pe_source_model();
        void test_reactive_current_step_response_of_pe_source_model();
        void test_reactive_voltage_step_response_of_pe_source_model();
        void test_bus_magnitude_step_response_of_pe_source_model();
        void test_bus_angle_step_response_of_pe_source_model();
        void test_variable_step_simulation_with_active_current_step_response();
        void test_get_standard_model_string();

        void run_a_step();
        void show_simulation_title();
        void show_simulation_result(double time);

        virtual void test_get_model_name() = 0;
        virtual void test_set_get_parameters() = 0;
        virtual void test_initialize_and_get_initialized_inputs() = 0;
    private:
        PE_SOURCE* pesource_ptr;
        POWER_SYSTEM_DATABASE* db;
};

#endif//PE_SOURCE_MODEL_TEST_H

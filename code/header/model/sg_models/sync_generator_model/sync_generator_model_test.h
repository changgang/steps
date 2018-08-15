#ifndef SYNC_GENERATOR_MODEL_TEST_H
#define SYNC_GENERATOR_MODEL_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "header/model/sg_models/sg_model_test.h"

#include "header/model/sg_models/sync_generator_model/gencls.h"
#include "header/power_system_database.h"

using namespace std;

class SYNC_GENERATOR_MODEL_TEST : public SG_MODEL_TEST
{
    public:
        SYNC_GENERATOR_MODEL_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        void test_get_model_type();
        void test_set_get_parameters();
        void test_is_saturation_considered();
        void test_set_get_initial_excitation_voltage();
        void test_set_get_initial_mechanical_power();
        void test_pmech_step_response_of_sync_generator_model();
        void test_efd_step_response_of_sync_generator_model();
        void test_bus_step_response_of_sync_generator_model();
        void test_variable_step_simulation_with_pmech_step_response();
        void test_get_standard_model_string();

        void run_a_step();

        virtual void test_get_model_name() = 0;
        virtual void test_update_source_impedance() = 0;

    private:
        void export_meter_title();
        void export_meter_values(double time);
};

#endif//SYNC_GENERATOR_MODEL_TEST_H

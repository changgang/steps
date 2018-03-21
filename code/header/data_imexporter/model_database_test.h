#ifndef MODEL_DATABASE_TEST_H
#define MODEL_DATABASE_TEST_H


#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest-1.1.2/src/cpptest.h"

#include "header/model_database.h"

using namespace std;

class MODEL_DATABASE_TEST : public Test::Suite
{
    public:
        MODEL_DATABASE_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();
        void test_append_and_get_model();
        void test_get_model_table_and_valid_model_count();
        void test_clear_model_database();
        void test_delete_model_of_index();
        void test_all_models_initialize_flag();
        void test_all_models_update_flag();

        void test_append_sync_generator_model();
        void test_append_exciter_model();
        void test_append_stabilizer_model();
        void test_append_turbine_governor_model();
        void test_append_wind_turbine_generator_model();
        void test_append_wind_electrical_model();
        void test_append_wind_mechanical_model();
        void test_append_wind_pitch_control_model();
        void test_append_load_model();
        void test_append_load_frequency_relay_model();
        void test_append_load_voltage_relay_model();
        void test_append_hvdc_model();
        void test_append_line_relay_model();
    private:
        POWER_SYSTEM_DATABASE* db;
};

#endif//MODEL_DATABASE_TEST_H

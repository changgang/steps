#ifndef ENERGY_STORAGE_MODEL_TEST_H
#define ENERGY_STORAGE_MODEL_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "thirdparty/cpptest/cpptest.h"

#include "header/power_system_database.h"

using namespace std;

class ENERGY_STORAGE_MODEL_TEST : public Test::Suite
{
    public:
        ENERGY_STORAGE_MODEL_TEST();
        ENERGY_STORAGE* get_test_energy_storage();
        ENERGY_STORAGE_MODEL* get_test_energy_storage_model();
    protected:
        virtual void setup();
        virtual void tear_down();
    public:
        void test_get_model_type();
        void test_set_get_max_power();
        void test_set_get_inital_energy();
        void test_set_get_nominal_energy();
        void test_set_get_initial_Pref();
        void test_set_get_initial_Vref();
        void test_frequency_step_response();
        void test_voltage_step_response();
        void test_get_standard_model_string();

        void export_meter_title();
        void export_meter_values();
        void run_a_step();

        virtual void test_get_model_name() = 0;
        virtual void test_set_get_parameters() = 0;
    private:
};

#endif//ENERGY_STORAGE_MODEL_TEST_H

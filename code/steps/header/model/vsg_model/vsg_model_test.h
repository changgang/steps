#ifndef VSG_MODEL_TEST_H
#define VSG_MODEL_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/power_system_database.h"
#include "header/STEPS.h"
#include "header/model/vsg_model/vsg_model.h"
#include "header/model/wtg_models/wt_generator_model/wt3g0.h"
using namespace std;

class VSG_MODEL_TEST : public Test::Suite
{
    public:
        VSG_MODEL_TEST();

        WT_GENERATOR* get_test_wt_generator();
        VSG_MODEL* get_test_model();
        void set_test_model(VSG_MODEL* model);


        void test_get_mbase_in_MVA();
        void test_get_one_over_mbase_in_one_over_MVA();

        void test_get_terminal_complex_power_in_pu_based_on_mbase();
        void test_get_terminal_voltage_in_pu();
        void test_get_terminal_complex_voltage_in_pu();
        void test_get_terminal_voltage_angle_in_rad();
        void test_get_bus_base_frequency_in_Hz();
        void test_get_bus_base_angle_speed_in_radps();
        void test_get_source_impedance_in_pu_based_on_mbase();

        void test_set_get_Pref_in_pu_based_on_mbase();
        void test_set_get_Qref_in_pu_based_on_mbase();
        void test_set_get_Vref_in_pu();

        void test_get_virtual_speed_in_pu();

        void test_active_power_step_response_of_vsg_model();
        void test_reactive_power_step_response_of_vsg_model();
        void test_voltage_step_response_of_vsg_model();

        void run_a_step();
        void export_meter_title();
        void export_meter_values(double time);
    protected:
        virtual setup();
        virtual tear_down();
    private:
        VSG_MODEL* model;
        WT3G0* wtg_model;
};

#endif//VSG_MODEL_TEST_H

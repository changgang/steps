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
#include "header/model/converter_common_models/converter_model/vs_converter_model/vsg_model/vsg_model.h"
using namespace std;

class VSG_MODEL_TEST : public Test::Suite
{
    public:
        VSG_MODEL_TEST();

        VSG_MODEL* get_test_model();
        void set_test_model(VSG_MODEL* model);

        void test_set_get_Pref_in_pu_based_on_mbase();
        void test_set_get_Qref_in_pu_based_on_mbase();
        void test_set_get_Vref_in_pu();

        void test_set_get_P_in_pu_based_on_mbase();
        void test_set_get_Q_in_pu_based_on_mbase();
        void test_set_get_V_in_pu();
        void test_set_get_initial_virtual_voltage_in_pu();
        void test_set_get_initial_virtual_angle_in_rad();

        void test_active_power_step_response_of_vsg_model();
        void test_reactive_power_step_response_of_vsg_model();
        void test_voltage_step_response_of_vsg_model();

        void run_a_step();
        void export_meter_title();
        void export_meter_values(double time);
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        VSG_MODEL* model;
};

#endif//VSG_MODEL_TEST_H

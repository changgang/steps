#ifndef TURBINE_GOVERNOR_TEST_H
#define TURBINE_GOVERNOR_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "header/model/sg_models/sg_model_test.h"
#include "header/power_system_database.h"
using namespace std;

class TURBINE_GOVERNOR_MODEL_TEST : public SG_MODEL_TEST
{
    public:
        TURBINE_GOVERNOR_MODEL_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        void apply_speed_drop_of_1_percent();
        void run_step_response_of_turbine_govnernor_model();

        virtual void test_get_model_type() = 0;
        virtual void test_get_model_name() = 0;
        virtual void test_set_get_parameters() = 0;
        void test_get_rotor_speed();
        virtual void test_initialize() = 0;
        virtual void test_set_get_mechanical_power_reference() = 0;
        virtual void test_set_get_mechanical_power_upper_limit() = 0;
        virtual void test_set_get_mechanical_power_lower_limit() = 0;
        virtual void test_step_response() = 0;
        virtual void test_get_standard_psse_string() = 0;
    private:
        void export_meter_title();
        void export_meter_values(double time);
};

#endif//TURBINE_GOVERNOR_TEST_H

#ifndef EXCITER_MODEL_TEST_H
#define EXCITER_MODEL_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "header/model/sg_models/sg_model_test.h"

#include "header/power_system_database.h"
#include "header/STEPS.h"
using namespace std;

class EXCITER_MODEL_TEST : public SG_MODEL_TEST
{
    public:
        EXCITER_MODEL_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        void test_get_model_type();
        void test_set_get_voltage_reference();
        void test_get_compensated_voltage();
        void test_get_field_current();
        void test_step_response();
        void test_get_standard_psse_string();

        void run_step_response_of_exciter_model();
        void apply_voltage_drop_of_10_percent();

        virtual void test_get_model_name() = 0;
        virtual void test_set_get_parameters() = 0;
        virtual void test_initialize() = 0;
    private:
        void export_meter_title();
        void export_meter_values(double time);
};

#endif//EXCITER_MODEL_TEST_H

#ifndef HVDC_MODEL_TEST_H
#define HVDC_MODEL_TEST_H

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
using namespace std;

class HVDC_MODEL_TEST : public Test::Suite
{
    public:
        HVDC_MODEL_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        HVDC* get_test_hvdc();
        HVDC_MODEL* get_test_hvdc_model();
        void test_initialize();
        void test_rectifier_voltage_ramp_response();
        void test_inverter_voltage_ramp_response();
        void test_manual_block_and_unblock();
        void test_manual_bypass_and_unbypass();
        void test_get_standard_psse_string();

        void test_get_auxiliary_signal();

        virtual void test_get_model_type() = 0;
        virtual void test_get_model_name() = 0;
        virtual void test_set_get_parameters() = 0;
    private:
        void export_meter_titles();
        void export_meter_values(double time);
};

#endif//HVDC_MODEL_TEST_H

#ifndef LOAD_MODEL_TEST_H
#define LOAD_MODEL_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "thirdparty/cpptest/cpptest.h"

#include "header/model/load_model/IEEL.h"
#include "header/power_system_database.h"
using namespace std;

class LOAD_MODEL_TEST : public Test::Suite
{
    public:
        LOAD_MODEL_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        LOAD* get_load();
        void test_get_model_type();
        void test_get_bus_voltage();
        void test_get_bus_frequency_deviation();
        void test_set_get_subsystem_type();

        void test_initialize();
        void test_run_voltage_ramp_response();
        void test_run_frequency_ramp_response();

        virtual void test_get_model_name() = 0;
        virtual void test_get_detailed_model_name() = 0;
        virtual void test_set_get_parameters() = 0;

    private:
        void export_meter_title();
        void export_meter_values(double time);

        LOAD* load_ptr;
};

#endif//LOAD_TEST_H

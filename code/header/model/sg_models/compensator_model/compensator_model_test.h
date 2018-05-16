#ifndef COMPENSATOR_MODEL_TEST_H
#define COMPENSATOR_MODEL_TEST_H


#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "header/model/sg_models/sg_model_test.h"

#include "header/model/sg_models/compensator_model/COMP.h"
#include "header/power_system_database.h"
using namespace std;

class COMPENSATOR_MODEL_TEST : public SG_MODEL_TEST
{
    public:
        COMPENSATOR_MODEL_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        void apply_voltage_drop_of_10_percent();
        void run_step_response_of_compensator_model();

        virtual void test_get_model_type() = 0;
        virtual void test_get_model_name() = 0;
        virtual void test_set_get_parameters() = 0;
        void test_get_generator_terminal_voltage();
        void test_get_generator_terminal_current();
        virtual void test_initialize() = 0;
        virtual void test_step_response() = 0;
        virtual void test_get_standard_model_string() = 0;
    private:
        void export_meter_title();
        void export_meter_values(double time);
};

#endif//COMPENSATOR_MODEL_TEST_H

#ifndef AUXILIARY_SIGNAL_MODEL_TEST_H
#define AUXILIARY_SIGNAL_MODEL_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/power_system_database.h"
using namespace std;

class AUXILIARY_SIGNAL_MODEL_TEST : public Test::Suite
{
    public:
        AUXILIARY_SIGNAL_MODEL_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        LCC_HVDC2T* get_2t_lcc_hvdc();
        void apply_voltage_drop_of_10_percent();
        void run_step_response_of_exciter_model(AUXILIARY_SIGNAL_MODEL* model);

        virtual void test_get_model_type() = 0;
        virtual void test_get_model_name() = 0;
        virtual void test_set_get_parameters() = 0;
        void test_get_compensated_voltage();
        virtual void test_initialize() = 0;
        virtual void test_set_get_voltage_reference() = 0;
        virtual void test_step_response() = 0;
        virtual void test_get_standard_psse_string() = 0;
    private:
        LCC_HVDC2T* hvdcptr;
};

#endif//AUXILIARY_SIGNAL_MODEL_TEST_H

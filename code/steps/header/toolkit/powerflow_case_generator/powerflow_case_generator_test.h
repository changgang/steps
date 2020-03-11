#ifndef POWERFLOW_CASE_GENERATOR_TEST_H
#define POWERFLOW_CASE_GENERATOR_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/toolkit/powerflow_case_generator/powerflow_case_generator.h"
#include "header/STEPS.h"

using namespace std;

class POWERFLOW_CASE_GENERATOR_TEST : public Test::Suite
{
    public:
        POWERFLOW_CASE_GENERATOR_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();
        void test_set_and_get_power_system_database_maximum_bus_number();

        void test_set_get_generator_title();
        void test_set_get_base_powerflow_data_filename();
        void test_set_get_uniform_load_scale();
        void test_set_get_random_load_scale();
        void test_set_get_maximum_case_count_to_generate();
        void test_generate_load_scale_randoms();
        void test_generate_cases();
    private:
        POWERFLOW_CASE_GENERATOR* generator;
};

#endif//POWERFLOW_CASE_GENERATOR_TEST_H

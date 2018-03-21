#include "header/toolkit/powerflow_case_generator/powerflow_case_generator_test.h"
#include "header/basic/utility.h"
#include "header/data_imexporter/psse_imexporter.h"
#include "header/prepare_for_tests/prepare_models_for_test.h"
#include "header/meter/meter_setter.h"
#include "header/steps_namespace.h"
#include "header/toolkit/powerflow_solver/powerflow_solver.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cstdio>
#include <cmath>

using namespace std;

POWERFLOW_CASE_GENERATOR_TEST::POWERFLOW_CASE_GENERATOR_TEST()
{
    TEST_ADD(POWERFLOW_CASE_GENERATOR_TEST::test_constructor);
    TEST_ADD(POWERFLOW_CASE_GENERATOR_TEST::test_set_and_get_power_system_database_maximum_bus_number);

    TEST_ADD(POWERFLOW_CASE_GENERATOR_TEST::test_set_get_generator_title);
    TEST_ADD(POWERFLOW_CASE_GENERATOR_TEST::test_set_get_base_powerflow_data_filename);
    TEST_ADD(POWERFLOW_CASE_GENERATOR_TEST::test_set_get_uniform_load_scale);
    TEST_ADD(POWERFLOW_CASE_GENERATOR_TEST::test_set_get_random_load_scale);
    TEST_ADD(POWERFLOW_CASE_GENERATOR_TEST::test_set_get_maximum_case_count_to_generate);
    TEST_ADD(POWERFLOW_CASE_GENERATOR_TEST::test_generate_load_scale_randoms);
    TEST_ADD(POWERFLOW_CASE_GENERATOR_TEST::test_generate_cases);
}

void POWERFLOW_CASE_GENERATOR_TEST::setup()
{
    generator = new POWERFLOW_CASE_GENERATOR();

}

void POWERFLOW_CASE_GENERATOR_TEST::tear_down()
{
    delete generator;

    show_test_end_information();
}

void POWERFLOW_CASE_GENERATOR_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWERFLOW_CASE_GENERATOR_TEST");

    TEST_ASSERT(generator->get_power_system_database_maximum_bus_number()==10000);
    TEST_ASSERT(generator->get_generator_title()=="MODE GENERATOR");
    TEST_ASSERT(generator->get_base_powerflow_data_filename()=="");
    TEST_ASSERT(fabs(generator->get_uniform_load_scale()-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(generator->get_random_load_scale()-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(generator->get_maximum_case_count_to_generate()==100);
}

void POWERFLOW_CASE_GENERATOR_TEST::test_set_and_get_power_system_database_maximum_bus_number()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWERFLOW_CASE_GENERATOR_TEST");

    TEST_ASSERT(generator->get_power_system_database_maximum_bus_number()==10000);

    generator->set_power_system_database_maximum_bus_number(1000);

    TEST_ASSERT(generator->get_power_system_database_maximum_bus_number()==1000);
}


void POWERFLOW_CASE_GENERATOR_TEST::test_set_get_generator_title()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWERFLOW_CASE_GENERATOR_TEST");

    generator->set_generator_title("OPERATION MODE GENERATOR");

    TEST_ASSERT(generator->get_generator_title()=="OPERATION MODE GENERATOR");
}

void POWERFLOW_CASE_GENERATOR_TEST::test_set_get_base_powerflow_data_filename()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWERFLOW_CASE_GENERATOR_TEST");

    generator->set_base_powerflow_data_filename("abc.raw");

    TEST_ASSERT(generator->get_base_powerflow_data_filename()=="abc.raw");
}

void POWERFLOW_CASE_GENERATOR_TEST::test_set_get_uniform_load_scale()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWERFLOW_CASE_GENERATOR_TEST");

    generator->set_uniform_load_scale(0.1);

    TEST_ASSERT(fabs(generator->get_uniform_load_scale()-0.1)<FLOAT_EPSILON);
}

void POWERFLOW_CASE_GENERATOR_TEST::test_set_get_random_load_scale()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWERFLOW_CASE_GENERATOR_TEST");

    generator->set_random_load_scale(0.1);

    TEST_ASSERT(fabs(generator->get_random_load_scale()-0.1)<FLOAT_EPSILON);
}

void POWERFLOW_CASE_GENERATOR_TEST::test_set_get_maximum_case_count_to_generate()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWERFLOW_CASE_GENERATOR_TEST");

    generator->set_maximum_case_count_to_generate(20);

    TEST_ASSERT(generator->get_maximum_case_count_to_generate()==20);
}

void POWERFLOW_CASE_GENERATOR_TEST::test_generate_load_scale_randoms()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWERFLOW_CASE_GENERATOR_TEST");

    ostringstream sstream;

    generator->set_power_system_database_maximum_bus_number(100);
    generator->set_uniform_load_scale(0.2);
    generator->set_random_load_scale(0.05);
    generator->set_base_powerflow_data_filename("ieee9.raw");

    vector< vector<double> > cases = generator->generate_load_scale_randoms();
    for(size_t i=0; i<cases.size(); ++i)
    {
        vector<double> rands = cases[i];
        sstream<<"randoms of case "<<i<<": ";
        for(size_t j=0; j<rands.size(); ++j)
            sstream<<rands[j]<<", ";
        sstream<<endl;
    }
    show_information_with_leading_time_stamp(sstream);
}

void POWERFLOW_CASE_GENERATOR_TEST::test_generate_cases()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWERFLOW_CASE_GENERATOR_TEST");

    generator->set_power_system_database_maximum_bus_number(1000);
    generator->set_uniform_load_scale(0.05);
    generator->set_random_load_scale(0.01);
    generator->set_base_powerflow_data_filename("bench_shandong.raw");
    generator->set_maximum_case_count_to_generate(10);

    generator->generate_cases();
}


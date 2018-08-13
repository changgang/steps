#include "header/model/pvu_models/pv_irradiance_model/fileirrd_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cstdio>
#include <cmath>

using namespace std;

FILEIRRAD_TEST::FILEIRRAD_TEST() : PV_IRRADIANCE_MODEL_TEST()
{
    TEST_ADD(FILEIRRAD_TEST::test_get_model_name);
    TEST_ADD(FILEIRRAD_TEST::test_set_get_parameters);
    TEST_ADD(FILEIRRAD_TEST::test_initialize);
    TEST_ADD(FILEIRRAD_TEST::test_get_solar_irradiance);
    TEST_ADD(FILEIRRAD_TEST::test_get_solar_irradiance_direction);
}

void FILEIRRAD_TEST::setup()
{
    PV_IRRADIANCE_MODEL_TEST::setup();
}

void FILEIRRAD_TEST::tear_down()
{
    PV_IRRADIANCE_MODEL_TEST::tear_down();

    show_test_end_information();
}

void FILEIRRAD_TEST::prepare_solar_irradiance_file(string file)
{
}

void FILEIRRAD_TEST::test_get_model_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"FILEIRRAD_TEST");
}

void FILEIRRAD_TEST::test_set_get_parameters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"FILEIRRAD_TEST");
}

void FILEIRRAD_TEST::test_initialize()
{
    show_test_information_for_function_of_class(__FUNCTION__,"FILEIRRAD_TEST");
}


void FILEIRRAD_TEST::test_get_solar_irradiance()
{
    show_test_information_for_function_of_class(__FUNCTION__,"FILEIRRAD_TEST");
}

void FILEIRRAD_TEST::test_get_solar_irradiance_direction()
{
    show_test_information_for_function_of_class(__FUNCTION__,"FILEIRRAD_TEST");
}

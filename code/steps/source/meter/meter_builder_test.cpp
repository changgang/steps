#include "header/basic/test_macro.h"
#include "header/meter/meter_builder_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/prepare_for_tests/prepare_models_for_test.h"
#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <cstdio>
#include <cmath>


#ifdef ENABLE_STEPS_TEST
using namespace std;

METER_BUILDER_TEST::METER_BUILDER_TEST() : setter(default_toolkit)
{
    TEST_ADD(METER_BUILDER_TEST::test_constructor);
    TEST_ADD(METER_BUILDER_TEST::test_prepare_bus_related_meters);
    TEST_ADD(METER_BUILDER_TEST::test_prepare_ac_line_related_meters);
    TEST_ADD(METER_BUILDER_TEST::test_prepare_transformer_related_meters);
    TEST_ADD(METER_BUILDER_TEST::test_prepare_load_related_meters);
    TEST_ADD(METER_BUILDER_TEST::test_prepare_generator_related_meters);
    TEST_ADD(METER_BUILDER_TEST::test_prepare_wt_generator_related_meters);
    TEST_ADD(METER_BUILDER_TEST::test_prepare_pv_unit_related_meters);
    TEST_ADD(METER_BUILDER_TEST::test_prepare_energy_storage_related_meters);
    TEST_ADD(METER_BUILDER_TEST::test_prepare_lcc_hvdc2t_related_meters);
    TEST_ADD(METER_BUILDER_TEST::test_prepare_vsc_hvdc_related_meters);
    TEST_ADD(METER_BUILDER_TEST::test_prepare_equivalent_device_related_meters);
}

void METER_BUILDER_TEST::setup()
{
    ;
}

void METER_BUILDER_TEST::tear_down()
{
    default_toolkit.clear();
    show_test_end_information();
}

void METER_BUILDER_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_BUILDER_TEST");
}

void METER_BUILDER_TEST::test_prepare_bus_complete_meters()
{
}

void METER_BUILDER_TEST::test_prepare_ac_line_complete_meters()
{
}

void METER_BUILDER_TEST::test_prepare_transformer_complete_meters()
{
}

void METER_BUILDER_TEST::test_prepare_load_complete_meters()
{
}

void METER_BUILDER_TEST::test_prepare_generator_complete_meters()
{
}

void METER_BUILDER_TEST::test_prepare_wt_generator_complete_meters()
{
}

void METER_BUILDER_TEST::test_prepare_pv_unit_complete_meters()
{
}

void METER_BUILDER_TEST::test_prepare_energy_storage_complete_meters()
{
}

void METER_BUILDER_TEST::test_prepare_lcc_hvdc2t_complete_meters()
{
}

void METER_BUILDER_TEST::test_prepare_vsc_hvdc_complete_meters()
{
}

void METER_BUILDER_TEST::test_prepare_equivalent_device_complete_meters()
{
}


#endif

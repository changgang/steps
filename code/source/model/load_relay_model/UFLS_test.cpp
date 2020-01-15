#include "header/basic/test_macro.h"
#include "header/model/load_relay_model/UFLS_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <cstdio>
#include <cmath>

#ifdef ENABLE_STEPS_TEST
using namespace std;
UFLS_TEST::UFLS_TEST() : LOAD_FREQUENCY_RELAY_MODEL_TEST()
{
    TEST_ADD(UFLS_TEST::test_model_name);
    TEST_ADD(UFLS_TEST::test_set_get_parameters);
}



void UFLS_TEST::setup()
{
    LOAD_FREQUENCY_RELAY_MODEL_TEST::setup();

    LOAD* load = get_load();

    UFLS model;
    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();
    model.set_toolkit(default_toolkit);
    model.set_device_id(load->get_device_id());

    size_t stage = 0;
    model.set_frequency_threshold_in_Hz_of_stage(stage, 49.0);
    model.set_time_delay_in_s_of_stage(stage, 0.2);
    model.set_scale_in_pu_of_stage(stage, 0.05);

	++stage;
    model.set_frequency_threshold_in_Hz_of_stage(stage, 48.5);
    model.set_time_delay_in_s_of_stage(stage, 0.3);
    model.set_scale_in_pu_of_stage(stage, 0.06);

	++stage;
    model.set_frequency_threshold_in_Hz_of_stage(stage, 48.0);
    model.set_time_delay_in_s_of_stage(stage, 0.15);
    model.set_scale_in_pu_of_stage(stage, 0.03);

	++stage;
    model.set_frequency_threshold_in_Hz_of_stage(stage, 49.5);
    model.set_time_delay_in_s_of_stage(stage, 3.0);
    model.set_scale_in_pu_of_stage(stage, 0.02);

    model.set_breaker_time_in_s(0.05);
    model.set_frequency_sensor_time_in_s(0.1);

    dmdb.add_model(&model);
}

void UFLS_TEST::tear_down()
{
    LOAD_FREQUENCY_RELAY_MODEL_TEST::tear_down();

    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();
    dmdb.remove_the_last_model();
}

void UFLS_TEST::test_model_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"UFLS_TEST");

    TEST_ASSERT(get_load()->get_load_frequency_relay_model()->get_model_name()=="UFLS");
}

void UFLS_TEST::test_set_get_parameters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"UFLS_TEST");

    UFLS* model = (UFLS*) get_load()->get_load_frequency_relay_model();

    size_t stage = 0;

    TEST_ASSERT(fabs(model->get_frequency_threshold_in_Hz_of_stage(stage)-49.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_time_delay_in_s_of_stage(stage)-0.2)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_scale_in_pu_of_stage(stage)-0.05)<FLOAT_EPSILON);

	++stage;
    TEST_ASSERT(fabs(model->get_frequency_threshold_in_Hz_of_stage(stage)-48.5)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_time_delay_in_s_of_stage(stage)-0.3)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_scale_in_pu_of_stage(stage)-0.06)<FLOAT_EPSILON);

	++stage;
    TEST_ASSERT(fabs(model->get_frequency_threshold_in_Hz_of_stage(stage)-48.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_time_delay_in_s_of_stage(stage)-0.15)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_scale_in_pu_of_stage(stage)-0.03)<FLOAT_EPSILON);

	++stage;
    TEST_ASSERT(fabs(model->get_frequency_threshold_in_Hz_of_stage(stage)-49.5)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_time_delay_in_s_of_stage(stage)-3.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_scale_in_pu_of_stage(stage)-0.02)<FLOAT_EPSILON);

    TEST_ASSERT(fabs(model->get_breaker_time_in_s()-0.05)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_frequency_sensor_time_in_s()-0.1)<FLOAT_EPSILON);
}


#endif

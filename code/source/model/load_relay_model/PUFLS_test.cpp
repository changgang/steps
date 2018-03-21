#include "header/model/load_relay_model/PUFLS_test.h"
#include "header/basic/utility.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cstdio>
#include <cmath>

using namespace std;
PUFLS_TEST::PUFLS_TEST() : LOAD_FREQUENCY_RELAY_MODEL_TEST()
{
    TEST_ADD(PUFLS_TEST::test_model_name);
    TEST_ADD(PUFLS_TEST::test_set_get_parameters);
    TEST_ADD(PUFLS_TEST::test_continuous_load_shedding_scheme_with_realtime_frequency_additional_stage);
    TEST_ADD(PUFLS_TEST::test_continuous_load_shedding_scheme_with_minimum_frequency_additional_stage);
    TEST_ADD(PUFLS_TEST::test_discrete_load_shedding_scheme_with_realtime_frequency_additional_stage);
    TEST_ADD(PUFLS_TEST::test_discrete_load_shedding_scheme_with_minimum_frequency_additional_stage);
    TEST_ADD(PUFLS_TEST::test_composite_load_shedding_scheme_with_realtime_frequency_additional_stage);
    TEST_ADD(PUFLS_TEST::test_composite_load_shedding_scheme_with_minimum_frequency_additional_stage);
}



void PUFLS_TEST::setup()
{
    LOAD_FREQUENCY_RELAY_MODEL_TEST::setup();

    LOAD* load = get_load();

    PUFLS model;

    model.set_frequency_sensor_time_in_s(0.0);
    model.set_frequency_threshold_in_Hz(49.5);
    model.set_time_delay_in_s(0.2);
    model.set_scale_K_in_pu_per_Hz(0.2);
    model.set_maximum_continuous_shed_scale_in_pu(0.5);
    model.set_additional_stage_trigger_signal(MINIMUM_FREQUENCY);
    model.set_additional_stage_shed_scale_in_pu(0.1);
    model.set_additional_stage_time_delay_in_s(5.0);

    model.set_discrete_stage_time_delay_in_s(0.1);
    size_t stage = 0;
    model.set_discrete_stage_shed_scale_in_pu(stage, 0.05); ++stage;
    model.set_discrete_stage_shed_scale_in_pu(stage, 0.05); ++stage;
    model.set_discrete_stage_shed_scale_in_pu(stage, 0.05); ++stage;
    model.set_discrete_stage_shed_scale_in_pu(stage, 0.05); ++stage;
    model.set_discrete_stage_shed_scale_in_pu(stage, 0.05); ++stage;
    model.set_discrete_stage_shed_scale_in_pu(stage, 0.05); ++stage;
    model.set_discrete_stage_shed_scale_in_pu(stage, 0.05); ++stage;
    model.set_discrete_stage_shed_scale_in_pu(stage, 0.05); ++stage;

    load->set_model(&model);
}

void PUFLS_TEST::tear_down()
{
    LOAD_FREQUENCY_RELAY_MODEL_TEST::tear_down();

    show_test_end_information();
}

void PUFLS_TEST::test_model_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PUFLS_TEST");

    TEST_ASSERT(get_load()->get_load_frequency_relay_model()->get_model_name()=="PUFLS");
}

void PUFLS_TEST::test_set_get_parameters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PUFLS_TEST");

    PUFLS* model = (PUFLS*) get_load()->get_load_frequency_relay_model();

    TEST_ASSERT(fabs(model->get_frequency_sensor_time_in_s()-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_frequency_threshold_in_Hz()-49.5)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_time_delay_in_s()-0.2)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_scale_K_in_pu_per_Hz()-0.2)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_maximum_continuous_shed_scale_in_pu()-0.5)<FLOAT_EPSILON);
    TEST_ASSERT(model->get_additional_stage_trigger_signal()==MINIMUM_FREQUENCY);
    TEST_ASSERT(fabs(model->get_additional_stage_shed_scale_in_pu()-0.1)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_additional_stage_time_delay_in_s()-5.0)<FLOAT_EPSILON);

    TEST_ASSERT(fabs(model->get_discrete_stage_time_delay_in_s()-0.1)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_discrete_stage_shed_scale_in_pu(0)-0.05)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_discrete_stage_shed_scale_in_pu(1)-0.05)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_discrete_stage_shed_scale_in_pu(2)-0.05)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_discrete_stage_shed_scale_in_pu(3)-0.05)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_discrete_stage_shed_scale_in_pu(4)-0.05)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_discrete_stage_shed_scale_in_pu(5)-0.05)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_discrete_stage_shed_scale_in_pu(6)-0.05)<FLOAT_EPSILON);
}


void PUFLS_TEST::test_continuous_load_shedding_scheme_with_realtime_frequency_additional_stage()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PUFLS_TEST");

    PUFLS* model = (PUFLS*) get_load()->get_load_frequency_relay_model();

    model->set_frequency_sensor_time_in_s(0.0);
    model->set_frequency_threshold_in_Hz(49.5);
    model->set_time_delay_in_s(0.2);
    model->set_scale_K_in_pu_per_Hz(0.2);
    model->set_maximum_continuous_shed_scale_in_pu(0.5);
    model->set_additional_stage_trigger_signal(REALTIME_FREQUENCY);
    model->set_additional_stage_shed_scale_in_pu(0.1);
    model->set_additional_stage_time_delay_in_s(5.0);

    model->set_discrete_stage_time_delay_in_s(0.1);

    for(size_t stage=0; stage!=MAX_LOAD_RELAY_STAGE; ++stage)
        model->set_discrete_stage_shed_scale_in_pu(stage, 0.);

    string outputfile = "test_log/frequency_ramp_response_of_"+model->get_model_name()+"_model_continuous_realtime_frequency.txt";
    LOAD_FREQUENCY_RELAY_MODEL_TEST::run_model(outputfile);
}

void PUFLS_TEST::test_continuous_load_shedding_scheme_with_minimum_frequency_additional_stage()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PUFLS_TEST");

    PUFLS* model = (PUFLS*) get_load()->get_load_frequency_relay_model();

    model->set_frequency_sensor_time_in_s(0.0);
    model->set_frequency_threshold_in_Hz(49.5);
    model->set_time_delay_in_s(0.2);
    model->set_scale_K_in_pu_per_Hz(0.2);
    model->set_maximum_continuous_shed_scale_in_pu(0.5);
    model->set_additional_stage_trigger_signal(MINIMUM_FREQUENCY);
    model->set_additional_stage_shed_scale_in_pu(0.1);
    model->set_additional_stage_time_delay_in_s(5.0);

    model->set_discrete_stage_time_delay_in_s(0.1);

    for(size_t stage=0; stage!=MAX_LOAD_RELAY_STAGE; ++stage)
        model->set_discrete_stage_shed_scale_in_pu(stage, 0.);

    string outputfile = "test_log/frequency_ramp_response_of_"+model->get_model_name()+"_model_continuous_minimum_frequency.txt";
    LOAD_FREQUENCY_RELAY_MODEL_TEST::run_model(outputfile);
}

void PUFLS_TEST::test_discrete_load_shedding_scheme_with_realtime_frequency_additional_stage()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PUFLS_TEST");

    PUFLS* model = (PUFLS*) get_load()->get_load_frequency_relay_model();

    model->set_frequency_sensor_time_in_s(0.0);
    model->set_frequency_threshold_in_Hz(49.5);
    model->set_time_delay_in_s(0.2);
    model->set_scale_K_in_pu_per_Hz(0.2);
    model->set_maximum_continuous_shed_scale_in_pu(0.0);
    model->set_additional_stage_trigger_signal(REALTIME_FREQUENCY);
    model->set_additional_stage_shed_scale_in_pu(0.1);
    model->set_additional_stage_time_delay_in_s(5.0);

    model->set_discrete_stage_time_delay_in_s(0.1);

    size_t stage = 0;
    model->set_discrete_stage_shed_scale_in_pu(stage, 0.02); ++stage;
    model->set_discrete_stage_shed_scale_in_pu(stage, 0.03); ++stage;
    model->set_discrete_stage_shed_scale_in_pu(stage, 0.04); ++stage;
    model->set_discrete_stage_shed_scale_in_pu(stage, 0.05); ++stage;
    model->set_discrete_stage_shed_scale_in_pu(stage, 0.06); ++stage;
    model->set_discrete_stage_shed_scale_in_pu(stage, 0.07); ++stage;
    model->set_discrete_stage_shed_scale_in_pu(stage, 0.08); ++stage;
    model->set_discrete_stage_shed_scale_in_pu(stage, 0.09); ++stage;

    string outputfile = "test_log/frequency_ramp_response_of_"+model->get_model_name()+"_model_discrete_realtime_frequency.txt";
    LOAD_FREQUENCY_RELAY_MODEL_TEST::run_model(outputfile);
}

void PUFLS_TEST::test_discrete_load_shedding_scheme_with_minimum_frequency_additional_stage()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PUFLS_TEST");

    PUFLS* model = (PUFLS*) get_load()->get_load_frequency_relay_model();

    model->set_frequency_sensor_time_in_s(0.0);
    model->set_frequency_threshold_in_Hz(49.5);
    model->set_time_delay_in_s(0.2);
    model->set_scale_K_in_pu_per_Hz(0.2);
    model->set_maximum_continuous_shed_scale_in_pu(0.0);
    model->set_additional_stage_trigger_signal(MINIMUM_FREQUENCY);
    model->set_additional_stage_shed_scale_in_pu(0.1);
    model->set_additional_stage_time_delay_in_s(5.0);

    model->set_discrete_stage_time_delay_in_s(0.1);

    size_t stage = 0;
    model->set_discrete_stage_shed_scale_in_pu(stage, 0.02); ++stage;
    model->set_discrete_stage_shed_scale_in_pu(stage, 0.03); ++stage;
    model->set_discrete_stage_shed_scale_in_pu(stage, 0.04); ++stage;
    model->set_discrete_stage_shed_scale_in_pu(stage, 0.05); ++stage;
    model->set_discrete_stage_shed_scale_in_pu(stage, 0.06); ++stage;
    model->set_discrete_stage_shed_scale_in_pu(stage, 0.07); ++stage;
    model->set_discrete_stage_shed_scale_in_pu(stage, 0.08); ++stage;
    model->set_discrete_stage_shed_scale_in_pu(stage, 0.09); ++stage;

    string outputfile = "test_log/frequency_ramp_response_of_"+model->get_model_name()+"_model_discrete_minimum_frequency.txt";
    LOAD_FREQUENCY_RELAY_MODEL_TEST::run_model(outputfile);
}

void PUFLS_TEST::test_composite_load_shedding_scheme_with_realtime_frequency_additional_stage()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PUFLS_TEST");

    PUFLS* model = (PUFLS*) get_load()->get_load_frequency_relay_model();

    model->set_frequency_sensor_time_in_s(0.0);
    model->set_frequency_threshold_in_Hz(49.5);
    model->set_time_delay_in_s(0.2);
    model->set_scale_K_in_pu_per_Hz(0.2);
    model->set_maximum_continuous_shed_scale_in_pu(0.2);
    model->set_additional_stage_trigger_signal(REALTIME_FREQUENCY);
    model->set_additional_stage_shed_scale_in_pu(0.1);
    model->set_additional_stage_time_delay_in_s(5.0);

    model->set_discrete_stage_time_delay_in_s(0.1);

    size_t stage = 0;
    model->set_discrete_stage_shed_scale_in_pu(stage, 0.02); ++stage;
    model->set_discrete_stage_shed_scale_in_pu(stage, 0.03); ++stage;
    model->set_discrete_stage_shed_scale_in_pu(stage, 0.04); ++stage;
    model->set_discrete_stage_shed_scale_in_pu(stage, 0.05); ++stage;
    model->set_discrete_stage_shed_scale_in_pu(stage, 0.06); ++stage;
    model->set_discrete_stage_shed_scale_in_pu(stage, 0.07); ++stage;
    model->set_discrete_stage_shed_scale_in_pu(stage, 0.08); ++stage;
    model->set_discrete_stage_shed_scale_in_pu(stage, 0.09); ++stage;

    string outputfile = "test_log/frequency_ramp_response_of_"+model->get_model_name()+"_model_composite_realtime_frequency.txt";
    LOAD_FREQUENCY_RELAY_MODEL_TEST::run_model(outputfile);
}

void PUFLS_TEST::test_composite_load_shedding_scheme_with_minimum_frequency_additional_stage()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PUFLS_TEST");

    PUFLS* model = (PUFLS*) get_load()->get_load_frequency_relay_model();

    model->set_frequency_sensor_time_in_s(0.0);
    model->set_frequency_threshold_in_Hz(49.5);
    model->set_time_delay_in_s(0.2);
    model->set_scale_K_in_pu_per_Hz(0.2);
    model->set_maximum_continuous_shed_scale_in_pu(0.2);
    model->set_additional_stage_trigger_signal(MINIMUM_FREQUENCY);
    model->set_additional_stage_shed_scale_in_pu(0.1);
    model->set_additional_stage_time_delay_in_s(5.0);

    model->set_discrete_stage_time_delay_in_s(0.1);

    size_t stage = 0;
    model->set_discrete_stage_shed_scale_in_pu(stage, 0.02); ++stage;
    model->set_discrete_stage_shed_scale_in_pu(stage, 0.03); ++stage;
    model->set_discrete_stage_shed_scale_in_pu(stage, 0.04); ++stage;
    model->set_discrete_stage_shed_scale_in_pu(stage, 0.05); ++stage;
    model->set_discrete_stage_shed_scale_in_pu(stage, 0.06); ++stage;
    model->set_discrete_stage_shed_scale_in_pu(stage, 0.07); ++stage;
    model->set_discrete_stage_shed_scale_in_pu(stage, 0.08); ++stage;
    model->set_discrete_stage_shed_scale_in_pu(stage, 0.09); ++stage;

    string outputfile = "test_log/frequency_ramp_response_of_"+model->get_model_name()+"_model_composite_minimum_frequency.txt";
    LOAD_FREQUENCY_RELAY_MODEL_TEST::run_model(outputfile);
}

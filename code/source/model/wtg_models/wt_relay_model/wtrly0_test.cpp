#include "header/model/wtg_models/wt_relay_model/wtrly0_test.h"
#include "header/basic/utility.h"
#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <cstdio>
#include <cmath>

using namespace std;

WTRLY0_TEST::WTRLY0_TEST() : WT_RELAY_MODEL_TEST()
{
    TEST_ADD(WTRLY0_TEST::test_get_model_name);
    TEST_ADD(WTRLY0_TEST::test_set_get_parameters);
    TEST_ADD(WTRLY0_TEST::test_initialize);
}

void WTRLY0_TEST::setup()
{
    WT_RELAY_MODEL_TEST::setup();

    WT_GENERATOR* wt_gen = get_test_wt_generator();

    WTRLY0 model;
    model.set_wind_spped_relay_pair_in_pu_s(0, 0.3, 0.0);
    model.set_wind_spped_relay_pair_in_pu_s(1, 1.5, 0.0);

    model.set_rotor_speed_relay_pair_in_pu_s(0, 0.3, 0.0);
    model.set_rotor_speed_relay_pair_in_pu_s(1, 1.5, 0.0);

    model.set_bus_frequency_relay_pair_in_pu_s(0, 0.95, 1.0);
    model.set_bus_frequency_relay_pair_in_pu_s(1, 1.05, 1.0);

    model.set_bus_voltage_relay_pair_in_pu_s(0, 0.5, 0.5);
    model.set_bus_voltage_relay_pair_in_pu_s(1, 0.7, 2.0);
    model.set_bus_voltage_relay_pair_in_pu_s(2, 1.2, 1.0);
    model.set_bus_voltage_relay_pair_in_pu_s(3, 1.3, 0.0);


    wt_gen->set_model(&model);
}

void WTRLY0_TEST::tear_down()
{
    WT_RELAY_MODEL_TEST::tear_down();

    show_test_end_information();
}


void WTRLY0_TEST::test_get_model_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"WTRLY0_TEST");

    WT_RELAY_MODEL* model = get_test_wt_relay_model();
    if(model!=NULL)
    {
        TEST_ASSERT(model->get_model_name()=="WTRLY0");
    }
    else
        TEST_ASSERT(false);
}

void WTRLY0_TEST::test_set_get_parameters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"WTRLY0_TEST");

    WTRLY0* model = (WTRLY0*) get_test_wt_relay_model();

    model->set_wind_spped_relay_pair_in_pu_s(0, 0.3, 0.0);
    model->set_wind_spped_relay_pair_in_pu_s(1, 1.5, 0.0);

    model->set_rotor_speed_relay_pair_in_pu_s(0, 0.3, 0.0);
    model->set_rotor_speed_relay_pair_in_pu_s(1, 1.5, 0.0);

    model->set_bus_frequency_relay_pair_in_pu_s(0, 0.95, 1.0);
    model->set_bus_frequency_relay_pair_in_pu_s(1, 1.05, 1.0);

    model->set_bus_voltage_relay_pair_in_pu_s(0, 0.5, 0.5);
    model->set_bus_voltage_relay_pair_in_pu_s(1, 0.7, 2.0);
    model->set_bus_voltage_relay_pair_in_pu_s(2, 1.2, 1.0);
    model->set_bus_voltage_relay_pair_in_pu_s(3, 1.3, 0.0);

    TEST_ASSERT(fabs(model->get_wind_speed_relay_threshold_in_pu(0)-0.3)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_wind_speed_relay_delay_in_s(0)-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_wind_speed_relay_threshold_in_pu(1)-1.5)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_wind_speed_relay_delay_in_s(1)-0.0)<FLOAT_EPSILON);

    TEST_ASSERT(fabs(model->get_rotor_speed_relay_threshold_in_pu(0)-0.3)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_rotor_speed_relay_delay_in_s(0)-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_rotor_speed_relay_threshold_in_pu(1)-1.5)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_rotor_speed_relay_delay_in_s(1)-0.0)<FLOAT_EPSILON);

    TEST_ASSERT(fabs(model->get_bus_frequency_relay_threshold_in_pu(0)-0.95)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_bus_frequency_relay_delay_in_s(0)-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_bus_frequency_relay_threshold_in_pu(1)-1.05)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_bus_frequency_relay_delay_in_s(1)-1.0)<FLOAT_EPSILON);

    TEST_ASSERT(fabs(model->get_bus_voltage_relay_threshold_in_pu(0)-0.5)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_bus_voltage_relay_delay_in_s(0)-0.5)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_bus_voltage_relay_threshold_in_pu(1)-0.7)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_bus_voltage_relay_delay_in_s(1)-2.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_bus_voltage_relay_threshold_in_pu(2)-1.2)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_bus_voltage_relay_delay_in_s(2)-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_bus_voltage_relay_threshold_in_pu(3)-1.3)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_bus_voltage_relay_delay_in_s(3)-0.0)<FLOAT_EPSILON);
}

void WTRLY0_TEST::test_initialize()
{
    show_test_information_for_function_of_class(__FUNCTION__,"WTRLY0_TEST");

    ostringstream osstream;

    WTRLY0* model = (WTRLY0*) get_test_wt_relay_model();
    model->initialize();
    osstream<<"WTRLY0 model after initialized:"<<endl;
    show_information_with_leading_time_stamp(osstream);
}

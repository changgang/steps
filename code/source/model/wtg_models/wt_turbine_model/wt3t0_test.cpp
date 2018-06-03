#include "header/model/wtg_models/wt_turbine_model/wt3t0_test.h"
#include "header/basic/utility.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cstdio>
#include <cmath>

using namespace std;

WT3T0_TEST::WT3T0_TEST() : WT_TURBINE_MODEL_TEST()
{
    TEST_ADD(WT3T0_TEST::test_get_model_name);
    TEST_ADD(WT3T0_TEST::test_set_get_parameters);
    TEST_ADD(WT3T0_TEST::test_initialize);
}

void WT3T0_TEST::setup()
{
    WT_TURBINE_MODEL_TEST::setup();

    WT_GENERATOR* wt_gen = get_test_wt_generator();

    WT3T0 model;
    model.set_Hturbine_in_s(5.0);
    model.set_Hgenerator_in_s(3.0);
    model.set_Kshaft_in_pu(20.0);
    model.set_damping_in_pu(1.0);
    model.set_Dshaft_in_pu(1.0);

    wt_gen->set_model(&model);
}

void WT3T0_TEST::tear_down()
{
    WT_TURBINE_MODEL_TEST::tear_down();

    show_test_end_information();
}


void WT3T0_TEST::test_get_model_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"WT3T0_TEST");

    WT_TURBINE_MODEL* model = get_test_wt_turbine_model();
    if(model!=NULL)
    {
        TEST_ASSERT(model->get_model_name()=="WT3T0");
    }
    else
        TEST_ASSERT(false);
}

void WT3T0_TEST::test_set_get_parameters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"WT3T0_TEST");

    WT3T0* model = (WT3T0*) get_test_wt_turbine_model();

    model->set_Hturbine_in_s(5.0);
    model->set_Hgenerator_in_s(3.0);
    model->set_Kshaft_in_pu(20.0);
    model->set_damping_in_pu(2.0);
    model->set_Dshaft_in_pu(1.0);

    TEST_ASSERT(fabs(model->get_Hturbine_in_s()-5.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_Hgenerator_in_s()-3.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_Kshaft_in_pu()-20.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_damping_in_pu()-2.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_Dshaft_in_pu()-1.0)<FLOAT_EPSILON);
}

void WT3T0_TEST::test_initialize()
{
    show_test_information_for_function_of_class(__FUNCTION__,"WT3T0_TEST");

    ostringstream sstream;

    WT3T0* model = (WT3T0*) get_test_wt_turbine_model();
    model->initialize();
    sstream<<"WT3T0 model after initialized:"<<endl;
    sstream<<"Turbine speed = "<<model->get_turbine_speed_in_pu()<<" pu, generator speed = "<<model->get_generator_speed_in_pu()<<" pu"<<endl
           <<"Rotor angle = "<<model->get_rotor_angle_in_deg()<<" deg";
    show_information_with_leading_time_stamp(sstream);
}

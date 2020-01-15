#include "header/basic/test_macro.h"
#include "header/model/wtg_models/wt_turbine_model/wt3t0_test.h"
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

WT3T0_TEST::WT3T0_TEST() : WT_TURBINE_MODEL_TEST()
{
    TEST_ADD(WT3T0_TEST::test_get_model_name);
    TEST_ADD(WT3T0_TEST::test_set_get_parameters);
    TEST_ADD(WT3T0_TEST::test_initialize);
}

void WT3T0_TEST::setup()
{
    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();

    WT_TURBINE_MODEL_TEST::setup();

    WT_GENERATOR* wt_gen = get_test_wt_generator();

    WT3T0 model;
    model.set_toolkit(default_toolkit);
    model.set_device_id(wt_gen->get_device_id());

    model.set_Hturbine_in_s(5.0);
    model.set_Hgenerator_in_s(3.0);
    model.set_Kshaft_in_pu(20.0);
    model.set_damping_in_pu(1.0);
    model.set_Dshaft_in_pu(1.0);

    dmdb.add_model(&model);
}

void WT3T0_TEST::tear_down()
{
    WT_TURBINE_MODEL_TEST::tear_down();

    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();
    dmdb.remove_the_last_model();

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

    ostringstream osstream;

    WT3T0* model = (WT3T0*) get_test_wt_turbine_model();
    model->initialize();
    osstream<<"WT3T0 model after initialized:"<<endl;
    osstream<<"Turbine speed = "<<model->get_turbine_speed_in_pu()<<" pu, generator speed = "<<model->get_generator_speed_in_pu()<<" pu"<<endl
           <<"Rotor angle = "<<model->get_rotor_angle_in_deg()<<" deg";
    default_toolkit.show_information_with_leading_time_stamp(osstream);
}

#endif

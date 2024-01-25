#include "header/basic/test_macro.h"
#include "header/model/vsg_model/vsgcontrol0_test.h"
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

VSGCONTROL0_TEST::VSGCONTROL0_TEST()
{
    ;
}

void VSGCONTROL0_TEST::setup()
{
    VSG_MODEL_TEST::setup();

    VSGCONTROL0* model = new VSGCONTROL0;
    model->set_toolkit(default_toolkit);

    WT_GENERATOR* gen = get_test_wt_generator();
    model->set_device_id(gen->get_device_id());

    set_test_model(model);
}

void VSGCONTROL0_TEST::tear_down()
{
    VSGCONTROL0* model= get_model();

    delete model;
}

VSGCONTROL0* VSGCONTROL0_TEST::get_model()
{
    VSG_MODEL* model = get_test_model();
    return (VSGCONTROL0*) model;
}

void VSGCONTROL0_TEST::test_get_model_name()
{
    VSGCONTROL0* model= get_model();
    TEST_ASSERT(model->get_model_name()=="VSGCONTROL0");
}

void VSGCONTROL0_TEST::test_set_get_parameters()
{
    VSGCONTROL0* model= get_model();
    model->set_Tj_in_s(1.0);
    model->set_D(2.0);
    model->set_Ku(3.0);
    model->set_Te_in_s(4.0);
    TEST_ASSERT(fabs(model->get_Tj_in_s()-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_D()-2.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_Ku()-3.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_Te_in_s()-4.0)<FLOAT_EPSILON);
}

#endif

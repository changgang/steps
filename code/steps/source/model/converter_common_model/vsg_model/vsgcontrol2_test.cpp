#include "header/basic/test_macro.h"
#include "header/model/converter_common_models/vsg_model/vsgcontrol2_test.h"
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

VSGCONTROL2_TEST::VSGCONTROL2_TEST() : VSG_MODEL_TEST()
{
    TEST_ADD(VSGCONTROL2_TEST::test_get_model_name);
    TEST_ADD(VSGCONTROL2_TEST::test_set_get_parameters);
}

void VSGCONTROL2_TEST::setup()
{
    VSG_MODEL_TEST::setup();

    VSGCONTROL2* model = new VSGCONTROL2(default_toolkit);
    model->set_bus(1);

    model->set_Tj_in_s(5.0);
    model->set_D(0.2);
    model->set_Ku(0.2);
    model->set_Te_in_s(4.0);

    set_test_model(model);
}

void VSGCONTROL2_TEST::tear_down()
{
    VSGCONTROL2* model= get_model();
    delete model;
}

VSGCONTROL2* VSGCONTROL2_TEST::get_model()
{
    VSG_MODEL2* model = get_test_model();
    return (VSGCONTROL2*) model;
}

void VSGCONTROL2_TEST::test_get_model_name()
{
    VSGCONTROL2* model= get_model();
    TEST_ASSERT(model->get_model_name()=="VSGCONTROL2");
}

void VSGCONTROL2_TEST::test_set_get_parameters()
{
    VSGCONTROL2* model= get_model();
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

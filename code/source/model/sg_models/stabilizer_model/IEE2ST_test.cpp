#include "header/model/sg_models/stabilizer_model/IEE2ST_test.h"
#include "header/basic/utility.h"
using namespace std;

IEE2ST_TEST::IEE2ST_TEST() : STABILIZER_MODEL_TEST()
{
    TEST_ADD(IEE2ST_TEST::test_get_model_name);
    TEST_ADD(IEE2ST_TEST::test_set_get_parameters);

}

void IEE2ST_TEST::setup()
{
    STABILIZER_MODEL_TEST::setup();

    GENERATOR* genptr = get_test_generator();
    POWER_SYSTEM_DATABASE* psdb = get_test_power_system_database();
    IEE2ST model;
    model.set_power_system_database(psdb);
    genptr->set_model(&model);
}

void IEE2ST_TEST::tear_down()
{
    STABILIZER_MODEL_TEST::tear_down();
}


void IEE2ST_TEST::test_get_model_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"IEE2ST_TEST");

    IEE2ST* model = (IEE2ST*) get_test_stabilizer_model();
    TEST_ASSERT(model->get_model_name()=="IEE2ST");
}

void IEE2ST_TEST::test_set_get_parameters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"IEE2ST_TEST");

    IEE2ST* model = (IEE2ST*) get_test_stabilizer_model();
    model->set_K1(1.0);
    TEST_ASSERT(fabs(model->get_K1()-1.0)<FLOAT_EPSILON);

    model->set_T1_in_s(0.1);
    TEST_ASSERT(fabs(model->get_T1_in_s()-0.1)<FLOAT_EPSILON);

    model->set_K2(2.0);
    TEST_ASSERT(fabs(model->get_K2()-2.0)<FLOAT_EPSILON);

    model->set_T2_in_s(0.2);
    TEST_ASSERT(fabs(model->get_T2_in_s()-0.2)<FLOAT_EPSILON);

    model->set_T3_in_s(0.3);
    TEST_ASSERT(fabs(model->get_T3_in_s()-0.3)<FLOAT_EPSILON);

    model->set_T4_in_s(0.4);
    TEST_ASSERT(fabs(model->get_T4_in_s()-0.4)<FLOAT_EPSILON);

    model->set_T5_in_s(0.5);
    TEST_ASSERT(fabs(model->get_T5_in_s()-0.5)<FLOAT_EPSILON);

    model->set_T6_in_s(0.6);
    TEST_ASSERT(fabs(model->get_T6_in_s()-0.6)<FLOAT_EPSILON);

    model->set_T7_in_s(0.7);
    TEST_ASSERT(fabs(model->get_T7_in_s()-0.7)<FLOAT_EPSILON);

    model->set_T8_in_s(0.8);
    TEST_ASSERT(fabs(model->get_T8_in_s()-0.8)<FLOAT_EPSILON);

    model->set_T9_in_s(0.9);
    TEST_ASSERT(fabs(model->get_T9_in_s()-0.9)<FLOAT_EPSILON);

    model->set_T10_in_s(1.0);
    TEST_ASSERT(fabs(model->get_T10_in_s()-1.0)<FLOAT_EPSILON);
}

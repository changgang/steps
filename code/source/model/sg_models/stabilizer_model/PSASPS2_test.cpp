#include "header/model/sg_models/stabilizer_model/PSASPS2_test.h"
#include "header/basic/utility.h"
using namespace std;

PSASPS2_TEST::PSASPS2_TEST() : STABILIZER_MODEL_TEST()
{
    TEST_ADD(PSASPS2_TEST::test_get_model_name);
    TEST_ADD(PSASPS2_TEST::test_set_get_parameters);

}

void PSASPS2_TEST::setup()
{
    STABILIZER_MODEL_TEST::setup();

    GENERATOR* genptr = get_test_generator();
    PSASPS2 model;
    genptr->set_model(&model);
}

void PSASPS2_TEST::tear_down()
{
    STABILIZER_MODEL_TEST::tear_down();
}


void PSASPS2_TEST::test_get_model_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PSASPS2_TEST");

    PSASPS2* model = (PSASPS2*) get_test_stabilizer_model();
    TEST_ASSERT(model->get_model_name()=="PSASPS2");
}

void PSASPS2_TEST::test_set_get_parameters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PSASPS2_TEST");

    PSASPS2* model = (PSASPS2*) get_test_stabilizer_model();

    model->set_Kw(1.0);
    TEST_ASSERT(fabs(model->get_Kw()-1.0)<FLOAT_EPSILON);

    model->set_Kp(1.5);
    TEST_ASSERT(fabs(model->get_Kp()-1.5)<FLOAT_EPSILON);

    model->set_Kt(2.5);
    TEST_ASSERT(fabs(model->get_Kt()-2.5)<FLOAT_EPSILON);

    model->set_Tw1_in_s(0.1);
    TEST_ASSERT(fabs(model->get_Tw1_in_s()-0.1)<FLOAT_EPSILON);

    model->set_Tw2_in_s(0.1);
    TEST_ASSERT(fabs(model->get_Tw2_in_s()-0.1)<FLOAT_EPSILON);

    model->set_T1_in_s(0.2);
    TEST_ASSERT(fabs(model->get_T1_in_s()-0.2)<FLOAT_EPSILON);

    model->set_T2_in_s(0.3);
    TEST_ASSERT(fabs(model->get_T2_in_s()-0.3)<FLOAT_EPSILON);

    model->set_T3_in_s(0.4);
    TEST_ASSERT(fabs(model->get_T3_in_s()-0.4)<FLOAT_EPSILON);

    model->set_T4_in_s(0.5);
    TEST_ASSERT(fabs(model->get_T4_in_s()-0.5)<FLOAT_EPSILON);

    model->set_T5_in_s(0.6);
    TEST_ASSERT(fabs(model->get_T5_in_s()-0.6)<FLOAT_EPSILON);

    model->set_T6_in_s(0.7);
    TEST_ASSERT(fabs(model->get_T6_in_s()-0.7)<FLOAT_EPSILON);

    model->set_Vsmax(3.0);
    TEST_ASSERT(fabs(model->get_Vsmax()-3.0)<FLOAT_EPSILON);

    model->set_Vsmin(2.0);
    TEST_ASSERT(fabs(model->get_Vsmin()-2.0)<FLOAT_EPSILON);
}

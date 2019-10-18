#include "header/basic/test_macro.h"
#include "header/model/sg_models/stabilizer_model/PSASPS3_test.h"
#include "header/basic/utility.h"

#ifdef ENABLE_STEPS_TEST
using namespace std;

PSASPS3_TEST::PSASPS3_TEST() : STABILIZER_MODEL_TEST()
{
    TEST_ADD(PSASPS3_TEST::test_get_model_name);
    TEST_ADD(PSASPS3_TEST::test_set_get_parameters);

}

void PSASPS3_TEST::setup()
{
    STABILIZER_MODEL_TEST::setup();

    GENERATOR* genptr = get_test_generator();
    PSASPS3 model;
    genptr->set_model(&model);
}

void PSASPS3_TEST::tear_down()
{
    STABILIZER_MODEL_TEST::tear_down();
}


void PSASPS3_TEST::test_get_model_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PSASPS3_TEST");

    PSASPS3* model = (PSASPS3*) get_test_stabilizer_model();
    TEST_ASSERT(model->get_model_name()=="PSASPS3");
}

void PSASPS3_TEST::test_set_get_parameters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PSASPS3_TEST");

    PSASPS3* model = (PSASPS3*) get_test_stabilizer_model();

    model->set_Macc(1.0);
    TEST_ASSERT(fabs(model->get_Macc()-1.0)<FLOAT_EPSILON);

    model->set_Iacc(1.5);
    TEST_ASSERT(fabs(model->get_Iacc()-1.5)<FLOAT_EPSILON);

    model->set_Nacc(2.5);
    TEST_ASSERT(fabs(model->get_Nacc()-2.5)<FLOAT_EPSILON);

    model->set_dedc_1_flag(true);
    TEST_ASSERT(model->get_dedc_1_flag()==true);

    model->set_dedc_1_flag(false);
    TEST_ASSERT(model->get_dedc_1_flag()==false);

    model->set_Tacc_in_s(0.1);
    TEST_ASSERT(fabs(model->get_Tacc_in_s()-0.1)<FLOAT_EPSILON);

    model->set_TD_in_s(0.15);
    TEST_ASSERT(fabs(model->get_TD_in_s()-0.15)<FLOAT_EPSILON);

    model->set_Ti1_in_s(0.2);
    TEST_ASSERT(fabs(model->get_Ti1_in_s()-0.2)<FLOAT_EPSILON);

    model->set_Ti2_in_s(0.3);
    TEST_ASSERT(fabs(model->get_Ti2_in_s()-0.3)<FLOAT_EPSILON);

    model->set_Ti3_in_s(0.4);
    TEST_ASSERT(fabs(model->get_Ti3_in_s()-0.4)<FLOAT_EPSILON);

    model->set_Ti4_in_s(0.5);
    TEST_ASSERT(fabs(model->get_Ti4_in_s()-0.5)<FLOAT_EPSILON);

    model->set_K0(0.6);
    TEST_ASSERT(fabs(model->get_K0()-0.6)<FLOAT_EPSILON);

    model->set_K1(0.62);
    TEST_ASSERT(fabs(model->get_K1()-0.62)<FLOAT_EPSILON);

    model->set_K2(0.7);
    TEST_ASSERT(fabs(model->get_K2()-0.7)<FLOAT_EPSILON);

    model->set_K3(0.8);
    TEST_ASSERT(fabs(model->get_K3()-0.8)<FLOAT_EPSILON);

    model->set_K4(0.9);
    TEST_ASSERT(fabs(model->get_K4()-0.9)<FLOAT_EPSILON);

    model->set_KP(0.65);
    TEST_ASSERT(fabs(model->get_KP()-0.65)<FLOAT_EPSILON);

    model->set_Vsmax(3.0);
    TEST_ASSERT(fabs(model->get_Vsmax()-3.0)<FLOAT_EPSILON);

    model->set_Vsmin(2.0);
    TEST_ASSERT(fabs(model->get_Vsmin()-2.0)<FLOAT_EPSILON);
}

#endif

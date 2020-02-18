#include "header/basic/test_macro.h"
#include "header/model/sg_models/stabilizer_model/PSASPS8_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

#ifdef ENABLE_STEPS_TEST
using namespace std;

PSASPS8_TEST::PSASPS8_TEST() : STABILIZER_MODEL_TEST()
{
    TEST_ADD(PSASPS8_TEST::test_get_model_name);
    TEST_ADD(PSASPS8_TEST::test_set_get_parameters);

}

void PSASPS8_TEST::setup()
{
    STABILIZER_MODEL_TEST::setup();

    GENERATOR* genptr = get_test_generator();
    PSASPS8 model(default_toolkit);
    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();
    model.set_device_id(genptr->get_device_id());

    dmdb.add_model(&model);
}

void PSASPS8_TEST::tear_down()
{
    STABILIZER_MODEL_TEST::tear_down();

    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();
    dmdb.remove_the_last_model();
}


void PSASPS8_TEST::test_get_model_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PSASPS8_TEST");

    PSASPS8* model = (PSASPS8*) get_test_stabilizer_model();
    TEST_ASSERT(model->get_model_name()=="PSASPS8");
}

void PSASPS8_TEST::test_set_get_parameters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PSASPS8_TEST");

    PSASPS8* model = (PSASPS8*) get_test_stabilizer_model();

    model->set_Kqv(1.0);
    TEST_ASSERT(fabs(model->get_Kqv()-1.0)<FLOAT_EPSILON);

    model->set_Tqv_in_s(0.01);
    TEST_ASSERT(fabs(model->get_Tqv_in_s()-0.01)<FLOAT_EPSILON);

    model->set_Tq1_in_s(0.1);
    TEST_ASSERT(fabs(model->get_Tq1_in_s()-0.1)<FLOAT_EPSILON);

    model->set_Tq1p_in_s(0.2);
    TEST_ASSERT(fabs(model->get_Tq1p_in_s()-0.2)<FLOAT_EPSILON);

    model->set_Tq2_in_s(0.15);
    TEST_ASSERT(fabs(model->get_Tq2_in_s()-0.15)<FLOAT_EPSILON);

    model->set_Tq2p_in_s(0.25);
    TEST_ASSERT(fabs(model->get_Tq2p_in_s()-0.25)<FLOAT_EPSILON);

    model->set_Tq3_in_s(0.3);
    TEST_ASSERT(fabs(model->get_Tq3_in_s()-0.3)<FLOAT_EPSILON);

    model->set_Tq3p_in_s(0.4);
    TEST_ASSERT(fabs(model->get_Tq3p_in_s()-0.4)<FLOAT_EPSILON);

    model->set_Vsmax(3.0);
    TEST_ASSERT(fabs(model->get_Vsmax()-3.0)<FLOAT_EPSILON);

    model->set_Vsmin(2.0);
    TEST_ASSERT(fabs(model->get_Vsmin()-2.0)<FLOAT_EPSILON);
}

#endif

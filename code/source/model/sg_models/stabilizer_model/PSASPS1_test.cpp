#include "header/basic/test_macro.h"
#include "header/model/sg_models/stabilizer_model/PSASPS1_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

#ifdef ENABLE_STEPS_TEST
using namespace std;

PSASPS1_TEST::PSASPS1_TEST() : STABILIZER_MODEL_TEST()
{
    TEST_ADD(PSASPS1_TEST::test_get_model_name);
    TEST_ADD(PSASPS1_TEST::test_set_get_parameters);

}

void PSASPS1_TEST::setup()
{
    STABILIZER_MODEL_TEST::setup();

    GENERATOR* genptr = get_test_generator();
    PSASPS1 model(default_toolkit);
    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();
    model.set_device_id(genptr->get_device_id());

    dmdb.add_model(&model);
}

void PSASPS1_TEST::tear_down()
{
    STABILIZER_MODEL_TEST::tear_down();

    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();
    dmdb.remove_the_last_model();
}


void PSASPS1_TEST::test_get_model_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PSASPS1_TEST");

    PSASPS1* model = (PSASPS1*) get_test_stabilizer_model();
    TEST_ASSERT(model->get_model_name()=="PSASPS1");
}

void PSASPS1_TEST::test_set_get_parameters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PSASPS1_TEST");

    PSASPS1* model = (PSASPS1*) get_test_stabilizer_model();

    model->set_Kq1(1.0);
    TEST_ASSERT(fabs(model->get_Kq1()-1.0)<FLOAT_EPSILON);

    model->set_Kq2(1.5);
    TEST_ASSERT(fabs(model->get_Kq2()-1.5)<FLOAT_EPSILON);

    model->set_Kq3(2.5);
    TEST_ASSERT(fabs(model->get_Kq3()-2.5)<FLOAT_EPSILON);

    model->set_K(1);
    TEST_ASSERT(model->get_K()==1);

    model->set_K(0);
    TEST_ASSERT(model->get_K()==0);

    model->set_Tq_in_s(0.1);
    TEST_ASSERT(fabs(model->get_Tq_in_s()-0.1)<FLOAT_EPSILON);

    model->set_T1e_in_s(0.2);
    TEST_ASSERT(fabs(model->get_T1e_in_s()-0.2)<FLOAT_EPSILON);

    model->set_T2e_in_s(0.3);
    TEST_ASSERT(fabs(model->get_T2e_in_s()-0.3)<FLOAT_EPSILON);

    model->set_T3e_in_s(0.4);
    TEST_ASSERT(fabs(model->get_T3e_in_s()-0.4)<FLOAT_EPSILON);

    model->set_T4e_in_s(0.5);
    TEST_ASSERT(fabs(model->get_T4e_in_s()-0.5)<FLOAT_EPSILON);

    model->set_Vsmax(3.0);
    TEST_ASSERT(fabs(model->get_Vsmax()-3.0)<FLOAT_EPSILON);

    model->set_Vsmin(2.0);
    TEST_ASSERT(fabs(model->get_Vsmin()-2.0)<FLOAT_EPSILON);
}

#endif

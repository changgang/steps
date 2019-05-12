#include "header/model/sg_models/exciter_model/PSASPE2_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

using namespace std;
PSASPE2_TEST::PSASPE2_TEST() : EXCITER_MODEL_TEST()
{
    TEST_ADD(PSASPE2_TEST::test_get_model_name);
    TEST_ADD(PSASPE2_TEST::test_set_get_parameters);
    TEST_ADD(PSASPE2_TEST::test_initialize);
}



void PSASPE2_TEST::setup()
{
    EXCITER_MODEL_TEST::setup();

    PSASPE2 model;

    GENERATOR* genptr = get_test_generator();

    model.set_KR(1.0);
    model.set_TR_in_s(0.02);
    model.set_K2(true);
    model.set_T1_in_s(2.0);
    model.set_T2_in_s(2.0);
    model.set_T3_in_s(2.0);
    model.set_T4_in_s(2.0);
    model.set_KA(20.0);
    model.set_TA_in_s(0.02);
    model.set_Efdmax_in_pu(3.3);
    model.set_Efdmin_in_pu(-2.6);
    model.set_Vta_in_pu(0.9175);
    model.set_Vtb_in_pu(1.0825);
    model.set_Kpt(1.0);
    model.set_Kit(0.25);
    model.set_Ke(0.12);

    genptr->set_model(&model);
}

void PSASPE2_TEST::tear_down()
{
    EXCITER_MODEL_TEST::tear_down();
}

void PSASPE2_TEST::test_get_model_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PSASPE2_TEST");

    EXCITER_MODEL* model = get_test_exciter_model();

    TEST_ASSERT(model->get_model_name()=="PSASPE2");
}

void PSASPE2_TEST::test_set_get_parameters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PSASPE2_TEST");

    PSASPE2* model = (PSASPE2*) get_test_exciter_model();

    TEST_ASSERT(fabs(model->get_KR()-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_TR_in_s()-0.02)<FLOAT_EPSILON);
    TEST_ASSERT(model->get_K2() == true);
    TEST_ASSERT(fabs(model->get_T1_in_s()-2.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_T2_in_s()-2.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_T3_in_s()-2.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_T4_in_s()-2.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_KA()-20.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_TA_in_s()-0.02)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_Efdmax_in_pu()-3.3)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_Efdmin_in_pu()-(-2.6))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_Vta_in_pu()-0.9175)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_Vtb_in_pu()-1.0825)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_Kpt()-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_Kit()-0.25)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_Ke()-0.12)<FLOAT_EPSILON);
}

void PSASPE2_TEST::test_initialize()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PSASPE2_TEST");

    PSASPE2* model = (PSASPE2*) get_test_exciter_model();

    SYNC_GENERATOR_MODEL* genmodel = get_test_sync_generator_model();
    genmodel->initialize();

    double Efd0 = genmodel->get_initial_excitation_voltage_in_pu();
    double Ecomp = model->get_compensated_voltage_in_pu();

    model->initialize();

    TEST_ASSERT(fabs(model->get_excitation_voltage_in_pu()-Efd0)<FLOAT_EPSILON);
    double Vref = Ecomp;
    TEST_ASSERT(fabs(model->get_voltage_reference_in_pu()-Vref)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_stabilizing_signal_in_pu()-0.0)<FLOAT_EPSILON);
}

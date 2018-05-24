#include "header/model/sg_models/exciter_model/PSASPE1_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

using namespace std;
PSASPE1_TEST::PSASPE1_TEST() : EXCITER_MODEL_TEST()
{
    TEST_ADD(PSASPE1_TEST::test_get_model_name);
    TEST_ADD(PSASPE1_TEST::test_set_get_parameters);
    TEST_ADD(PSASPE1_TEST::test_initialize);
}



void PSASPE1_TEST::setup()
{
    EXCITER_MODEL_TEST::setup();

    PSASPE1 model;

    GENERATOR* genptr = get_test_generator();
    model.set_power_system_database(genptr->get_power_system_database());
    model.set_device_id(genptr->get_device_id());

    model.set_KR(1.0);
    model.set_TR_in_s(0.03);
    model.set_KA(50.0);
    model.set_TA_in_s(0.03);
    model.set_TE_in_s(0.5);
    model.set_Efdmax_in_pu(5.0);
    model.set_Efdmin_in_pu(0.0);
    model.set_KF(0.04);
    model.set_TF_in_s(0.715);

    genptr->set_model(&model);
}

void PSASPE1_TEST::tear_down()
{
    EXCITER_MODEL_TEST::tear_down();
}

void PSASPE1_TEST::test_get_model_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PSASPE1_TEST");

    EXCITER_MODEL* model = get_test_exciter_model();

    TEST_ASSERT(model->get_model_name()=="PSASPE1");
}

void PSASPE1_TEST::test_set_get_parameters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PSASPE1_TEST");

    PSASPE1* model = (PSASPE1*) get_test_exciter_model();

    TEST_ASSERT(fabs(model->get_KR()-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_TR_in_s()-0.03)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_KA()-50.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_TA_in_s()-0.03)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_TE_in_s()-0.5)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_Efdmax_in_pu()-5.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_Efdmin_in_pu()-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_KF()-0.04)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_TF_in_s()-0.715)<FLOAT_EPSILON);
}

void PSASPE1_TEST::test_initialize()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PSASPE1_TEST");

    GENERATOR* genptr = get_test_generator();
    PSASPE1* model = (PSASPE1*) get_test_exciter_model();

    SYNC_GENERATOR_MODEL* genmodel = genptr->get_sync_generator_model();
    genmodel->initialize();

    double Efd0 = genmodel->get_initial_excitation_voltage_in_pu();
    double Ecomp = model->get_compensated_voltage_in_pu();

    model->initialize();

    TEST_ASSERT(fabs(model->get_excitation_voltage_in_pu()-Efd0)<FLOAT_EPSILON);
    double Vref = Ecomp;
    TEST_ASSERT(fabs(model->get_voltage_reference_in_pu()-Vref)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_stabilizing_signal_in_pu()-0.0)<FLOAT_EPSILON);
}

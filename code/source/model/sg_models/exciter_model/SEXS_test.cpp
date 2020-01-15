#include "header/basic/test_macro.h"
#include "header/model/sg_models/exciter_model/SEXS_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

#ifdef ENABLE_STEPS_TEST
using namespace std;
SEXS_TEST::SEXS_TEST() : EXCITER_MODEL_TEST()
{
    TEST_ADD(SEXS_TEST::test_get_model_name);
    TEST_ADD(SEXS_TEST::test_set_get_parameters);
    TEST_ADD(SEXS_TEST::test_initialize);
}



void SEXS_TEST::setup()
{
    EXCITER_MODEL_TEST::setup();
    GENERATOR* genptr = get_test_generator();

    SEXS model;
    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();
    model.set_toolkit(default_toolkit);
    model.set_device_id(genptr->get_device_id());

    model.set_TA_in_s(1.0);
    model.set_TB_in_s(4.0);
    model.set_K(50.0);
    model.set_TE_in_s(0.02);
    model.set_Efdmax_in_pu(6.0);
    model.set_Efdmin_in_pu(-4.0);
    dmdb.add_model(&model);
}

void SEXS_TEST::tear_down()
{
    EXCITER_MODEL_TEST::tear_down();

    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();
    dmdb.remove_the_last_model();
}


void SEXS_TEST::test_get_model_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SEXS_TEST");

    EXCITER_MODEL* model = get_test_exciter_model();

    TEST_ASSERT(model->get_model_name()=="SEXS");
}

void SEXS_TEST::test_set_get_parameters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SEXS_TEST");

    SEXS* model = (SEXS*) get_test_exciter_model();

    TEST_ASSERT(fabs(model->get_TA_in_s()-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_TB_in_s()-4.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_K()-50.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_TE_in_s()-0.02)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_Efdmax_in_pu()-6.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_Efdmin_in_pu()-(-4.0))<FLOAT_EPSILON);
}

void SEXS_TEST::test_initialize()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SEXS_TEST");

    SYNC_GENERATOR_MODEL* genmodel = get_test_sync_generator_model();
    genmodel->initialize();

    SEXS* model = (SEXS*) get_test_exciter_model();

    double Efd0 = genmodel->get_initial_excitation_voltage_in_pu();
    double Ecomp = model->get_compensated_voltage_in_pu();

    model->initialize();

    TEST_ASSERT(fabs(model->get_excitation_voltage_in_pu()-Efd0)<FLOAT_EPSILON);
    double Vref = Efd0/model->get_K()+Ecomp;
    TEST_ASSERT(fabs(model->get_voltage_reference_in_pu()-Vref)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_stabilizing_signal_in_pu()-0.0)<FLOAT_EPSILON);
}

#endif

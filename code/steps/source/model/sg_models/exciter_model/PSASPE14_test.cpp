#include "header/basic/test_macro.h"
#include "header/model/sg_models/exciter_model/PSASPE14_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

#ifdef ENABLE_STEPS_TEST
using namespace std;
PSASPE14_TEST::PSASPE14_TEST() : EXCITER_MODEL_TEST()
{
    TEST_ADD(PSASPE14_TEST::test_get_model_name);
    TEST_ADD(PSASPE14_TEST::test_set_get_parameters);
    TEST_ADD(PSASPE14_TEST::test_initialize);
}



void PSASPE14_TEST::setup()
{
    EXCITER_MODEL_TEST::setup();

    GENERATOR* genptr = get_test_generator();

    PSASPE14 model(default_toolkit);
    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();
    model.set_device_id(genptr->get_device_id());

    dmdb.add_model(&model);
}

void PSASPE14_TEST::tear_down()
{
    EXCITER_MODEL_TEST::tear_down();

    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();
    dmdb.remove_the_last_model();
}

void PSASPE14_TEST::test_get_model_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PSASPE14_TEST");

    EXCITER_MODEL* model = get_test_exciter_model();

    TEST_ASSERT(model->get_model_name()=="PSASPE14");
}

void PSASPE14_TEST::test_set_get_parameters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PSASPE14_TEST");

    //PSASPE14* model = (PSASPE14*) get_test_exciter_model();

}

void PSASPE14_TEST::test_initialize()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PSASPE14_TEST");

    SYNC_GENERATOR_MODEL* genmodel = get_test_sync_generator_model();
    genmodel->initialize();

    PSASPE14* model = (PSASPE14*) get_test_exciter_model();

    //double Efd0 = genmodel->get_initial_excitation_voltage_in_pu();
    //double Ecomp = model->get_compensated_voltage_in_pu();

    model->initialize();

}

#endif

#include "header/basic/test_macro.h"
#include "header/model/sg_models/turbine_governor_model/TGOV1_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

#ifdef ENABLE_STEPS_TEST
using namespace std;

TGOV1_TEST::TGOV1_TEST()
{
    TEST_ADD(TGOV1_TEST::test_get_model_type);
    TEST_ADD(TGOV1_TEST::test_get_model_name);
    TEST_ADD(TGOV1_TEST::test_set_get_parameters);
    TEST_ADD(TGOV1_TEST::test_initialize);
    TEST_ADD(TGOV1_TEST::test_set_get_mechanical_power_reference);
    TEST_ADD(TGOV1_TEST::test_set_get_mechanical_power_upper_limit);
    TEST_ADD(TGOV1_TEST::test_set_get_mechanical_power_lower_limit);
    TEST_ADD(TGOV1_TEST::test_step_response);

}

void TGOV1_TEST::setup()
{
    TURBINE_GOVERNOR_MODEL_TEST::setup();

    GENERATOR* genptr = get_test_generator();

    TGOV1 model;
    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();
    model.set_toolkit(default_toolkit);
    model.set_device_id(genptr->get_device_id());

    model.set_R(0.05);
    model.set_T1_in_s(0.1);
    model.set_T2_in_s(0.3);
    model.set_T3_in_s(7.0);
    model.set_D(2.0);
    model.set_Valvemax_in_pu(1.0);
    model.set_Valvemin_in_pu(0.0);

    dmdb.add_model(&model);
}

void TGOV1_TEST::tear_down()
{
    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();
    dmdb.remove_the_last_model();

    TURBINE_GOVERNOR_MODEL_TEST::tear_down();
}

void TGOV1_TEST::test_get_model_type()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TGOV1_TEST");

    TURBINE_GOVERNOR_MODEL* model = get_test_turbine_governor_model();

    TEST_ASSERT(model->get_model_type()=="TURBINE GOVERNOR");
}

void TGOV1_TEST::test_get_model_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TGOV1_TEST");

    TURBINE_GOVERNOR_MODEL* model = get_test_turbine_governor_model();

    TEST_ASSERT(model->get_model_name()=="TGOV1");
}

void TGOV1_TEST::test_set_get_parameters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TGOV1_TEST");

    TGOV1* model = (TGOV1*) get_test_turbine_governor_model();

    TEST_ASSERT(fabs(model->get_R()-0.05)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_T1_in_s()-0.1)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_T2_in_s()-0.3)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_T3_in_s()-7.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_D()-2.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_Valvemax_in_pu()-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_Valvemin_in_pu()-0.0)<FLOAT_EPSILON);
}

void TGOV1_TEST::test_initialize()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TGOV1_TEST");

    SYNC_GENERATOR_MODEL* genmodel = get_test_sync_generator_model();
    genmodel->initialize();

    double Pmech0 = genmodel->get_initial_mechanical_power_in_pu_based_on_mbase();

    TGOV1* model = (TGOV1*) get_test_turbine_governor_model();

    model->set_Valvemax_in_pu(1.2);
    model->initialize();

    TEST_ASSERT(fabs(model->get_mechanical_power_in_pu_based_on_mbase()-Pmech0)<FLOAT_EPSILON);
    double Pref = Pmech0*model->get_R();
    TEST_ASSERT(fabs(model->get_mechanical_power_reference_in_pu_based_on_mbase()-Pref)<FLOAT_EPSILON);
}

void TGOV1_TEST::test_set_get_mechanical_power_reference()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TGOV1_TEST");

    TGOV1* model = (TGOV1*) get_test_turbine_governor_model();

    model->set_initial_mechanical_power_reference_in_pu_based_on_mbase(1.5);
    TEST_ASSERT(fabs(model->get_mechanical_power_reference_in_pu_based_on_mbase()-1.5)<FLOAT_EPSILON);
}

void TGOV1_TEST::test_set_get_mechanical_power_upper_limit()
{
    show_test_information_for_function_of_class(__FUNCTION__,"IEEEG1_TEST");

    TGOV1* model = (TGOV1*) get_test_turbine_governor_model();

    model->set_Valvemax_in_pu(1.5);
    TEST_ASSERT(fabs(model->get_mechanical_power_upper_limit_in_pu_based_on_mbase()-1.5)<FLOAT_EPSILON);
}

void TGOV1_TEST::test_set_get_mechanical_power_lower_limit()
{
    show_test_information_for_function_of_class(__FUNCTION__,"IEEEG1_TEST");

    TGOV1* model = (TGOV1*) get_test_turbine_governor_model();

    model->set_Valvemin_in_pu(0.5);
    TEST_ASSERT(fabs(model->get_mechanical_power_lower_limit_in_pu_based_on_mbase()-0.5)<FLOAT_EPSILON);
}

void TGOV1_TEST::test_step_response()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TGOV1_TEST");

    default_toolkit.open_log_file("test_log/step_response_of_TGOV1_model.txt");

    TGOV1* model = (TGOV1*) get_test_turbine_governor_model();

    model->set_Valvemax_in_pu(1.2);

    run_step_response_of_turbine_govnernor_model();

    default_toolkit.close_log_file();

}

void TGOV1_TEST::test_get_standard_psse_string()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TGOV1_TEST");
}



#endif

#include "header/model/sg_models/turbine_governor_model/IEEEG3_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
using namespace std;

IEEEG3_TEST::IEEEG3_TEST()
{
    TEST_ADD(IEEEG3_TEST::test_get_model_type);
    TEST_ADD(IEEEG3_TEST::test_get_model_name);
    TEST_ADD(IEEEG3_TEST::test_set_get_parameters);
    TEST_ADD(IEEEG3_TEST::test_initialize);
    TEST_ADD(IEEEG3_TEST::test_set_get_mechanical_power_reference);
    TEST_ADD(IEEEG3_TEST::test_set_get_mechanical_power_upper_limit);
    TEST_ADD(IEEEG3_TEST::test_set_get_mechanical_power_lower_limit);
    TEST_ADD(IEEEG3_TEST::test_step_response);

}

void IEEEG3_TEST::setup()
{
    TURBINE_GOVERNOR_MODEL_TEST::setup();

    IEEEG3 model;
    GENERATOR* genptr = get_test_generator();

    model.set_TG_in_s(0.5);
    model.set_TP_in_s(0.2);
    model.set_Uo_in_pu(0.1);
    model.set_Uc_in_pu(-0.1);
    model.set_Pmax_in_pu(1.0);
    model.set_Pmin_in_pu(0.0);
    model.set_sigma(0.05);
    model.set_delta(0.3);
    model.set_TR_in_s(3.0);
    model.set_TW_in_s(2.0);
    model.set_a11(0.5);
    model.set_a13(1.5);
    model.set_a21(1.0);
    model.set_a23(1.0);

    genptr->set_model(&model);
}

void IEEEG3_TEST::tear_down()
{
    TURBINE_GOVERNOR_MODEL_TEST::tear_down();
}

void IEEEG3_TEST::test_get_model_type()
{
    show_test_information_for_function_of_class(__FUNCTION__,"IEEEG3_TEST");

    TURBINE_GOVERNOR_MODEL* model = get_test_turbine_governor_model();

    TEST_ASSERT(model->get_model_type()=="TURBINE GOVERNOR");
}

void IEEEG3_TEST::test_get_model_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"IEEEG3_TEST");

    TURBINE_GOVERNOR_MODEL* model = get_test_turbine_governor_model();

    TEST_ASSERT(model->get_model_name()=="IEEEG3");
}

void IEEEG3_TEST::test_set_get_parameters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"IEEEG3_TEST");

    IEEEG3* model = (IEEEG3*) get_test_turbine_governor_model();

    TEST_ASSERT(fabs(model->get_TG_in_s()-0.5)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_TP_in_s()-0.2)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_Uo_in_pu()-0.1)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_Uc_in_pu()+0.1)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_Pmax_in_pu()-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_Pmin_in_pu()-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_sigma()-0.05)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_delta()-0.3)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_TR_in_s()-3.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_TW_in_s()-2.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_a11()-0.5)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_a13()-1.5)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_a21()-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_a23()-1.0)<FLOAT_EPSILON);
}

void IEEEG3_TEST::test_initialize()
{
    show_test_information_for_function_of_class(__FUNCTION__,"IEEEG3_TEST");

    SYNC_GENERATOR_MODEL* genmodel = get_test_sync_generator_model();
    genmodel->initialize();

    double Pmech0 = genmodel->get_initial_mechanical_power_in_pu_based_on_mbase();

    IEEEG3* model = (IEEEG3*) get_test_turbine_governor_model();

    model->set_Pmax_in_pu(1.2);

    model->initialize();

    double a23 = model->get_a23();
    double sigma = model->get_sigma();

    double Pref = Pmech0/a23*sigma;

    TEST_ASSERT(fabs(model->get_mechanical_power_in_pu_based_on_mbase()-Pmech0)<FLOAT_EPSILON);

    TEST_ASSERT(fabs(model->get_mechanical_power_reference_in_pu_based_on_mbase()-Pref)<FLOAT_EPSILON);
}

void IEEEG3_TEST::test_set_get_mechanical_power_reference()
{
    show_test_information_for_function_of_class(__FUNCTION__,"IEEEG3_TEST");

    IEEEG3* model = (IEEEG3*) get_test_turbine_governor_model();

    model->set_initial_mechanical_power_reference_in_pu_based_on_mbase(1.5);

    TEST_ASSERT(fabs(model->get_mechanical_power_reference_in_pu_based_on_mbase()-1.5)<FLOAT_EPSILON);
}

void IEEEG3_TEST::test_set_get_mechanical_power_upper_limit()
{
    show_test_information_for_function_of_class(__FUNCTION__,"IEEEG1_TEST");

    IEEEG3* model = (IEEEG3*) get_test_turbine_governor_model();

    model->set_Pmax_in_pu(1.5);
    TEST_ASSERT(fabs(model->get_mechanical_power_upper_limit_in_pu_based_on_mbase()-1.5)<FLOAT_EPSILON);
}

void IEEEG3_TEST::test_set_get_mechanical_power_lower_limit()
{
    show_test_information_for_function_of_class(__FUNCTION__,"IEEEG1_TEST");

    IEEEG3* model = (IEEEG3*) get_test_turbine_governor_model();

    model->set_Pmin_in_pu(0.5);
    TEST_ASSERT(fabs(model->get_mechanical_power_lower_limit_in_pu_based_on_mbase()-0.5)<FLOAT_EPSILON);
}
void IEEEG3_TEST::test_step_response()
{
    show_test_information_for_function_of_class(__FUNCTION__,"IEEEG3_TEST");

    default_toolkit.open_log_file("test_log/step_response_of_IEEEG3_model.txt");

    IEEEG3* model = (IEEEG3*) get_test_turbine_governor_model();

    model->set_Pmax_in_pu(1.2);

    run_step_response_of_turbine_govnernor_model();

    default_toolkit.close_log_file();

}

void IEEEG3_TEST::test_get_standard_model_string()
{
    show_test_information_for_function_of_class(__FUNCTION__,"IEEEG3_TEST");
}



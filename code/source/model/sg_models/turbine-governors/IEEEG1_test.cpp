#include "header/model/sg_models/turbine_governor_model/IEEEG1_test.h"
#include "header/basic/utility.h"
using namespace std;

IEEEG1_TEST::IEEEG1_TEST()
{
    TEST_ADD(IEEEG1_TEST::test_get_model_type);
    TEST_ADD(IEEEG1_TEST::test_get_model_name);
    TEST_ADD(IEEEG1_TEST::test_set_get_parameters);
    TEST_ADD(IEEEG1_TEST::test_initialize);
    TEST_ADD(IEEEG1_TEST::test_set_get_mechanical_power_reference);
    TEST_ADD(IEEEG1_TEST::test_set_get_mechanical_power_upper_limit);
    TEST_ADD(IEEEG1_TEST::test_set_get_mechanical_power_lower_limit);
    TEST_ADD(IEEEG1_TEST::test_step_response);

}

void IEEEG1_TEST::setup()
{
    TURBINE_GOVERNOR_MODEL_TEST::setup();

    GENERATOR* genptr = get_test_generator();
    IEEEG1 model;
    model.set_K(20.0);
    model.set_T1_in_s(0.1);
    model.set_T2_in_s(0.5);
    model.set_T3_in_s(0.2);
    model.set_Uo_in_pu(0.1);
    model.set_Uc_in_pu(-0.1);
    model.set_Pmax_in_pu(1.0);
    model.set_Pmin_in_pu(0.0);
    model.set_T4_in_s(0.3);
    model.set_K1(0.3);
    model.set_T5_in_s(7.0);
    model.set_K3(0.3);
    model.set_T6_in_s(1.0);
    model.set_K5(0.4);

    genptr->set_model(&model);
}

void IEEEG1_TEST::tear_down()
{
    TURBINE_GOVERNOR_MODEL_TEST::tear_down();
}

void IEEEG1_TEST::test_get_model_type()
{
    show_test_information_for_function_of_class(__FUNCTION__,"IEEEG1_TEST");

    TURBINE_GOVERNOR_MODEL* model = get_test_turbine_governor_model();

    TEST_ASSERT(model->get_model_type()=="TURBINE GOVERNOR");
}

void IEEEG1_TEST::test_get_model_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"IEEEG1_TEST");

    TURBINE_GOVERNOR_MODEL* model = get_test_turbine_governor_model();

    TEST_ASSERT(model->get_model_name()=="IEEEG1");
}

void IEEEG1_TEST::test_set_get_parameters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"IEEEG1_TEST");

    IEEEG1* model = (IEEEG1*) get_test_turbine_governor_model();

    TEST_ASSERT(fabs(model->get_K()-20.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_T1_in_s()-0.1)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_T2_in_s()-0.5)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_T3_in_s()-0.2)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_Uo_in_pu()-0.1)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_Uc_in_pu()+0.1)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_Pmax_in_pu()-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_Pmin_in_pu()-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_T4_in_s()-0.3)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_K1()-0.3)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_T5_in_s()-7.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_K3()-0.3)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_T6_in_s()-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_K5()-0.4)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_T7_in_s()-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_K2()-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_K4()-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_K6()-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_K7()-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_K8()-0.0)<FLOAT_EPSILON);
}

void IEEEG1_TEST::test_initialize()
{
    show_test_information_for_function_of_class(__FUNCTION__,"IEEEG1_TEST");

    SYNC_GENERATOR_MODEL* genmodel = get_test_sync_generator_model();
    genmodel->initialize();

    double Pmech0 = genmodel->get_initial_mechanical_power_in_pu_based_on_mbase();

    IEEEG1* model = (IEEEG1*) get_test_turbine_governor_model();

    model->set_Pmax_in_pu(1.2);

    model->initialize();
    double Pref = Pmech0;

    TEST_ASSERT(fabs(model->get_mechanical_power_in_pu_based_on_mbase()-Pmech0)<FLOAT_EPSILON);

    TEST_ASSERT(fabs(model->get_mechanical_power_reference_in_pu_based_on_mbase()-Pref)<FLOAT_EPSILON);
}

void IEEEG1_TEST::test_set_get_mechanical_power_reference()
{
    show_test_information_for_function_of_class(__FUNCTION__,"IEEEG1_TEST");

    IEEEG1* model = (IEEEG1*) get_test_turbine_governor_model();

    model->set_initial_mechanical_power_reference_in_pu_based_on_mbase(1.5);
    TEST_ASSERT(fabs(model->get_mechanical_power_reference_in_pu_based_on_mbase()-1.5)<FLOAT_EPSILON);
}

void IEEEG1_TEST::test_set_get_mechanical_power_upper_limit()
{
    show_test_information_for_function_of_class(__FUNCTION__,"IEEEG1_TEST");

    IEEEG1* model = (IEEEG1*) get_test_turbine_governor_model();

    model->set_Pmax_in_pu(1.5);
    TEST_ASSERT(fabs(model->get_mechanical_power_upper_limit_in_pu_based_on_mbase()-1.5)<FLOAT_EPSILON);
}

void IEEEG1_TEST::test_set_get_mechanical_power_lower_limit()
{
    show_test_information_for_function_of_class(__FUNCTION__,"IEEEG1_TEST");

    IEEEG1* model = (IEEEG1*) get_test_turbine_governor_model();

    model->set_Pmin_in_pu(0.5);
    TEST_ASSERT(fabs(model->get_mechanical_power_lower_limit_in_pu_based_on_mbase()-0.5)<FLOAT_EPSILON);
}

void IEEEG1_TEST::test_step_response()
{
    show_test_information_for_function_of_class(__FUNCTION__,"IEEEG1_TEST");

    redirect_stdout_to_file("test_log/step_response_of_IEEEG1_model.txt");

    IEEEG1* model = (IEEEG1*) get_test_turbine_governor_model();

    model->set_Pmax_in_pu(1.2);

    run_step_response_of_turbine_govnernor_model();

    recover_stdout();

}

void IEEEG1_TEST::test_get_standard_model_string()
{
    show_test_information_for_function_of_class(__FUNCTION__,"IEEEG1_TEST");
}



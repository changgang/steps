#include "header/model/turbine_governor_model/IEESGO_test.h"
#include "header/basic/utility.h"
using namespace std;

IEESGO_TEST::IEESGO_TEST()
{
    TEST_ADD(IEESGO_TEST::test_get_model_type);
    TEST_ADD(IEESGO_TEST::test_get_model_name);
    TEST_ADD(IEESGO_TEST::test_set_get_parameters);
    TEST_ADD(IEESGO_TEST::test_initialize);
    TEST_ADD(IEESGO_TEST::test_set_get_mechanical_power_reference);
    TEST_ADD(IEESGO_TEST::test_set_get_mechanical_power_upper_limit);
    TEST_ADD(IEESGO_TEST::test_set_get_mechanical_power_lower_limit);
    TEST_ADD(IEESGO_TEST::test_step_response);

}

void IEESGO_TEST::setup()
{
    TURBINE_GOVERNOR_MODEL_TEST::setup();

    IEESGO model;

    GENERATOR* genptr = get_generator();

    model.set_K1(25.0);
    model.set_K2(0.7);
    model.set_K3(0.3);
    model.set_T1_in_s(0.1);
    model.set_T2_in_s(0.1);
    model.set_T3_in_s(0.2);
    model.set_T4_in_s(0.3);
    model.set_T5_in_s(7.0);
    model.set_T6_in_s(0.5);
    model.set_Pmax_in_pu(1.0);
    model.set_Pmin_in_pu(0.0);

    genptr->set_model(&model);
}

void IEESGO_TEST::tear_down()
{
    TURBINE_GOVERNOR_MODEL_TEST::tear_down();
}

void IEESGO_TEST::test_get_model_type()
{
    show_test_information_for_function_of_class(__FUNCTION__,"IEESGO_TEST");

    GENERATOR* genptr = get_generator();
    TURBINE_GOVERNOR_MODEL* model = genptr->get_turbine_governor_model();

    TEST_ASSERT(model->get_model_type()=="TURBINE GOVERNOR");
}

void IEESGO_TEST::test_get_model_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"IEESGO_TEST");

    GENERATOR* genptr = get_generator();
    TURBINE_GOVERNOR_MODEL* model = genptr->get_turbine_governor_model();

    TEST_ASSERT(model->get_model_name()=="IEESGO");
}

void IEESGO_TEST::test_set_get_parameters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"IEESGO_TEST");

    GENERATOR* genptr = get_generator();
    IEESGO* model = (IEESGO*) genptr->get_turbine_governor_model();

    TEST_ASSERT(fabs(model->get_K1()-25.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_K2()-0.7)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_K3()-0.3)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_T1_in_s()-0.1)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_T2_in_s()-0.1)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_T3_in_s()-0.2)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_T4_in_s()-0.3)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_T5_in_s()-7.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_T6_in_s()-0.5)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_Pmax_in_pu()-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_Pmin_in_pu()-0.0)<FLOAT_EPSILON);
}

void IEESGO_TEST::test_initialize()
{
    show_test_information_for_function_of_class(__FUNCTION__,"IEESGO_TEST");

    GENERATOR* genptr = get_generator();
    SYNC_GENERATOR_MODEL* genmodel = genptr->get_sync_generator_model();
    genmodel->initialize();

    double Pmech0 = genmodel->get_initial_mechanical_power_in_pu_based_on_mbase();

    IEESGO* model = (IEESGO*) genptr->get_turbine_governor_model();

    model->set_Pmax_in_pu(1.2);
    model->initialize();

    TEST_ASSERT(fabs(model->get_mechanical_power_in_pu_based_on_mbase()-Pmech0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_mechanical_power_reference_in_pu_based_on_mbase()-Pmech0)<FLOAT_EPSILON);
}

void IEESGO_TEST::test_set_get_mechanical_power_reference()
{
    show_test_information_for_function_of_class(__FUNCTION__,"IEESGO_TEST");

    GENERATOR* genptr = get_generator();
    IEESGO* model = (IEESGO*) genptr->get_turbine_governor_model();

    model->set_mechanical_power_reference_in_pu_based_on_mbase(1.5);
    TEST_ASSERT(fabs(model->get_mechanical_power_reference_in_pu_based_on_mbase()-1.5)<FLOAT_EPSILON);
}

void IEESGO_TEST::test_set_get_mechanical_power_upper_limit()
{
    show_test_information_for_function_of_class(__FUNCTION__,"IEEEG1_TEST");

    GENERATOR* genptr = get_generator();
    IEESGO* model = (IEESGO*) genptr->get_turbine_governor_model();

    model->set_Pmax_in_pu(1.5);
    TEST_ASSERT(fabs(model->get_mechanical_power_upper_limit_in_pu_based_on_mbase()-1.5)<FLOAT_EPSILON);
}

void IEESGO_TEST::test_set_get_mechanical_power_lower_limit()
{
    show_test_information_for_function_of_class(__FUNCTION__,"IEEEG1_TEST");

    GENERATOR* genptr = get_generator();
    IEESGO* model = (IEESGO*) genptr->get_turbine_governor_model();

    model->set_Pmin_in_pu(0.5);
    TEST_ASSERT(fabs(model->get_mechanical_power_lower_limit_in_pu_based_on_mbase()-0.5)<FLOAT_EPSILON);
}

void IEESGO_TEST::test_step_response()
{
    show_test_information_for_function_of_class(__FUNCTION__,"IEESGO_TEST");

    redirect_stdout_to_file("test_log/step_response_of_IEESGO_model.txt");

    GENERATOR* genptr = get_generator();
    IEESGO* model = (IEESGO*) genptr->get_turbine_governor_model();

    model->set_Pmax_in_pu(1.2);

    run_step_response_of_turbine_govnernor_model();

    recover_stdout();

}

void IEESGO_TEST::test_get_standard_model_string()
{
    show_test_information_for_function_of_class(__FUNCTION__,"IEESGO_TEST");
}



#include "header/basic/test_macro.h"
#include "header/model/sg_models/turbine_governor_model/GAST2A_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

#ifdef ENABLE_STEPS_TEST
using namespace std;

GAST2A_TEST::GAST2A_TEST()
{
    TEST_ADD(GAST2A_TEST::test_get_model_type);
    TEST_ADD(GAST2A_TEST::test_get_model_name);
    TEST_ADD(GAST2A_TEST::test_set_get_parameters);
    TEST_ADD(GAST2A_TEST::test_initialize);
    TEST_ADD(GAST2A_TEST::test_set_get_mechanical_power_reference);
    TEST_ADD(GAST2A_TEST::test_set_get_mechanical_power_upper_limit);
    TEST_ADD(GAST2A_TEST::test_set_get_mechanical_power_lower_limit);
    TEST_ADD(GAST2A_TEST::test_step_response);

}

void GAST2A_TEST::setup()
{
    TURBINE_GOVERNOR_MODEL_TEST::setup();

    GENERATOR* genptr = get_test_generator();
    GAST2A model(default_toolkit);
    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();
    model.set_device_id(genptr->get_device_id());

    model.set_gas_W(20.0);
    model.set_gas_X_in_s(0.1);
    model.set_gas_Y_in_s(0.5);
    model.set_gas_Z(0.2);
    model.set_gas_ETD_in_s(0.1);
    model.set_gas_TCD_in_s(-0.1);
    model.set_gas_Prate_in_MW(1.0);
    model.set_gas_T_in_s(0.0);
    model.set_gas_max_in_pu(0.3);
    model.set_gas_min_in_pu(0.3);
    model.set_gas_ECR_in_s(7.0);
    model.set_gas_K3(0.3);
    model.set_gas_a(1.0);
    model.set_gas_b_in_s(0.4);
    model.set_gas_c(0.4);
    model.set_gas_Kf(0.4);
    model.set_gas_K5(0.4);
    model.set_gas_K4(0.4);
    model.set_gas_T3_in_s(0.4);
    model.set_gas_T4_in_s(0.4);
    model.set_gas_Tt_in_s(0.4);
    model.set_gas_T5_in_s(0.4);
    model.set_gas_af1(0.4);
    model.set_gas_bf1(0.4);
    model.set_gas_af2(0.4);
    model.set_gas_bf2(0.4);
    model.set_gas_cf2(0.4);
    model.set_gas_TR_in_deg(0.4);
    model.set_gas_K6(0.4);
    model.set_gas_TC_in_deg(0.4);

    dmdb.add_model(&model);
}

void GAST2A_TEST::tear_down()
{
    TURBINE_GOVERNOR_MODEL_TEST::tear_down();

    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();
    dmdb.remove_the_last_model();
}

void GAST2A_TEST::test_get_model_type()
{
    show_test_information_for_function_of_class(__FUNCTION__,"GAST2A_TEST");

    TURBINE_GOVERNOR_MODEL* model = get_test_turbine_governor_model();

    TEST_ASSERT(model->get_model_type()=="TURBINE GOVERNOR");
}

void GAST2A_TEST::test_get_model_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"GAST2A_TEST");

    TURBINE_GOVERNOR_MODEL* model = get_test_turbine_governor_model();

    TEST_ASSERT(model->get_model_name()=="GAST2A");
}

void GAST2A_TEST::test_set_get_parameters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"GAST2A_TEST");

    GAST2A* model = (GAST2A*) get_test_turbine_governor_model();

    TEST_ASSERT(fabs(model->get_gas_W()-20.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_gas_X_in_s()-0.1)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_gas_Y_in_s()-0.5)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_gas_Z()-0.2)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_gas_ETD_in_s()-0.1)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_gas_TCD_in_s()+0.1)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_gas_Prate_in_MW()-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_gas_T_in_s()-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_gas_max_in_pu()-0.3)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_gas_min_in_pu()-0.3)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_gas_ECR_in_s()-7.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_gas_K3()-0.3)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_gas_a()-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_gas_b_in_s()-0.4)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_gas_c()-0.4)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_gas_Kf()-0.4)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_gas_K5()-0.4)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_gas_K4()-0.4)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_gas_T3_in_s()-0.4)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_gas_T4_in_s()-0.4)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_gas_T5_in_s()-0.4)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_gas_af1()-0.4)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_gas_bf1()-0.4)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_gas_af2()-0.4)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_gas_bf2()-0.4)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_gas_cf2()-0.4)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_gas_TR_in_deg()-0.4)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_gas_K6()-0.4)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_gas_TC_in_deg()-0.4)<FLOAT_EPSILON);
}

void GAST2A_TEST::test_initialize()
{
    show_test_information_for_function_of_class(__FUNCTION__,"GAST2A_TEST");

    SYNC_GENERATOR_MODEL* genmodel = get_test_sync_generator_model();
    genmodel->initialize();

    double Pmech0 = genmodel->get_initial_mechanical_power_in_pu_based_on_mbase();

    GAST2A* model = (GAST2A*) get_test_turbine_governor_model();

    model->set_Pmax_in_pu(1.2);

    model->initialize();
    double Pref = Pmech0;

    TEST_ASSERT(fabs(model->get_mechanical_power_in_pu_based_on_mbase()-Pmech0)<FLOAT_EPSILON);

    TEST_ASSERT(fabs(model->get_mechanical_power_reference_in_pu_based_on_mbase()-Pref)<FLOAT_EPSILON);
}

void GAST2A_TEST::test_set_get_mechanical_power_reference()
{
    show_test_information_for_function_of_class(__FUNCTION__,"GAST2A_TEST");

    GAST2A* model = (GAST2A*) get_test_turbine_governor_model();

    model->set_initial_mechanical_power_reference_in_pu_based_on_mbase(1.5);
    TEST_ASSERT(fabs(model->get_mechanical_power_reference_in_pu_based_on_mbase()-1.5)<FLOAT_EPSILON);
}

void GAST2A_TEST::test_set_get_mechanical_power_upper_limit()
{
    show_test_information_for_function_of_class(__FUNCTION__,"GAST2A_TEST");

    GAST2A* model = (GAST2A*) get_test_turbine_governor_model();

    model->set_Pmax_in_pu(1.5);
    TEST_ASSERT(fabs(model->get_mechanical_power_upper_limit_in_pu_based_on_mbase()-1.5)<FLOAT_EPSILON);
}

void GAST2A_TEST::test_set_get_mechanical_power_lower_limit()
{
    show_test_information_for_function_of_class(__FUNCTION__,"GAST2A_TEST");

    GAST2A* model = (GAST2A*) get_test_turbine_governor_model();

    model->set_Pmin_in_pu(0.5);
    TEST_ASSERT(fabs(model->get_mechanical_power_lower_limit_in_pu_based_on_mbase()-0.5)<FLOAT_EPSILON);
}

void GAST2A_TEST::test_step_response()
{
    show_test_information_for_function_of_class(__FUNCTION__,"GAST2A_TEST");

    default_toolkit.open_log_file("test_log/step_response_of_GAST2A_model.txt");

    GAST2A* model = (GAST2A*) get_test_turbine_governor_model();

    model->set_Pmax_in_pu(1.2);

    run_step_response_of_turbine_govnernor_model();

    default_toolkit.close_log_file();

}

void GAST2A_TEST::test_get_standard_psse_string()
{
    show_test_information_for_function_of_class(__FUNCTION__,"GAST2A_TEST");
}

#endif

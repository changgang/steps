#include "header/basic/test_macro.h"
#include "header/model/sg_models/turbine_load_controller_model/turbine_load_controller_model_test.h"
#include "header/model/sg_models/sync_generator_model/gencls.h"
#include "header/model/sg_models/turbine_governor_model/IEEEG1.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

#ifdef ENABLE_STEPS_TEST
using namespace std;
TURBINE_LOAD_CONTROLLER_MODEL_TEST::TURBINE_LOAD_CONTROLLER_MODEL_TEST()
{
    TEST_ADD(TURBINE_LOAD_CONTROLLER_MODEL_TEST::test_get_model_type);
    TEST_ADD(TURBINE_LOAD_CONTROLLER_MODEL_TEST::test_get_initial_mechanical_power_reference);
    TEST_ADD(TURBINE_LOAD_CONTROLLER_MODEL_TEST::test_get_terminal_active_power);
    TEST_ADD(TURBINE_LOAD_CONTROLLER_MODEL_TEST::test_get_mechanical_power_reference);
}



void TURBINE_LOAD_CONTROLLER_MODEL_TEST::setup()
{
    SG_MODEL_TEST::setup();

    GENERATOR* genptr = get_test_generator();

    GENCLS sg_model;
    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();
    sg_model.set_toolkit(default_toolkit);
    sg_model.set_device_id(genptr->get_device_id());

    sg_model.set_H_in_s(3.0);
    sg_model.set_D(2.0);

    dmdb.add_model(&sg_model);

    IEEEG1 tg_model;
    tg_model.set_toolkit(default_toolkit);
    tg_model.set_device_id(genptr->get_device_id());

    tg_model.set_K(20.0);
    tg_model.set_T1_in_s(0.1);
    tg_model.set_T2_in_s(0.5);
    tg_model.set_T3_in_s(0.2);
    tg_model.set_Uo_in_pu(0.1);
    tg_model.set_Uc_in_pu(-0.1);
    tg_model.set_Pmax_in_pu(1.0);
    tg_model.set_Pmin_in_pu(0.0);
    tg_model.set_T4_in_s(0.3);
    tg_model.set_K1(0.3);
    tg_model.set_T5_in_s(7.0);
    tg_model.set_K3(0.3);
    tg_model.set_T6_in_s(1.0);
    tg_model.set_K5(0.4);

    dmdb.add_model(&tg_model);

}

void TURBINE_LOAD_CONTROLLER_MODEL_TEST::tear_down()
{
    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();
    dmdb.remove_the_last_model();
    dmdb.remove_the_last_model();

    SG_MODEL_TEST::tear_down();
    show_test_end_information();
}

void TURBINE_LOAD_CONTROLLER_MODEL_TEST::test_get_model_type()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TURBINE_LOAD_CONTROLLER_MODEL_TEST");

    TURBINE_LOAD_CONTROLLER_MODEL* model = get_test_turbine_load_controller_model();
    TEST_ASSERT(model->get_model_type()=="TURBINE LOAD CONTROLLER");
}

void TURBINE_LOAD_CONTROLLER_MODEL_TEST::test_get_initial_mechanical_power_reference()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TURBINE_LOAD_CONTROLLER_MODEL_TEST");

    TURBINE_GOVERNOR_MODEL* tg_model = get_test_turbine_governor_model();
    if(not tg_model->is_model_initialized())
        tg_model->initialize();

    TURBINE_LOAD_CONTROLLER_MODEL* model = get_test_turbine_load_controller_model();
    TEST_ASSERT(fabs(model->get_initial_mechanical_power_reference_in_pu_based_on_mbase_from_turbine_governor_model()-
                     tg_model->get_initial_mechanical_power_reference_in_pu_based_on_mbase())<FLOAT_EPSILON);
}

void TURBINE_LOAD_CONTROLLER_MODEL_TEST::test_get_terminal_active_power()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TURBINE_LOAD_CONTROLLER_MODEL_TEST");
    SYNC_GENERATOR_MODEL* sg_model = get_test_sync_generator_model();
    if(not sg_model->is_model_initialized())
        sg_model->initialize();
    TURBINE_LOAD_CONTROLLER_MODEL* model = get_test_turbine_load_controller_model();
    TEST_ASSERT(fabs(model->get_terminal_active_power_in_pu_based_on_mbase_from_generator_model()-
                     sg_model->get_terminal_active_power_in_pu_based_on_mbase())<FLOAT_EPSILON);
}

void TURBINE_LOAD_CONTROLLER_MODEL_TEST::test_get_mechanical_power_reference()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TURBINE_LOAD_CONTROLLER_MODEL_TEST");

    TURBINE_LOAD_CONTROLLER_MODEL* model = get_test_turbine_load_controller_model();
    model->initialize();

    TEST_ASSERT(fabs(model->get_mechanical_power_reference_in_pu_based_on_mbase()-0.5)<FLOAT_EPSILON);
}


#endif

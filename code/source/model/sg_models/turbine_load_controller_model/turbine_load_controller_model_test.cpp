#include "header/model/sg_models/turbine_load_controller_model/turbine_load_controller_model_test.h"
#include "header/model/sg_models/turbine_governor_model/IEEEG1.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
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

    IEEEG1 tg_model;

    genptr->set_model(&tg_model);

}

void TURBINE_LOAD_CONTROLLER_MODEL_TEST::tear_down()
{
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
    TURBINE_LOAD_CONTROLLER_MODEL* model = get_test_turbine_load_controller_model();
    TEST_ASSERT(fabs(model->get_initial_mechanical_power_reference_in_pu_based_on_mbase_from_turbine_governor_model()-
                     get_test_turbine_governor_model()->get_initial_mechanical_power_reference_in_pu_based_on_mbase())<FLOAT_EPSILON);
}

void TURBINE_LOAD_CONTROLLER_MODEL_TEST::test_get_terminal_active_power()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TURBINE_LOAD_CONTROLLER_MODEL_TEST");
    TURBINE_LOAD_CONTROLLER_MODEL* model = get_test_turbine_load_controller_model();
    TEST_ASSERT(fabs(model->get_terminal_active_power_in_pu_based_on_mbase_from_generator_model()-
                     get_test_sync_generator_model()->get_terminal_active_power_in_pu_based_on_mbase())<FLOAT_EPSILON);
}

void TURBINE_LOAD_CONTROLLER_MODEL_TEST::test_get_mechanical_power_reference()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TURBINE_LOAD_CONTROLLER_MODEL_TEST");

    TURBINE_LOAD_CONTROLLER_MODEL* model = get_test_turbine_load_controller_model();
    TEST_ASSERT(fabs(model->get_mechanical_power_reference_in_pu_based_on_mbase()-0.5)<FLOAT_EPSILON);
}

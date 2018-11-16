#include "header/model/sg_models/turbine_load_controller_model/lcfb1_test.h"
#include "header/basic/utility.h"
using namespace std;

LCFB1_TEST::LCFB1_TEST() : TURBINE_LOAD_CONTROLLER_MODEL_TEST()
{
    TEST_ADD(LCFB1_TEST::test_get_model_name);
    TEST_ADD(LCFB1_TEST::test_set_get_parameters);
}

void LCFB1_TEST::setup()
{
    TURBINE_LOAD_CONTROLLER_MODEL_TEST::setup();

    GENERATOR* genptr = get_test_generator();
    LCFB1 model;
    genptr->set_model(&model);
}

void LCFB1_TEST::tear_down()
{
    TURBINE_LOAD_CONTROLLER_MODEL_TEST::tear_down();
}


void LCFB1_TEST::test_get_model_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LCFB1_TEST");

    LCFB1* model = (LCFB1*) get_test_turbine_load_controller_model();
    TEST_ASSERT(model->get_model_name()=="LCFB1");
}

void LCFB1_TEST::test_set_get_parameters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LCFB1_TEST");

    LCFB1* model = (LCFB1*) get_test_turbine_load_controller_model();

    model->set_frequency_regulation_flag(true);
    TEST_ASSERT(model->get_frequency_regulation_flag()==true);
    model->set_frequency_regulation_flag(false);
    TEST_ASSERT(model->get_frequency_regulation_flag()==false);

    model->set_power_regulation_flag(true);
    TEST_ASSERT(model->get_power_regulation_flag()==true);
    model->set_power_regulation_flag(false);
    TEST_ASSERT(model->get_power_regulation_flag()==false);

    model->set_Fb(0.2);
    TEST_ASSERT(fabs(model->get_Fb()-0.2)<FLOAT_EPSILON);

    model->set_Tpelec_in_s(0.1);
    TEST_ASSERT(fabs(model->get_Tpelec_in_s()-0.1)<FLOAT_EPSILON);

    model->set_db(0.3);
    TEST_ASSERT(fabs(model->get_db()-0.3)<FLOAT_EPSILON);

    model->set_Emax(1.0);
    TEST_ASSERT(fabs(model->get_Emax()-1.0)<FLOAT_EPSILON);

    model->set_Kp(10.0);
    TEST_ASSERT(fabs(model->get_Kp()-10.0)<FLOAT_EPSILON);

    model->set_Ki(20.0);
    TEST_ASSERT(fabs(model->get_Ki()-20.0)<FLOAT_EPSILON);

    model->set_Irmax(5.0);
    TEST_ASSERT(fabs(model->get_Irmax()-5.0)<FLOAT_EPSILON);
}

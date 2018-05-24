#include "header/model/sg_models/stabilizer_model/stabilizer_model_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
using namespace std;
STABILIZER_MODEL_TEST::STABILIZER_MODEL_TEST()
{
    TEST_ADD(STABILIZER_MODEL_TEST::test_get_model_type);
    TEST_ADD(STABILIZER_MODEL_TEST::test_set_get_input_signal);
    TEST_ADD(STABILIZER_MODEL_TEST::test_get_stabilizing_signal);
}



void STABILIZER_MODEL_TEST::setup()
{
    SG_MODEL_TEST::setup();

    GENERATOR* genptr = get_test_generator();
    POWER_SYSTEM_DATABASE* psdb = get_test_power_system_database();

    SEXS exciter_model;
    exciter_model.set_power_system_database(psdb);

    genptr->set_model(&exciter_model);

}

void STABILIZER_MODEL_TEST::tear_down()
{
    SG_MODEL_TEST::tear_down();
    show_test_end_information();
}

void STABILIZER_MODEL_TEST::test_get_model_type()
{
    show_test_information_for_function_of_class(__FUNCTION__,"STABILIZER_MODEL_TEST");

    STABILIZER_MODEL* model = get_test_stabilizer_model();
    TEST_ASSERT(model->get_model_type()=="STABILIZER");
}

void STABILIZER_MODEL_TEST::test_set_get_input_signal()
{
    show_test_information_for_function_of_class(__FUNCTION__,"STABILIZER_MODEL_TEST");

    GENERATOR* generator = get_test_generator();
    POWER_SYSTEM_DATABASE* psdb = get_test_power_system_database();

    SIGNAL signal(psdb);
    signal.set_device_id(generator->get_device_id());
    signal.set_meter_type("GENERATOR ROTOR SPEED DEVIATION IN PU");

    STABILIZER_MODEL* model = get_test_stabilizer_model();
    model->set_input_signal_at_slot(0, signal);
    SIGNAL signal2 = model->get_input_signal_at_slot(0);
    TEST_ASSERT(signal2==signal);
}

void STABILIZER_MODEL_TEST::test_get_stabilizing_signal()
{
    show_test_information_for_function_of_class(__FUNCTION__,"STABILIZER_MODEL_TEST");

    STABILIZER_MODEL* model = get_test_stabilizer_model();
    TEST_ASSERT(fabs(model->get_stabilizing_signal_in_pu()-0.0)<FLOAT_EPSILON);
}




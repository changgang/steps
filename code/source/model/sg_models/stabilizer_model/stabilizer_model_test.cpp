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

    DEVICE_ID did;
    did.set_device_type("GENERATOR");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    model = new IEE2ST();
    model->set_power_system_database(db);
    model->set_device_id(did);

    exciter_model = new SEXS();
    exciter_model->set_power_system_database(db);
    exciter_model->set_device_id(did);
}

void STABILIZER_MODEL_TEST::tear_down()
{
    SG_MODEL_TEST::tear_down();
    delete model;
    delete exciter_model;

    show_test_end_information();
}


EXCITER_MODEL* STABILIZER_MODEL_TEST::get_exciter_model();
STABILIZER_MODEL* STABILIZER_MODEL_TEST::get_stabilizer_model();

void STABILIZER_MODEL_TEST::test_get_model_type()
{
    show_test_information_for_function_of_class(__FUNCTION__,"STABILIZER_MODEL_TEST");

    TEST_ASSERT(model->get_model_type()=="STABILIZER");
}

void STABILIZER_MODEL_TEST::test_set_get_input_signal()
{
    show_test_information_for_function_of_class(__FUNCTION__,"STABILIZER_MODEL_TEST");

    GENERATOR* generator = (GENERATOR*) model->get_device_pointer();

    SIGNAL signal(db);
    signal.set_device_id(generator->get_device_id());
    signal.set_meter_type("GENERATOR ROTOR SPEED DEVIATION IN PU");

    model->set_input_signal_at_slot(0, signal);
    SIGNAL signal2 = model->get_input_signal_at_slot(0);
    TEST_ASSERT(signal2==signal);
}

void STABILIZER_MODEL_TEST::test_get_stabilizing_signal()
{
    show_test_information_for_function_of_class(__FUNCTION__,"STABILIZER_MODEL_TEST");

    TEST_ASSERT(fabs(model->get_stabilizing_signal_in_pu()-0.0)<FLOAT_EPSILON);
}




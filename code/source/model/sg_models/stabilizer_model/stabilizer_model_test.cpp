#include "header/basic/test_macro.h"
#include "header/model/sg_models/stabilizer_model/stabilizer_model_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

#ifdef ENABLE_STEPS_TEST
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

    SEXS exciter_model(default_toolkit);
    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();
    exciter_model.set_device_id(genptr->get_device_id());

    dmdb.add_model(&exciter_model);
}

void STABILIZER_MODEL_TEST::tear_down()
{
    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();
    dmdb.remove_the_last_model();

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

    cout<<__FILE__<<__LINE__<<endl;
    GENERATOR* generator = get_test_generator();

    cout<<__FILE__<<__LINE__<<endl;
    SIGNAL signal(default_toolkit);

    cout<<__FILE__<<__LINE__<<endl;
    signal.set_device_id(generator->get_device_id());
    signal.set_meter_type("ROTOR SPEED DEVIATION IN PU");

    cout<<__FILE__<<__LINE__<<endl;
    STABILIZER_MODEL* model = get_test_stabilizer_model();
    cout<<__FILE__<<__LINE__<<model<<endl;
    model->initialize();
    cout<<__FILE__<<__LINE__<<endl;
    model->set_input_signal_at_slot(0, signal);
    SIGNAL signal2 = model->get_input_signal_at_slot(0);
    TEST_ASSERT(signal2==signal);
}

void STABILIZER_MODEL_TEST::test_get_stabilizing_signal()
{
    show_test_information_for_function_of_class(__FUNCTION__,"STABILIZER_MODEL_TEST");

    cout<<__FILE__<<__LINE__<<endl;
    STABILIZER_MODEL* model = get_test_stabilizer_model();
    cout<<__FILE__<<__LINE__<<model<<endl;
    model->initialize();
    cout<<__FILE__<<__LINE__<<endl;
    TEST_ASSERT(fabs(model->get_stabilizing_signal_in_pu()-0.0)<FLOAT_EPSILON);
    cout<<__FILE__<<__LINE__<<endl;
}

#endif

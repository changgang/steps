#include "header/basic/test_macro.h"
#include "header/model/sg_models/compensator_model/PSASPVC_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

#ifdef ENABLE_STEPS_TEST
using namespace std;
PSASPVC_TEST::PSASPVC_TEST()
{
    TEST_ADD(PSASPVC_TEST::test_get_model_type);
    TEST_ADD(PSASPVC_TEST::test_get_model_name);
    TEST_ADD(PSASPVC_TEST::test_set_get_parameters);
    TEST_ADD(PSASPVC_TEST::test_initialize);
    TEST_ADD(PSASPVC_TEST::test_step_response);
    TEST_ADD(PSASPVC_TEST::test_get_standard_psse_string);
}



void PSASPVC_TEST::setup()
{
    COMPENSATOR_MODEL_TEST::setup();

    GENERATOR* generator  = get_test_generator();

    PSASPVC model;
    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();
    model.set_toolkit(default_toolkit);
    model.set_device_id(generator->get_device_id());

    model.set_Xc(0.1);
    dmdb.add_model(&model);
}

void PSASPVC_TEST::tear_down()
{
    COMPENSATOR_MODEL_TEST::tear_down();

    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();
    dmdb.remove_the_last_model();
}


void PSASPVC_TEST::test_get_model_type()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PSASPVC_TEST");

    COMPENSATOR_MODEL* model = get_test_compensator_model();

    TEST_ASSERT(model->get_model_type()=="PSASPVCENSATOR");
}
void PSASPVC_TEST::test_get_model_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PSASPVC_TEST");

    COMPENSATOR_MODEL* model = get_test_compensator_model();

    TEST_ASSERT(model->get_model_name()=="PSASPVC");
}



void PSASPVC_TEST::test_set_get_parameters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PSASPVC_TEST");

    PSASPVC* model = (PSASPVC*) get_test_compensator_model();

    model->set_Xc(0.1);
    TEST_ASSERT(fabs(model->get_Xc()-0.1)<FLOAT_EPSILON);
}


void PSASPVC_TEST::test_initialize()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PSASPVC_TEST");

    PSASPVC* model = (PSASPVC*) get_test_compensator_model();

    model->set_Xc(0.1);

    SYNC_GENERATOR_MODEL* genmodel = get_test_sync_generator_model();
    genmodel->initialize();

    model->initialize();

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    complex<double> V = psdb.get_bus_positive_sequence_complex_voltage_in_pu(1);
    complex<double> I = genmodel->get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase();

    double ecomp = steps_fast_complex_abs(V - 0.1*I*complex<double>(0,1.0));

    TEST_ASSERT(fabs(model->get_compensated_voltage_in_pu()-ecomp)<FLOAT_EPSILON);
}
void PSASPVC_TEST::test_step_response()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PSASPVC_TEST");

    default_toolkit.open_log_file("test_log/step_response_of_PSASPVC_model.txt");

    run_step_response_of_compensator_model();

    default_toolkit.close_log_file();
}
void PSASPVC_TEST::test_get_standard_psse_string()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PSASPVC_TEST");

}


#endif

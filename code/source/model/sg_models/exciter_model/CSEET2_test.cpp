#include "header/model/sg_models/exciter_model/CSEET2_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

using namespace std;
CSEET2_TEST::CSEET2_TEST() : EXCITER_MODEL_TEST()
{
    TEST_ADD(CSEET2_TEST::test_get_model_name);
    TEST_ADD(CSEET2_TEST::test_set_get_parameters);
    TEST_ADD(CSEET2_TEST::test_initialize);
    TEST_ADD(CSEET2_TEST::test_step_response_SEPARATE_EXCIATION_and_stabilizer_AT_VOLTAGE_ERROR);
    TEST_ADD(CSEET2_TEST::test_step_response_SEPARATE_EXCIATION_and_stabilizer_AT_REGULATOR);
    TEST_ADD(CSEET2_TEST::test_step_response_SELF_EXCIATION_and_stabilizer_AT_VOLTAGE_ERROR);
    TEST_ADD(CSEET2_TEST::test_step_response_SELF_EXCIATION_and_stabilizer_AT_REGULATOR);
}



void CSEET2_TEST::setup()
{
    EXCITER_MODEL_TEST::setup();

    CSEET2 model;

    GENERATOR* genptr = get_test_generator();

    model.set_excitation_source(SELF_EXCITATION);
    model.set_stabilizer_slot(AT_VOLTAGE_ERROR);
    model.set_tuner_type(SERIAL_TUNER);

    model.set_TR_in_s(0.02);
    model.set_serial_tuner_KV(true);
    model.set_serial_tuner_K(25);
    model.set_serial_tuner_T1_in_s(1.0);
    model.set_serial_tuner_T2_in_s(10.0);
    model.set_serial_tuner_VA1max_in_pu(5.0);
    model.set_serial_tuner_VA1min_in_pu(-5.0);
    model.set_serial_tuner_T3_in_s(0.3);
    model.set_serial_tuner_T4_in_s(0.03);
    model.set_KA(1.0);
    model.set_TA_in_s(0.03);
    model.set_VAmax_in_pu(10.0);
    model.set_VAmin_in_pu(-10.0);
    model.set_KF(1.0);
    model.set_TF_in_s(1.0);
    model.set_VRmax_in_pu(20.0);
    model.set_VRmin_in_pu(-15.0);
    model.set_KC(0.1);

    genptr->set_model(&model);
}

void CSEET2_TEST::tear_down()
{
    EXCITER_MODEL_TEST::tear_down();
}

void CSEET2_TEST::test_get_model_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CSEET2_TEST");

    EXCITER_MODEL* model = get_test_exciter_model();

    TEST_ASSERT(model->get_model_name()=="CSEET2");
}

void CSEET2_TEST::test_set_get_parameters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CSEET2_TEST");

    CSEET2* model = (CSEET2*) get_test_exciter_model();

    TEST_ASSERT(model->get_excitation_source()==SELF_EXCITATION);
    TEST_ASSERT(model->get_stabilizer_slot()==AT_VOLTAGE_ERROR);
    TEST_ASSERT(model->get_tuner_type()==SERIAL_TUNER);

    TEST_ASSERT(fabs(model->get_TR_in_s()-0.02)<FLOAT_EPSILON);
    TEST_ASSERT(model->get_serial_tuner_KV()==true);
    TEST_ASSERT(fabs(model->get_serial_tuner_K()-25)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_serial_tuner_T1_in_s()-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_serial_tuner_T2_in_s()-10.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_serial_tuner_VA1max_in_pu()-5.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_serial_tuner_VA1min_in_pu()-(-5.0))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_serial_tuner_T3_in_s()-0.3)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_serial_tuner_T4_in_s()-0.03)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_KA()-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_TA_in_s()-0.03)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_VAmax_in_pu()-10.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_VAmin_in_pu()-(-10.0))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_KF()-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_TF_in_s()-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_VRmax_in_pu()-20.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_VRmin_in_pu()-(-15.0))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_KC()-0.1)<FLOAT_EPSILON);
}

void CSEET2_TEST::test_initialize()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CSEET2_TEST");

    GENERATOR* genptr = get_test_generator();
    SYNC_GENERATOR_MODEL* genmodel = genptr->get_sync_generator_model();
    genmodel->initialize();

    CSEET2* model = (CSEET2*) get_test_exciter_model();

    double Efd0 = genmodel->get_initial_excitation_voltage_in_pu();
    double Ecomp = model->get_compensated_voltage_in_pu();
    double Ifd = genmodel->get_field_current_in_pu_based_on_mbase();
    double Kc = model->get_KC();

    model->initialize();

    TEST_ASSERT(fabs(model->get_excitation_voltage_in_pu()-Efd0)<FLOAT_EPSILON);
    double Vref;
    if(model->get_serial_tuner_KV()==true)
    {
        double Vr = Efd0+Kc*Ifd;
        Vr = Vr/model->get_KA();
        Vr = Vr/model->get_serial_tuner_K();
        Vref = Ecomp+Vr;
    }
    else
        Vref = Ecomp;

    TEST_ASSERT(fabs(model->get_voltage_reference_in_pu()-Vref)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_stabilizing_signal_in_pu()-0.0)<FLOAT_EPSILON);
}

void CSEET2_TEST::test_step_response_SEPARATE_EXCIATION_and_stabilizer_AT_VOLTAGE_ERROR()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CSEET2_TEST");

    CSEET2* model = (CSEET2*) get_test_exciter_model();

    model->set_excitation_source(SEPARATE_EXCITATION);
    model->set_stabilizer_slot(AT_VOLTAGE_ERROR);

    default_toolkit.redirect_stdout_to_file("test_log/step_response_of_CSEET2_model_1.txt");

    run_step_response_of_exciter_model();

    default_toolkit.recover_stdout();
}
void CSEET2_TEST::test_step_response_SEPARATE_EXCIATION_and_stabilizer_AT_REGULATOR()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CSEET2_TEST");

    CSEET2* model = (CSEET2*) get_test_exciter_model();

    model->set_excitation_source(SEPARATE_EXCITATION);
    model->set_stabilizer_slot(AT_REGULATOR);

    default_toolkit.redirect_stdout_to_file("test_log/step_response_of_CSEET2_model_2.txt");

    run_step_response_of_exciter_model();

    default_toolkit.recover_stdout();
}
void CSEET2_TEST::test_step_response_SELF_EXCIATION_and_stabilizer_AT_VOLTAGE_ERROR()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CSEET2_TEST");

    CSEET2* model = (CSEET2*) get_test_exciter_model();

    model->set_excitation_source(SELF_EXCITATION);
    model->set_stabilizer_slot(AT_VOLTAGE_ERROR);

    default_toolkit.redirect_stdout_to_file("test_log/step_response_of_CSEET2_model_3.txt");

    run_step_response_of_exciter_model();

    default_toolkit.recover_stdout();
}
void CSEET2_TEST::test_step_response_SELF_EXCIATION_and_stabilizer_AT_REGULATOR()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CSEET2_TEST");

    CSEET2* model = (CSEET2*) get_test_exciter_model();

    model->set_excitation_source(SELF_EXCITATION);
    model->set_stabilizer_slot(AT_REGULATOR);

    default_toolkit.redirect_stdout_to_file("test_log/step_response_of_CSEET2_model_4.txt");

    run_step_response_of_exciter_model();

    default_toolkit.recover_stdout();
}


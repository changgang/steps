#include "header/model/sg_models/exciter_model/CSEET1_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

using namespace std;
CSEET1_TEST::CSEET1_TEST()
{
    TEST_ADD(CSEET1_TEST::test_get_model_name);
    TEST_ADD(CSEET1_TEST::test_set_get_parameters);
    TEST_ADD(CSEET1_TEST::test_initialize);
    TEST_ADD(CSEET1_TEST::test_step_response_SEPARATE_EXCIATION_and_WITH_BRUSH_and_feedbacker_AT_VOLTAGE_ERROR);
    TEST_ADD(CSEET1_TEST::test_step_response_SEPARATE_EXCIATION_and_WITH_BRUSH_and_feedbacker_AT_REGULATOR);
    TEST_ADD(CSEET1_TEST::test_step_response_SEPARATE_EXCIATION_and_WITHOUT_BRUSH_and_feedbacker_AT_VOLTAGE_ERROR);
    TEST_ADD(CSEET1_TEST::test_step_response_SEPARATE_EXCIATION_and_WITHOUT_BRUSH_and_feedbacker_AT_REGULATOR);
    TEST_ADD(CSEET1_TEST::test_step_response_SELF_EXCIATION_and_WITH_BRUSH_and_feedbacker_AT_VOLTAGE_ERROR);
    TEST_ADD(CSEET1_TEST::test_step_response_SELF_EXCIATION_and_WITH_BRUSH_and_feedbacker_AT_REGULATOR);
    TEST_ADD(CSEET1_TEST::test_step_response_SELF_EXCIATION_and_WITHOUT_BRUSH_and_feedbacker_AT_VOLTAGE_ERROR);
    TEST_ADD(CSEET1_TEST::test_step_response_SELF_EXCIATION_and_WITHOUT_BRUSH_and_feedbacker_AT_REGULATOR);
}



void CSEET1_TEST::setup()
{
    EXCITER_MODEL_TEST::setup();

    CSEET1 model;

    GENERATOR* genptr = get_test_generator();
    model.set_power_system_database(genptr->get_power_system_database());
    model.set_device_id(genptr->get_device_id());

    model.set_excitation_source(SELF_EXCITATION);
    model.set_exciter_brush(WITHOUT_BRUSH);
    model.set_feedback_slot(AT_VOLTAGE_ERROR);
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
    model.set_KH(0.05);
    model.set_KF(1.0);
    model.set_TF_in_s(1.0);
    model.set_KB(70.0);
    model.set_T5_in_s(0.01);
    model.set_VRmax_in_pu(20.0);
    model.set_VRmin_in_pu(-15.0);
    model.set_KE(1.0);
    model.set_TE_in_s(1.2);
    model.set_Vemax_in_pu(7.0);
    model.set_SE75_in_pu(0.01);
    model.set_SE100_in_pu(0.5);
    model.set_KC(0.1);
    model.set_KD(2.0);
    model.set_Efdmax_in_pu(6.0);

    genptr->set_model(&model);
}

void CSEET1_TEST::tear_down()
{
    EXCITER_MODEL_TEST::tear_down();
}

void CSEET1_TEST::test_get_model_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CSEET1_TEST");

    EXCITER_MODEL* model = get_test_exciter_model();

    TEST_ASSERT(model->get_model_name()=="CSEET1");
}

void CSEET1_TEST::test_set_get_parameters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CSEET1_TEST");

    CSEET1* model = (CSEET1*) get_test_exciter_model();

    TEST_ASSERT(model->get_excitation_source()==SELF_EXCITATION);
    TEST_ASSERT(model->get_exciter_brush()==WITHOUT_BRUSH);
    TEST_ASSERT(model->get_feedback_slot()==AT_VOLTAGE_ERROR);
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
    TEST_ASSERT(fabs(model->get_KH()-0.05)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_KF()-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_TF_in_s()-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_KB()-70.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_T5_in_s()-0.01)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_VRmax_in_pu()-20.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_VRmin_in_pu()-(-15.0))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_KE()-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_TE_in_s()-1.2)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_Vemax_in_pu()-7.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_SE75_in_pu()-0.01)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_SE100_in_pu()-0.5)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_KC()-0.1)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_KD()-2.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_Efdmax_in_pu()-6.0)<FLOAT_EPSILON);
}

void CSEET1_TEST::test_initialize()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CSEET1_TEST");

    GENERATOR* genptr = get_test_generator();
    CSEET1* model = (CSEET1*) get_test_exciter_model();

    model->set_SE75_in_pu(0.0);
    model->set_SE100_in_pu(0.0);

    SYNC_GENERATOR_MODEL* genmodel = genptr->get_sync_generator_model();
    genmodel->initialize();

    double Efd0 = genmodel->get_initial_excitation_voltage_in_pu();
    double Ecomp = model->get_compensated_voltage_in_pu();

    model->initialize();

    TEST_ASSERT(fabs(model->get_excitation_voltage_in_pu()-Efd0)<FLOAT_EPSILON);
    //double Vref = Ecomp;
    //TEST_ASSERT(fabs(model->get_voltage_reference_in_pu()-Vref)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_stabilizing_signal_in_pu()-0.0)<FLOAT_EPSILON);
}

void CSEET1_TEST::test_step_response_SEPARATE_EXCIATION_and_WITH_BRUSH_and_feedbacker_AT_VOLTAGE_ERROR()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CSEET1_TEST");

    CSEET1* model = (CSEET1*) get_test_exciter_model();

    model->set_excitation_source(SEPARATE_EXCITATION);
    model->set_exciter_brush(WITH_BRUSH);
    model->set_feedback_slot(AT_VOLTAGE_ERROR);

    redirect_stdout_to_file("test_log/step_response_of_CSEET1_model_1.txt");

    run_step_response_of_exciter_model();

    recover_stdout();
}
void CSEET1_TEST::test_step_response_SEPARATE_EXCIATION_and_WITH_BRUSH_and_feedbacker_AT_REGULATOR()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CSEET1_TEST");

    CSEET1* model = (CSEET1*) get_test_exciter_model();

    model->set_excitation_source(SEPARATE_EXCITATION);
    model->set_exciter_brush(WITH_BRUSH);
    model->set_feedback_slot(AT_REGULATOR);

    redirect_stdout_to_file("test_log/step_response_of_CSEET1_model_2.txt");

    run_step_response_of_exciter_model();

    recover_stdout();
}
void CSEET1_TEST::test_step_response_SEPARATE_EXCIATION_and_WITHOUT_BRUSH_and_feedbacker_AT_VOLTAGE_ERROR()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CSEET1_TEST");

    CSEET1* model = (CSEET1*) get_test_exciter_model();

    model->set_excitation_source(SEPARATE_EXCITATION);
    model->set_exciter_brush(WITHOUT_BRUSH);
    model->set_feedback_slot(AT_VOLTAGE_ERROR);

    redirect_stdout_to_file("test_log/step_response_of_CSEET1_model_3.txt");

    run_step_response_of_exciter_model();

    recover_stdout();
}
void CSEET1_TEST::test_step_response_SEPARATE_EXCIATION_and_WITHOUT_BRUSH_and_feedbacker_AT_REGULATOR()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CSEET1_TEST");

    CSEET1* model = (CSEET1*) get_test_exciter_model();

    model->set_excitation_source(SEPARATE_EXCITATION);
    model->set_exciter_brush(WITHOUT_BRUSH);
    model->set_feedback_slot(AT_REGULATOR);

    redirect_stdout_to_file("test_log/step_response_of_CSEET1_model_4.txt");

    run_step_response_of_exciter_model();

    recover_stdout();
}
void CSEET1_TEST::test_step_response_SELF_EXCIATION_and_WITH_BRUSH_and_feedbacker_AT_VOLTAGE_ERROR()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CSEET1_TEST");

    CSEET1* model = (CSEET1*) get_test_exciter_model();

    model->set_excitation_source(SELF_EXCITATION);
    model->set_exciter_brush(WITH_BRUSH);
    model->set_feedback_slot(AT_VOLTAGE_ERROR);

    redirect_stdout_to_file("test_log/step_response_of_CSEET1_model_5.txt");

    run_step_response_of_exciter_model();

    recover_stdout();
}
void CSEET1_TEST::test_step_response_SELF_EXCIATION_and_WITH_BRUSH_and_feedbacker_AT_REGULATOR()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CSEET1_TEST");

    CSEET1* model = (CSEET1*) get_test_exciter_model();

    model->set_excitation_source(SELF_EXCITATION);
    model->set_exciter_brush(WITH_BRUSH);
    model->set_feedback_slot(AT_REGULATOR);

    redirect_stdout_to_file("test_log/step_response_of_CSEET1_model_6.txt");

    run_step_response_of_exciter_model();

    recover_stdout();
}
void CSEET1_TEST::test_step_response_SELF_EXCIATION_and_WITHOUT_BRUSH_and_feedbacker_AT_VOLTAGE_ERROR()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CSEET1_TEST");

    CSEET1* model = (CSEET1*) get_test_exciter_model();

    model->set_excitation_source(SELF_EXCITATION);
    model->set_exciter_brush(WITHOUT_BRUSH);
    model->set_feedback_slot(AT_VOLTAGE_ERROR);

    redirect_stdout_to_file("test_log/step_response_of_CSEET1_model_7.txt");

    run_step_response_of_exciter_model();

    recover_stdout();
}
void CSEET1_TEST::test_step_response_SELF_EXCIATION_and_WITHOUT_BRUSH_and_feedbacker_AT_REGULATOR()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CSEET1_TEST");

    CSEET1* model = (CSEET1*) get_test_exciter_model();

    model->set_excitation_source(SELF_EXCITATION);
    model->set_exciter_brush(WITHOUT_BRUSH);
    model->set_feedback_slot(AT_REGULATOR);

    redirect_stdout_to_file("test_log/step_response_of_CSEET1_model_8.txt");

    run_step_response_of_exciter_model();

    recover_stdout();
}

#include "header/model/compensator_model/COMP_test.h"
#include "header/basic/utility.h"

using namespace std;
COMP_TEST::COMP_TEST()
{
    TEST_ADD(COMP_TEST::test_get_model_type);
    TEST_ADD(COMP_TEST::test_get_model_name);
    TEST_ADD(COMP_TEST::test_set_get_parameters);
    TEST_ADD(COMP_TEST::test_initialize);
    TEST_ADD(COMP_TEST::test_step_response);
    TEST_ADD(COMP_TEST::test_get_standard_model_string);
}



void COMP_TEST::setup()
{
    COMPENSATOR_MODEL_TEST::setup();

    COMP model;

    GENERATOR* generator  = get_generator();

    model.set_Xe(0.1);

    generator->set_model(&model);
}

void COMP_TEST::tear_down()
{
    COMPENSATOR_MODEL_TEST::tear_down();
}


void COMP_TEST::test_get_model_type()
{
    show_test_information_for_function_of_class(__FUNCTION__,"COMP_TEST");

    GENERATOR* genptr = get_generator();
    COMPENSATOR_MODEL* model = genptr->get_compensator_model();

    TEST_ASSERT(model->get_model_type()=="COMPENSATOR");
}
void COMP_TEST::test_get_model_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"COMP_TEST");

    GENERATOR* genptr = get_generator();
    COMPENSATOR_MODEL* model = genptr->get_compensator_model();

    TEST_ASSERT(model->get_model_name()=="COMP");
}



void COMP_TEST::test_set_get_parameters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"COMP_TEST");

    GENERATOR* genptr = get_generator();
    COMP* model = (COMP*) genptr->get_compensator_model();

    model->set_Xe(0.1);
    TEST_ASSERT(fabs(model->get_Xe()-0.1)<FLOAT_EPSILON);
}


void COMP_TEST::test_initialize()
{
    show_test_information_for_function_of_class(__FUNCTION__,"COMP_TEST");

    GENERATOR* genptr = get_generator();
    COMP* model = (COMP*) genptr->get_compensator_model();

    model->set_Xe(0.1);

    SYNC_GENERATOR_MODEL* genmodel = genptr->get_sync_generator_model();
    genmodel->initialize();

    model->initialize();

    POWER_SYSTEM_DATABASE* psdb = model->get_power_system_database();
    complex<double> V = psdb->get_bus_complex_voltage_in_pu(1);
    complex<double> I = genmodel->get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase();

    double ecomp = abs(V - 0.1*I*complex<double>(0,1.0));

    TEST_ASSERT(fabs(model->get_compensated_voltage_in_pu()-ecomp)<FLOAT_EPSILON);
}
void COMP_TEST::test_step_response()
{
    show_test_information_for_function_of_class(__FUNCTION__,"COMP_TEST");

    redirect_stdout_to_file("test_log/step_response_of_COMP_model.txt");

    run_step_response_of_compensator_model();

    recover_stdout();
}
void COMP_TEST::test_get_standard_model_string()
{
    show_test_information_for_function_of_class(__FUNCTION__,"COMP_TEST");

}


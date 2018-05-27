#include "header/model/wtg_models/wt_electrical_model/wt_electrical_model_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

#include "header/model/sg_models/exciter_model/SEXS.h"
#include "header/model/sg_models/compensator_model/COMP.h"
#include "header/model/sg_models/stabilizer_model/IEE2ST.h"
#include "header/model/sg_models/sync_generator_model/gencls.h"

using namespace std;
WT_ELECTRICAL_MODEL_TEST::WT_ELECTRICAL_MODEL_TEST()
{
    TEST_ADD(WT_ELECTRICAL_MODEL_TEST::test_get_model_type);
    TEST_ADD(WT_ELECTRICAL_MODEL_TEST::test_set_get_voltage_reference);
    TEST_ADD(WT_ELECTRICAL_MODEL_TEST::test_step_response);
    TEST_ADD(WT_ELECTRICAL_MODEL_TEST::test_get_standard_model_string);
}



void WT_ELECTRICAL_MODEL_TEST::setup()
{
    WTG_MODEL_TEST::setup();
}

void WT_ELECTRICAL_MODEL_TEST::tear_down()
{
    WTG_MODEL_TEST::tear_down();
    show_test_end_information();
}


void WT_ELECTRICAL_MODEL_TEST::test_get_model_type()
{
    WT_GENERATOR* genptr = get_test_wt_generator();
    WT_ELECTRICAL_MODEL* model = genptr->get_wt_electrical_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    TEST_ASSERT(model->get_model_type()=="WT_ELECTRICAL");
}

void WT_ELECTRICAL_MODEL_TEST::test_set_get_voltage_reference()
{
    WT_GENERATOR* genptr = get_test_wt_generator();
    WT_ELECTRICAL_MODEL* model = genptr->get_wt_electrical_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    model->set_voltage_reference_in_pu(1.5);
    TEST_ASSERT(fabs(model->get_voltage_reference_in_pu()-1.5)<FLOAT_EPSILON);
}

void WT_ELECTRICAL_MODEL_TEST::test_step_response()
{
    WT_GENERATOR* genptr = get_test_wt_generator();
    WT_ELECTRICAL_MODEL* model = genptr->get_wt_electrical_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    redirect_stdout_to_file("test_log/step_response_of_"+model->get_model_name()+"_model.txt");

    run_step_response_of_exciter_model();

    recover_stdout();
}

void WT_ELECTRICAL_MODEL_TEST::export_meter_title()
{
    ostringstream sstream;
    sstream<<"TIME\tVOLT\tEFD";
    show_information_with_leading_time_stamp(sstream);
}

void WT_ELECTRICAL_MODEL_TEST::export_meter_values(double time)
{
    BUS* bus = db->get_bus(1);

    WT_GENERATOR* genptr = get_test_wt_generator();
    WT_ELECTRICAL_MODEL* model = genptr->get_wt_electrical_model();

    double voltage = bus->get_voltage_in_pu();
/*    double efd = model->get_excitation_voltage_in_pu();
    ostringstream sstream;
    sstream<<setw(6)<<setprecision(3)<<time<<"\t"
      <<setw(10)<<setprecision(6)<<voltage<<"\t"
      <<setw(10)<<setprecision(6)<<efd;
    show_information_with_leading_time_stamp(sstream);*/
}

void WT_ELECTRICAL_MODEL_TEST::apply_voltage_drop_of_10_percent()
{
    BUS* bus = db->get_bus(1);

    bus->set_voltage_in_pu(bus->get_voltage_in_pu()-0.1);
}

void WT_ELECTRICAL_MODEL_TEST::run_step_response_of_exciter_model()
{
    ostringstream sstream;
}

void WT_ELECTRICAL_MODEL_TEST::test_get_standard_model_string()
{
    WT_GENERATOR* genptr = get_test_wt_generator();
    WT_ELECTRICAL_MODEL* model = genptr->get_wt_electrical_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    show_information_with_leading_time_stamp(model->get_standard_model_string());
}

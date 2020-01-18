#include "header/basic/test_macro.h"
#include "header/model/sg_models/exciter_model/exciter_model_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

#include "header/model/sg_models/exciter_model/SEXS.h"
#include "header/model/sg_models/compensator_model/COMP.h"
#include "header/model/sg_models/stabilizer_model/IEE2ST.h"
#include "header/model/sg_models/sync_generator_model/gencls.h"

#ifdef ENABLE_STEPS_TEST
using namespace std;
EXCITER_MODEL_TEST::EXCITER_MODEL_TEST()
{
    TEST_ADD(EXCITER_MODEL_TEST::test_get_model_type);
    TEST_ADD(EXCITER_MODEL_TEST::test_set_get_voltage_reference);
    TEST_ADD(EXCITER_MODEL_TEST::test_get_compensated_voltage);
    TEST_ADD(EXCITER_MODEL_TEST::test_get_field_current);
    TEST_ADD(EXCITER_MODEL_TEST::test_step_response);
    TEST_ADD(EXCITER_MODEL_TEST::test_get_standard_psse_string);
}



void EXCITER_MODEL_TEST::setup()
{
    SG_MODEL_TEST::setup();

    GENERATOR* genptr = get_test_generator();

    GENCLS gen_model;
    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();
    gen_model.set_toolkit(default_toolkit);
    gen_model.set_device_id(genptr->get_device_id());

    gen_model.set_Tj_in_s(6.0);
    gen_model.set_D(2.0);
    dmdb.add_model(&gen_model);

    COMP comp_model;
    comp_model.set_toolkit(default_toolkit);
    comp_model.set_device_id(genptr->get_device_id());

    comp_model.set_Xe(0.1);
    dmdb.add_model(&comp_model);

    IEE2ST pss_model;
    pss_model.set_toolkit(default_toolkit);
    pss_model.set_device_id(genptr->get_device_id());

    pss_model.set_K1(1.0);
    pss_model.set_T1_in_s(0.1);
    pss_model.set_K2(1.0);
    pss_model.set_T2_in_s(0.1);
    pss_model.set_T3_in_s(1.0);
    pss_model.set_T4_in_s(2.0);
    pss_model.set_T5_in_s(1.0);
    pss_model.set_T6_in_s(2.0);
    pss_model.set_T7_in_s(2.0);
    pss_model.set_T8_in_s(3.0);
    pss_model.set_T9_in_s(3.0);
    pss_model.set_T10_in_s(4.0);

    dmdb.add_model(&pss_model);
}

void EXCITER_MODEL_TEST::tear_down()
{
    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();
    dmdb.remove_the_last_model();
    dmdb.remove_the_last_model();
    dmdb.remove_the_last_model();

    SG_MODEL_TEST::tear_down();

    show_test_end_information();
}

void EXCITER_MODEL_TEST::test_get_model_type()
{
    EXCITER_MODEL* model = get_test_exciter_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    TEST_ASSERT(model->get_model_type()=="EXCITER");
}

void EXCITER_MODEL_TEST::test_set_get_voltage_reference()
{
    EXCITER_MODEL* model = get_test_exciter_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    model->set_voltage_reference_in_pu(1.5);
    TEST_ASSERT(fabs(model->get_voltage_reference_in_pu()-1.5)<FLOAT_EPSILON);
}

void EXCITER_MODEL_TEST::test_get_compensated_voltage()
{
    EXCITER_MODEL* model = get_test_exciter_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    COMPENSATOR_MODEL* comp_model = get_test_compensator_model();

    comp_model->initialize();

    TEST_ASSERT(fabs(model->get_compensated_voltage_in_pu()-comp_model->get_compensated_voltage_in_pu())<FLOAT_EPSILON);
}

void EXCITER_MODEL_TEST::test_get_field_current()
{
    EXCITER_MODEL* model = get_test_exciter_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    SYNC_GENERATOR_MODEL* gen_model = get_test_sync_generator_model();

    gen_model->initialize();

    TEST_ASSERT(fabs(model->get_field_current_in_pu()-gen_model->get_field_current_in_pu_based_on_mbase())<FLOAT_EPSILON);
}

void EXCITER_MODEL_TEST::test_step_response()
{
    EXCITER_MODEL* model = get_test_exciter_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    default_toolkit.open_log_file("test_log/step_response_of_"+model->get_model_name()+"_model.txt");

    run_step_response_of_exciter_model();

    default_toolkit.close_log_file();
}

void EXCITER_MODEL_TEST::export_meter_title()
{
    ostringstream osstream;
    osstream<<"TIME\tVOLT\tEFD";
    default_toolkit.show_information_with_leading_time_stamp(osstream);
}

void EXCITER_MODEL_TEST::export_meter_values(double time)
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    BUS* bus = psdb.get_bus(1);

    EXCITER_MODEL* model = get_test_exciter_model();

    double voltage = bus->get_positive_sequence_voltage_in_pu();
    double efd = model->get_excitation_voltage_in_pu();
    ostringstream osstream;
    osstream<<setw(6)<<setprecision(3)<<time<<"\t"
      <<setw(10)<<setprecision(6)<<voltage<<"\t"
      <<setw(10)<<setprecision(6)<<efd;
    default_toolkit.show_information_with_leading_time_stamp(osstream);
}

void EXCITER_MODEL_TEST::apply_voltage_drop_of_10_percent()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    BUS* bus = psdb.get_bus(1);

    bus->set_positive_sequence_voltage_in_pu(bus->get_positive_sequence_voltage_in_pu()-0.1);
}

void EXCITER_MODEL_TEST::run_step_response_of_exciter_model()
{
    ostringstream osstream;
    //GENERATOR* genptr = get_test_generator();
    //genptr->clear_compensator_model();

    SYNC_GENERATOR_MODEL* genmodel = get_test_sync_generator_model();
    genmodel->initialize();
    genmodel->set_initial_excitation_voltage_in_pu(1.0);

    EXCITER_MODEL* model = get_test_exciter_model();

    osstream<<"Model:"<<model->get_standard_psse_string()<<endl;
    default_toolkit.show_information_with_leading_time_stamp(osstream);

    double delt = 0.001;
    default_toolkit.set_dynamic_simulation_time_step_in_s(delt);

    double TIME = -delt*2.0;
    double efd;

    model->initialize();

    export_meter_title();
    export_meter_values(TIME);
    while(true)
    {
        TIME += delt;
        if(TIME>1.0+FLOAT_EPSILON)
        {
            TIME -=delt;
            break;
        }
        efd =  model->get_excitation_voltage_in_pu();
        while(true)
        {
            model->run(INTEGRATE_MODE);
            if(fabs(efd-model->get_excitation_voltage_in_pu())>1e-6)
                efd = model->get_excitation_voltage_in_pu();
            else
                break;
        }
        model->run(UPDATE_MODE);

        export_meter_values(TIME);
    }

    apply_voltage_drop_of_10_percent();
    model->run(UPDATE_MODE);
    export_meter_values(TIME);

    while(true)
    {
        TIME += delt;

        if(TIME>6.0+FLOAT_EPSILON)
        {
            TIME -=delt;
            break;
        }
        efd =  model->get_excitation_voltage_in_pu();
        while(true)
        {
            model->run(INTEGRATE_MODE);
            if(fabs(efd-model->get_excitation_voltage_in_pu())>1e-6)
                efd = model->get_excitation_voltage_in_pu();
            else
                break;
        }
        model->run(UPDATE_MODE);

        export_meter_values(TIME);
    }
}

void EXCITER_MODEL_TEST::test_get_standard_psse_string()
{
    EXCITER_MODEL* model = get_test_exciter_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    default_toolkit.show_information_with_leading_time_stamp(model->get_standard_psse_string());
}

#endif

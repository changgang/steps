#include "header/model/exciter_model/exciter_model_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

#include "header/model/exciter_model/SEXS.h"
#include "header/model/compensator_model/COMP.h"
#include "header/model/stabilizer_model/IEE2ST.h"
#include "header/model/source_model/sync_generator_model/gencls.h"

using namespace std;
EXCITER_MODEL_TEST::EXCITER_MODEL_TEST()
{
    TEST_ADD(EXCITER_MODEL_TEST::test_get_model_type);
    TEST_ADD(EXCITER_MODEL_TEST::test_set_get_voltage_reference);
    TEST_ADD(EXCITER_MODEL_TEST::test_get_compensated_voltage);
    TEST_ADD(EXCITER_MODEL_TEST::test_step_response);
    TEST_ADD(EXCITER_MODEL_TEST::test_get_standard_model_string);
}



void EXCITER_MODEL_TEST::setup()
{
    db = new POWER_SYSTEM_DATABASE;
    db->set_allowed_max_bus_number(100);
    db->set_system_base_frequency_in_Hz(50.0);
    db->set_system_base_power_in_MVA(100.0);

    BUS bus(db);
    bus.set_bus_number(1);
    bus.set_bus_type(PV_TYPE);
    bus.set_base_voltage_in_kV(21.0);
    bus.set_voltage_in_pu(1.0);
    bus.set_angle_in_rad(0.0);

    db->append_bus(bus);

    GENERATOR generator(db);
    generator.set_generator_bus(1);
    generator.set_identifier("#1");
    generator.set_status(true);
    generator.set_mbase_in_MVA(200.0);
    generator.set_source_impedance_in_pu(complex<double>(0.01, 0.1));
    generator.set_p_generation_in_MW(100.0);
    generator.set_q_generation_in_MVar(30.0);

    db->append_generator(generator);

    DEVICE_ID did;
    did.set_device_type("GENERATOR");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    genptr = db->get_generator(did);

    GENCLS gen_model;
    gen_model.set_Tj_in_s(6.0);
    gen_model.set_D(2.0);
    genptr->set_model(&gen_model);

    COMP comp_model;
    comp_model.set_Xe(0.1);
    genptr->set_model(&comp_model);

    IEE2ST pss_model;
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

    genptr->set_model(&pss_model);
}

void EXCITER_MODEL_TEST::tear_down()
{
    delete db;

    show_test_end_information();
}

GENERATOR* EXCITER_MODEL_TEST::get_generator()
{
    return genptr;
}


void EXCITER_MODEL_TEST::test_get_model_type()
{
    GENERATOR* genptr = get_generator();
    EXCITER_MODEL* model = genptr->get_exciter_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    TEST_ASSERT(model->get_model_type()=="EXCITER");
}

void EXCITER_MODEL_TEST::test_set_get_voltage_reference()
{
    GENERATOR* genptr = get_generator();
    EXCITER_MODEL* model = genptr->get_exciter_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    model->set_voltage_reference_in_pu(1.5);
    TEST_ASSERT(fabs(model->get_voltage_reference_in_pu()-1.5)<FLOAT_EPSILON);
}

void EXCITER_MODEL_TEST::test_get_compensated_voltage()
{
    GENERATOR* genptr = get_generator();
    EXCITER_MODEL* model = genptr->get_exciter_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    COMPENSATOR_MODEL* comp_model = genptr->get_compensator_model();

    comp_model->initialize();

    TEST_ASSERT(fabs(model->get_compensated_voltage_in_pu()-comp_model->get_compensated_voltage_in_pu())<FLOAT_EPSILON);
}

void EXCITER_MODEL_TEST::test_step_response()
{
    GENERATOR* genptr = get_generator();
    EXCITER_MODEL* model = genptr->get_exciter_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    redirect_stdout_to_file("test_log/step_response_of_"+model->get_model_name()+"_model.txt");

    run_step_response_of_exciter_model();

    recover_stdout();
}

void EXCITER_MODEL_TEST::export_meter_title()
{
    ostringstream sstream;
    sstream<<"TIME\tVOLT\tEFD";
    show_information_with_leading_time_stamp(sstream);
}

void EXCITER_MODEL_TEST::export_meter_values(double time)
{
    BUS* bus = db->get_bus(1);

    GENERATOR* genptr = get_generator();
    EXCITER_MODEL* model = genptr->get_exciter_model();

    double voltage = bus->get_voltage_in_pu();
    double efd = model->get_excitation_voltage_in_pu();
    ostringstream sstream;
    sstream<<setw(6)<<setprecision(3)<<time<<"\t"
      <<setw(10)<<setprecision(6)<<voltage<<"\t"
      <<setw(10)<<setprecision(6)<<efd;
    show_information_with_leading_time_stamp(sstream);
}

void EXCITER_MODEL_TEST::apply_voltage_drop_of_10_percent()
{
    BUS* bus = db->get_bus(1);

    bus->set_voltage_in_pu(bus->get_voltage_in_pu()-0.1);
}

void EXCITER_MODEL_TEST::run_step_response_of_exciter_model()
{
    ostringstream sstream;
    GENERATOR* genptr = get_generator();
    genptr->clear_compensator_model();

    SYNC_GENERATOR_MODEL* genmodel = genptr->get_sync_generator_model();
    genmodel->initialize();
    genmodel->set_initial_excitation_voltage_in_pu(1.0);

    EXCITER_MODEL* model = genptr->get_exciter_model();

    sstream<<"Model:"<<model->get_standard_model_string()<<endl;
    show_information_with_leading_time_stamp(sstream);

    double delt = 0.001;
    set_dynamic_simulation_time_step_in_s(delt);

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

void EXCITER_MODEL_TEST::test_get_standard_model_string()
{
    GENERATOR* genptr = get_generator();
    EXCITER_MODEL* model = genptr->get_exciter_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    show_information_with_leading_time_stamp(model->get_standard_model_string());
}

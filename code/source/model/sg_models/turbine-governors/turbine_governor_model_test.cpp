#include "header/model/sg_models/turbine_governor_model/turbine_governor_model_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

#include "header/model/sg_models/turbine_governor_model/TGOV1.h"
#include "header/model/sg_models/sync_generator_model/gencls.h"

using namespace std;
TURBINE_GOVERNOR_MODEL_TEST::TURBINE_GOVERNOR_MODEL_TEST()
{
    TEST_ADD(TURBINE_GOVERNOR_MODEL_TEST::test_get_rotor_speed);
}



void TURBINE_GOVERNOR_MODEL_TEST::setup()
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
    generator.set_source_impedance_in_pu(complex<double>(0.0, 0.1));
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
}

void TURBINE_GOVERNOR_MODEL_TEST::tear_down()
{
    delete db;

    show_test_end_information();
}

GENERATOR* TURBINE_GOVERNOR_MODEL_TEST::get_generator()
{
    return genptr;
}

void TURBINE_GOVERNOR_MODEL_TEST::apply_speed_drop_of_1_percent()
{
    GENERATOR* genptr = get_generator();

    GENCLS* genmodel = (GENCLS*) genptr->get_sync_generator_model();
    genmodel->set_rotor_speed_deviation_in_pu(-0.01);
}


void TURBINE_GOVERNOR_MODEL_TEST::export_meter_title()
{
    ostringstream sstream;
    sstream<<"TIME\tSPEED\tPMECH";
    show_information_with_leading_time_stamp(sstream);
}

void TURBINE_GOVERNOR_MODEL_TEST::export_meter_values(double time)
{
    ostringstream sstream;

    GENERATOR* genptr = get_generator();
    TURBINE_GOVERNOR_MODEL* model = genptr->get_turbine_governor_model();

    double speed = model->get_rotor_speed_deviation_in_pu_from_sync_generator_model();
    double pmech = model->get_mechanical_power_in_pu_based_on_mbase();

    sstream<<setw(6)<<setprecision(3)<<fixed<<time<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<speed<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<pmech;
    show_information_with_leading_time_stamp(sstream);
}

void TURBINE_GOVERNOR_MODEL_TEST::run_step_response_of_turbine_govnernor_model()
{
    ostringstream sstream;

    GENERATOR* genptr = get_generator();
    TURBINE_GOVERNOR_MODEL* model = genptr->get_turbine_governor_model();
    sstream<<"Model:"<<model->get_standard_model_string()<<endl;
    show_information_with_leading_time_stamp(sstream);

    double delt = 0.001;
    set_dynamic_simulation_time_step_in_s(delt);

    SYNC_GENERATOR_MODEL* genmodel = genptr->get_sync_generator_model();

    genmodel->initialize();
    //genmodel->set_initial_mechanical_power_in_pu_based_on_mbase(0.5);

    double TIME = -delt*2.0;
    double pmech;

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
        pmech =  model->get_mechanical_power_in_pu_based_on_mbase();
        while(true)
        {
            model->run(INTEGRATE_MODE);
            if(fabs(pmech-model->get_mechanical_power_in_pu_based_on_mbase())>1e-6)
                pmech = model->get_mechanical_power_in_pu_based_on_mbase();
            else
                break;
        }
        model->run(UPDATE_MODE);

        export_meter_values(TIME);
    }

    apply_speed_drop_of_1_percent();
    model->run(UPDATE_MODE);
    pmech = model->get_mechanical_power_in_pu_based_on_mbase();

    export_meter_values(TIME);

    while(true)
    {
        TIME += delt;

        if(TIME>6.0+FLOAT_EPSILON)
        {
            TIME -=delt;
            break;
        }
        pmech =  model->get_mechanical_power_in_pu_based_on_mbase();
        while(true)
        {
            model->run(INTEGRATE_MODE);
            if(fabs(pmech-model->get_mechanical_power_in_pu_based_on_mbase())>1e-6)
                pmech = model->get_mechanical_power_in_pu_based_on_mbase();
            else
                break;
        }
        model->run(UPDATE_MODE);

        export_meter_values(TIME);
    }
}

void TURBINE_GOVERNOR_MODEL_TEST::test_get_rotor_speed()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TURBINE_GOVERNOR_MODEL_TEST");

    GENERATOR* genptr = get_generator();
    SYNC_GENERATOR_MODEL* genmodel = genptr->get_sync_generator_model();

    TGOV1 model;
    model.set_power_system_database(db);
    model.set_device_id(genptr->get_device_id());

    TEST_ASSERT(fabs(model.get_rotor_speed_deviation_in_pu_from_sync_generator_model()-genmodel->get_rotor_speed_deviation_in_pu())<FLOAT_EPSILON);
}

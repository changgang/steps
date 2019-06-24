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
    SG_MODEL_TEST::setup();

    GENERATOR* genptr = get_test_generator();

    GENCLS gen_model;
    gen_model.set_Tj_in_s(6.0);
    gen_model.set_D(2.0);
    genptr->set_model(&gen_model);
}

void TURBINE_GOVERNOR_MODEL_TEST::tear_down()
{
    SG_MODEL_TEST::tear_down();

    show_test_end_information();
}


void TURBINE_GOVERNOR_MODEL_TEST::apply_speed_drop_of_1_percent()
{
    GENCLS* genmodel = (GENCLS*) get_test_sync_generator_model();
    genmodel->set_rotor_speed_deviation_in_pu(-0.01);
}


void TURBINE_GOVERNOR_MODEL_TEST::export_meter_title()
{
    ostringstream osstream;
    osstream<<"TIME\tSPEED\tPMECH\tPRESSURE";
    default_toolkit.show_information_with_leading_time_stamp(osstream);
}

void TURBINE_GOVERNOR_MODEL_TEST::export_meter_values(double time)
{
    ostringstream osstream;

    GENERATOR* genptr = get_test_generator();
    TURBINE_GOVERNOR_MODEL* model = genptr->get_turbine_governor_model();

    double speed = model->get_rotor_speed_deviation_in_pu_from_sync_generator_model();
    double pmech = model->get_mechanical_power_in_pu_based_on_mbase();

    osstream<<setw(6)<<setprecision(3)<<fixed<<time<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<speed<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<pmech<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<model->get_model_internal_variable_with_name("STEAM PRESSURE");
    default_toolkit.show_information_with_leading_time_stamp(osstream);
}

void TURBINE_GOVERNOR_MODEL_TEST::run_step_response_of_turbine_govnernor_model()
{
    ostringstream osstream;

    GENERATOR* genptr = get_test_generator();
    TURBINE_GOVERNOR_MODEL* model = genptr->get_turbine_governor_model();
    osstream<<"Model:"<<model->get_standard_model_string()<<endl;
    default_toolkit.show_information_with_leading_time_stamp(osstream);

    double delt = 0.001;
    default_toolkit.set_dynamic_simulation_time_step_in_s(delt);

    SYNC_GENERATOR_MODEL* genmodel = get_test_sync_generator_model();

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

    GENERATOR* genptr = get_test_generator();
    SYNC_GENERATOR_MODEL* genmodel = get_test_sync_generator_model();

    TGOV1 model;
    model.set_toolkit(default_toolkit);
    model.set_device_id(genptr->get_device_id());

    TEST_ASSERT(fabs(model.get_rotor_speed_deviation_in_pu_from_sync_generator_model()-genmodel->get_rotor_speed_deviation_in_pu())<FLOAT_EPSILON);
}

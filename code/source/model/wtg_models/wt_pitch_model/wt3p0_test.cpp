#include "header/model/wtg_models/wt_pitch_model/wt3p0_test.h"
#include "header/basic/utility.h"
#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <cstdio>
#include <cmath>

using namespace std;

WT3P0_TEST::WT3P0_TEST() : WT_PITCH_MODEL_TEST()
{
    TEST_ADD(WT3P0_TEST::test_get_model_name);
    TEST_ADD(WT3P0_TEST::test_set_get_parameters);
    TEST_ADD(WT3P0_TEST::test_initialize);
}

void WT3P0_TEST::setup()
{
    WT_PITCH_MODEL_TEST::setup();

    WT_GENERATOR* wt_gen = get_test_wt_generator();

    WT3P0 model;
    model.set_Tp_in_s(0.5);
    model.set_Kp_speed_controller(1.0);
    model.set_Ki_speed_controller(2.0);
    model.set_frequency_lower_deadband_in_pu(0.995);
    model.set_frequency_upper_deadband_in_pu(1.005);
    model.set_Kp_frequency_controller(1.0);
    model.set_Ki_frequency_controller(2.0);
    model.set_ratePitchmax_in_deg_per_s(3.0);
    model.set_Pitchmin_in_deg(0.0);
    model.set_Pitchmax_in_deg(30.0);

    wt_gen->set_model(&model);
}

void WT3P0_TEST::tear_down()
{
    WT_PITCH_MODEL_TEST::tear_down();

    show_test_end_information();
}


void WT3P0_TEST::test_get_model_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"WT3P0_TEST");

    WT_PITCH_MODEL* model = get_test_wt_pitch_model();
    if(model!=NULL)
    {
        TEST_ASSERT(model->get_model_name()=="WT3P0");
    }
    else
        TEST_ASSERT(false);
}

void WT3P0_TEST::test_set_get_parameters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"WT3P0_TEST");

    WT3P0* model = (WT3P0*) get_test_wt_pitch_model();

    model->set_Tp_in_s(0.5);
    TEST_ASSERT(fabs(model->get_Tp_in_s()-0.5)<FLOAT_EPSILON);

    model->set_Kp_speed_controller(1.0);
    TEST_ASSERT(fabs(model->get_Kp_speed_controller()-1.0)<FLOAT_EPSILON);

    model->set_Ki_speed_controller(2.0);
    TEST_ASSERT(fabs(model->get_Ki_speed_controller()-2.0)<FLOAT_EPSILON);

    model->set_Kp_frequency_controller(1.0);
    TEST_ASSERT(fabs(model->get_Kp_frequency_controller()-1.0)<FLOAT_EPSILON);

    model->set_Ki_frequency_controller(2.0);
    TEST_ASSERT(fabs(model->get_Ki_frequency_controller()-2.0)<FLOAT_EPSILON);

    model->set_frequency_upper_deadband_in_pu(1.005);
    TEST_ASSERT(fabs(model->get_frequency_upper_deadband_in_pu()-1.005)<FLOAT_EPSILON);

    model->set_frequency_lower_deadband_in_pu(0.995);
    TEST_ASSERT(fabs(model->get_frequency_lower_deadband_in_pu()-0.995)<FLOAT_EPSILON);

    model->set_Pitchmax_in_deg(30.0);
    TEST_ASSERT(fabs(model->get_Pitchmax_in_deg()-30.0)<FLOAT_EPSILON);

    model->set_Pitchmin_in_deg(0.0);
    TEST_ASSERT(fabs(model->get_Pitchmin_in_deg()-0.0)<FLOAT_EPSILON);

    model->set_ratePitchmax_in_deg_per_s(3.0);
    TEST_ASSERT(fabs(model->get_ratePitchmax_in_deg_per_s()-3.0)<FLOAT_EPSILON);
}

void WT3P0_TEST::test_initialize()
{
    show_test_information_for_function_of_class(__FUNCTION__,"WT3P0_TEST");
    set_dynamic_simulation_time_step_in_s(0.001);
    ostringstream osstream;

    WT_GENERATOR_MODEL* wtgenmodel = get_test_wt_generator_model();
    WT_AERODYNAMIC_MODEL* aerdmodel = get_test_wt_aerodynamic_model();
    WT_TURBINE_MODEL* turbinemodel = get_test_wt_turbine_model();
    WT3P0* model = (WT3P0*) get_test_wt_pitch_model();

    wtgenmodel->initialize();
    aerdmodel->initialize();
    turbinemodel->initialize();
    model->initialize();
    /*osstream<<"WT3P0 model after initialized with generation of each turbine: "<<gen->get_p_generation_in_MW()/gen->get_number_of_lumped_wt_generators()<<" MW"<<endl;
    osstream<<"Turbine blade radius = "<<model->get_turbine_blade_radius_in_m()<<" m, generator/turbine turn ratio = "<<model->get_generator_to_turbine_gear_ratio()<<endl
           <<"Pitch angle = "<<model->get_initial_pitch_angle_in_deg()<<" deg, turbine speed = "<<model->get_initial_turbine_speed_in_rad_per_s()<<" rad/s ("
           <<model->get_initial_turbine_speed_in_pu()<<" pu)";
    show_information_with_leading_time_stamp(osstream);
    */
}

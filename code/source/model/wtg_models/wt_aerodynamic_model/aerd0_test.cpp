#include "header/model/wtg_models/wt_aerodynamic_model/aerd0_test.h"
#include "header/basic/utility.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cstdio>
#include <cmath>

using namespace std;

AERD0_TEST::AERD0_TEST() : WT_AERODYNAMIC_MODEL_TEST()
{
    TEST_ADD(AERD0_TEST::test_get_model_name);
    TEST_ADD(AERD0_TEST::test_set_get_parameters);
    //TEST_ADD(AERD0_TEST::test_initialize_and_get_initialized_inputs_with_overspeed_flag);
    //TEST_ADD(AERD0_TEST::test_initialize_and_get_initialized_inputs_without_overspeed_flag);
    TEST_ADD(AERD0_TEST::test_set_as_typical_wt_generator);
}

void AERD0_TEST::setup()
{
    WT_AERODYNAMIC_MODEL_TEST::setup();

    WT_GENERATOR* wt_gen = get_test_wt_generator();
    wt_gen->set_p_generation_in_MW(20.0);
    wt_gen->set_rated_power_per_wt_generator_in_MW(1.5);
    wt_gen->set_number_of_lumped_wt_generators(20.0);

    AERD0 model;
    model.set_number_of_pole_pairs(2);
    model.set_generator_to_turbine_gear_ratio(100.0);
    model.set_gear_efficiency(1.0);
    model.set_turbine_blade_radius_in_m(25.0);
    model.set_nominal_wind_speed_in_mps(13.0);
    model.set_nominal_air_density_in_kgpm3(1.2);
    model.set_initial_wind_speed_in_mps(13.0);
    model.set_initial_pitch_angle_in_deg(0.0);
    model.set_initial_turbine_speed_in_rad_per_s(0.0);
    model.set_air_density_in_kgpm3(1.2);
    model.set_overspeed_mode_flag(false);

    model.set_C1(0.22);
    model.set_C2(116.0);
    model.set_C3(0.4);
    model.set_C4(5.0);
    model.set_C5(-12.5);
    model.set_C6(0.0);

    wt_gen->set_model(&model);
}

void AERD0_TEST::tear_down()
{
    WT_AERODYNAMIC_MODEL_TEST::tear_down();

    show_test_end_information();
}


void AERD0_TEST::test_get_model_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"AERD0_TEST");

    WT_AERODYNAMIC_MODEL* model = get_test_wt_aerodynamic_model();
    if(model!=NULL)
    {
        TEST_ASSERT(model->get_model_name()=="AERD0");
    }
    else
        TEST_ASSERT(false);
}

void AERD0_TEST::test_set_get_parameters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"AERD0_TEST");

    AERD0* model = (AERD0*) get_test_wt_aerodynamic_model();

    model->set_C1(1.1);
    model->set_C2(2.2);
    model->set_C3(3.3);
    model->set_C4(4.4);
    model->set_C5(5.5);
    model->set_C6(6.6);

    TEST_ASSERT(fabs(model->get_C1()-1.1)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_C2()-2.2)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_C3()-3.3)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_C4()-4.4)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_C5()-5.5)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_C6()-6.6)<FLOAT_EPSILON);
}

void AERD0_TEST::test_initialize_and_get_initialized_inputs_with_overspeed_flag()
{
    show_test_information_for_function_of_class(__FUNCTION__,"AERD0_TEST");

    ostringstream sstream;

    AERD0* model = (AERD0*) get_test_wt_aerodynamic_model();
    model->set_overspeed_mode_flag(true);

    model->initialize();
    sstream<<"AERD0 model after initialized:"<<endl;
    sstream<<"Turbine blade radius = "<<model->get_turbine_blade_radius_in_m()<<" m, turbine speed = "<<model->get_turbine_speed_in_rad_per_s()<<endl;
    show_information_with_leading_time_stamp(sstream);
}

void AERD0_TEST::test_initialize_and_get_initialized_inputs_without_overspeed_flag()
{
    show_test_information_for_function_of_class(__FUNCTION__,"AERD0_TEST");

    ostringstream sstream;

    AERD0* model = (AERD0*) get_test_wt_aerodynamic_model();
    model->set_overspeed_mode_flag(false);

    model->initialize();
    sstream<<"AERD0 model after initialized:"<<endl;
    sstream<<"Turbine blade radius = "<<model->get_turbine_blade_radius_in_m()<<" m, turbine speed = "<<model->get_turbine_speed_in_rad_per_s()<<endl;
    show_information_with_leading_time_stamp(sstream);
}

void AERD0_TEST::test_set_as_typical_wt_generator()
{
    ;
}

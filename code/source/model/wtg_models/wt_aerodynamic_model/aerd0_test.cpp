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
    TEST_ADD(AERD0_TEST::test_initialize_and_get_initialized_inputs_without_overspeed_flag);
    TEST_ADD(AERD0_TEST::test_initialize_and_get_initialized_inputs_with_overspeed_flag);
    TEST_ADD(AERD0_TEST::test_set_as_typical_wt_generator);
    TEST_ADD(AERD0_TEST::test_list_Cp_and_mechanical_power_data_of_different_wind_speed);
}

void AERD0_TEST::setup()
{
    WT_AERODYNAMIC_MODEL_TEST::setup();

    WT_GENERATOR* wt_gen = get_test_wt_generator();

    AERD0 model;
    model.set_number_of_pole_pairs(2);
    model.set_generator_to_turbine_gear_ratio(100.0);
    model.set_gear_efficiency(1.0);
    model.set_turbine_blade_radius_in_m(25.0);
    model.set_nominal_wind_speed_in_mps(13.0);
    model.set_nominal_air_density_in_kgpm3(1.25);
    model.set_air_density_in_kgpm3(1.25);
    model.set_overspeed_mode_flag(false);

    model.set_C1(0.22);
    model.set_C2(116.0);
    model.set_C3(0.4);
    model.set_C4(5.0);
    model.set_C5(12.5);
    model.set_C6(0.0);

    model.set_C1(0.5176);
    model.set_C2(116.0);
    model.set_C3(0.4);
    model.set_C4(5.0);
    model.set_C5(21.0);
    model.set_C6(0.0068);


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

void AERD0_TEST::test_initialize_and_get_initialized_inputs_without_overspeed_flag()
{
    show_test_information_for_function_of_class(__FUNCTION__,"AERD0_TEST");
    set_dynamic_simulation_time_step_in_s(0.001);
    ostringstream osstream;

    WT_GENERATOR_MODEL* wtgenmodel = get_test_wt_generator_model();
    AERD0* model = (AERD0*) get_test_wt_aerodynamic_model();
    model->set_overspeed_mode_flag(false);

    WT_GENERATOR* gen = get_test_wt_generator();

    for(double p=5.0; p<=31.0; p+=5.0)
    {
        if(p>=30.0)
            p = 29.0;
        gen->set_p_generation_in_MW(p);
        wtgenmodel->set_flag_model_initialized_as_false();
        model->set_flag_model_initialized_as_false();
        wtgenmodel->initialize();
        model->initialize();
        osstream<<"AERD0 model after initialized with generation of each turbine: "<<gen->get_p_generation_in_MW()/gen->get_number_of_lumped_wt_generators()<<" MW"<<endl;
        osstream<<"Turbine blade radius = "<<model->get_turbine_blade_radius_in_m()<<" m, generator/turbine turn ratio = "<<model->get_generator_to_turbine_gear_ratio()<<endl
               <<"Pitch angle = "<<model->get_initial_pitch_angle_in_deg()<<" deg, turbine speed = "<<model->get_initial_turbine_speed_in_rad_per_s()<<" rad/s ("
               <<model->get_initial_turbine_speed_in_pu()<<" pu)";
        show_information_with_leading_time_stamp(osstream);
    }
}

void AERD0_TEST::test_initialize_and_get_initialized_inputs_with_overspeed_flag()
{
    show_test_information_for_function_of_class(__FUNCTION__,"AERD0_TEST");
    set_dynamic_simulation_time_step_in_s(0.001);
    ostringstream osstream;

    WT_GENERATOR_MODEL* wtgenmodel = get_test_wt_generator_model();
    AERD0* model = (AERD0*) get_test_wt_aerodynamic_model();
    model->set_overspeed_mode_flag(true);

    WT_GENERATOR* gen = get_test_wt_generator();

    for(double p=5.0; p<31.0; p+=5.0)
    {
        if(p>=30.0)
            p = 29.0;
        gen->set_p_generation_in_MW(p);
        wtgenmodel->set_flag_model_initialized_as_false();
        model->set_flag_model_initialized_as_false();
        wtgenmodel->initialize();
        model->initialize();
        osstream<<"AERD0 model after initialized with generation of each turbine: "<<gen->get_p_generation_in_MW()/gen->get_number_of_lumped_wt_generators()<<" MW"<<endl;
        osstream<<"Turbine blade radius = "<<model->get_turbine_blade_radius_in_m()<<" m, generator/turbine turn ratio = "<<model->get_generator_to_turbine_gear_ratio()<<endl
               <<"Pitch angle = "<<model->get_initial_pitch_angle_in_deg()<<" deg, turbine speed = "<<model->get_initial_turbine_speed_in_rad_per_s()<<" rad/s ("
               <<model->get_initial_turbine_speed_in_pu()<<" pu)";
        show_information_with_leading_time_stamp(osstream);
    }
}

void AERD0_TEST::test_list_Cp_and_mechanical_power_data_of_different_wind_speed()
{
    ostringstream osstream;

    AERD0* model = (AERD0*) get_test_wt_aerodynamic_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        model->initialize();
        string file =  "test_log/test_"+model->get_model_name()+"_"+__FUNCTION__+".txt";
        redirect_stdout_to_file(file);

        double r = model->get_turbine_blade_radius_in_m();
        osstream<<"Power curve when pitch = 0.0 deg, and wind speed = "<<model->get_nominal_wind_speed_in_mps()<<" m/s, blade radius = "<<r<<" m";
        show_information_with_leading_time_stamp(osstream);
        osstream<<"Speed(rad/s)\tLambda\tCP\tPmech(MW)";
        show_information_with_leading_time_stamp(osstream);

        /*for(double w=0.1; w<4.0*PI; w+=0.1)
        {
            double lambda = w*r/vwind;
            double cp = model->get_Cp(lambda, pitch);
            if(cp<0.0)
                break;
            double pmech = model->get_turbine_mechanical_power_per_wt_generator_in_MW();
            osstream<<setw(10)<<setprecision(6)<<w<<"\t"
                   <<setw(10)<<setprecision(6)<<lambda<<"\t"
                   <<setw(10)<<setprecision(6)<<cp<<"\t"
                   <<setw(10)<<setprecision(6)<<pmech;
            show_information_with_leading_time_stamp(osstream);
        }*/
        recover_stdout();
    }
    else
        TEST_ASSERT(false);
}

void AERD0_TEST::test_set_as_typical_wt_generator()
{
    ;
}

void AERD0_TEST::test_get_turbine_speed_reference()
{

}

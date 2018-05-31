#include "header/model/wtg_models/wt_aerodynamic_model/wt_aerodynamic_model_test.h"
#include "header/basic/utility.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cstdio>
#include <cmath>

using namespace std;

WT_AERODYNAMIC_MODEL_TEST::WT_AERODYNAMIC_MODEL_TEST()
{
    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_get_model_type);

    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_get_damping_in_pu);

    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_set_get_number_of_pole_pairs);
    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_set_get_generator_to_turbine_gear_ratio);
    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_set_get_gear_efficiency);
    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_set_get_turbine_blade_radius);
    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_set_get_nominal_wind_speed);
    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_set_get_nominal_air_density);

    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_get_nominal_turbine_speed);

    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_set_get_initial_pitch_angle);
    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_set_get_initial_turbine_speed);

    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_set_get_air_density);
    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_set_get_overspeed_mode_flag);
    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_set_get_max_steady_state_turbine_speed);
    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_set_get_min_steady_state_turbine_speed);

    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_get_wind_speed);
    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_get_pitch_angle);
    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_get_turbine_frequency);
    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_get_turbine_speed);

    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_get_standard_model_string);
}

void WT_AERODYNAMIC_MODEL_TEST::setup()
{
    WTG_MODEL_TEST::setup();
}

void WT_AERODYNAMIC_MODEL_TEST::tear_down()
{
    WTG_MODEL_TEST::tear_down();
}

void WT_AERODYNAMIC_MODEL_TEST::test_get_model_type()
{
    WT_AERODYNAMIC_MODEL* model = get_test_wt_aerodynamic_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        TEST_ASSERT(model->get_model_type()=="WT AERODYNAMIC");
    }
    else
        TEST_ASSERT(false);
}

void WT_AERODYNAMIC_MODEL_TEST::test_get_damping_in_pu()
{
    WT_AERODYNAMIC_MODEL* model = get_test_wt_aerodynamic_model();
    WT_TURBINE_MODEL* turbine = get_test_wt_turbine_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        if(turbine==NULL)
        {
            TEST_ASSERT(fabs(model->get_damping_in_pu()-0.0)<FLOAT_EPSILON);
        }
        else
            TEST_ASSERT(fabs(model->get_damping_in_pu()-turbine->get_damping_in_pu())<FLOAT_EPSILON);
    }
    else
        TEST_ASSERT(false);
}

void WT_AERODYNAMIC_MODEL_TEST::test_set_get_number_of_pole_pairs()
{
    WT_AERODYNAMIC_MODEL* model = get_test_wt_aerodynamic_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        model->set_number_of_pole_pairs(2);
        TEST_ASSERT(model->get_number_of_pole_pairs()==2);
        model->set_number_of_pole_pairs(5);
        TEST_ASSERT(model->get_number_of_pole_pairs()==5);
    }
    else
        TEST_ASSERT(false);
}

void WT_AERODYNAMIC_MODEL_TEST::test_set_get_generator_to_turbine_gear_ratio()
{
    WT_AERODYNAMIC_MODEL* model = get_test_wt_aerodynamic_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        model->set_generator_to_turbine_gear_ratio(100.0);
        TEST_ASSERT(fabs(model->get_generator_to_turbine_gear_ratio()-100.0)<FLOAT_EPSILON);
        model->set_generator_to_turbine_gear_ratio(150.0);
        TEST_ASSERT(fabs(model->get_generator_to_turbine_gear_ratio()-150.0)<FLOAT_EPSILON);
    }
    else
        TEST_ASSERT(false);
}

void WT_AERODYNAMIC_MODEL_TEST::test_set_get_gear_efficiency()
{
    WT_AERODYNAMIC_MODEL* model = get_test_wt_aerodynamic_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        model->set_gear_efficiency(0.99);
        TEST_ASSERT(fabs(model->get_gear_efficiency()-0.99)<FLOAT_EPSILON);
        model->set_gear_efficiency(0.9);
        TEST_ASSERT(fabs(model->get_gear_efficiency()-0.9)<FLOAT_EPSILON);
    }
    else
        TEST_ASSERT(false);
}

void WT_AERODYNAMIC_MODEL_TEST::test_set_get_turbine_blade_radius()
{
    WT_AERODYNAMIC_MODEL* model = get_test_wt_aerodynamic_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        model->set_turbine_blade_radius_in_m(30.0);
        TEST_ASSERT(fabs(model->get_turbine_blade_radius_in_m()-30.0)<FLOAT_EPSILON);
        model->set_turbine_blade_radius_in_m(80.0);
        TEST_ASSERT(fabs(model->get_turbine_blade_radius_in_m()-80.0)<FLOAT_EPSILON);
    }
    else
        TEST_ASSERT(false);
}

void WT_AERODYNAMIC_MODEL_TEST::test_set_get_nominal_wind_speed()
{
    WT_AERODYNAMIC_MODEL* model = get_test_wt_aerodynamic_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        model->set_nominal_wind_speed_in_mps(13.0);
        TEST_ASSERT(fabs(model->get_nominal_wind_speed_in_mps()-13.0)<FLOAT_EPSILON);
        model->set_nominal_wind_speed_in_mps(18.0);
        TEST_ASSERT(fabs(model->get_nominal_wind_speed_in_mps()-18.0)<FLOAT_EPSILON);
    }
    else
        TEST_ASSERT(false);
}

void WT_AERODYNAMIC_MODEL_TEST::test_set_get_nominal_air_density()
{
    WT_AERODYNAMIC_MODEL* model = get_test_wt_aerodynamic_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        model->set_nominal_air_density_in_kgpm3(1.2);
        TEST_ASSERT(fabs(model->get_nominal_air_density_in_kgpm3()-1.2)<FLOAT_EPSILON);
        model->set_nominal_air_density_in_kgpm3(1.5);
        TEST_ASSERT(fabs(model->get_nominal_air_density_in_kgpm3()-1.5)<FLOAT_EPSILON);
    }
    else
        TEST_ASSERT(false);
}

void WT_AERODYNAMIC_MODEL_TEST::test_get_nominal_turbine_speed()
{
    WT_AERODYNAMIC_MODEL* model = get_test_wt_aerodynamic_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        POWER_SYSTEM_DATABASE* psdb = get_test_power_system_database();
        if(psdb!=NULL)
        {
            double fbase = psdb->get_system_base_frequency_in_Hz();
            size_t npole = model->get_number_of_pole_pairs();
            double turn_ratio = model->get_generator_to_turbine_gear_ratio();
            double w = 2*PI*fbase;
            w = w/npole/turn_ratio;
            TEST_ASSERT(fabs(model->get_nominal_turbine_speed_in_rad_per_s()-w)<FLOAT_EPSILON);
        }
        else
            TEST_ASSERT(false);
    }
    else
        TEST_ASSERT(false);
}

void WT_AERODYNAMIC_MODEL_TEST::test_set_get_initial_pitch_angle()
{
    WT_AERODYNAMIC_MODEL* model = get_test_wt_aerodynamic_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        model->set_initial_pitch_angle_in_deg(0.0);
        TEST_ASSERT(fabs(model->get_initial_pitch_angle_in_deg()-0.0)<FLOAT_EPSILON);
        model->set_initial_pitch_angle_in_deg(1.0);
        TEST_ASSERT(fabs(model->get_initial_pitch_angle_in_deg()-1.0)<FLOAT_EPSILON);
    }
    else
        TEST_ASSERT(false);
}

void WT_AERODYNAMIC_MODEL_TEST::test_set_get_initial_turbine_speed()
{
    WT_AERODYNAMIC_MODEL* model = get_test_wt_aerodynamic_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        model->set_initial_turbine_speed_in_rad_per_s(2.0);
        TEST_ASSERT(fabs(model->get_initial_turbine_speed_in_rad_per_s()-2.0)<FLOAT_EPSILON);
        model->set_initial_turbine_speed_in_rad_per_s(0.0);
        TEST_ASSERT(fabs(model->get_initial_turbine_speed_in_rad_per_s()-0.0)<FLOAT_EPSILON);
    }
    else
        TEST_ASSERT(false);
}

void WT_AERODYNAMIC_MODEL_TEST::test_set_get_air_density()
{
    WT_AERODYNAMIC_MODEL* model = get_test_wt_aerodynamic_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        model->set_air_density_in_kgpm3(1.2);
        TEST_ASSERT(fabs(model->get_air_density_in_kgpm3()-1.2)<FLOAT_EPSILON);
        model->set_air_density_in_kgpm3(1.5);
        TEST_ASSERT(fabs(model->get_air_density_in_kgpm3()-1.5)<FLOAT_EPSILON);
    }
    else
        TEST_ASSERT(false);
}

void WT_AERODYNAMIC_MODEL_TEST::test_set_get_overspeed_mode_flag()
{
    WT_AERODYNAMIC_MODEL* model = get_test_wt_aerodynamic_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        model->set_overspeed_mode_flag(false);
        TEST_ASSERT(model->get_overspeed_mode_flag()==false);
        model->set_overspeed_mode_flag(true);
        TEST_ASSERT(model->get_overspeed_mode_flag()==true);
    }
    else
        TEST_ASSERT(false);
}

void WT_AERODYNAMIC_MODEL_TEST::test_set_get_max_steady_state_turbine_speed()
{
    WT_AERODYNAMIC_MODEL* model = get_test_wt_aerodynamic_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        model->set_max_steady_state_turbine_speed_in_pu(1.2);
        TEST_ASSERT(fabs(model->get_max_steady_state_turbine_speed_in_pu()-1.2)<FLOAT_EPSILON);
        model->set_max_steady_state_turbine_speed_in_pu(1.5);
        TEST_ASSERT(fabs(model->get_max_steady_state_turbine_speed_in_pu()-1.5)<FLOAT_EPSILON);
    }
    else
        TEST_ASSERT(false);
}

void WT_AERODYNAMIC_MODEL_TEST::test_set_get_min_steady_state_turbine_speed()
{
    WT_AERODYNAMIC_MODEL* model = get_test_wt_aerodynamic_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        model->set_min_steady_state_turbine_speed_in_pu(0.6);
        TEST_ASSERT(fabs(model->get_min_steady_state_turbine_speed_in_pu()-0.6)<FLOAT_EPSILON);
        model->set_min_steady_state_turbine_speed_in_pu(0.5);
        TEST_ASSERT(fabs(model->get_min_steady_state_turbine_speed_in_pu()-0.5)<FLOAT_EPSILON);
    }
    else
        TEST_ASSERT(false);
}


void WT_AERODYNAMIC_MODEL_TEST::test_get_wind_speed()
{
    WT_AERODYNAMIC_MODEL* model = get_test_wt_aerodynamic_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        WIND_SPEED_MODEL* windmodel = get_test_wind_speed_model();
        if(windmodel!=NULL)
        {
            TEST_ASSERT(fabs(model->get_wind_speed_in_mps()-windmodel->get_wind_speed_in_mps())<FLOAT_EPSILON);
        }
        else
        {
            model->set_nominal_wind_speed_in_mps(10.0);
            TEST_ASSERT(fabs(model->get_wind_speed_in_mps()-model->get_nominal_wind_speed_in_mps())<FLOAT_EPSILON);
            model->set_nominal_wind_speed_in_mps(12.0);
            TEST_ASSERT(fabs(model->get_wind_speed_in_mps()-model->get_nominal_wind_speed_in_mps())<FLOAT_EPSILON);
        }
    }
    else
        TEST_ASSERT(false);
}

void WT_AERODYNAMIC_MODEL_TEST::test_get_pitch_angle()
{
    WT_AERODYNAMIC_MODEL* model = get_test_wt_aerodynamic_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        WT_PITCH_MODEL* pitchmodel = get_test_wt_pitch_model();
        if(pitchmodel!=NULL)
        {
            TEST_ASSERT(fabs(model->get_pitch_angle_in_deg()-pitchmodel->get_pitch_angle_in_deg())<FLOAT_EPSILON);
        }
        else
        {
            TEST_ASSERT(fabs(model->get_pitch_angle_in_deg()-0.0)<FLOAT_EPSILON);
        }
    }
    else
        TEST_ASSERT(false);
}

void WT_AERODYNAMIC_MODEL_TEST::test_get_turbine_frequency()
{
    WT_AERODYNAMIC_MODEL* model = get_test_wt_aerodynamic_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        WT_TURBINE_MODEL* turbinemodel = get_test_wt_turbine_model();
        if(turbinemodel!=NULL)
        {
            double speed = turbinemodel->get_wind_turbine_generator_speed_in_pu();
            speed *= model->get_nominal_turbine_speed_in_rad_per_s();
            speed /=(2.0*PI);
            TEST_ASSERT(fabs(model->get_turbine_frequency_in_Hz()-speed)<FLOAT_EPSILON);
        }
        else
        {
            TEST_ASSERT(fabs(model->get_turbine_frequency_in_Hz()-radps2hz(model->get_initial_turbine_speed_in_rad_per_s()))<FLOAT_EPSILON);
        }
    }
    else
        TEST_ASSERT(false);
}

void WT_AERODYNAMIC_MODEL_TEST::test_get_turbine_speed()
{
    WT_AERODYNAMIC_MODEL* model = get_test_wt_aerodynamic_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        WT_TURBINE_MODEL* turbinemodel = get_test_wt_turbine_model();
        if(turbinemodel!=NULL)
        {
            double speed = turbinemodel->get_wind_turbine_generator_speed_in_pu();
            speed *= model->get_nominal_turbine_speed_in_rad_per_s();
            TEST_ASSERT(fabs(model->get_turbine_speed_in_rad_per_s()-speed)<FLOAT_EPSILON);
        }
        else
        {
            TEST_ASSERT(fabs(model->get_turbine_speed_in_rad_per_s()-model->get_initial_turbine_speed_in_rad_per_s())<FLOAT_EPSILON);
        }
    }
    else
        TEST_ASSERT(false);
}


void WT_AERODYNAMIC_MODEL_TEST::test_get_standard_model_string()
{
    WT_AERODYNAMIC_MODEL* model = get_test_wt_aerodynamic_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        model->get_standard_model_string();
    }
    else
        TEST_ASSERT(false);
}


#include "header/model/wtg_models/wt_aerodynamic_model/wt_aerodynamic_model_test.h"
#include "header/basic/utility.h"
#include "header/model/wtg_models/wt_generator_model/wt3g2.h"
#include <cstdlib>
#include <cstring>
#include <istream>
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
    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_set_get_turbine_speed_mode);
    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_set_get_max_steady_state_turbine_speed);
    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_set_get_min_steady_state_turbine_speed);

    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_get_wind_speed);
    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_get_pitch_angle);
    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_get_turbine_frequency);
    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_get_turbine_speed);

    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_get_standard_model_string);

    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_initialize_and_get_initialized_inputs_with_underspeed_mode);
    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_initialize_and_get_initialized_inputs_with_overspeed_mode);
    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_initialize_and_get_initialized_inputs_with_mppt_mode);
    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_list_Cp_and_mechanical_power_data_of_different_wind_speed);
}

void WT_AERODYNAMIC_MODEL_TEST::setup()
{
    WTG_MODEL_TEST::setup();

    WT_GENERATOR* wt_gen = get_test_wt_generator();
    wt_gen->set_p_generation_in_MW(28.0);
    wt_gen->set_rated_power_per_wt_generator_in_MW(1.5);
    wt_gen->set_number_of_lumped_wt_generators(20);

    WT3G2 genmodel;
    genmodel.set_converter_activer_current_command_T_in_s(0.2);
    genmodel.set_converter_reactiver_voltage_command_T_in_s(0.2);
    genmodel.set_KPLL(20.0);
    genmodel.set_KIPLL(10.0);
    genmodel.set_PLLmax(0.1);
    LVPL lvpl;
    lvpl.set_low_voltage_in_pu(0.5);
    lvpl.set_high_voltage_in_pu(0.8);
    lvpl.set_gain_at_high_voltage(20.0);
    genmodel.set_LVPL(lvpl);
    genmodel.set_HVRC_voltage_in_pu(0.8);
    genmodel.set_HVRC_current_in_pu(20.0);
    genmodel.set_LVPL_max_rate_of_active_current_change(0.2);
    genmodel.set_LVPL_voltage_sensor_T_in_s(0.1);

    wt_gen->set_model(&genmodel);
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

void WT_AERODYNAMIC_MODEL_TEST::test_set_get_turbine_speed_mode()
{
    WT_AERODYNAMIC_MODEL* model = get_test_wt_aerodynamic_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        model->set_turbine_speed_mode(WT_UNDERSPEED_MODE);
        TEST_ASSERT(model->get_turbine_speed_mode()==WT_UNDERSPEED_MODE);
        model->set_turbine_speed_mode(WT_MPPT_MODE);
        TEST_ASSERT(model->get_turbine_speed_mode()==WT_MPPT_MODE);
        model->set_turbine_speed_mode(WT_OVERSPEED_MODE);
        TEST_ASSERT(model->get_turbine_speed_mode()==WT_OVERSPEED_MODE);
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
            TEST_ASSERT(fabs(model->get_pitch_angle_in_deg()-model->get_initial_pitch_angle_in_deg())<FLOAT_EPSILON);
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
            double speed = turbinemodel->get_turbine_speed_in_pu();
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
            double speed = turbinemodel->get_turbine_speed_in_pu();
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


void WT_AERODYNAMIC_MODEL_TEST::test_initialize_and_get_initialized_inputs_with_underspeed_mode()
{
    show_test_information_for_function_of_class(__FUNCTION__,"AERD0_TEST");
    set_dynamic_simulation_time_step_in_s(0.001);
    ostringstream osstream;

    WT_GENERATOR_MODEL* wtgenmodel = get_test_wt_generator_model();
    WT_AERODYNAMIC_MODEL* model = get_test_wt_aerodynamic_model();
    model->set_turbine_speed_mode(WT_UNDERSPEED_MODE);

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
        osstream<<model->get_model_name()<<" model after initialized with generation of each turbine: "<<gen->get_p_generation_in_MW()/gen->get_number_of_lumped_wt_generators()<<" MW"<<endl;
        osstream<<"Turbine blade radius = "<<model->get_turbine_blade_radius_in_m()<<" m, generator/turbine turn ratio = "<<model->get_generator_to_turbine_gear_ratio()<<endl
               <<"Pitch angle = "<<model->get_initial_pitch_angle_in_deg()<<" deg, turbine speed = "<<model->get_initial_turbine_speed_in_rad_per_s()<<" rad/s ("
               <<model->get_initial_turbine_speed_in_pu()<<" pu)";
        show_information_with_leading_time_stamp(osstream);
    }
}

void WT_AERODYNAMIC_MODEL_TEST::test_initialize_and_get_initialized_inputs_with_mppt_mode()
{
    show_test_information_for_function_of_class(__FUNCTION__,"AERD0_TEST");
    set_dynamic_simulation_time_step_in_s(0.001);
    ostringstream osstream;

    WT_GENERATOR_MODEL* wtgenmodel = get_test_wt_generator_model();
    WT_AERODYNAMIC_MODEL* model = get_test_wt_aerodynamic_model();
    model->set_turbine_speed_mode(WT_MPPT_MODE);

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
        osstream<<model->get_model_name()<<" model after initialized with generation of each turbine: "<<gen->get_p_generation_in_MW()/gen->get_number_of_lumped_wt_generators()<<" MW"<<endl;
        osstream<<"Turbine blade radius = "<<model->get_turbine_blade_radius_in_m()<<" m, generator/turbine turn ratio = "<<model->get_generator_to_turbine_gear_ratio()<<endl
               <<"Pitch angle = "<<model->get_initial_pitch_angle_in_deg()<<" deg, turbine speed = "<<model->get_initial_turbine_speed_in_rad_per_s()<<" rad/s ("
               <<model->get_initial_turbine_speed_in_pu()<<" pu)";
        show_information_with_leading_time_stamp(osstream);
    }
}


void WT_AERODYNAMIC_MODEL_TEST::test_initialize_and_get_initialized_inputs_with_overspeed_mode()
{
    show_test_information_for_function_of_class(__FUNCTION__,"AERD0_TEST");
    set_dynamic_simulation_time_step_in_s(0.001);
    ostringstream osstream;

    WT_GENERATOR_MODEL* wtgenmodel = get_test_wt_generator_model();
    WT_AERODYNAMIC_MODEL* model = get_test_wt_aerodynamic_model();
    model->set_turbine_speed_mode(WT_OVERSPEED_MODE);

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
        osstream<<model->get_model_name()<<" model after initialized with generation of each turbine: "<<gen->get_p_generation_in_MW()/gen->get_number_of_lumped_wt_generators()<<" MW"<<endl;
        osstream<<"Turbine blade radius = "<<model->get_turbine_blade_radius_in_m()<<" m, generator/turbine turn ratio = "<<model->get_generator_to_turbine_gear_ratio()<<endl
               <<"Pitch angle = "<<model->get_initial_pitch_angle_in_deg()<<" deg, turbine speed = "<<model->get_initial_turbine_speed_in_rad_per_s()<<" rad/s ("
               <<model->get_initial_turbine_speed_in_pu()<<" pu)";
        show_information_with_leading_time_stamp(osstream);
    }
}

void WT_AERODYNAMIC_MODEL_TEST::test_list_Cp_and_mechanical_power_data_of_different_wind_speed()
{
    ostringstream osstream;

    WT_AERODYNAMIC_MODEL* model = get_test_wt_aerodynamic_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        model->initialize();
        string file =  "test_log/test_"+model->get_model_name()+"_"+__FUNCTION__+".txt";
        redirect_stdout_to_file(file);

        osstream<<model->get_standard_model_string();
        show_information_with_leading_time_stamp(osstream);

        double r = model->get_turbine_blade_radius_in_m();
        double vwind = model->get_nominal_wind_speed_in_mps();
        double pitch = 0.0;
        double pmax = model->get_total_wind_power_per_wt_generator_in_MW(vwind);
        osstream<<"Power curve when pitch = 0.0 deg, and wind speed = "<<vwind<<" m/s, blade radius = "<<r<<" m";
        show_information_with_leading_time_stamp(osstream);
        osstream<<"Speed(rad/s)\tLambda\tCP\tPmech(MW)";
        show_information_with_leading_time_stamp(osstream);

        for(double w=0.1; w<4.0*PI; w+=0.1)
        {
            double lambda = w*r/vwind;
            double cp = model->get_Cp(lambda, pitch);
            if(cp<0.0)
                break;
            double pmech = pmax*cp;
            osstream<<setw(10)<<setprecision(6)<<w<<"\t"
                   <<setw(10)<<setprecision(6)<<lambda<<"\t"
                   <<setw(10)<<setprecision(6)<<cp<<"\t"
                   <<setw(10)<<setprecision(6)<<pmech;
            show_information_with_leading_time_stamp(osstream);
        }
        recover_stdout();
    }
    else
        TEST_ASSERT(false);
}

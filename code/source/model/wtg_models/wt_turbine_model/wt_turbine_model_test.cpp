#include "header/model/wtg_models/wt_turbine_model/wt_turbine_model_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/model/wtg_models/wt_generator_model/wt3g2.h"
#include "header/model/wtg_models/wt_aerodynamic_model/aerd0.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cstdio>
#include <cmath>

using namespace std;

WT_TURBINE_MODEL_TEST::WT_TURBINE_MODEL_TEST()
{
    TEST_ADD(WT_TURBINE_MODEL_TEST::test_get_model_type);
    TEST_ADD(WT_TURBINE_MODEL_TEST::test_set_get_damping);
    TEST_ADD(WT_TURBINE_MODEL_TEST::test_get_standard_model_string);
    TEST_ADD(WT_TURBINE_MODEL_TEST::test_step_response_of_wt_turbine_model_with_pitch_angle_increase);
    TEST_ADD(WT_TURBINE_MODEL_TEST::test_step_response_of_wt_turbine_model_with_generator_power_order_drop);
}

void WT_TURBINE_MODEL_TEST::setup()
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

    AERD0 aeromodel;
    aeromodel.set_number_of_pole_pairs(2);
    aeromodel.set_generator_to_turbine_gear_ratio(100.0);
    aeromodel.set_gear_efficiency(1.0);
    aeromodel.set_turbine_blade_radius_in_m(25.0);
    aeromodel.set_nominal_wind_speed_in_mps(13.0);
    aeromodel.set_nominal_air_density_in_kgpm3(1.25);
    aeromodel.set_air_density_in_kgpm3(1.25);
    aeromodel.set_overspeed_mode_flag(false);

    aeromodel.set_C1(0.22);
    aeromodel.set_C2(116.0);
    aeromodel.set_C3(0.4);
    aeromodel.set_C4(5.0);
    aeromodel.set_C5(-12.5);
    aeromodel.set_C6(0.0);

    aeromodel.set_C1(0.5176);
    aeromodel.set_C2(116.0);
    aeromodel.set_C3(0.4);
    aeromodel.set_C4(5.0);
    aeromodel.set_C5(-21.0);
    aeromodel.set_C6(0.0068);

    wt_gen->set_model(&aeromodel);
}

void WT_TURBINE_MODEL_TEST::tear_down()
{
    WTG_MODEL_TEST::tear_down();
}

void WT_TURBINE_MODEL_TEST::test_get_model_type()
{
    WT_TURBINE_MODEL* model = get_test_wt_turbine_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        TEST_ASSERT(model->get_model_type()=="WT TURBINE");
    }
    else
        TEST_ASSERT(false);
}

void WT_TURBINE_MODEL_TEST::test_set_get_damping()
{
    WT_TURBINE_MODEL* model = get_test_wt_turbine_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        model->set_damping_in_pu(0.0);
        TEST_ASSERT(fabs(model->get_damping_in_pu()-0.0)<FLOAT_EPSILON);
        model->set_damping_in_pu(1.0);
        TEST_ASSERT(fabs(model->get_damping_in_pu()-1.0)<FLOAT_EPSILON);
    }
    else
        TEST_ASSERT(false);
}

void WT_TURBINE_MODEL_TEST::test_get_standard_model_string()
{
    WT_TURBINE_MODEL* model = get_test_wt_turbine_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        model->get_standard_model_string();
    }
    else
        TEST_ASSERT(false);
}

void WT_TURBINE_MODEL_TEST::test_step_response_of_wt_turbine_model_with_pitch_angle_increase()
{
    WT_TURBINE_MODEL* model = get_test_wt_turbine_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");
        redirect_stdout_to_file("test_log/"+model->get_model_name()+"_"+__FUNCTION__+".txt");
        run_step_response_of_wt_turbine_model_with_pitch_angle_increase();
        recover_stdout();
    }
    else
        TEST_ASSERT(false);
}

void WT_TURBINE_MODEL_TEST::test_step_response_of_wt_turbine_model_with_generator_power_order_drop()
{
    WT_TURBINE_MODEL* model = get_test_wt_turbine_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");
        redirect_stdout_to_file("test_log/"+model->get_model_name()+"_"+__FUNCTION__+".txt");
        run_step_response_of_wt_turbine_model_with_generator_power_order_drop();
        recover_stdout();
    }
    else
        TEST_ASSERT(false);
}


void WT_TURBINE_MODEL_TEST::run_step_response_of_wt_turbine_model_with_pitch_angle_increase()
{
    ostringstream sstream;
    WT_GENERATOR* genptr = get_test_wt_generator();

    WT_GENERATOR_MODEL* genmodel = get_test_wt_generator_model();
    genmodel->initialize();

    WT_ELECTRICAL_MODEL* elecmodel = get_test_wt_electrical_model();
    elecmodel->initialize();

    WT_TURBINE_MODEL*model = get_test_wt_turbine_model();

    sstream<<"Model:"<<model->get_standard_model_string()<<endl;
    show_information_with_leading_time_stamp(sstream);

    double delt = 0.001;
    set_dynamic_simulation_time_step_in_s(delt);

    STEPS::TIME = -delt*2.0;
    double generator_speed, turbine_speed;

    model->initialize();
    generator_speed = model->get_generator_speed_in_pu();

    export_meter_title();
    export_meter_values();
    while(true)
    {
        STEPS::TIME += delt;
        if(STEPS::TIME>1.0+FLOAT_EPSILON)
        {
            STEPS::TIME -=delt;
            break;
        }
        generator_speed =  model->get_generator_speed_in_pu();
        while(true)
        {
            model->run(INTEGRATE_MODE);
            if(fabs(generator_speed-model->get_generator_speed_in_pu())>1e-6)
                generator_speed = model->get_generator_speed_in_pu();
            else
                break;
        }
        model->run(UPDATE_MODE);

        export_meter_values();
    }

    apply_1deg_pitch_angle_increase();
    model->run(UPDATE_MODE);
    export_meter_values();

    while(true)
    {
        STEPS::TIME += delt;

        if(STEPS::TIME>6.0+FLOAT_EPSILON)
        {
            STEPS::TIME -=delt;
            break;
        }
        generator_speed =  model->get_generator_speed_in_pu();
        while(true)
        {
            model->run(INTEGRATE_MODE);
            if(fabs(generator_speed-model->get_generator_speed_in_pu())>1e-6)
                generator_speed = model->get_generator_speed_in_pu();
            else
                break;
        }
        model->run(UPDATE_MODE);

        export_meter_values();
    }
}

void WT_TURBINE_MODEL_TEST::apply_1deg_pitch_angle_increase()
{
    WT_AERODYNAMIC_MODEL* aero_model = get_test_wt_aerodynamic_model();
    aero_model->set_initial_pitch_angle_in_deg(aero_model->get_initial_pitch_angle_in_deg()+1.0);
}

void WT_TURBINE_MODEL_TEST::run_step_response_of_wt_turbine_model_with_generator_power_order_drop()
{
    ostringstream sstream;
    WT_GENERATOR* genptr = get_test_wt_generator();

    WT_GENERATOR_MODEL* genmodel = get_test_wt_generator_model();
    genmodel->initialize();

    WT_ELECTRICAL_MODEL* elecmodel = get_test_wt_electrical_model();
    elecmodel->initialize();

    WT_TURBINE_MODEL*model = get_test_wt_turbine_model();

    sstream<<"Model:"<<model->get_standard_model_string()<<endl;
    show_information_with_leading_time_stamp(sstream);

    double delt = 0.001;
    set_dynamic_simulation_time_step_in_s(delt);

    STEPS::TIME = -delt*2.0;
    double generator_speed, turbine_speed;

    model->initialize();
    generator_speed = model->get_generator_speed_in_pu();

    export_meter_title();
    export_meter_values();
    while(true)
    {
        STEPS::TIME += delt;
        if(STEPS::TIME>1.0+FLOAT_EPSILON)
        {
            STEPS::TIME -=delt;
            break;
        }
        generator_speed =  model->get_generator_speed_in_pu();
        while(true)
        {
            model->run(INTEGRATE_MODE);
            if(fabs(generator_speed-model->get_generator_speed_in_pu())>1e-6)
                generator_speed = model->get_generator_speed_in_pu();
            else
                break;
        }
        model->run(UPDATE_MODE);

        export_meter_values();
    }

    apply_10_percent_power_order_drop();
    model->run(UPDATE_MODE);
    export_meter_values();

    while(true)
    {
        STEPS::TIME += delt;

        if(STEPS::TIME>6.0+FLOAT_EPSILON)
        {
            STEPS::TIME -=delt;
            break;
        }
        generator_speed =  model->get_generator_speed_in_pu();
        while(true)
        {
            model->run(INTEGRATE_MODE);
            if(fabs(generator_speed-model->get_generator_speed_in_pu())>1e-6)
                generator_speed = model->get_generator_speed_in_pu();
            else
                break;
        }
        model->run(UPDATE_MODE);

        export_meter_values();
    }
}

void WT_TURBINE_MODEL_TEST::apply_10_percent_power_order_drop()
{
    WT_GENERATOR_MODEL* genmodel = get_test_wt_generator_model();
    genmodel->set_initial_active_current_command_in_pu_based_on_mbase(genmodel->get_initial_active_current_command_in_pu_based_on_mbase()*0.9);
}


void WT_TURBINE_MODEL_TEST::export_meter_title()
{
    ostringstream sstream;
    sstream<<"TIME\tPELEC\tPMECH\tTSPEED\tGSPEED\tANGLE";
    show_information_with_leading_time_stamp(sstream);
}

void WT_TURBINE_MODEL_TEST::export_meter_values()
{
    ostringstream sstream;

    WT_TURBINE_MODEL* model = get_test_wt_turbine_model();

    sstream<<setw(10)<<setprecision(6)<<STEPS::TIME<<"\t"
           <<setw(10)<<setprecision(6)<<model->get_wt_generator_active_power_generation_in_MW()<<"\t"
           <<setw(10)<<setprecision(6)<<model->get_mechanical_power_in_pu_from_wt_aerodynamic_model()*model->get_mbase_in_MVA()<<"\t"
           <<setw(10)<<setprecision(6)<<model->get_turbine_speed_in_pu()<<"\t"
           <<setw(10)<<setprecision(6)<<model->get_generator_speed_in_pu()<<"\t"
           <<setw(10)<<setprecision(6)<<model->get_rotor_angle_in_deg();
    show_information_with_leading_time_stamp(sstream);
}


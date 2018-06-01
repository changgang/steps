#include "header/model/wtg_models/wt_turbine_model/wt_turbine_model_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
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
}

void WT_TURBINE_MODEL_TEST::setup()
{
    WTG_MODEL_TEST::setup();
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


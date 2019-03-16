#include "header/model/wtg_models/wt_pitch_model/wt_pitch_model_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/model/wtg_models/wt_generator_model/wt3g0.h"
#include "header/model/wtg_models/wt_aerodynamic_model/aerd0.h"
#include "header/model/wtg_models/wt_turbine_model/wt3t0.h"
#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <cstdio>
#include <cmath>

using namespace std;

WT_PITCH_MODEL_TEST::WT_PITCH_MODEL_TEST()
{
    TEST_ADD(WT_PITCH_MODEL_TEST::test_get_model_type);
    TEST_ADD(WT_PITCH_MODEL_TEST::test_get_wt_generator_speed);
    TEST_ADD(WT_PITCH_MODEL_TEST::test_get_wt_generator_reference_speed);
    TEST_ADD(WT_PITCH_MODEL_TEST::test_get_bus_frequency);
    TEST_ADD(WT_PITCH_MODEL_TEST::test_get_bus_frequency_deviation);
    TEST_ADD(WT_PITCH_MODEL_TEST::test_get_initial_pitch_angle_in_deg_from_wt_aerodynamic_model);
    TEST_ADD(WT_PITCH_MODEL_TEST::test_set_get_frequency_upper_deadband);
    TEST_ADD(WT_PITCH_MODEL_TEST::test_set_get_frequency_lower_deadband);
    TEST_ADD(WT_PITCH_MODEL_TEST::test_get_standard_model_string);
    TEST_ADD(WT_PITCH_MODEL_TEST::test_step_response_with_active_power_order_drop);
    TEST_ADD(WT_PITCH_MODEL_TEST::test_step_response_with_frequency_drop);
}

void WT_PITCH_MODEL_TEST::setup()
{
    WTG_MODEL_TEST::setup();

    WT_GENERATOR* wt_gen = get_test_wt_generator();
    wt_gen->set_p_generation_in_MW(28.0);
    wt_gen->set_rated_power_per_wt_generator_in_MW(1.5);
    wt_gen->set_number_of_lumped_wt_generators(20);

    WT3G0 genmodel;
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
    aeromodel.set_turbine_speed_mode(WT_UNDERSPEED_MODE);

    aeromodel.set_C1(0.22);
    aeromodel.set_C2(116.0);
    aeromodel.set_C3(0.4);
    aeromodel.set_C4(5.0);
    aeromodel.set_C5(12.5);
    aeromodel.set_C6(0.0);

    aeromodel.set_C1(0.5176);
    aeromodel.set_C2(116.0);
    aeromodel.set_C3(0.4);
    aeromodel.set_C4(5.0);
    aeromodel.set_C5(21.0);
    aeromodel.set_C6(0.0068);

    wt_gen->set_model(&aeromodel);

    WT3T0 model;
    model.set_Hturbine_in_s(5.0);
    model.set_Hgenerator_in_s(3.0);
    model.set_Kshaft_in_pu(20.0);
    model.set_damping_in_pu(0.01);
    model.set_Dshaft_in_pu(1.0);

    wt_gen->set_model(&model);
}

void WT_PITCH_MODEL_TEST::tear_down()
{
    WTG_MODEL_TEST::tear_down();
}

void WT_PITCH_MODEL_TEST::test_get_model_type()
{
    WT_PITCH_MODEL* model = get_test_wt_pitch_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        TEST_ASSERT(model->get_model_type()=="WT PITCH");
    }
    else
        TEST_ASSERT(false);
}


void WT_PITCH_MODEL_TEST::test_get_wt_generator_speed()
{
    WT_PITCH_MODEL* model = get_test_wt_pitch_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        WT_TURBINE_MODEL* turbine = get_test_wt_turbine_model();
        if(turbine==NULL)
        {
            TEST_ASSERT(false);
        }
        else
        {
            if(turbine->is_model_initialized())
            {
                TEST_ASSERT(fabs(model->get_wt_generator_speed_in_pu()-turbine->get_generator_speed_in_pu())<FLOAT_EPSILON);
            }
        }
    }
    else
        TEST_ASSERT(false);
}

void WT_PITCH_MODEL_TEST::test_get_wt_generator_reference_speed()
{
    WT_PITCH_MODEL* model = get_test_wt_pitch_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        WT_AERODYNAMIC_MODEL* aerd = get_test_wt_aerodynamic_model();
        if(aerd==NULL)
        {
            TEST_ASSERT(false);
        }
        else
        {
            if(aerd->is_model_initialized())
            {
                TEST_ASSERT(fabs(model->get_wt_generator_reference_speed_in_pu()-aerd->get_turbine_reference_speed_in_pu())<FLOAT_EPSILON);
            }
        }
    }
    else
        TEST_ASSERT(false);
}

void WT_PITCH_MODEL_TEST::test_get_bus_frequency()
{
    WT_PITCH_MODEL* model = get_test_wt_pitch_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        WT_GENERATOR* gen = get_test_wt_generator();
        size_t bus = gen->get_generator_bus();

        POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
        BUS* busptrr = psdb.get_bus(bus);

        TEST_ASSERT(fabs(model->get_bus_frequency_in_pu()-psdb.get_bus_frequency_in_pu(bus))<FLOAT_EPSILON);
        BUS_FREQUENCY_MODEL* freqmodel = busptrr->get_bus_frequency_model();
        freqmodel->set_frequency_deviation_in_pu(0.05);
        TEST_ASSERT(fabs(model->get_bus_frequency_in_pu()-psdb.get_bus_frequency_in_pu(bus))<FLOAT_EPSILON);
    }
    else
        TEST_ASSERT(false);
}

void WT_PITCH_MODEL_TEST::test_get_bus_frequency_deviation()
{
    WT_PITCH_MODEL* model = get_test_wt_pitch_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        WT_GENERATOR* gen = get_test_wt_generator();
        size_t bus = gen->get_generator_bus();

        POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
        BUS* busptrr = psdb.get_bus(bus);

        TEST_ASSERT(fabs(model->get_bus_frequency_deviation_in_pu()-psdb.get_bus_frequency_deviation_in_pu(bus))<FLOAT_EPSILON);
        BUS_FREQUENCY_MODEL* freqmodel = busptrr->get_bus_frequency_model();
        freqmodel->set_frequency_deviation_in_pu(0.05);
        TEST_ASSERT(fabs(model->get_bus_frequency_deviation_in_pu()-psdb.get_bus_frequency_deviation_in_pu(bus))<FLOAT_EPSILON);
    }
    else
        TEST_ASSERT(false);
}

void WT_PITCH_MODEL_TEST::test_get_initial_pitch_angle_in_deg_from_wt_aerodynamic_model()
{
    WT_PITCH_MODEL* model = get_test_wt_pitch_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        WT_AERODYNAMIC_MODEL* aerd = get_test_wt_aerodynamic_model();
        if(aerd==NULL)
        {
            TEST_ASSERT(false);
        }
        else
        {
            if(aerd->is_model_initialized())
            {
                TEST_ASSERT(fabs(model->get_initial_pitch_angle_in_deg_from_wt_aerodynamic_model()-aerd->get_initial_pitch_angle_in_deg())<FLOAT_EPSILON);
            }
        }
    }
    else
        TEST_ASSERT(false);
}

void WT_PITCH_MODEL_TEST::test_set_get_frequency_upper_deadband()
{
    WT_PITCH_MODEL* model = get_test_wt_pitch_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");
        model->set_frequency_upper_deadband_in_pu(1.0);
        TEST_ASSERT(fabs(model->get_frequency_upper_deadband_in_pu()-1.0)<FLOAT_EPSILON);
        model->set_frequency_upper_deadband_in_pu(1.1);
        TEST_ASSERT(fabs(model->get_frequency_upper_deadband_in_pu()-1.1)<FLOAT_EPSILON);
    }
    else
        TEST_ASSERT(false);
}

void WT_PITCH_MODEL_TEST::test_set_get_frequency_lower_deadband()
{
    WT_PITCH_MODEL* model = get_test_wt_pitch_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");
        model->set_frequency_lower_deadband_in_pu(1.0);
        TEST_ASSERT(fabs(model->get_frequency_lower_deadband_in_pu()-1.0)<FLOAT_EPSILON);
        model->set_frequency_lower_deadband_in_pu(0.9);
        TEST_ASSERT(fabs(model->get_frequency_lower_deadband_in_pu()-0.9)<FLOAT_EPSILON);
    }
    else
        TEST_ASSERT(false);
}

void WT_PITCH_MODEL_TEST::test_get_standard_model_string()
{
    WT_PITCH_MODEL* model = get_test_wt_pitch_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");
        show_information_with_leading_time_stamp(model->get_standard_model_string());
    }
    else
        TEST_ASSERT(false);
}

void WT_PITCH_MODEL_TEST::test_step_response_with_active_power_order_drop()
{
    WT_PITCH_MODEL* model = get_test_wt_pitch_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");
        redirect_stdout_to_file("test_log/"+model->get_model_name()+"_"+__FUNCTION__+".txt");

        initialize_models();
        run_to_time(1.0);

        apply_active_power_order_drop_of_5_percent();

        run_to_time(6.0);

        recover_stdout();
    }
    else
        TEST_ASSERT(false);
}

void WT_PITCH_MODEL_TEST::test_step_response_with_frequency_drop()
{
    WT_PITCH_MODEL* model = get_test_wt_pitch_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");
        redirect_stdout_to_file("test_log/"+model->get_model_name()+"_"+__FUNCTION__+".txt");

        initialize_models();
        run_to_time(1.0);

        apply_frequency_drop_of_5_percent();

        run_to_time(6.0);

        recover_stdout();
    }
    else
        TEST_ASSERT(false);
}


void WT_PITCH_MODEL_TEST::apply_active_power_order_drop_of_5_percent()
{
    WT_GENERATOR_MODEL* genmodel = get_test_wt_generator_model();
    genmodel->set_initial_active_current_command_in_pu_based_on_mbase(genmodel->get_initial_active_current_command_in_pu_based_on_mbase()*0.95);
    update_models();
}

void WT_PITCH_MODEL_TEST::apply_frequency_drop_of_5_percent()
{
    WT_GENERATOR* gen = get_test_wt_generator();
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    size_t bus = gen->get_generator_bus();
    BUS* busptr = psdb.get_bus(bus);

    BUS_FREQUENCY_MODEL* model = busptr->get_bus_frequency_model();
    model->set_frequency_deviation_in_pu(0.05);

    update_models();
}

void WT_PITCH_MODEL_TEST::initialize_models()
{
    double delt = 0.001;
    set_dynamic_simulation_time_step_in_s(delt);
    STEPS::TIME = -2.0*delt;

    WT_GENERATOR_MODEL* genmodel = get_test_wt_generator_model();
    WT_AERODYNAMIC_MODEL* aerd = get_test_wt_aerodynamic_model();
    WT_TURBINE_MODEL* turbine = get_test_wt_turbine_model();
    WT_PITCH_MODEL* model = get_test_wt_pitch_model();
    genmodel->initialize();
    aerd->initialize();
    turbine->initialize();
    model->initialize();

    export_meter_title();
    export_meter_values();
}

void WT_PITCH_MODEL_TEST::update_models()
{
    WT_GENERATOR_MODEL* genmodel = get_test_wt_generator_model();
    WT_AERODYNAMIC_MODEL* aerd = get_test_wt_aerodynamic_model();
    WT_TURBINE_MODEL* turbine = get_test_wt_turbine_model();
    WT_PITCH_MODEL* model = get_test_wt_pitch_model();
    model->run(UPDATE_MODE);
    turbine->run(UPDATE_MODE);
    aerd->run(UPDATE_MODE);
    genmodel->run(UPDATE_MODE);

    export_meter_values();
}

void WT_PITCH_MODEL_TEST::run_to_time(double tend)
{
    ostringstream oosstream;

    WT_GENERATOR_MODEL* genmodel = get_test_wt_generator_model();
    WT_AERODYNAMIC_MODEL* aerd = get_test_wt_aerodynamic_model();
    WT_TURBINE_MODEL* turbine = get_test_wt_turbine_model();
    WT_PITCH_MODEL* model = get_test_wt_pitch_model();

    double delt =get_dynamic_simulation_time_step_in_s();
    while(true)
    {
        STEPS::TIME += delt;
        if(STEPS::TIME>tend)
        {
            STEPS::TIME -= delt;
            break;
        }
        double pitch=0.0;
        while(true)
        {
            model->run(INTEGRATE_MODE);
            turbine->run(INTEGRATE_MODE);
            aerd->run(INTEGRATE_MODE);
            genmodel->run(INTEGRATE_MODE);
            if(fabs(pitch-model->get_pitch_angle_in_deg())<1e-6)
                break;

            pitch = model->get_pitch_angle_in_deg();
        }
        model->run(UPDATE_MODE);
        turbine->run(UPDATE_MODE);
        aerd->run(UPDATE_MODE);
        genmodel->run(UPDATE_MODE);
        export_meter_values();
   }
}

void WT_PITCH_MODEL_TEST::export_meter_title()
{
    ostringstream osstream;
    osstream<<"TIME\tSPEED\tSPEEDREF\tFREQ\tPITCH";
    show_information_with_leading_time_stamp(osstream);
}

void WT_PITCH_MODEL_TEST::export_meter_values()
{
    ostringstream osstream;

    WT_PITCH_MODEL* model = get_test_wt_pitch_model();

    osstream<<setw(10)<<setprecision(6)<<fixed<<STEPS::TIME<<"\t"
           <<setw(10)<<setprecision(6)<<fixed<<model->get_wt_generator_speed_in_pu()<<"\t"
           <<setw(10)<<setprecision(6)<<fixed<<model->get_wt_generator_reference_speed_in_pu()<<"\t"
           <<setw(10)<<setprecision(6)<<fixed<<model->get_bus_frequency_in_pu()<<"\t"
           <<setw(10)<<setprecision(6)<<fixed<<model->get_pitch_angle_in_deg();
    show_information_with_leading_time_stamp(osstream);
}


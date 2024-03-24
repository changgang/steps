#include "header/basic/test_macro.h"
#include "header/model/wtg_models/wt_relay_model/wt_relay_model_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/model/wtg_models/wt_generator_model/wt3g0.h"
#include "header/model/wtg_models/wt_aerodynamic_model/aerd0.h"
#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <cstdio>
#include <cmath>

#ifdef ENABLE_STEPS_TEST
using namespace std;

WT_RELAY_MODEL_TEST::WT_RELAY_MODEL_TEST()
{
    TEST_ADD(WT_RELAY_MODEL_TEST::test_get_model_type);
    TEST_ADD(WT_RELAY_MODEL_TEST::test_set_get_damping);
    TEST_ADD(WT_RELAY_MODEL_TEST::test_get_standard_psse_string);
    TEST_ADD(WT_RELAY_MODEL_TEST::test_step_response_of_wt_turbine_model_with_pitch_angle_increase_in_underspeed_mode);
    TEST_ADD(WT_RELAY_MODEL_TEST::test_step_response_of_wt_turbine_model_with_pitch_angle_increase_in_mppt_mode);
    TEST_ADD(WT_RELAY_MODEL_TEST::test_step_response_of_wt_turbine_model_with_pitch_angle_increase_in_overspeed_mode);
    TEST_ADD(WT_RELAY_MODEL_TEST::test_step_response_of_wt_turbine_model_with_generator_power_order_drop_in_underspeed_mode);
    TEST_ADD(WT_RELAY_MODEL_TEST::test_step_response_of_wt_turbine_model_with_generator_power_order_drop_in_mppt_mode);
    TEST_ADD(WT_RELAY_MODEL_TEST::test_step_response_of_wt_turbine_model_with_generator_power_order_drop_in_overspeed_mode);
}

void WT_RELAY_MODEL_TEST::setup()
{
    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();

    WTG_MODEL_TEST::setup();

    WT_GENERATOR* wt_gen = get_test_wt_generator();
    wt_gen->set_p_generation_in_MW(28.0);
    wt_gen->set_rated_power_per_wt_generator_in_MW(1.5);
    wt_gen->set_number_of_lumped_wt_generators(20);

    WT3G0 genmodel(default_toolkit);
    genmodel.set_device_id(wt_gen->get_device_id());
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

    dmdb.add_model(&genmodel);

    AERD0 aeromodel(default_toolkit);
    aeromodel.set_device_id(wt_gen->get_device_id());

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

    dmdb.add_model(&aeromodel);
}

void WT_RELAY_MODEL_TEST::tear_down()
{
    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();
    dmdb.remove_the_last_model();
    dmdb.remove_the_last_model();

    WTG_MODEL_TEST::tear_down();
}

void WT_RELAY_MODEL_TEST::test_get_model_type()
{
    WT_RELAY_MODEL* model = get_test_wt_relay_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        TEST_ASSERT(model->get_model_type()=="WT RELAY");
    }
    else
        TEST_ASSERT(false);
}

void WT_RELAY_MODEL_TEST::test_set_get_damping()
{
    WT_RELAY_MODEL* model = get_test_wt_relay_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    }
    else
        TEST_ASSERT(false);
}

void WT_RELAY_MODEL_TEST::test_get_standard_psse_string()
{
    WT_RELAY_MODEL* model = get_test_wt_relay_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        model->get_standard_psse_string(export_internal_bus_number);
    }
    else
        TEST_ASSERT(false);
}

void WT_RELAY_MODEL_TEST::test_step_response_of_wt_turbine_model_with_pitch_angle_increase_in_underspeed_mode()
{
    WT_RELAY_MODEL* model = get_test_wt_relay_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");
        toolkit.open_log_file("test_log/"+model->get_model_name()+"_"+__FUNCTION__+".txt");
        run_step_response_of_wt_turbine_model_with_pitch_angle_increase_in_underspeed_mode();
        toolkit.close_log_file();
    }
    else
        TEST_ASSERT(false);
}

void WT_RELAY_MODEL_TEST::test_step_response_of_wt_turbine_model_with_pitch_angle_increase_in_mppt_mode()
{
    WT_RELAY_MODEL* model = get_test_wt_relay_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");
        toolkit.open_log_file("test_log/"+model->get_model_name()+"_"+__FUNCTION__+".txt");
        run_step_response_of_wt_turbine_model_with_pitch_angle_increase_in_mppt_mode();
        toolkit.close_log_file();
    }
    else
        TEST_ASSERT(false);
}

void WT_RELAY_MODEL_TEST::test_step_response_of_wt_turbine_model_with_pitch_angle_increase_in_overspeed_mode()
{
    WT_RELAY_MODEL* model = get_test_wt_relay_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");
        toolkit.open_log_file("test_log/"+model->get_model_name()+"_"+__FUNCTION__+".txt");
        run_step_response_of_wt_turbine_model_with_pitch_angle_increase_in_overspeed_mode();
        toolkit.close_log_file();
    }
    else
        TEST_ASSERT(false);
}

void WT_RELAY_MODEL_TEST::test_step_response_of_wt_turbine_model_with_generator_power_order_drop_in_underspeed_mode()
{
    WT_RELAY_MODEL* model = get_test_wt_relay_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");
        toolkit.open_log_file("test_log/"+model->get_model_name()+"_"+__FUNCTION__+".txt");
        run_step_response_of_wt_turbine_model_with_generator_power_order_drop_in_underspeed_mode();
        toolkit.close_log_file();
    }
    else
        TEST_ASSERT(false);
}

void WT_RELAY_MODEL_TEST::test_step_response_of_wt_turbine_model_with_generator_power_order_drop_in_mppt_mode()
{
    WT_RELAY_MODEL* model = get_test_wt_relay_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");
        toolkit.open_log_file("test_log/"+model->get_model_name()+"_"+__FUNCTION__+".txt");
        run_step_response_of_wt_turbine_model_with_generator_power_order_drop_in_mppt_mode();
        toolkit.close_log_file();
    }
    else
        TEST_ASSERT(false);
}


void WT_RELAY_MODEL_TEST::test_step_response_of_wt_turbine_model_with_generator_power_order_drop_in_overspeed_mode()
{
    WT_RELAY_MODEL* model = get_test_wt_turbine_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");
        toolkit.open_log_file("test_log/"+model->get_model_name()+"_"+__FUNCTION__+".txt");
        run_step_response_of_wt_turbine_model_with_generator_power_order_drop_in_overspeed_mode();
        toolkit.close_log_file();
    }
    else
        TEST_ASSERT(false);
}


void WT_RELAY_MODEL_TEST::run_step_response_of_wt_turbine_model_with_pitch_angle_increase_in_underspeed_mode()
{
    ostringstream osstream;
    double delt = 0.001;
    toolkit.set_dynamic_simulation_time_step_in_s(delt);

    WT_GENERATOR* genptr = get_test_wt_generator();
    if(genptr==NULL)
        cout<<"Fatal error. No WT_GENERATOR is found in "<<__FUNCTION__<<endl;

    WT_GENERATOR_MODEL* genmodel = get_test_wt_generator_model();
    if(genmodel==NULL)
        cout<<"Fatal error. No WT_GENERATOR_MODEL is found in "<<__FUNCTION__<<endl;
    genmodel->initialize();

    WT_AERODYNAMIC_MODEL* aeromodel = get_test_wt_aerodynamic_model();
    aeromodel->set_turbine_speed_mode(WT_UNDERSPEED_MODE);

    WT_RELAY_MODEL*model = get_test_wt_turbine_model();

    osstream<<"Model:"<<model->get_standard_psse_string()<<endl;
    toolkit.show_information_with_leading_time_stamp(osstream);


    toolkit.TIME -= (2.0*delt);
    double generator_speed;

    model->initialize();
    generator_speed = model->get_generator_speed_in_pu();

    export_meter_title();
    export_meter_values();
    while(true)
    {
        toolkit.TIME += delt;
        if(toolkit.TIME>1.0+FLOAT_EPSILON)
        {
            toolkit.TIME -=delt;
            break;
        }
        generator_speed =  model->get_generator_speed_in_pu();
        while(true)
        {
            model->run(DYNAMIC_INTEGRATE_MODE);
            if(fabs(generator_speed-model->get_generator_speed_in_pu())>1e-6)
                generator_speed = model->get_generator_speed_in_pu();
            else
                break;
        }
        model->run(DYNAMIC_UPDATE_MODE);

        export_meter_values();
    }

    apply_1deg_pitch_angle_increase();
    model->run(DYNAMIC_UPDATE_MODE);
    export_meter_values();

    while(true)
    {
        toolkit.TIME += delt;

        if(toolkit.TIME>6.0+FLOAT_EPSILON)
        {
            toolkit.TIME -=delt;
            break;
        }
        generator_speed =  model->get_generator_speed_in_pu();
        while(true)
        {
            model->run(DYNAMIC_INTEGRATE_MODE);
            if(fabs(generator_speed-model->get_generator_speed_in_pu())>1e-6)
                generator_speed = model->get_generator_speed_in_pu();
            else
                break;
        }
        model->run(DYNAMIC_UPDATE_MODE);

        export_meter_values();
    }
}

void WT_RELAY_MODEL_TEST::run_step_response_of_wt_turbine_model_with_pitch_angle_increase_in_mppt_mode()
{
    ostringstream osstream;
    double delt = 0.001;
    toolkit.set_dynamic_simulation_time_step_in_s(delt);

    WT_GENERATOR* genptr = get_test_wt_generator();
    if(genptr==NULL)
        cout<<"Fatal error. No WT_GENERATOR is found in "<<__FUNCTION__<<endl;

    WT_GENERATOR_MODEL* genmodel = get_test_wt_generator_model();
    if(genmodel==NULL)
        cout<<"Fatal error. No WT_GENERATOR_MODEL is found in "<<__FUNCTION__<<endl;
    genmodel->initialize();

    WT_AERODYNAMIC_MODEL* aeromodel = get_test_wt_aerodynamic_model();
    aeromodel->set_turbine_speed_mode(WT_MPPT_MODE);

    WT_RELAY_MODEL*model = get_test_wt_turbine_model();

    osstream<<"Model:"<<model->get_standard_psse_string()<<endl;
    toolkit.show_information_with_leading_time_stamp(osstream);


    toolkit.TIME -= (2.0*delt);
    double generator_speed;

    model->initialize();
    generator_speed = model->get_generator_speed_in_pu();

    export_meter_title();
    export_meter_values();
    while(true)
    {
        toolkit.TIME += delt;
        if(toolkit.TIME>1.0+FLOAT_EPSILON)
        {
            toolkit.TIME -=delt;
            break;
        }
        generator_speed =  model->get_generator_speed_in_pu();
        while(true)
        {
            model->run(DYNAMIC_INTEGRATE_MODE);
            if(fabs(generator_speed-model->get_generator_speed_in_pu())>1e-6)
                generator_speed = model->get_generator_speed_in_pu();
            else
                break;
        }
        model->run(DYNAMIC_UPDATE_MODE);

        export_meter_values();
    }

    apply_1deg_pitch_angle_increase();
    model->run(DYNAMIC_UPDATE_MODE);
    export_meter_values();

    while(true)
    {
        toolkit.TIME += delt;

        if(toolkit.TIME>6.0+FLOAT_EPSILON)
        {
            toolkit.TIME -=delt;
            break;
        }
        generator_speed =  model->get_generator_speed_in_pu();
        while(true)
        {
            model->run(DYNAMIC_INTEGRATE_MODE);
            if(fabs(generator_speed-model->get_generator_speed_in_pu())>1e-6)
                generator_speed = model->get_generator_speed_in_pu();
            else
                break;
        }
        model->run(DYNAMIC_UPDATE_MODE);

        export_meter_values();
    }
}


void WT_RELAY_MODEL_TEST::run_step_response_of_wt_turbine_model_with_pitch_angle_increase_in_overspeed_mode()
{
    ostringstream osstream;
    double delt = 0.001;
    toolkit.set_dynamic_simulation_time_step_in_s(delt);

    WT_GENERATOR* genptr = get_test_wt_generator();
    if(genptr==NULL)
        cout<<"Fatal error. No WT_GENERATOR is found in "<<__FUNCTION__<<endl;

    WT_GENERATOR_MODEL* genmodel = get_test_wt_generator_model();
    if(genmodel==NULL)
        cout<<"Fatal error. No WT_GENERATOR_MODEL is found in "<<__FUNCTION__<<endl;
    genmodel->initialize();

    WT_AERODYNAMIC_MODEL* aeromodel = get_test_wt_aerodynamic_model();
    aeromodel->set_turbine_speed_mode(WT_OVERSPEED_MODE);

    WT_RELAY_MODEL*model = get_test_wt_turbine_model();

    osstream<<"Model:"<<model->get_standard_psse_string()<<endl;
    toolkit.show_information_with_leading_time_stamp(osstream);


    toolkit.TIME -= (2.0*delt);
    double generator_speed;

    model->initialize();
    generator_speed = model->get_generator_speed_in_pu();

    export_meter_title();
    export_meter_values();
    while(true)
    {
        toolkit.TIME += delt;
        if(toolkit.TIME>1.0+FLOAT_EPSILON)
        {
            toolkit.TIME -=delt;
            break;
        }
        generator_speed =  model->get_generator_speed_in_pu();
        while(true)
        {
            model->run(DYNAMIC_INTEGRATE_MODE);
            if(fabs(generator_speed-model->get_generator_speed_in_pu())>1e-6)
                generator_speed = model->get_generator_speed_in_pu();
            else
                break;
        }
        model->run(DYNAMIC_UPDATE_MODE);

        export_meter_values();
    }

    apply_1deg_pitch_angle_increase();
    model->run(DYNAMIC_UPDATE_MODE);
    export_meter_values();

    while(true)
    {
        toolkit.TIME += delt;

        if(toolkit.TIME>6.0+FLOAT_EPSILON)
        {
            toolkit.TIME -=delt;
            break;
        }
        generator_speed =  model->get_generator_speed_in_pu();
        while(true)
        {
            model->run(DYNAMIC_INTEGRATE_MODE);
            if(fabs(generator_speed-model->get_generator_speed_in_pu())>1e-6)
                generator_speed = model->get_generator_speed_in_pu();
            else
                break;
        }
        model->run(DYNAMIC_UPDATE_MODE);

        export_meter_values();
    }
}

void WT_RELAY_MODEL_TEST::apply_1deg_pitch_angle_increase()
{
    WT_AERODYNAMIC_MODEL* aero_model = get_test_wt_aerodynamic_model();
	if (aero_model != NULL)
		aero_model->set_initial_pitch_angle_in_deg(aero_model->get_initial_pitch_angle_in_deg() + 1.0);
	else
	{
		cout << "Fatal error. No WT_AERODYNAMIC_MODEL is found in " << __FUNCTION__ << endl;
		return;
	}

}

void WT_RELAY_MODEL_TEST::run_step_response_of_wt_turbine_model_with_generator_power_order_drop_in_underspeed_mode()
{
    ostringstream osstream;

    double delt = 0.001;
    toolkit.set_dynamic_simulation_time_step_in_s(delt);

    WT_GENERATOR_MODEL* genmodel = get_test_wt_generator_model();
    genmodel->initialize();

    WT_AERODYNAMIC_MODEL* aeromodel = get_test_wt_aerodynamic_model();
    aeromodel->set_turbine_speed_mode(WT_UNDERSPEED_MODE);

    WT_RELAY_MODEL*model = get_test_wt_turbine_model();

    osstream<<"Model:"<<model->get_standard_psse_string()<<endl;
    toolkit.show_information_with_leading_time_stamp(osstream);

    toolkit.TIME -= (2.0*delt);
    double generator_speed;

    model->initialize();
    generator_speed = model->get_generator_speed_in_pu();

    export_meter_title();
    export_meter_values();
    while(true)
    {
        toolkit.TIME += delt;
        if(toolkit.TIME>1.0+FLOAT_EPSILON)
        {
            toolkit.TIME -=delt;
            break;
        }
        generator_speed =  model->get_generator_speed_in_pu();
        while(true)
        {
            genmodel->run(DYNAMIC_INTEGRATE_MODE);
            model->run(DYNAMIC_INTEGRATE_MODE);
            if(fabs(generator_speed-model->get_generator_speed_in_pu())>1e-6)
                generator_speed = model->get_generator_speed_in_pu();
            else
                break;
        }
        genmodel->run(DYNAMIC_UPDATE_MODE);
        model->run(DYNAMIC_UPDATE_MODE);

        export_meter_values();
    }

    apply_10_percent_power_order_drop();
    genmodel->run(DYNAMIC_UPDATE_MODE);
    model->run(DYNAMIC_UPDATE_MODE);
    export_meter_values();

    while(true)
    {
        toolkit.TIME += delt;

        if(toolkit.TIME>6.0+FLOAT_EPSILON)
        {
            toolkit.TIME -=delt;
            break;
        }
        generator_speed =  model->get_generator_speed_in_pu();
        while(true)
        {
            genmodel->run(DYNAMIC_INTEGRATE_MODE);
            model->run(DYNAMIC_INTEGRATE_MODE);
            if(fabs(generator_speed-model->get_generator_speed_in_pu())>1e-6)
                generator_speed = model->get_generator_speed_in_pu();
            else
                break;
        }
        genmodel->run(DYNAMIC_UPDATE_MODE);
        model->run(DYNAMIC_UPDATE_MODE);

        export_meter_values();
    }
}

void WT_RELAY_MODEL_TEST::run_step_response_of_wt_turbine_model_with_generator_power_order_drop_in_mppt_mode()
{
    ostringstream osstream;

    double delt = 0.001;
    toolkit.set_dynamic_simulation_time_step_in_s(delt);

    WT_GENERATOR_MODEL* genmodel = get_test_wt_generator_model();
    genmodel->initialize();

    WT_AERODYNAMIC_MODEL* aeromodel = get_test_wt_aerodynamic_model();
    aeromodel->set_turbine_speed_mode(WT_MPPT_MODE);

    WT_RELAY_MODEL*model = get_test_wt_turbine_model();

    osstream<<"Model:"<<model->get_standard_psse_string()<<endl;
    toolkit.show_information_with_leading_time_stamp(osstream);

    toolkit.TIME -= (2.0*delt);
    double generator_speed;

    model->initialize();
    generator_speed = model->get_generator_speed_in_pu();

    export_meter_title();
    export_meter_values();
    while(true)
    {
        toolkit.TIME += delt;
        if(toolkit.TIME>1.0+FLOAT_EPSILON)
        {
            toolkit.TIME -=delt;
            break;
        }
        generator_speed =  model->get_generator_speed_in_pu();
        while(true)
        {
            genmodel->run(DYNAMIC_INTEGRATE_MODE);
            model->run(DYNAMIC_INTEGRATE_MODE);
            if(fabs(generator_speed-model->get_generator_speed_in_pu())>1e-6)
                generator_speed = model->get_generator_speed_in_pu();
            else
                break;
        }
        genmodel->run(DYNAMIC_UPDATE_MODE);
        model->run(DYNAMIC_UPDATE_MODE);

        export_meter_values();
    }

    apply_10_percent_power_order_drop();
    genmodel->run(DYNAMIC_UPDATE_MODE);
    model->run(DYNAMIC_UPDATE_MODE);
    export_meter_values();

    while(true)
    {
        toolkit.TIME += delt;

        if(toolkit.TIME>6.0+FLOAT_EPSILON)
        {
            toolkit.TIME -=delt;
            break;
        }
        generator_speed =  model->get_generator_speed_in_pu();
        while(true)
        {
            genmodel->run(DYNAMIC_INTEGRATE_MODE);
            model->run(DYNAMIC_INTEGRATE_MODE);
            if(fabs(generator_speed-model->get_generator_speed_in_pu())>1e-6)
                generator_speed = model->get_generator_speed_in_pu();
            else
                break;
        }
        genmodel->run(DYNAMIC_UPDATE_MODE);
        model->run(DYNAMIC_UPDATE_MODE);

        export_meter_values();
    }
}


void WT_RELAY_MODEL_TEST::run_step_response_of_wt_turbine_model_with_generator_power_order_drop_in_overspeed_mode()
{
    ostringstream osstream;

    double delt = 0.001;
    toolkit.set_dynamic_simulation_time_step_in_s(delt);

    WT_GENERATOR_MODEL* genmodel = get_test_wt_generator_model();
    genmodel->initialize();

    WT_AERODYNAMIC_MODEL* aeromodel = get_test_wt_aerodynamic_model();
    aeromodel->set_turbine_speed_mode(WT_OVERSPEED_MODE);

    WT_RELAY_MODEL*model = get_test_wt_turbine_model();

    osstream<<"Model:"<<model->get_standard_psse_string()<<endl;
    toolkit.show_information_with_leading_time_stamp(osstream);

    toolkit.TIME -= (2.0*delt);
    double generator_speed;

    model->initialize();
    generator_speed = model->get_generator_speed_in_pu();

    export_meter_title();
    export_meter_values();
    while(true)
    {
        toolkit.TIME += delt;
        if(toolkit.TIME>1.0+FLOAT_EPSILON)
        {
            toolkit.TIME -=delt;
            break;
        }
        generator_speed =  model->get_generator_speed_in_pu();
        while(true)
        {
            genmodel->run(DYNAMIC_INTEGRATE_MODE);
            model->run(DYNAMIC_INTEGRATE_MODE);
            if(fabs(generator_speed-model->get_generator_speed_in_pu())>1e-6)
                generator_speed = model->get_generator_speed_in_pu();
            else
                break;
        }
        genmodel->run(DYNAMIC_UPDATE_MODE);
        model->run(DYNAMIC_UPDATE_MODE);

        export_meter_values();
    }

    apply_10_percent_power_order_drop();
    genmodel->run(DYNAMIC_UPDATE_MODE);
    model->run(DYNAMIC_UPDATE_MODE);
    export_meter_values();

    while(true)
    {
        toolkit.TIME += delt;

        if(toolkit.TIME>6.0+FLOAT_EPSILON)
        {
            toolkit.TIME -=delt;
            break;
        }
        generator_speed =  model->get_generator_speed_in_pu();
        while(true)
        {
            genmodel->run(DYNAMIC_INTEGRATE_MODE);
            model->run(DYNAMIC_INTEGRATE_MODE);
            if(fabs(generator_speed-model->get_generator_speed_in_pu())>1e-6)
                generator_speed = model->get_generator_speed_in_pu();
            else
                break;
        }
        genmodel->run(DYNAMIC_UPDATE_MODE);
        model->run(DYNAMIC_UPDATE_MODE);

        export_meter_values();
    }
}

void WT_RELAY_MODEL_TEST::apply_10_percent_power_order_drop()
{
    WT_GENERATOR_MODEL* genmodel = get_test_wt_generator_model();
    double ipcmd = genmodel->get_initial_active_current_command_in_pu_based_on_mbase();
    genmodel->set_initial_active_current_command_in_pu_based_on_mbase(ipcmd*0.9);
}


void WT_RELAY_MODEL_TEST::export_meter_title()
{
    ostringstream osstream;
    osstream<<"TIME\tPELEC\tPMECH\tTSPEED\tGSPEED\tANGLE";
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void WT_RELAY_MODEL_TEST::export_meter_values()
{
    ostringstream osstream;

    WT_RELAY_MODEL* model = get_test_wt_turbine_model();

    osstream<<setw(10)<<setprecision(6)<<fixed<<toolkit.get_dynamic_simulation_time_in_s()<<"\t"
           <<setw(10)<<setprecision(6)<<fixed<<model->get_wt_generator_active_power_generation_in_MW()<<"\t"
           <<setw(10)<<setprecision(6)<<fixed<<model->get_mechanical_power_in_pu_from_wt_aerodynamic_model()*model->get_mbase_in_MVA()<<"\t"
           <<setw(10)<<setprecision(6)<<fixed<<model->get_turbine_speed_in_pu()<<"\t"
           <<setw(10)<<setprecision(6)<<fixed<<model->get_generator_speed_in_pu()<<"\t"
           <<setw(10)<<setprecision(6)<<fixed<<model->get_rotor_angle_in_deg();
    toolkit.show_information_with_leading_time_stamp(osstream);
}


#endif

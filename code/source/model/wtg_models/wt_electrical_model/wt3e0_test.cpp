#include "header/model/wtg_models/wt_electrical_model/wt3e0_test.h"
#include "header/basic/utility.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cstdio>
#include <cmath>

using namespace std;

WT3E0_TEST::WT3E0_TEST() : WT_ELECTRICAL_MODEL_TEST()
{
    TEST_ADD(WT3E0_TEST::test_get_model_name);
    TEST_ADD(WT3E0_TEST::test_set_get_parameters);
    TEST_ADD(WT3E0_TEST::test_initialize);
}

void WT3E0_TEST::setup()
{
    WT_ELECTRICAL_MODEL_TEST::setup();

    WT_GENERATOR* wt_gen = get_test_wt_generator();

    WT3E0 model;
    model.set_bus_to_regulate(0);
    model.set_voltage_flag(1);
    model.set_var_control_mode(CONSTANT_VOLTAGE_MODE);
    model.set_Xcomp_in_pu(0.1);
    model.set_TRV_in_s(0.1);
    model.set_Fn(1.0);
    model.set_KPV(10.0);
    model.set_TV_in_s(0.2);
    model.set_KIV(20.0);
    model.set_Qmin_in_pu(-0.5);
    model.set_Qmax_in_pu(1.5);
    model.set_TFV_in_s(0.5);
    model.set_TP_in_s(0.3);
    model.set_KQI(5.0);
    model.set_Vmin_in_pu(0.9);
    model.set_Vmax_in_pu(1.3);
    model.set_KQV(1.5);
    model.set_EQmin_in_pu(-1.0);
    model.set_EQmax_in_pu(1.5);
    model.set_Tspeed_in_s(0.5);
    model.set_KPP(2.0);
    model.set_KIP(3.0);
    model.set_Kvi(2.0);
    model.set_Tvi_in_s(0.2);
    model.set_Kdroop(10.0);
    model.set_Tdroop_in_s(0.2);
    model.set_rPmin_in_pu(-0.1);
    model.set_rPmax_in_pu(0.1);
    model.set_TFP_in_s(0.4);
    model.set_Pmin_in_pu(0.3);
    model.set_Pmax_in_pu(1.2);
    model.set_IPmax_in_pu(1.5);

    wt_gen->set_model(&model);
}

void WT3E0_TEST::tear_down()
{
    WT_ELECTRICAL_MODEL_TEST::tear_down();

    show_test_end_information();
}


void WT3E0_TEST::test_get_model_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"WT3E0_TEST");

    WT_ELECTRICAL_MODEL* model = get_test_wt_electrical_model();
    if(model!=NULL)
    {
        TEST_ASSERT(model->get_model_name()=="WT3E0");
    }
    else
        TEST_ASSERT(false);
}

void WT3E0_TEST::test_set_get_parameters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"WT3E0_TEST");

    WT3E0* model = (WT3E0*) get_test_wt_electrical_model();

    model->set_bus_to_regulate(1);
    TEST_ASSERT(model->get_bus_to_regulate()==1);

    model->set_voltage_flag(1);
    TEST_ASSERT(model->get_voltage_flag()==1);

    model->set_var_control_mode(CONSTANT_VAR_MODE);
    TEST_ASSERT(model->get_var_control_mode()==CONSTANT_VAR_MODE);

    model->set_Xcomp_in_pu(0.1);
    TEST_ASSERT(fabs(model->get_Xcomp_in_pu()-0.1)<FLOAT_EPSILON);

    model->set_TRV_in_s(0.1);
    TEST_ASSERT(fabs(model->get_TRV_in_s()-0.1)<FLOAT_EPSILON);

    model->set_Fn(1.0);
    TEST_ASSERT(fabs(model->get_Fn()-1.0)<FLOAT_EPSILON);

    model->set_KIV(20.0);
    TEST_ASSERT(fabs(model->get_KIV()-20.0)<FLOAT_EPSILON);

    model->set_Qmax_in_pu(1.0);
    TEST_ASSERT(fabs(model->get_Qmax_in_pu()-1.0)<FLOAT_EPSILON);

    model->set_Qmin_in_pu(-0.5);
    TEST_ASSERT(fabs(model->get_Qmin_in_pu()-(-0.5))<FLOAT_EPSILON);

    model->set_KPV(10.0);
    TEST_ASSERT(fabs(model->get_KPV()-10.0)<FLOAT_EPSILON);

    model->set_TV_in_s(0.2);
    TEST_ASSERT(fabs(model->get_TV_in_s()-0.2)<FLOAT_EPSILON);

    model->set_TFV_in_s(0.5);
    TEST_ASSERT(fabs(model->get_TFV_in_s()-0.5)<FLOAT_EPSILON);

    model->set_TP_in_s(0.3);
    TEST_ASSERT(fabs(model->get_TP_in_s()-0.3)<FLOAT_EPSILON);

    model->set_KQI(5.0);
    TEST_ASSERT(fabs(model->get_KQI()-5.0)<FLOAT_EPSILON);

    model->set_Vmax_in_pu(1.3);
    TEST_ASSERT(fabs(model->get_Vmax_in_pu()-1.3)<FLOAT_EPSILON);

    model->set_Vmin_in_pu(0.9);
    TEST_ASSERT(fabs(model->get_Vmin_in_pu()-0.9)<FLOAT_EPSILON);

    model->set_KQV(1.5);
    TEST_ASSERT(fabs(model->get_KQV()-1.5)<FLOAT_EPSILON);

    model->set_EQmax_in_pu(1.5);
    TEST_ASSERT(fabs(model->get_EQmax_in_pu()-1.5)<FLOAT_EPSILON);

    model->set_EQmin_in_pu(0.7);
    TEST_ASSERT(fabs(model->get_EQmin_in_pu()-0.7)<FLOAT_EPSILON);

    model->set_Tspeed_in_s(0.5);
    TEST_ASSERT(fabs(model->get_Tspeed_in_s()-0.5)<FLOAT_EPSILON);

    model->set_KPP(2.0);
    TEST_ASSERT(fabs(model->get_KPP()-2.0)<FLOAT_EPSILON);

    model->set_KIP(3.0);
    TEST_ASSERT(fabs(model->get_KIP()-3.0)<FLOAT_EPSILON);

    model->set_Kvi(2.0);
    TEST_ASSERT(fabs(model->get_Kvi()-2.0)<FLOAT_EPSILON);

    model->set_Tvi_in_s(0.2);
    TEST_ASSERT(fabs(model->get_Tvi_in_s()-0.2)<FLOAT_EPSILON);

    model->set_Kdroop(10.0);
    TEST_ASSERT(fabs(model->get_Kdroop()-10.0)<FLOAT_EPSILON);

    model->set_Tdroop_in_s(0.2);
    TEST_ASSERT(fabs(model->get_Tdroop_in_s()-0.2)<FLOAT_EPSILON);

    model->set_rPmax_in_pu(0.1);
    TEST_ASSERT(fabs(model->get_rPmax_in_pu()-0.1)<FLOAT_EPSILON);

    model->set_rPmin_in_pu(-0.1);
    TEST_ASSERT(fabs(model->get_rPmin_in_pu()-(-0.1))<FLOAT_EPSILON);

    model->set_TFP_in_s(0.4);
    TEST_ASSERT(fabs(model->get_TFP_in_s()-0.4)<FLOAT_EPSILON);

    model->set_Pmax_in_pu(1.2);
    TEST_ASSERT(fabs(model->get_Pmax_in_pu()-1.2)<FLOAT_EPSILON);

    model->set_Pmin_in_pu(0.3);
    TEST_ASSERT(fabs(model->get_Pmin_in_pu()-0.3)<FLOAT_EPSILON);

    model->set_IPmax_in_pu(1.5);
    TEST_ASSERT(fabs(model->get_IPmax_in_pu()-1.5)<FLOAT_EPSILON);
}

void WT3E0_TEST::test_initialize()
{
    show_test_information_for_function_of_class(__FUNCTION__,"WT3E0_TEST");
    set_dynamic_simulation_time_step_in_s(0.001);
    ostringstream osstream;

    WT_GENERATOR_MODEL* wtgenmodel = get_test_wt_generator_model();
    WT_AERODYNAMIC_MODEL* aerdmodel = get_test_wt_aerodynamic_model();
    WT_TURBINE_MODEL* turbinemodel = get_test_wt_turbine_model();
    WT3E0* model = (WT3E0*) get_test_wt_electrical_model();

    wtgenmodel->initialize();
    aerdmodel->initialize();
    turbinemodel->initialize();
    model->initialize();
    /*osstream<<"WT3E0 model after initialized with generation of each turbine: "<<gen->get_p_generation_in_MW()/gen->get_number_of_lumped_wt_generators()<<" MW"<<endl;
    osstream<<"Turbine blade radius = "<<model->get_turbine_blade_radius_in_m()<<" m, generator/turbine turn ratio = "<<model->get_generator_to_turbine_gear_ratio()<<endl
           <<"Pitch angle = "<<model->get_initial_pitch_angle_in_deg()<<" deg, turbine speed = "<<model->get_initial_turbine_speed_in_rad_per_s()<<" rad/s ("
           <<model->get_initial_turbine_speed_in_pu()<<" pu)";
    show_information_with_leading_time_stamp(osstream);
    */
}

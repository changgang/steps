#include "header/model/pvu_models/pv_converter_model/pvgu1_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <cstdio>
#include <cmath>

using namespace std;

PVGU1_TEST::PVGU1_TEST() : PV_CONVERTER_MODEL_TEST()
{
    TEST_ADD(PVGU1_TEST::test_get_model_name);
    TEST_ADD(PVGU1_TEST::test_set_get_parameters);
}

void PVGU1_TEST::setup()
{
    PV_CONVERTER_MODEL_TEST::setup();

    PV_UNIT* pv_unit = get_test_pv_unit();

    PVGU1 model;
    model.set_converter_activer_current_command_T_in_s(0.2);
    model.set_converter_reactiver_voltage_command_T_in_s(0.2);
    LVPL lvpl;
    lvpl.set_low_voltage_in_pu(0.5);
    lvpl.set_high_voltage_in_pu(0.8);
    lvpl.set_gain_at_high_voltage(20.0);
    model.set_LVPL(lvpl);
    model.set_HVRC_voltage_in_pu(0.8);
    model.set_HVRC_current_in_pu(20.0);
    model.set_LVPL_max_rate_of_active_current_change(0.2);
    model.set_LVPL_voltage_sensor_T_in_s(0.1);

    pv_unit->set_model(&model);
}

void PVGU1_TEST::tear_down()
{
    PV_CONVERTER_MODEL_TEST::tear_down();

    show_test_end_information();
}


void PVGU1_TEST::test_get_model_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PVGU1_TEST");

    PV_CONVERTER_MODEL* model = get_test_pv_converter_model();

    TEST_ASSERT(model->get_model_name()=="PVGU1");
}

void PVGU1_TEST::test_set_get_parameters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PVGU1_TEST");

    PVGU1* model = (PVGU1*) get_test_pv_converter_model();

    model->set_converter_activer_current_command_T_in_s(0.1);
    model->set_converter_reactiver_voltage_command_T_in_s(0.2);
    LVPL lvpl;
    lvpl.set_low_voltage_in_pu(0.5);
    lvpl.set_high_voltage_in_pu(0.8);
    lvpl.set_gain_at_high_voltage(20.0);
    model->set_LVPL(lvpl);
    model->set_HVRC_voltage_in_pu(0.8);
    model->set_HVRC_current_in_pu(0.6);
    model->set_LVPL_max_rate_of_active_current_change(0.2);
    model->set_LVPL_voltage_sensor_T_in_s(0.1);

    TEST_ASSERT(fabs(model->get_converter_activer_current_command_T_in_s()-0.1)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_converter_reactiver_voltage_command_T_in_s()-0.2)<FLOAT_EPSILON);
    LVPL lvpl2 = model->get_LVPL();
    TEST_ASSERT(fabs(lvpl2.get_low_voltage_in_pu()-0.5)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(lvpl2.get_high_voltage_in_pu()-0.8)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(lvpl2.get_gain_at_hig_voltage()-20.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_HVRC_voltage_in_pu()-0.8)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_HVRC_current_in_pu()-0.6)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_LVPL_max_rate_of_active_current_change()-0.2)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_LVPL_voltage_sensor_T_in_s()-0.1)<FLOAT_EPSILON);
}

void PVGU1_TEST::test_initialize_and_get_initialized_inputs()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PVGU1_TEST");

    ostringstream osstream;

    PV_UNIT* pv_unit = get_test_pv_unit();

    //complex<double> V=db->get_bus_complex_voltage_in_pu(1);

    PVGU1* model = (PVGU1*) get_test_pv_converter_model();

    //complex<double> Z=pv_unit->get_source_impedance_in_pu();
    complex<double> S(pv_unit->get_p_generation_in_MW(), pv_unit->get_q_generation_in_MVar());
    double mbase = pv_unit->get_mbase_in_MVA();
    S = S/mbase;

    //model->set_Tj_in_s(6.0);
    //model->set_D(1.0);

    model->initialize();
    osstream<<"PVGU1 model after initialized:"<<endl;
    osstream<<"PLL angle = "<<model->get_pll_angle_in_deg()<<" deg, PLL frequency = "<<model->get_pll_frequency_in_Hz()<<endl;
    osstream<<"Terminal P = "<<model->get_terminal_active_power_in_MW()<<" MW, Q = "<<model->get_terminal_reactive_power_in_MVar()<<" MVar"<<endl;
    default_toolkit.show_information_with_leading_time_stamp(osstream);
}

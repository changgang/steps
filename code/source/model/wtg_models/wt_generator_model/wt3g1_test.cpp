#include "header/basic/test_macro.h"
#include "header/model/wtg_models/wt_generator_model/wt3g1_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <cstdio>
#include <cmath>

#ifdef ENABLE_STEPS_TEST
using namespace std;

WT3G1_TEST::WT3G1_TEST() : WT_GENERATOR_MODEL_TEST()
{
    TEST_ADD(WT3G1_TEST::test_get_model_name);
    TEST_ADD(WT3G1_TEST::test_set_get_parameters);
    TEST_ADD(WT3G1_TEST::test_set_get_pll_angle);
}

void WT3G1_TEST::setup()
{
    WT_GENERATOR_MODEL_TEST::setup();

    WT_GENERATOR* wt_gen = get_test_wt_generator();

    WT3G1 model;
    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();
    model.set_toolkit(default_toolkit);
    model.set_device_id(wt_gen->get_device_id());

    model.set_Xeq_in_pu(0.2);
    model.set_KPLL(20.0);
    model.set_KIPLL(10.0);
    model.set_PLLmax(0.1);
    wt_gen->set_number_of_lumped_wt_generators(50);
    wt_gen->set_rated_power_per_wt_generator_in_MW(2.0);

    dmdb.add_model(&model);
}

void WT3G1_TEST::tear_down()
{
    WT_GENERATOR_MODEL_TEST::tear_down();

    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();
    dmdb.remove_the_last_model();

    show_test_end_information();
}


void WT3G1_TEST::test_get_model_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"WT3G1_TEST");

    WT_GENERATOR_MODEL* model = get_test_wt_generator_model();

    TEST_ASSERT(model->get_model_name()=="WT3G1");
}

void WT3G1_TEST::test_set_get_parameters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"WT3G1_TEST");

    WT_GENERATOR* wt_gen = get_test_wt_generator();
    WT3G1* model = (WT3G1*) get_test_wt_generator_model();

    model->set_Xeq_in_pu(0.2);
    model->set_KPLL(20.0);
    model->set_KIPLL(10.0);
    model->set_PLLmax(0.1);
    wt_gen->set_number_of_lumped_wt_generators(5);
    wt_gen->set_rated_power_per_wt_generator_in_MW(2.0);

    TEST_ASSERT(fabs(model->get_Xeq_in_pu()-0.2)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_KPLL()-20.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_KIPLL()-10.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_PLLmax()-0.1)<FLOAT_EPSILON);
    TEST_ASSERT(model->get_number_of_lumped_wt_generators()==5);
    TEST_ASSERT(fabs(model->get_rated_power_per_wt_generator_in_MW()-2.0)<FLOAT_EPSILON);
}

void WT3G1_TEST::test_initialize_and_get_initialized_inputs()
{
    show_test_information_for_function_of_class(__FUNCTION__,"WT3G1_TEST");

    ostringstream osstream;

    WT_GENERATOR* wt_gen = get_test_wt_generator();

    //complex<double> V=db->get_bus_complex_voltage_in_pu(1);

    WT3G1* model = (WT3G1*) get_test_wt_generator_model();

    //complex<double> Z=wt_generator->get_source_impedance_in_pu();
    complex<double> S(wt_gen->get_p_generation_in_MW(), wt_gen->get_q_generation_in_MVar());
    double one_over_mbase = wt_gen->get_one_over_mbase_in_one_over_MVA();
    S *= one_over_mbase;

    //model->set_Tj_in_s(6.0);
    //model->set_D(1.0);

    model->initialize();
    osstream<<"WT3G1 model after initialized:"<<endl;
    osstream<<"PLL angle = "<<model->get_pll_angle_in_deg()<<" deg, PLL frequency = "<<model->get_pll_frequency_in_Hz()<<endl;
    osstream<<"Terminal P = "<<model->get_terminal_active_power_in_MW()<<" MW, Q = "<<model->get_terminal_reactive_power_in_MVar()<<" MVar"<<endl;
    default_toolkit.show_information_with_leading_time_stamp(osstream);
}


void WT3G1_TEST::test_set_get_pll_angle()
{
    show_test_information_for_function_of_class(__FUNCTION__,"WT3G1_TEST");

    WT3G1* model = (WT3G1*) get_test_wt_generator_model();
    model->initialize();
    model->set_pll_angle_in_deg(10.0);
    TEST_ASSERT(fabs(model->get_pll_angle_in_deg()-10.0)<FLOAT_EPSILON);
}

#endif

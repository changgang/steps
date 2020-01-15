#include "header/basic/test_macro.h"
#include "header/model/wtg_models/wt_aerodynamic_model/aerd0_test.h"
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

AERD0_TEST::AERD0_TEST() : WT_AERODYNAMIC_MODEL_TEST()
{
    TEST_ADD(AERD0_TEST::test_get_model_name);
    TEST_ADD(AERD0_TEST::test_set_get_parameters);
}

void AERD0_TEST::setup()
{
    WT_AERODYNAMIC_MODEL_TEST::setup();

    WT_GENERATOR* wt_gen = get_test_wt_generator();

    AERD0 model;
    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();
    model.set_toolkit(default_toolkit);
    model.set_device_id(wt_gen->get_device_id());

    model.set_number_of_pole_pairs(2);
    model.set_generator_to_turbine_gear_ratio(100.0);
    model.set_gear_efficiency(1.0);
    model.set_turbine_blade_radius_in_m(25.0);
    model.set_nominal_wind_speed_in_mps(13.0);
    model.set_nominal_air_density_in_kgpm3(1.25);
    model.set_air_density_in_kgpm3(1.25);
    model.set_turbine_speed_mode(WT_UNDERSPEED_MODE);

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

    dmdb.add_model(&model);
}

void AERD0_TEST::tear_down()
{
    WT_AERODYNAMIC_MODEL_TEST::tear_down();

    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();
    dmdb.remove_the_last_model();

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
    model->set_C7(7.7);
    model->set_C8(8.8);

    TEST_ASSERT(fabs(model->get_C1()-1.1)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_C2()-2.2)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_C3()-3.3)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_C4()-4.4)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_C5()-5.5)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_C6()-6.6)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_C7()-7.7)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_C8()-8.8)<FLOAT_EPSILON);
}

#endif

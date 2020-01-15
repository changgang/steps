#include "header/basic/test_macro.h"
#include "header/model/wtg_models/wind_speed_model/wind_speed_model_test.h"
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

#ifdef ENABLE_STEPS_TEST
using namespace std;

WIND_SPEED_MODEL_TEST::WIND_SPEED_MODEL_TEST()
{
    TEST_ADD(WIND_SPEED_MODEL_TEST::test_get_model_type);
    TEST_ADD(WIND_SPEED_MODEL_TEST::test_get_nominal_wind_speed);
    TEST_ADD(WIND_SPEED_MODEL_TEST::test_get_standard_psse_string);
}

void WIND_SPEED_MODEL_TEST::setup()
{
    WTG_MODEL_TEST::setup();

    WT_GENERATOR* wt_gen = get_test_wt_generator();
    wt_gen->set_p_generation_in_MW(28.0);
    wt_gen->set_rated_power_per_wt_generator_in_MW(1.5);
    wt_gen->set_number_of_lumped_wt_generators(20);

    AERD0 aeromodel;
    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();
    aeromodel.set_toolkit(default_toolkit);
    aeromodel.set_device_id(wt_gen->get_device_id());

    aeromodel.set_nominal_wind_speed_in_mps(13.0);
    dmdb.add_model(&aeromodel);
}

void WIND_SPEED_MODEL_TEST::tear_down()
{
    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();
    dmdb.remove_the_last_model();

    WTG_MODEL_TEST::tear_down();
}

void WIND_SPEED_MODEL_TEST::test_get_model_type()
{
    WIND_SPEED_MODEL* model = get_test_wind_speed_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        TEST_ASSERT(model->get_model_type()=="WIND SPEED");
    }
    else
        TEST_ASSERT(false);
}

void WIND_SPEED_MODEL_TEST::test_get_nominal_wind_speed()
{
    WIND_SPEED_MODEL* model = get_test_wind_speed_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        WT_AERODYNAMIC_MODEL* aerd = get_test_wt_aerodynamic_model();

        TEST_ASSERT(fabs(model->get_nominal_wind_speed_in_mps()-aerd->get_nominal_wind_speed_in_mps())<FLOAT_EPSILON);
    }
    else
        TEST_ASSERT(false);
}

void WIND_SPEED_MODEL_TEST::test_get_standard_psse_string()
{
    WIND_SPEED_MODEL* model = get_test_wind_speed_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");
        default_toolkit.show_information_with_leading_time_stamp(model->get_standard_psse_string());
    }
    else
        TEST_ASSERT(false);
}

#endif

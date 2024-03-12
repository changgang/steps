#include "header/basic/test_macro.h"
#include "header/model/lcc_hvdc2t_model/CDC4T_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

#ifdef ENABLE_STEPS_TEST
using namespace std;
CDC4T_TEST::CDC4T_TEST() : LCC_HVDC2T_MODEL_TEST()
{
    TEST_ADD(CDC4T_TEST::test_get_model_type);
    TEST_ADD(CDC4T_TEST::test_get_model_name);
    TEST_ADD(CDC4T_TEST::test_set_get_parameters);
}

void CDC4T_TEST::setup()
{
    LCC_HVDC2T_MODEL_TEST::setup();

    LCC_HVDC2T* hvdcptr = get_test_2t_lcc_hvdc();

    CDC4T model(default_toolkit);
    model.set_device_id(hvdcptr->get_device_id());

    model.set_converter_dynamic_min_alpha_or_gamma_in_deg(RECTIFIER, 5.0);
    model.set_converter_dynamic_min_alpha_or_gamma_in_deg(INVERTER, 5.0);
    model.set_inverter_dc_voltage_sensor_T_in_s(0.1);
    model.set_dc_current_sensor_T_in_s(0.1);
    model.set_rectifier_ac_instantaneous_blocking_voltage_in_pu(0.7);
    model.set_rectifier_ac_instantaneous_unblocking_voltage_in_pu(0.85);
    model.set_mininum_blocking_time_in_s(0.5);
    model.set_inverter_dc_instantaneous_bypassing_voltage_in_kV(500.0);
    model.set_inverter_ac_instantaneous_unbypassing_voltage_in_pu(0.8);
    model.set_mininum_bypassing_time_in_s(0.3);
    model.set_minimum_dc_voltage_in_kV_following_unblocking_and_unbypassing(100.0);
    model.set_minimum_dc_current_in_kA_following_unblocking(1.0);
    model.set_dc_voltage_command_recovery_rate_in_pu_per_second(0.3);
    model.set_dc_current_command_recovery_rate_in_pu_per_second(0.3);
    model.set_minimum_dc_current_command_in_kA(0.1);
    VDCOL limiter;
    limiter.append_vdcol_point_in_kV_kA(200.0, 1.5);
    limiter.append_vdcol_point_in_kV_kA(400.0, 3.0);
    limiter.append_vdcol_point_in_kV_kA(600.0, 4.0);
    model.set_VDCOL(limiter);
    model.set_minimum_time_in_switched_mode_in_s(0.1);

    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();
    dmdb.add_model(&model);
}

void CDC4T_TEST::tear_down()
{
    LCC_HVDC2T_MODEL_TEST::tear_down();

    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();
    dmdb.remove_the_last_model();
}

void CDC4T_TEST::test_get_model_type()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CDC4T_TEST");

    LCC_HVDC2T_MODEL* model = get_test_2t_lcc_hvdc_model();

    TEST_ASSERT(model->get_model_type()=="2T LCC HVDC");
}

void CDC4T_TEST::test_get_model_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CDC4T_TEST");

    LCC_HVDC2T_MODEL* model = get_test_2t_lcc_hvdc_model();

    TEST_ASSERT(model->get_model_name()=="CDC4T");
}

void CDC4T_TEST::test_set_get_parameters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CDC4T_TEST");

    CDC4T* model = (CDC4T*) get_test_2t_lcc_hvdc_model();

    TEST_ASSERT(fabs(model->get_converter_dynamic_min_alpha_or_gamma_in_deg(RECTIFIER)-5.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_converter_dynamic_min_alpha_or_gamma_in_deg(INVERTER) -5.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_inverter_dc_voltage_sensor_T_in_s()-0.1)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_dc_current_sensor_T_in_s()-0.1)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_rectifier_ac_instantaneous_blocking_voltage_in_pu()-0.7)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_rectifier_ac_instantaneous_unblocking_voltage_in_pu()-0.85)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_mininum_blocking_time_in_s()-0.5)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_inverter_dc_instantaneous_bypassing_voltage_in_kV()-500.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_inverter_ac_instantaneous_unbypassing_voltage_in_pu()-0.8)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_mininum_bypassing_time_in_s()-0.3)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_minimum_dc_voltage_in_kV_following_unblocking_and_unbypassing()-100.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_minimum_dc_current_in_kA_following_unblocking()-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_dc_voltage_command_recovery_rate_in_pu_per_second()-0.3)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_dc_current_command_recovery_rate_in_pu_per_second()-0.3)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_minimum_dc_current_command_in_kA()-0.1)<FLOAT_EPSILON);
    VDCOL limiter = model->get_VDCOL();
    unsigned int n = limiter.get_vdcol_point_count();
    TEST_ASSERT(n==3);
    TEST_ASSERT(fabs(limiter.get_vdcol_voltage_of_point_in_kV(0)-200.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(limiter.get_vdcol_current_of_point_in_kA(0)-1.5)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(limiter.get_vdcol_voltage_of_point_in_kV(1)-400.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(limiter.get_vdcol_current_of_point_in_kA(1)-3.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(limiter.get_vdcol_voltage_of_point_in_kV(2)-600.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(limiter.get_vdcol_current_of_point_in_kA(2)-4.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_minimum_time_in_switched_mode_in_s()-0.1)<FLOAT_EPSILON);
}

#endif

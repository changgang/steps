#include "header/basic/test_macro.h"
#include "header/model/hvdc_model/vdcol_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

#ifdef ENABLE_STEPS_TEST
using namespace std;
VDCOL_TEST::VDCOL_TEST()
{
    TEST_ADD(VDCOL_TEST::test_constructor);
    TEST_ADD(VDCOL_TEST::test_set_get_voltage_current_points);
    TEST_ADD(VDCOL_TEST::test_get_maximum_current_limit);
}

void VDCOL_TEST::setup()
{
    ;
}

void VDCOL_TEST::tear_down()
{
    limiter.clear();

    show_test_end_information();
}


void VDCOL_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VDCOL_TEST");

    TEST_ASSERT(limiter.get_vdcol_point_count()==0);
}

void VDCOL_TEST::test_set_get_voltage_current_points()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VDCOL_TEST");

    limiter.append_vdcol_point_in_kV_kA(100.0, 1.0);
    TEST_ASSERT(limiter.get_vdcol_point_count()==1);
    TEST_ASSERT(fabs(limiter.get_vdcol_voltage_of_point_in_kV(0)-100.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(limiter.get_vdcol_current_of_point_in_kA(0)-1.0)<FLOAT_EPSILON);

    limiter.append_vdcol_point_in_kV_kA(200.0, 2.0);
    TEST_ASSERT(limiter.get_vdcol_point_count()==2);
    TEST_ASSERT(fabs(limiter.get_vdcol_voltage_of_point_in_kV(0)-100.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(limiter.get_vdcol_current_of_point_in_kA(0)-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(limiter.get_vdcol_voltage_of_point_in_kV(1)-200.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(limiter.get_vdcol_current_of_point_in_kA(1)-2.0)<FLOAT_EPSILON);

    limiter.append_vdcol_point_in_kV_kA(300.0, 3.0);
    TEST_ASSERT(limiter.get_vdcol_point_count()==3);
    TEST_ASSERT(fabs(limiter.get_vdcol_voltage_of_point_in_kV(0)-100.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(limiter.get_vdcol_current_of_point_in_kA(0)-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(limiter.get_vdcol_voltage_of_point_in_kV(1)-200.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(limiter.get_vdcol_current_of_point_in_kA(1)-2.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(limiter.get_vdcol_voltage_of_point_in_kV(2)-300.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(limiter.get_vdcol_current_of_point_in_kA(2)-3.0)<FLOAT_EPSILON);
}

void VDCOL_TEST::test_get_maximum_current_limit()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VDCOL_TEST");

    limiter.append_vdcol_point_in_kV_kA(100.0, 1.0);
    limiter.append_vdcol_point_in_kV_kA(200.0, 2.0);
    limiter.append_vdcol_point_in_kV_kA(300.0, 3.0);

    TEST_ASSERT(fabs(limiter.get_vocol_maximum_current_command_in_kA_with_inverter_dc_voltage_in_kV(0.0)-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(limiter.get_vocol_maximum_current_command_in_kA_with_inverter_dc_voltage_in_kV(50.0)-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(limiter.get_vocol_maximum_current_command_in_kA_with_inverter_dc_voltage_in_kV(100.0)-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(limiter.get_vocol_maximum_current_command_in_kA_with_inverter_dc_voltage_in_kV(150.0)-1.5)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(limiter.get_vocol_maximum_current_command_in_kA_with_inverter_dc_voltage_in_kV(170.0)-1.7)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(limiter.get_vocol_maximum_current_command_in_kA_with_inverter_dc_voltage_in_kV(200.0)-2.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(limiter.get_vocol_maximum_current_command_in_kA_with_inverter_dc_voltage_in_kV(250.0)-2.5)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(limiter.get_vocol_maximum_current_command_in_kA_with_inverter_dc_voltage_in_kV(270.0)-2.7)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(limiter.get_vocol_maximum_current_command_in_kA_with_inverter_dc_voltage_in_kV(300.0)-3.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(limiter.get_vocol_maximum_current_command_in_kA_with_inverter_dc_voltage_in_kV(330.0)-3.0)<FLOAT_EPSILON);
}


void VDCOL_TEST::test_copy_with_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VDCOL_TEST");

    limiter.append_vdcol_point_in_kV_kA(100.0, 1.0);
    limiter.append_vdcol_point_in_kV_kA(200.0, 2.0);
    limiter.append_vdcol_point_in_kV_kA(300.0, 3.0);

    VDCOL limiter2 = limiter;
    TEST_ASSERT(limiter2.get_vdcol_point_count()==3);
    TEST_ASSERT(fabs(limiter2.get_vdcol_voltage_of_point_in_kV(0)-100.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(limiter2.get_vdcol_current_of_point_in_kA(0)-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(limiter2.get_vdcol_voltage_of_point_in_kV(1)-200.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(limiter2.get_vdcol_current_of_point_in_kA(1)-2.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(limiter2.get_vdcol_voltage_of_point_in_kV(1)-300.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(limiter2.get_vdcol_current_of_point_in_kA(1)-3.0)<FLOAT_EPSILON);
}

void VDCOL_TEST::test_copy_with_operator_equal()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VDCOL_TEST");

    limiter.append_vdcol_point_in_kV_kA(100.0, 1.0);
    limiter.append_vdcol_point_in_kV_kA(200.0, 2.0);
    limiter.append_vdcol_point_in_kV_kA(300.0, 3.0);

    VDCOL limiter2;
    limiter2 = limiter;
    TEST_ASSERT(limiter2.get_vdcol_point_count()==3);
    TEST_ASSERT(fabs(limiter2.get_vdcol_voltage_of_point_in_kV(0)-100.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(limiter2.get_vdcol_current_of_point_in_kA(0)-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(limiter2.get_vdcol_voltage_of_point_in_kV(1)-200.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(limiter2.get_vdcol_current_of_point_in_kA(1)-2.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(limiter2.get_vdcol_voltage_of_point_in_kV(1)-300.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(limiter2.get_vdcol_current_of_point_in_kA(1)-3.0)<FLOAT_EPSILON);
}


#endif

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif
#include <cmath>
#include "cpptest.h"
#include "header/basic/test_macro.h"

#include "header/device/vsc_hvdc_test.h"
#include "header/basic/constants.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/model/hvdc_model/hvdc_models.h"

#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <cstdio>
#include <algorithm>

//#ifdef ENABLE_STEPS_TEST
using namespace std;

VSC_HVDC_TEST::VSC_HVDC_TEST():vsc(default_toolkit)
{
    TEST_ADD(VSC_HVDC_TEST::test_constructor);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_identifier);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_name);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_converter_count);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_dc_bus_count);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_dc_line_count);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_set_ac_converter_bus_with_dc_voltage_control);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_converter_bus);

    TEST_ADD(VSC_HVDC_TEST::test_set_get_converter_dc_operation_mode);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_converter_ac_operation_mode);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_converter_nominal_dc_power_command_in_MW);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_converter_nominal_dc_voltage_command_in_kV);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_converter_nominal_ac_voltage_command_in_pu);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_converter_nominal_ac_power_factor_command);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_converter_loss_factor_A_in_kW);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_converter_loss_factor_B_in_kW_per_amp);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_converter_minimum_loss_in_kW);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_converter_rated_capacity_in_MVA);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_converter_current_rating_in_amp);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_converter_power_weighting_factor);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_converter_Qmax_in_MVar);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_converter_Qmin_in_MVar);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_converter_remote_bus_to_regulate);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_converter_remote_regulation_percent);

    TEST_ADD(VSC_HVDC_TEST::test_set_get_dc_bus_number);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_dc_bus_name);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_dc_bus_area);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_dc_bus_zone);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_ac_bus_number_of_dc_bus);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_owner_number);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_ground_resistance_in_ohm);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_dc_bus_generation_power_in_MW);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_dc_bus_load_power_in_MW);

    TEST_ADD(VSC_HVDC_TEST::test_set_get_dc_sending_side_bus);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_dc_receiving_side_bus);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_meter_end_bus);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_line_resistance_in_ohm);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_line_inductance_in_mH);
    TEST_ADD(VSC_HVDC_TEST::test_get_device_id);
}

void VSC_HVDC_TEST::setup()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.set_system_base_power_in_MVA(100.0);
    psdb.set_allowed_max_bus_number(200);

    BUS bus(default_toolkit);
    bus.set_bus_number(1);
    bus.set_base_voltage_in_kV(500.0);
    bus.set_positive_sequence_voltage_in_pu(1.0);
    bus.set_positive_sequence_angle_in_rad(0.0);
    bus.set_bus_type(PQ_TYPE);
    psdb.append_bus(bus);
    bus.set_bus_number(2);
    psdb.append_bus(bus);
    bus.set_bus_number(3);
    psdb.append_bus(bus);
    bus.set_bus_number(4);
    psdb.append_bus(bus);
    bus.set_bus_number(5);
    psdb.append_bus(bus);
}

void VSC_HVDC_TEST::tear_down()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.clear();
    vsc.clear();
}

void VSC_HVDC_TEST::test_constructor()
{
    TEST_ASSERT(vsc.get_identifier()=="");
    TEST_ASSERT(vsc.get_name()=="");
    TEST_ASSERT(vsc.get_converter_count()==0);
    TEST_ASSERT(vsc.get_dc_bus_count()==0);
    TEST_ASSERT(vsc.get_dc_line_count()==0);
}

void VSC_HVDC_TEST::test_set_get_identifier()
{
    vsc.set_identifier("1");
    TEST_ASSERT(vsc.get_identifier()=="1");
    vsc.set_identifier("#1");
    TEST_ASSERT(vsc.get_identifier()=="#1");
    TEST_ASSERT(vsc.get_name()=="#1");
}

void VSC_HVDC_TEST::test_set_get_name()
{
    vsc.set_name("1");
    TEST_ASSERT(vsc.get_name()=="1");
    vsc.set_name("#1");
    TEST_ASSERT(vsc.get_name()=="#1");
    TEST_ASSERT(vsc.get_identifier()=="#1");
}

void VSC_HVDC_TEST::test_set_get_converter_count()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");

    vsc.set_converter_count(2);
    TEST_ASSERT(vsc.get_converter_count()==2);

    vsc.set_converter_count(3);
    TEST_ASSERT(vsc.get_converter_count()==3);

    vsc.set_converter_count(10);
    TEST_ASSERT(vsc.get_converter_count()==10);

    vsc.set_converter_count(1);
    TEST_ASSERT(vsc.get_converter_count()==2);
}

void VSC_HVDC_TEST::test_set_get_dc_bus_count()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");

    vsc.set_dc_bus_count(2);
    TEST_ASSERT(vsc.get_dc_bus_count()==2);

    vsc.set_dc_bus_count(3);
    TEST_ASSERT(vsc.get_dc_bus_count()==3);

    vsc.set_dc_bus_count(10);
    TEST_ASSERT(vsc.get_dc_bus_count()==10);

    vsc.set_dc_bus_count(1);
    TEST_ASSERT(vsc.get_dc_bus_count()==2);
}

void VSC_HVDC_TEST::test_set_get_dc_line_count()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");

    vsc.set_dc_line_count(2);
    TEST_ASSERT(vsc.get_dc_line_count()==2);

    vsc.set_dc_line_count(3);
    TEST_ASSERT(vsc.get_dc_line_count()==3);

    vsc.set_dc_line_count(10);
    TEST_ASSERT(vsc.get_dc_line_count()==10);

    vsc.set_dc_line_count(1);
    TEST_ASSERT(vsc.get_dc_line_count()==1);

    vsc.set_dc_line_count(0);
    TEST_ASSERT(vsc.get_dc_line_count()==1);
}


void VSC_HVDC_TEST::test_set_get_set_ac_converter_bus_with_dc_voltage_control()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");

    vsc.set_ac_converter_bus_with_dc_voltage_control(1);
    TEST_ASSERT(vsc.get_ac_converter_bus_with_dc_voltage_control()==1);

    vsc.set_ac_converter_bus_with_dc_voltage_control(2);
    TEST_ASSERT(vsc.get_ac_converter_bus_with_dc_voltage_control()==2);

    vsc.set_ac_converter_bus_with_dc_voltage_control(10);
    TEST_ASSERT(vsc.get_ac_converter_bus_with_dc_voltage_control()==0);

    vsc.set_ac_converter_bus_with_dc_voltage_control(20);
    TEST_ASSERT(vsc.get_ac_converter_bus_with_dc_voltage_control()==0);
}

void VSC_HVDC_TEST::test_set_get_converter_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");

    vsc.set_converter_count(2);

    vsc.set_converter_bus(0,1);
    TEST_ASSERT(vsc.get_converter_bus(0)==1);

    vsc.set_converter_bus(1,2);
    TEST_ASSERT(vsc.get_converter_bus(1)==2);

    vsc.set_converter_bus(2,3);
    TEST_ASSERT(vsc.get_converter_bus(2)==0);

    vsc.set_converter_count(5);

    vsc.set_converter_bus(0,1);
    TEST_ASSERT(vsc.get_converter_bus(0)==1);

    vsc.set_converter_bus(1,2);
    TEST_ASSERT(vsc.get_converter_bus(1)==2);

    vsc.set_converter_bus(2,3);
    TEST_ASSERT(vsc.get_converter_bus(2)==3);

    vsc.set_converter_bus(3,4);
    TEST_ASSERT(vsc.get_converter_bus(3)==4);

    vsc.set_converter_bus(4,5);
    TEST_ASSERT(vsc.get_converter_bus(4)==5);

    vsc.set_converter_bus(5,6);
    TEST_ASSERT(vsc.get_converter_bus(5)==0);
}

void VSC_HVDC_TEST::test_set_get_converter_dc_operation_mode()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");

    prepare_2_terminal_vsc_hvdc();

    vsc.set_converter_dc_operation_mode(0,VSC_DC_VOLTAGE_CONTORL);
    TEST_ASSERT(vsc.get_converter_dc_operation_mode(0)==VSC_DC_VOLTAGE_CONTORL);

    vsc.set_converter_dc_operation_mode(0,VSC_AC_ACTIVE_POWER_CONTORL);
    TEST_ASSERT(vsc.get_converter_dc_operation_mode(0)==VSC_AC_ACTIVE_POWER_CONTORL);

    vsc.set_converter_dc_operation_mode(1,VSC_DC_VOLTAGE_CONTORL);
    TEST_ASSERT(vsc.get_converter_dc_operation_mode(1)==VSC_DC_VOLTAGE_CONTORL);

    vsc.set_converter_dc_operation_mode(1,VSC_AC_ACTIVE_POWER_CONTORL);
    TEST_ASSERT(vsc.get_converter_dc_operation_mode(1)==VSC_AC_ACTIVE_POWER_CONTORL);
}

void VSC_HVDC_TEST::test_set_get_converter_ac_operation_mode()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");

    prepare_2_terminal_vsc_hvdc();

    vsc.set_converter_ac_operation_mode(0,VSC_AC_VOLTAGE_CONTROL);
    TEST_ASSERT(vsc.get_converter_ac_operation_mode(0)==VSC_AC_VOLTAGE_CONTROL);

    vsc.set_converter_ac_operation_mode(0,VSC_AC_REACTIVE_POWER_CONTROL);
    TEST_ASSERT(vsc.get_converter_ac_operation_mode(0)==VSC_AC_REACTIVE_POWER_CONTROL);

    vsc.set_converter_ac_operation_mode(1,VSC_AC_VOLTAGE_CONTROL);
    TEST_ASSERT(vsc.get_converter_ac_operation_mode(1)==VSC_AC_VOLTAGE_CONTROL);

    vsc.set_converter_ac_operation_mode(1,VSC_AC_REACTIVE_POWER_CONTROL);
    TEST_ASSERT(vsc.get_converter_ac_operation_mode(1)==VSC_AC_REACTIVE_POWER_CONTROL);
}

void VSC_HVDC_TEST::test_set_get_converter_nominal_dc_power_command_in_MW()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");

    prepare_2_terminal_vsc_hvdc();

    vsc.set_converter_nominal_ac_active_power_command_in_MW(0, 50);
    TEST_ASSERT(fabs(vsc.get_converter_nominal_ac_active_power_command_in_MW(0)-50)<FLOAT_EPSILON);

    vsc.set_converter_nominal_ac_active_power_command_in_MW(1,410);
    TEST_ASSERT(fabs(vsc.get_converter_nominal_ac_active_power_command_in_MW(1)-410)<FLOAT_EPSILON);
}

void VSC_HVDC_TEST::test_set_get_converter_nominal_dc_voltage_command_in_kV()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_2_terminal_vsc_hvdc();

    vsc.set_converter_nominal_dc_voltage_command_in_kV(0,100);
    TEST_ASSERT(fabs(vsc.get_converter_nominal_dc_voltage_command_in_kV(0)-100)<FLOAT_EPSILON);

    vsc.set_converter_nominal_dc_voltage_command_in_kV(1,200);
    TEST_ASSERT(fabs(vsc.get_converter_nominal_dc_voltage_command_in_kV(1)-200)<FLOAT_EPSILON);
}

void VSC_HVDC_TEST::test_set_get_converter_nominal_ac_voltage_command_in_pu()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_2_terminal_vsc_hvdc();

    vsc.set_converter_nominal_ac_voltage_command_in_pu(0,1.05);
    TEST_ASSERT(fabs(vsc.get_converter_nominal_ac_voltage_command_in_pu(0)-1.05)<FLOAT_EPSILON);

    vsc.set_converter_nominal_ac_voltage_command_in_pu(1,0.99);
    TEST_ASSERT(fabs(vsc.get_converter_nominal_ac_voltage_command_in_pu(1)-0.99)<FLOAT_EPSILON);
}

void VSC_HVDC_TEST::test_set_get_converter_nominal_ac_power_factor_command()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_2_terminal_vsc_hvdc();

    vsc.set_converter_nominal_ac_voltage_command_in_pu(0,0.93);
    TEST_ASSERT(fabs(vsc.get_converter_nominal_ac_voltage_command_in_pu(0)-0.93)<FLOAT_EPSILON);

    vsc.set_converter_nominal_ac_voltage_command_in_pu(1,0.98);
    TEST_ASSERT(fabs(vsc.get_converter_nominal_ac_voltage_command_in_pu(1)-0.98)<FLOAT_EPSILON);
}

void VSC_HVDC_TEST::test_set_get_converter_loss_factor_A_in_kW()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_2_terminal_vsc_hvdc();

    vsc.set_converter_loss_factor_A_in_kW(0,200);
    TEST_ASSERT(fabs(vsc.get_converter_loss_factor_A_in_kW(0)-200)<FLOAT_EPSILON);

    vsc.set_converter_loss_factor_A_in_kW(1,300);
    TEST_ASSERT(fabs(vsc.get_converter_loss_factor_A_in_kW(1)-300)<FLOAT_EPSILON);
}

void VSC_HVDC_TEST::test_set_get_converter_loss_factor_B_in_kW_per_amp()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_2_terminal_vsc_hvdc();

    vsc.set_converter_loss_factor_B_in_kW_per_amp(0,10);
    TEST_ASSERT(fabs(vsc.get_converter_loss_factor_B_in_kW_per_amp(0)-10)<FLOAT_EPSILON);

    vsc.set_converter_loss_factor_B_in_kW_per_amp(1,11);
    TEST_ASSERT(fabs(vsc.get_converter_loss_factor_B_in_kW_per_amp(1)-11)<FLOAT_EPSILON);
}

void VSC_HVDC_TEST::test_set_get_converter_minimum_loss_in_kW()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_2_terminal_vsc_hvdc();

    vsc.set_converter_minimum_loss_in_kW(0,5);
    TEST_ASSERT(fabs(vsc.get_converter_minimum_loss_in_kW(0)-5)<FLOAT_EPSILON);

    vsc.set_converter_minimum_loss_in_kW(1,6);
    TEST_ASSERT(fabs(vsc.get_converter_minimum_loss_in_kW(1)-6)<FLOAT_EPSILON);
}

void VSC_HVDC_TEST::test_set_get_converter_rated_capacity_in_MVA()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_2_terminal_vsc_hvdc();

    vsc.set_converter_rated_capacity_in_MVA(0,100);
    TEST_ASSERT(fabs(vsc.get_converter_rated_capacity_in_MVA(0)-100)<FLOAT_EPSILON);

    vsc.set_converter_rated_capacity_in_MVA(1,120);
    TEST_ASSERT(fabs(vsc.get_converter_rated_capacity_in_MVA(1)-120)<FLOAT_EPSILON);
}

void VSC_HVDC_TEST::test_set_get_converter_current_rating_in_amp()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_2_terminal_vsc_hvdc();

    vsc.set_converter_rated_current_in_A(0,10.1);
    TEST_ASSERT(fabs(vsc.get_converter_current_rating_in_amp(0)-10.1)<FLOAT_EPSILON);

    vsc.set_converter_rated_current_in_A(1,12.2);
    TEST_ASSERT(fabs(vsc.get_converter_current_rating_in_amp(1)-12.2)<FLOAT_EPSILON);
}

void VSC_HVDC_TEST::test_set_get_converter_power_weighting_factor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_2_terminal_vsc_hvdc();

    vsc.set_converter_power_weighting_factor(0,0.8);
    TEST_ASSERT(fabs(vsc.get_converter_power_weighting_factor(0)-0.8)<FLOAT_EPSILON);

    vsc.set_converter_power_weighting_factor(1,-1.1);
    TEST_ASSERT(fabs(vsc.get_converter_power_weighting_factor(1)+1.1)<FLOAT_EPSILON);
}

void VSC_HVDC_TEST::test_set_get_converter_Qmax_in_MVar()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_2_terminal_vsc_hvdc();

    vsc.set_converter_power_weighting_factor(0,0.8);
    TEST_ASSERT(fabs(vsc.get_converter_power_weighting_factor(0)-0.8)<FLOAT_EPSILON);

    vsc.set_converter_power_weighting_factor(1,-1.1);
    TEST_ASSERT(fabs(vsc.get_converter_power_weighting_factor(1)+1.1)<FLOAT_EPSILON);
}

void VSC_HVDC_TEST::test_set_get_converter_Qmin_in_MVar()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_2_terminal_vsc_hvdc();

    vsc.set_converter_Qmin_in_MVar(0,60);
    TEST_ASSERT(fabs(vsc.get_converter_Qmin_in_MVar(0)-60)<FLOAT_EPSILON);

    vsc.set_converter_Qmin_in_MVar(1,80);
    TEST_ASSERT(fabs(vsc.get_converter_Qmin_in_MVar(1)-80)<FLOAT_EPSILON);
}

void VSC_HVDC_TEST::test_set_get_converter_remote_bus_to_regulate()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_2_terminal_vsc_hvdc();

    vsc.set_converter_remote_bus_to_regulate(0,60);
    TEST_ASSERT(fabs(vsc.get_converter_remote_bus_to_regulate(0)-60)<FLOAT_EPSILON);

    vsc.set_converter_remote_bus_to_regulate(1,80);
    TEST_ASSERT(fabs(vsc.get_converter_remote_bus_to_regulate(1)-80)<FLOAT_EPSILON);
}

void VSC_HVDC_TEST::test_set_get_converter_remote_regulation_percent()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_2_terminal_vsc_hvdc();

    vsc.set_converter_remote_regulation_percent(0,60);
    TEST_ASSERT(fabs(vsc.get_converter_remote_regulation_percent(0)-60)<FLOAT_EPSILON);

    vsc.set_converter_remote_regulation_percent(1,80);
    TEST_ASSERT(fabs(vsc.get_converter_remote_regulation_percent(1)-80)<FLOAT_EPSILON);
}


void VSC_HVDC_TEST::test_set_get_dc_bus_number()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_2_terminal_vsc_hvdc();

    vsc.set_dc_bus_number(0,1);
    TEST_ASSERT(fabs(vsc.get_dc_bus_number(0)-1)<FLOAT_EPSILON);

    vsc.set_dc_bus_number(1,2);
    TEST_ASSERT(fabs(vsc.get_dc_bus_number(1)-2)<FLOAT_EPSILON);
}

void VSC_HVDC_TEST::test_set_get_dc_bus_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_2_terminal_vsc_hvdc();

    vsc.set_dc_bus_name(0,"DC1");
    TEST_ASSERT(vsc.get_dc_bus_name(0)=="DC1");

    vsc.set_dc_bus_name(1,"DC2");
    TEST_ASSERT(vsc.get_dc_bus_name(1)=="DC2");
}

void VSC_HVDC_TEST::test_set_get_dc_bus_area()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_2_terminal_vsc_hvdc();

    vsc.set_dc_bus_area(0,1);
    TEST_ASSERT(fabs(vsc.get_dc_bus_area(0)-1)<FLOAT_EPSILON);

    vsc.set_dc_bus_area(1,2);
    TEST_ASSERT(fabs(vsc.get_dc_bus_area(1)-2)<FLOAT_EPSILON);
}

void VSC_HVDC_TEST::test_set_get_dc_bus_zone()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_2_terminal_vsc_hvdc();

    vsc.set_dc_bus_zone(0,1);
    TEST_ASSERT(fabs(vsc.get_dc_bus_zone(0)-1)<FLOAT_EPSILON);

    vsc.set_dc_bus_zone(1,2);
    TEST_ASSERT(fabs(vsc.get_dc_bus_zone(1)-2)<FLOAT_EPSILON);
}

void VSC_HVDC_TEST::test_set_get_ac_bus_number_of_dc_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_2_terminal_vsc_hvdc();

    vsc.set_ac_bus_number_of_dc_bus(0,212);
    TEST_ASSERT(fabs(vsc.get_ac_bus_number_of_dc_bus(0)-212)<FLOAT_EPSILON);

    vsc.set_ac_bus_number_of_dc_bus(1,213);
    TEST_ASSERT(fabs(vsc.get_ac_bus_number_of_dc_bus(1)-213)<FLOAT_EPSILON);
}

void VSC_HVDC_TEST::test_set_get_owner_number()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_2_terminal_vsc_hvdc();

    vsc.set_owner_number(0,2);
    TEST_ASSERT(fabs(vsc.get_owner_number(0)-2)<FLOAT_EPSILON);

    vsc.set_owner_number(1,1);
    TEST_ASSERT(fabs(vsc.get_owner_number(1)-1)<FLOAT_EPSILON);
}

void VSC_HVDC_TEST::test_set_get_ground_resistance_in_ohm()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_2_terminal_vsc_hvdc();

    vsc.set_ground_resistance_in_ohm(0,1.02);
    TEST_ASSERT(fabs(vsc.get_ground_resistance_in_ohm(0)-1.02)<FLOAT_EPSILON);

    vsc.set_ground_resistance_in_ohm(1,2.02);
    TEST_ASSERT(fabs(vsc.get_ground_resistance_in_ohm(1)-2.02)<FLOAT_EPSILON);
}

void VSC_HVDC_TEST::test_set_get_dc_bus_generation_power_in_MW()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_2_terminal_vsc_hvdc();

    vsc.set_dc_bus_generation_power_in_MW(0,50);
    TEST_ASSERT(fabs(vsc.get_dc_bus_generation_power_in_MW(0)-50)<FLOAT_EPSILON);
}

void VSC_HVDC_TEST::test_set_get_dc_bus_load_power_in_MW()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_2_terminal_vsc_hvdc();

    vsc.set_dc_bus_load_power_in_MW(0,50);
    TEST_ASSERT(fabs(vsc.get_dc_bus_load_power_in_MW(0)-50)<FLOAT_EPSILON);
}

void VSC_HVDC_TEST::test_set_get_dc_sending_side_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_2_terminal_vsc_hvdc();

    vsc.set_dc_line_sending_side_bus(0,3);

    TEST_ASSERT(fabs(vsc.get_dc_line_sending_side_bus(0)-3)<FLOAT_EPSILON);

    //vsc.set_dc_line_sending_side_bus(1,2);
    //TEST_ASSERT(fabs(vsc.get_dc_line_sending_side_bus(1)-2)<FLOAT_EPSILON);
}

void VSC_HVDC_TEST::test_set_get_dc_receiving_side_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_2_terminal_vsc_hvdc();

    vsc.set_dc_line_receiving_side_bus(0,1);
    TEST_ASSERT(fabs(vsc.get_dc_line_receiving_side_bus(0)-1)<FLOAT_EPSILON);

    //vsc.set_dc_line_receiving_side_bus(1,2);
    //TEST_ASSERT(fabs(vsc.get_dc_line_receiving_side_bus(1)-2)<FLOAT_EPSILON);
}


void VSC_HVDC_TEST::test_set_get_meter_end_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_2_terminal_vsc_hvdc();

    vsc.set_dc_line_sending_side_bus(0,3);
    vsc.set_dc_line_meter_end_bus(0,1);

    TEST_ASSERT(vsc.get_dc_line_meter_end_bus(0)==3);

    //vsc.set_dc_line_meter_end_bus(1,2);
    //TEST_ASSERT(vsc.get_dc_line_meter_end_bus(1)==2);
}

void VSC_HVDC_TEST::test_set_get_line_resistance_in_ohm()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_2_terminal_vsc_hvdc();

    vsc.set_dc_line_resistance_in_ohm(0,29.01);
    TEST_ASSERT(fabs(vsc.get_dc_line_resistance_in_ohm(0)-29.01)<FLOAT_EPSILON);

    //vsc.set_dc_line_resistance_in_ohm(1,29);
    //TEST_ASSERT(fabs(vsc.get_dc_line_resistance_in_ohm(0)-29)<FLOAT_EPSILON);
}

void VSC_HVDC_TEST::test_set_get_line_inductance_in_mH()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_2_terminal_vsc_hvdc();

    vsc.set_dc_line_inductance_in_mH(0,5);
    TEST_ASSERT(fabs(vsc.get_dc_line_inductance_in_mH(0)-5)<FLOAT_EPSILON);

    //vsc.set_dc_line_inductance_in_mH(1,29);
    //TEST_ASSERT(fabs(vsc.get_dc_line_inductance_in_mH(0)-29)<FLOAT_EPSILON);
}
void VSC_HVDC_TEST::test_get_device_id()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_2_terminal_vsc_hvdc();

    vsc.set_converter_bus(0,1);
    vsc.set_converter_bus(1,2);
    vsc.set_identifier("#1");

    DEVICE_ID did;
    did.set_device_type(STEPS_VSC_HVDC);
    TERMINAL terminal;
    terminal.append_bus(1);
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier_index(get_index_of_string("#1"));

    TEST_ASSERT(vsc.get_device_id()==did);

    //vsc.set_dc_line_inductance_in_mH(1,29);
    //TEST_ASSERT(fabs(vsc.get_dc_line_inductance_in_mH(0)-29)<FLOAT_EPSILON);
}
void VSC_HVDC_TEST::test_build_conductance_matrix()
{
    /*

    */
    prepare_3_terminal_vsc_hvdc_converter_ac_buses();
    vsc.set_converter_count(3);
    vsc.set_dc_bus_count(3);
    vsc.set_dc_line_count(3);

    /*vsc.build_matrix();
    y = vsc.get_matrix();
    TEST_ASSERT();
    TEST_ASSERT(fabs(y(1,1)-0.9)<FLOAT_EPSILON);
    */
}

void VSC_HVDC_TEST::test_set_get_vsc_hvdc_model()
{
    ;
}


void VSC_HVDC_TEST::prepare_2_terminal_vsc_hvdc()
{
    vsc.set_converter_count(2);
    vsc.set_dc_bus_count(2);
    vsc.set_dc_line_count(1);
}

void VSC_HVDC_TEST::prepare_3_terminal_vsc_hvdc()
{
    vsc.set_converter_count(3);
}

void VSC_HVDC_TEST::prepare_4_terminal_vsc_hvdc()
{
    vsc.set_converter_count(4);
}

void VSC_HVDC_TEST::prepare_5_terminal_vsc_hvdc()
{
    vsc.set_converter_count(5);
}


void VSC_HVDC_TEST::prepare_2_terminal_vsc_hvdc_converter_ac_buses()
{
    vsc.set_converter_bus(0,1);
    vsc.set_converter_bus(1,2);
}

void VSC_HVDC_TEST::prepare_3_terminal_vsc_hvdc_converter_ac_buses()
{
    prepare_2_terminal_vsc_hvdc_converter_ac_buses();
    vsc.set_converter_bus(2,3);
}

void VSC_HVDC_TEST::prepare_4_terminal_vsc_hvdc_converter_ac_buses()
{
    prepare_3_terminal_vsc_hvdc_converter_ac_buses();
    vsc.set_converter_bus(3,4);
}

void VSC_HVDC_TEST::prepare_5_terminal_vsc_hvdc_converter_ac_buses()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    BUS bus(default_toolkit);

    bus.set_bus_number(101);
    bus.set_bus_name("BUS 101");
    bus.set_base_voltage_in_kV(500.0);
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.0);
    bus.set_positive_sequence_angle_in_deg(10.0);
    psdb.append_bus(bus);

    bus.set_bus_number(105);
    bus.set_bus_name("BUS 105");
    bus.set_base_voltage_in_kV(220.0);
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.05);
    bus.set_positive_sequence_angle_in_deg(0.0);
    psdb.append_bus(bus);

    bus.set_bus_number(102);
    bus.set_bus_name("BUS 102");
    bus.set_base_voltage_in_kV(110.0);
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(0.95);
    bus.set_positive_sequence_angle_in_deg(-30.0);
    psdb.append_bus(bus);

    bus.set_bus_number(104);
    bus.set_bus_name("BUS 104");
    bus.set_base_voltage_in_kV(500.0);
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.02);
    bus.set_positive_sequence_angle_in_deg(-10.0);
    psdb.append_bus(bus);

    bus.set_bus_number(103);
    bus.set_bus_name("BUS 103");
    bus.set_base_voltage_in_kV(500.0);
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.02);
    bus.set_positive_sequence_angle_in_deg(-10.0);
    psdb.append_bus(bus);

    vsc.set_converter_count(5);
    vsc.set_dc_bus_count(7);
    vsc.set_dc_line_count(10);

    vsc.set_converter_bus(0, 105);

    vsc.set_converter_bus(1, 102);

    vsc.set_converter_bus(2, 104);

    vsc.set_converter_bus(3, 101);

    vsc.set_converter_bus(4, 103);


    vsc.set_dc_bus_number(0, 1007);
    vsc.set_ac_bus_number_of_dc_bus(0, 101);

    vsc.set_dc_bus_number(0, 1001);
    vsc.set_ac_bus_number_of_dc_bus(0, 102);

    vsc.set_dc_line_sending_side_bus(0,1003);
    vsc.set_dc_line_receiving_side_bus(0,1002);
}

//#endif

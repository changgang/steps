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

#ifdef ENABLE_STEPS_TEST
using namespace std;

VSC_HVDC_TEST::VSC_HVDC_TEST():vsc(default_toolkit)
{
    TEST_ADD(VSC_HVDC_TEST::test_constructor);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_identifier);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_name);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_converter_count);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_dc_bus_count);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_dc_line_count);
    //TEST_ADD(VSC_HVDC_TEST::test_set_get_set_ac_converter_bus_with_dc_voltage_control);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_converter_ac_bus);

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
    TEST_ADD(VSC_HVDC_TEST::test_set_get_dc_bus_ac_bus_number);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_dc_bus_owner_number);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_dc_bus_ground_resistance_in_ohm);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_dc_bus_generation_power_in_MW);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_dc_bus_load_power_in_MW);

    TEST_ADD(VSC_HVDC_TEST::test_set_get_dc_sending_side_bus);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_dc_receiving_side_bus);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_meter_end_bus);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_line_resistance_in_ohm);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_line_inductance_in_mH);
    TEST_ADD(VSC_HVDC_TEST::test_get_device_id);

    TEST_ADD(VSC_HVDC_TEST::test_build_inphno);
    TEST_ADD(VSC_HVDC_TEST::test_build_conductance_matrix);
    TEST_ADD(VSC_HVDC_TEST::test_build_jacobian);
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
    show_test_end_information();
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
    /*
    vsc.set_ac_converter_bus_with_dc_voltage_control(1);
    TEST_ASSERT(vsc.get_ac_converter_bus_with_dc_voltage_control()==1);

    vsc.set_ac_converter_bus_with_dc_voltage_control(2);
    TEST_ASSERT(vsc.get_ac_converter_bus_with_dc_voltage_control()==2);

    vsc.set_ac_converter_bus_with_dc_voltage_control(10);
    TEST_ASSERT(vsc.get_ac_converter_bus_with_dc_voltage_control()==0);

    vsc.set_ac_converter_bus_with_dc_voltage_control(20);
    TEST_ASSERT(vsc.get_ac_converter_bus_with_dc_voltage_control()==0);
    */
}

void VSC_HVDC_TEST::test_set_get_converter_ac_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");

    vsc.set_converter_count(2);

    vsc.set_converter_ac_bus(0,1);
    TEST_ASSERT(vsc.get_converter_ac_bus(0)==1);

    vsc.set_converter_ac_bus(1,2);
    TEST_ASSERT(vsc.get_converter_ac_bus(1)==2);

    vsc.set_converter_ac_bus(2,3);
    TEST_ASSERT(vsc.get_converter_ac_bus(2)==0);

    vsc.set_converter_count(5);

    vsc.set_converter_ac_bus(0,1);
    TEST_ASSERT(vsc.get_converter_ac_bus(0)==1);

    vsc.set_converter_ac_bus(1,2);
    TEST_ASSERT(vsc.get_converter_ac_bus(1)==2);

    vsc.set_converter_ac_bus(2,3);
    TEST_ASSERT(vsc.get_converter_ac_bus(2)==3);

    vsc.set_converter_ac_bus(3,4);
    TEST_ASSERT(vsc.get_converter_ac_bus(3)==4);

    vsc.set_converter_ac_bus(4,5);
    TEST_ASSERT(vsc.get_converter_ac_bus(4)==5);

    vsc.set_converter_ac_bus(5,6);
    TEST_ASSERT(vsc.get_converter_ac_bus(5)==0);
}

void VSC_HVDC_TEST::test_set_get_converter_dc_operation_mode()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");

    prepare_2_terminal_vsc_hvdc();

    vsc.set_converter_ac_bus(0,1);
    vsc.set_converter_ac_bus(1,2);

    TEST_ASSERT(vsc.get_ac_converter_bus_with_dc_voltage_control()==0);
    vsc.set_converter_dc_operation_mode(0,VSC_DC_VOLTAGE_CONTORL);
    TEST_ASSERT(vsc.get_converter_dc_operation_mode(0)==VSC_DC_VOLTAGE_CONTORL);
    TEST_ASSERT(vsc.get_ac_converter_bus_with_dc_voltage_control()==1);

    vsc.set_converter_dc_operation_mode(0,VSC_AC_ACTIVE_POWER_CONTORL);
    TEST_ASSERT(vsc.get_converter_dc_operation_mode(0)==VSC_AC_ACTIVE_POWER_CONTORL);

    vsc.set_converter_dc_operation_mode(1,VSC_DC_VOLTAGE_CONTORL);
    TEST_ASSERT(vsc.get_converter_dc_operation_mode(1)==VSC_DC_VOLTAGE_CONTORL);
    TEST_ASSERT(vsc.get_ac_converter_bus_with_dc_voltage_control()==1);

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

void VSC_HVDC_TEST::test_set_get_dc_bus_ac_bus_number()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_2_terminal_vsc_hvdc();

    vsc.set_dc_bus_ac_bus_number(0,212);
    TEST_ASSERT(fabs(vsc.get_dc_bus_ac_bus_number(0)-212)<FLOAT_EPSILON);

    vsc.set_dc_bus_ac_bus_number(1,213);
    TEST_ASSERT(fabs(vsc.get_dc_bus_ac_bus_number(1)-213)<FLOAT_EPSILON);
}

void VSC_HVDC_TEST::test_set_get_dc_bus_owner_number()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_2_terminal_vsc_hvdc();

    vsc.set_dc_bus_owner_number(0,2);
    TEST_ASSERT(fabs(vsc.get_dc_bus_owner_number(0)-2)<FLOAT_EPSILON);

    vsc.set_dc_bus_owner_number(1,1);
    TEST_ASSERT(fabs(vsc.get_dc_bus_owner_number(1)-1)<FLOAT_EPSILON);
}

void VSC_HVDC_TEST::test_set_get_dc_bus_ground_resistance_in_ohm()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_2_terminal_vsc_hvdc();

    vsc.set_dc_bus_ground_resistance_in_ohm(0,1.02);
    TEST_ASSERT(fabs(vsc.get_dc_bus_ground_resistance_in_ohm(0)-1.02)<FLOAT_EPSILON);

    vsc.set_dc_bus_ground_resistance_in_ohm(1,2.02);
    TEST_ASSERT(fabs(vsc.get_dc_bus_ground_resistance_in_ohm(1)-2.02)<FLOAT_EPSILON);
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

    vsc.set_converter_ac_bus(0,1);
    vsc.set_converter_ac_bus(1,2);
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

void VSC_HVDC_TEST::test_is_connected_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_2_terminal_vsc_hvdc();

    vsc.set_converter_ac_bus(0,1);
    vsc.set_converter_ac_bus(1,2);
    vsc.set_identifier("#1");

    TEST_ASSERT(vsc.is_connected_to_bus(1)==true);
    TEST_ASSERT(vsc.is_connected_to_bus(2)==true);
    TEST_ASSERT(vsc.is_connected_to_bus(3)==false);

}

void VSC_HVDC_TEST::test_is_valid()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_2_terminal_vsc_hvdc();

    vsc.set_converter_ac_bus(0,1);
    vsc.set_converter_ac_bus(1,2);
    vsc.set_identifier("#1");
    TEST_ASSERT(vsc.is_valid()==true);

    vsc.set_converter_ac_bus(1,6);
    TEST_ASSERT(vsc.is_valid()==false);
}


void VSC_HVDC_TEST::test_build_inphno()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_5_terminal_vsc_hvdc_converter_ac_buses();
    vsc.build_inphno();
    TEST_ASSERT(true);
}

void VSC_HVDC_TEST::test_build_conductance_matrix()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_5_terminal_vsc_hvdc_converter_ac_buses();
    vsc.build_dc_network_matrix();
    TEST_ASSERT(fabs(vsc.get_dc_network_matrix_entry_between_dc_bus(3,3)-1.666)<FLOAT_EPSILON);
    vsc.show_dc_network_matrix();
    vsc.export_dc_network_matrix("test_DC_network.csv");
}

void VSC_HVDC_TEST::test_build_jacobian()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_5_terminal_vsc_hvdc_converter_ac_buses();
    vsc.build_inphno();
    vsc.initialize_Udc_vector();
    vsc.initialize_alpha_vector();
    vsc.initialize_beta_vector();
    vsc.build_jacobian();
    vsc.show_jacobian_matrix();
    TEST_ASSERT(true);
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
    vsc.set_converter_ac_bus(0,1);
    vsc.set_converter_ac_bus(1,2);
}

void VSC_HVDC_TEST::prepare_3_terminal_vsc_hvdc_converter_ac_buses()
{
    prepare_2_terminal_vsc_hvdc_converter_ac_buses();
    vsc.set_converter_ac_bus(2,3);
}

void VSC_HVDC_TEST::prepare_4_terminal_vsc_hvdc_converter_ac_buses()
{
    prepare_3_terminal_vsc_hvdc_converter_ac_buses();
    vsc.set_converter_ac_bus(3,4);
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

    vsc.set_dc_network_base_voltage_in_kV(200.0);

    vsc.set_converter_count(5);
    vsc.set_dc_bus_count(7);
    vsc.set_dc_line_count(7);

    vsc.set_converter_ac_bus(0, 105);
    vsc.set_converter_ac_bus(1, 102);
    vsc.set_converter_ac_bus(2, 104);
    vsc.set_converter_ac_bus(3, 101);
    vsc.set_converter_ac_bus(4, 103);

    vsc.set_dc_bus_number(0, 1);
    vsc.set_dc_bus_ac_bus_number(0, 101);
    vsc.set_dc_bus_number(1, 2);
    vsc.set_dc_bus_ac_bus_number(1, 102);
    vsc.set_dc_bus_number(2,3);
    vsc.set_dc_bus_ac_bus_number(2,103);
    vsc.set_dc_bus_number(3,4);
    vsc.set_dc_bus_ac_bus_number(3,104);
    vsc.set_dc_bus_number(4,5);
    vsc.set_dc_bus_ac_bus_number(4,105);
    vsc.set_dc_bus_number(5,6);
    vsc.set_dc_bus_ac_bus_number(5,0);
    vsc.set_dc_bus_number(6,7);
    vsc.set_dc_bus_ac_bus_number(6,0);

    vsc.set_dc_line_sending_side_bus(0, 1);
    vsc.set_dc_line_receiving_side_bus(0, 6);
    vsc.set_dc_line_sending_side_bus(1, 1);
    vsc.set_dc_line_receiving_side_bus(1, 7);
    vsc.set_dc_line_sending_side_bus(2, 2);
    vsc.set_dc_line_receiving_side_bus(2, 3);
    vsc.set_dc_line_sending_side_bus(3, 2);
    vsc.set_dc_line_receiving_side_bus(3, 4);
    vsc.set_dc_line_sending_side_bus(4, 3);
    vsc.set_dc_line_receiving_side_bus(4, 7);
    vsc.set_dc_line_sending_side_bus(5, 4);
    vsc.set_dc_line_receiving_side_bus(5, 7);
    vsc.set_dc_line_sending_side_bus(6, 5);
    vsc.set_dc_line_receiving_side_bus(6, 6);

    vsc.set_dc_line_resistance_in_ohm(0, 4);
    vsc.set_dc_line_resistance_in_ohm(1, 2);
    vsc.set_dc_line_resistance_in_ohm(2, 6);
    vsc.set_dc_line_resistance_in_ohm(3, 6);
    vsc.set_dc_line_resistance_in_ohm(4, 4);
    vsc.set_dc_line_resistance_in_ohm(5, 6);
    vsc.set_dc_line_resistance_in_ohm(6, 6);

    vsc.set_converter_ac_operation_mode(0, VSC_AC_REACTIVE_POWER_CONTROL);
    vsc.set_converter_dc_operation_mode(0, VSC_DC_VOLTAGE_CONTORL);
    vsc.set_converter_ac_operation_mode(1, VSC_AC_REACTIVE_POWER_CONTROL);
    vsc.set_converter_dc_operation_mode(1, VSC_AC_ACTIVE_POWER_CONTORL);
    vsc.set_converter_ac_operation_mode(2, VSC_AC_REACTIVE_POWER_CONTROL);
    vsc.set_converter_dc_operation_mode(2, VSC_DC_ACTIVE_POWER_VOLTAGE_DROOP_CONTROL);
    vsc.set_converter_ac_operation_mode(3, VSC_AC_REACTIVE_POWER_CONTROL);
    vsc.set_converter_dc_operation_mode(3, VSC_DC_ACTIVE_POWER_VOLTAGE_DROOP_CONTROL);
    vsc.set_converter_ac_operation_mode(4, VSC_AC_VOLTAGE_CONTROL);
    vsc.set_converter_dc_operation_mode(4, VSC_AC_ACTIVE_POWER_CONTORL);

    vsc.set_converter_nominal_ac_reactive_power_command_in_Mvar(0,50);
    vsc.set_converter_nominal_dc_voltage_command_in_kV(0,200);

    vsc.set_converter_nominal_ac_reactive_power_command_in_Mvar(1,50);
    vsc.set_converter_nominal_ac_active_power_command_in_MW(1,60);

    vsc.set_converter_nominal_ac_reactive_power_command_in_Mvar(2,40);
    vsc.set_converter_initial_dc_active_power_reference_in_MW(2,60);
    vsc.set_converter_initial_dc_voltage_reference_in_kV(2,220);
    vsc.set_converter_initial_power_voltage_droop_coefficient(2,1.0);

    vsc.set_converter_nominal_ac_reactive_power_command_in_Mvar(3,-70);
    vsc.set_converter_initial_dc_active_power_reference_in_MW(3,-81);
    vsc.set_converter_initial_dc_voltage_reference_in_kV(3,190);
    vsc.set_converter_initial_power_voltage_droop_coefficient(3,0.5);

    vsc.set_converter_nominal_ac_reactive_power_command_in_Mvar(4,80);
    vsc.set_converter_nominal_ac_active_power_command_in_MW(4,-50);

}

#endif

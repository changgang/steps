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

    TEST_ADD(VSC_HVDC_TEST::test_set_get_converter_active_power_operation_mode);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_converter_reactive_power_operation_mode);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_converter_nominal_dc_power_command_in_MW);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_converter_nominal_dc_voltage_command_in_kV);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_converter_nominal_ac_voltage_command_in_pu);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_converter_nominal_ac_power_factor_command);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_converter_loss_factor_A_in_kW);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_converter_loss_factor_B_in_kW_per_amp);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_converter_transformer_impedance_in_pu)
    TEST_ADD(VSC_HVDC_TEST::test_set_get_converter_minimum_loss_in_kW);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_converter_rated_capacity_in_MVA);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_converter_rated_current_in_A);
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

    TEST_ADD(VSC_HVDC_TEST::test_initialize_steady_state_solver);
    TEST_ADD(VSC_HVDC_TEST::test_optimize_network_ordering);
    TEST_ADD(VSC_HVDC_TEST::test_build_dc_network_matrix);
    TEST_ADD(VSC_HVDC_TEST::test_update_P_equation_internal_buses);
    TEST_ADD(VSC_HVDC_TEST::test_update_current_dc_slack_bus);

    TEST_ADD(VSC_HVDC_TEST::test_calculate_raw_bus_power_mismatch);
    TEST_ADD(VSC_HVDC_TEST::test_build_dc_bus_power_mismatch_vector);
    TEST_ADD(VSC_HVDC_TEST::test_build_jacobian_with_5_terminal_data);
    TEST_ADD(VSC_HVDC_TEST::test_update_dc_bus_voltage);
    TEST_ADD(VSC_HVDC_TEST::test_solve_dc_network_with_5_terminal_without_VP_droop)
    TEST_ADD(VSC_HVDC_TEST::test_solve_dc_network_with_5_terminal_data)
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

void VSC_HVDC_TEST::test_set_get_converter_active_power_operation_mode()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");

    prepare_2_terminal_vsc_hvdc();

    vsc.set_converter_ac_bus(0,1);
    vsc.set_converter_ac_bus(1,2);

    TEST_ASSERT(vsc.get_ac_converter_bus_with_dc_voltage_control()==0);
    vsc.set_converter_active_power_operation_mode(0,VSC_DC_VOLTAGE_CONTORL);
    TEST_ASSERT(vsc.get_converter_active_power_operation_mode(0)==VSC_DC_VOLTAGE_CONTORL);
    TEST_ASSERT(vsc.get_ac_converter_bus_with_dc_voltage_control()==1);

    vsc.set_converter_active_power_operation_mode(0,VSC_AC_ACTIVE_POWER_CONTORL);
    TEST_ASSERT(vsc.get_converter_active_power_operation_mode(0)==VSC_AC_ACTIVE_POWER_CONTORL);

    vsc.set_converter_active_power_operation_mode(1,VSC_DC_VOLTAGE_CONTORL);
    TEST_ASSERT(vsc.get_converter_active_power_operation_mode(1)==VSC_DC_VOLTAGE_CONTORL);
    TEST_ASSERT(vsc.get_ac_converter_bus_with_dc_voltage_control()==1);

    vsc.set_converter_active_power_operation_mode(1,VSC_AC_ACTIVE_POWER_CONTORL);
    TEST_ASSERT(vsc.get_converter_active_power_operation_mode(1)==VSC_AC_ACTIVE_POWER_CONTORL);
}

void VSC_HVDC_TEST::test_set_get_converter_reactive_power_operation_mode()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");

    prepare_2_terminal_vsc_hvdc();

    vsc.set_converter_reactive_power_operation_mode(0,VSC_AC_VOLTAGE_CONTROL);
    TEST_ASSERT(vsc.get_converter_reactive_power_operation_mode(0)==VSC_AC_VOLTAGE_CONTROL);

    vsc.set_converter_reactive_power_operation_mode(0,VSC_AC_REACTIVE_POWER_CONTROL);
    TEST_ASSERT(vsc.get_converter_reactive_power_operation_mode(0)==VSC_AC_REACTIVE_POWER_CONTROL);

    vsc.set_converter_reactive_power_operation_mode(1,VSC_AC_VOLTAGE_CONTROL);
    TEST_ASSERT(vsc.get_converter_reactive_power_operation_mode(1)==VSC_AC_VOLTAGE_CONTROL);

    vsc.set_converter_reactive_power_operation_mode(1,VSC_AC_REACTIVE_POWER_CONTROL);
    TEST_ASSERT(vsc.get_converter_reactive_power_operation_mode(1)==VSC_AC_REACTIVE_POWER_CONTROL);
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

void VSC_HVDC_TEST::test_set_get_converter_transformer_impedance_in_pu()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_2_terminal_vsc_hvdc();

    complex<double> Zt(0.1,0.1);
    vsc.set_converter_transformer_impedance_in_pu(0,Zt);
    TEST_ASSERT(vsc.get_converter_transformer_impedance_in_pu(0)==Zt);
    TEST_ASSERT(vsc.get_converter_transformer_impedance_in_pu(0).real()==0.1);
    TEST_ASSERT(vsc.get_converter_transformer_impedance_in_pu(0).imag()==0.1);

    vsc.set_converter_transformer_impedance_in_pu(1,0.0);
    TEST_ASSERT(vsc.get_converter_transformer_impedance_in_pu(1)==0.0);
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

void VSC_HVDC_TEST::test_set_get_converter_rated_current_in_A()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_2_terminal_vsc_hvdc();

    vsc.set_converter_rated_current_in_A(0,10.1);
    TEST_ASSERT(fabs(vsc.get_converter_rated_current_in_A(0)-10.1)<FLOAT_EPSILON);

    vsc.set_converter_rated_current_in_A(1,12.2);
    TEST_ASSERT(fabs(vsc.get_converter_rated_current_in_A(1)-12.2)<FLOAT_EPSILON);
}

void VSC_HVDC_TEST::test_set_get_converter_Qmax_in_MVar()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_2_terminal_vsc_hvdc();

    vsc.set_converter_Qmax_in_MVar(0,0.8);
    TEST_ASSERT(fabs(vsc.get_converter_Qmax_in_MVar(0)-0.8)<FLOAT_EPSILON);

    vsc.set_converter_Qmax_in_MVar(1,-1.1);
    TEST_ASSERT(fabs(vsc.get_converter_Qmax_in_MVar(1)+1.1)<FLOAT_EPSILON);
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
    prepare_3_terminal_vsc_hvdc();

    vsc.set_converter_ac_bus(0,101);
    vsc.set_converter_ac_bus(1,111);
    vsc.set_converter_ac_bus(2,121);
    vsc.set_dc_bus_converter_index_with_ac_bus_number(0,111);
    vsc.set_dc_bus_converter_index_with_ac_bus_number(1,121);
    vsc.set_dc_bus_converter_index_with_ac_bus_number(2,101);
    vsc.set_dc_bus_number(0,1);
    vsc.set_dc_bus_number(1,2);
    vsc.set_dc_bus_number(2,3);
    TEST_ASSERT(fabs(vsc.get_converter_ac_bus_number_with_dc_bus_index(0)-111)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(vsc.get_converter_ac_bus_number_with_dc_bus_index(1)-121)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(vsc.get_converter_ac_bus_number_with_dc_bus_index(2)-101)<FLOAT_EPSILON);

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

void VSC_HVDC_TEST::test_initialize_steady_state_solver()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_5_terminal_vsc_hvdc_converter_ac_buses();
    vsc.initialize_alpha_vector();
    vsc.initialize_beta_vector();
    vsc.initialize_active_and_reactive_power_control_mode_vector();
    vsc.initialize_dc_bus_voltage();
}

void VSC_HVDC_TEST::test_optimize_network_ordering()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_5_terminal_vsc_hvdc_converter_ac_buses();
    vsc.optimize_network_ordering();
    vsc.show_inphno_bus_number();
    TEST_ASSERT(true);
}

void VSC_HVDC_TEST::test_build_dc_network_matrix()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_5_terminal_vsc_hvdc_converter_ac_buses();
    vsc.optimize_network_ordering();
    vsc.build_dc_network_matrix();
    vsc.show_dc_network_matrix();

    TEST_ASSERT(fabs(vsc.get_dc_network_matrix_entry_between_dc_bus(3,3)-0.4167)<0.01);
    TEST_ASSERT(fabs(vsc.get_dc_network_matrix_entry_between_dc_bus(4,4)-0.3333)<0.01);
    TEST_ASSERT(fabs(vsc.get_dc_network_matrix_entry_between_dc_bus(6,6)-0.41667)<0.01);
    TEST_ASSERT(fabs(vsc.get_dc_network_matrix_entry_between_dc_bus(3,2)+0.1667)<0.01);
    TEST_ASSERT(fabs(vsc.get_dc_network_matrix_entry_between_dc_bus(7,7)-0.91667)<0.01);
    //vsc.export_dc_network_matrix("test_DC_network.csv");
}

void VSC_HVDC_TEST::test_update_P_equation_internal_buses()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_5_terminal_vsc_hvdc_converter_ac_buses();
    vsc.initialize_steady_state_solver();
    vsc.optimize_network_ordering();
    vsc.build_dc_network_matrix();
    vsc.update_P_equation_internal_buses();

    //vsc.show_jacobian_matrix();
    //TEST_ASSERT((vsc.get_jacobian_matrix_entry_between_dc_bus(4,4)+67.666)<1);
    //TEST_ASSERT((vsc.get_jacobian_matrix_entry_between_dc_bus(6,6)+80.8333)<1);
    //TEST_ASSERT((vsc.get_jacobian_matrix_entry_between_dc_bus(4,2)-33.333)<1);
    //sTEST_ASSERT((vsc.get_jacobian_matrix_entry_between_dc_bus(3,3)+83.3333)<1);
}

void VSC_HVDC_TEST::test_update_current_dc_slack_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_5_terminal_vsc_hvdc_converter_ac_buses();
    vsc.update_current_dc_slack_bus();

}

void VSC_HVDC_TEST::test_calculate_raw_bus_power_mismatch()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_5_terminal_vsc_hvdc_converter_ac_buses();
    vsc.initialize_steady_state_solver();
    vsc.optimize_network_ordering();
    vsc.build_dc_network_matrix();
    vsc.update_P_equation_internal_buses();
    vsc.update_current_dc_slack_bus();
    vsc.export_dc_bus_voltage_with_network_ordering();
    vsc.calculate_raw_bus_power_mismatch();

}

void VSC_HVDC_TEST::test_build_dc_bus_power_mismatch_vector()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_5_terminal_vsc_hvdc_converter_ac_buses();
    vsc.initialize_steady_state_solver();
    vsc.optimize_network_ordering();
    vsc.build_dc_network_matrix();
    vsc.update_P_equation_internal_buses();
    vsc.update_current_dc_slack_bus();
    vsc.export_dc_bus_voltage_with_network_ordering();
    vsc.calculate_raw_bus_power_mismatch();
    vsc.build_dc_bus_power_mismatch_vector();

}

void VSC_HVDC_TEST::test_build_jacobian_with_5_terminal_data()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_5_terminal_vsc_hvdc_converter_ac_buses();
    vsc.initialize_steady_state_solver();
    vsc.optimize_network_ordering();
    vsc.build_dc_network_matrix();
    vsc.update_P_equation_internal_buses();
    vsc.update_current_dc_slack_bus();
    vsc.export_dc_bus_voltage_with_network_ordering();
    vsc.calculate_raw_bus_power_mismatch();
    vsc.build_dc_bus_power_mismatch_vector();
    vsc.build_jacobian();
    vsc.show_jacobian_matrix();
}

void VSC_HVDC_TEST::test_update_dc_bus_voltage()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_5_terminal_vsc_hvdc_converter_ac_buses();
    vsc.initialize_steady_state_solver();
    vsc.optimize_network_ordering();
    vsc.build_dc_network_matrix();
    vsc.update_P_equation_internal_buses();
    vsc.update_current_dc_slack_bus();
    vsc.export_dc_bus_voltage_with_network_ordering();
    vsc.calculate_raw_bus_power_mismatch();
    vsc.build_dc_bus_power_mismatch_vector();
    vsc.update_dc_bus_voltage();
}

void VSC_HVDC_TEST::test_solve_dc_network_with_5_terminal_without_VP_droop()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_5_terminal_vsc_hvdc_converter_without_droop_control();
    vsc.set_max_iteration(4);
    vsc.set_allowed_max_P_mismatch_in_MW(0.0001);
    vsc.solve_steady_state();
}

void VSC_HVDC_TEST::test_solve_dc_network_with_5_terminal_data()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VSC_HVDC_TEST");
    prepare_5_terminal_vsc_hvdc_converter_ac_buses();
    vsc.set_max_iteration(4);
    vsc.set_allowed_max_P_mismatch_in_MW(0.0001);
    vsc.solve_steady_state();
}

void VSC_HVDC_TEST::test_set_get_vsc_hvdc_model()
{

}


void VSC_HVDC_TEST::prepare_2_terminal_vsc_hvdc()
{
    vsc.set_converter_count(2);
    vsc.set_dc_bus_count(2);
    vsc.set_dc_line_count(1);

}

void VSC_HVDC_TEST::prepare_3_terminal_vsc_hvdc()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    BUS bus(default_toolkit);

    vsc.set_converter_count(3);
    vsc.set_dc_bus_count(3);
    vsc.set_dc_line_count(3);

    bus.set_bus_number(101);
    bus.set_bus_name("BUS 101");
    bus.set_base_voltage_in_kV(500.0);
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.0);
    bus.set_positive_sequence_angle_in_deg(10.0);
    psdb.append_bus(bus);

    bus.set_bus_number(111);
    bus.set_bus_name("BUS 105");
    bus.set_base_voltage_in_kV(220.0);
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.05);
    bus.set_positive_sequence_angle_in_deg(0.0);
    psdb.append_bus(bus);

    bus.set_bus_number(121);
    bus.set_bus_name("BUS 102");
    bus.set_base_voltage_in_kV(110.0);
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(0.95);
    bus.set_positive_sequence_angle_in_deg(-30.0);
    psdb.append_bus(bus);
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
    bus.set_base_voltage_in_kV(220.0);
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
    bus.set_base_voltage_in_kV(220.0);
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(0.95);
    bus.set_positive_sequence_angle_in_deg(-30.0);
    psdb.append_bus(bus);

    bus.set_bus_number(104);
    bus.set_bus_name("BUS 104");
    bus.set_base_voltage_in_kV(220.0);
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.02);
    bus.set_positive_sequence_angle_in_deg(-10.0);
    psdb.append_bus(bus);

    bus.set_bus_number(103);
    bus.set_bus_name("BUS 103");
    bus.set_base_voltage_in_kV(220.0);
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.02);
    bus.set_positive_sequence_angle_in_deg(-10.0);
    psdb.append_bus(bus);

    vsc.set_dc_network_base_voltage_in_kV(200.0);

    vsc.set_converter_count(5);
    vsc.set_dc_bus_count(7);
    vsc.set_dc_line_count(7);
    vsc.set_reserve_master_converter_ac_bus(102);

    vsc.set_converter_ac_bus(0, 105);
    vsc.set_converter_ac_bus(1, 102);
    vsc.set_converter_ac_bus(2, 104);
    vsc.set_converter_ac_bus(3, 101);
    vsc.set_converter_ac_bus(4, 103);

    vsc.set_dc_bus_number(0, 1);
    vsc.set_dc_bus_converter_index_with_ac_bus_number(0, 101);
    vsc.set_dc_bus_Vdc_in_kV(0,0.0);
    vsc.set_dc_bus_number(1, 2);
    vsc.set_dc_bus_converter_index_with_ac_bus_number(1, 102);
    vsc.set_dc_bus_Vdc_in_kV(1,0.0);
    vsc.set_dc_bus_number(2,3);
    vsc.set_dc_bus_converter_index_with_ac_bus_number(2,103);
    vsc.set_dc_bus_Vdc_in_kV(2,0.0);
    vsc.set_dc_bus_number(3,4);
    vsc.set_dc_bus_converter_index_with_ac_bus_number(3,104);
    vsc.set_dc_bus_Vdc_in_kV(3,0.0);
    vsc.set_dc_bus_number(4,5);
    vsc.set_dc_bus_converter_index_with_ac_bus_number(4,105);
    vsc.set_dc_bus_Vdc_in_kV(4,0.0);
    vsc.set_dc_bus_number(5,6);
    vsc.set_dc_bus_converter_index_with_ac_bus_number(5,0);
    vsc.set_dc_bus_Vdc_in_kV(5,0.0);
    vsc.set_dc_bus_generation_power_in_MW(5,10);
    vsc.set_dc_bus_load_power_in_MW(5,5);
    vsc.set_dc_bus_number(6,7);
    vsc.set_dc_bus_converter_index_with_ac_bus_number(6,0);
    vsc.set_dc_bus_Vdc_in_kV(6,0.0);
    vsc.set_dc_bus_generation_power_in_MW(6,12);
    vsc.set_dc_bus_load_power_in_MW(6,4);

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

    vsc.set_converter_reactive_power_operation_mode(0, VSC_AC_REACTIVE_POWER_CONTROL);
    vsc.set_converter_active_power_operation_mode(0, VSC_DC_VOLTAGE_CONTORL);
    vsc.set_converter_reactive_power_operation_mode(1, VSC_AC_REACTIVE_POWER_CONTROL);
    vsc.set_converter_active_power_operation_mode(1, VSC_AC_ACTIVE_POWER_CONTORL);
    vsc.set_converter_reactive_power_operation_mode(2, VSC_AC_REACTIVE_POWER_CONTROL);
    vsc.set_converter_active_power_operation_mode(2, VSC_DC_ACTIVE_POWER_VOLTAGE_DROOP_CONTROL);
    vsc.set_converter_reactive_power_operation_mode(3, VSC_AC_REACTIVE_POWER_CONTROL);
    vsc.set_converter_active_power_operation_mode(3, VSC_DC_ACTIVE_POWER_VOLTAGE_DROOP_CONTROL);
    vsc.set_converter_reactive_power_operation_mode(4, VSC_AC_VOLTAGE_CONTROL);
    vsc.set_converter_active_power_operation_mode(4, VSC_AC_ACTIVE_POWER_CONTORL);

    vsc.set_converter_nominal_ac_reactive_power_command_in_Mvar(0,50);
    vsc.set_converter_initial_dc_voltage_reference_in_kV(0,215);

    vsc.set_converter_nominal_ac_reactive_power_command_in_Mvar(1,50);
    vsc.set_converter_nominal_ac_active_power_command_in_MW(1,60);

    vsc.set_converter_nominal_ac_reactive_power_command_in_Mvar(2,40);
    vsc.set_converter_initial_dc_active_power_reference_in_MW(2,55);
    vsc.set_converter_initial_dc_voltage_reference_in_kV(2,210);
    vsc.set_converter_initial_power_voltage_droop_coefficient(2,100);

    vsc.set_converter_nominal_ac_reactive_power_command_in_Mvar(3,70);
    vsc.set_converter_initial_dc_active_power_reference_in_MW(3,-81);
    vsc.set_converter_initial_dc_voltage_reference_in_kV(3,205);
    vsc.set_converter_initial_power_voltage_droop_coefficient(3,50);

    vsc.set_converter_nominal_ac_reactive_power_command_in_Mvar(4,80);
    vsc.set_converter_nominal_ac_active_power_command_in_MW(4,50);

    vsc.set_converter_loss_factor_A_in_kW(0,12);
    vsc.set_converter_loss_factor_B_in_kW_per_amp(0,0.004);
    vsc.set_converter_loss_factor_C_in_KW_per_amp_squared(0,0.0006);
    vsc.set_converter_minimum_loss_in_kW(0,100);
    vsc.set_converter_rated_capacity_in_MVA(0,200);
    vsc.set_converter_rated_current_in_A(0,100);
    vsc.set_converter_transformer_capacity_in_MVA(0,200);
    vsc.set_converter_transformer_AC_side_base_voltage_in_kV(0,220);
    vsc.set_converter_transformer_converter_side_base_voltage_in_kV(0,200);
    vsc.set_converter_transformer_off_nominal_turn_ratio(0,1.1);
    complex<double> Zt(0.001,0.05);
    complex<double> Zc(0.5,5.0);
    complex<double> Yf(0.0,0.001);
    vsc.set_converter_transformer_impedance_in_pu(0,Zt);
    vsc.set_converter_commutating_impedance_in_ohm(0,Zc);
    vsc.set_converter_filter_admittance_in_siemens(0,Yf);
    vsc.set_converter_Pmax_in_MW(0,144);
    vsc.set_converter_Pmin_in_MW(0,-144);
    vsc.set_converter_Qmax_in_MVar(0,150);
    vsc.set_converter_Qmin_in_MVar(0,-150);
    vsc.set_converter_Udmax_in_kV(0,220);
    vsc.set_converter_Udmin_in_kV(0,180);

    vsc.set_converter_loss_factor_A_in_kW(1,12);
    vsc.set_converter_loss_factor_B_in_kW_per_amp(1,0.014);
    vsc.set_converter_loss_factor_C_in_KW_per_amp_squared(1,0.006);
    vsc.set_converter_minimum_loss_in_kW(1,100);
    vsc.set_converter_rated_capacity_in_MVA(1,200);
    vsc.set_converter_rated_current_in_A(1,100);
    vsc.set_converter_transformer_capacity_in_MVA(1,200);
    vsc.set_converter_transformer_AC_side_base_voltage_in_kV(1,220);
    vsc.set_converter_transformer_converter_side_base_voltage_in_kV(1,200);
    vsc.set_converter_transformer_off_nominal_turn_ratio(1,1.1);
    vsc.set_converter_transformer_impedance_in_pu(1,Zt);

    vsc.set_converter_commutating_impedance_in_ohm(1,Zc);
    vsc.set_converter_filter_admittance_in_siemens(1,Yf);
    vsc.set_converter_Pmax_in_MW(1,1440);
    vsc.set_converter_Pmin_in_MW(1,-1440);
    vsc.set_converter_Qmax_in_MVar(1,150);
    vsc.set_converter_Qmin_in_MVar(1,-150);
    vsc.set_converter_Udmax_in_kV(1,220);
    vsc.set_converter_Udmin_in_kV(1,180);

    vsc.set_converter_loss_factor_A_in_kW(2,12);
    vsc.set_converter_loss_factor_B_in_kW_per_amp(2,0.004);
    vsc.set_converter_loss_factor_C_in_KW_per_amp_squared(2,0.0006);
    vsc.set_converter_minimum_loss_in_kW(2,100);
    vsc.set_converter_rated_capacity_in_MVA(2,200);
    vsc.set_converter_rated_current_in_A(2,100);
    vsc.set_converter_transformer_capacity_in_MVA(2,200);
    vsc.set_converter_transformer_AC_side_base_voltage_in_kV(2,220);
    vsc.set_converter_transformer_converter_side_base_voltage_in_kV(2,200);
    vsc.set_converter_transformer_off_nominal_turn_ratio(2,1.1);
    vsc.set_converter_transformer_impedance_in_pu(2,Zt);
    vsc.set_converter_commutating_impedance_in_ohm(2,Zc);
    vsc.set_converter_filter_admittance_in_siemens(2,Yf);
    vsc.set_converter_Pmax_in_MW(2,1440);
    vsc.set_converter_Pmin_in_MW(2,-1440);
    vsc.set_converter_Qmax_in_MVar(2,150);
    vsc.set_converter_Qmin_in_MVar(2,-150);
    vsc.set_converter_Udmax_in_kV(2,220);
    vsc.set_converter_Udmin_in_kV(2,180);

    vsc.set_converter_loss_factor_A_in_kW(3,12);
    vsc.set_converter_loss_factor_B_in_kW_per_amp(3,0.004);
    vsc.set_converter_loss_factor_C_in_KW_per_amp_squared(3,0.0006);
    vsc.set_converter_minimum_loss_in_kW(3,100);
    vsc.set_converter_rated_capacity_in_MVA(3,200);
    vsc.set_converter_rated_current_in_A(3,100);
    vsc.set_converter_transformer_capacity_in_MVA(3,200);
    vsc.set_converter_transformer_AC_side_base_voltage_in_kV(3,220);
    vsc.set_converter_transformer_converter_side_base_voltage_in_kV(3,200);
    vsc.set_converter_transformer_off_nominal_turn_ratio(3,1.1);
    vsc.set_converter_transformer_impedance_in_pu(3,Zt);
    vsc.set_converter_commutating_impedance_in_ohm(3,Zc);
    vsc.set_converter_filter_admittance_in_siemens(3,Yf);
    vsc.set_converter_Pmax_in_MW(3,1440);
    vsc.set_converter_Pmin_in_MW(3,-1440);
    vsc.set_converter_Qmax_in_MVar(3,150);
    vsc.set_converter_Qmin_in_MVar(3,-150);
    vsc.set_converter_Udmax_in_kV(3,220);
    vsc.set_converter_Udmin_in_kV(3,180);

    vsc.set_converter_loss_factor_A_in_kW(4,12);
    vsc.set_converter_loss_factor_B_in_kW_per_amp(4,0.004);
    vsc.set_converter_loss_factor_C_in_KW_per_amp_squared(4,0.0006);
    vsc.set_converter_minimum_loss_in_kW(4,100);
    vsc.set_converter_rated_capacity_in_MVA(4,200);
    vsc.set_converter_rated_current_in_A(4,100);
    vsc.set_converter_transformer_capacity_in_MVA(4,200);
    vsc.set_converter_transformer_AC_side_base_voltage_in_kV(4,220);
    vsc.set_converter_transformer_converter_side_base_voltage_in_kV(4,200);
    vsc.set_converter_transformer_off_nominal_turn_ratio(4,1.1);
    vsc.set_converter_transformer_impedance_in_pu(4,Zt);
    vsc.set_converter_commutating_impedance_in_ohm(4,Zc);
    vsc.set_converter_filter_admittance_in_siemens(4,Yf);
    vsc.set_converter_Pmax_in_MW(4,1440);
    vsc.set_converter_Pmin_in_MW(4,-1440);
    vsc.set_converter_Qmax_in_MVar(4,150);
    vsc.set_converter_Qmin_in_MVar(4,-150);
    vsc.set_converter_Udmax_in_kV(4,220);
    vsc.set_converter_Udmin_in_kV(4,180);


}

void VSC_HVDC_TEST::prepare_5_terminal_vsc_hvdc_converter_without_droop_control()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    BUS bus(default_toolkit);

    bus.set_bus_number(101);
    bus.set_bus_name("BUS 101");
    bus.set_base_voltage_in_kV(220.0);
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
    bus.set_base_voltage_in_kV(220.0);
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(0.95);
    bus.set_positive_sequence_angle_in_deg(-30.0);
    psdb.append_bus(bus);

    bus.set_bus_number(104);
    bus.set_bus_name("BUS 104");
    bus.set_base_voltage_in_kV(220.0);
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.02);
    bus.set_positive_sequence_angle_in_deg(-10.0);
    psdb.append_bus(bus);

    bus.set_bus_number(103);
    bus.set_bus_name("BUS 103");
    bus.set_base_voltage_in_kV(220.0);
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.02);
    bus.set_positive_sequence_angle_in_deg(-10.0);
    psdb.append_bus(bus);

    vsc.set_dc_network_base_voltage_in_kV(200.0);

    vsc.set_converter_count(5);
    vsc.set_dc_bus_count(7);
    vsc.set_dc_line_count(7);
    vsc.set_reserve_master_converter_ac_bus(102);

    vsc.set_converter_ac_bus(0, 105);
    vsc.set_converter_ac_bus(1, 102);
    vsc.set_converter_ac_bus(2, 104);
    vsc.set_converter_ac_bus(3, 101);
    vsc.set_converter_ac_bus(4, 103);

    vsc.set_dc_bus_number(0, 1);
    vsc.set_dc_bus_converter_index_with_ac_bus_number(0, 101);
    vsc.set_dc_bus_Vdc_in_kV(0,0.0);
    vsc.set_dc_bus_number(1, 2);
    vsc.set_dc_bus_converter_index_with_ac_bus_number(1, 102);
    vsc.set_dc_bus_Vdc_in_kV(1,0.0);
    vsc.set_dc_bus_number(2,3);
    vsc.set_dc_bus_converter_index_with_ac_bus_number(2,103);
    vsc.set_dc_bus_Vdc_in_kV(2,0.0);
    vsc.set_dc_bus_number(3,4);
    vsc.set_dc_bus_converter_index_with_ac_bus_number(3,104);
    vsc.set_dc_bus_Vdc_in_kV(3,0.0);
    vsc.set_dc_bus_number(4,5);
    vsc.set_dc_bus_converter_index_with_ac_bus_number(4,105);
    vsc.set_dc_bus_Vdc_in_kV(4,0.0);
    vsc.set_dc_bus_number(5,6);
    vsc.set_dc_bus_converter_index_with_ac_bus_number(5,0);
    vsc.set_dc_bus_Vdc_in_kV(5,0.0);
    vsc.set_dc_bus_generation_power_in_MW(5,10);
    vsc.set_dc_bus_load_power_in_MW(5,5);
    vsc.set_dc_bus_number(6,7);
    vsc.set_dc_bus_converter_index_with_ac_bus_number(6,0);
    vsc.set_dc_bus_Vdc_in_kV(6,0.0);
    vsc.set_dc_bus_generation_power_in_MW(6,12);
    vsc.set_dc_bus_load_power_in_MW(6,4);

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

    vsc.set_converter_reactive_power_operation_mode(0, VSC_AC_REACTIVE_POWER_CONTROL);
    vsc.set_converter_active_power_operation_mode(0, VSC_DC_VOLTAGE_CONTORL);
    vsc.set_converter_reactive_power_operation_mode(1, VSC_AC_REACTIVE_POWER_CONTROL);
    vsc.set_converter_active_power_operation_mode(1, VSC_AC_ACTIVE_POWER_CONTORL);
    vsc.set_converter_reactive_power_operation_mode(2, VSC_AC_REACTIVE_POWER_CONTROL);
    vsc.set_converter_active_power_operation_mode(2, VSC_AC_ACTIVE_POWER_CONTORL);
    vsc.set_converter_reactive_power_operation_mode(3, VSC_AC_REACTIVE_POWER_CONTROL);
    vsc.set_converter_active_power_operation_mode(3, VSC_AC_ACTIVE_POWER_CONTORL);
    vsc.set_converter_reactive_power_operation_mode(4, VSC_AC_VOLTAGE_CONTROL);
    vsc.set_converter_active_power_operation_mode(4, VSC_AC_ACTIVE_POWER_CONTORL);

    vsc.set_converter_nominal_ac_reactive_power_command_in_Mvar(0,50);
    vsc.set_converter_initial_dc_voltage_reference_in_kV(0,215);

    vsc.set_converter_nominal_ac_reactive_power_command_in_Mvar(1,50);
    vsc.set_converter_nominal_ac_active_power_command_in_MW(1,60);

    vsc.set_converter_nominal_ac_reactive_power_command_in_Mvar(2,40);
    vsc.set_converter_nominal_ac_active_power_command_in_MW(1,-55);

    vsc.set_converter_nominal_ac_reactive_power_command_in_Mvar(3,70);
    vsc.set_converter_nominal_ac_active_power_command_in_MW(1,-65);

    vsc.set_converter_nominal_ac_reactive_power_command_in_Mvar(4,80);
    vsc.set_converter_nominal_ac_active_power_command_in_MW(4,50);

    vsc.set_converter_loss_factor_A_in_kW(0,12);
    vsc.set_converter_loss_factor_B_in_kW_per_amp(0,0.004);
    vsc.set_converter_loss_factor_C_in_KW_per_amp_squared(0,0.0006);
    vsc.set_converter_minimum_loss_in_kW(0,100);
    vsc.set_converter_rated_capacity_in_MVA(0,200);
    vsc.set_converter_rated_current_in_A(0,100);
    vsc.set_converter_transformer_capacity_in_MVA(0,200);
    vsc.set_converter_transformer_AC_side_base_voltage_in_kV(0,220);
    vsc.set_converter_transformer_converter_side_base_voltage_in_kV(0,200);
    vsc.set_converter_transformer_off_nominal_turn_ratio(0,1.1);
    complex<double> Zt(0.001,0.05);
    complex<double> Zc(0.5,5.0);
    complex<double> Yf(0.0,0.001);
    vsc.set_converter_transformer_impedance_in_pu(0,Zt);
    vsc.set_converter_commutating_impedance_in_ohm(0,Zc);
    vsc.set_converter_filter_admittance_in_siemens(0,Yf);
    vsc.set_converter_Pmax_in_MW(0,144);
    vsc.set_converter_Pmin_in_MW(0,-144);
    vsc.set_converter_Qmax_in_MVar(0,150);
    vsc.set_converter_Qmin_in_MVar(0,-150);
    vsc.set_converter_Udmax_in_kV(0,220);
    vsc.set_converter_Udmin_in_kV(0,180);

    vsc.set_converter_loss_factor_A_in_kW(1,12);
    vsc.set_converter_loss_factor_B_in_kW_per_amp(1,0.014);
    vsc.set_converter_loss_factor_C_in_KW_per_amp_squared(1,0.006);
    vsc.set_converter_minimum_loss_in_kW(1,100);
    vsc.set_converter_rated_capacity_in_MVA(1,200);
    vsc.set_converter_rated_current_in_A(1,100);
    vsc.set_converter_transformer_capacity_in_MVA(1,200);
    vsc.set_converter_transformer_AC_side_base_voltage_in_kV(1,220);
    vsc.set_converter_transformer_converter_side_base_voltage_in_kV(1,200);
    vsc.set_converter_transformer_off_nominal_turn_ratio(1,1.1);
    vsc.set_converter_transformer_impedance_in_pu(1,Zt);

    vsc.set_converter_commutating_impedance_in_ohm(1,Zc);
    vsc.set_converter_filter_admittance_in_siemens(1,Yf);
    vsc.set_converter_Pmax_in_MW(1,1440);
    vsc.set_converter_Pmin_in_MW(1,-1440);
    vsc.set_converter_Qmax_in_MVar(1,150);
    vsc.set_converter_Qmin_in_MVar(1,-150);
    vsc.set_converter_Udmax_in_kV(1,220);
    vsc.set_converter_Udmin_in_kV(1,180);

    vsc.set_converter_loss_factor_A_in_kW(2,12);
    vsc.set_converter_loss_factor_B_in_kW_per_amp(2,0.004);
    vsc.set_converter_loss_factor_C_in_KW_per_amp_squared(2,0.0006);
    vsc.set_converter_minimum_loss_in_kW(2,100);
    vsc.set_converter_rated_capacity_in_MVA(2,200);
    vsc.set_converter_rated_current_in_A(2,100);
    vsc.set_converter_transformer_capacity_in_MVA(2,200);
    vsc.set_converter_transformer_AC_side_base_voltage_in_kV(2,220);
    vsc.set_converter_transformer_converter_side_base_voltage_in_kV(2,200);
    vsc.set_converter_transformer_off_nominal_turn_ratio(2,1.1);
    vsc.set_converter_transformer_impedance_in_pu(2,Zt);
    vsc.set_converter_commutating_impedance_in_ohm(2,Zc);
    vsc.set_converter_filter_admittance_in_siemens(2,Yf);
    vsc.set_converter_Pmax_in_MW(2,1440);
    vsc.set_converter_Pmin_in_MW(2,-1440);
    vsc.set_converter_Qmax_in_MVar(2,150);
    vsc.set_converter_Qmin_in_MVar(2,-150);
    vsc.set_converter_Udmax_in_kV(2,220);
    vsc.set_converter_Udmin_in_kV(2,180);

    vsc.set_converter_loss_factor_A_in_kW(3,12);
    vsc.set_converter_loss_factor_B_in_kW_per_amp(3,0.004);
    vsc.set_converter_loss_factor_C_in_KW_per_amp_squared(3,0.0006);
    vsc.set_converter_minimum_loss_in_kW(3,100);
    vsc.set_converter_rated_capacity_in_MVA(3,200);
    vsc.set_converter_rated_current_in_A(3,100);
    vsc.set_converter_transformer_capacity_in_MVA(3,200);
    vsc.set_converter_transformer_AC_side_base_voltage_in_kV(3,220);
    vsc.set_converter_transformer_converter_side_base_voltage_in_kV(3,200);
    vsc.set_converter_transformer_off_nominal_turn_ratio(3,1.1);
    vsc.set_converter_transformer_impedance_in_pu(3,Zt);
    vsc.set_converter_commutating_impedance_in_ohm(3,Zc);
    vsc.set_converter_filter_admittance_in_siemens(3,Yf);
    vsc.set_converter_Pmax_in_MW(3,1440);
    vsc.set_converter_Pmin_in_MW(3,-1440);
    vsc.set_converter_Qmax_in_MVar(3,150);
    vsc.set_converter_Qmin_in_MVar(3,-150);
    vsc.set_converter_Udmax_in_kV(3,220);
    vsc.set_converter_Udmin_in_kV(3,180);

    vsc.set_converter_loss_factor_A_in_kW(4,12);
    vsc.set_converter_loss_factor_B_in_kW_per_amp(4,0.004);
    vsc.set_converter_loss_factor_C_in_KW_per_amp_squared(4,0.0006);
    vsc.set_converter_minimum_loss_in_kW(4,100);
    vsc.set_converter_rated_capacity_in_MVA(4,200);
    vsc.set_converter_rated_current_in_A(4,100);
    vsc.set_converter_transformer_capacity_in_MVA(4,200);
    vsc.set_converter_transformer_AC_side_base_voltage_in_kV(4,220);
    vsc.set_converter_transformer_converter_side_base_voltage_in_kV(4,200);
    vsc.set_converter_transformer_off_nominal_turn_ratio(4,1.1);
    vsc.set_converter_transformer_impedance_in_pu(4,Zt);
    vsc.set_converter_commutating_impedance_in_ohm(4,Zc);
    vsc.set_converter_filter_admittance_in_siemens(4,Yf);
    vsc.set_converter_Pmax_in_MW(4,1440);
    vsc.set_converter_Pmin_in_MW(4,-1440);
    vsc.set_converter_Qmax_in_MVar(4,150);
    vsc.set_converter_Qmin_in_MVar(4,-150);
    vsc.set_converter_Udmax_in_kV(4,220);
    vsc.set_converter_Udmin_in_kV(4,180);

}


void VSC_HVDC_TEST::prepare_7_terminal_vsc_hvdc_converter_ac_buses()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    BUS bus(default_toolkit);

    bus.set_bus_number(101);
    bus.set_bus_name("BUS 101");
    bus.set_base_voltage_in_kV(220.0);
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
    bus.set_base_voltage_in_kV(220.0);
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(0.95);
    bus.set_positive_sequence_angle_in_deg(-30.0);
    psdb.append_bus(bus);

    bus.set_bus_number(104);
    bus.set_bus_name("BUS 104");
    bus.set_base_voltage_in_kV(220.0);
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.02);
    bus.set_positive_sequence_angle_in_deg(-10.0);
    psdb.append_bus(bus);

    bus.set_bus_number(103);
    bus.set_bus_name("BUS 103");
    bus.set_base_voltage_in_kV(220.0);
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.02);
    bus.set_positive_sequence_angle_in_deg(-10.0);
    psdb.append_bus(bus);

    bus.set_bus_number(107);
    bus.set_bus_name("BUS 107");
    bus.set_base_voltage_in_kV(220.0);
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.02);
    bus.set_positive_sequence_angle_in_deg(-10.0);
    psdb.append_bus(bus);

    bus.set_bus_number(106);
    bus.set_bus_name("BUS 106");
    bus.set_base_voltage_in_kV(220.0);
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.02);
    bus.set_positive_sequence_angle_in_deg(-10.0);
    psdb.append_bus(bus);

    vsc.set_dc_network_base_voltage_in_kV(200.0);

    vsc.set_converter_count(7);
    vsc.set_dc_bus_count(9);
    vsc.set_dc_line_count(9);

    vsc.set_converter_ac_bus(0, 105);
    vsc.set_converter_ac_bus(1, 102);
    vsc.set_converter_ac_bus(2, 104);
    vsc.set_converter_ac_bus(3, 101);
    vsc.set_converter_ac_bus(4, 103);
    vsc.set_converter_ac_bus(5, 106);
    vsc.set_converter_ac_bus(6, 107);

    vsc.set_dc_bus_number(0, 1);
    vsc.set_dc_bus_converter_index_with_ac_bus_number(0, 101);
    vsc.set_dc_bus_Vdc_in_kV(0,0.0);
    vsc.set_dc_bus_number(1, 2);
    vsc.set_dc_bus_converter_index_with_ac_bus_number(1, 102);
    vsc.set_dc_bus_Vdc_in_kV(1,0.0);
    vsc.set_dc_bus_number(2,3);
    vsc.set_dc_bus_converter_index_with_ac_bus_number(2,103);
    vsc.set_dc_bus_Vdc_in_kV(2,0.0);
    vsc.set_dc_bus_number(3,4);
    vsc.set_dc_bus_converter_index_with_ac_bus_number(3,104);
    vsc.set_dc_bus_Vdc_in_kV(3,0.0);
    vsc.set_dc_bus_number(4,5);
    vsc.set_dc_bus_converter_index_with_ac_bus_number(4,105);
    vsc.set_dc_bus_Vdc_in_kV(4,0.0);
    vsc.set_dc_bus_number(5,6);
    vsc.set_dc_bus_converter_index_with_ac_bus_number(5,0);
    vsc.set_dc_bus_Vdc_in_kV(5,0.0);
    vsc.set_dc_bus_generation_power_in_MW(5,10);
    vsc.set_dc_bus_load_power_in_MW(5,5);
    vsc.set_dc_bus_number(6,7);
    vsc.set_dc_bus_converter_index_with_ac_bus_number(6,0);
    vsc.set_dc_bus_Vdc_in_kV(6,0.0);
    vsc.set_dc_bus_generation_power_in_MW(6,12);
    vsc.set_dc_bus_load_power_in_MW(6,4);
    vsc.set_dc_bus_number(7,8);
    vsc.set_dc_bus_converter_index_with_ac_bus_number(7,106);
    vsc.set_dc_bus_Vdc_in_kV(6,0.0);
    vsc.set_dc_bus_number(8,9);
    vsc.set_dc_bus_converter_index_with_ac_bus_number(8,107);

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
    vsc.set_dc_line_sending_side_bus(7, 6);
    vsc.set_dc_line_receiving_side_bus(7, 8);
    vsc.set_dc_line_sending_side_bus(8, 7);
    vsc.set_dc_line_receiving_side_bus(8, 9);

    vsc.set_dc_line_resistance_in_ohm(0, 4);
    vsc.set_dc_line_resistance_in_ohm(1, 2);
    vsc.set_dc_line_resistance_in_ohm(2, 6);
    vsc.set_dc_line_resistance_in_ohm(3, 6);
    vsc.set_dc_line_resistance_in_ohm(4, 4);
    vsc.set_dc_line_resistance_in_ohm(5, 6);
    vsc.set_dc_line_resistance_in_ohm(6, 6);
    vsc.set_dc_line_resistance_in_ohm(7, 5);
    vsc.set_dc_line_resistance_in_ohm(8, 3);

    vsc.set_converter_reactive_power_operation_mode(0, VSC_AC_REACTIVE_POWER_CONTROL);
    vsc.set_converter_active_power_operation_mode(0, VSC_DC_VOLTAGE_CONTORL);
    vsc.set_converter_reactive_power_operation_mode(1, VSC_AC_REACTIVE_POWER_CONTROL);
    vsc.set_converter_active_power_operation_mode(1, VSC_AC_ACTIVE_POWER_CONTORL);
    vsc.set_converter_reactive_power_operation_mode(2, VSC_AC_REACTIVE_POWER_CONTROL);
    vsc.set_converter_active_power_operation_mode(2, VSC_DC_ACTIVE_POWER_VOLTAGE_DROOP_CONTROL);
    vsc.set_converter_reactive_power_operation_mode(3, VSC_AC_REACTIVE_POWER_CONTROL);
    vsc.set_converter_active_power_operation_mode(3, VSC_DC_ACTIVE_POWER_VOLTAGE_DROOP_CONTROL);
    vsc.set_converter_reactive_power_operation_mode(4, VSC_AC_VOLTAGE_CONTROL);
    vsc.set_converter_active_power_operation_mode(4, VSC_AC_ACTIVE_POWER_CONTORL);
    vsc.set_converter_reactive_power_operation_mode(5, VSC_AC_VOLTAGE_CONTROL);
    vsc.set_converter_active_power_operation_mode(5, VSC_DC_CURRENT_VOLTAGE_DROOP_CONTROL);
    vsc.set_converter_reactive_power_operation_mode(6, VSC_AC_REACTIVE_POWER_CONTROL);
    vsc.set_converter_active_power_operation_mode(6, VSC_DC_CURRENT_VOLTAGE_DROOP_CONTROL);


    vsc.set_converter_nominal_ac_reactive_power_command_in_Mvar(0,50);
    vsc.set_converter_nominal_dc_voltage_command_in_kV(0,215);

    vsc.set_converter_nominal_ac_reactive_power_command_in_Mvar(1,50);
    vsc.set_converter_nominal_ac_active_power_command_in_MW(1,60);

    vsc.set_converter_nominal_ac_reactive_power_command_in_Mvar(2,40);
    vsc.set_converter_initial_dc_active_power_reference_in_MW(2,-55);
    vsc.set_converter_initial_dc_voltage_reference_in_kV(2,220);
    vsc.set_converter_initial_power_voltage_droop_coefficient(2,1.0);

    vsc.set_converter_nominal_ac_reactive_power_command_in_Mvar(3,-70);
    vsc.set_converter_initial_dc_active_power_reference_in_MW(3,-81);
    vsc.set_converter_initial_dc_voltage_reference_in_kV(3,215);
    vsc.set_converter_initial_power_voltage_droop_coefficient(3,0.5);

    vsc.set_converter_nominal_ac_reactive_power_command_in_Mvar(4,80);
    vsc.set_converter_nominal_ac_active_power_command_in_MW(4,50);

    vsc.set_converter_nominal_ac_voltage_command_in_pu(5,1.06);
    vsc.set_converter_initial_dc_current_reference_in_kA(5,0.6);
    vsc.set_converter_initial_dc_voltage_reference_in_kV(5,209);
    vsc.set_converter_initial_power_voltage_droop_coefficient(5,1.1);

    vsc.set_converter_nominal_ac_reactive_power_command_in_Mvar(6,-70);
    vsc.set_converter_initial_dc_current_reference_in_kA(6,-0.8);
    vsc.set_converter_initial_dc_voltage_reference_in_kV(6,212);
    vsc.set_converter_initial_power_voltage_droop_coefficient(6,0.8);

}

#endif

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif
#include <cmath>
#include "cpptest.h"
#include "header/basic/test_macro.h"

#include "header/device/hvdc_test.h"
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

HVDC_TEST::HVDC_TEST()
{
    TEST_ADD(HVDC_TEST::test_constructor);
    TEST_ADD(HVDC_TEST::test_set_get_converter_bus);
    TEST_ADD(HVDC_TEST::test_set_get_converter_valve_side_bus_name);
    TEST_ADD(HVDC_TEST::test_set_get_identifier);
    TEST_ADD(HVDC_TEST::test_set_get_name);
    TEST_ADD(HVDC_TEST::test_set_get_status);
    TEST_ADD(HVDC_TEST::test_is_blocked);
    TEST_ADD(HVDC_TEST::test_set_get_number_of_poles);
    TEST_ADD(HVDC_TEST::test_set_get_line_resistance);
    TEST_ADD(HVDC_TEST::test_set_get_line_inductance);
    TEST_ADD(HVDC_TEST::test_set_get_line_capacitance);
    TEST_ADD(HVDC_TEST::test_set_get_line_smoothing_inductance);
    TEST_ADD(HVDC_TEST::test_set_get_line_smoothing_resistance);
    TEST_ADD(HVDC_TEST::test_set_get_nominal_dc_power_per_pole);
    TEST_ADD(HVDC_TEST::test_set_get_side_to_hold_power);
    TEST_ADD(HVDC_TEST::test_set_get_nominal_dc_current);
    TEST_ADD(HVDC_TEST::test_set_get_nominal_dc_voltage);
    TEST_ADD(HVDC_TEST::test_set_get_compensating_resistance_to_hold_voltage);
    TEST_ADD(HVDC_TEST::test_set_get_dc_voltage_threshold_for_mode_switching);
    TEST_ADD(HVDC_TEST::test_set_get_dc_current_power_margin);
    TEST_ADD(HVDC_TEST::test_set_get_meter_end);
    TEST_ADD(HVDC_TEST::test_set_get_converter_operation_mode);
    TEST_ADD(HVDC_TEST::test_set_get_converter_number_of_bridge);
    TEST_ADD(HVDC_TEST::test_set_get_converter_voltage_drop_per_bridge);
    TEST_ADD(HVDC_TEST::test_set_get_converter_firing_angle_and_limit);
    TEST_ADD(HVDC_TEST::test_set_get_converter_transformer_base_voltage);
    TEST_ADD(HVDC_TEST::test_set_get_converter_transformer_impedance);
    TEST_ADD(HVDC_TEST::test_set_get_converter_transformer_admittance);
    TEST_ADD(HVDC_TEST::test_set_get_converter_transformer_tap_and_limit);
    TEST_ADD(HVDC_TEST::test_set_get_converter_transformer_number_of_taps);

    TEST_ADD(HVDC_TEST::test_get_rectifier_dc_power_command);
    TEST_ADD(HVDC_TEST::test_get_rectifier_dc_current_command);
    TEST_ADD(HVDC_TEST::test_get_inverter_dc_voltage_command);

    TEST_ADD(HVDC_TEST::test_reverse_converters);

    TEST_ADD(HVDC_TEST::test_is_valid);
    TEST_ADD(HVDC_TEST::test_clear);
    TEST_ADD(HVDC_TEST::test_is_connected_to_bus);
    TEST_ADD(HVDC_TEST::test_copy_with_operator_equal);
    TEST_ADD(HVDC_TEST::test_get_device_id);

    TEST_ADD(HVDC_TEST::test_solve_hvdc);
    TEST_ADD(HVDC_TEST::test_set_get_dc_current);
    TEST_ADD(HVDC_TEST::test_set_get_converter_dc_current);
    TEST_ADD(HVDC_TEST::test_set_get_converter_dc_voltage);
    TEST_ADD(HVDC_TEST::test_get_converter_dc_power);
    TEST_ADD(HVDC_TEST::test_get_converter_ac_active_power);
    TEST_ADD(HVDC_TEST::test_get_converter_ac_reactive_power);
    TEST_ADD(HVDC_TEST::test_get_converter_ac_apparent_power);
    TEST_ADD(HVDC_TEST::test_get_converter_ac_power_factor);
    TEST_ADD(HVDC_TEST::test_get_converter_commutating_overlap_angle);

    TEST_ADD(HVDC_TEST::test_set_get_hvdc_model);
}

void HVDC_TEST::setup()
{
    hvdc.set_toolkit(default_toolkit);

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.set_system_base_power_in_MVA(100.0);
    psdb.set_allowed_max_bus_number(100);

    BUS bus;
    bus.set_bus_number(1);
    bus.set_base_voltage_in_kV(500.0);
    bus.set_positive_sequence_voltage_in_pu(1.0);
    bus.set_positive_sequence_angle_in_rad(1.0);
    bus.set_bus_type(PQ_TYPE);
    psdb.append_bus(bus);
    bus.set_bus_number(2);
    psdb.append_bus(bus);
}

void HVDC_TEST::tear_down()
{
    hvdc.clear();

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.clear();

    show_test_end_information();
}

void HVDC_TEST::prepare_hvdc_buses()
{
    hvdc.set_converter_bus(RECTIFIER, 1);
    hvdc.set_converter_bus(INVERTER, 2);
}

void HVDC_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"HVDC_TEST");

    TEST_ASSERT(hvdc.get_converter_bus(RECTIFIER)==0);
    TEST_ASSERT(hvdc.get_converter_bus(INVERTER)==0);
    TEST_ASSERT(hvdc.get_status()==false);
    TEST_ASSERT(hvdc.get_converter_operation_mode(RECTIFIER)==RECTIFIER_CONSTANT_POWER);
    TEST_ASSERT(hvdc.get_converter_operation_mode(INVERTER)==INVERTER_CONSTANT_VOLTAGE);
    TEST_ASSERT(hvdc.get_line_resistance_in_ohm()==0.0);
    TEST_ASSERT(hvdc.get_line_inductance_in_mH()==0.0);
    TEST_ASSERT(hvdc.get_line_capacitance_in_uF()==0.0);
    TEST_ASSERT(hvdc.get_nominal_dc_power_per_pole_in_MW()==0.0);
    TEST_ASSERT(hvdc.get_nominal_dc_voltage_per_pole_in_kV()==0.0);
    TEST_ASSERT(hvdc.get_nominal_dc_current_per_pole_in_kA()==0.0);
}

void HVDC_TEST::test_set_get_converter_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"HVDC_TEST");

    hvdc.set_converter_bus(RECTIFIER, 1);
    hvdc.set_converter_bus(INVERTER, 2);

    TEST_ASSERT(hvdc.get_converter_bus(RECTIFIER)==1);
    TEST_ASSERT(hvdc.get_converter_bus(INVERTER)==2);
}

void HVDC_TEST::test_set_get_converter_valve_side_bus_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"HVDC_TEST");

    hvdc.set_converter_valve_side_bus_name(RECTIFIER, "bus1");
    hvdc.set_converter_valve_side_bus_name(INVERTER, "bus2");

    TEST_ASSERT(hvdc.get_converter_valve_side_bus_name(RECTIFIER)=="bus1");
    TEST_ASSERT(hvdc.get_converter_valve_side_bus_name(INVERTER)=="bus2");
}

void HVDC_TEST::test_set_get_identifier()
{
    show_test_information_for_function_of_class(__FUNCTION__,"HVDC_TEST");

    prepare_hvdc_buses();

    hvdc.set_identifier("P");
    TEST_ASSERT(hvdc.get_identifier()=="P");
}

void HVDC_TEST::test_set_get_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"HVDC_TEST");

    prepare_hvdc_buses();

    hvdc.set_name("DC LINK a");
    TEST_ASSERT(hvdc.get_name()=="DC LINK a");
}

void HVDC_TEST::test_set_get_status()
{
    show_test_information_for_function_of_class(__FUNCTION__,"HVDC_TEST");

    prepare_hvdc_buses();

    hvdc.set_status(false);
    TEST_ASSERT(hvdc.get_status()==false);
    hvdc.set_status(true);
    TEST_ASSERT(hvdc.get_status()==true);
}

void HVDC_TEST::test_is_blocked()
{
    show_test_information_for_function_of_class(__FUNCTION__,"HVDC_TEST");

    prepare_hvdc_buses();

    hvdc.set_status(false);
    TEST_ASSERT(hvdc.is_blocked()==true);
    hvdc.set_status(true);
    TEST_ASSERT(hvdc.is_blocked()==false);

}
void HVDC_TEST::test_set_get_number_of_poles()
{
    show_test_information_for_function_of_class(__FUNCTION__,"HVDC_TEST");

    prepare_hvdc_buses();

    hvdc.set_number_of_poles(SINGLE_POLE);
    TEST_ASSERT(hvdc.get_number_of_poles()==SINGLE_POLE);
    hvdc.set_number_of_poles(DOUBLE_POLE);
    TEST_ASSERT(hvdc.get_number_of_poles()==DOUBLE_POLE);
}

void HVDC_TEST::test_set_get_line_resistance()
{
    show_test_information_for_function_of_class(__FUNCTION__,"HVDC_TEST");

    prepare_hvdc_buses();

    hvdc.set_line_resistance_in_ohm(10.0);
    TEST_ASSERT(hvdc.get_line_resistance_in_ohm()==10.0);
    hvdc.set_line_resistance_in_ohm(-10.0);
    TEST_ASSERT(hvdc.get_line_resistance_in_ohm()==0.0);
    hvdc.set_line_resistance_in_ohm(0.0);
    TEST_ASSERT(hvdc.get_line_resistance_in_ohm()==0.0);
}

void HVDC_TEST::test_set_get_line_inductance()
{
    show_test_information_for_function_of_class(__FUNCTION__,"HVDC_TEST");

    prepare_hvdc_buses();

    hvdc.set_line_inductance_in_mH(100.0);
    TEST_ASSERT(fabs(hvdc.get_line_inductance_in_mH()-100.0)<FLOAT_EPSILON);
    hvdc.set_line_inductance_in_mH(-100.0);
    TEST_ASSERT(hvdc.get_line_inductance_in_mH()==0.0);
    hvdc.set_line_inductance_in_mH(0.0);
    TEST_ASSERT(hvdc.get_line_inductance_in_mH()==0.0);
}

void HVDC_TEST::test_set_get_line_capacitance()
{
    show_test_information_for_function_of_class(__FUNCTION__,"HVDC_TEST");

    prepare_hvdc_buses();

    hvdc.set_line_capacitance_in_uF(200.0);
    TEST_ASSERT(fabs(hvdc.get_line_capacitance_in_uF()-200.0)<FLOAT_EPSILON);
    hvdc.set_line_capacitance_in_uF(-200.0);
    TEST_ASSERT(hvdc.get_line_capacitance_in_uF()==0.0);
    hvdc.set_line_capacitance_in_uF(0.0);
    TEST_ASSERT(hvdc.get_line_capacitance_in_uF()==0.0);
}

void HVDC_TEST::test_set_get_line_smoothing_inductance()
{
    show_test_information_for_function_of_class(__FUNCTION__,"HVDC_TEST");

    prepare_hvdc_buses();

    hvdc.set_line_smooting_inductance_in_mH(RECTIFIER, 2000.0);
    TEST_ASSERT(hvdc.get_line_smooting_inductance_in_mH(RECTIFIER)==2000.0);
    hvdc.set_line_smooting_inductance_in_mH(INVERTER, -2000.0);
    TEST_ASSERT(hvdc.get_line_smooting_inductance_in_mH(INVERTER)==0.0);
    hvdc.set_line_smooting_inductance_in_mH(RECTIFIER, 0.0);
    TEST_ASSERT(hvdc.get_line_smooting_inductance_in_mH(RECTIFIER)==0.0);
}

void HVDC_TEST::test_set_get_line_smoothing_resistance()
{
    show_test_information_for_function_of_class(__FUNCTION__,"HVDC_TEST");

    prepare_hvdc_buses();

    hvdc.set_line_smooting_resistance_in_ohm(RECTIFIER, 2000.0);
    TEST_ASSERT(hvdc.get_line_smooting_resistance_in_ohm(RECTIFIER)==2000.0);
    hvdc.set_line_smooting_resistance_in_ohm(INVERTER, -2000.0);
    TEST_ASSERT(hvdc.get_line_smooting_resistance_in_ohm(INVERTER)==0.0);
    hvdc.set_line_smooting_resistance_in_ohm(RECTIFIER, 0.0);
    TEST_ASSERT(hvdc.get_line_smooting_resistance_in_ohm(RECTIFIER)==0.0);
}

void HVDC_TEST::test_set_get_nominal_dc_power_per_pole()
{
    show_test_information_for_function_of_class(__FUNCTION__,"HVDC_TEST");

    prepare_hvdc_buses();

    hvdc.set_nominal_dc_power_per_pole_in_MW(4000.0);
    TEST_ASSERT(hvdc.get_nominal_dc_power_per_pole_in_MW()==4000.0);
    hvdc.set_nominal_dc_power_per_pole_in_MW(-4000.0);
    TEST_ASSERT(hvdc.get_nominal_dc_power_per_pole_in_MW()==0.0);
    hvdc.set_nominal_dc_power_per_pole_in_MW(0.0);
    TEST_ASSERT(hvdc.get_nominal_dc_power_per_pole_in_MW()==0.0);
}

void HVDC_TEST::test_set_get_side_to_hold_power()
{
    show_test_information_for_function_of_class(__FUNCTION__,"HVDC_TEST");

    prepare_hvdc_buses();

    hvdc.set_side_to_hold_power(RECTIFIER);
    TEST_ASSERT(hvdc.get_side_to_hold_dc_power()==RECTIFIER);
    hvdc.set_side_to_hold_power(INVERTER);
    TEST_ASSERT(hvdc.get_side_to_hold_dc_power()==INVERTER);
}

void HVDC_TEST::test_set_get_nominal_dc_current()
{
    show_test_information_for_function_of_class(__FUNCTION__,"HVDC_TEST");

    prepare_hvdc_buses();

    hvdc.set_nominal_dc_current_per_pole_in_kA(5.0);
    TEST_ASSERT(hvdc.get_nominal_dc_current_per_pole_in_kA()==5.0);
    hvdc.set_nominal_dc_current_per_pole_in_kA(-5.0);
    TEST_ASSERT(hvdc.get_nominal_dc_current_per_pole_in_kA()==0.0);
    hvdc.set_nominal_dc_current_per_pole_in_kA(0.0);
    TEST_ASSERT(hvdc.get_nominal_dc_current_per_pole_in_kA()==0.0);
}

void HVDC_TEST::test_set_get_nominal_dc_voltage()
{
    show_test_information_for_function_of_class(__FUNCTION__,"HVDC_TEST");

    prepare_hvdc_buses();

    hvdc.set_nominal_dc_voltage_per_pole_in_kV(800.0);
    TEST_ASSERT(hvdc.get_nominal_dc_voltage_per_pole_in_kV()==800.0);
    hvdc.set_nominal_dc_voltage_per_pole_in_kV(-800.0);
    TEST_ASSERT(hvdc.get_nominal_dc_voltage_per_pole_in_kV()==0.0);
    hvdc.set_nominal_dc_voltage_per_pole_in_kV(0.0);
    TEST_ASSERT(hvdc.get_nominal_dc_voltage_per_pole_in_kV()==0.0);
}

void HVDC_TEST::test_set_get_compensating_resistance_to_hold_voltage()
{
    show_test_information_for_function_of_class(__FUNCTION__,"HVDC_TEST");

    prepare_hvdc_buses();

    hvdc.set_compensating_resistance_to_hold_dc_voltage_in_ohm(10.0);
    TEST_ASSERT(hvdc.get_compensating_resistance_to_hold_dc_voltage_in_ohm()==10.0);
    hvdc.set_compensating_resistance_to_hold_dc_voltage_in_ohm(-10.0);
    TEST_ASSERT(hvdc.get_compensating_resistance_to_hold_dc_voltage_in_ohm()==-10.0);
    hvdc.set_compensating_resistance_to_hold_dc_voltage_in_ohm(0.0);
    TEST_ASSERT(hvdc.get_compensating_resistance_to_hold_dc_voltage_in_ohm()==0.0);
}

void HVDC_TEST::test_set_get_dc_voltage_threshold_for_mode_switching()
{
    show_test_information_for_function_of_class(__FUNCTION__,"HVDC_TEST");

    prepare_hvdc_buses();

    hvdc.set_threshold_dc_voltage_for_constant_power_and_constant_current_mode_in_kV(700.0);
    TEST_ASSERT(hvdc.get_threshold_dc_voltage_for_constant_power_and_constant_current_mode_in_kV()==700.0);
    hvdc.set_threshold_dc_voltage_for_constant_power_and_constant_current_mode_in_kV(-700.0);
    TEST_ASSERT(hvdc.get_threshold_dc_voltage_for_constant_power_and_constant_current_mode_in_kV()==0.0);
    hvdc.set_threshold_dc_voltage_for_constant_power_and_constant_current_mode_in_kV(0.0);
    TEST_ASSERT(hvdc.get_threshold_dc_voltage_for_constant_power_and_constant_current_mode_in_kV()==0.0);
}

void HVDC_TEST::test_set_get_dc_current_power_margin()
{
    show_test_information_for_function_of_class(__FUNCTION__,"HVDC_TEST");

    prepare_hvdc_buses();

    hvdc.set_current_power_margin(0.25);
    TEST_ASSERT(hvdc.get_current_power_margin()==0.25);
    hvdc.set_current_power_margin(-0.25);
    TEST_ASSERT(hvdc.get_current_power_margin()==0.15);
    hvdc.set_current_power_margin(-0.0);
    TEST_ASSERT(hvdc.get_current_power_margin()==0.15);
}

void HVDC_TEST::test_set_get_meter_end()
{
    show_test_information_for_function_of_class(__FUNCTION__,"HVDC_TEST");

    prepare_hvdc_buses();

    TEST_ASSERT(hvdc.get_meter_end()==INVERTER);
    hvdc.set_meter_end(RECTIFIER);
    TEST_ASSERT(hvdc.get_meter_end()==RECTIFIER);
    hvdc.set_meter_end(INVERTER);
    TEST_ASSERT(hvdc.get_meter_end()==INVERTER);
}

void HVDC_TEST::test_set_get_converter_operation_mode()
{
    show_test_information_for_function_of_class(__FUNCTION__,"HVDC_TEST");

    prepare_hvdc_buses();

    hvdc.set_converter_operation_mode(RECTIFIER, RECTIFIER_CONSTANT_POWER);
    TEST_ASSERT(hvdc.get_converter_operation_mode(RECTIFIER)==RECTIFIER_CONSTANT_POWER);
    hvdc.set_converter_operation_mode(RECTIFIER, RECTIFIER_CONSTANT_CURRENT);
    TEST_ASSERT(hvdc.get_converter_operation_mode(RECTIFIER)==RECTIFIER_CONSTANT_CURRENT);

    hvdc.set_converter_operation_mode(INVERTER, INVERTER_CONSTANT_VOLTAGE);
    TEST_ASSERT(hvdc.get_converter_operation_mode(INVERTER)==INVERTER_CONSTANT_VOLTAGE);
    hvdc.set_converter_operation_mode(INVERTER, INVERTER_CONSTANT_GAMMA);
    TEST_ASSERT(hvdc.get_converter_operation_mode(INVERTER)==INVERTER_CONSTANT_GAMMA);
}

void HVDC_TEST::test_set_get_converter_number_of_bridge()
{
    show_test_information_for_function_of_class(__FUNCTION__,"HVDC_TEST");

    prepare_hvdc_buses();

    hvdc.set_converter_number_of_bridge(RECTIFIER, 2);
    TEST_ASSERT(hvdc.get_converter_number_of_bridge(RECTIFIER)==2);
    hvdc.set_converter_number_of_bridge(RECTIFIER, 0);
    TEST_ASSERT(hvdc.get_converter_number_of_bridge(RECTIFIER)==0);
    hvdc.set_converter_number_of_bridge(RECTIFIER, 1);
    TEST_ASSERT(hvdc.get_converter_number_of_bridge(RECTIFIER)==1);

    hvdc.set_converter_number_of_bridge(INVERTER, 2);
    TEST_ASSERT(hvdc.get_converter_number_of_bridge(INVERTER)==2);
    hvdc.set_converter_number_of_bridge(INVERTER, 0);
    TEST_ASSERT(hvdc.get_converter_number_of_bridge(INVERTER)==0);
    hvdc.set_converter_number_of_bridge(INVERTER, 1);
    TEST_ASSERT(hvdc.get_converter_number_of_bridge(INVERTER)==1);
}

void HVDC_TEST::test_set_get_converter_voltage_drop_per_bridge()
{
    show_test_information_for_function_of_class(__FUNCTION__,"HVDC_TEST");

    prepare_hvdc_buses();

    hvdc.set_converter_voltage_drop_per_bridge_in_kV(RECTIFIER, 100.0);
    TEST_ASSERT(hvdc.get_converter_voltage_drop_per_bridge_in_kV(RECTIFIER)==100.0);
    hvdc.set_converter_voltage_drop_per_bridge_in_kV(RECTIFIER, -100.0);
    TEST_ASSERT(hvdc.get_converter_voltage_drop_per_bridge_in_kV(RECTIFIER)==0.0);
    hvdc.set_converter_voltage_drop_per_bridge_in_kV(RECTIFIER, 0.0);
    TEST_ASSERT(hvdc.get_converter_voltage_drop_per_bridge_in_kV(RECTIFIER)==0.0);

    hvdc.set_converter_voltage_drop_per_bridge_in_kV(INVERTER, 100.0);
    TEST_ASSERT(hvdc.get_converter_voltage_drop_per_bridge_in_kV(INVERTER)==100.0);
    hvdc.set_converter_voltage_drop_per_bridge_in_kV(INVERTER, -100.0);
    TEST_ASSERT(hvdc.get_converter_voltage_drop_per_bridge_in_kV(INVERTER)==0.0);
    hvdc.set_converter_voltage_drop_per_bridge_in_kV(INVERTER, 0.0);
    TEST_ASSERT(hvdc.get_converter_voltage_drop_per_bridge_in_kV(INVERTER)==0.0);
}

void HVDC_TEST::test_set_get_converter_firing_angle_and_limit()
{
    show_test_information_for_function_of_class(__FUNCTION__,"HVDC_TEST");

    prepare_hvdc_buses();

    hvdc.set_converter_alpha_or_gamma_in_deg(RECTIFIER, 10.0);
    TEST_ASSERT(hvdc.get_converter_alpha_or_gamma_in_deg(RECTIFIER)==10.0);
    hvdc.set_converter_alpha_or_gamma_in_deg(RECTIFIER, -10.0);
    TEST_ASSERT(hvdc.get_converter_alpha_or_gamma_in_deg(RECTIFIER)==0.0);
    hvdc.set_converter_alpha_or_gamma_in_deg(RECTIFIER, 0.0);
    TEST_ASSERT(hvdc.get_converter_alpha_or_gamma_in_deg(RECTIFIER)==0.0);

    hvdc.set_converter_alpha_or_gamma_in_deg(INVERTER, 10.0);
    TEST_ASSERT(hvdc.get_converter_alpha_or_gamma_in_deg(INVERTER)==10.0);
    hvdc.set_converter_alpha_or_gamma_in_deg(INVERTER, -10.0);
    TEST_ASSERT(hvdc.get_converter_alpha_or_gamma_in_deg(INVERTER)==0.0);
    hvdc.set_converter_alpha_or_gamma_in_deg(INVERTER, 0.0);
    TEST_ASSERT(hvdc.get_converter_alpha_or_gamma_in_deg(INVERTER)==0.0);

    hvdc.set_converter_max_alpha_or_gamma_in_deg(RECTIFIER, 90.0);
    TEST_ASSERT(hvdc.get_converter_max_alpha_or_gamma_in_deg(RECTIFIER)==90.0);
    hvdc.set_converter_max_alpha_or_gamma_in_deg(RECTIFIER, -90.0);
    TEST_ASSERT(hvdc.get_converter_max_alpha_or_gamma_in_deg(RECTIFIER)==0.0);
    hvdc.set_converter_max_alpha_or_gamma_in_deg(RECTIFIER, 0.0);
    TEST_ASSERT(hvdc.get_converter_max_alpha_or_gamma_in_deg(RECTIFIER)==0.0);

    hvdc.set_converter_max_alpha_or_gamma_in_deg(INVERTER, 90.0);
    TEST_ASSERT(hvdc.get_converter_max_alpha_or_gamma_in_deg(INVERTER)==90.0);
    hvdc.set_converter_max_alpha_or_gamma_in_deg(INVERTER, -90.0);
    TEST_ASSERT(hvdc.get_converter_max_alpha_or_gamma_in_deg(INVERTER)==0.0);
    hvdc.set_converter_max_alpha_or_gamma_in_deg(INVERTER, 0.0);
    TEST_ASSERT(hvdc.get_converter_max_alpha_or_gamma_in_deg(INVERTER)==0.0);

    hvdc.set_converter_min_alpha_or_gamma_in_deg(RECTIFIER, 5.0);
    TEST_ASSERT(hvdc.get_converter_min_alpha_or_gamma_in_deg(RECTIFIER)==5.0);
    hvdc.set_converter_min_alpha_or_gamma_in_deg(RECTIFIER, -5.0);
    TEST_ASSERT(hvdc.get_converter_min_alpha_or_gamma_in_deg(RECTIFIER)==0.0);
    hvdc.set_converter_min_alpha_or_gamma_in_deg(RECTIFIER, 0.0);
    TEST_ASSERT(hvdc.get_converter_min_alpha_or_gamma_in_deg(RECTIFIER)==0.0);

    hvdc.set_converter_min_alpha_or_gamma_in_deg(INVERTER, 7.0);
    TEST_ASSERT(hvdc.get_converter_min_alpha_or_gamma_in_deg(INVERTER)==7.0);
    hvdc.set_converter_min_alpha_or_gamma_in_deg(INVERTER, -7.0);
    TEST_ASSERT(hvdc.get_converter_min_alpha_or_gamma_in_deg(INVERTER)==0.0);
    hvdc.set_converter_min_alpha_or_gamma_in_deg(INVERTER, 0.0);
    TEST_ASSERT(hvdc.get_converter_min_alpha_or_gamma_in_deg(INVERTER)==0.0);
}

void HVDC_TEST::test_set_get_converter_transformer_base_voltage()
{
    show_test_information_for_function_of_class(__FUNCTION__,"HVDC_TEST");

    prepare_hvdc_buses();

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    size_t rec_bus = hvdc.get_converter_bus(RECTIFIER);
    hvdc.set_converter_transformer_grid_side_base_voltage_in_kV(RECTIFIER, 500.0);
    TEST_ASSERT(hvdc.get_converter_transformer_grid_side_base_voltage_in_kV(RECTIFIER)==500.0);
    hvdc.set_converter_transformer_grid_side_base_voltage_in_kV(RECTIFIER, -500.0);
    TEST_ASSERT(hvdc.get_converter_transformer_grid_side_base_voltage_in_kV(RECTIFIER)==psdb.get_bus_base_voltage_in_kV(rec_bus));
    hvdc.set_converter_transformer_grid_side_base_voltage_in_kV(RECTIFIER, 0.0);
    TEST_ASSERT(hvdc.get_converter_transformer_grid_side_base_voltage_in_kV(RECTIFIER)==psdb.get_bus_base_voltage_in_kV(rec_bus));

    hvdc.set_converter_transformer_converter_side_base_voltage_in_kV(RECTIFIER, 300.0);
    TEST_ASSERT(hvdc.get_converter_transformer_converter_side_base_voltage_in_kV(RECTIFIER)==300.0);
    hvdc.set_converter_transformer_converter_side_base_voltage_in_kV(RECTIFIER, -300.0);
    TEST_ASSERT(hvdc.get_converter_transformer_converter_side_base_voltage_in_kV(RECTIFIER)==0.0);
    hvdc.set_converter_transformer_converter_side_base_voltage_in_kV(RECTIFIER, 0.0);
    TEST_ASSERT(hvdc.get_converter_transformer_converter_side_base_voltage_in_kV(RECTIFIER)==0.0);

    size_t inv_bus = hvdc.get_converter_bus(INVERTER);
    hvdc.set_converter_transformer_grid_side_base_voltage_in_kV(INVERTER, 500.0);
    TEST_ASSERT(hvdc.get_converter_transformer_grid_side_base_voltage_in_kV(INVERTER)==500.0);
    hvdc.set_converter_transformer_grid_side_base_voltage_in_kV(INVERTER, -500.0);
    TEST_ASSERT(hvdc.get_converter_transformer_grid_side_base_voltage_in_kV(INVERTER)==psdb.get_bus_base_voltage_in_kV(inv_bus));
    hvdc.set_converter_transformer_grid_side_base_voltage_in_kV(INVERTER, 0.0);
    TEST_ASSERT(hvdc.get_converter_transformer_grid_side_base_voltage_in_kV(INVERTER)==psdb.get_bus_base_voltage_in_kV(inv_bus));

    hvdc.set_converter_transformer_converter_side_base_voltage_in_kV(INVERTER, 300.0);
    TEST_ASSERT(hvdc.get_converter_transformer_converter_side_base_voltage_in_kV(INVERTER)==300.0);
    hvdc.set_converter_transformer_converter_side_base_voltage_in_kV(INVERTER, -300.0);
    TEST_ASSERT(hvdc.get_converter_transformer_converter_side_base_voltage_in_kV(INVERTER)==0.0);
    hvdc.set_converter_transformer_converter_side_base_voltage_in_kV(INVERTER, 0.0);
    TEST_ASSERT(hvdc.get_converter_transformer_converter_side_base_voltage_in_kV(INVERTER)==0.0);
}

void HVDC_TEST::test_set_get_converter_transformer_impedance()
{
    show_test_information_for_function_of_class(__FUNCTION__,"HVDC_TEST");

    prepare_hvdc_buses();

    complex<double>  z(2.0, 10.0);
    hvdc.set_converter_transformer_impedance_in_ohm(RECTIFIER, z);
    TEST_ASSERT(hvdc.get_converter_transformer_impedance_in_ohm(RECTIFIER)==z);
    hvdc.set_converter_transformer_impedance_in_ohm(RECTIFIER, 0.0);
    TEST_ASSERT(hvdc.get_converter_transformer_impedance_in_ohm(RECTIFIER)==0.0);

    hvdc.set_converter_transformer_impedance_in_ohm(INVERTER, z);
    TEST_ASSERT(hvdc.get_converter_transformer_impedance_in_ohm(INVERTER)==z);
    hvdc.set_converter_transformer_impedance_in_ohm(INVERTER, 0.0);
    TEST_ASSERT(hvdc.get_converter_transformer_impedance_in_ohm(INVERTER)==0.0);
}

void HVDC_TEST::test_set_get_converter_transformer_admittance()
{
    show_test_information_for_function_of_class(__FUNCTION__,"HVDC_TEST");

    prepare_hvdc_buses();

    complex<double>  y(0.0, 10.0);
    hvdc.set_converter_transformer_admittance_in_siemens(RECTIFIER, y);
    TEST_ASSERT(hvdc.get_converter_transformer_admittance_in_siemens(RECTIFIER)==y);
    hvdc.set_converter_transformer_admittance_in_siemens(RECTIFIER, 0.0);
    TEST_ASSERT(hvdc.get_converter_transformer_admittance_in_siemens(RECTIFIER)==0.0);

    hvdc.set_converter_transformer_admittance_in_siemens(INVERTER, y);
    TEST_ASSERT(hvdc.get_converter_transformer_admittance_in_siemens(INVERTER)==y);
    hvdc.set_converter_transformer_admittance_in_siemens(INVERTER, 0.0);
    TEST_ASSERT(hvdc.get_converter_transformer_admittance_in_siemens(INVERTER)==0.0);
}

void HVDC_TEST::test_set_get_converter_transformer_tap_and_limit()
{
    show_test_information_for_function_of_class(__FUNCTION__,"HVDC_TEST");

    prepare_hvdc_buses();

    hvdc.set_converter_transformer_tap_in_pu(RECTIFIER, 1.5);
    TEST_ASSERT(hvdc.get_converter_transformer_tap_in_pu(RECTIFIER)==1.5);
    hvdc.set_converter_transformer_tap_in_pu(RECTIFIER, -1.5);
    TEST_ASSERT(hvdc.get_converter_transformer_tap_in_pu(RECTIFIER)==1.0);
    hvdc.set_converter_transformer_tap_in_pu(RECTIFIER, 0.0);
    TEST_ASSERT(hvdc.get_converter_transformer_tap_in_pu(RECTIFIER)==1.0);

    hvdc.set_converter_transformer_tap_in_pu(INVERTER, 1.5);
    TEST_ASSERT(hvdc.get_converter_transformer_tap_in_pu(INVERTER)==1.5);
    hvdc.set_converter_transformer_tap_in_pu(INVERTER, -1.5);
    TEST_ASSERT(hvdc.get_converter_transformer_tap_in_pu(INVERTER)==1.0);
    hvdc.set_converter_transformer_tap_in_pu(INVERTER, 0.0);
    TEST_ASSERT(hvdc.get_converter_transformer_tap_in_pu(INVERTER)==1.0);

    hvdc.set_converter_transformer_max_tap_in_pu(RECTIFIER, 1.5);
    TEST_ASSERT(hvdc.get_converter_transformer_max_tap_in_pu(RECTIFIER)==1.5);
    hvdc.set_converter_transformer_max_tap_in_pu(RECTIFIER, -1.5);
    TEST_ASSERT(hvdc.get_converter_transformer_max_tap_in_pu(RECTIFIER)==1.0);
    hvdc.set_converter_transformer_max_tap_in_pu(RECTIFIER, 0.0);
    TEST_ASSERT(hvdc.get_converter_transformer_max_tap_in_pu(RECTIFIER)==1.0);

    hvdc.set_converter_transformer_min_tap_in_pu(RECTIFIER, 0.8);
    TEST_ASSERT(hvdc.get_converter_transformer_min_tap_in_pu(RECTIFIER)==0.8);
    hvdc.set_converter_transformer_min_tap_in_pu(RECTIFIER, -0.8);
    TEST_ASSERT(hvdc.get_converter_transformer_min_tap_in_pu(RECTIFIER)==1.0);
    hvdc.set_converter_transformer_min_tap_in_pu(RECTIFIER, 0.0);
    TEST_ASSERT(hvdc.get_converter_transformer_min_tap_in_pu(RECTIFIER)==1.0);

    hvdc.set_converter_transformer_max_tap_in_pu(INVERTER, 1.5);
    TEST_ASSERT(hvdc.get_converter_transformer_max_tap_in_pu(INVERTER)==1.5);
    hvdc.set_converter_transformer_max_tap_in_pu(INVERTER, -1.5);
    TEST_ASSERT(hvdc.get_converter_transformer_max_tap_in_pu(INVERTER)==1.0);
    hvdc.set_converter_transformer_max_tap_in_pu(INVERTER, 0.0);
    TEST_ASSERT(hvdc.get_converter_transformer_max_tap_in_pu(INVERTER)==1.0);

    hvdc.set_converter_transformer_min_tap_in_pu(INVERTER, 0.8);
    TEST_ASSERT(hvdc.get_converter_transformer_min_tap_in_pu(INVERTER)==0.8);
    hvdc.set_converter_transformer_min_tap_in_pu(INVERTER, -0.8);
    TEST_ASSERT(hvdc.get_converter_transformer_min_tap_in_pu(INVERTER)==1.0);
    hvdc.set_converter_transformer_min_tap_in_pu(INVERTER, 0.0);
    TEST_ASSERT(hvdc.get_converter_transformer_min_tap_in_pu(INVERTER)==1.0);
}

void HVDC_TEST::test_set_get_converter_transformer_number_of_taps()
{
    show_test_information_for_function_of_class(__FUNCTION__,"HVDC_TEST");

    prepare_hvdc_buses();

    hvdc.set_converter_transformer_number_of_taps(RECTIFIER, 33);
    TEST_ASSERT(hvdc.get_converter_transformer_number_of_taps(RECTIFIER)==33);
    hvdc.set_converter_transformer_number_of_taps(RECTIFIER, 0);
    TEST_ASSERT(hvdc.get_converter_transformer_number_of_taps(RECTIFIER)==1);

    hvdc.set_converter_transformer_number_of_taps(INVERTER, 33);
    TEST_ASSERT(hvdc.get_converter_transformer_number_of_taps(INVERTER)==33);
    hvdc.set_converter_transformer_number_of_taps(INVERTER, 0);
    TEST_ASSERT(hvdc.get_converter_transformer_number_of_taps(INVERTER)==1);
}


void HVDC_TEST::test_get_rectifier_dc_power_command()
{
    show_test_information_for_function_of_class(__FUNCTION__,"HVDC_TEST");

    prepare_hvdc_buses();

    hvdc.set_converter_bus(RECTIFIER, 1);
    hvdc.set_converter_bus(INVERTER, 2);
    hvdc.set_line_resistance_in_ohm(10.0);

    hvdc.set_converter_operation_mode(RECTIFIER, RECTIFIER_CONSTANT_POWER);
    hvdc.set_side_to_hold_power(RECTIFIER);
    hvdc.set_nominal_dc_power_per_pole_in_MW(1000.0);

    hvdc.set_converter_operation_mode(INVERTER, INVERTER_CONSTANT_VOLTAGE);
    hvdc.set_nominal_dc_voltage_per_pole_in_kV(500.0);
    hvdc.set_compensating_resistance_to_hold_dc_voltage_in_ohm(0.0);

    TEST_ASSERT(fabs(hvdc.get_rectifier_nominal_dc_power_command_in_MW()-1000.0)<FLOAT_EPSILON);

    hvdc.set_converter_operation_mode(RECTIFIER, RECTIFIER_CONSTANT_POWER);
    hvdc.set_side_to_hold_power(RECTIFIER);
    hvdc.set_nominal_dc_power_per_pole_in_MW(1000.0);

    hvdc.set_converter_operation_mode(INVERTER, INVERTER_CONSTANT_VOLTAGE);
    hvdc.set_nominal_dc_voltage_per_pole_in_kV(500.0);
    hvdc.set_compensating_resistance_to_hold_dc_voltage_in_ohm(10.0);

    TEST_ASSERT(fabs(hvdc.get_rectifier_nominal_dc_power_command_in_MW()-1000.0)<FLOAT_EPSILON);

    hvdc.set_converter_operation_mode(RECTIFIER, RECTIFIER_CONSTANT_POWER);
    hvdc.set_side_to_hold_power(RECTIFIER);
    hvdc.set_nominal_dc_power_per_pole_in_MW(1000.0);

    hvdc.set_converter_operation_mode(INVERTER, INVERTER_CONSTANT_VOLTAGE);
    hvdc.set_nominal_dc_voltage_per_pole_in_kV(500.0);
    hvdc.set_compensating_resistance_to_hold_dc_voltage_in_ohm(5.0);

    TEST_ASSERT(fabs(hvdc.get_rectifier_nominal_dc_power_command_in_MW()-1000.0)<FLOAT_EPSILON);

    hvdc.set_converter_operation_mode(RECTIFIER, RECTIFIER_CONSTANT_POWER);
    hvdc.set_side_to_hold_power(INVERTER);
    hvdc.set_nominal_dc_power_per_pole_in_MW(1000.0);

    hvdc.set_converter_operation_mode(INVERTER, INVERTER_CONSTANT_VOLTAGE);
    hvdc.set_nominal_dc_voltage_per_pole_in_kV(500.0);
    hvdc.set_compensating_resistance_to_hold_dc_voltage_in_ohm(0.0);

    // vdci = 500, Idc = 2.0, Vdcr = 520, Pdcr = 1040
    TEST_ASSERT(fabs(hvdc.get_rectifier_nominal_dc_power_command_in_MW()-1040.0)<FLOAT_EPSILON);

    hvdc.set_converter_operation_mode(RECTIFIER, RECTIFIER_CONSTANT_POWER);
    hvdc.set_side_to_hold_power(INVERTER);
    hvdc.set_nominal_dc_power_per_pole_in_MW(1000.0);

    hvdc.set_converter_operation_mode(INVERTER, INVERTER_CONSTANT_VOLTAGE);
    hvdc.set_nominal_dc_voltage_per_pole_in_kV(500.0);
    hvdc.set_compensating_resistance_to_hold_dc_voltage_in_ohm(10.0);

    // vdcr = 500,  (500-10*Idc)*Idc = 1000, 10*Idc2-500*Idc+1000=0, Idc2-50*Idc+100=0, Idc = (50-sqrt(50*50-400))/2=2.08712152522079996705976403136
    // pdcr = Idc*500=1043.56076261039998352988201568
    TEST_ASSERT(fabs(hvdc.get_rectifier_nominal_dc_power_command_in_MW()-1043.56076261039998352988201568)<FLOAT_EPSILON);

    hvdc.set_converter_operation_mode(RECTIFIER, RECTIFIER_CONSTANT_POWER);
    hvdc.set_side_to_hold_power(INVERTER);
    hvdc.set_nominal_dc_power_per_pole_in_MW(1000.0);

    hvdc.set_converter_operation_mode(INVERTER, INVERTER_CONSTANT_VOLTAGE);
    hvdc.set_nominal_dc_voltage_per_pole_in_kV(500.0);
    hvdc.set_compensating_resistance_to_hold_dc_voltage_in_ohm(5.0);

    // vdci = 500-5.0*Idc, (500-5*Idc)*Idc = 1000, 5*Idc2-500*Idc+1000=0, Idc2-100*Idc+200=0,
    // Idc = (100-sqrt(100*100-800))/2=2.0416847668728045840256193583731
    // vdcr = 500+5*Idc=510.20842383436402292012809679187
    // pdcr = Idc*Vdcr=1041.6847668728045840256193583731
    TEST_ASSERT(fabs(hvdc.get_rectifier_nominal_dc_power_command_in_MW()-1041.6847668728045840256193583731)<FLOAT_EPSILON);

    // constant current mode
    hvdc.set_converter_operation_mode(RECTIFIER, RECTIFIER_CONSTANT_CURRENT);
    hvdc.set_side_to_hold_power(RECTIFIER);
    hvdc.set_nominal_dc_current_per_pole_in_kA(2.0);

    hvdc.set_converter_operation_mode(INVERTER, INVERTER_CONSTANT_VOLTAGE);
    hvdc.set_nominal_dc_voltage_per_pole_in_kV(500.0);
    hvdc.set_compensating_resistance_to_hold_dc_voltage_in_ohm(0.0);
    // vdci = 500; vdcr = 520; Pdcr = 1040
    TEST_ASSERT(fabs(hvdc.get_rectifier_nominal_dc_power_command_in_MW()-1040.0)<FLOAT_EPSILON);

    hvdc.set_converter_operation_mode(RECTIFIER, RECTIFIER_CONSTANT_CURRENT);
    hvdc.set_side_to_hold_power(RECTIFIER);
    hvdc.set_nominal_dc_current_per_pole_in_kA(2.0);

    hvdc.set_converter_operation_mode(INVERTER, INVERTER_CONSTANT_VOLTAGE);
    hvdc.set_nominal_dc_voltage_per_pole_in_kV(500.0);
    hvdc.set_compensating_resistance_to_hold_dc_voltage_in_ohm(10.0);
    // vdcr = 500, pdcr = 1000
    TEST_ASSERT(fabs(hvdc.get_rectifier_nominal_dc_power_command_in_MW()-1000.0)<FLOAT_EPSILON);

    hvdc.set_converter_operation_mode(RECTIFIER, RECTIFIER_CONSTANT_CURRENT);
    hvdc.set_side_to_hold_power(RECTIFIER);
    hvdc.set_nominal_dc_current_per_pole_in_kA(2.0);

    hvdc.set_converter_operation_mode(INVERTER, INVERTER_CONSTANT_VOLTAGE);
    hvdc.set_nominal_dc_voltage_per_pole_in_kV(500.0);
    hvdc.set_compensating_resistance_to_hold_dc_voltage_in_ohm(5.0);
    // vdcr = 500+5*2=510, pdcr = 1020
    TEST_ASSERT(fabs(hvdc.get_rectifier_nominal_dc_power_command_in_MW()-1020.0)<FLOAT_EPSILON);
}

void HVDC_TEST::test_get_rectifier_dc_current_command()
{
    show_test_information_for_function_of_class(__FUNCTION__,"HVDC_TEST");

    prepare_hvdc_buses();

    hvdc.set_converter_bus(RECTIFIER, 1);
    hvdc.set_converter_bus(INVERTER, 2);
    hvdc.set_line_resistance_in_ohm(10.0);

    hvdc.set_converter_operation_mode(RECTIFIER, RECTIFIER_CONSTANT_POWER);
    hvdc.set_side_to_hold_power(RECTIFIER);
    hvdc.set_nominal_dc_power_per_pole_in_MW(1000.0);

    hvdc.set_converter_operation_mode(INVERTER, INVERTER_CONSTANT_VOLTAGE);
    hvdc.set_nominal_dc_voltage_per_pole_in_kV(500.0);
    hvdc.set_compensating_resistance_to_hold_dc_voltage_in_ohm(0.0);
    // (500+10*Idc)*Idc=1000, 10*Idc2+500*Idc-1000=0, Idc2+50*Idc-100=0, Idc= (-50+sqrt(50*50+400))/2=1.9258240356725201562535524577016
    TEST_ASSERT(fabs(hvdc.get_rectifier_nominal_dc_current_command_in_kA()-1.9258240356725201562535524577016)<FLOAT_EPSILON);

    hvdc.set_converter_operation_mode(RECTIFIER, RECTIFIER_CONSTANT_POWER);
    hvdc.set_side_to_hold_power(RECTIFIER);
    hvdc.set_nominal_dc_power_per_pole_in_MW(1000.0);

    hvdc.set_converter_operation_mode(INVERTER, INVERTER_CONSTANT_VOLTAGE);
    hvdc.set_nominal_dc_voltage_per_pole_in_kV(500.0);
    hvdc.set_compensating_resistance_to_hold_dc_voltage_in_ohm(10.0);

    TEST_ASSERT(fabs(hvdc.get_rectifier_nominal_dc_current_command_in_kA()-2.0)<FLOAT_EPSILON);

    hvdc.set_converter_operation_mode(RECTIFIER, RECTIFIER_CONSTANT_POWER);
    hvdc.set_side_to_hold_power(RECTIFIER);
    hvdc.set_nominal_dc_power_per_pole_in_MW(1000.0);

    hvdc.set_converter_operation_mode(INVERTER, INVERTER_CONSTANT_VOLTAGE);
    hvdc.set_nominal_dc_voltage_per_pole_in_kV(500.0);
    hvdc.set_compensating_resistance_to_hold_dc_voltage_in_ohm(5.0);
    // (500+5*Idc)*Idc = 1000,5*Idc2+500*Idc-1000=0, Idc2+100*Idc-200=0, Idc = (-100+sqrt(100*100+800))/2=1.9615242270663188058233902451762
    TEST_ASSERT(fabs(hvdc.get_rectifier_nominal_dc_current_command_in_kA()-1.9615242270663188058233902451762)<FLOAT_EPSILON);

    hvdc.set_converter_operation_mode(RECTIFIER, RECTIFIER_CONSTANT_POWER);
    hvdc.set_side_to_hold_power(INVERTER);
    hvdc.set_nominal_dc_power_per_pole_in_MW(1000.0);

    hvdc.set_converter_operation_mode(INVERTER, INVERTER_CONSTANT_VOLTAGE);
    hvdc.set_nominal_dc_voltage_per_pole_in_kV(500.0);
    hvdc.set_compensating_resistance_to_hold_dc_voltage_in_ohm(0.0);

    TEST_ASSERT(fabs(hvdc.get_rectifier_nominal_dc_current_command_in_kA()-2.0)<FLOAT_EPSILON);

    hvdc.set_converter_operation_mode(RECTIFIER, RECTIFIER_CONSTANT_POWER);
    hvdc.set_side_to_hold_power(INVERTER);
    hvdc.set_nominal_dc_power_per_pole_in_MW(1000.0);

    hvdc.set_converter_operation_mode(INVERTER, INVERTER_CONSTANT_VOLTAGE);
    hvdc.set_nominal_dc_voltage_per_pole_in_kV(500.0);
    hvdc.set_compensating_resistance_to_hold_dc_voltage_in_ohm(10.0);
    // (500-10*Idc)*Idc=1000, 10*Idc2-500*Idc+1000=0, Idc2-50*Idc+100=0, Idc = (50-sqrt(50*50-400))/2=2.08712152522079996705976403136
    TEST_ASSERT(fabs(hvdc.get_rectifier_nominal_dc_current_command_in_kA()-2.08712152522079996705976403136)<FLOAT_EPSILON);

    hvdc.set_converter_operation_mode(RECTIFIER, RECTIFIER_CONSTANT_POWER);
    hvdc.set_side_to_hold_power(INVERTER);
    hvdc.set_nominal_dc_power_per_pole_in_MW(1000.0);

    hvdc.set_converter_operation_mode(INVERTER, INVERTER_CONSTANT_VOLTAGE);
    hvdc.set_nominal_dc_voltage_per_pole_in_kV(500.0);
    hvdc.set_compensating_resistance_to_hold_dc_voltage_in_ohm(5.0);
    // (500-5*Idc)*Idc=1000, 5*Idc2-500*Idc+1000=0, Idc2-100Idc+200=0, Idc = (100-sqrt(100*100-800))/2=2.0416847668728045840256193583731
    TEST_ASSERT(fabs(hvdc.get_rectifier_nominal_dc_current_command_in_kA()-2.0416847668728045840256193583731)<FLOAT_EPSILON);

    // constant current mode
    hvdc.set_converter_operation_mode(RECTIFIER, RECTIFIER_CONSTANT_CURRENT);
    hvdc.set_side_to_hold_power(RECTIFIER);
    hvdc.set_nominal_dc_current_per_pole_in_kA(2.0);

    hvdc.set_converter_operation_mode(INVERTER, INVERTER_CONSTANT_VOLTAGE);
    hvdc.set_nominal_dc_voltage_per_pole_in_kV(500.0);
    hvdc.set_compensating_resistance_to_hold_dc_voltage_in_ohm(0.0);

    TEST_ASSERT(fabs(hvdc.get_rectifier_nominal_dc_current_command_in_kA()-2.0)<FLOAT_EPSILON);

    hvdc.set_converter_operation_mode(RECTIFIER, RECTIFIER_CONSTANT_CURRENT);
    hvdc.set_side_to_hold_power(RECTIFIER);
    hvdc.set_nominal_dc_current_per_pole_in_kA(2.0);

    hvdc.set_converter_operation_mode(INVERTER, INVERTER_CONSTANT_VOLTAGE);
    hvdc.set_nominal_dc_voltage_per_pole_in_kV(500.0);
    hvdc.set_compensating_resistance_to_hold_dc_voltage_in_ohm(10.0);

    TEST_ASSERT(fabs(hvdc.get_rectifier_nominal_dc_current_command_in_kA()-2.0)<FLOAT_EPSILON);

    hvdc.set_converter_operation_mode(RECTIFIER, RECTIFIER_CONSTANT_CURRENT);
    hvdc.set_side_to_hold_power(RECTIFIER);
    hvdc.set_nominal_dc_current_per_pole_in_kA(2.0);

    hvdc.set_converter_operation_mode(INVERTER, INVERTER_CONSTANT_VOLTAGE);
    hvdc.set_nominal_dc_voltage_per_pole_in_kV(500.0);
    hvdc.set_compensating_resistance_to_hold_dc_voltage_in_ohm(5.0);

    TEST_ASSERT(fabs(hvdc.get_rectifier_nominal_dc_current_command_in_kA()-2.0)<FLOAT_EPSILON);
}

void HVDC_TEST::test_get_inverter_dc_voltage_command()
{
    show_test_information_for_function_of_class(__FUNCTION__,"HVDC_TEST");

    prepare_hvdc_buses();

    hvdc.set_converter_bus(RECTIFIER, 1);
    hvdc.set_converter_bus(INVERTER, 2);
    hvdc.set_line_resistance_in_ohm(10.0);

    hvdc.set_converter_operation_mode(RECTIFIER, RECTIFIER_CONSTANT_POWER);
    hvdc.set_side_to_hold_power(RECTIFIER);
    hvdc.set_nominal_dc_power_per_pole_in_MW(1000.0);

    hvdc.set_converter_operation_mode(INVERTER, INVERTER_CONSTANT_VOLTAGE);
    hvdc.set_nominal_dc_voltage_per_pole_in_kV(500.0);
    hvdc.set_compensating_resistance_to_hold_dc_voltage_in_ohm(0.0);

    TEST_ASSERT(fabs(hvdc.get_inverter_nominal_dc_voltage_command_in_kV()-500.0)<FLOAT_EPSILON);

    hvdc.set_converter_operation_mode(RECTIFIER, RECTIFIER_CONSTANT_POWER);
    hvdc.set_side_to_hold_power(RECTIFIER);
    hvdc.set_nominal_dc_power_per_pole_in_MW(1000.0);

    hvdc.set_converter_operation_mode(INVERTER, INVERTER_CONSTANT_VOLTAGE);
    hvdc.set_nominal_dc_voltage_per_pole_in_kV(500.0);
    hvdc.set_compensating_resistance_to_hold_dc_voltage_in_ohm(10.0);
    // idc=2, vdci = 500-2*10=480
    TEST_ASSERT(fabs(hvdc.get_inverter_nominal_dc_voltage_command_in_kV()-480.0)<FLOAT_EPSILON);

    hvdc.set_converter_operation_mode(RECTIFIER, RECTIFIER_CONSTANT_POWER);
    hvdc.set_side_to_hold_power(RECTIFIER);
    hvdc.set_nominal_dc_power_per_pole_in_MW(1000.0);

    hvdc.set_converter_operation_mode(INVERTER, INVERTER_CONSTANT_VOLTAGE);
    hvdc.set_nominal_dc_voltage_per_pole_in_kV(500.0);
    hvdc.set_compensating_resistance_to_hold_dc_voltage_in_ohm(5.0);
    //(500+5*Idc)*Idc=1000, 5Idc2+500Idc-1000=0, Idc2+100Idc-200=0, Idc=(-100+sqrt(100*100+800))/2=1.9615242270663188058233902451762
    // vdci = 500-5*idc=490.19237886466840597088304877412
    TEST_ASSERT(fabs(hvdc.get_inverter_nominal_dc_voltage_command_in_kV()-490.19237886466840597088304877412)<FLOAT_EPSILON);

    hvdc.set_converter_operation_mode(RECTIFIER, RECTIFIER_CONSTANT_POWER);
    hvdc.set_side_to_hold_power(INVERTER);
    hvdc.set_nominal_dc_power_per_pole_in_MW(1000.0);

    hvdc.set_converter_operation_mode(INVERTER, INVERTER_CONSTANT_VOLTAGE);
    hvdc.set_nominal_dc_voltage_per_pole_in_kV(500.0);
    hvdc.set_compensating_resistance_to_hold_dc_voltage_in_ohm(0.0);

    TEST_ASSERT(fabs(hvdc.get_inverter_nominal_dc_voltage_command_in_kV()-500.0)<FLOAT_EPSILON);

    hvdc.set_converter_operation_mode(RECTIFIER, RECTIFIER_CONSTANT_POWER);
    hvdc.set_side_to_hold_power(INVERTER);
    hvdc.set_nominal_dc_power_per_pole_in_MW(1000.0);

    hvdc.set_converter_operation_mode(INVERTER, INVERTER_CONSTANT_VOLTAGE);
    hvdc.set_nominal_dc_voltage_per_pole_in_kV(500.0);
    hvdc.set_compensating_resistance_to_hold_dc_voltage_in_ohm(10.0);
    //(500-10Idc)Idc=1000, 10Idc2-500Idc+1000=0, Idc2-50Idc+100=0, Idc=(50-sqrt(50*50-400))/2=2.08712152522079996705976403136
    // vdci = 500-10Idc=479.1287847477920003294023596864
    TEST_ASSERT(fabs(hvdc.get_inverter_nominal_dc_voltage_command_in_kV()-479.1287847477920003294023596864)<FLOAT_EPSILON);

    hvdc.set_converter_operation_mode(RECTIFIER, RECTIFIER_CONSTANT_POWER);
    hvdc.set_side_to_hold_power(INVERTER);
    hvdc.set_nominal_dc_power_per_pole_in_MW(1000.0);

    hvdc.set_converter_operation_mode(INVERTER, INVERTER_CONSTANT_VOLTAGE);
    hvdc.set_nominal_dc_voltage_per_pole_in_kV(500.0);
    hvdc.set_compensating_resistance_to_hold_dc_voltage_in_ohm(5.0);
    // (500-5*Idc)*Idc=1000, 5*Idc2-500*Idc+1000=0, Idc2-100Idc+200=0, Idc = (100-sqrt(100*100-800))/2=2.0416847668728045840256193583731
    // vdci = 500-5*Idc
    TEST_ASSERT(fabs(hvdc.get_inverter_nominal_dc_voltage_command_in_kV()-489.79157616563597707987190320813)<FLOAT_EPSILON);

    // constant current mode
    hvdc.set_converter_operation_mode(RECTIFIER, RECTIFIER_CONSTANT_CURRENT);
    hvdc.set_side_to_hold_power(RECTIFIER);
    hvdc.set_nominal_dc_current_per_pole_in_kA(2.0);

    hvdc.set_converter_operation_mode(INVERTER, INVERTER_CONSTANT_VOLTAGE);
    hvdc.set_nominal_dc_voltage_per_pole_in_kV(500.0);
    hvdc.set_compensating_resistance_to_hold_dc_voltage_in_ohm(0.0);

    TEST_ASSERT(fabs(hvdc.get_inverter_nominal_dc_voltage_command_in_kV()-500.0)<FLOAT_EPSILON);

    hvdc.set_converter_operation_mode(RECTIFIER, RECTIFIER_CONSTANT_CURRENT);
    hvdc.set_side_to_hold_power(RECTIFIER);
    hvdc.set_nominal_dc_current_per_pole_in_kA(2.0);

    hvdc.set_converter_operation_mode(INVERTER, INVERTER_CONSTANT_VOLTAGE);
    hvdc.set_nominal_dc_voltage_per_pole_in_kV(500.0);
    hvdc.set_compensating_resistance_to_hold_dc_voltage_in_ohm(10.0);
    // 500-10*2=480
    TEST_ASSERT(fabs(hvdc.get_inverter_nominal_dc_voltage_command_in_kV()-480.0)<FLOAT_EPSILON);

    hvdc.set_converter_operation_mode(RECTIFIER, RECTIFIER_CONSTANT_CURRENT);
    hvdc.set_side_to_hold_power(RECTIFIER);
    hvdc.set_nominal_dc_current_per_pole_in_kA(2.0);

    hvdc.set_converter_operation_mode(INVERTER, INVERTER_CONSTANT_VOLTAGE);
    hvdc.set_nominal_dc_voltage_per_pole_in_kV(500.0);
    hvdc.set_compensating_resistance_to_hold_dc_voltage_in_ohm(5.0);
    // 500-5*2=490
    TEST_ASSERT(fabs(hvdc.get_inverter_nominal_dc_voltage_command_in_kV()-490.0)<FLOAT_EPSILON);
}

void HVDC_TEST::test_reverse_converters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"HVDC_TEST");

    hvdc.set_converter_bus(RECTIFIER, 1);
    hvdc.set_converter_bus(INVERTER, 2);

    hvdc.set_converter_operation_mode(RECTIFIER, RECTIFIER_CONSTANT_POWER);
    hvdc.set_converter_operation_mode(INVERTER, INVERTER_CONSTANT_VOLTAGE);
    hvdc.set_converter_number_of_bridge(RECTIFIER, 2);
    hvdc.set_converter_number_of_bridge(INVERTER, 2);
    hvdc.set_converter_voltage_drop_per_bridge_in_kV(RECTIFIER, 1.0);
    hvdc.set_converter_voltage_drop_per_bridge_in_kV(INVERTER, 1.5);
    hvdc.set_converter_max_alpha_or_gamma_in_deg(RECTIFIER, 80.0);
    hvdc.set_converter_max_alpha_or_gamma_in_deg(INVERTER, 70.0);
    hvdc.set_converter_min_alpha_or_gamma_in_deg(RECTIFIER, 6.0);
    hvdc.set_converter_min_alpha_or_gamma_in_deg(INVERTER, 7.0);

    hvdc.set_converter_transformer_grid_side_base_voltage_in_kV(RECTIFIER, 500.0);
    hvdc.set_converter_transformer_grid_side_base_voltage_in_kV(INVERTER, 510.0);
    hvdc.set_converter_transformer_converter_side_base_voltage_in_kV(RECTIFIER, 230.0);
    hvdc.set_converter_transformer_converter_side_base_voltage_in_kV(INVERTER, 220.0);
    hvdc.set_converter_transformer_impedance_in_ohm(RECTIFIER, complex<double>(0.0, 0.12));
    hvdc.set_converter_transformer_impedance_in_ohm(INVERTER, complex<double>(0.1, 0.22));
    hvdc.set_converter_transformer_admittance_in_siemens(RECTIFIER, complex<double>(0.0, 0.002));
    hvdc.set_converter_transformer_admittance_in_siemens(INVERTER, complex<double>(0.001, 0.022));
    hvdc.set_converter_transformer_max_tap_in_pu(RECTIFIER, 1.1);
    hvdc.set_converter_transformer_max_tap_in_pu(INVERTER, 1.2);
    hvdc.set_converter_transformer_min_tap_in_pu(RECTIFIER, 0.9);
    hvdc.set_converter_transformer_min_tap_in_pu(INVERTER, 0.8);
    hvdc.set_converter_transformer_number_of_taps(RECTIFIER, 10);
    hvdc.set_converter_transformer_number_of_taps(INVERTER, 8);

    hvdc.set_converter_alpha_or_gamma_in_deg(RECTIFIER, 7.0);
    hvdc.set_converter_alpha_or_gamma_in_deg(INVERTER, 9.0);
    hvdc.set_converter_transformer_tap_in_pu(RECTIFIER, 1.0);
    hvdc.set_converter_transformer_tap_in_pu(INVERTER, 0.9);



    TEST_ASSERT(hvdc.get_converter_bus(RECTIFIER)== 1);
    TEST_ASSERT(hvdc.get_converter_bus(INVERTER)==2);

    TEST_ASSERT(hvdc.get_converter_operation_mode(RECTIFIER) == RECTIFIER_CONSTANT_POWER);
    TEST_ASSERT(hvdc.get_converter_operation_mode(INVERTER) == INVERTER_CONSTANT_VOLTAGE);
    TEST_ASSERT(hvdc.get_converter_number_of_bridge(RECTIFIER) == 2);
    TEST_ASSERT(hvdc.get_converter_number_of_bridge(INVERTER) == 2);
    TEST_ASSERT(fabs(hvdc.get_converter_voltage_drop_per_bridge_in_kV(RECTIFIER) - 1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(hvdc.get_converter_voltage_drop_per_bridge_in_kV(INVERTER) - 1.5)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(hvdc.get_converter_max_alpha_or_gamma_in_deg(RECTIFIER) - 80.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(hvdc.get_converter_max_alpha_or_gamma_in_deg(INVERTER) - 70.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(hvdc.get_converter_min_alpha_or_gamma_in_deg(RECTIFIER) - 6.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(hvdc.get_converter_min_alpha_or_gamma_in_deg(INVERTER) - 7.0)<FLOAT_EPSILON);

    TEST_ASSERT(fabs(hvdc.get_converter_transformer_grid_side_base_voltage_in_kV(RECTIFIER) - 500.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(hvdc.get_converter_transformer_grid_side_base_voltage_in_kV(INVERTER) - 510.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(hvdc.get_converter_transformer_converter_side_base_voltage_in_kV(RECTIFIER) - 230.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(hvdc.get_converter_transformer_converter_side_base_voltage_in_kV(INVERTER) - 220.0)<FLOAT_EPSILON);
    TEST_ASSERT(abs(hvdc.get_converter_transformer_impedance_in_ohm(RECTIFIER) - complex<double>(0.0, 0.12))<FLOAT_EPSILON);
    TEST_ASSERT(abs(hvdc.get_converter_transformer_impedance_in_ohm(INVERTER) - complex<double>(0.1, 0.22))<FLOAT_EPSILON);
    TEST_ASSERT(abs(hvdc.get_converter_transformer_admittance_in_siemens(RECTIFIER) - complex<double>(0.0, 0.002))<FLOAT_EPSILON);
    TEST_ASSERT(abs(hvdc.get_converter_transformer_admittance_in_siemens(INVERTER) - complex<double>(0.001, 0.022))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(hvdc.get_converter_transformer_max_tap_in_pu(RECTIFIER) - 1.1)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(hvdc.get_converter_transformer_max_tap_in_pu(INVERTER) - 1.2)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(hvdc.get_converter_transformer_min_tap_in_pu(RECTIFIER) - 0.9)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(hvdc.get_converter_transformer_min_tap_in_pu(INVERTER) - 0.8)<FLOAT_EPSILON);
    TEST_ASSERT(hvdc.get_converter_transformer_number_of_taps(RECTIFIER) == 10);
    TEST_ASSERT(hvdc.get_converter_transformer_number_of_taps(INVERTER) == 8);

    TEST_ASSERT(fabs(hvdc.get_converter_alpha_or_gamma_in_deg(RECTIFIER) - 7.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(hvdc.get_converter_alpha_or_gamma_in_deg(INVERTER) - 9.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(hvdc.get_converter_transformer_tap_in_pu(RECTIFIER) - 1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(hvdc.get_converter_transformer_tap_in_pu(INVERTER) - 0.9)<FLOAT_EPSILON);



    hvdc.reverse_converters();

    TEST_ASSERT(hvdc.get_converter_bus(RECTIFIER)== 2);
    TEST_ASSERT(hvdc.get_converter_bus(INVERTER)==1);

    TEST_ASSERT(hvdc.get_converter_operation_mode(RECTIFIER) == RECTIFIER_CONSTANT_POWER);
    TEST_ASSERT(hvdc.get_converter_operation_mode(INVERTER) == INVERTER_CONSTANT_VOLTAGE);
    TEST_ASSERT(hvdc.get_converter_number_of_bridge(RECTIFIER) == 2);
    TEST_ASSERT(hvdc.get_converter_number_of_bridge(INVERTER) == 2);
    TEST_ASSERT(fabs(hvdc.get_converter_voltage_drop_per_bridge_in_kV(RECTIFIER) - 1.5)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(hvdc.get_converter_voltage_drop_per_bridge_in_kV(INVERTER) - 1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(hvdc.get_converter_max_alpha_or_gamma_in_deg(RECTIFIER) - 70.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(hvdc.get_converter_max_alpha_or_gamma_in_deg(INVERTER) - 80.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(hvdc.get_converter_min_alpha_or_gamma_in_deg(RECTIFIER) - 7.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(hvdc.get_converter_min_alpha_or_gamma_in_deg(INVERTER) - 6.0)<FLOAT_EPSILON);

    TEST_ASSERT(fabs(hvdc.get_converter_transformer_grid_side_base_voltage_in_kV(RECTIFIER) - 510.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(hvdc.get_converter_transformer_grid_side_base_voltage_in_kV(INVERTER) - 500.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(hvdc.get_converter_transformer_converter_side_base_voltage_in_kV(RECTIFIER) - 220.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(hvdc.get_converter_transformer_converter_side_base_voltage_in_kV(INVERTER) - 230.0)<FLOAT_EPSILON);
    TEST_ASSERT(abs(hvdc.get_converter_transformer_impedance_in_ohm(RECTIFIER) - complex<double>(0.1, 0.22))<FLOAT_EPSILON);
    TEST_ASSERT(abs(hvdc.get_converter_transformer_impedance_in_ohm(INVERTER) - complex<double>(0.0, 0.12))<FLOAT_EPSILON);
    TEST_ASSERT(abs(hvdc.get_converter_transformer_admittance_in_siemens(RECTIFIER) - complex<double>(0.001, 0.022))<FLOAT_EPSILON);
    TEST_ASSERT(abs(hvdc.get_converter_transformer_admittance_in_siemens(INVERTER) - complex<double>(0.0, 0.002))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(hvdc.get_converter_transformer_max_tap_in_pu(RECTIFIER) - 1.2)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(hvdc.get_converter_transformer_max_tap_in_pu(INVERTER) - 1.1)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(hvdc.get_converter_transformer_min_tap_in_pu(RECTIFIER) - 0.8)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(hvdc.get_converter_transformer_min_tap_in_pu(INVERTER) - 0.9)<FLOAT_EPSILON);
    TEST_ASSERT(hvdc.get_converter_transformer_number_of_taps(RECTIFIER) == 8);
    TEST_ASSERT(hvdc.get_converter_transformer_number_of_taps(INVERTER) == 10);

    TEST_ASSERT(fabs(hvdc.get_converter_alpha_or_gamma_in_deg(RECTIFIER) - 9.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(hvdc.get_converter_alpha_or_gamma_in_deg(INVERTER) - 7.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(hvdc.get_converter_transformer_tap_in_pu(RECTIFIER) - 0.9)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(hvdc.get_converter_transformer_tap_in_pu(INVERTER) - 1.0)<FLOAT_EPSILON);



}


void HVDC_TEST::test_is_valid()
{
    show_test_information_for_function_of_class(__FUNCTION__,"HVDC_TEST");

    TEST_ASSERT(hvdc.is_valid()==false);
    hvdc.set_converter_bus(RECTIFIER, 1);
    TEST_ASSERT(hvdc.is_valid()==false);
    hvdc.set_converter_bus(INVERTER, 2);
    TEST_ASSERT(hvdc.is_valid()==true);
}

void HVDC_TEST::test_clear()
{
    show_test_information_for_function_of_class(__FUNCTION__,"HVDC_TEST");

    hvdc.set_converter_bus(RECTIFIER, 1);
    hvdc.set_converter_bus(INVERTER, 2);
    hvdc.set_status(true);

    hvdc.clear();
    test_constructor();
}

void HVDC_TEST::test_is_connected_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"HVDC_TEST");

    hvdc.set_converter_bus(RECTIFIER, 1);
    hvdc.set_converter_bus(INVERTER, 2);

    TEST_ASSERT(hvdc.is_connected_to_bus(1)==true);
    TEST_ASSERT(hvdc.is_connected_to_bus(2)==true);
    TEST_ASSERT(hvdc.is_connected_to_bus(3)==false);
}

void HVDC_TEST::test_copy_with_operator_equal()
{
    show_test_information_for_function_of_class(__FUNCTION__,"HVDC_TEST");

    hvdc.set_converter_bus(RECTIFIER, 1);
    hvdc.set_converter_bus(INVERTER, 2);
    hvdc.set_status(true);

    HVDC newhvdc = hvdc;

    TEST_ASSERT(newhvdc.get_converter_bus(RECTIFIER)==1);
    TEST_ASSERT(newhvdc.get_converter_bus(INVERTER)==2);
    TEST_ASSERT(newhvdc.get_status()==true);
}

void HVDC_TEST::test_get_device_id()
{
    show_test_information_for_function_of_class(__FUNCTION__,"HVDC_TEST");

    hvdc.set_converter_bus(RECTIFIER, 1);
    hvdc.set_converter_bus(INVERTER, 2);
    hvdc.set_identifier("P");

    DEVICE_ID did = hvdc.get_device_id();

    DEVICE_ID did2;
    did2.set_device_type("HVDC");
    TERMINAL terminal;
    terminal.append_bus(1);
    terminal.append_bus(2);
    did2.set_device_terminal(terminal);
    did2.set_device_identifier("P");

    TEST_ASSERT(did==did2);

    terminal.clear();
    terminal.append_bus(2);
    terminal.append_bus(3);
    did2.set_device_terminal(terminal);

    TEST_ASSERT(did!=did2);
}

void HVDC_TEST::test_solve_hvdc()
{
    show_test_information_for_function_of_class(__FUNCTION__,"HVDC_TEST");

    ostringstream osstream;

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    hvdc.set_converter_bus(RECTIFIER, 1);
    hvdc.set_converter_bus(INVERTER, 2);
    hvdc.set_identifier("P");
    hvdc.set_status(true);
    hvdc.set_line_resistance_in_ohm(10.0);
    hvdc.set_number_of_poles(SINGLE_POLE);
    hvdc.set_nominal_dc_voltage_per_pole_in_kV(800.0);
    hvdc.set_nominal_dc_power_per_pole_in_MW(4000.0);
    hvdc.set_current_power_margin(0.15);
    hvdc.set_converter_operation_mode(RECTIFIER, RECTIFIER_CONSTANT_POWER);
    hvdc.set_side_to_hold_power(INVERTER);
    hvdc.set_converter_operation_mode(INVERTER, INVERTER_CONSTANT_VOLTAGE);
    hvdc.set_compensating_resistance_to_hold_dc_voltage_in_ohm(0.0);
    hvdc.set_converter_number_of_bridge(RECTIFIER, 2);
    hvdc.set_converter_number_of_bridge(INVERTER, 2);
    hvdc.set_converter_max_alpha_or_gamma_in_deg(RECTIFIER, 50.0);
    hvdc.set_converter_min_alpha_or_gamma_in_deg(RECTIFIER, 5.0);
    hvdc.set_converter_max_alpha_or_gamma_in_deg(INVERTER, 50.0);
    hvdc.set_converter_min_alpha_or_gamma_in_deg(INVERTER, 5.0);
    hvdc.set_converter_voltage_drop_per_bridge_in_kV(RECTIFIER, 0.0);
    hvdc.set_converter_voltage_drop_per_bridge_in_kV(INVERTER, 0.0);
    hvdc.set_converter_transformer_grid_side_base_voltage_in_kV(RECTIFIER, 500.0);
    hvdc.set_converter_transformer_converter_side_base_voltage_in_kV(RECTIFIER, 400.0);
    hvdc.set_converter_transformer_grid_side_base_voltage_in_kV(INVERTER, 500.0);
    hvdc.set_converter_transformer_converter_side_base_voltage_in_kV(INVERTER, 400.0);
    hvdc.set_converter_transformer_impedance_in_ohm(RECTIFIER, complex<double>(1.0, 5.0));
    hvdc.set_converter_transformer_impedance_in_ohm(INVERTER, complex<double>(1.0, 5.0));
    hvdc.set_converter_transformer_max_tap_in_pu(RECTIFIER, 1.5);
    hvdc.set_converter_transformer_min_tap_in_pu(RECTIFIER, 0.5);
    hvdc.set_converter_transformer_max_tap_in_pu(INVERTER, 1.5);
    hvdc.set_converter_transformer_min_tap_in_pu(INVERTER, 0.5);
    hvdc.set_converter_transformer_number_of_taps(RECTIFIER, 21);
    hvdc.set_converter_transformer_number_of_taps(INVERTER, 21);

    hvdc.set_converter_transformer_tap_in_pu(RECTIFIER, 1.19);
    hvdc.set_converter_transformer_tap_in_pu(INVERTER, 1.27);

    osstream<<"Test 1: solve with 1.0 AC voltage at both sides";
    show_information_with_leading_time_stamp_with_default_toolkit(osstream);
    hvdc.solve_steady_state();
    hvdc.show_solved_hvdc_steady_state();

    osstream<<"Test 2: solve with 0.8 AC voltage at rectifier side";
    show_information_with_leading_time_stamp_with_default_toolkit(osstream);
    BUS* busptr = psdb.get_bus(hvdc.get_converter_bus(RECTIFIER));
    busptr->set_positive_sequence_voltage_in_pu(0.8);
    hvdc.solve_steady_state();
    hvdc.show_solved_hvdc_steady_state();

    osstream<<"Test 3: solve with 0.8 AC voltage at inverter side";
    show_information_with_leading_time_stamp_with_default_toolkit(osstream);
    busptr = psdb.get_bus(hvdc.get_converter_bus(RECTIFIER));
    busptr->set_positive_sequence_voltage_in_pu(1.0);
    busptr = psdb.get_bus(hvdc.get_converter_bus(INVERTER));
    busptr->set_positive_sequence_voltage_in_pu(0.8);
    hvdc.solve_steady_state();
    hvdc.show_solved_hvdc_steady_state();

    osstream<<"Test 4: solve with 0.8 AC voltage at both sides";
    show_information_with_leading_time_stamp_with_default_toolkit(osstream);
    busptr = psdb.get_bus(hvdc.get_converter_bus(RECTIFIER));
    busptr->set_positive_sequence_voltage_in_pu(0.8);
    busptr = psdb.get_bus(hvdc.get_converter_bus(INVERTER));
    busptr->set_positive_sequence_voltage_in_pu(0.8);
    hvdc.solve_steady_state();
    hvdc.show_solved_hvdc_steady_state();

    //hvdc.set_();
    //TEST_ASSERT(hvdc.get_()==);
}

void HVDC_TEST::test_set_get_dc_current()
{
    show_test_information_for_function_of_class(__FUNCTION__,"HVDC_TEST");

    hvdc.set_line_dc_current_in_kA(5.0);
    TEST_ASSERT(hvdc.get_line_dc_current_in_kA()==5.0);
    hvdc.set_line_dc_current_in_kA(-5.0);
    TEST_ASSERT(hvdc.get_line_dc_current_in_kA()==-5.0);
    hvdc.set_line_dc_current_in_kA(0.0);
    TEST_ASSERT(hvdc.get_line_dc_current_in_kA()==0.0);
}

void HVDC_TEST::test_set_get_converter_dc_current()
{
    show_test_information_for_function_of_class(__FUNCTION__,"HVDC_TEST");

    hvdc.set_line_dc_current_in_kA(5.0);
    TEST_ASSERT(hvdc.get_line_dc_current_in_kA()==5.0);
    TEST_ASSERT(hvdc.get_converter_dc_current_in_kA(RECTIFIER)==5.0);
    TEST_ASSERT(hvdc.get_converter_dc_current_in_kA(INVERTER)==5.0);
    hvdc.set_converter_dc_current_in_kA(RECTIFIER, 6.0);
    TEST_ASSERT(hvdc.get_line_dc_current_in_kA()==6.0);
    TEST_ASSERT(hvdc.get_converter_dc_current_in_kA(RECTIFIER)==6.0);
    TEST_ASSERT(hvdc.get_converter_dc_current_in_kA(INVERTER)==5.0);
    hvdc.set_converter_dc_current_in_kA(INVERTER, 7.0);
    TEST_ASSERT(hvdc.get_line_dc_current_in_kA()==6.0);
    TEST_ASSERT(hvdc.get_converter_dc_current_in_kA(RECTIFIER)==6.0);
    TEST_ASSERT(hvdc.get_converter_dc_current_in_kA(INVERTER)==7.0);
}

void HVDC_TEST::test_set_get_converter_dc_voltage()
{
    show_test_information_for_function_of_class(__FUNCTION__,"HVDC_TEST");

    hvdc.set_converter_dc_voltage_in_kV(RECTIFIER, 800.0);
    TEST_ASSERT(hvdc.get_converter_dc_voltage_in_kV(RECTIFIER)==800.0);
    hvdc.set_converter_dc_voltage_in_kV(RECTIFIER, -800.0);
    TEST_ASSERT(hvdc.get_converter_dc_voltage_in_kV(RECTIFIER)==0.0);
    hvdc.set_converter_dc_voltage_in_kV(RECTIFIER, 0.0);
    TEST_ASSERT(hvdc.get_converter_dc_voltage_in_kV(RECTIFIER)==0.0);

    hvdc.set_converter_dc_voltage_in_kV(INVERTER, 800.0);
    TEST_ASSERT(hvdc.get_converter_dc_voltage_in_kV(INVERTER)==800.0);
    hvdc.set_converter_dc_voltage_in_kV(INVERTER, -800.0);
    TEST_ASSERT(hvdc.get_converter_dc_voltage_in_kV(INVERTER)==0.0);
    hvdc.set_converter_dc_voltage_in_kV(INVERTER, 0.0);
    TEST_ASSERT(hvdc.get_converter_dc_voltage_in_kV(INVERTER)==0.0);
}


void HVDC_TEST::test_get_converter_dc_power()
{
    ostringstream osstream;
    osstream<<"HAVENOT Run testing of HVDC_TEST::"<<__FUNCTION__;
    show_information_with_leading_time_stamp_with_default_toolkit(osstream);

}

void HVDC_TEST::test_get_converter_ac_active_power()
{
    ostringstream osstream;
    osstream<<"HAVENOT Run testing of HVDC_TEST::"<<__FUNCTION__;
    show_information_with_leading_time_stamp_with_default_toolkit(osstream);

}

void HVDC_TEST::test_get_converter_ac_reactive_power()
{
    ostringstream osstream;
    osstream<<"HAVENOT Run testing of HVDC_TEST::"<<__FUNCTION__;
    show_information_with_leading_time_stamp_with_default_toolkit(osstream);

}

void HVDC_TEST::test_get_converter_ac_apparent_power()
{
    ostringstream osstream;
    osstream<<"HAVENOT Run testing of HVDC_TEST::"<<__FUNCTION__;
    show_information_with_leading_time_stamp_with_default_toolkit(osstream);

}

void HVDC_TEST::test_get_converter_ac_power_factor()
{
    ostringstream osstream;
    osstream<<"HAVENOT Run testing of HVDC_TEST::"<<__FUNCTION__;
    show_information_with_leading_time_stamp_with_default_toolkit(osstream);

}

void HVDC_TEST::test_get_converter_commutating_overlap_angle()
{
    ostringstream osstream;
    osstream<<"HAVENOT Run testing of HVDC_TEST::"<<__FUNCTION__;
    show_information_with_leading_time_stamp_with_default_toolkit(osstream);

}


void HVDC_TEST::test_set_get_hvdc_model()
{
    show_test_information_for_function_of_class(__FUNCTION__,"HVDC_TEST");

    TEST_ASSERT(hvdc.get_hvdc_model()==NULL);
    CDC4T model;
    model.set_inverter_dc_voltage_sensor_T_in_s(0.1);
    hvdc.set_model(&model);

    TEST_ASSERT(hvdc.get_hvdc_model()!=NULL);
}

#endif

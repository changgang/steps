#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif
#include <cmath>
#include "thirdparty/cpptest/cpptest.h"

#include "header/device/transformer_test.h"
#include "header/steps_namespace.h"
#include "header/basic/utility.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cstdio>

using namespace std;


TRANSFORMER_TEST::TRANSFORMER_TEST()
{
    TEST_ADD(TRANSFORMER_TEST::test_constructor);
    TEST_ADD(TRANSFORMER_TEST::test_set_get_winding_buses);
    TEST_ADD(TRANSFORMER_TEST::test_set_get_identifier);
    TEST_ADD(TRANSFORMER_TEST::test_set_get_name);
    TEST_ADD(TRANSFORMER_TEST::test_set_get_winding_breaker_status);
    TEST_ADD(TRANSFORMER_TEST::test_set_get_non_metered_end_bus);
    TEST_ADD(TRANSFORMER_TEST::test_set_get_winding_nominal_capacity);
    TEST_ADD(TRANSFORMER_TEST::test_set_get_winding_nominal_voltage);
    TEST_ADD(TRANSFORMER_TEST::test_set_get_winding_leakage_impedance);
    TEST_ADD(TRANSFORMER_TEST::test_set_get_magnetizing_admittance);
    TEST_ADD(TRANSFORMER_TEST::test_set_get_winding_turn_ratio);
    TEST_ADD(TRANSFORMER_TEST::test_set_get_winding_angle_shift);
    TEST_ADD(TRANSFORMER_TEST::test_set_get_winding_max_min_turn_ratio);
    TEST_ADD(TRANSFORMER_TEST::test_set_get_winding_max_min_angle_shift);
    TEST_ADD(TRANSFORMER_TEST::test_set_get_winding_number_of_taps);
    TEST_ADD(TRANSFORMER_TEST::test_set_get_winding_controlled_bus);
    TEST_ADD(TRANSFORMER_TEST::test_set_get_winding_controlled_max_min_voltage);
    TEST_ADD(TRANSFORMER_TEST::test_set_get_controlled_reactive_power_into_windings);
    TEST_ADD(TRANSFORMER_TEST::test_set_get_controlled_active_power_into_windings);

    TEST_ADD(TRANSFORMER_TEST::test_is_valid);
    TEST_ADD(TRANSFORMER_TEST::test_clear);
    TEST_ADD(TRANSFORMER_TEST::test_copy_with_operator_equal);
    TEST_ADD(TRANSFORMER_TEST::test_is_connected_to_bus);
    TEST_ADD(TRANSFORMER_TEST::test_get_device_id);

    TEST_ADD(TRANSFORMER_TEST::test_check_transformer_type);
    TEST_ADD(TRANSFORMER_TEST::test_get_winding_complex_current_power_2_winding_trans);
    TEST_ADD(TRANSFORMER_TEST::test_get_winding_complex_current_power_3_winding_trans);
}

void TRANSFORMER_TEST::setup()
{
    db = new POWER_SYSTEM_DATABASE;
    db->set_system_base_power_in_MVA(100.0);
    db->set_allowed_max_bus_number(10);

    BUS bus(db);
    bus.set_bus_number(1);
    bus.set_bus_type(PQ_TYPE);
    bus.set_base_voltage_in_kV(13.8);
    bus.set_voltage_in_pu(1.05);
    bus.set_angle_in_deg(0.0);
    db->append_bus(bus);

    bus.set_bus_number(2);
    bus.set_bus_type(PQ_TYPE);
    bus.set_base_voltage_in_kV(220.0);
    bus.set_voltage_in_pu(1.025);
    bus.set_angle_in_deg(10.0);
    db->append_bus(bus);

    bus.set_bus_number(3);
    bus.set_bus_type(PQ_TYPE);
    bus.set_base_voltage_in_kV(35.0);
    bus.set_voltage_in_pu(1.0);
    bus.set_angle_in_deg(6.0);
    db->append_bus(bus);

    transformer = new TRANSFORMER(db);
}
void TRANSFORMER_TEST::tear_down()
{
    delete transformer;
    transformer = NULL;

    delete db;
    db = NULL;

    show_test_end_information();
}

void TRANSFORMER_TEST::prepare_two_winding_transformer_bus_and_identifier()
{
    transformer->set_winding_bus(PRIMARY_SIDE, 1);
    transformer->set_winding_bus(SECONDARY_SIDE, 2);
    transformer->set_identifier("1#");
}

void TRANSFORMER_TEST::prepare_three_winding_transformer_bus_and_identifier()
{
    transformer->set_winding_bus(PRIMARY_SIDE, 1);
    transformer->set_winding_bus(SECONDARY_SIDE, 2);
    transformer->set_winding_bus(TERTIARY_SIDE, 3);
    transformer->set_identifier("1#");
}

void TRANSFORMER_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TRANSFORMER_TEST");

    TEST_ASSERT(transformer->get_winding_bus(PRIMARY_SIDE)==0);
    TEST_ASSERT(transformer->get_winding_bus(SECONDARY_SIDE)==0);
    TEST_ASSERT(transformer->get_winding_bus(TERTIARY_SIDE)==0);
    TEST_ASSERT(transformer->get_winding_breaker_status(PRIMARY_SIDE)==false);
    TEST_ASSERT(transformer->get_winding_breaker_status(SECONDARY_SIDE)==false);
    TEST_ASSERT(transformer->get_winding_breaker_status(TERTIARY_SIDE)==false);
    TEST_ASSERT(transformer->get_winding_connection_type(PRIMARY_SIDE)==WYE_CONNECTION);
    TEST_ASSERT(transformer->get_winding_connection_type(SECONDARY_SIDE)==WYE_CONNECTION);
    TEST_ASSERT(transformer->get_winding_connection_type(TERTIARY_SIDE)==WYE_CONNECTION);
    TEST_ASSERT(transformer->get_winding_nominal_capacity_in_MVA(PRIMARY_SIDE, SECONDARY_SIDE)==0.0);
    TEST_ASSERT(transformer->get_winding_nominal_capacity_in_MVA(PRIMARY_SIDE, TERTIARY_SIDE)==0.0);
    TEST_ASSERT(transformer->get_winding_nominal_capacity_in_MVA(SECONDARY_SIDE, TERTIARY_SIDE)==0.0);
    TEST_ASSERT(transformer->get_winding_nominal_voltage_in_kV(PRIMARY_SIDE)==0.0);
    TEST_ASSERT(transformer->get_winding_nominal_voltage_in_kV(SECONDARY_SIDE)==0.0);
    TEST_ASSERT(transformer->get_winding_nominal_voltage_in_kV(TERTIARY_SIDE)==0.0);

    TEST_ASSERT(transformer->get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE)==0.0);
    TEST_ASSERT(transformer->get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, TERTIARY_SIDE)==0.0);
    TEST_ASSERT(transformer->get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(SECONDARY_SIDE, TERTIARY_SIDE)==0.0);
    TEST_ASSERT(transformer->get_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(PRIMARY_SIDE)==1.0);
    TEST_ASSERT(transformer->get_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(SECONDARY_SIDE)==1.0);
    TEST_ASSERT(transformer->get_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(TERTIARY_SIDE)==1.0);
    TEST_ASSERT(transformer->get_winding_angle_shift_in_deg(PRIMARY_SIDE)==0.0);
    TEST_ASSERT(transformer->get_winding_angle_shift_in_deg(SECONDARY_SIDE)==0.0);
    TEST_ASSERT(transformer->get_winding_angle_shift_in_deg(TERTIARY_SIDE)==0.0);
    TEST_ASSERT(transformer->get_winding_number_of_taps(PRIMARY_SIDE)==DEFAULT_WINDING_NUMBER_OF_TAPS);
    TEST_ASSERT(transformer->get_winding_number_of_taps(SECONDARY_SIDE)==DEFAULT_WINDING_NUMBER_OF_TAPS);
    TEST_ASSERT(transformer->get_winding_number_of_taps(TERTIARY_SIDE)==DEFAULT_WINDING_NUMBER_OF_TAPS);
    TEST_ASSERT(transformer->get_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(PRIMARY_SIDE)==1.1);
    TEST_ASSERT(transformer->get_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(SECONDARY_SIDE)==1.1);
    TEST_ASSERT(transformer->get_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(TERTIARY_SIDE)==1.1);
    TEST_ASSERT(transformer->get_winding_min_turn_ratio_based_on_winding_nominal_voltage_in_pu(PRIMARY_SIDE)==0.9);
    TEST_ASSERT(transformer->get_winding_min_turn_ratio_based_on_winding_nominal_voltage_in_pu(SECONDARY_SIDE)==0.9);
    TEST_ASSERT(transformer->get_winding_min_turn_ratio_based_on_winding_nominal_voltage_in_pu(TERTIARY_SIDE)==0.9);
    TEST_ASSERT(transformer->get_winding_max_angle_shift_in_deg(PRIMARY_SIDE)==0.0);
    TEST_ASSERT(transformer->get_winding_max_angle_shift_in_deg(SECONDARY_SIDE)==0.0);
    TEST_ASSERT(transformer->get_winding_max_angle_shift_in_deg(TERTIARY_SIDE)==0.0);
    TEST_ASSERT(transformer->get_winding_min_angle_shift_in_deg(PRIMARY_SIDE)==0.0);
    TEST_ASSERT(transformer->get_winding_min_angle_shift_in_deg(SECONDARY_SIDE)==0.0);
    TEST_ASSERT(transformer->get_winding_min_angle_shift_in_deg(TERTIARY_SIDE)==0.0);
    TEST_ASSERT(transformer->get_winding_control_mode(PRIMARY_SIDE)==TRANSFORMER_TAP_NO_CONTROL);
    TEST_ASSERT(transformer->get_winding_control_mode(SECONDARY_SIDE)==TRANSFORMER_TAP_NO_CONTROL);
    TEST_ASSERT(transformer->get_winding_control_mode(TERTIARY_SIDE)==TRANSFORMER_TAP_NO_CONTROL);
    TEST_ASSERT(transformer->get_winding_controlled_bus(PRIMARY_SIDE)==0);
    TEST_ASSERT(transformer->get_winding_controlled_bus(SECONDARY_SIDE)==0);
    TEST_ASSERT(transformer->get_winding_controlled_bus(TERTIARY_SIDE)==0);
    TEST_ASSERT(transformer->get_winding_controlled_max_voltage_in_pu(PRIMARY_SIDE)==1.1);
    TEST_ASSERT(transformer->get_winding_controlled_max_voltage_in_pu(SECONDARY_SIDE)==1.1);
    TEST_ASSERT(transformer->get_winding_controlled_max_voltage_in_pu(TERTIARY_SIDE)==1.1);
    TEST_ASSERT(transformer->get_winding_controlled_min_voltage_in_pu(PRIMARY_SIDE)==0.9);
    TEST_ASSERT(transformer->get_winding_controlled_min_voltage_in_pu(SECONDARY_SIDE)==0.9);
    TEST_ASSERT(transformer->get_winding_controlled_min_voltage_in_pu(TERTIARY_SIDE)==0.9);
    TEST_ASSERT(transformer->get_controlled_max_reactive_power_into_winding_in_MVar(PRIMARY_SIDE)==0.0);
    TEST_ASSERT(transformer->get_controlled_max_reactive_power_into_winding_in_MVar(SECONDARY_SIDE)==0.0);
    TEST_ASSERT(transformer->get_controlled_max_reactive_power_into_winding_in_MVar(TERTIARY_SIDE)==0.0);
    TEST_ASSERT(transformer->get_controlled_min_reactive_power_into_winding_in_MVar(PRIMARY_SIDE)==0.0);
    TEST_ASSERT(transformer->get_controlled_min_reactive_power_into_winding_in_MVar(SECONDARY_SIDE)==0.0);
    TEST_ASSERT(transformer->get_controlled_min_reactive_power_into_winding_in_MVar(TERTIARY_SIDE)==0.0);
    TEST_ASSERT(transformer->get_controlled_max_active_power_into_winding_in_MW(PRIMARY_SIDE)==0.0);
    TEST_ASSERT(transformer->get_controlled_max_active_power_into_winding_in_MW(SECONDARY_SIDE)==0.0);
    TEST_ASSERT(transformer->get_controlled_max_active_power_into_winding_in_MW(TERTIARY_SIDE)==0.0);
    TEST_ASSERT(transformer->get_controlled_min_active_power_into_winding_in_MW(PRIMARY_SIDE)==0.0);
    TEST_ASSERT(transformer->get_controlled_min_active_power_into_winding_in_MW(SECONDARY_SIDE)==0.0);
    TEST_ASSERT(transformer->get_controlled_min_active_power_into_winding_in_MW(TERTIARY_SIDE)==0.0);
}

void TRANSFORMER_TEST::test_set_get_winding_buses()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TRANSFORMER_TEST");

    transformer->set_winding_bus(PRIMARY_SIDE, 1);
    TEST_ASSERT(transformer->get_winding_bus(PRIMARY_SIDE)==1);
    transformer->set_winding_bus(SECONDARY_SIDE, 2);
    TEST_ASSERT(transformer->get_winding_bus(SECONDARY_SIDE)==2);
    transformer->set_winding_bus(TERTIARY_SIDE, 3);
    TEST_ASSERT(transformer->get_winding_bus(TERTIARY_SIDE)==3);
    transformer->set_winding_bus(PRIMARY_SIDE, 0);
    TEST_ASSERT(transformer->get_winding_bus(PRIMARY_SIDE)==0);
    transformer->set_winding_bus(SECONDARY_SIDE, 0);
    TEST_ASSERT(transformer->get_winding_bus(SECONDARY_SIDE)==0);
    transformer->set_winding_bus(TERTIARY_SIDE, 0);
    TEST_ASSERT(transformer->get_winding_bus(TERTIARY_SIDE)==0);
}

void TRANSFORMER_TEST::test_set_get_identifier()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TRANSFORMER_TEST");

    transformer->set_identifier("1#");
    TEST_ASSERT(transformer->get_identifier()=="1#");
}

void TRANSFORMER_TEST::test_set_get_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TRANSFORMER_TEST");

    transformer->set_transformer_name("trans 1");
    TEST_ASSERT(transformer->get_transformer_name()=="trans 1");
}

void TRANSFORMER_TEST::test_set_get_winding_breaker_status()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TRANSFORMER_TEST");

    prepare_three_winding_transformer_bus_and_identifier();

    transformer->set_winding_breaker_status(PRIMARY_SIDE, true);
    TEST_ASSERT(transformer->get_winding_breaker_status(PRIMARY_SIDE)==true);
    transformer->set_winding_breaker_status(SECONDARY_SIDE, true);
    TEST_ASSERT(transformer->get_winding_breaker_status(SECONDARY_SIDE)==true);
    transformer->set_winding_breaker_status(TERTIARY_SIDE, true);
    TEST_ASSERT(transformer->get_winding_breaker_status(TERTIARY_SIDE)==true);

    transformer->set_winding_breaker_status(PRIMARY_SIDE, false);
    TEST_ASSERT(transformer->get_winding_breaker_status(PRIMARY_SIDE)==false);
    transformer->set_winding_breaker_status(SECONDARY_SIDE, false);
    TEST_ASSERT(transformer->get_winding_breaker_status(SECONDARY_SIDE)==false);
    transformer->set_winding_breaker_status(TERTIARY_SIDE, false);
    TEST_ASSERT(transformer->get_winding_breaker_status(TERTIARY_SIDE)==false);
}

void TRANSFORMER_TEST::test_set_get_non_metered_end_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TRANSFORMER_TEST");

    prepare_three_winding_transformer_bus_and_identifier();

    transformer->set_non_metered_end_bus(0);
    TEST_ASSERT(transformer->get_non_metered_end_bus()==transformer->get_winding_bus(SECONDARY_SIDE));
    transformer->set_non_metered_end_bus(1);
    TEST_ASSERT(transformer->get_non_metered_end_bus()==1);
    transformer->set_non_metered_end_bus(2);
    TEST_ASSERT(transformer->get_non_metered_end_bus()==2);
    transformer->set_non_metered_end_bus(3);
    TEST_ASSERT(transformer->get_non_metered_end_bus()==3);
}

void TRANSFORMER_TEST::test_set_get_winding_nominal_capacity()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TRANSFORMER_TEST");

    prepare_three_winding_transformer_bus_and_identifier();

    transformer->set_winding_nominal_capacity_in_MVA(PRIMARY_SIDE, SECONDARY_SIDE, 120.0);
    TEST_ASSERT(transformer->get_winding_nominal_capacity_in_MVA(PRIMARY_SIDE, SECONDARY_SIDE)==120.0);
    transformer->set_winding_nominal_capacity_in_MVA(PRIMARY_SIDE, PRIMARY_SIDE, 130.0);
    TEST_ASSERT(transformer->get_winding_nominal_capacity_in_MVA(PRIMARY_SIDE, SECONDARY_SIDE)==120.0);
    transformer->set_winding_nominal_capacity_in_MVA(PRIMARY_SIDE, TERTIARY_SIDE, 130.0);
    TEST_ASSERT(transformer->get_winding_nominal_capacity_in_MVA(PRIMARY_SIDE, TERTIARY_SIDE)==130.0);
    transformer->set_winding_nominal_capacity_in_MVA(TERTIARY_SIDE, TERTIARY_SIDE, 140.0);
    TEST_ASSERT(transformer->get_winding_nominal_capacity_in_MVA(PRIMARY_SIDE, TERTIARY_SIDE)==130.0);
    transformer->set_winding_nominal_capacity_in_MVA(SECONDARY_SIDE, TERTIARY_SIDE, 140.0);
    TEST_ASSERT(transformer->get_winding_nominal_capacity_in_MVA(SECONDARY_SIDE, TERTIARY_SIDE)==140.0);
    transformer->set_winding_nominal_capacity_in_MVA(SECONDARY_SIDE, SECONDARY_SIDE, 150.0);
    TEST_ASSERT(transformer->get_winding_nominal_capacity_in_MVA(SECONDARY_SIDE, TERTIARY_SIDE)==140.0);
    TEST_ASSERT(transformer->get_winding_nominal_capacity_in_MVA(PRIMARY_SIDE, SECONDARY_SIDE)==120.0);
    TEST_ASSERT(transformer->get_winding_nominal_capacity_in_MVA(PRIMARY_SIDE, TERTIARY_SIDE)==130.0);
}

void TRANSFORMER_TEST::test_set_get_winding_nominal_voltage()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TRANSFORMER_TEST");

    prepare_three_winding_transformer_bus_and_identifier();

    transformer->set_winding_nominal_voltage_in_kV(PRIMARY_SIDE, 14.0);
    TEST_ASSERT(transformer->get_winding_nominal_voltage_in_kV(PRIMARY_SIDE)==14.0);
    transformer->set_winding_nominal_voltage_in_kV(SECONDARY_SIDE, 230.0);
    TEST_ASSERT(transformer->get_winding_nominal_voltage_in_kV(SECONDARY_SIDE)==230.0);
    transformer->set_winding_nominal_voltage_in_kV(TERTIARY_SIDE, 36.0);
    TEST_ASSERT(transformer->get_winding_nominal_voltage_in_kV(TERTIARY_SIDE)==36.0);
}
void TRANSFORMER_TEST::test_set_get_winding_leakage_impedance()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TRANSFORMER_TEST");

    prepare_three_winding_transformer_bus_and_identifier();

    complex<double>z;
    z=complex<double>(0.001, 0.02);

    transformer->set_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE, z);
    TEST_ASSERT(transformer->get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE)==z);
    transformer->set_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(SECONDARY_SIDE, TERTIARY_SIDE, z);
    TEST_ASSERT(transformer->get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(SECONDARY_SIDE, TERTIARY_SIDE)==z);
    transformer->set_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, TERTIARY_SIDE, z);
    TEST_ASSERT(transformer->get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, TERTIARY_SIDE)==z);
}

void TRANSFORMER_TEST::test_set_get_magnetizing_admittance()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TRANSFORMER_TEST");

    prepare_three_winding_transformer_bus_and_identifier();
    complex<double>y(0.0, -0.02);
    transformer->set_magnetizing_admittance_based_on_primary_winding_bus_base_voltage_and_system_base_power_in_pu(y);
    TEST_ASSERT(transformer->get_magnetizing_admittance_based_on_primary_winding_bus_base_voltage_and_system_base_power_in_pu()==y)
}

void TRANSFORMER_TEST::test_set_get_winding_turn_ratio()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TRANSFORMER_TEST");

    prepare_three_winding_transformer_bus_and_identifier();

    transformer->set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(PRIMARY_SIDE, 1.2);
    TEST_ASSERT(transformer->get_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(PRIMARY_SIDE)==1.2);
    transformer->set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(SECONDARY_SIDE, 1.2);
    TEST_ASSERT(transformer->get_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(SECONDARY_SIDE)==1.2);
    transformer->set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(TERTIARY_SIDE, 1.2);
    TEST_ASSERT(transformer->get_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(TERTIARY_SIDE)==1.2);
}

void TRANSFORMER_TEST::test_set_get_winding_angle_shift()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TRANSFORMER_TEST");

    prepare_three_winding_transformer_bus_and_identifier();

    transformer->set_winding_angle_shift_in_deg(PRIMARY_SIDE, 30.0);
    TEST_ASSERT(transformer->get_winding_angle_shift_in_deg(PRIMARY_SIDE)==30.0);
    transformer->set_winding_angle_shift_in_deg(SECONDARY_SIDE, 25.0);
    TEST_ASSERT(transformer->get_winding_angle_shift_in_deg(SECONDARY_SIDE)==25.0);
    transformer->set_winding_angle_shift_in_deg(TERTIARY_SIDE, 15.0);
    TEST_ASSERT(transformer->get_winding_angle_shift_in_deg(TERTIARY_SIDE)==15.0);
}

void TRANSFORMER_TEST::test_set_get_winding_rating()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TRANSFORMER_TEST");

    prepare_three_winding_transformer_bus_and_identifier();

    RATING rate;
    rate.set_rating_A_MVA(100.0);
    rate.set_rating_B_MVA(200.0);
    rate.set_rating_C_MVA(300.0);

    transformer->set_winding_rating_in_MVA(PRIMARY_SIDE, rate);
    TEST_ASSERT(fabs(transformer->get_winding_rating_in_MVA(PRIMARY_SIDE).get_rating_A_MVA()-100.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(transformer->get_winding_rating_in_MVA(PRIMARY_SIDE).get_rating_B_MVA()-200.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(transformer->get_winding_rating_in_MVA(PRIMARY_SIDE).get_rating_C_MVA()-300.0)<FLOAT_EPSILON);

    rate.set_rating_A_MVA(400.0);
    rate.set_rating_B_MVA(500.0);
    rate.set_rating_C_MVA(600.0);

    transformer->set_winding_rating_in_MVA(SECONDARY_SIDE, rate);
    TEST_ASSERT(fabs(transformer->get_winding_rating_in_MVA(SECONDARY_SIDE).get_rating_A_MVA()-400.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(transformer->get_winding_rating_in_MVA(SECONDARY_SIDE).get_rating_B_MVA()-500.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(transformer->get_winding_rating_in_MVA(SECONDARY_SIDE)[0]-600.0)<FLOAT_EPSILON);

    rate.set_rating_A_MVA(700.0);
    rate.set_rating_B_MVA(800.0);
    rate.set_rating_C_MVA(900.0);

    transformer->set_winding_rating_in_MVA(TERTIARY_SIDE, rate);
    TEST_ASSERT(fabs(transformer->get_winding_rating_in_MVA(TERTIARY_SIDE).get_rating_A_MVA()-700.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(transformer->get_winding_rating_in_MVA(TERTIARY_SIDE).get_rating_B_MVA()-800.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(transformer->get_winding_rating_in_MVA(TERTIARY_SIDE).get_rating_C_MVA()-900.0)<FLOAT_EPSILON);
}

void TRANSFORMER_TEST::test_set_get_winding_max_min_turn_ratio()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TRANSFORMER_TEST");

    prepare_three_winding_transformer_bus_and_identifier();

    transformer->set_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(PRIMARY_SIDE, 1.2);
    TEST_ASSERT(transformer->get_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(PRIMARY_SIDE)==1.2);
    transformer->set_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(SECONDARY_SIDE, 1.2);
    TEST_ASSERT(transformer->get_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(SECONDARY_SIDE)==1.2);
    transformer->set_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(TERTIARY_SIDE, 1.2);
    TEST_ASSERT(transformer->get_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(TERTIARY_SIDE)==1.2);

    transformer->set_winding_min_turn_ratio_based_on_winding_nominal_voltage_in_pu(PRIMARY_SIDE, 0.8);
    TEST_ASSERT(transformer->get_winding_min_turn_ratio_based_on_winding_nominal_voltage_in_pu(PRIMARY_SIDE)==0.8);
    transformer->set_winding_min_turn_ratio_based_on_winding_nominal_voltage_in_pu(SECONDARY_SIDE, 0.8);
    TEST_ASSERT(transformer->get_winding_min_turn_ratio_based_on_winding_nominal_voltage_in_pu(SECONDARY_SIDE)==0.8);
    transformer->set_winding_min_turn_ratio_based_on_winding_nominal_voltage_in_pu(TERTIARY_SIDE, 0.8);
    TEST_ASSERT(transformer->get_winding_min_turn_ratio_based_on_winding_nominal_voltage_in_pu(TERTIARY_SIDE)==0.8);
}

void TRANSFORMER_TEST::test_set_get_winding_max_min_angle_shift()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TRANSFORMER_TEST");

    prepare_three_winding_transformer_bus_and_identifier();

    transformer->set_winding_max_angle_shift_in_deg(PRIMARY_SIDE, 30.0);
    TEST_ASSERT(transformer->get_winding_max_angle_shift_in_deg(PRIMARY_SIDE)==30.0);
    transformer->set_winding_max_angle_shift_in_deg(SECONDARY_SIDE, 30.0);
    TEST_ASSERT(transformer->get_winding_max_angle_shift_in_deg(SECONDARY_SIDE)==30.0);
    transformer->set_winding_max_angle_shift_in_deg(TERTIARY_SIDE, 30.0);
    TEST_ASSERT(transformer->get_winding_max_angle_shift_in_deg(TERTIARY_SIDE)==30.0);

    transformer->set_winding_min_angle_shift_in_deg(PRIMARY_SIDE, -30.0);
    TEST_ASSERT(transformer->get_winding_min_angle_shift_in_deg(PRIMARY_SIDE)==-30.0);
    transformer->set_winding_min_angle_shift_in_deg(SECONDARY_SIDE, -30.0);
    TEST_ASSERT(transformer->get_winding_min_angle_shift_in_deg(SECONDARY_SIDE)==-30.0);
    transformer->set_winding_min_angle_shift_in_deg(TERTIARY_SIDE, -30.0);
    TEST_ASSERT(transformer->get_winding_min_angle_shift_in_deg(TERTIARY_SIDE)==-30.0);
}

void TRANSFORMER_TEST::test_set_get_winding_number_of_taps()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TRANSFORMER_TEST");

    prepare_three_winding_transformer_bus_and_identifier();

    transformer->set_winding_number_of_taps(PRIMARY_SIDE, 30);
    TEST_ASSERT(transformer->get_winding_number_of_taps(PRIMARY_SIDE)==30);
    transformer->set_winding_number_of_taps(SECONDARY_SIDE, 20);
    TEST_ASSERT(transformer->get_winding_number_of_taps(SECONDARY_SIDE)==20);
    transformer->set_winding_number_of_taps(TERTIARY_SIDE, 21);
    TEST_ASSERT(transformer->get_winding_number_of_taps(TERTIARY_SIDE)==21);
}

void TRANSFORMER_TEST::test_set_get_winding_controlled_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TRANSFORMER_TEST");

    prepare_three_winding_transformer_bus_and_identifier();

    transformer->set_winding_controlled_bus(PRIMARY_SIDE, 1);
    TEST_ASSERT(transformer->get_winding_controlled_bus(PRIMARY_SIDE)==1);
    transformer->set_winding_controlled_bus(SECONDARY_SIDE, 2);
    TEST_ASSERT(transformer->get_winding_controlled_bus(SECONDARY_SIDE)==2);
    transformer->set_winding_controlled_bus(TERTIARY_SIDE, 3);
    TEST_ASSERT(transformer->get_winding_controlled_bus(TERTIARY_SIDE)==3);
}

void TRANSFORMER_TEST::test_set_get_winding_controlled_max_min_voltage()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TRANSFORMER_TEST");

    prepare_three_winding_transformer_bus_and_identifier();

    transformer->set_winding_controlled_max_voltage_in_pu(PRIMARY_SIDE, 1.1);
    TEST_ASSERT(transformer->get_winding_controlled_max_voltage_in_pu(PRIMARY_SIDE)==1.1);
    transformer->set_winding_controlled_max_voltage_in_pu(SECONDARY_SIDE, 1.2);
    TEST_ASSERT(transformer->get_winding_controlled_max_voltage_in_pu(SECONDARY_SIDE)==1.2);
    transformer->set_winding_controlled_max_voltage_in_pu(TERTIARY_SIDE, 1.03);
    TEST_ASSERT(transformer->get_winding_controlled_max_voltage_in_pu(TERTIARY_SIDE)==1.03);

    transformer->set_winding_controlled_min_voltage_in_pu(PRIMARY_SIDE, 0.9);
    TEST_ASSERT(transformer->get_winding_controlled_min_voltage_in_pu(PRIMARY_SIDE)==0.9);
    transformer->set_winding_controlled_min_voltage_in_pu(SECONDARY_SIDE, 0.8);
    TEST_ASSERT(transformer->get_winding_controlled_min_voltage_in_pu(SECONDARY_SIDE)==0.8);
    transformer->set_winding_controlled_min_voltage_in_pu(TERTIARY_SIDE, 0.97);
    TEST_ASSERT(transformer->get_winding_controlled_min_voltage_in_pu(TERTIARY_SIDE)==0.97);
}

void TRANSFORMER_TEST::test_set_get_controlled_reactive_power_into_windings()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TRANSFORMER_TEST");

    transformer->set_controlled_max_reactive_power_into_winding_in_MVar(PRIMARY_SIDE, 120.0);
    transformer->set_controlled_min_reactive_power_into_winding_in_MVar(PRIMARY_SIDE, 60.0);
    transformer->set_controlled_max_reactive_power_into_winding_in_MVar(SECONDARY_SIDE, 120.0);
    transformer->set_controlled_min_reactive_power_into_winding_in_MVar(SECONDARY_SIDE, 60.0);
    transformer->set_controlled_max_reactive_power_into_winding_in_MVar(TERTIARY_SIDE, 120.0);
    transformer->set_controlled_min_reactive_power_into_winding_in_MVar(TERTIARY_SIDE, 60.0);

    TEST_ASSERT(transformer->get_controlled_max_reactive_power_into_winding_in_MVar(PRIMARY_SIDE)==120.0);
    TEST_ASSERT(transformer->get_controlled_min_reactive_power_into_winding_in_MVar(PRIMARY_SIDE)==60.0);
    TEST_ASSERT(transformer->get_controlled_max_reactive_power_into_winding_in_MVar(SECONDARY_SIDE)==120.0);
    TEST_ASSERT(transformer->get_controlled_min_reactive_power_into_winding_in_MVar(SECONDARY_SIDE)==60.0);
    TEST_ASSERT(transformer->get_controlled_max_reactive_power_into_winding_in_MVar(TERTIARY_SIDE)==120.0);
    TEST_ASSERT(transformer->get_controlled_min_reactive_power_into_winding_in_MVar(TERTIARY_SIDE)==60.0);
}

void TRANSFORMER_TEST::test_set_get_controlled_active_power_into_windings()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TRANSFORMER_TEST");

    transformer->set_controlled_max_active_power_into_winding_in_MW(PRIMARY_SIDE, 120.0);
    transformer->set_controlled_min_active_power_into_winding_in_MW(PRIMARY_SIDE, 60.0);
    transformer->set_controlled_max_active_power_into_winding_in_MW(SECONDARY_SIDE, 120.0);
    transformer->set_controlled_min_active_power_into_winding_in_MW(SECONDARY_SIDE, 60.0);
    transformer->set_controlled_max_active_power_into_winding_in_MW(TERTIARY_SIDE, 120.0);
    transformer->set_controlled_min_active_power_into_winding_in_MW(TERTIARY_SIDE, 60.0);

    TEST_ASSERT(transformer->get_controlled_max_active_power_into_winding_in_MW(PRIMARY_SIDE)==120.0);
    TEST_ASSERT(transformer->get_controlled_min_active_power_into_winding_in_MW(PRIMARY_SIDE)==60.0);
    TEST_ASSERT(transformer->get_controlled_max_active_power_into_winding_in_MW(SECONDARY_SIDE)==120.0);
    TEST_ASSERT(transformer->get_controlled_min_active_power_into_winding_in_MW(SECONDARY_SIDE)==60.0);
    TEST_ASSERT(transformer->get_controlled_max_active_power_into_winding_in_MW(TERTIARY_SIDE)==120.0);
    TEST_ASSERT(transformer->get_controlled_min_active_power_into_winding_in_MW(TERTIARY_SIDE)==60.0);
}

void TRANSFORMER_TEST::test_is_valid()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TRANSFORMER_TEST");

    TEST_ASSERT(transformer->is_valid()==false);
    transformer->set_winding_bus(PRIMARY_SIDE, 1);
    TEST_ASSERT(transformer->is_valid()==false);
    transformer->set_winding_bus(SECONDARY_SIDE, 2);
    TEST_ASSERT(transformer->is_valid()==true);
    transformer->set_winding_bus(TERTIARY_SIDE, 3);
    TEST_ASSERT(transformer->is_valid()==true);
}

void TRANSFORMER_TEST::test_clear()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TRANSFORMER_TEST");

    transformer->set_winding_bus(PRIMARY_SIDE, 1);
    transformer->set_winding_bus(SECONDARY_SIDE, 2);
    transformer->set_winding_bus(TERTIARY_SIDE, 3);
    transformer->set_identifier("1#");
    transformer->set_winding_breaker_status(PRIMARY_SIDE, true);
    transformer->set_winding_breaker_status(SECONDARY_SIDE, true);
    transformer->set_winding_breaker_status(TERTIARY_SIDE, true);

    transformer->clear();

    test_constructor();
}

void TRANSFORMER_TEST::test_copy_with_operator_equal()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TRANSFORMER_TEST");

    prepare_three_winding_transformer_bus_and_identifier();

    transformer->set_winding_breaker_status(PRIMARY_SIDE, true);
    transformer->set_winding_nominal_voltage_in_kV(PRIMARY_SIDE, 14.0);
    transformer->set_winding_nominal_capacity_in_MVA(PRIMARY_SIDE, SECONDARY_SIDE, 200.0);
    transformer->set_winding_connection_type(PRIMARY_SIDE, WYE_CONNECTION);
    transformer->set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(PRIMARY_SIDE, 1.05);
    transformer->set_winding_angle_shift_in_deg(PRIMARY_SIDE, 0.0);
    transformer->set_winding_number_of_taps(PRIMARY_SIDE, 11);
    transformer->set_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(PRIMARY_SIDE, 1.2);
    transformer->set_winding_min_turn_ratio_based_on_winding_nominal_voltage_in_pu(PRIMARY_SIDE, 0.8);
    transformer->set_winding_max_angle_shift_in_deg(PRIMARY_SIDE, 60.0);
    transformer->set_winding_min_angle_shift_in_deg(PRIMARY_SIDE, -60.0);

    transformer->set_winding_breaker_status(SECONDARY_SIDE, true);
    transformer->set_winding_nominal_voltage_in_kV(SECONDARY_SIDE, 230.0);
    transformer->set_winding_nominal_capacity_in_MVA(SECONDARY_SIDE,  TERTIARY_SIDE, 200.0);
    transformer->set_winding_connection_type(SECONDARY_SIDE, DELTA_CONNECTION);
    transformer->set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(SECONDARY_SIDE, 1.05);
    transformer->set_winding_angle_shift_in_deg(SECONDARY_SIDE, 0.0);
    transformer->set_winding_number_of_taps(SECONDARY_SIDE, 11);
    transformer->set_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(SECONDARY_SIDE, 1.2);
    transformer->set_winding_min_turn_ratio_based_on_winding_nominal_voltage_in_pu(SECONDARY_SIDE, 0.8);
    transformer->set_winding_max_angle_shift_in_deg(SECONDARY_SIDE, 60.0);
    transformer->set_winding_min_angle_shift_in_deg(SECONDARY_SIDE, -60.0);

    transformer->set_winding_breaker_status(TERTIARY_SIDE, true);
    transformer->set_winding_nominal_voltage_in_kV(TERTIARY_SIDE, 36.0);
    transformer->set_winding_nominal_capacity_in_MVA(PRIMARY_SIDE,  TERTIARY_SIDE, 200.0);
    transformer->set_winding_connection_type(TERTIARY_SIDE, WYE_N_CONNECTION);
    transformer->set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(TERTIARY_SIDE, 1.05);
    transformer->set_winding_angle_shift_in_deg(TERTIARY_SIDE, 0.0);
    transformer->set_winding_number_of_taps(TERTIARY_SIDE, 11);
    transformer->set_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(TERTIARY_SIDE, 1.2);
    transformer->set_winding_min_turn_ratio_based_on_winding_nominal_voltage_in_pu(TERTIARY_SIDE, 0.8);
    transformer->set_winding_max_angle_shift_in_deg(TERTIARY_SIDE, 60.0);
    transformer->set_winding_min_angle_shift_in_deg(TERTIARY_SIDE, -60.0);


    TRANSFORMER newtransformer(db);

    newtransformer = (*transformer);

    TEST_ASSERT(newtransformer.get_winding_breaker_status(PRIMARY_SIDE)==true);
    TEST_ASSERT(fabs(newtransformer.get_winding_nominal_voltage_in_kV(PRIMARY_SIDE)-14.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(newtransformer.get_winding_nominal_capacity_in_MVA(PRIMARY_SIDE, SECONDARY_SIDE)-200.0)<FLOAT_EPSILON);
    TEST_ASSERT(newtransformer.get_winding_connection_type(PRIMARY_SIDE)==WYE_CONNECTION);
    TEST_ASSERT(fabs(newtransformer.get_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(PRIMARY_SIDE)-1.05)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(newtransformer.get_winding_angle_shift_in_deg(PRIMARY_SIDE)-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(newtransformer.get_winding_number_of_taps(PRIMARY_SIDE)==11);
    TEST_ASSERT(fabs(newtransformer.get_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(PRIMARY_SIDE)-1.2)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(newtransformer.get_winding_min_turn_ratio_based_on_winding_nominal_voltage_in_pu(PRIMARY_SIDE)-0.8)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(newtransformer.get_winding_max_angle_shift_in_deg(PRIMARY_SIDE)-60.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(newtransformer.get_winding_min_angle_shift_in_deg(PRIMARY_SIDE)-(-60.0))<FLOAT_EPSILON);

    TEST_ASSERT(newtransformer.get_winding_breaker_status(SECONDARY_SIDE)==true);
    TEST_ASSERT(fabs(newtransformer.get_winding_nominal_voltage_in_kV(SECONDARY_SIDE)-230.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(newtransformer.get_winding_nominal_capacity_in_MVA(SECONDARY_SIDE,  TERTIARY_SIDE)-200.0)<FLOAT_EPSILON);
    TEST_ASSERT(newtransformer.get_winding_connection_type(SECONDARY_SIDE)==DELTA_CONNECTION);
    TEST_ASSERT(fabs(newtransformer.get_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(SECONDARY_SIDE)-1.05)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(newtransformer.get_winding_angle_shift_in_deg(SECONDARY_SIDE)-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(newtransformer.get_winding_number_of_taps(SECONDARY_SIDE)==11);
    TEST_ASSERT(fabs(newtransformer.get_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(SECONDARY_SIDE)-1.2)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(newtransformer.get_winding_min_turn_ratio_based_on_winding_nominal_voltage_in_pu(SECONDARY_SIDE)-0.8)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(newtransformer.get_winding_max_angle_shift_in_deg(SECONDARY_SIDE)-60.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(newtransformer.get_winding_min_angle_shift_in_deg(SECONDARY_SIDE)-(-60.0))<FLOAT_EPSILON);

    TEST_ASSERT(newtransformer.get_winding_breaker_status(TERTIARY_SIDE)==true);
    TEST_ASSERT(fabs(newtransformer.get_winding_nominal_voltage_in_kV(TERTIARY_SIDE)-36.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(newtransformer.get_winding_nominal_capacity_in_MVA(PRIMARY_SIDE,  TERTIARY_SIDE)-200.0)<FLOAT_EPSILON);
    TEST_ASSERT(newtransformer.get_winding_connection_type(TERTIARY_SIDE)==WYE_N_CONNECTION);
    TEST_ASSERT(fabs(newtransformer.get_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(TERTIARY_SIDE)-1.05)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(newtransformer.get_winding_angle_shift_in_deg(TERTIARY_SIDE)-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(newtransformer.get_winding_number_of_taps(TERTIARY_SIDE)==11);
    TEST_ASSERT(fabs(newtransformer.get_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(TERTIARY_SIDE)-1.2)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(newtransformer.get_winding_min_turn_ratio_based_on_winding_nominal_voltage_in_pu(TERTIARY_SIDE)-0.8)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(newtransformer.get_winding_max_angle_shift_in_deg(TERTIARY_SIDE)-60.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(newtransformer.get_winding_min_angle_shift_in_deg(TERTIARY_SIDE)-(-60.0))<FLOAT_EPSILON);
}


void TRANSFORMER_TEST::test_is_connected_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TRANSFORMER_TEST");

    prepare_three_winding_transformer_bus_and_identifier();
    TEST_ASSERT(transformer->is_connected_to_bus(0)==false);
    TEST_ASSERT(transformer->is_connected_to_bus(1)==true);
    TEST_ASSERT(transformer->is_connected_to_bus(2)==true);
    TEST_ASSERT(transformer->is_connected_to_bus(3)==true);
    TEST_ASSERT(transformer->is_connected_to_bus(4)==false);
}


void TRANSFORMER_TEST::test_get_device_id()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TRANSFORMER_TEST");

    transformer->set_winding_bus(PRIMARY_SIDE, 1);
    transformer->set_winding_bus(SECONDARY_SIDE, 2);
    transformer->set_identifier("#1");

    DEVICE_ID did = transformer->get_device_id();

    DEVICE_ID did2;
    did2.set_device_type("TRANSFORMER");
    TERMINAL terminal;
    terminal.append_bus(1);
    terminal.append_bus(2);
    did2.set_device_terminal(terminal);
    did2.set_device_identifier("#1");

    TEST_ASSERT(did==did2);

    terminal.append_bus(0);
    did2.set_device_terminal(terminal);

    TEST_ASSERT(did==did2);

    terminal.clear();
    terminal.append_bus(2);
    terminal.append_bus(3);
    did2.set_device_terminal(terminal);

    TEST_ASSERT(did!=did2);

    transformer->set_winding_bus(PRIMARY_SIDE, 1);
    transformer->set_winding_bus(SECONDARY_SIDE, 2);
    transformer->set_winding_bus(TERTIARY_SIDE, 3);
    transformer->set_identifier("#1");

    did = transformer->get_device_id();

    did2.set_device_terminal(terminal);
    did2.set_device_identifier("#1");

    terminal.clear();
    terminal.append_bus(1);
    terminal.append_bus(2);
    terminal.append_bus(3);
    did2.set_device_terminal(terminal);
    did2.set_device_identifier("#1");

    TEST_ASSERT(did==did2);
}


void TRANSFORMER_TEST::test_check_transformer_type()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TRANSFORMER_TEST");

    prepare_two_winding_transformer_bus_and_identifier();
    TEST_ASSERT(transformer->is_two_winding_transformer()==true);
    TEST_ASSERT(transformer->is_three_winding_transformer()==false);

    prepare_three_winding_transformer_bus_and_identifier();
    TEST_ASSERT(transformer->is_two_winding_transformer()==false);
    TEST_ASSERT(transformer->is_three_winding_transformer()==true);
}

void TRANSFORMER_TEST::test_get_winding_complex_current_power_2_winding_trans()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TRANSFORMER_TEST");
    ostringstream sstream;
    sstream<<"Magnetizing admittance is placed at the middle of leakage impedance. Test pass with PSS/E."<<endl
      <<"Minor difference can be observed when testing transformer with non-zero magnetizing admittance.";
    show_information_with_leading_time_stamp(sstream);

    prepare_two_winding_transformer_bus_and_identifier();

    BUS* bus = db->get_bus(1);
    bus->set_base_voltage_in_kV(13.8);
    bus->set_voltage_in_pu(1.05);
    bus->set_angle_in_deg(0.0);

    bus = db->get_bus(2);
    bus->set_base_voltage_in_kV(220.0);
    bus->set_voltage_in_pu(1.025);
    bus->set_angle_in_deg(10.0);

    transformer->set_winding_breaker_status(PRIMARY_SIDE, true);
    transformer->set_winding_breaker_status(SECONDARY_SIDE, true);

    double Vnom1 = 13.0, Vnom2 = 230.0;

    transformer->set_winding_nominal_voltage_in_kV(PRIMARY_SIDE, Vnom1);
    transformer->set_winding_nominal_voltage_in_kV(SECONDARY_SIDE, Vnom2);

    double Snom1 = 300.0;
    transformer->set_winding_nominal_capacity_in_MVA(PRIMARY_SIDE, SECONDARY_SIDE, Snom1);
    complex<double> Zps(0.01, 0.1), Y(0.001, -0.004);
    transformer->set_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE, Zps);
    transformer->set_magnetizing_admittance_based_on_primary_winding_bus_base_voltage_and_system_base_power_in_pu(Y);
    double k1 = 1.1, k2 = 1.05;
    transformer->set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(PRIMARY_SIDE, k1);
    transformer->set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(SECONDARY_SIDE, k2);
    double angle1 = 5.0, angle2 = 0.0;
    transformer->set_winding_angle_shift_in_deg(PRIMARY_SIDE, angle1);
    transformer->set_winding_angle_shift_in_deg(SECONDARY_SIDE, angle2);

    sstream<<"With all windings in service:";
    show_information_with_leading_time_stamp(sstream);
    complex<double> Vstar = transformer->get_star_bus_complex_voltage_in_pu();
    sstream<<"Star bus voltage = "<<abs(Vstar)<<" pu, "<<rad2deg(arg(Vstar))<<" deg.";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Primary current = "<<abs(transformer->get_winding_complex_current_in_pu(PRIMARY_SIDE))<<" pu, "<<abs(transformer->get_winding_complex_current_in_kA(PRIMARY_SIDE))<<" kA.";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Secondary current = "<<abs(transformer->get_winding_complex_current_in_pu(SECONDARY_SIDE))<<" pu, "<<abs(transformer->get_winding_complex_current_in_kA(SECONDARY_SIDE))<<" kA.";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Primary power (into winding) = "<<transformer->get_winding_complex_power_in_MVA(PRIMARY_SIDE)<<" MVA";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Secondary power (into winding) = "<<transformer->get_winding_complex_power_in_MVA(SECONDARY_SIDE)<<" MVA";
    show_information_with_leading_time_stamp(sstream);


    transformer->set_winding_breaker_status(PRIMARY_SIDE, false);
    transformer->set_winding_breaker_status(SECONDARY_SIDE, true);

    sstream<<"With winding 1 out of service:";
    show_information_with_leading_time_stamp(sstream);
    Vstar = transformer->get_star_bus_complex_voltage_in_pu();
    sstream<<"Star bus voltage = "<<abs(Vstar)<<" pu, "<<rad2deg(arg(Vstar))<<" deg.";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Primary current = "<<abs(transformer->get_winding_complex_current_in_pu(PRIMARY_SIDE))<<" pu, "<<abs(transformer->get_winding_complex_current_in_kA(PRIMARY_SIDE))<<" kA.";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Secondary current = "<<abs(transformer->get_winding_complex_current_in_pu(SECONDARY_SIDE))<<" pu, "<<abs(transformer->get_winding_complex_current_in_kA(SECONDARY_SIDE))<<" kA.";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Primary power (into winding) = "<<transformer->get_winding_complex_power_in_MVA(PRIMARY_SIDE)<<" MVA";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Secondary power (into winding) = "<<transformer->get_winding_complex_power_in_MVA(SECONDARY_SIDE)<<" MVA";
    show_information_with_leading_time_stamp(sstream);


    transformer->set_winding_breaker_status(PRIMARY_SIDE, true);
    transformer->set_winding_breaker_status(SECONDARY_SIDE, false);

    sstream<<"With winding 2 out of service:";
    show_information_with_leading_time_stamp(sstream);
    Vstar = transformer->get_star_bus_complex_voltage_in_pu();
    sstream<<"Star bus voltage = "<<abs(Vstar)<<" pu, "<<rad2deg(arg(Vstar))<<" deg.";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Primary current = "<<abs(transformer->get_winding_complex_current_in_pu(PRIMARY_SIDE))<<" pu, "<<abs(transformer->get_winding_complex_current_in_kA(PRIMARY_SIDE))<<" kA.";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Secondary current = "<<abs(transformer->get_winding_complex_current_in_pu(SECONDARY_SIDE))<<" pu, "<<abs(transformer->get_winding_complex_current_in_kA(SECONDARY_SIDE))<<" kA.";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Primary power (into winding) = "<<transformer->get_winding_complex_power_in_MVA(PRIMARY_SIDE)<<" MVA";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Secondary power (into winding) = "<<transformer->get_winding_complex_power_in_MVA(SECONDARY_SIDE)<<" MVA";
    show_information_with_leading_time_stamp(sstream);
}



void TRANSFORMER_TEST::test_get_winding_complex_current_power_3_winding_trans()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TRANSFORMER_TEST");
    ostringstream sstream;
    sstream<<"Magnetizing admittance is placed at the virtual star bus. Test pass with PSS/E."<<endl
      <<"Minor difference can be observed when testing transformer with non-zero magnetizing admittance.";
    show_information_with_leading_time_stamp(sstream);

    prepare_three_winding_transformer_bus_and_identifier();

    BUS* bus;
    bus = db->get_bus(1);
    bus->set_base_voltage_in_kV(13.8);
    bus->set_voltage_in_pu(1.05);
    bus->set_angle_in_deg(0.0);

    bus = db->get_bus(2);
    bus->set_base_voltage_in_kV(220.0);
    bus->set_voltage_in_pu(1.025);
    bus->set_angle_in_deg(10.0);

    bus = db->get_bus(3);
    bus->set_base_voltage_in_kV(35.0);
    bus->set_voltage_in_pu(1.0);
    bus->set_angle_in_deg(6.0);

    transformer->set_winding_breaker_status(PRIMARY_SIDE, true);
    transformer->set_winding_breaker_status(SECONDARY_SIDE, true);
    transformer->set_winding_breaker_status(TERTIARY_SIDE, true);

    double Vnom1 = 13.0, Vnom2 = 230.0, Vnom3 = 36.0;
    //Vnom1 = Vbase1; Vnom2 = Vbase2; Vnom3 = Vbase3;

    transformer->set_winding_nominal_voltage_in_kV(PRIMARY_SIDE, Vnom1);
    transformer->set_winding_nominal_voltage_in_kV(SECONDARY_SIDE, Vnom2);
    transformer->set_winding_nominal_voltage_in_kV(TERTIARY_SIDE, Vnom3);

    //double Snom1 = 100.0, Snom2 = 100.0, Snom3 = 100.0;
    double Snom1 = 300.0, Snom2 = 200.0, Snom3 = 100.0;
    transformer->set_winding_nominal_capacity_in_MVA(PRIMARY_SIDE, SECONDARY_SIDE, Snom1);
    transformer->set_winding_nominal_capacity_in_MVA(SECONDARY_SIDE,  TERTIARY_SIDE, Snom2);
    transformer->set_winding_nominal_capacity_in_MVA(PRIMARY_SIDE,  TERTIARY_SIDE, Snom3);
    complex<double> Zps(0.001, 0.1), Zst(0.002, 0.2), Zpt(0.003, 0.4), Y(0.001, -0.004);
    transformer->set_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE, Zps);
    transformer->set_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(SECONDARY_SIDE, TERTIARY_SIDE, Zst);
    transformer->set_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, TERTIARY_SIDE, Zpt);
    transformer->set_magnetizing_admittance_based_on_primary_winding_bus_base_voltage_and_system_base_power_in_pu(Y);
    double k1 = 1.1, k2 = 1.05, k3 = 1.025;
    //double k1 = 1.0, k2 = 1.0, k3 = 1.0;
    transformer->set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(PRIMARY_SIDE, k1);
    transformer->set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(SECONDARY_SIDE, k2);
    transformer->set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(TERTIARY_SIDE, k3);
    double angle1 = 5.0, angle2 = 15.0, angle3 = 30.0;
    //double angle1 = 0.0, angle2 = 0.0, angle3 = 0.0;
    transformer->set_winding_angle_shift_in_deg(PRIMARY_SIDE, angle1);
    transformer->set_winding_angle_shift_in_deg(SECONDARY_SIDE, angle2);
    transformer->set_winding_angle_shift_in_deg(TERTIARY_SIDE, angle3);

    sstream<<"With all windings in service:";
    show_information_with_leading_time_stamp(sstream);
    complex<double> Vstar = transformer->get_star_bus_complex_voltage_in_pu();
    sstream<<"Star bus voltage = "<<abs(Vstar)<<" pu, "<<rad2deg(arg(Vstar))<<" deg.";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Primary current = "<<abs(transformer->get_winding_complex_current_in_pu(PRIMARY_SIDE))<<" pu, "<<abs(transformer->get_winding_complex_current_in_kA(PRIMARY_SIDE))<<" kA.";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Secondary current = "<<abs(transformer->get_winding_complex_current_in_pu(SECONDARY_SIDE))<<" pu, "<<abs(transformer->get_winding_complex_current_in_kA(SECONDARY_SIDE))<<" kA.";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Tertiary current = "<<abs(transformer->get_winding_complex_current_in_pu(TERTIARY_SIDE))<<" pu, "<<abs(transformer->get_winding_complex_current_in_kA(TERTIARY_SIDE))<<" kA.";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Primary power (into winding) = "<<transformer->get_winding_complex_power_in_MVA(PRIMARY_SIDE)<<" MVA";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Secondary power (into winding) = "<<transformer->get_winding_complex_power_in_MVA(SECONDARY_SIDE)<<" MVA";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Tertiary power (into winding) = "<<transformer->get_winding_complex_power_in_MVA(TERTIARY_SIDE)<<" MVA";
    show_information_with_leading_time_stamp(sstream);



    transformer->set_winding_breaker_status(PRIMARY_SIDE, false);
    transformer->set_winding_breaker_status(SECONDARY_SIDE, true);
    transformer->set_winding_breaker_status(TERTIARY_SIDE, true);

    sstream<<"With winding 1 out of service:";
    show_information_with_leading_time_stamp(sstream);
    Vstar = transformer->get_star_bus_complex_voltage_in_pu();
    sstream<<"Star bus voltage = "<<abs(Vstar)<<" pu, "<<rad2deg(arg(Vstar))<<" deg.";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Primary current = "<<abs(transformer->get_winding_complex_current_in_pu(PRIMARY_SIDE))<<" pu, "<<abs(transformer->get_winding_complex_current_in_kA(PRIMARY_SIDE))<<" kA.";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Secondary current = "<<abs(transformer->get_winding_complex_current_in_pu(SECONDARY_SIDE))<<" pu, "<<abs(transformer->get_winding_complex_current_in_kA(SECONDARY_SIDE))<<" kA.";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Tertiary current = "<<abs(transformer->get_winding_complex_current_in_pu(TERTIARY_SIDE))<<" pu, "<<abs(transformer->get_winding_complex_current_in_kA(TERTIARY_SIDE))<<" kA.";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Primary power (into winding) = "<<transformer->get_winding_complex_power_in_MVA(PRIMARY_SIDE)<<" MVA";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Secondary power (into winding) = "<<transformer->get_winding_complex_power_in_MVA(SECONDARY_SIDE)<<" MVA";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Tertiary power (into winding) = "<<transformer->get_winding_complex_power_in_MVA(TERTIARY_SIDE)<<" MVA";
    show_information_with_leading_time_stamp(sstream);




    transformer->set_winding_breaker_status(PRIMARY_SIDE, true);
    transformer->set_winding_breaker_status(SECONDARY_SIDE, false);
    transformer->set_winding_breaker_status(TERTIARY_SIDE, true);

    sstream<<"With winding 2 out of service:";
    show_information_with_leading_time_stamp(sstream);
    Vstar = transformer->get_star_bus_complex_voltage_in_pu();
    sstream<<"Star bus voltage = "<<abs(Vstar)<<" pu, "<<rad2deg(arg(Vstar))<<" deg.";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Primary current = "<<abs(transformer->get_winding_complex_current_in_pu(PRIMARY_SIDE))<<" pu, "<<abs(transformer->get_winding_complex_current_in_kA(PRIMARY_SIDE))<<" kA.";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Secondary current = "<<abs(transformer->get_winding_complex_current_in_pu(SECONDARY_SIDE))<<" pu, "<<abs(transformer->get_winding_complex_current_in_kA(SECONDARY_SIDE))<<" kA.";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Tertiary current = "<<abs(transformer->get_winding_complex_current_in_pu(TERTIARY_SIDE))<<" pu, "<<abs(transformer->get_winding_complex_current_in_kA(TERTIARY_SIDE))<<" kA.";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Primary power (into winding) = "<<transformer->get_winding_complex_power_in_MVA(PRIMARY_SIDE)<<" MVA";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Secondary power (into winding) = "<<transformer->get_winding_complex_power_in_MVA(SECONDARY_SIDE)<<" MVA";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Tertiary power (into winding) = "<<transformer->get_winding_complex_power_in_MVA(TERTIARY_SIDE)<<" MVA";
    show_information_with_leading_time_stamp(sstream);




    transformer->set_winding_breaker_status(PRIMARY_SIDE, true);
    transformer->set_winding_breaker_status(SECONDARY_SIDE, true);
    transformer->set_winding_breaker_status(TERTIARY_SIDE, false);

    sstream<<"With winding 3 out of service:";
    show_information_with_leading_time_stamp(sstream);
    Vstar = transformer->get_star_bus_complex_voltage_in_pu();
    sstream<<"Star bus voltage = "<<abs(Vstar)<<" pu, "<<rad2deg(arg(Vstar))<<" deg.";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Primary current = "<<abs(transformer->get_winding_complex_current_in_pu(PRIMARY_SIDE))<<" pu, "<<abs(transformer->get_winding_complex_current_in_kA(PRIMARY_SIDE))<<" kA.";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Secondary current = "<<abs(transformer->get_winding_complex_current_in_pu(SECONDARY_SIDE))<<" pu, "<<abs(transformer->get_winding_complex_current_in_kA(SECONDARY_SIDE))<<" kA.";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Tertiary current = "<<abs(transformer->get_winding_complex_current_in_pu(TERTIARY_SIDE))<<" pu, "<<abs(transformer->get_winding_complex_current_in_kA(TERTIARY_SIDE))<<" kA.";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Primary power (into winding) = "<<transformer->get_winding_complex_power_in_MVA(PRIMARY_SIDE)<<" MVA";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Secondary power (into winding) = "<<transformer->get_winding_complex_power_in_MVA(SECONDARY_SIDE)<<" MVA";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Tertiary power (into winding) = "<<transformer->get_winding_complex_power_in_MVA(TERTIARY_SIDE)<<" MVA";
    show_information_with_leading_time_stamp(sstream);


    transformer->set_winding_breaker_status(PRIMARY_SIDE, false);
    transformer->set_winding_breaker_status(SECONDARY_SIDE, false);
    transformer->set_winding_breaker_status(TERTIARY_SIDE, true);

    sstream<<"With winding 1 and 2 out of service:";
    show_information_with_leading_time_stamp(sstream);
    Vstar = transformer->get_star_bus_complex_voltage_in_pu();
    sstream<<"Star bus voltage = "<<abs(Vstar)<<" pu, "<<rad2deg(arg(Vstar))<<" deg.";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Primary current = "<<abs(transformer->get_winding_complex_current_in_pu(PRIMARY_SIDE))<<" pu, "<<abs(transformer->get_winding_complex_current_in_kA(PRIMARY_SIDE))<<" kA.";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Secondary current = "<<abs(transformer->get_winding_complex_current_in_pu(SECONDARY_SIDE))<<" pu, "<<abs(transformer->get_winding_complex_current_in_kA(SECONDARY_SIDE))<<" kA.";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Tertiary current = "<<abs(transformer->get_winding_complex_current_in_pu(TERTIARY_SIDE))<<" pu, "<<abs(transformer->get_winding_complex_current_in_kA(TERTIARY_SIDE))<<" kA.";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Primary power (into winding) = "<<transformer->get_winding_complex_power_in_MVA(PRIMARY_SIDE)<<" MVA";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Secondary power (into winding) = "<<transformer->get_winding_complex_power_in_MVA(SECONDARY_SIDE)<<" MVA";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Tertiary power (into winding) = "<<transformer->get_winding_complex_power_in_MVA(TERTIARY_SIDE)<<" MVA";
    show_information_with_leading_time_stamp(sstream);



    transformer->set_winding_breaker_status(PRIMARY_SIDE, false);
    transformer->set_winding_breaker_status(SECONDARY_SIDE, true);
    transformer->set_winding_breaker_status(TERTIARY_SIDE, false);

    sstream<<"With winding 1 and 3 out of service:";
    show_information_with_leading_time_stamp(sstream);
    Vstar = transformer->get_star_bus_complex_voltage_in_pu();
    sstream<<"Star bus voltage = "<<abs(Vstar)<<" pu, "<<rad2deg(arg(Vstar))<<" deg.";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Primary current = "<<abs(transformer->get_winding_complex_current_in_pu(PRIMARY_SIDE))<<" pu, "<<abs(transformer->get_winding_complex_current_in_kA(PRIMARY_SIDE))<<" kA.";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Secondary current = "<<abs(transformer->get_winding_complex_current_in_pu(SECONDARY_SIDE))<<" pu, "<<abs(transformer->get_winding_complex_current_in_kA(SECONDARY_SIDE))<<" kA.";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Tertiary current = "<<abs(transformer->get_winding_complex_current_in_pu(TERTIARY_SIDE))<<" pu, "<<abs(transformer->get_winding_complex_current_in_kA(TERTIARY_SIDE))<<" kA.";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Primary power (into winding) = "<<transformer->get_winding_complex_power_in_MVA(PRIMARY_SIDE)<<" MVA";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Secondary power (into winding) = "<<transformer->get_winding_complex_power_in_MVA(SECONDARY_SIDE)<<" MVA";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Tertiary power (into winding) = "<<transformer->get_winding_complex_power_in_MVA(TERTIARY_SIDE)<<" MVA";
    show_information_with_leading_time_stamp(sstream);



    transformer->set_winding_breaker_status(PRIMARY_SIDE, true);
    transformer->set_winding_breaker_status(SECONDARY_SIDE, false);
    transformer->set_winding_breaker_status(TERTIARY_SIDE, false);

    sstream<<"With winding 2 and 3 out of service:";
    show_information_with_leading_time_stamp(sstream);
    Vstar = transformer->get_star_bus_complex_voltage_in_pu();
    sstream<<"Star bus voltage = "<<abs(Vstar)<<" pu, "<<rad2deg(arg(Vstar))<<" deg.";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Primary current = "<<abs(transformer->get_winding_complex_current_in_pu(PRIMARY_SIDE))<<" pu, "<<abs(transformer->get_winding_complex_current_in_kA(PRIMARY_SIDE))<<" kA.";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Secondary current = "<<abs(transformer->get_winding_complex_current_in_pu(SECONDARY_SIDE))<<" pu, "<<abs(transformer->get_winding_complex_current_in_kA(SECONDARY_SIDE))<<" kA.";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Tertiary current = "<<abs(transformer->get_winding_complex_current_in_pu(TERTIARY_SIDE))<<" pu, "<<abs(transformer->get_winding_complex_current_in_kA(TERTIARY_SIDE))<<" kA.";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Primary power (into winding) = "<<transformer->get_winding_complex_power_in_MVA(PRIMARY_SIDE)<<" MVA";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Secondary power (into winding) = "<<transformer->get_winding_complex_power_in_MVA(SECONDARY_SIDE)<<" MVA";
    show_information_with_leading_time_stamp(sstream);
    sstream<<"Tertiary power (into winding) = "<<transformer->get_winding_complex_power_in_MVA(TERTIARY_SIDE)<<" MVA";
    show_information_with_leading_time_stamp(sstream);
}


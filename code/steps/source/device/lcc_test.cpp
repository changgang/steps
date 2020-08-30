#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif
#include <cmath>
#include "cpptest.h"
#include "header/basic/test_macro.h"
#include "header/device/lcc_test.h"
#include "header/basic/constants.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <cstdio>

#ifdef ENABLE_STEPS_TEST
using namespace std;


LCC_TEST::LCC_TEST() : converter(default_toolkit)
{
    TEST_ADD(LCC_TEST::test_constructor);
    TEST_ADD(LCC_TEST::test_set_get_number_of_bridge);
    TEST_ADD(LCC_TEST::test_set_get_voltage_drop_per_bridge);
    TEST_ADD(LCC_TEST::test_set_get_max_angle);
    TEST_ADD(LCC_TEST::test_set_get_min_angle);
    TEST_ADD(LCC_TEST::test_set_get_angle);
    TEST_ADD(LCC_TEST::test_clear);
    TEST_ADD(LCC_TEST::test_copy_with_operator_equal);
    TEST_ADD(LCC_TEST::test_solve_with_desired_dc_voltage_and_current);
}

void LCC_TEST::setup()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.set_allowed_max_bus_number(100);
    BUS bus(default_toolkit);
    bus.set_bus_number(1);
    bus.set_base_voltage_in_kV(110.0);
    bus.set_positive_sequence_voltage_in_pu(1.05);
    bus.set_positive_sequence_angle_in_deg(30.0);
    bus.set_area_number(1);
    bus.set_zone_number(1);
    psdb.append_bus(bus);

    converter.set_name("TEST LCC Converter");
    converter.set_valve_side_bus_name("TEST LCC Valve");
    converter.set_bus(1);
    converter.set_side(RECTIFIER);
    converter.set_power_percent(1.0);
    converter.set_transformer_grid_side_base_voltage_in_kV(110.0);
    converter.set_transformer_valve_side_base_voltage_in_kV(50.0);
    converter.set_transformer_impedance_in_ohm(complex<double>(0.01, 0.5));
    converter.set_transformer_max_tap_in_pu(1.2);
    converter.set_transformer_min_tap_in_pu(0.8);
    converter.set_transformer_number_of_taps(11);
}

void LCC_TEST::tear_down()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.clear();

    converter.clear();

    show_test_end_information();
}

void LCC_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LCC_TEST");

    TEST_ASSERT(converter.get_name() == "TEST LCC Converter");
    TEST_ASSERT(converter.get_side() == RECTIFIER);
    TEST_ASSERT(converter.get_bus()==1);
    TEST_ASSERT(converter.get_valve_side_bus_name()=="TEST LCC Valve");
    TEST_ASSERT(converter.get_power_percent()==1.0);
    TEST_ASSERT(converter.get_transformer_grid_side_base_voltage_in_kV()==110.0);
    TEST_ASSERT(converter.get_transformer_valve_side_base_voltage_in_kV()==50.0);
    TEST_ASSERT(converter.get_transformer_impedance_in_ohm()==complex<double>(0.01, 0.5));
    TEST_ASSERT(converter.get_transformer_max_tap_in_pu()==1.2);
    TEST_ASSERT(converter.get_transformer_min_tap_in_pu()==0.8);
    TEST_ASSERT(converter.get_transformer_number_of_taps()==11);
    TEST_ASSERT(converter.get_transformer_tap_in_pu()==1.0);

    TEST_ASSERT(converter.get_number_of_bridge()==2);
    TEST_ASSERT(converter.get_voltage_drop_per_bridge_in_kV()==0.0);
    TEST_ASSERT(converter.get_max_angle_in_deg()==90.0);
    TEST_ASSERT(converter.get_min_angle_in_deg()==0.0);
    TEST_ASSERT(converter.get_angle_in_deg()==0.0);
}
void LCC_TEST::test_set_get_number_of_bridge()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LCC_TEST");

    converter.set_number_of_bridge(4);
    TEST_ASSERT(converter.get_number_of_bridge()==4);
    converter.set_number_of_bridge(2);
    TEST_ASSERT(converter.get_number_of_bridge()==2);
}

void LCC_TEST::test_set_get_voltage_drop_per_bridge()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LCC_TEST");

    converter.set_voltage_drop_per_bridge_in_kV(0.5);
    TEST_ASSERT(converter.get_voltage_drop_per_bridge_in_kV()==0.5)
}

void LCC_TEST::test_set_get_max_angle()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LCC_TEST");

    converter.set_max_angle_in_deg(88.0);
    TEST_ASSERT(converter.get_max_angle_in_deg()==88.0);
}

void LCC_TEST::test_set_get_min_angle()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LCC_TEST");

    converter.set_min_angle_in_deg(5.5);
    TEST_ASSERT(converter.get_min_angle_in_deg()==5.5);
}

void LCC_TEST::test_set_get_angle()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LCC_TEST");

    converter.set_angle_in_deg(15.0);
    TEST_ASSERT(converter.get_angle_in_deg()==15.0);
}

void LCC_TEST::test_clear()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LCC_TEST");

    converter.set_number_of_bridge(2);
    converter.set_voltage_drop_per_bridge_in_kV(1.0);
    converter.set_max_angle_in_deg(80.0);
    converter.set_min_angle_in_deg(4.0);
    converter.set_angle_in_deg(7.0);

    converter.clear();


    TEST_ASSERT(converter.get_number_of_bridge()==2);
    TEST_ASSERT(converter.get_voltage_drop_per_bridge_in_kV()==0.0);
    TEST_ASSERT(converter.get_max_angle_in_deg()==90.0);
    TEST_ASSERT(converter.get_min_angle_in_deg()==0.0);
    TEST_ASSERT(converter.get_angle_in_deg()==0.0);
}

void LCC_TEST::test_copy_with_operator_equal()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LCC_TEST");

    converter.set_number_of_bridge(2);
    converter.set_voltage_drop_per_bridge_in_kV(1.0);
    converter.set_max_angle_in_deg(80.0);
    converter.set_min_angle_in_deg(4.0);
    converter.set_angle_in_deg(7.0);

    LCC newconverter = converter;

    TEST_ASSERT(converter.get_number_of_bridge()==2);
    TEST_ASSERT(converter.get_voltage_drop_per_bridge_in_kV()==1.0);
    TEST_ASSERT(converter.get_max_angle_in_deg()==80.0);
    TEST_ASSERT(converter.get_min_angle_in_deg()==4.0);
    TEST_ASSERT(converter.get_angle_in_deg()==7.0);
}

void LCC_TEST::test_solve_with_desired_dc_voltage_and_current()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LCC_TEST");

    converter.set_number_of_bridge(2);
    converter.set_voltage_drop_per_bridge_in_kV(0.0);
    converter.set_max_angle_in_deg(80.0);
    converter.set_min_angle_in_deg(7.0);
    converter.set_angle_in_deg(10.0);

    converter.solve_with_desired_dc_voltage_and_current(100.0, 0.5);
    converter.set_dc_current_in_kA(0.5);
    converter.show_steady_state();
}


#endif

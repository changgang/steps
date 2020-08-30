#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif
#include <cmath>
#include "cpptest.h"
#include "header/basic/test_macro.h"
#include "header/device/converter_test.h"
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


CONVERTER_TEST::CONVERTER_TEST() : converter(default_toolkit)
{
    TEST_ADD(CONVERTER_TEST::test_constructor);
    TEST_ADD(CONVERTER_TEST::test_set_get_side);
    TEST_ADD(CONVERTER_TEST::test_set_get_bus);
    TEST_ADD(CONVERTER_TEST::test_set_get_valve_side_bus_name);
    TEST_ADD(CONVERTER_TEST::test_set_get_name);
    TEST_ADD(CONVERTER_TEST::test_set_get_power_percent);
    TEST_ADD(CONVERTER_TEST::test_set_get_dc_voltage);
    TEST_ADD(CONVERTER_TEST::test_set_get_dc_current);
    TEST_ADD(CONVERTER_TEST::test_set_get_transformer_grid_side_base_voltage);
    TEST_ADD(CONVERTER_TEST::test_set_get_transformer_valve_side_base_voltage);
    TEST_ADD(CONVERTER_TEST::test_set_get_transformer_impedance);
    TEST_ADD(CONVERTER_TEST::test_set_get_transformer_admittance);
    TEST_ADD(CONVERTER_TEST::test_set_get_transformer_max_tap);
    TEST_ADD(CONVERTER_TEST::test_set_get_transformer_min_tap);
    TEST_ADD(CONVERTER_TEST::test_set_get_transformer_number_of_taps);
    TEST_ADD(CONVERTER_TEST::test_set_get_transformer_tap);
    TEST_ADD(CONVERTER_TEST::test_clear);
    TEST_ADD(CONVERTER_TEST::test_copy_with_operator_equal);
    TEST_ADD(CONVERTER_TEST::test_get_bus_voltage);
    TEST_ADD(CONVERTER_TEST::test_get_bus_complex_voltage);
    TEST_ADD(CONVERTER_TEST::test_get_bus_base_voltage);
    TEST_ADD(CONVERTER_TEST::test_is_connected_to_bus);
    TEST_ADD(CONVERTER_TEST::test_is_in_area);
    TEST_ADD(CONVERTER_TEST::test_is_in_zone);
}

void CONVERTER_TEST::setup()
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
}

void CONVERTER_TEST::tear_down()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.clear();

    converter.clear();

    show_test_end_information();
}

void CONVERTER_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CONVERTER_TEST");

    TEST_ASSERT(converter.get_side() == RECTIFIER);
    TEST_ASSERT(converter.get_bus()==0);
    TEST_ASSERT(converter.get_valve_side_bus_name()=="");
    TEST_ASSERT(converter.get_name()=="");
    TEST_ASSERT(converter.get_power_percent()==1.0);
    TEST_ASSERT(converter.get_transformer_grid_side_base_voltage_in_kV()==1.0);
    TEST_ASSERT(converter.get_transformer_valve_side_base_voltage_in_kV()==1.0);
    TEST_ASSERT(converter.get_transformer_impedance_in_ohm()==1.0);
    TEST_ASSERT(converter.get_transformer_admittance_in_siemens()==1.0);
    TEST_ASSERT(converter.get_transformer_max_tap_in_pu()==1.0);
    TEST_ASSERT(converter.get_transformer_min_tap_in_pu()==1.0);
    TEST_ASSERT(converter.get_transformer_number_of_taps()==1);
    TEST_ASSERT(converter.get_transformer_tap_in_pu()==1.0);
}

void CONVERTER_TEST::test_set_get_side()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CONVERTER_TEST");

    converter.set_side(INVERTER);
    TEST_ASSERT(converter.get_side()==INVERTER);
    converter.set_side(RECTIFIER);
    TEST_ASSERT(converter.get_side()==RECTIFIER);
}

void CONVERTER_TEST::test_set_get_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CONVERTER_TEST");

    converter.set_bus(1);
    TEST_ASSERT(converter.get_bus()==1);
}

void CONVERTER_TEST::test_set_get_valve_side_bus_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CONVERTER_TEST");

    converter.set_valve_side_bus_name("BUS A");
    TEST_ASSERT(converter.get_valve_side_bus_name()=="BUS A");
}

void CONVERTER_TEST::test_set_get_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CONVERTER_TEST");

    converter.set_name("CONVERTER A");
    TEST_ASSERT(converter.get_name()=="CONVERTER A");
}

void CONVERTER_TEST::test_set_get_power_percent()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CONVERTER_TEST");

    converter.set_power_percent(0.25);
    TEST_ASSERT(converter.get_power_percent()==0.25);
}

void CONVERTER_TEST::test_set_get_dc_voltage()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CONVERTER_TEST");

    converter.set_dc_voltage_in_kV(100.0);
    TEST_ASSERT(converter.get_dc_voltage_in_kV()==100.0);
}

void CONVERTER_TEST::test_set_get_dc_current()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CONVERTER_TEST");

    converter.set_dc_current_in_kA(50.0);
    TEST_ASSERT(converter.get_dc_current_in_kA()==50.0);
}

void CONVERTER_TEST::test_set_get_transformer_grid_side_base_voltage()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CONVERTER_TEST");

    converter.set_transformer_grid_side_base_voltage_in_kV(120.0);
    TEST_ASSERT(converter.get_transformer_grid_side_base_voltage_in_kV()==120.0);
}

void CONVERTER_TEST::test_set_get_transformer_valve_side_base_voltage()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CONVERTER_TEST");

    converter.set_transformer_valve_side_base_voltage_in_kV(230.0);
    TEST_ASSERT(converter.get_transformer_valve_side_base_voltage_in_kV()==230.0);
}

void CONVERTER_TEST::test_set_get_transformer_impedance()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CONVERTER_TEST");

    complex<double> z(0.1, 0.2);
    converter.set_transformer_impedance_in_ohm(z);
    TEST_ASSERT(converter.get_transformer_impedance_in_ohm()==z);
}

void CONVERTER_TEST::test_set_get_transformer_admittance()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CONVERTER_TEST");

    complex<double> y(0.1, 0.2);
    converter.set_transformer_admittance_in_siemens(y);
    TEST_ASSERT(converter.get_transformer_admittance_in_siemens()==y);
}

void CONVERTER_TEST::test_set_get_transformer_max_tap()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CONVERTER_TEST");

    converter.set_transformer_max_tap_in_pu(1.5);
    TEST_ASSERT(converter.get_transformer_max_tap_in_pu()==1.5);
}

void CONVERTER_TEST::test_set_get_transformer_min_tap()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CONVERTER_TEST");

    converter.set_transformer_min_tap_in_pu(0.5);
    TEST_ASSERT(converter.get_transformer_min_tap_in_pu()==0.5);
}

void CONVERTER_TEST::test_set_get_transformer_number_of_taps()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CONVERTER_TEST");

    converter.set_transformer_number_of_taps(5);
    TEST_ASSERT(converter.get_transformer_number_of_taps()==5);
}

void CONVERTER_TEST::test_set_get_transformer_tap()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CONVERTER_TEST");

    converter.set_transformer_tap_in_pu(1.05);
    TEST_ASSERT(converter.get_transformer_tap_in_pu()==1.05);
}

void CONVERTER_TEST::test_clear()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CONVERTER_TEST");

    complex<double> y(0.1, 0.2);
    converter.set_transformer_admittance_in_siemens(y);
    converter.set_transformer_max_tap_in_pu(1.5);
    converter.set_transformer_min_tap_in_pu(0.5);
    converter.set_transformer_number_of_taps(5);

    converter.clear();

    TEST_ASSERT(converter.get_side() == RECTIFIER);
    TEST_ASSERT(converter.get_bus()==0);
    TEST_ASSERT(converter.get_valve_side_bus_name()=="");
    TEST_ASSERT(converter.get_name()=="");
    TEST_ASSERT(converter.get_power_percent()==1.0);
    TEST_ASSERT(converter.get_transformer_grid_side_base_voltage_in_kV()==1.0);
    TEST_ASSERT(converter.get_transformer_valve_side_base_voltage_in_kV()==1.0);
    TEST_ASSERT(converter.get_transformer_impedance_in_ohm()==1.0);
    TEST_ASSERT(converter.get_transformer_admittance_in_siemens()==1.0);
    TEST_ASSERT(converter.get_transformer_max_tap_in_pu()==1.0);
    TEST_ASSERT(converter.get_transformer_min_tap_in_pu()==1.0);
    TEST_ASSERT(converter.get_transformer_number_of_taps()==1);
    TEST_ASSERT(converter.get_transformer_tap_in_pu()==1.0);
}

void CONVERTER_TEST::test_copy_with_operator_equal()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CONVERTER_TEST");

    converter.set_name("CONVERTER A");
    complex<double> y(0.1, 0.2);
    converter.set_transformer_admittance_in_siemens(y);
    converter.set_transformer_max_tap_in_pu(1.5);
    converter.set_transformer_min_tap_in_pu(0.5);
    converter.set_transformer_number_of_taps(5);

    CONVERTER newconverter = converter;
    TEST_ASSERT(newconverter.get_name()==converter.get_name());
    TEST_ASSERT(newconverter.get_transformer_admittance_in_siemens()==converter.get_transformer_admittance_in_siemens());
    TEST_ASSERT(newconverter.get_transformer_max_tap_in_pu()==converter.get_transformer_max_tap_in_pu());
}

void CONVERTER_TEST::test_get_bus_voltage()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CONVERTER_TEST");

    converter.set_bus(1);
    TEST_ASSERT(converter.get_bus_positive_sequence_voltage_in_pu()==1.05);
}

void CONVERTER_TEST::test_get_bus_complex_voltage()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CONVERTER_TEST");

    converter.set_bus(1);
    TEST_ASSERT(converter.get_bus_positive_sequence_complex_voltage_in_pu()==1.05*complex<double>(cos(30.0/180*PI), sin(30.0/180*PI)));
}

void CONVERTER_TEST::test_get_bus_base_voltage()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CONVERTER_TEST");

    converter.set_bus(1);
    TEST_ASSERT(converter.get_bus_base_voltage_in_kV()==110.0);
}

void CONVERTER_TEST::test_is_connected_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CONVERTER_TEST");

    converter.set_bus(1);
    TEST_ASSERT(converter.is_connected_to_bus(1)==true);
    TEST_ASSERT(converter.is_connected_to_bus(2)==false);
}

void CONVERTER_TEST::test_is_in_area()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CONVERTER_TEST");

    converter.set_bus(1);
    TEST_ASSERT(converter.is_in_area(1)==true);
    TEST_ASSERT(converter.is_in_area(2)==false);
}

void CONVERTER_TEST::test_is_in_zone()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CONVERTER_TEST");

    converter.set_bus(1);
    TEST_ASSERT(converter.is_in_zone(1)==true);
    TEST_ASSERT(converter.is_in_zone(2)==false);
}

#endif

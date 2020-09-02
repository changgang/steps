#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif
#include <cmath>
#include "cpptest.h"
#include "header/basic/test_macro.h"
#include "header/device/lcc_station_test.h"
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


LCC_STATION_TEST::LCC_STATION_TEST()
{
    TEST_ADD(LCC_STATION_TEST::test_constructor);
    TEST_ADD(LCC_STATION_TEST::test_set_get_number_of_converters);
    TEST_ADD(LCC_STATION_TEST::test_set_get_total_power_percent_of_converter_station);
    TEST_ADD(LCC_STATION_TEST::test_clear);
    TEST_ADD(LCC_STATION_TEST::test_copy_with_operator_equal);
    TEST_ADD(LCC_STATION_TEST::test_is_connected_to_bus);
    TEST_ADD(LCC_STATION_TEST::test_is_in_area);
    TEST_ADD(LCC_STATION_TEST::test_is_in_zone);
    TEST_ADD(LCC_STATION_TEST::test_solve);
}

void LCC_STATION_TEST::setup()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.set_allowed_max_bus_number(100);

    AREA area(default_toolkit);
    area.set_area_number(1);
    psdb.append_area(area);
    area.set_area_number(2);
    psdb.append_area(area);
    area.set_area_number(3);
    psdb.append_area(area);
    area.set_area_number(4);
    psdb.append_area(area);

    ZONE zone(default_toolkit);
    zone.set_zone_number(1);
    psdb.append_zone(zone);
    zone.set_zone_number(2);
    psdb.append_zone(zone);
    zone.set_zone_number(3);
    psdb.append_zone(zone);
    zone.set_zone_number(4);
    psdb.append_zone(zone);



    BUS bus(default_toolkit);
    bus.set_bus_number(1);
    bus.set_base_voltage_in_kV(110.0);
    bus.set_area_number(1);
    bus.set_zone_number(1);
    psdb.append_bus(bus);

    bus.set_bus_number(2);
    bus.set_area_number(2);
    bus.set_zone_number(2);
    psdb.append_bus(bus);

    bus.set_bus_number(3);
    bus.set_area_number(3);
    bus.set_zone_number(3);
    psdb.append_bus(bus);

    bus.set_bus_number(4);
    bus.set_area_number(3);
    bus.set_zone_number(4);
    psdb.append_bus(bus);
}

void LCC_STATION_TEST::tear_down()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.clear();

    station.set_toolkit(default_toolkit);
    station.clear();

    show_test_end_information();
}

void LCC_STATION_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LCC_STATION_TEST");

    TEST_ASSERT(station.get_name()=="");
    TEST_ASSERT(station.get_converter_side()==RECTIFIER);
    TEST_ASSERT(station.get_converter_side_name()=="RECTIFIER");
    TEST_ASSERT(station.get_number_of_converters()==0);
    TEST_ASSERT(station.get_total_power_percent_of_converter_station()==0.0);
}

void LCC_STATION_TEST::test_set_get_number_of_converters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LCC_STATION_TEST");

    station.set_number_of_converters(2);
    TEST_ASSERT(station.get_number_of_converters()==2);
    station.set_number_of_converters(3);
    TEST_ASSERT(station.get_number_of_converters()==3);
}

void LCC_STATION_TEST::test_set_get_total_power_percent_of_converter_station()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LCC_STATION_TEST");

    station.set_number_of_converters(3);
    LCC* conv = station.get_mutable_converter(0);
    conv->set_power_percent(0.4);
    conv = station.get_mutable_converter(1);
    conv->set_power_percent(0.5);
    conv = station.get_mutable_converter(2);
    conv->set_power_percent(0.2);
    TEST_ASSERT(fabs(station.get_total_power_percent_of_converter_station()-1.1)<FLOAT_EPSILON);
}

void LCC_STATION_TEST::test_clear()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LCC_STATION_TEST");

    station.set_number_of_converters(2);
    station.clear();
    TEST_ASSERT(station.get_number_of_converters()==0)
    TEST_ASSERT(station.get_total_power_percent_of_converter_station()==0);
}

void LCC_STATION_TEST::test_copy_with_operator_equal()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LCC_STATION_TEST");

    station.set_number_of_converters(2);

    LCC_STATION newstation = station;

    TEST_ASSERT(newstation.get_number_of_converters()==2);
}

void LCC_STATION_TEST::test_is_connected_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LCC_STATION_TEST");

    station.set_number_of_converters(3);

    LCC* conv = station.get_mutable_converter(0);
    conv->set_bus(1);
    conv = station.get_mutable_converter(1);
    conv->set_bus(2);
    conv = station.get_mutable_converter(2);
    conv->set_bus(3);

    TEST_ASSERT(station.is_connected_to_bus(1)==true);
    TEST_ASSERT(station.is_connected_to_bus(2)==true);
    TEST_ASSERT(station.is_connected_to_bus(3)==true);
    TEST_ASSERT(station.is_connected_to_bus(4)==false);
}

void LCC_STATION_TEST::test_is_in_area()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LCC_STATION_TEST");

    station.set_number_of_converters(3);

    LCC* conv = station.get_mutable_converter(0);
    conv->set_bus(1);
    conv = station.get_mutable_converter(1);
    conv->set_bus(2);
    conv = station.get_mutable_converter(2);
    conv->set_bus(3);

    TEST_ASSERT(station.is_in_area(1)==true);
    TEST_ASSERT(station.is_in_area(2)==true);
    TEST_ASSERT(station.is_in_area(3)==true);
    TEST_ASSERT(station.is_in_area(4)==false);
}

void LCC_STATION_TEST::test_is_in_zone()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LCC_STATION_TEST");

    station.set_number_of_converters(3);

    LCC* conv = station.get_mutable_converter(0);
    conv->set_bus(1);
    conv = station.get_mutable_converter(1);
    conv->set_bus(2);
    conv = station.get_mutable_converter(2);
    conv->set_bus(3);

    TEST_ASSERT(station.is_in_zone(1)==true);
    TEST_ASSERT(station.is_in_zone(2)==true);
    TEST_ASSERT(station.is_in_zone(3)==true);
    TEST_ASSERT(station.is_in_zone(4)==false);
}

void LCC_STATION_TEST::test_solve()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LCC_STATION_TEST");

}

#endif

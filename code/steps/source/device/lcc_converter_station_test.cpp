#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif
#include <cmath>
#include "cpptest.h"
#include "header/basic/test_macro.h"
#include "header/device/lcc_converter_station_test.h"
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


LCC_CONVERTER_STATION_TEST::LCC_CONVERTER_STATION_TEST()
{
    TEST_ADD(LCC_CONVERTER_STATION_TEST::test_constructor);
    TEST_ADD(LCC_CONVERTER_STATION_TEST::test_set_get_number_of_converters);
    TEST_ADD(LCC_CONVERTER_STATION_TEST::test_set_get_total_power_percent_of_converter_station);
    TEST_ADD(LCC_CONVERTER_STATION_TEST::test_clear);
    TEST_ADD(LCC_CONVERTER_STATION_TEST::test_copy_with_operator_equal);
    TEST_ADD(LCC_CONVERTER_STATION_TEST::test_is_connected_to_bus);
    TEST_ADD(LCC_CONVERTER_STATION_TEST::test_is_in_area);
    TEST_ADD(LCC_CONVERTER_STATION_TEST::test_is_in_zone);
}

void LCC_CONVERTER_STATION_TEST::setup()
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

void LCC_CONVERTER_STATION_TEST::tear_down()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.clear();

    station.set_toolkit(default_toolkit);
    station.clear();

    show_test_end_information();
}

void LCC_CONVERTER_STATION_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LCC_CONVERTER_STATION_TEST");

    TEST_ASSERT(station.get_name()=="");
    TEST_ASSERT(station.get_converter_side()==RECTIFIER);
    TEST_ASSERT(station.get_converter_side_name()=="RECTIFIER");
}

void LCC_CONVERTER_STATION_TEST::test_set_get_number_of_converters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LCC_CONVERTER_STATION_TEST");

}

void LCC_CONVERTER_STATION_TEST::test_set_get_total_power_percent_of_converter_station()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LCC_CONVERTER_STATION_TEST");

}

void LCC_CONVERTER_STATION_TEST::test_clear()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LCC_CONVERTER_STATION_TEST");

}

void LCC_CONVERTER_STATION_TEST::test_copy_with_operator_equal()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LCC_CONVERTER_STATION_TEST");

}

void LCC_CONVERTER_STATION_TEST::test_is_connected_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LCC_CONVERTER_STATION_TEST");

}

void LCC_CONVERTER_STATION_TEST::test_is_in_area()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LCC_CONVERTER_STATION_TEST");

}

void LCC_CONVERTER_STATION_TEST::test_is_in_zone()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LCC_CONVERTER_STATION_TEST");

}

void LCC_CONVERTER_STATION_TEST::test_solve()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LCC_CONVERTER_STATION_TEST");

}

#endif

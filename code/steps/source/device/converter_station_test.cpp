#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif
#include <cmath>
#include "cpptest.h"
#include "header/basic/test_macro.h"
#include "header/device/converter_station_test.h"
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


CONVERTER_STATION_TEST::CONVERTER_STATION_TEST()
{
    TEST_ADD(CONVERTER_STATION_TEST::test_constructor);
    TEST_ADD(CONVERTER_STATION_TEST::test_set_get_name);
    TEST_ADD(CONVERTER_STATION_TEST::test_set_get_converter_side);
    TEST_ADD(CONVERTER_STATION_TEST::test_clear);
    TEST_ADD(CONVERTER_STATION_TEST::test_copy_with_operator_equal);
}

void CONVERTER_STATION_TEST::setup()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.set_allowed_max_bus_number(100);
}

void CONVERTER_STATION_TEST::tear_down()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.clear();

    station.set_toolkit(default_toolkit);
    station.clear();

    show_test_end_information();
}

void CONVERTER_STATION_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CONVERTER_STATION_TEST");

    TEST_ASSERT(station.get_name()=="");
    TEST_ASSERT(station.get_converter_side()==RECTIFIER);
    TEST_ASSERT(station.get_converter_side_name()=="RECTIFIER");
}

void CONVERTER_STATION_TEST::test_set_get_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CONVERTER_STATION_TEST");

    station.set_name("station A");
    TEST_ASSERT(station.get_name()=="station A");
}

void CONVERTER_STATION_TEST::test_set_get_converter_side()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CONVERTER_STATION_TEST");

    station.set_converter_side(INVERTER);
    TEST_ASSERT(station.get_converter_side()==INVERTER);
    station.set_converter_side(RECTIFIER);
    TEST_ASSERT(station.get_converter_side()==RECTIFIER);
}

void CONVERTER_STATION_TEST::test_clear()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CONVERTER_STATION_TEST");

    station.set_converter_side(INVERTER);

    station.clear();

    TEST_ASSERT(station.get_name()=="");
    TEST_ASSERT(station.get_converter_side()==RECTIFIER);
    TEST_ASSERT(station.get_converter_side_name()=="RECTIFIER");
}

void CONVERTER_STATION_TEST::test_copy_with_operator_equal()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CONVERTER_STATION_TEST");

    station.set_name("station B");
    station.set_converter_side(INVERTER);

    CONVERTER_STATION newstation = station;

    TEST_ASSERT(newstation.get_name()==station.get_name());
    TEST_ASSERT(newstation.get_converter_side()==station.get_converter_side());
}

#endif

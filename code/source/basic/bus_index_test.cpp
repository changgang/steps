#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif
#include <cmath>
#include "cpptest.h"

#include "header/basic/bus_index_test.h"
#include "header/basic/constants.h"
#include "header/basic/utility.h"
#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cstdio>

using namespace std;


BUS_INDEX_TEST::BUS_INDEX_TEST()
{
    TEST_ADD(BUS_INDEX_TEST::test_constructor);
    TEST_ADD(BUS_INDEX_TEST::test_set_get_bus_index);
    TEST_ADD(BUS_INDEX_TEST::test_capacity);
    TEST_ADD(BUS_INDEX_TEST::test_clear_index);
    TEST_ADD(BUS_INDEX_TEST::test_get_index_of_bus_and_operator_bracket);
}

void BUS_INDEX_TEST::setup()
{
}

void BUS_INDEX_TEST::tear_down()
{
    bus_index.clear();
    show_test_end_information();
}

void BUS_INDEX_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BUS_INDEX_TEST");

    TEST_ASSERT(bus_index.get_max_bus_number()==0);
 }

void BUS_INDEX_TEST::test_capacity()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BUS_INDEX_TEST");

    bus_index.set_max_bus_number(0);
    TEST_ASSERT(bus_index.get_max_bus_number()==0);
    bus_index.set_max_bus_number(100);
    TEST_ASSERT(bus_index.get_max_bus_number()==100);
    bus_index.set_max_bus_number(1000000);
    TEST_ASSERT(bus_index.get_max_bus_number()==1000000);
}

void BUS_INDEX_TEST::test_set_get_bus_index()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BUS_INDEX_TEST");

    bus_index.set_max_bus_number(100000);
    TEST_ASSERT(bus_index.get_max_bus_number()==100000);
    bus_index.set_bus_with_index(1, 10);
    bus_index.set_bus_with_index(3, 20);
    bus_index.set_bus_with_index(1000, 23);
    bus_index.set_bus_with_index(100000000, 24);
    TEST_ASSERT(bus_index.get_index_of_bus(1)==10);
    TEST_ASSERT(bus_index.get_index_of_bus(3)==20);
    TEST_ASSERT(bus_index.get_index_of_bus(1000)==23);
    TEST_ASSERT(bus_index.get_index_of_bus(100000000)==INDEX_NOT_EXIST);
}

void BUS_INDEX_TEST::test_clear_index()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BUS_INDEX_TEST");

    bus_index.set_max_bus_number(100000);
    bus_index.set_bus_with_index(1, 10);
    bus_index.set_bus_with_index(3, 20);
    bus_index.set_bus_with_index(1000, 23);
    bus_index.set_bus_with_index(100000000, 24);
    bus_index.clear();
    size_t max_bus_number = bus_index.get_max_bus_number();
    for(size_t i=1; i<=max_bus_number; ++i)
        TEST_ASSERT(bus_index.get_index_of_bus(i)==INDEX_NOT_EXIST);
}

void BUS_INDEX_TEST::test_get_index_of_bus_and_operator_bracket()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BUS_INDEX_TEST");

    bus_index.set_max_bus_number(100000);
    bus_index.set_bus_with_index(1, 10);
    bus_index.set_bus_with_index(3, 20);
    bus_index.set_bus_with_index(1000, 23);
    bus_index.set_bus_with_index(100000000, 24);
    TEST_ASSERT(bus_index.get_index_of_bus(1)==10);
    TEST_ASSERT(bus_index[1]==10);
    TEST_ASSERT(bus_index.get_index_of_bus(3)==20);
    TEST_ASSERT(bus_index[3]==20);
    TEST_ASSERT(bus_index.get_index_of_bus(1000)==23);
    TEST_ASSERT(bus_index[1000]==23);
    TEST_ASSERT(bus_index.get_index_of_bus(100000000)==INDEX_NOT_EXIST);
    TEST_ASSERT(bus_index[100000000]==INDEX_NOT_EXIST);
}

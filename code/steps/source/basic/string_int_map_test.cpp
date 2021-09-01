#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif
#include <cmath>
#include "cpptest.h"
#include "header/basic/test_macro.h"
#include "header/basic/string_int_map_test.h"
#include "header/basic/constants.h"
#include "header/basic/utility.h"
#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cstdio>

#ifdef ENABLE_STEPS_TEST
using namespace std;


STR_INT_MAP_TEST::STR_INT_MAP_TEST()
{
    TEST_ADD(STR_INT_MAP_TEST::test_constructor);
    TEST_ADD(STR_INT_MAP_TEST::test_map_add_and_index);
}

void STR_INT_MAP_TEST::setup()
{
    test_map.clear();
}

void STR_INT_MAP_TEST::tear_down()
{
    test_map.clear();

    show_test_end_information();
}

void STR_INT_MAP_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"STR_INT_MAP_TEST");

    TEST_ASSERT(test_map.get_map_size() == 1);

    TEST_ASSERT(test_map[""] == 0);
}

void STR_INT_MAP_TEST::test_map_add_and_index()
{
    show_test_information_for_function_of_class(__FUNCTION__,"STR_INT_MAP_TEST");

    TEST_ASSERT(test_map.get_map_size() == 1);

    test_map.add_new_string("A");
    test_map.add_new_string("Z");
    test_map.add_new_string("#1");

    TEST_ASSERT(test_map.get_map_size()==4);

    TEST_ASSERT(test_map[""] == 0);
    TEST_ASSERT(test_map["A"] == 1);
    TEST_ASSERT(test_map["Z"] == 2);
    TEST_ASSERT(test_map["#1"] == 3);

    test_map.add_new_string("Z");

    TEST_ASSERT(test_map.get_map_size()==4);

    TEST_ASSERT(test_map[""] == 0);
    TEST_ASSERT(test_map["A"] == 1);
    TEST_ASSERT(test_map["Z"] == 2);
    TEST_ASSERT(test_map["#1"] == 3);

    TEST_ASSERT(test_map["#2"] == INDEX_NOT_EXIST);
    TEST_ASSERT(test_map["B"] == INDEX_NOT_EXIST);
}
#endif

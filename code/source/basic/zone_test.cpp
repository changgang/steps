#include "header/basic/zone_test.h"
#include "header/basic/utility.h"

#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <cstdio>

using namespace std;

ZONE_TEST::ZONE_TEST()
{
    TEST_ADD(ZONE_TEST::test_constructor);
    TEST_ADD(ZONE_TEST::test_set_get_zone_number);
    TEST_ADD(ZONE_TEST::test_set_get_zone_name);
    TEST_ADD(ZONE_TEST::test_is_valid);
    TEST_ADD(ZONE_TEST::test_clear);
    TEST_ADD(ZONE_TEST::test_copy_with_operator_equal);
}

void ZONE_TEST::setup()
{
    db = get_default_power_system_database_pointer();

    zone = new ZONE(db);
}

void ZONE_TEST::tear_down()
{
    delete zone;

    db->clear_database();

    show_test_end_information();
}

void ZONE_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"ZONE_TEST");

    TEST_ASSERT(zone->get_zone_number()==0);
    TEST_ASSERT(zone->get_zone_name()=="");
}

void ZONE_TEST::test_set_get_zone_number()
{
    show_test_information_for_function_of_class(__FUNCTION__,"ZONE_TEST");

    TEST_ASSERT(zone->get_zone_number()==0);
    zone->set_zone_number(1);
    TEST_ASSERT(zone->get_zone_number()==1);
    zone->set_zone_number(2);
    TEST_ASSERT(zone->get_zone_number()==2);
    zone->set_zone_number(0);
    TEST_ASSERT(zone->get_zone_number()==0);
}

void ZONE_TEST::test_set_get_zone_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"ZONE_TEST");

    TEST_ASSERT(zone->get_zone_name()=="");
    zone->set_zone_name("zone 1");
    TEST_ASSERT(zone->get_zone_name()=="zone 1");
}

void ZONE_TEST::test_is_valid()
{
    show_test_information_for_function_of_class(__FUNCTION__,"ZONE_TEST");

    TEST_ASSERT(zone->is_valid()==false);
    zone->set_zone_number(1);
    TEST_ASSERT(zone->is_valid()==true);
}
void ZONE_TEST::test_clear()
{
    show_test_information_for_function_of_class(__FUNCTION__,"ZONE_TEST");

    zone->set_zone_number(1);
    zone->set_zone_name("zone 1");
    zone->clear();

    test_constructor();
}

void ZONE_TEST::test_copy_with_operator_equal()
{
    show_test_information_for_function_of_class(__FUNCTION__,"ZONE_TEST");

    zone->set_zone_number(1);
    zone->set_zone_name("zone 1");

    ZONE newzone = (*zone);

    TEST_ASSERT(newzone.get_zone_number()==1);
    TEST_ASSERT(newzone.get_zone_name()=="zone 1");
}

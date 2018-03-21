#include <cstdlib>
#include <cstring>
#include <iostream>

#include "header/basic/fault_location_test.h"

using namespace std;

FAULT_LOCATION_TEST::FAULT_LOCATION_TEST()
{
    TEST_ADD(FAULT_LOCATION_TEST::test_constructor);
    TEST_ADD(FAULT_LOCATION_TEST::test_set_get_fault_location_in_pu);
    TEST_ADD(FAULT_LOCATION_TEST::test_operator_less);
    TEST_ADD(FAULT_LOCATION_TEST::test_operator_equal);
}

void FAULT_LOCATION_TEST::setup()
{
    fault_location = new FAULT_LOCATION;
}

void FAULT_LOCATION_TEST::tear_down()
{
    delete fault_location;
    fault_location = NULL;
}

void FAULT_LOCATION_TEST::test_constructor()
{
    TEST_ASSERT(fault_location->get_fault_location_in_pu()==0.0);
}

void FAULT_LOCATION_TEST::test_set_get_fault_location_in_pu()
{
    double location = 0.2;
    fault_location->set_fault_location_in_pu(location);
    TEST_ASSERT(fault_location->get_fault_location_in_pu()==location);
}

void FAULT_LOCATION_TEST::test_operator_less()
{
    double location = 0.2;
    fault_location->set_fault_location_in_pu(location);

    FAULT_LOCATION fault_location2;

    fault_location2.set_fault_location_in_pu(location-0.002);
    TEST_ASSERT(fault_location2<(*fault_location));

    fault_location2.set_fault_location_in_pu(location-0.0001);
    TEST_ASSERT(not(fault_location2<(*fault_location)));
}


void FAULT_LOCATION_TEST::test_operator_equal()
{
    double location = 0.2;
    fault_location->set_fault_location_in_pu(location);

    FAULT_LOCATION fault_location2;

    fault_location2.set_fault_location_in_pu(location-0.001);
    TEST_ASSERT(fault_location2==(*fault_location));

    fault_location2.set_fault_location_in_pu(location+0.001);
    TEST_ASSERT(fault_location2==(*fault_location));

    fault_location2.set_fault_location_in_pu(location-0.00111);
    TEST_ASSERT(not(fault_location2==(*fault_location)));

    fault_location2.set_fault_location_in_pu(location+0.00111);
    TEST_ASSERT(not(fault_location2==(*fault_location)));
}

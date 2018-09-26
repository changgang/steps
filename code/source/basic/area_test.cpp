#include "header/basic/area_test.h"
#include "header/basic/utility.h"

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cstdio>

using namespace std;

AREA_TEST::AREA_TEST()
{
    TEST_ADD(AREA_TEST::test_constructor);
    TEST_ADD(AREA_TEST::test_set_get_area_number);
    TEST_ADD(AREA_TEST::test_set_get_area_name);
    TEST_ADD(AREA_TEST::test_set_get_area_swing_bus);
    TEST_ADD(AREA_TEST::test_set_get_expected_power_leaving_area_in_MW);
    TEST_ADD(AREA_TEST::test_set_get_area_power_mismatch_tolerance_in_MW);
    TEST_ADD(AREA_TEST::test_is_valid);
    TEST_ADD(AREA_TEST::test_clear);
    TEST_ADD(AREA_TEST::test_copy_with_operator_equal);
}

void AREA_TEST::setup()
{
    db = get_default_power_system_database();
    db->set_allowed_max_bus_number(100);

    BUS bus;
    bus.set_bus_number(1);
    bus.set_base_voltage_in_kV(110.0);
    bus.set_bus_type(PQ_TYPE);
    bus.set_area_number(1);
    db->append_bus(bus);

    bus.set_bus_number(2);
    bus.set_bus_type(PV_TYPE);
    db->append_bus(bus);

    bus.set_bus_number(3);
    bus.set_bus_type(SLACK_TYPE);
    db->append_bus(bus);

    area = new AREA(db);
}

void AREA_TEST::tear_down()
{
    delete area;

    db->clear_database();

    show_test_end_information();
}

void AREA_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"AREA_TEST");

    TEST_ASSERT(area->get_area_number()==0);
    TEST_ASSERT(area->get_area_name()=="");
    TEST_ASSERT(fabs(area->get_expected_power_leaving_area_in_MW()-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(area->get_area_swing_bus()==0);
    TEST_ASSERT(fabs(area->get_area_power_mismatch_tolerance_in_MW()-10.0)<FLOAT_EPSILON);
}

void AREA_TEST::test_set_get_area_number()
{
    show_test_information_for_function_of_class(__FUNCTION__,"AREA_TEST");

    // area number is size_t,
    // when constructing area, 0 is set
    // however, when setting with setter function,
    // 0 is not allowed.
    // if 0 is given, error message will be displayed, and 0 area number will be set to indicate invalid area
    area->set_area_number(1);
    TEST_ASSERT(area->get_area_number()==1);
    area->set_area_number(2);
    TEST_ASSERT(area->get_area_number()==2);
    area->set_area_number(0);
    TEST_ASSERT(area->get_area_number()==0);
}
void AREA_TEST::test_set_get_area_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"AREA_TEST");

    area->set_area_name("area 1");
    TEST_ASSERT(area->get_area_name()=="area 1");
}

void AREA_TEST::test_set_get_area_swing_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"AREA_TEST");

    // area swing bus must be valid.
    // 0 or other invalid buses will trigger error message, and
    // 0 swing bus will be set to indicate invalid area
    area->set_area_number(1);
    area->set_area_swing_bus(1);
    TEST_ASSERT(area->get_area_swing_bus()==0);
    area->set_area_swing_bus(2);
    TEST_ASSERT(area->get_area_swing_bus()==2);
    area->set_area_swing_bus(3);
    TEST_ASSERT(area->get_area_swing_bus()==3);
    area->set_area_swing_bus(0);
    TEST_ASSERT(area->get_area_swing_bus()==3);
    area->set_area_swing_bus(4);
    TEST_ASSERT(area->get_area_swing_bus()==0);
}

void AREA_TEST::test_set_get_expected_power_leaving_area_in_MW()
{
    show_test_information_for_function_of_class(__FUNCTION__,"AREA_TEST");

    TEST_ASSERT(fabs(area->get_expected_power_leaving_area_in_MW()-0.0)<FLOAT_EPSILON);
    area->set_expected_power_leaving_area_in_MW(100.0);
    TEST_ASSERT(fabs(area->get_expected_power_leaving_area_in_MW()-100.0)<FLOAT_EPSILON);
    area->set_expected_power_leaving_area_in_MW(-100.0);
    TEST_ASSERT(fabs(area->get_expected_power_leaving_area_in_MW()-(-100.0))<FLOAT_EPSILON);
}
void AREA_TEST::test_set_get_area_power_mismatch_tolerance_in_MW()
{
    show_test_information_for_function_of_class(__FUNCTION__,"AREA_TEST");

    TEST_ASSERT(fabs(area->get_area_power_mismatch_tolerance_in_MW()-10.0)<FLOAT_EPSILON);
    area->set_area_power_mismatch_tolerance_in_MW(-15.0);
    TEST_ASSERT(fabs(area->get_area_power_mismatch_tolerance_in_MW()-10.0)<FLOAT_EPSILON);
}
void AREA_TEST::test_is_valid()
{
    show_test_information_for_function_of_class(__FUNCTION__,"AREA_TEST");

    TEST_ASSERT(area->is_valid()==false);
    area->set_area_number(1);
    area->set_area_swing_bus(2);
    TEST_ASSERT(area->is_valid()==true);
    area->set_area_swing_bus(0);
    TEST_ASSERT(area->is_valid()==true);
    area->set_area_number(0);
    TEST_ASSERT(area->is_valid()==false);
}

void AREA_TEST::test_clear()
{
    show_test_information_for_function_of_class(__FUNCTION__,"AREA_TEST");

    area->set_area_number(1);
    area->set_area_name("area 1");
    area->set_area_swing_bus(2);
    area->set_expected_power_leaving_area_in_MW(100.0);
    area->set_area_power_mismatch_tolerance_in_MW(20.0);

    area->clear();

    TEST_ASSERT(area->get_area_number()==0);
    TEST_ASSERT(area->get_area_name()=="");
    TEST_ASSERT(fabs(area->get_expected_power_leaving_area_in_MW()-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(area->get_area_swing_bus()==0);
    TEST_ASSERT(fabs(area->get_area_power_mismatch_tolerance_in_MW()-10.0)<FLOAT_EPSILON);
}
void AREA_TEST::test_copy_with_operator_equal()
{
    show_test_information_for_function_of_class(__FUNCTION__,"AREA_TEST");

    area->set_area_number(1);
    area->set_area_name("area 1");
    area->set_area_swing_bus(2);
    area->set_expected_power_leaving_area_in_MW(100.0);
    area->set_area_power_mismatch_tolerance_in_MW(20.0);

    AREA newarea = (*area);

    TEST_ASSERT(newarea.get_area_number()==1);
    TEST_ASSERT(newarea.get_area_name()=="area 1");
    TEST_ASSERT(newarea.get_area_swing_bus()==2);
    TEST_ASSERT(fabs(newarea.get_expected_power_leaving_area_in_MW()-100.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(newarea.get_area_power_mismatch_tolerance_in_MW()-20.0)<FLOAT_EPSILON);
}

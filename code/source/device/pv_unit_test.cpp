#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif
#include <cmath>
#include "thirdparty/cpptest/cpptest.h"

#include "header/device/pv_unit_test.h"
#include "header/basic/constants.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <cstdio>

using namespace std;

PV_UNIT_TEST::PV_UNIT_TEST()
{
    TEST_ADD(PV_UNIT_TEST::test_constructor);
    TEST_ADD(PV_UNIT_TEST::test_set_get_unit_bus);
    TEST_ADD(PV_UNIT_TEST::test_set_get_source_impedance);
    TEST_ADD(PV_UNIT_TEST::test_copy_with_operator_equal);
    TEST_ADD(PV_UNIT_TEST::test_get_device_id);
}

void PV_UNIT_TEST::setup()
{
    db = get_default_power_system_database();

    db->set_system_base_power_in_MVA(100.0);
    db->set_allowed_max_bus_number(100);

    BUS bus;
    bus.set_bus_number(1);
    bus.set_bus_name("bus 1");
    bus.set_base_voltage_in_kV(35.0);
    db->append_bus(bus);

    bus.set_bus_number(2);
    bus.set_bus_name("bus 2");
    db->append_bus(bus);

    pv_unit = new PV_UNIT(db);
}

void PV_UNIT_TEST::tear_down()
{
    delete pv_unit;
    pv_unit = NULL;

    db->clear_database();

    show_test_end_information();
}

void PV_UNIT_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PV_UNIT_TEST");

    TEST_ASSERT(pv_unit->get_unit_bus()==0);
    TEST_ASSERT(pv_unit->get_identifier()=="");
    TEST_ASSERT(pv_unit->get_status()==false);
    TEST_ASSERT(fabs(pv_unit->get_mbase_in_MVA()-db->get_system_base_power_in_MVA())<FLOAT_EPSILON);
    TEST_ASSERT(fabs(pv_unit->get_p_generation_in_MW()-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(pv_unit->get_q_generation_in_MVar()-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(pv_unit->get_p_max_in_MW()-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(pv_unit->get_p_min_in_MW()-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(pv_unit->get_q_max_in_MVar()-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(pv_unit->get_q_min_in_MVar()-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(pv_unit->get_voltage_to_regulate_in_pu()-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(pv_unit->get_bus_to_regulate()==0);
    TEST_ASSERT(pv_unit->get_source_impedance_in_pu()==0.0);
}

void PV_UNIT_TEST::test_set_get_unit_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PV_UNIT_TEST");

    pv_unit->set_unit_bus(1);
    TEST_ASSERT(pv_unit->get_unit_bus()==1);
    pv_unit->set_unit_bus(2);
    TEST_ASSERT(pv_unit->get_unit_bus()==2);
    pv_unit->set_unit_bus(0);
    TEST_ASSERT(pv_unit->get_unit_bus()==0);
}

void PV_UNIT_TEST::test_set_get_source_impedance()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PV_UNIT_TEST");

    complex<double> z(0.0, 0.01);
    pv_unit->set_source_impedance_in_pu(z);
    TEST_ASSERT(pv_unit->get_source_impedance_in_pu()==z);
}


void PV_UNIT_TEST::test_copy_with_operator_equal()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PV_UNIT_TEST");

    pv_unit->set_unit_bus(1);
    pv_unit->set_identifier("1#");
    pv_unit->set_status(true);
    pv_unit->set_mbase_in_MVA(120.0);

    PV_UNIT newsource = (*pv_unit);

    TEST_ASSERT(newsource.get_unit_bus()==1);
    TEST_ASSERT(newsource.get_identifier()=="1#");
    TEST_ASSERT(newsource.get_status()==true);
    TEST_ASSERT(fabs(newsource.get_mbase_in_MVA()-120.0)<FLOAT_EPSILON);
}

void PV_UNIT_TEST::test_get_device_id()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PV_UNIT_TEST");

    pv_unit->set_unit_bus(1);
    pv_unit->set_identifier("1#");

    DEVICE_ID did = pv_unit->get_device_id();

    DEVICE_ID did2;
    did2.set_device_type("PV UNIT");
    TERMINAL terminal;
    terminal.append_bus(1);
    did2.set_device_terminal(terminal);
    did2.set_device_identifier("1#");

    TEST_ASSERT(did==did2);

    terminal.clear();
    terminal.append_bus(2);
    did2.set_device_terminal(terminal);

    TEST_ASSERT(did!=did2);
}


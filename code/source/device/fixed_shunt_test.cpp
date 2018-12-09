#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif
#include <cmath>
#include "cpptest.h"

#include "header/steps_namespace.h"
#include "header/basic/utility.h"
#include "header/device/fixed_shunt_test.h"
#include <cstring>
#include <istream>
#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace std;


FIXED_SHUNT_TEST::FIXED_SHUNT_TEST()
{
    TEST_ADD(FIXED_SHUNT_TEST::test_constructor);
    TEST_ADD(FIXED_SHUNT_TEST::test_set_get_shunt_bus);
    TEST_ADD(FIXED_SHUNT_TEST::test_set_get_identifier);
    TEST_ADD(FIXED_SHUNT_TEST::test_set_get_status);
    TEST_ADD(FIXED_SHUNT_TEST::test_set_get_nominal_impedance_shunt);

    TEST_ADD(FIXED_SHUNT_TEST::test_is_valid);
    TEST_ADD(FIXED_SHUNT_TEST::test_clear);
    TEST_ADD(FIXED_SHUNT_TEST::test_copy_with_operator_equal);
    TEST_ADD(FIXED_SHUNT_TEST::test_is_connected_to_bus);

    TEST_ADD(FIXED_SHUNT_TEST::test_get_device_id);

    TEST_ADD(FIXED_SHUNT_TEST::test_get_actual_impedance_shunt);
}

void FIXED_SHUNT_TEST::setup()
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    psdb.set_system_base_power_in_MVA(100.0);
    psdb.set_allowed_max_bus_number(10);

    BUS bus;
    bus.set_bus_number(1);
    bus.set_base_voltage_in_kV(100.0);
    bus.set_bus_type(PQ_TYPE);
    bus.set_voltage_in_pu(1.1);
    psdb.append_bus(bus);
}
void FIXED_SHUNT_TEST::tear_down()
{
    fixed_shunt.clear();

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    psdb.clear_database();

    show_test_end_information();
}

void FIXED_SHUNT_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"FIXED_SHUNT_TEST");

    TEST_ASSERT(fixed_shunt.get_shunt_bus()==0);
    TEST_ASSERT(fixed_shunt.get_identifier()=="");
    TEST_ASSERT(fixed_shunt.get_status()==false);
    TEST_ASSERT(fixed_shunt.get_nominal_impedance_shunt_in_MVA()==0.0);
}

void FIXED_SHUNT_TEST::test_set_get_shunt_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"FIXED_SHUNT_TEST");

    fixed_shunt.set_shunt_bus(1);
    TEST_ASSERT(fixed_shunt.get_shunt_bus()==1);
    fixed_shunt.set_shunt_bus(0);
    TEST_ASSERT(fixed_shunt.get_shunt_bus()==0);
}

void FIXED_SHUNT_TEST::test_set_get_identifier()
{
    show_test_information_for_function_of_class(__FUNCTION__,"FIXED_SHUNT_TEST");

    fixed_shunt.set_identifier("1#");
    TEST_ASSERT(fixed_shunt.get_identifier()=="1#");
}

void FIXED_SHUNT_TEST::test_set_get_status()
{
    show_test_information_for_function_of_class(__FUNCTION__,"FIXED_SHUNT_TEST");

    fixed_shunt.set_status(true);
    TEST_ASSERT(fixed_shunt.get_status()==true);
    fixed_shunt.set_status(false);
    TEST_ASSERT(fixed_shunt.get_status()==false);
}

void FIXED_SHUNT_TEST::test_set_get_nominal_impedance_shunt()
{
    show_test_information_for_function_of_class(__FUNCTION__,"FIXED_SHUNT_TEST");

    complex<double> s(10, 300);
    fixed_shunt.set_nominal_impedance_shunt_in_MVA(s);
    TEST_ASSERT(fixed_shunt.get_nominal_impedance_shunt_in_MVA()==s);
    TEST_ASSERT(abs(fixed_shunt.get_nominal_impedance_shunt_in_pu()-conj(100.0/s))<FLOAT_EPSILON);
    TEST_ASSERT(abs(fixed_shunt.get_nominal_admittance_shunt_in_pu()-conj(s)/100.0)<FLOAT_EPSILON);
}

void FIXED_SHUNT_TEST::test_is_valid()
{
    show_test_information_for_function_of_class(__FUNCTION__,"FIXED_SHUNT_TEST");

    TEST_ASSERT(fixed_shunt.is_valid()==false);

    fixed_shunt.set_shunt_bus(1);
    TEST_ASSERT(fixed_shunt.is_valid()==true);
}

void FIXED_SHUNT_TEST::test_clear()
{
    show_test_information_for_function_of_class(__FUNCTION__,"FIXED_SHUNT_TEST");

    fixed_shunt.set_shunt_bus(1);
    fixed_shunt.set_identifier("1#");
    fixed_shunt.set_status(true);

    fixed_shunt.clear();

    test_constructor();
}

void FIXED_SHUNT_TEST::test_copy_with_operator_equal()
{
    show_test_information_for_function_of_class(__FUNCTION__,"FIXED_SHUNT_TEST");

    fixed_shunt.set_shunt_bus(1);
    fixed_shunt.set_identifier("1#");
    fixed_shunt.set_status(false);
    fixed_shunt.set_nominal_impedance_shunt_in_MVA(complex<double>(10.0, 300.0));

    FIXED_SHUNT fixed_shunt2 = fixed_shunt;

    TEST_ASSERT(fixed_shunt2.get_shunt_bus()==1);
    TEST_ASSERT(fixed_shunt2.get_identifier()=="1#");
    TEST_ASSERT(fixed_shunt2.get_status()==false);
    TEST_ASSERT(fixed_shunt2.get_nominal_impedance_shunt_in_MVA()==complex<double>(10.0, 300.0));
}

void FIXED_SHUNT_TEST::test_is_connected_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"FIXED_SHUNT_TEST");

    fixed_shunt.set_shunt_bus(1);
    TEST_ASSERT(fixed_shunt.is_connected_to_bus(1)==true);
    TEST_ASSERT(fixed_shunt.is_connected_to_bus(2)==false);
}

void FIXED_SHUNT_TEST::test_get_device_id()
{
    show_test_information_for_function_of_class(__FUNCTION__,"FIXED_SHUNT_TEST");

    fixed_shunt.set_shunt_bus(1);
    fixed_shunt.set_identifier("F1");

    DEVICE_ID did = fixed_shunt.get_device_id();

    DEVICE_ID did2;
    did2.set_device_type("FIXED SHUNT");
    TERMINAL terminal;
    terminal.append_bus(1);
    did2.set_device_terminal(terminal);
    did2.set_device_identifier("F1");

    TEST_ASSERT(did==did2);

    terminal.clear();
    terminal.append_bus(2);
    did2.set_device_terminal(terminal);

    TEST_ASSERT(did!=did2);
}

void FIXED_SHUNT_TEST::test_get_actual_impedance_shunt()
{
    show_test_information_for_function_of_class(__FUNCTION__,"FIXED_SHUNT_TEST");

    fixed_shunt.set_shunt_bus(1);
    fixed_shunt.set_status(true);
    complex<double> s_Z(10.0, 300.0);
    fixed_shunt.set_nominal_impedance_shunt_in_MVA(s_Z);

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    BUS* bus = psdb.get_bus(fixed_shunt.get_shunt_bus());

    bus->set_voltage_in_pu(0.95);

    complex<double> s = s_Z*0.95*0.95;
    TEST_ASSERT(abs(fixed_shunt.get_actual_impedance_shunt_in_MVA()-s)<FLOAT_EPSILON);
}

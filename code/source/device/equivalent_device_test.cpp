#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif
#include <cmath>
#include "cpptest.h"
#include "header/basic/test_macro.h"

#include "header/steps_namespace.h"
#include "header/basic/utility.h"
#include "header/device/equivalent_device_test.h"
#include <cstring>
#include <istream>
#include <iostream>
#include <cstdio>
#include <cstdlib>

#ifdef ENABLE_STEPS_TEST
using namespace std;


EQUIVALENT_DEVICE_TEST::EQUIVALENT_DEVICE_TEST()
{
    TEST_ADD(EQUIVALENT_DEVICE_TEST::test_constructor);
    TEST_ADD(EQUIVALENT_DEVICE_TEST::test_set_get_equivalent_device_bus);
    TEST_ADD(EQUIVALENT_DEVICE_TEST::test_set_get_identifier);
    TEST_ADD(EQUIVALENT_DEVICE_TEST::test_set_get_status);
    TEST_ADD(EQUIVALENT_DEVICE_TEST::test_set_get_equivalent_voltage_source_status);
    TEST_ADD(EQUIVALENT_DEVICE_TEST::test_set_get_equivalent_load_status);
    TEST_ADD(EQUIVALENT_DEVICE_TEST::test_set_get_equivalent_voltage_source_voltage);
    TEST_ADD(EQUIVALENT_DEVICE_TEST::test_set_get_equivalent_voltage_source_impedance);
    TEST_ADD(EQUIVALENT_DEVICE_TEST::test_set_get_equivalent_nominal_constant_power_load);
    TEST_ADD(EQUIVALENT_DEVICE_TEST::test_set_get_equivalent_nominal_constant_current_load);
    TEST_ADD(EQUIVALENT_DEVICE_TEST::test_set_get_equivalent_nominal_constant_impedance_load);

    TEST_ADD(EQUIVALENT_DEVICE_TEST::test_is_valid);
    TEST_ADD(EQUIVALENT_DEVICE_TEST::test_clear);
    TEST_ADD(EQUIVALENT_DEVICE_TEST::test_copy_with_operator_equal);
    TEST_ADD(EQUIVALENT_DEVICE_TEST::test_is_connected_to_bus);

    TEST_ADD(EQUIVALENT_DEVICE_TEST::test_get_device_id);
}

void EQUIVALENT_DEVICE_TEST::setup()
{
    edevice.set_toolkit(default_toolkit);

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.set_system_base_power_in_MVA(100.0);
    psdb.set_allowed_max_bus_number(10);

    BUS bus;
    bus.set_bus_number(1);
    bus.set_base_voltage_in_kV(100.0);
    bus.set_bus_type(PQ_TYPE);
    bus.set_voltage_in_pu(1.1);
    psdb.append_bus(bus);
}
void EQUIVALENT_DEVICE_TEST::tear_down()
{
    edevice.clear();

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.clear();

    show_test_end_information();
}

void EQUIVALENT_DEVICE_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"EQUIVALENT_DEVICE_TEST");

    TEST_ASSERT(edevice.get_equivalent_device_bus()==0);
    TEST_ASSERT(edevice.get_identifier()=="");
    TEST_ASSERT(edevice.get_status()==false);
    TEST_ASSERT(edevice.get_equivalent_voltage_source_status()==false);
    TEST_ASSERT(edevice.get_equivalent_load_status()==false);
    TEST_ASSERT(edevice.get_equivalent_voltage_source_voltage_in_pu()==0.0);
    TEST_ASSERT(edevice.get_equivalent_voltage_source_impedance_in_pu()==0.0);
    TEST_ASSERT(edevice.get_equivalent_nominal_constant_power_load_in_MVA()==0.0);
    TEST_ASSERT(edevice.get_equivalent_nominal_constant_current_load_in_MVA()==0.0);
    TEST_ASSERT(edevice.get_equivalent_nominal_constant_impedance_load_in_MVA()==0.0);
}

void EQUIVALENT_DEVICE_TEST::test_set_get_equivalent_device_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"EQUIVALENT_DEVICE_TEST");

    edevice.set_equivalent_device_bus(1);
    TEST_ASSERT(edevice.get_equivalent_device_bus()==1);
    edevice.set_equivalent_device_bus(0);
    TEST_ASSERT(edevice.get_equivalent_device_bus()==0);
}

void EQUIVALENT_DEVICE_TEST::test_set_get_identifier()
{
    show_test_information_for_function_of_class(__FUNCTION__,"EQUIVALENT_DEVICE_TEST");

    edevice.set_identifier("1#");
    TEST_ASSERT(edevice.get_identifier()=="1#");
}

void EQUIVALENT_DEVICE_TEST::test_set_get_status()
{
    show_test_information_for_function_of_class(__FUNCTION__,"EQUIVALENT_DEVICE_TEST");

    edevice.set_status(true);
    TEST_ASSERT(edevice.get_status()==true);
    edevice.set_status(false);
    TEST_ASSERT(edevice.get_status()==false);
}

void EQUIVALENT_DEVICE_TEST::test_set_get_equivalent_voltage_source_status()
{
    show_test_information_for_function_of_class(__FUNCTION__,"EQUIVALENT_DEVICE_TEST");

    edevice.set_equivalent_voltage_source_status(true);
    TEST_ASSERT(edevice.get_equivalent_voltage_source_status()==true);
    edevice.set_equivalent_voltage_source_status(false);
    TEST_ASSERT(edevice.get_equivalent_voltage_source_status()==false);
}

void EQUIVALENT_DEVICE_TEST::test_set_get_equivalent_load_status()
{
    show_test_information_for_function_of_class(__FUNCTION__,"EQUIVALENT_DEVICE_TEST");

    edevice.set_equivalent_load_status(true);
    TEST_ASSERT(edevice.get_equivalent_load_status()==true);
    edevice.set_equivalent_load_status(false);
    TEST_ASSERT(edevice.get_equivalent_load_status()==false);
}

void EQUIVALENT_DEVICE_TEST::test_set_get_equivalent_voltage_source_voltage()
{
    show_test_information_for_function_of_class(__FUNCTION__,"EQUIVALENT_DEVICE_TEST");

    edevice.set_equivalent_voltage_source_voltage_in_pu(0.0);
    TEST_ASSERT(edevice.get_equivalent_voltage_source_voltage_in_pu()==0.0);
    edevice.set_equivalent_voltage_source_voltage_in_pu(complex<double>(1.0, 0.5));
    TEST_ASSERT(edevice.get_equivalent_voltage_source_voltage_in_pu()==complex<double>(1.0, 0.5));
}

void EQUIVALENT_DEVICE_TEST::test_set_get_equivalent_voltage_source_impedance()
{
    show_test_information_for_function_of_class(__FUNCTION__,"EQUIVALENT_DEVICE_TEST");

    edevice.set_equivalent_voltage_source_impedance_in_pu(0.0);
    TEST_ASSERT(edevice.get_equivalent_voltage_source_impedance_in_pu()==0.0);
    edevice.set_equivalent_voltage_source_impedance_in_pu(complex<double>(1.0, 0.5));
    TEST_ASSERT(edevice.get_equivalent_voltage_source_impedance_in_pu()==complex<double>(1.0, 0.5));
}

void EQUIVALENT_DEVICE_TEST::test_set_get_equivalent_nominal_constant_power_load()
{
    show_test_information_for_function_of_class(__FUNCTION__,"EQUIVALENT_DEVICE_TEST");

    edevice.set_equivalent_nominal_constant_power_load_in_MVA(0.0);
    TEST_ASSERT(edevice.get_equivalent_nominal_constant_power_load_in_MVA()==0.0);
    edevice.set_equivalent_nominal_constant_power_load_in_MVA(complex<double>(1.0, 0.5));
    TEST_ASSERT(edevice.get_equivalent_nominal_constant_power_load_in_MVA()==complex<double>(1.0, 0.5));
}

void EQUIVALENT_DEVICE_TEST::test_set_get_equivalent_nominal_constant_current_load()
{
    show_test_information_for_function_of_class(__FUNCTION__,"EQUIVALENT_DEVICE_TEST");

    edevice.set_equivalent_nominal_constant_current_load_in_MVA(0.0);
    TEST_ASSERT(edevice.get_equivalent_nominal_constant_current_load_in_MVA()==0.0);
    edevice.set_equivalent_nominal_constant_current_load_in_MVA(complex<double>(1.0, 0.5));
    TEST_ASSERT(edevice.get_equivalent_nominal_constant_current_load_in_MVA()==complex<double>(1.0, 0.5));
}

void EQUIVALENT_DEVICE_TEST::test_set_get_equivalent_nominal_constant_impedance_load()
{
    show_test_information_for_function_of_class(__FUNCTION__,"EQUIVALENT_DEVICE_TEST");

    edevice.set_equivalent_nominal_constant_impedance_load_in_MVA(0.0);
    TEST_ASSERT(edevice.get_equivalent_nominal_constant_impedance_load_in_MVA()==0.0);
    edevice.set_equivalent_nominal_constant_impedance_load_in_MVA(complex<double>(1.0, 0.5));
    TEST_ASSERT(edevice.get_equivalent_nominal_constant_impedance_load_in_MVA()==complex<double>(1.0, 0.5));
}

void EQUIVALENT_DEVICE_TEST::test_is_valid()
{
    show_test_information_for_function_of_class(__FUNCTION__,"EQUIVALENT_DEVICE_TEST");

    TEST_ASSERT(edevice.is_valid()==false);

    edevice.set_equivalent_device_bus(1);
    TEST_ASSERT(edevice.is_valid()==true);
}

void EQUIVALENT_DEVICE_TEST::test_clear()
{
    show_test_information_for_function_of_class(__FUNCTION__,"EQUIVALENT_DEVICE_TEST");

    edevice.set_equivalent_device_bus(1);
    edevice.set_identifier("1#");
    edevice.set_status(true);

    edevice.clear();

    test_constructor();
}

void EQUIVALENT_DEVICE_TEST::test_copy_with_operator_equal()
{
    show_test_information_for_function_of_class(__FUNCTION__,"EQUIVALENT_DEVICE_TEST");

    edevice.set_equivalent_device_bus(1);
    edevice.set_identifier("1#");
    edevice.set_status(false);
    edevice.set_equivalent_voltage_source_status(true);
    edevice.set_equivalent_load_status(true);
    edevice.set_equivalent_voltage_source_voltage_in_pu(1.0);
    edevice.set_equivalent_voltage_source_impedance_in_pu(complex<double>(0.0, 0.5));
    edevice.set_equivalent_nominal_constant_power_load_in_MVA(complex<double>(100.0, 50.0));
    edevice.set_equivalent_nominal_constant_current_load_in_MVA(complex<double>(10.0, 5.0));
    edevice.set_equivalent_nominal_constant_impedance_load_in_MVA(complex<double>(1.0, 0.5));

    EQUIVALENT_DEVICE edevice2 = edevice;

    TEST_ASSERT(edevice2.get_equivalent_device_bus()==1);
    TEST_ASSERT(edevice2.get_identifier()=="1#");
    TEST_ASSERT(edevice2.get_status()==false);
    TEST_ASSERT(edevice2.get_equivalent_voltage_source_status()==true);
    TEST_ASSERT(edevice2.get_equivalent_load_status()==true);
    TEST_ASSERT(edevice2.get_equivalent_voltage_source_voltage_in_pu()==1.0);
    TEST_ASSERT(edevice2.get_equivalent_voltage_source_impedance_in_pu()==complex<double>(0.0, 0.5));
    TEST_ASSERT(edevice2.get_equivalent_nominal_constant_power_load_in_MVA()==complex<double>(100.0, 50.0));
    TEST_ASSERT(edevice2.get_equivalent_nominal_constant_current_load_in_MVA()==complex<double>(10.0, 5.0));
    TEST_ASSERT(edevice2.get_equivalent_nominal_constant_impedance_load_in_MVA()==complex<double>(1.0, 0.5));
}

void EQUIVALENT_DEVICE_TEST::test_is_connected_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"EQUIVALENT_DEVICE_TEST");

    edevice.set_equivalent_device_bus(1);
    TEST_ASSERT(edevice.is_connected_to_bus(1)==true);
    TEST_ASSERT(edevice.is_connected_to_bus(2)==false);
}

void EQUIVALENT_DEVICE_TEST::test_get_device_id()
{
    show_test_information_for_function_of_class(__FUNCTION__,"EQUIVALENT_DEVICE_TEST");

    edevice.set_equivalent_device_bus(1);
    edevice.set_identifier("E1");

    DEVICE_ID did = edevice.get_device_id();

    DEVICE_ID did2;
    did2.set_device_type("EQUIVALENT DEVICE");
    TERMINAL terminal;
    terminal.append_bus(1);
    did2.set_device_terminal(terminal);
    did2.set_device_identifier("E1");

    TEST_ASSERT(did==did2);

    terminal.clear();
    terminal.append_bus(2);
    did2.set_device_terminal(terminal);

    TEST_ASSERT(did!=did2);
}

#endif

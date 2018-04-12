#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif
#include <cmath>
#include "thirdparty/cpptest/cpptest.h"

#include "header/device/pe_source_test.h"
#include "header/basic/constants.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cstdio>

using namespace std;

PE_SOURCE_TEST::PE_SOURCE_TEST()
{
    TEST_ADD(PE_SOURCE_TEST::test_constructor);
    TEST_ADD(PE_SOURCE_TEST::test_set_get_source_bus);
    TEST_ADD(PE_SOURCE_TEST::test_set_get_source_impedance);
    TEST_ADD(PE_SOURCE_TEST::test_copy_with_operator_equal);
    TEST_ADD(PE_SOURCE_TEST::test_get_device_id);
    TEST_ADD(PE_SOURCE_TEST::test_set_get_source_type);
}

void PE_SOURCE_TEST::setup()
{
    db = new POWER_SYSTEM_DATABASE;

    db->set_system_base_power_in_MVA(100.0);
    db->set_allowed_max_bus_number(100);

    BUS bus(db);
    bus.set_bus_number(1);
    bus.set_bus_name("bus 1");
    bus.set_base_voltage_in_kV(35.0);
    db->append_bus(bus);

    bus.set_bus_number(2);
    bus.set_bus_name("bus 2");
    db->append_bus(bus);

    pesource = new PE_SOURCE(db);
}

void PE_SOURCE_TEST::tear_down()
{
    delete pesource;
    pesource = NULL;

    delete db;
    db=NULL;

    show_test_end_information();
}

void PE_SOURCE_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PE_SOURCE_TEST");

    TEST_ASSERT(pesource->get_source_bus()==0);
    TEST_ASSERT(pesource->get_identifier()=="");
    TEST_ASSERT(pesource->get_status()==false);
    TEST_ASSERT(fabs(pesource->get_mbase_in_MVA()-db->get_system_base_power_in_MVA())<FLOAT_EPSILON);
    TEST_ASSERT(fabs(pesource->get_p_generation_in_MW()-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(pesource->get_q_generation_in_MVar()-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(pesource->get_p_max_in_MW()-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(pesource->get_p_min_in_MW()-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(pesource->get_q_max_in_MVar()-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(pesource->get_q_min_in_MVar()-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(pesource->get_voltage_to_regulate_in_pu()-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(pesource->get_bus_to_regulate()==0);
    TEST_ASSERT(pesource->get_source_impedance_in_pu()==0.0);
    TEST_ASSERT(pesource->get_regulating_mode()==REGULATING_PV);
}

void PE_SOURCE_TEST::test_set_get_source_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PE_SOURCE_TEST");

    pesource->set_source_bus(1);
    TEST_ASSERT(pesource->get_source_bus()==1);
    pesource->set_source_bus(2);
    TEST_ASSERT(pesource->get_source_bus()==2);
    pesource->set_source_bus(0);
    TEST_ASSERT(pesource->get_source_bus()==0);
}

void PE_SOURCE_TEST::test_set_get_source_impedance()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PE_SOURCE_TEST");

    complex<double> z(0.0, 0.01);
    pesource->set_source_impedance_in_pu(z);
    TEST_ASSERT(pesource->get_source_impedance_in_pu()==z);
}


void PE_SOURCE_TEST::test_copy_with_operator_equal()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PE_SOURCE_TEST");

    pesource->set_source_bus(1);
    pesource->set_identifier("1#");
    pesource->set_status(true);
    pesource->set_mbase_in_MVA(120.0);

    PE_SOURCE newsource = (*pesource);

    TEST_ASSERT(newsource.get_source_bus()==1);
    TEST_ASSERT(newsource.get_identifier()=="1#");
    TEST_ASSERT(newsource.get_status()==true);
    TEST_ASSERT(fabs(newsource.get_mbase_in_MVA()-120.0)<FLOAT_EPSILON);
}

void PE_SOURCE_TEST::test_get_device_id()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PE_SOURCE_TEST");

    pesource->set_source_bus(1);
    pesource->set_identifier("1#");

    DEVICE_ID did = pesource->get_device_id();

    DEVICE_ID did2;
    did2.set_device_type("PE SOURCE");
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


void PE_SOURCE_TEST::test_set_get_source_type()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PE_SOURCE_TEST");

    TEST_ASSERT(pesource->get_source_type()=="");

    pesource->set_source_bus(1);
    pesource->set_identifier("1#");

    DEVICE_ID did = pesource->get_device_id();

    DEVICE_ID did2;
    did2.set_device_type("PE SOURCE");
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

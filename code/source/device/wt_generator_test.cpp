#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif
#include <cmath>
#include "thirdparty/cpptest/cpptest.h"

#include "header/device/wt_generator_test.h"
#include "header/basic/constants.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <cstdio>

using namespace std;

WT_GENERATOR_TEST::WT_GENERATOR_TEST()
{
    TEST_ADD(WT_GENERATOR_TEST::test_constructor);
    TEST_ADD(WT_GENERATOR_TEST::test_set_get_generator_bus);
    TEST_ADD(WT_GENERATOR_TEST::test_set_get_source_impedance);
    TEST_ADD(WT_GENERATOR_TEST::test_copy_with_operator_equal);
    TEST_ADD(WT_GENERATOR_TEST::test_get_device_id);
}

void WT_GENERATOR_TEST::setup()
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

    wt_generator = new WT_GENERATOR(db);
}

void WT_GENERATOR_TEST::tear_down()
{
    delete wt_generator;
    wt_generator = NULL;

    delete db;
    db=NULL;

    show_test_end_information();
}

void WT_GENERATOR_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"WT_GENERATOR_TEST");

    TEST_ASSERT(wt_generator->get_generator_bus()==0);
    TEST_ASSERT(wt_generator->get_identifier()=="");
    TEST_ASSERT(wt_generator->get_status()==false);
    TEST_ASSERT(fabs(wt_generator->get_mbase_in_MVA()-db->get_system_base_power_in_MVA())<FLOAT_EPSILON);
    TEST_ASSERT(fabs(wt_generator->get_p_generation_in_MW()-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(wt_generator->get_q_generation_in_MVar()-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(wt_generator->get_p_max_in_MW()-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(wt_generator->get_p_min_in_MW()-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(wt_generator->get_q_max_in_MVar()-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(wt_generator->get_q_min_in_MVar()-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(wt_generator->get_voltage_to_regulate_in_pu()-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(wt_generator->get_bus_to_regulate()==0);
    TEST_ASSERT(wt_generator->get_source_impedance_in_pu()==0.0);
    TEST_ASSERT(wt_generator->get_regulating_mode()==REGULATING_PV);
}

void WT_GENERATOR_TEST::test_set_get_generator_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"WT_GENERATOR_TEST");

    wt_generator->set_generator_bus(1);
    TEST_ASSERT(wt_generator->get_generator_bus()==1);
    wt_generator->set_generator_bus(2);
    TEST_ASSERT(wt_generator->get_generator_bus()==2);
    wt_generator->set_generator_bus(0);
    TEST_ASSERT(wt_generator->get_generator_bus()==0);
}

void WT_GENERATOR_TEST::test_set_get_source_impedance()
{
    show_test_information_for_function_of_class(__FUNCTION__,"WT_GENERATOR_TEST");

    complex<double> z(0.0, 0.01);
    wt_generator->set_source_impedance_in_pu(z);
    TEST_ASSERT(wt_generator->get_source_impedance_in_pu()==z);
}


void WT_GENERATOR_TEST::test_copy_with_operator_equal()
{
    show_test_information_for_function_of_class(__FUNCTION__,"WT_GENERATOR_TEST");

    wt_generator->set_generator_bus(1);
    wt_generator->set_identifier("1#");
    wt_generator->set_status(true);
    wt_generator->set_mbase_in_MVA(120.0);

    WT_GENERATOR newsource = (*wt_generator);

    TEST_ASSERT(newsource.get_generator_bus()==1);
    TEST_ASSERT(newsource.get_identifier()=="1#");
    TEST_ASSERT(newsource.get_status()==true);
    TEST_ASSERT(fabs(newsource.get_mbase_in_MVA()-120.0)<FLOAT_EPSILON);
}

void WT_GENERATOR_TEST::test_get_device_id()
{
    show_test_information_for_function_of_class(__FUNCTION__,"WT_GENERATOR_TEST");

    wt_generator->set_generator_bus(1);
    wt_generator->set_identifier("1#");

    DEVICE_ID did = wt_generator->get_device_id();

    DEVICE_ID did2;
    did2.set_device_type("WT GENERATOR");
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


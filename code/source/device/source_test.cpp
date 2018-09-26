#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif
#include <cmath>
#include "thirdparty/cpptest/cpptest.h"

#include "header/device/source_test.h"
#include "header/basic/constants.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <cstdio>
using namespace std;

SOURCE_TEST::SOURCE_TEST()
{
    TEST_ADD(SOURCE_TEST::test_constructor);
    TEST_ADD(SOURCE_TEST::test_set_get_source_bus);
    TEST_ADD(SOURCE_TEST::test_set_get_identifier);
    TEST_ADD(SOURCE_TEST::test_set_get_status);
    TEST_ADD(SOURCE_TEST::test_set_get_mbase);
    TEST_ADD(SOURCE_TEST::test_set_get_p_generation);
    TEST_ADD(SOURCE_TEST::test_set_get_q_generation);
    TEST_ADD(SOURCE_TEST::test_set_get_p_max_min);
    TEST_ADD(SOURCE_TEST::test_set_get_q_max_min);
    TEST_ADD(SOURCE_TEST::test_set_get_v_to_regulate);
    TEST_ADD(SOURCE_TEST::test_set_get_bus_to_regulate);
    TEST_ADD(SOURCE_TEST::test_set_get_source_impedance);
    TEST_ADD(SOURCE_TEST::test_is_valid);
    TEST_ADD(SOURCE_TEST::test_clear);
    TEST_ADD(SOURCE_TEST::test_is_connected_to_bus);
}

void SOURCE_TEST::setup()
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

    generator = new GENERATOR(db);
}

void SOURCE_TEST::tear_down()
{
    delete generator;
    generator = NULL;

    db->clear_database();

    show_test_end_information();
}

void SOURCE_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SOURCE_TEST");
    TEST_ASSERT(generator->get_generator_bus()==0);
    TEST_ASSERT(generator->get_identifier()=="");
    TEST_ASSERT(generator->get_status()==false);
    TEST_ASSERT(generator->get_mbase_in_MVA()==db->get_system_base_power_in_MVA());
    TEST_ASSERT(generator->get_p_generation_in_MW()==0.0);
    TEST_ASSERT(generator->get_q_generation_in_MVar()==0.0);
    TEST_ASSERT(generator->get_p_max_in_MW()==0.0);
    TEST_ASSERT(generator->get_p_min_in_MW()==0.0);
    TEST_ASSERT(generator->get_q_max_in_MVar()==0.0);
    TEST_ASSERT(generator->get_q_min_in_MVar()==0.0);
    TEST_ASSERT(generator->get_voltage_to_regulate_in_pu()==1.0);
    TEST_ASSERT(generator->get_bus_to_regulate()==0);
    TEST_ASSERT(generator->get_generator_impedance_in_pu()==0.0);
}


void SOURCE_TEST::test_set_get_source_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SOURCE_TEST");

    generator->set_source_bus(1);
    TEST_ASSERT(generator->get_source_bus()==1);
    generator->set_source_bus(2);
    TEST_ASSERT(generator->get_source_bus()==2);
    generator->set_source_bus(0);
    TEST_ASSERT(generator->get_source_bus()==0);
}

void SOURCE_TEST::test_set_get_identifier()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SOURCE_TEST");

    generator->set_identifier("1#");
    TEST_ASSERT(generator->get_identifier()=="1#");
}

void SOURCE_TEST::test_set_get_status()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SOURCE_TEST");

    generator->set_status(true);
    TEST_ASSERT(generator->get_status()==true);
    generator->set_status(false);
    TEST_ASSERT(generator->get_status()==false);
}

void SOURCE_TEST::test_set_get_mbase()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SOURCE_TEST");

    generator->set_mbase_in_MVA(200.0);
    TEST_ASSERT(generator->get_mbase_in_MVA()==200.0);
    generator->set_mbase_in_MVA(0.0);
    TEST_ASSERT(generator->get_mbase_in_MVA()==db->get_system_base_power_in_MVA());
    generator->set_mbase_in_MVA(-100.0);
    TEST_ASSERT(generator->get_mbase_in_MVA()==db->get_system_base_power_in_MVA());
}

void SOURCE_TEST::test_set_get_p_generation()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SOURCE_TEST");

    generator->set_p_generation_in_MW(10.0);
    TEST_ASSERT(generator->get_p_generation_in_MW()==10.0);
}

void SOURCE_TEST::test_set_get_q_generation()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SOURCE_TEST");

    generator->set_q_generation_in_MVar(10.0);
    TEST_ASSERT(generator->get_q_generation_in_MVar()==10.0);
}

void SOURCE_TEST::test_set_get_p_max_min()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SOURCE_TEST");

    generator->set_p_max_in_MW(100.0);
    TEST_ASSERT(generator->get_p_max_in_MW()==100.0);
    generator->set_p_min_in_MW(10.0);
    TEST_ASSERT(generator->get_p_min_in_MW()==10.0);
}

void SOURCE_TEST::test_set_get_q_max_min()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SOURCE_TEST");

    generator->set_q_max_in_MVar(100.0);
    TEST_ASSERT(generator->get_q_max_in_MVar()==100.0);
    generator->set_q_min_in_MVar(-10.0);
    TEST_ASSERT(fabs(generator->get_q_min_in_MVar()-(-10.0))<FLOAT_EPSILON);
}

void SOURCE_TEST::test_set_get_v_to_regulate()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SOURCE_TEST");

    generator->set_voltage_to_regulate_in_pu(0.99);
    TEST_ASSERT(generator->get_voltage_to_regulate_in_pu()==0.99);
    generator->set_voltage_to_regulate_in_pu(0.0);
    TEST_ASSERT(generator->get_voltage_to_regulate_in_pu()==1.0);
    generator->set_voltage_to_regulate_in_pu(-1.0);
    TEST_ASSERT(generator->get_voltage_to_regulate_in_pu()==1.0);
}

void SOURCE_TEST::test_set_get_bus_to_regulate()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SOURCE_TEST");

    generator->set_generator_bus(1);
    generator->set_bus_to_regulate(1);
    TEST_ASSERT(generator->get_bus_to_regulate()==1);
    generator->set_bus_to_regulate(2);
    TEST_ASSERT(generator->get_bus_to_regulate()==generator->get_generator_bus());
    generator->set_bus_to_regulate(3);
    TEST_ASSERT(generator->get_bus_to_regulate()==generator->get_generator_bus());
    generator->set_bus_to_regulate(0);
    TEST_ASSERT(generator->get_bus_to_regulate()==generator->get_generator_bus());
}

void SOURCE_TEST::test_set_get_source_impedance()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SOURCE_TEST");

    complex<double> z(0.0, 0.01);
    generator->set_source_impedance_in_pu(z);
    TEST_ASSERT(generator->get_source_impedance_in_pu()==z);
}

void SOURCE_TEST::test_is_valid()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SOURCE_TEST");

    TEST_ASSERT(generator->is_valid()==false);

    generator->set_generator_bus(1);
    TEST_ASSERT(generator->is_valid()==true);
}

void SOURCE_TEST::test_clear()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SOURCE_TEST");

    generator->set_generator_bus(1);
    generator->set_identifier("1#");
    generator->set_status(true);

    generator->clear();

    test_constructor();
}

void SOURCE_TEST::test_is_connected_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SOURCE_TEST");

    generator->set_generator_bus(1);
    TEST_ASSERT(generator->is_connected_to_bus(1)==true);
    TEST_ASSERT(generator->is_connected_to_bus(2)==false);
}

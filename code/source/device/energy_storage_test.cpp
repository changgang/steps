#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif
#include <cmath>
#include "cpptest.h"

#include "header/device/engergy_storage_test.h"
#include "header/basic/constants.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

#include "header/model/energy_storage_model/estr0.h"

#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <cstdio>
using namespace std;

ENERGY_STORAGE_TEST::ENERGY_STORAGE_TEST()
{
    TEST_ADD(ENERGY_STORAGE_TEST::test_constructor);

    TEST_ADD(ENERGY_STORAGE_TEST::test_set_get_energy_storage_bus);
    TEST_ADD(ENERGY_STORAGE_TEST::test_set_get_energy_storage_impedance);
    TEST_ADD(ENERGY_STORAGE_TEST::test_copy_with_operator_equal);
    TEST_ADD(ENERGY_STORAGE_TEST::test_get_device_id);

    TEST_ADD(ENERGY_STORAGE_TEST::test_set_get_energy_storage_model);
}

void ENERGY_STORAGE_TEST::setup()
{
    energy_storage.set_toolkit(default_toolkit);

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.set_system_base_power_in_MVA(100.0);
    psdb.set_allowed_max_bus_number(100);

    BUS bus;
    bus.set_bus_number(1);
    bus.set_bus_name("bus 1");
    bus.set_base_voltage_in_kV(35.0);
    psdb.append_bus(bus);

    bus.set_bus_number(2);
    bus.set_bus_name("bus 2");
    psdb.append_bus(bus);
}

void ENERGY_STORAGE_TEST::tear_down()
{
    energy_storage.clear();

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.clear();

    show_test_end_information();
}

void ENERGY_STORAGE_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"ENERGY_STORAGE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    TEST_ASSERT(energy_storage.get_energy_storage_bus()==0);
    TEST_ASSERT(energy_storage.get_identifier()=="");
    TEST_ASSERT(energy_storage.get_status()==false);
    TEST_ASSERT(energy_storage.get_mbase_in_MVA()==psdb.get_system_base_power_in_MVA());
    TEST_ASSERT(energy_storage.get_p_generation_in_MW()==0.0);
    TEST_ASSERT(energy_storage.get_q_generation_in_MVar()==0.0);
    TEST_ASSERT(energy_storage.get_p_max_in_MW()==0.0);
    TEST_ASSERT(energy_storage.get_p_min_in_MW()==0.0);
    TEST_ASSERT(energy_storage.get_q_max_in_MVar()==0.0);
    TEST_ASSERT(energy_storage.get_q_min_in_MVar()==0.0);
    TEST_ASSERT(energy_storage.get_voltage_to_regulate_in_pu()==1.0);
    TEST_ASSERT(energy_storage.get_bus_to_regulate()==0);
}

void ENERGY_STORAGE_TEST::test_set_get_energy_storage_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"ENERGY_STORAGE_TEST");

    energy_storage.set_energy_storage_bus(1);
    TEST_ASSERT(energy_storage.get_energy_storage_bus()==1);
    energy_storage.set_energy_storage_bus(2);
    TEST_ASSERT(energy_storage.get_energy_storage_bus()==2);
    energy_storage.set_energy_storage_bus(0);
    TEST_ASSERT(energy_storage.get_energy_storage_bus()==0);
}

void ENERGY_STORAGE_TEST::test_set_get_energy_storage_impedance()
{
    show_test_information_for_function_of_class(__FUNCTION__,"ENERGY_STORAGE_TEST");

    complex<double> z(0.1, 0.01);
    energy_storage.set_energy_storage_impedance_in_pu(z);
    TEST_ASSERT(steps_fast_complex_abs(energy_storage.get_energy_storage_impedance_in_pu()-z)<FLOAT_EPSILON);
}


void ENERGY_STORAGE_TEST::test_copy_with_operator_equal()
{
    show_test_information_for_function_of_class(__FUNCTION__,"ENERGY_STORAGE_TEST");

    energy_storage.set_energy_storage_bus(1);
    energy_storage.set_identifier("1#");
    energy_storage.set_status(true);
    energy_storage.set_mbase_in_MVA(120.0);

    ENERGY_STORAGE newenergy_storage = energy_storage;

    TEST_ASSERT(newenergy_storage.get_energy_storage_bus()==1);
    TEST_ASSERT(newenergy_storage.get_identifier()=="1#");
    TEST_ASSERT(newenergy_storage.get_status()==true);
    TEST_ASSERT(newenergy_storage.get_mbase_in_MVA()==120.0);
}

void ENERGY_STORAGE_TEST::test_get_device_id()
{
    show_test_information_for_function_of_class(__FUNCTION__,"ENERGY_STORAGE_TEST");

    energy_storage.set_energy_storage_bus(1);
    energy_storage.set_identifier("1#");

    DEVICE_ID did = energy_storage.get_device_id();

    DEVICE_ID did2;
    did2.set_device_type("ENERGY STORAGE");
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


void ENERGY_STORAGE_TEST::test_set_get_energy_storage_model()
{
    show_test_information_for_function_of_class(__FUNCTION__,"ENERGY_STORAGE_TEST");

    energy_storage.set_energy_storage_bus(1);
    energy_storage.set_identifier("1#");
    energy_storage.set_status(true);

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.append_energy_storage(energy_storage);

    DEVICE_ID did = energy_storage.get_device_id();

    ENERGY_STORAGE* estorage = psdb.get_energy_storage(did);

    ESTR0 model;

    model.set_Tp1_in_s(1.0);
    model.set_Tp2_in_s(2.0);
    model.set_Tr_in_s(3.0);

    estorage->set_model(&model);


    ENERGY_STORAGE_MODEL* modelptr = estorage->get_energy_storage_model();
    TEST_ASSERT(modelptr!=NULL);

    TEST_ASSERT(fabs(((ESTR0*)modelptr)->get_Tp1_in_s()-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(((ESTR0*)modelptr)->get_Tp2_in_s()-2.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(((ESTR0*)modelptr)->get_Tr_in_s()-3.0)<FLOAT_EPSILON);
}

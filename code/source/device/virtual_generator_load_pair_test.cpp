#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif
#include <cmath>
#include "thirdparty/cpptest/cpptest.h"

#include "header/steps_namespace.h"
#include "header/basic/utility.h"
#include "header/device/virtual_generator_load_pair_test.h"
#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <cstdio>

using namespace std;


VIRTUAL_GENERATOR_LOAD_PAIR_TEST::VIRTUAL_GENERATOR_LOAD_PAIR_TEST()
{
    TEST_ADD(VIRTUAL_GENERATOR_LOAD_PAIR_TEST::test_constructor);
    TEST_ADD(VIRTUAL_GENERATOR_LOAD_PAIR_TEST::test_set_get_generator_power_system_name);
    TEST_ADD(VIRTUAL_GENERATOR_LOAD_PAIR_TEST::test_set_get_load_power_system_name);
    TEST_ADD(VIRTUAL_GENERATOR_LOAD_PAIR_TEST::test_set_get_generator_power_system_index);
    TEST_ADD(VIRTUAL_GENERATOR_LOAD_PAIR_TEST::test_set_get_load_power_system_index);
    TEST_ADD(VIRTUAL_GENERATOR_LOAD_PAIR_TEST::test_set_get_virtual_generator);
    TEST_ADD(VIRTUAL_GENERATOR_LOAD_PAIR_TEST::test_set_get_virtual_load);
    TEST_ADD(VIRTUAL_GENERATOR_LOAD_PAIR_TEST::test_update);
    TEST_ADD(VIRTUAL_GENERATOR_LOAD_PAIR_TEST::test_is_valid);
    TEST_ADD(VIRTUAL_GENERATOR_LOAD_PAIR_TEST::test_clear);

    TEST_ADD(VIRTUAL_GENERATOR_LOAD_PAIR_TEST::test_is_connected_to_bus);
    TEST_ADD(VIRTUAL_GENERATOR_LOAD_PAIR_TEST::test_get_device_id);
}

void VIRTUAL_GENERATOR_LOAD_PAIR_TEST::setup()
{
    POWER_SYSTEM_DATABASE* psdb;
    psdb = STEPS::power_system_db+0;
    psdb->set_allowed_max_bus_number(100);
    psdb->set_system_name("System 0");

    BUS bus(psdb);
    bus.set_bus_number(1);
    bus.set_base_voltage_in_kV(100.0);
    bus.set_bus_type(PQ_TYPE);
    bus.set_voltage_in_pu(1.1);
    psdb->append_bus(bus);
    bus.set_bus_number(2);
    psdb->append_bus(bus);
    bus.set_bus_number(3);
    psdb->append_bus(bus);

    GENERATOR generator(psdb);
    generator.set_generator_bus(1);
    generator.set_identifier("#1");
    generator.set_status(true);
    generator.set_p_generation_in_MW(2000.0);
    generator.set_q_generation_in_MVar(1000.0);
    psdb->append_generator(generator);

    psdb = STEPS::power_system_db+1;
    psdb->set_allowed_max_bus_number(100);
    psdb->set_system_name("System 1");

    bus.set_power_system_database(psdb);

    bus.set_bus_number(1);
    bus.set_base_voltage_in_kV(100.0);
    bus.set_bus_type(PQ_TYPE);
    bus.set_voltage_in_pu(1.1);
    psdb->append_bus(bus);
    bus.set_voltage_in_pu(1.05);
    bus.set_angle_in_deg(10.0);
    bus.set_bus_number(2);
    psdb->append_bus(bus);
    bus.set_voltage_in_pu(1.0);
    bus.set_angle_in_deg(0.0);
    bus.set_bus_number(3);
    psdb->append_bus(bus);

    LOAD load(psdb);
    load.set_load_bus(2);
    load.set_identifier("#2");
    load.set_status(true);
    psdb->append_load(load);

    virtual_gen_load_pair = new VIRTUAL_GENERATOR_LOAD_PAIR;
}

void VIRTUAL_GENERATOR_LOAD_PAIR_TEST::tear_down()
{
    POWER_SYSTEM_DATABASE* psdb;

    psdb = STEPS::power_system_db+0;
    psdb->clear_database();

    psdb = STEPS::power_system_db+1;
    psdb->clear_database();

    delete virtual_gen_load_pair;
    virtual_gen_load_pair = NULL;

    show_test_end_information();
}

void VIRTUAL_GENERATOR_LOAD_PAIR_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VIRTUAL_GENERATOR_LOAD_PAIR_TEST");

    TEST_ASSERT(virtual_gen_load_pair->get_generator_power_system_name()=="");
    TEST_ASSERT(virtual_gen_load_pair->get_load_power_system_name()=="");
    TEST_ASSERT(virtual_gen_load_pair->get_generator_power_system_index()==INDEX_NOT_EXIST);
    TEST_ASSERT(virtual_gen_load_pair->get_load_power_system_index()==INDEX_NOT_EXIST);
    TEST_ASSERT(virtual_gen_load_pair->get_virtual_generator().is_valid()==false);
    TEST_ASSERT(virtual_gen_load_pair->get_virtual_load().is_valid()==false);
    TEST_ASSERT(virtual_gen_load_pair->is_valid()==false);
}


void VIRTUAL_GENERATOR_LOAD_PAIR_TEST::test_set_get_generator_power_system_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VIRTUAL_GENERATOR_LOAD_PAIR_TEST");

    virtual_gen_load_pair->set_generator_power_system_name("System 0");
    TEST_ASSERT(virtual_gen_load_pair->get_generator_power_system_name()=="System 0");
    virtual_gen_load_pair->set_generator_power_system_name("System 1");
    TEST_ASSERT(virtual_gen_load_pair->get_generator_power_system_name()=="System 1");
}

void VIRTUAL_GENERATOR_LOAD_PAIR_TEST::test_set_get_load_power_system_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VIRTUAL_GENERATOR_LOAD_PAIR_TEST");

    virtual_gen_load_pair->set_load_power_system_name("System 0");
    TEST_ASSERT(virtual_gen_load_pair->get_load_power_system_name()=="System 0");
    virtual_gen_load_pair->set_load_power_system_name("System 1");
    TEST_ASSERT(virtual_gen_load_pair->get_load_power_system_name()=="System 1");
}

void VIRTUAL_GENERATOR_LOAD_PAIR_TEST::test_set_get_generator_power_system_index()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VIRTUAL_GENERATOR_LOAD_PAIR_TEST");

    virtual_gen_load_pair->set_generator_power_system_index(0);
    TEST_ASSERT(virtual_gen_load_pair->get_generator_power_system_index()==0);
    virtual_gen_load_pair->set_generator_power_system_index(1);
    TEST_ASSERT(virtual_gen_load_pair->get_generator_power_system_index()==1);
}

void VIRTUAL_GENERATOR_LOAD_PAIR_TEST::test_set_get_load_power_system_index()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VIRTUAL_GENERATOR_LOAD_PAIR_TEST");

    virtual_gen_load_pair->set_load_power_system_index(0);
    TEST_ASSERT(virtual_gen_load_pair->get_load_power_system_index()==0);
    virtual_gen_load_pair->set_load_power_system_index(1);
    TEST_ASSERT(virtual_gen_load_pair->get_load_power_system_index()==1);
}


void VIRTUAL_GENERATOR_LOAD_PAIR_TEST::test_set_get_virtual_generator()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VIRTUAL_GENERATOR_LOAD_PAIR_TEST");


    DEVICE_ID did;
    did.set_device_type("GENERATOR");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    virtual_gen_load_pair->set_generator_power_system_name("System 0");
    virtual_gen_load_pair->set_virtual_generator(did);

    TEST_ASSERT(virtual_gen_load_pair->get_virtual_generator()==did);
}

void VIRTUAL_GENERATOR_LOAD_PAIR_TEST::test_set_get_virtual_load()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VIRTUAL_GENERATOR_LOAD_PAIR_TEST");

    DEVICE_ID did;
    did.set_device_type("LOAD");
    TERMINAL terminal;
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#2");

    virtual_gen_load_pair->set_load_power_system_name("System 1");
    virtual_gen_load_pair->set_virtual_load(did);

    TEST_ASSERT(virtual_gen_load_pair->get_virtual_load()==did);
}


void VIRTUAL_GENERATOR_LOAD_PAIR_TEST::test_update()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VIRTUAL_GENERATOR_LOAD_PAIR_TEST");

    build_pair();

    virtual_gen_load_pair->update();

    POWER_SYSTEM_DATABASE* psdb;

    psdb = STEPS::power_system_db+0;

    BUS* bus = psdb->get_bus(1);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.05)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-10.0)<FLOAT_EPSILON);

    psdb = STEPS::power_system_db+1;

    DEVICE_ID did1;
    did1.set_device_type("LOAD");
    TERMINAL terminal;
    terminal.append_bus(2);
    did1.set_device_terminal(terminal);
    did1.set_device_identifier("#2");

    LOAD* load =psdb->get_load(did1);
    complex<double> S = load->get_nominal_constant_power_load_in_MVA();

    TEST_ASSERT(S==complex<double>(2000.0, 1000.0));
}

void VIRTUAL_GENERATOR_LOAD_PAIR_TEST::test_is_valid()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VIRTUAL_GENERATOR_LOAD_PAIR_TEST");

    TEST_ASSERT(virtual_gen_load_pair->is_valid()==false);

    DEVICE_ID did0;
    did0.set_device_type("GENERATOR");
    TERMINAL terminal;
    terminal.append_bus(1);
    did0.set_device_terminal(terminal);
    did0.set_device_identifier("#1");

    virtual_gen_load_pair->set_generator_power_system_name("System 0");
    virtual_gen_load_pair->set_virtual_generator(did0);

    TEST_ASSERT(virtual_gen_load_pair->is_valid()==false);

    terminal.clear();

    DEVICE_ID did1;
    did1.set_device_type("LOAD");
    terminal.append_bus(2);
    did1.set_device_terminal(terminal);
    did1.set_device_identifier("#2");

    virtual_gen_load_pair->set_load_power_system_name("System 1");
    virtual_gen_load_pair->set_virtual_load(did1);

    TEST_ASSERT(virtual_gen_load_pair->is_valid()==true);
}

void VIRTUAL_GENERATOR_LOAD_PAIR_TEST::test_clear()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VIRTUAL_GENERATOR_LOAD_PAIR_TEST");

    build_pair();

    virtual_gen_load_pair->clear();

    TEST_ASSERT(virtual_gen_load_pair->is_valid()==false);
}

void VIRTUAL_GENERATOR_LOAD_PAIR_TEST::test_is_connected_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VIRTUAL_GENERATOR_LOAD_PAIR_TEST");

    build_pair();

    TEST_ASSERT(virtual_gen_load_pair->is_connected_to_bus(1)==true);
    TEST_ASSERT(virtual_gen_load_pair->is_connected_to_bus(2)==true);
}



void VIRTUAL_GENERATOR_LOAD_PAIR_TEST::test_get_device_id()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VIRTUAL_GENERATOR_LOAD_PAIR_TEST");

    build_pair();

    TEST_ASSERT(virtual_gen_load_pair->get_device_id().is_valid()==false);
}

/*
void VIRTUAL_GENERATOR_LOAD_PAIR_TEST::test_initialize()
{
    show_test_information_for_function_of_class(__FUNCTION__,"VIRTUAL_GENERATOR_LOAD_PAIR_TEST");

    build_pair();

    virtual_gen_load_pair->update();

    virtual_gen_load_pair->initialize();

    POWER_SYSTEM_DATABASE* db;
    db = STEPS::power_system_db+0;

    DEVICE_ID did0;
    did0.set_device_type("GENERATOR");
    TERMINAL terminal;
    terminal.append_bus(1);
    did0.set_device_terminal(terminal);
    did0.set_device_identifier("#1");

    GENERATOR* generator = db->get_generator(did0);
    SYNC_GENERATOR_MODEL* gen_model = generator->get_generator_model();

    TEST_ASSERT(gen_model->get_model_name()=="GENCLS");
    TEST_ASSERT(gen_model->get_Tj_in_s()==1e6);
    TEST_ASSERT(gen_model->get_D()==0.0);
    TEST_ASSERT(gen_model->get_Rs()==0.0);
    TEST_ASSERT(gen_model->get_Xdp()==1e-6);

    EXCITER_MODEL* ex_model = generator->get_exciter_model();
    TEST_ASSERT(ex_model==NULL);
    STABILIZER_MODEL* st_model = generator->get_stabilizer_model();
    TEST_ASSERT(st_model==NULL);
    TURBINE_GOVERNOR_MODEL* tg_model = generator->get_turbine_governor_model();
    TEST_ASSERT(tg_model==NULL);

    terminal.clear();

    db = STEPS::power_system_db+1;

    DEVICE_ID did1;
    did1.set_device_type("LOAD");
    terminal.append_bus(2);
    did1.set_device_terminal(terminal);
    did1.set_device_identifier("#2");

    LOAD* load = db->get_load(did1);

    LOAD_MODEL* load_model = load->get_load_model();
    TEST_ASSERT(load_model==NULL);
    LOAD_RELAY_MODEL* load_relay_model = load->get_load_voltage_relay_model();
    TEST_ASSERT(load_relay_model==NULL);
    load_relay_model = load->get_load_frequency_relay_model();
    TEST_ASSERT(load_relay_model==NULL);
}

void VIRTUAL_GENERATOR_LOAD_PAIR_TEST::test_run()
{
    //show_test_information_for_function_of_class(__FUNCTION__,"VIRTUAL_GENERATOR_LOAD_PAIR_TEST");

}


void VIRTUAL_GENERATOR_LOAD_PAIR_TEST::test_clear_all_models()
{
    ;
}*/

void VIRTUAL_GENERATOR_LOAD_PAIR_TEST::build_pair()
{
    DEVICE_ID did0;
    did0.set_device_type("GENERATOR");
    TERMINAL terminal;
    terminal.append_bus(1);
    did0.set_device_terminal(terminal);
    did0.set_device_identifier("#1");

    virtual_gen_load_pair->set_generator_power_system_name("System 0");
    virtual_gen_load_pair->set_virtual_generator(did0);

    terminal.clear();

    DEVICE_ID did1;
    did1.set_device_type("LOAD");
    terminal.append_bus(2);
    did1.set_device_terminal(terminal);
    did1.set_device_identifier("#2");

    virtual_gen_load_pair->set_load_power_system_name("System 1");
    virtual_gen_load_pair->set_virtual_load(did1);
}

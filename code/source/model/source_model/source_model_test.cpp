#include "header/model/source_model/source_model_test.h"
#include "header/basic/utility.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cstdio>
#include <cmath>

using namespace std;
SOURCE_MODEL_TEST::SOURCE_MODEL_TEST()
{
    TEST_ADD(SOURCE_MODEL_TEST::test_get_mbase_in_MVA);
    TEST_ADD(SOURCE_MODEL_TEST::test_get_terminal_voltage);
}



void SOURCE_MODEL_TEST::setup()
{
    db = new POWER_SYSTEM_DATABASE;
    db->set_allowed_max_bus_number(100);
    db->set_system_base_frequency_in_Hz(50.0);
    db->set_system_base_power_in_MVA(100.0);

    BUS bus(db);
    bus.set_bus_number(1);
    bus.set_bus_type(PV_TYPE);
    bus.set_base_voltage_in_kV(21.0);
    bus.set_voltage_in_pu(1.0);
    bus.set_angle_in_rad(0.0);

    db->append_bus(bus);

    GENERATOR generator(db);
    generator.set_generator_bus(1);
    generator.set_identifier("#1");
    generator.set_status(true);
    generator.set_mbase_in_MVA(200.0);
    generator.set_source_impedance_in_pu(complex<double>(0.01, 0.01));
    generator.set_p_generation_in_MW(100.0);
    generator.set_q_generation_in_MVar(30.0);

    db->append_generator(generator);

    DEVICE_ID did;
    did.set_device_type("GENERATOR");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    model = new GENCLS();
    model->set_power_system_database(db);
    model->set_device_id(did);
}

void SOURCE_MODEL_TEST::tear_down()
{
    delete model;
    delete db;

    show_test_end_information();
}

void SOURCE_MODEL_TEST::test_get_mbase_in_MVA()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SOURCE_MODEL_TEST");

    GENERATOR* generator = (GENERATOR*) model->get_device_pointer();
    double mbase = generator->get_mbase_in_MVA();

    TEST_ASSERT(fabs(model->get_mbase_in_MVA()-mbase)<FLOAT_EPSILON);
}



void SOURCE_MODEL_TEST::test_get_terminal_voltage()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SOURCE_MODEL_TEST");

    complex<double> V = db->get_bus_complex_voltage_in_pu(1);

    TEST_ASSERT(fast_complex_abs(model->get_terminal_complex_voltage_in_pu()-V)<FLOAT_EPSILON);
}


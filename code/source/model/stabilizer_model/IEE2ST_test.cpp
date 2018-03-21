#include "header/model/stabilizer_model/IEE2ST_test.h"
#include "header/basic/utility.h"
using namespace std;

IEE2ST_TEST::IEE2ST_TEST()
{
    TEST_ADD(IEE2ST_TEST::test_get_model_name);
    TEST_ADD(IEE2ST_TEST::test_set_get_parameters);

}



void IEE2ST_TEST::setup()
{
    db = new POWER_SYSTEM_DATABASE;
    db->set_system_name("TEST SYSTEM");
    db->set_allowed_max_bus_number(10);
    db->set_system_base_frequency_in_Hz(50.0);
    db->set_system_base_power_in_MVA(100.0);

    BUS bus(db);
    bus.set_bus_number(1);
    bus.set_base_voltage_in_kV(21.0);
    bus.set_bus_type(PV_TYPE);

    db->append_bus(bus);

    GENERATOR generator(db);
    generator.set_generator_bus(1);
    generator.set_identifier("#1");
    generator.set_status(true);

    db->append_generator(generator);

    DEVICE_ID did;
    did.set_device_type("GENERATOR");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    model = new IEE2ST();
    model->set_power_system_database(db);
    model->set_device_id(did);
}

void IEE2ST_TEST::tear_down()
{
    delete model;
}

void IEE2ST_TEST::test_get_model_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"IEE2ST_TEST");

    TEST_ASSERT(model->get_model_name()=="IEE2ST");
}

void IEE2ST_TEST::test_set_get_parameters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"IEE2ST_TEST");

    model->set_K1(1.0);
    TEST_ASSERT(fabs(model->get_K1()-1.0)<FLOAT_EPSILON);

    model->set_T1_in_s(0.1);
    TEST_ASSERT(fabs(model->get_T1_in_s()-0.1)<FLOAT_EPSILON);

    model->set_K2(2.0);
    TEST_ASSERT(fabs(model->get_K2()-2.0)<FLOAT_EPSILON);

    model->set_T2_in_s(0.2);
    TEST_ASSERT(fabs(model->get_T2_in_s()-0.2)<FLOAT_EPSILON);

    model->set_T3_in_s(0.3);
    TEST_ASSERT(fabs(model->get_T3_in_s()-0.3)<FLOAT_EPSILON);

    model->set_T4_in_s(0.4);
    TEST_ASSERT(fabs(model->get_T4_in_s()-0.4)<FLOAT_EPSILON);

    model->set_T5_in_s(0.5);
    TEST_ASSERT(fabs(model->get_T5_in_s()-0.5)<FLOAT_EPSILON);

    model->set_T6_in_s(0.6);
    TEST_ASSERT(fabs(model->get_T6_in_s()-0.6)<FLOAT_EPSILON);

    model->set_T7_in_s(0.7);
    TEST_ASSERT(fabs(model->get_T7_in_s()-0.7)<FLOAT_EPSILON);

    model->set_T8_in_s(0.8);
    TEST_ASSERT(fabs(model->get_T8_in_s()-0.8)<FLOAT_EPSILON);

    model->set_T9_in_s(0.9);
    TEST_ASSERT(fabs(model->get_T9_in_s()-0.9)<FLOAT_EPSILON);

    model->set_T10_in_s(1.0);
    TEST_ASSERT(fabs(model->get_T10_in_s()-1.0)<FLOAT_EPSILON);
}

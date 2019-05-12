#include "header/model/model_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

using namespace std;

MODEL_TEST::MODEL_TEST()
{
    TEST_ADD(MODEL_TEST::test_get_device_pointer_and_device_id)
    TEST_ADD(MODEL_TEST::test_set_get_flag_model_initialized)
    TEST_ADD(MODEL_TEST::test_set_get_flag_model_updated)
    TEST_ADD(MODEL_TEST::test_activate_and_deactive_model)
}


void MODEL_TEST::setup()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.clear();
    psdb.set_allowed_max_bus_number(100);
    psdb.set_system_base_power_in_MVA(100.0);

    BUS bus;
    bus.set_toolkit(default_toolkit);
    bus.set_bus_number(1);
    bus.set_bus_type(PV_TYPE);
    bus.set_base_voltage_in_kV(21.0);
    bus.set_voltage_in_pu(1.0);
    bus.set_angle_in_rad(0.0);

    psdb.append_bus(bus);

    GENERATOR generator;
    generator.set_toolkit(default_toolkit);
    generator.set_generator_bus(1);
    generator.set_identifier("#1");
    generator.set_status(true);

    psdb.append_generator(generator);

    DEVICE_ID did;
    did.set_device_type("GENERATOR");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    model.set_toolkit(default_toolkit);
    model.set_device_id(did);
}

void MODEL_TEST::tear_down()
{
    model.clear();

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.clear();

    show_test_end_information();
}

void MODEL_TEST::test_get_device_pointer_and_device_id()
{
    show_test_information_for_function_of_class(__FUNCTION__,"MODEL_TEST");

    DEVICE* device = model.get_device_pointer();
    TEST_ASSERT(device!=NULL);

    DEVICE_ID did;
    did.set_device_type("GENERATOR");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    TEST_ASSERT(device->get_device_id()==did);
    TEST_ASSERT(model.get_device_id()==did);
}

void MODEL_TEST::test_set_get_flag_model_initialized()
{
    show_test_information_for_function_of_class(__FUNCTION__,"MODEL_TEST");

    TEST_ASSERT(model.is_model_initialized()==false);

    model.set_flag_model_initialized_as_true();
    TEST_ASSERT(model.is_model_initialized()==true);
    model.set_flag_model_initialized_as_false();
    TEST_ASSERT(model.is_model_initialized()==false);
}

void MODEL_TEST::test_set_get_flag_model_updated()
{
    show_test_information_for_function_of_class(__FUNCTION__,"MODEL_TEST");

    TEST_ASSERT(model.is_model_updated()==false);

    model.set_flag_model_updated_as_true();
    TEST_ASSERT(model.is_model_updated()==true);
    model.set_flag_model_updated_as_false();
    TEST_ASSERT(model.is_model_updated()==false);
}

void MODEL_TEST::test_activate_and_deactive_model()
{
    show_test_information_for_function_of_class(__FUNCTION__,"MODEL_TEST");

    TEST_ASSERT(model.is_model_active()==true);

    model.deactivate_model();
    TEST_ASSERT(model.is_model_active()==false);
    model.activate_model();
    TEST_ASSERT(model.is_model_active()==true);
}

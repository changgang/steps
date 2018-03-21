#include "header/meter/meter_setter_test.h"
#include "header/basic/utility.h"
#include "header/prepare_for_tests/prepare_models_for_test.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cstdio>
#include <cmath>


using namespace std;

METER_SETTER_TEST::METER_SETTER_TEST()
{
    TEST_ADD(METER_SETTER_TEST::test_constructor);
    TEST_ADD(METER_SETTER_TEST::test_prepare_bus_related_meters);
    TEST_ADD(METER_SETTER_TEST::test_prepare_line_related_meters);
    TEST_ADD(METER_SETTER_TEST::test_prepare_transformer_related_meters);
    TEST_ADD(METER_SETTER_TEST::test_prepare_load_related_meters);
    TEST_ADD(METER_SETTER_TEST::test_prepare_generator_related_meters);
    TEST_ADD(METER_SETTER_TEST::test_prepare_hvdc_related_meters);
    TEST_ADD(METER_SETTER_TEST::test_prepare_equivalent_device_related_meters);
}

void METER_SETTER_TEST::setup()
{
    db = new POWER_SYSTEM_DATABASE;

    setter = new METER_SETTER;
    setter->set_power_system_database(db);
}

void METER_SETTER_TEST::tear_down()
{
    delete setter;
    setter = NULL;

    delete db;
    db = NULL;

    show_test_end_information();
}

void METER_SETTER_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_SETTER_TEST");

}

void METER_SETTER_TEST::test_prepare_bus_related_meters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_SETTER_TEST");

    prepare_basic_buses(db);

    METER meter(db);
    DEVICE_ID did;
    TERMINAL terminal;

    did.set_device_type("BUS");
    terminal.append_bus(1);
    did.set_device_terminal(terminal);

    meter = setter->prepare_bus_voltage_in_pu_meter(1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="BUS VOLTAGE IN PU");

    meter = setter->prepare_bus_voltage_in_kV_meter(1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="BUS VOLTAGE IN KV");

    meter = setter->prepare_bus_angle_in_deg_meter(1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="BUS ANGLE IN DEG");

    meter = setter->prepare_bus_angle_in_rad_meter(1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="BUS ANGLE IN RAD");

    meter = setter->prepare_bus_frequency_in_pu_meter(1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="BUS FREQUENCY IN PU");

    meter = setter->prepare_bus_frequency_in_Hz_meter(1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="BUS FREQUENCY IN HZ");

    meter = setter->prepare_bus_frequency_deviation_in_pu_meter(1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="BUS FREQUENCY DEVIATION IN PU");

    meter = setter->prepare_bus_frequency_deviation_in_Hz_meter(1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="BUS FREQUENCY DEVIATION IN HZ");

    meter = setter->prepare_bus_ROCOV_in_pu_per_s_meter(1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="BUS ROCOV IN PU/S");

    meter = setter->prepare_bus_ROCOV_in_kV_per_s_meter(1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="BUS ROCOV IN KV/S");

    meter = setter->prepare_bus_ROCOF_in_pu_per_s_meter(1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="BUS ROCOF IN PU/S");

    meter = setter->prepare_bus_ROCOF_in_Hz_per_s_meter(1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="BUS ROCOF IN HZ/S");
}

void METER_SETTER_TEST::test_prepare_line_related_meters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_SETTER_TEST");
    prepare_basic_lines(db);

    METER meter(db);
    DEVICE_ID did;
    TERMINAL terminal;

    did.set_device_type("LINE");
    terminal.append_bus(1);
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1#");

    meter = setter->prepare_line_current_in_kA_meter(did,1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="LINE CURRENT IN KA");
    TEST_ASSERT(meter.get_meter_side_bus()==1);

    meter = setter->prepare_line_current_in_kA_meter(did,2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="LINE CURRENT IN KA");
    TEST_ASSERT(meter.get_meter_side_bus()==2);

    meter = setter->prepare_line_active_power_in_MW_meter(did,1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="LINE ACTIVE POWER IN MW");
    TEST_ASSERT(meter.get_meter_side_bus()==1);

    meter = setter->prepare_line_active_power_in_MW_meter(did,2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="LINE ACTIVE POWER IN MW");
    TEST_ASSERT(meter.get_meter_side_bus()==2);

    meter = setter->prepare_line_reactive_power_in_MVar_meter(did,1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="LINE REACTIVE POWER IN MVAR");
    TEST_ASSERT(meter.get_meter_side_bus()==1);

    meter = setter->prepare_line_reactive_power_in_MVar_meter(did,2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="LINE REACTIVE POWER IN MVAR");
    TEST_ASSERT(meter.get_meter_side_bus()==2);

    meter = setter->prepare_line_apparent_impedance_in_Ohm_meter(did,1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="LINE APPARENT IMPEDANCE IN OHM");
    TEST_ASSERT(meter.get_meter_side_bus()==1);

    meter = setter->prepare_line_apparent_impedance_in_Ohm_meter(did,2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="LINE APPARENT IMPEDANCE IN OHM");
    TEST_ASSERT(meter.get_meter_side_bus()==2);

    meter = setter->prepare_line_apparent_impedance_angle_in_deg_meter(did,1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="LINE APPARENT IMPEDANCE ANGLE IN DEG");
    TEST_ASSERT(meter.get_meter_side_bus()==1);

    meter = setter->prepare_line_apparent_impedance_angle_in_deg_meter(did,2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="LINE APPARENT IMPEDANCE ANGLE IN DEG");
    TEST_ASSERT(meter.get_meter_side_bus()==2);

    meter = setter->prepare_line_current_in_pu_meter(did,1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="LINE CURRENT IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==1);

    meter = setter->prepare_line_current_in_pu_meter(did,2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="LINE CURRENT IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==2);

    meter = setter->prepare_line_active_power_in_pu_meter(did,1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="LINE ACTIVE POWER IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==1);

    meter = setter->prepare_line_active_power_in_pu_meter(did,2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="LINE ACTIVE POWER IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==2);

    meter = setter->prepare_line_reactive_power_in_pu_meter(did,1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="LINE REACTIVE POWER IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==1);

    meter = setter->prepare_line_reactive_power_in_pu_meter(did,2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="LINE REACTIVE POWER IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==2);

    meter = setter->prepare_line_apparent_impedance_in_pu_meter(did,1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="LINE APPARENT IMPEDANCE IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==1);

    meter = setter->prepare_line_apparent_impedance_in_pu_meter(did,2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="LINE APPARENT IMPEDANCE IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==2);

    meter = setter->prepare_line_apparent_impedance_angle_in_rad_meter(did,1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="LINE APPARENT IMPEDANCE ANGLE IN RAD");
    TEST_ASSERT(meter.get_meter_side_bus()==1);

    meter = setter->prepare_line_apparent_impedance_angle_in_rad_meter(did,2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="LINE APPARENT IMPEDANCE ANGLE IN RAD");
    TEST_ASSERT(meter.get_meter_side_bus()==2);
}

void METER_SETTER_TEST::test_prepare_transformer_related_meters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_SETTER_TEST");
    prepare_basic_transformers(db);

    METER meter(db);
    DEVICE_ID did;
    TERMINAL terminal;

    did.set_device_type("TRANSFORMER");
    terminal.append_bus(1);
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1#");

    meter = setter->prepare_transformer_current_in_kA_meter(did,1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TRANSFORMER CURRENT IN KA");
    TEST_ASSERT(meter.get_meter_side_bus()==1);

    meter = setter->prepare_transformer_current_in_kA_meter(did,2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TRANSFORMER CURRENT IN KA");
    TEST_ASSERT(meter.get_meter_side_bus()==2);

    meter = setter->prepare_transformer_active_power_in_MW_meter(did,1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TRANSFORMER ACTIVE POWER IN MW");
    TEST_ASSERT(meter.get_meter_side_bus()==1);

    meter = setter->prepare_transformer_active_power_in_MW_meter(did,2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TRANSFORMER ACTIVE POWER IN MW");
    TEST_ASSERT(meter.get_meter_side_bus()==2);

    meter = setter->prepare_transformer_reactive_power_in_MVar_meter(did,1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TRANSFORMER REACTIVE POWER IN MVAR");
    TEST_ASSERT(meter.get_meter_side_bus()==1);

    meter = setter->prepare_transformer_reactive_power_in_MVar_meter(did,2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TRANSFORMER REACTIVE POWER IN MVAR");
    TEST_ASSERT(meter.get_meter_side_bus()==2);

    meter = setter->prepare_transformer_current_in_pu_meter(did,1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TRANSFORMER CURRENT IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==1);

    meter = setter->prepare_transformer_current_in_pu_meter(did,2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TRANSFORMER CURRENT IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==2);

    meter = setter->prepare_transformer_active_power_in_pu_meter(did,1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TRANSFORMER ACTIVE POWER IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==1);

    meter = setter->prepare_transformer_active_power_in_pu_meter(did,2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TRANSFORMER ACTIVE POWER IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==2);

    meter = setter->prepare_transformer_reactive_power_in_pu_meter(did,1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TRANSFORMER REACTIVE POWER IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==1);

    meter = setter->prepare_transformer_reactive_power_in_pu_meter(did,2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TRANSFORMER REACTIVE POWER IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==2);

    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1#");

    meter = setter->prepare_transformer_current_in_kA_meter(did,1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TRANSFORMER CURRENT IN KA");
    TEST_ASSERT(meter.get_meter_side_bus()==1);

    meter = setter->prepare_transformer_current_in_kA_meter(did,2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TRANSFORMER CURRENT IN KA");
    TEST_ASSERT(meter.get_meter_side_bus()==2);

    meter = setter->prepare_transformer_current_in_kA_meter(did,3);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TRANSFORMER CURRENT IN KA");
    TEST_ASSERT(meter.get_meter_side_bus()==3);

    meter = setter->prepare_transformer_active_power_in_MW_meter(did,1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TRANSFORMER ACTIVE POWER IN MW");
    TEST_ASSERT(meter.get_meter_side_bus()==1);

    meter = setter->prepare_transformer_active_power_in_MW_meter(did,2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TRANSFORMER ACTIVE POWER IN MW");
    TEST_ASSERT(meter.get_meter_side_bus()==2);

    meter = setter->prepare_transformer_active_power_in_MW_meter(did,3);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TRANSFORMER ACTIVE POWER IN MW");
    TEST_ASSERT(meter.get_meter_side_bus()==3);

    meter = setter->prepare_transformer_reactive_power_in_MVar_meter(did,1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TRANSFORMER REACTIVE POWER IN MVAR");
    TEST_ASSERT(meter.get_meter_side_bus()==1);

    meter = setter->prepare_transformer_reactive_power_in_MVar_meter(did,2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TRANSFORMER REACTIVE POWER IN MVAR");
    TEST_ASSERT(meter.get_meter_side_bus()==2);

    meter = setter->prepare_transformer_reactive_power_in_MVar_meter(did,3);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TRANSFORMER REACTIVE POWER IN MVAR");
    TEST_ASSERT(meter.get_meter_side_bus()==3);

    meter = setter->prepare_transformer_current_in_pu_meter(did,1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TRANSFORMER CURRENT IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==1);

    meter = setter->prepare_transformer_current_in_pu_meter(did,2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TRANSFORMER CURRENT IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==2);

    meter = setter->prepare_transformer_current_in_pu_meter(did,3);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TRANSFORMER CURRENT IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==3);

    meter = setter->prepare_transformer_active_power_in_pu_meter(did,1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TRANSFORMER ACTIVE POWER IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==1);

    meter = setter->prepare_transformer_active_power_in_pu_meter(did,2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TRANSFORMER ACTIVE POWER IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==2);

    meter = setter->prepare_transformer_active_power_in_pu_meter(did,3);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TRANSFORMER ACTIVE POWER IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==3);

    meter = setter->prepare_transformer_reactive_power_in_pu_meter(did,1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TRANSFORMER REACTIVE POWER IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==1);

    meter = setter->prepare_transformer_reactive_power_in_pu_meter(did,2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TRANSFORMER REACTIVE POWER IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==2);

    meter = setter->prepare_transformer_reactive_power_in_pu_meter(did,3);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TRANSFORMER REACTIVE POWER IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==3);
}

void METER_SETTER_TEST::test_prepare_load_related_meters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_SETTER_TEST");
    prepare_basic_loads(db);

    METER meter(db);
    DEVICE_ID did;
    TERMINAL terminal;

    did.set_device_type("LOAD");
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1#");

    meter = setter->prepare_load_active_power_in_MW_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="LOAD ACTIVE POWER IN MW");

    meter = setter->prepare_load_reactive_power_in_MVar_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="LOAD REACTIVE POWER IN MVAR");

    meter = setter->prepare_load_current_in_kA_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="LOAD CURRENT IN KA");

    meter = setter->prepare_load_active_power_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="LOAD ACTIVE POWER IN PU");

    meter = setter->prepare_load_reactive_power_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="LOAD REACTIVE POWER IN PU");

    meter = setter->prepare_load_current_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="LOAD CURRENT IN PU");

    meter = setter->prepare_load_shed_scale_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="LOAD SHED SCALE IN PU");

    meter = setter->prepare_load_load_model_internal_variable_meter(did,1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="LOAD LOAD MODEL INTERNAL VARIABLE");
    TEST_ASSERT(meter.get_internal_variable_index()==1);

    meter = setter->prepare_load_frequency_relay_model_internal_variable_meter(did,2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="LOAD FREQUENCY RELAY MODEL INTERNAL VARIABLE");
    TEST_ASSERT(meter.get_internal_variable_index()==2);

    meter = setter->prepare_load_voltage_relay_model_internal_variable_meter(did,4);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="LOAD VOLTAGE RELAY MODEL INTERNAL VARIABLE");
    TEST_ASSERT(meter.get_internal_variable_index()==4);
}

void METER_SETTER_TEST::test_prepare_generator_related_meters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_SETTER_TEST");
    prepare_basic_generators(db);

    METER meter(db);
    DEVICE_ID did;
    TERMINAL terminal;

    did.set_device_type("GENERATOR");
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1#");

    meter = setter->prepare_generator_rotor_angle_in_deg_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="GENERATOR ROTOR ANGLE IN DEG");

    meter = setter->prepare_generator_rotor_speed_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="GENERATOR ROTOR SPEED IN PU");

    meter = setter->prepare_generator_rotor_speed_in_Hz_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="GENERATOR ROTOR SPEED IN HZ");

    meter = setter->prepare_generator_rotor_speed_deviation_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="GENERATOR ROTOR SPEED DEVIATION IN PU");

    meter = setter->prepare_generator_rotor_speed_deviation_in_Hz_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="GENERATOR ROTOR SPEED DEVIATION IN HZ");

    meter = setter->prepare_generator_internal_voltage_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="GENERATOR INTERNAL VOLTAGE IN PU");

    meter = setter->prepare_generator_terminal_current_in_pu_on_mbase_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="GENERATOR TERMINAL CURRENT IN PU ON MBASE");

    meter = setter->prepare_generator_terminal_current_in_pu_on_sbase_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="GENERATOR TERMINAL CURRENT IN PU ON SBASE");

    meter = setter->prepare_generator_terminal_current_in_kA_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="GENERATOR TERMINAL CURRENT IN KA");

    meter = setter->prepare_generator_terminal_active_power_in_pu_on_mbase_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="GENERATOR TERMINAL ACTIVE POWER IN PU ON MBASE");

    meter = setter->prepare_generator_terminal_active_power_in_pu_on_sbase_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="GENERATOR TERMINAL ACTIVE POWER IN PU ON SBASE");

    meter = setter->prepare_generator_terminal_active_power_in_MW_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="GENERATOR TERMINAL ACTIVE POWER IN MW");

    meter = setter->prepare_generator_terminal_reactive_power_in_pu_on_mbase_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="GENERATOR TERMINAL REACTIVE POWER IN PU ON MBASE");

    meter = setter->prepare_generator_terminal_reactive_power_in_pu_on_sbase_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="GENERATOR TERMINAL REACTIVE POWER IN PU ON SBASE");

    meter = setter->prepare_generator_terminal_reactive_power_in_MVar_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="GENERATOR TERMINAL REACTIVE POWER IN MVAR");

    meter = setter->prepare_generator_airgap_power_in_pu_on_mbase_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="GENERATOR AIRGAP POWER IN PU ON MBASE");

    meter = setter->prepare_generator_airgap_power_in_pu_on_sbase_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="GENERATOR AIRGAP POWER IN PU ON SBASE");

    meter = setter->prepare_generator_airgap_power_in_MW_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="GENERATOR AIRGAP POWER IN MW");

    meter = setter->prepare_generator_accelerating_power_in_pu_on_mbase_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="GENERATOR ACCELERATING POWER IN PU ON MBASE");

    meter = setter->prepare_generator_accelerating_power_in_pu_on_sbase_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="GENERATOR ACCELERATING POWER IN PU ON SBASE");

    meter = setter->prepare_generator_accelerating_power_in_MW_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="GENERATOR ACCELERATING POWER IN MW");

    meter = setter->prepare_generator_mechanical_power_in_pu_on_mbase_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="GENERATOR MECHANICAL POWER IN PU ON MBASE");

    meter = setter->prepare_generator_mechanical_power_in_pu_on_sbase_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="GENERATOR MECHANICAL POWER IN PU ON SBASE");

    meter = setter->prepare_generator_mechanical_power_in_MW_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="GENERATOR MECHANICAL POWER IN MW");

    meter = setter->prepare_generator_excitation_voltage_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="GENERATOR EXCITATION VOLTAGE IN PU");

    meter = setter->prepare_generator_compensated_voltage_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="GENERATOR COMPENSATED VOLTAGE IN PU");

    meter = setter->prepare_generator_stabilizing_signal_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="GENERATOR STABILIZING SIGNAL IN PU");

    meter = setter->prepare_generator_sync_generator_model_internal_variable_meter(did, 1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="GENERATOR SYNC GENERATOR MODEL INTERNAL VARIABLE");
    TEST_ASSERT(meter.get_internal_variable_index()==1);

    meter = setter->prepare_generator_compensator_model_internal_variable_meter(did, 2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="GENERATOR COMPENSATOR MODEL INTERNAL VARIABLE");
    TEST_ASSERT(meter.get_internal_variable_index()==2);

    meter = setter->prepare_generator_stabilizer_model_internal_variable_meter(did, 3);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="GENERATOR STABILIZER MODEL INTERNAL VARIABLE");
    TEST_ASSERT(meter.get_internal_variable_index()==3);

    meter = setter->prepare_generator_exciter_model_internal_variable_meter(did, 4);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="GENERATOR EXCITER MODEL INTERNAL VARIABLE");
    TEST_ASSERT(meter.get_internal_variable_index()==4);

    meter = setter->prepare_generator_turbine_governor_model_internal_variable_meter(did, 5);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="GENERATOR TURBINE GOVERNOR MODEL INTERNAL VARIABLE");
    TEST_ASSERT(meter.get_internal_variable_index()==5);
}

void METER_SETTER_TEST::test_prepare_pe_source_related_meters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_SETTER_TEST");
}

void METER_SETTER_TEST::test_prepare_hvdc_related_meters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_SETTER_TEST");
    prepare_basic_hvdcs(db);

    METER meter(db);
    DEVICE_ID did;
    TERMINAL terminal;

    did.set_device_type("HVDC");
    terminal.append_bus(1);
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("P");

    meter = setter->prepare_hvdc_dc_current_in_kA_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="HVDC DC CURRENT IN KA");

    meter = setter->prepare_hvdc_rectifier_dc_current_in_kA_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="HVDC RECTIFIER DC CURRENT IN KA");

    meter = setter->prepare_hvdc_inverter_dc_current_in_kA_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="HVDC INVERTER DC CURRENT IN KA");

    meter = setter->prepare_hvdc_rectifier_alpha_in_deg_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="HVDC RECTIFIER ALPHA IN DEG");

    meter = setter->prepare_hvdc_inverter_gamma_in_deg_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="HVDC INVERTER GAMMA IN DEG");

    meter = setter->prepare_hvdc_rectifier_mu_in_deg_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="HVDC RECTIFIER MU IN DEG");

    meter = setter->prepare_hvdc_inverter_mu_in_deg_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="HVDC INVERTER MU IN DEG");

    meter = setter->prepare_hvdc_rectifier_dc_voltage_in_kV_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="HVDC RECTIFIER DC VOLTAGE IN KV");

    meter = setter->prepare_hvdc_inverter_dc_voltage_in_kV_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="HVDC INVERTER DC VOLTAGE IN KV");

    meter = setter->prepare_hvdc_rectifier_ac_voltage_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="HVDC RECTIFIER AC VOLTAGE IN PU");

    meter = setter->prepare_hvdc_inverter_ac_voltage_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="HVDC INVERTER AC VOLTAGE IN PU");

    meter = setter->prepare_hvdc_rectifier_dc_power_in_MW_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="HVDC RECTIFIER DC POWER IN MW");

    meter = setter->prepare_hvdc_inverter_dc_power_in_MW_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="HVDC INVERTER DC POWER IN MW");

    meter = setter->prepare_hvdc_rectifier_ac_active_power_in_MW_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="HVDC RECTIFIER AC ACTIVE POWER IN MW");

    meter = setter->prepare_hvdc_inverter_ac_active_power_in_MW_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="HVDC INVERTER AC ACTIVE POWER IN MW");

    meter = setter->prepare_hvdc_rectifier_ac_reactive_power_in_MVar_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="HVDC RECTIFIER AC REACTIVE POWER IN MVAR");

    meter = setter->prepare_hvdc_inverter_ac_reactive_power_in_MVar_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="HVDC INVERTER AC REACTIVE POWER IN MVAR");

    meter = setter->prepare_hvdc_model_internal_variable_meter(did, 0);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="HVDC MODEL INTERNAL VARIABLE");
    TEST_ASSERT(meter.get_internal_variable_index()==0);
}

void METER_SETTER_TEST::test_prepare_equivalent_device_related_meters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_SETTER_TEST");
    prepare_basic_equivalent_devices(db);

    METER meter(db);
    DEVICE_ID did;
    TERMINAL terminal;

    did.set_device_type("EQUIVALENT DEVICE");
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("E1");

    meter = setter->prepare_equivalent_device_voltage_source_voltage_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="EQUIVALENT DEVICE VOLTAGE SOURCE VOLTAGE IN PU");

    meter = setter->prepare_equivalent_device_voltage_source_voltage_angle_in_deg_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="EQUIVALENT DEVICE VOLTAGE SOURCE VOLTAGE ANGLE IN DEG");

    meter = setter->prepare_equivalent_device_voltage_source_resistance_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="EQUIVALENT DEVICE VOLTAGE SOURCE RESISTANCE IN PU");

    meter = setter->prepare_equivalent_device_voltage_source_reactance_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="EQUIVALENT DEVICE VOLTAGE SOURCE REACTANCE IN PU");

    meter = setter->prepare_equivalent_device_nominal_active_constant_power_load_in_MW_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="EQUIVALENT DEVICE ACTIVE CONSTANT POWER LOAD IN MW");

    meter = setter->prepare_equivalent_device_nominal_active_constant_current_load_in_MW_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="EQUIVALENT DEVICE ACTIVE CONSTANT CURRENT LOAD IN MW");

    meter = setter->prepare_equivalent_device_nominal_active_constant_impedance_load_in_MW_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="EQUIVALENT DEVICE ACTIVE CONSTANT IMPEDANCE LOAD IN MW");

    meter = setter->prepare_equivalent_device_nominal_reactive_constant_power_load_in_MVar_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="EQUIVALENT DEVICE REACTIVE CONSTANT POWER LOAD IN MVAR");

    meter = setter->prepare_equivalent_device_nominal_reactive_constant_current_load_in_MVar_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="EQUIVALENT DEVICE REACTIVE CONSTANT CURRENT LOAD IN MVAR");

    meter = setter->prepare_equivalent_device_nominal_reactive_constant_impedance_load_in_MVar_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="EQUIVALENT DEVICE REACTIVE CONSTANT IMPEDANCE LOAD IN MVAR");

    meter = setter->prepare_equivalent_device_active_power_generation_in_MW_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="EQUIVALENT DEVICE ACTIVE POWER GENERATION IN MW");

    meter = setter->prepare_equivalent_device_reactive_power_generation_in_MVar_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="EQUIVALENT DEVICE REACTIVE POWER GENERATION IN MVAR");

    meter = setter->prepare_equivalent_device_active_power_load_in_MW_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="EQUIVALENT DEVICE ACTIVE POWER LOAD IN MW");

    meter = setter->prepare_equivalent_device_reactive_power_load_in_MVar_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="EQUIVALENT DEVICE REACTIVE POWER LOAD IN MVAR");

    meter = setter->prepare_equivalent_device_active_power_net_load_in_MW_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="EQUIVALENT DEVICE ACTIVE POWER NET LOAD IN MW");

    meter = setter->prepare_equivalent_device_reactive_power_net_load_in_MVar_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="EQUIVALENT DEVICE REACTIVE POWER NET LOAD IN MVAR");

    meter = setter->prepare_equivalent_device_nominal_active_constant_power_load_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="EQUIVALENT DEVICE ACTIVE CONSTANT POWER LOAD IN PU");

    meter = setter->prepare_equivalent_device_nominal_active_constant_current_load_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="EQUIVALENT DEVICE ACTIVE CONSTANT CURRENT LOAD IN PU");

    meter = setter->prepare_equivalent_device_nominal_active_constant_impedance_load_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="EQUIVALENT DEVICE ACTIVE CONSTANT IMPEDANCE LOAD IN PU");

    meter = setter->prepare_equivalent_device_nominal_reactive_constant_power_load_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="EQUIVALENT DEVICE REACTIVE CONSTANT POWER LOAD IN PU");

    meter = setter->prepare_equivalent_device_nominal_reactive_constant_current_load_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="EQUIVALENT DEVICE REACTIVE CONSTANT CURRENT LOAD IN PU");

    meter = setter->prepare_equivalent_device_nominal_reactive_constant_impedance_load_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="EQUIVALENT DEVICE REACTIVE CONSTANT IMPEDANCE LOAD IN PU");

    meter = setter->prepare_equivalent_device_active_power_generation_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="EQUIVALENT DEVICE ACTIVE POWER GENERATION IN PU");

    meter = setter->prepare_equivalent_device_reactive_power_generation_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="EQUIVALENT DEVICE REACTIVE POWER GENERATION IN PU");

    meter = setter->prepare_equivalent_device_active_power_load_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="EQUIVALENT DEVICE ACTIVE POWER LOAD IN PU");

    meter = setter->prepare_equivalent_device_reactive_power_load_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="EQUIVALENT DEVICE REACTIVE POWER LOAD IN PU");

    meter = setter->prepare_equivalent_device_active_power_net_load_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="EQUIVALENT DEVICE ACTIVE POWER NET LOAD IN PU");

    meter = setter->prepare_equivalent_device_reactive_power_net_load_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="EQUIVALENT DEVICE REACTIVE POWER NET LOAD IN PU");
}


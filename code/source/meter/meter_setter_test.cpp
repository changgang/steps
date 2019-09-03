#include "header/meter/meter_setter_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/prepare_for_tests/prepare_models_for_test.h"
#include <cstdlib>
#include <cstring>
#include <istream>
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
    TEST_ADD(METER_SETTER_TEST::test_prepare_wt_generator_related_meters);
    TEST_ADD(METER_SETTER_TEST::test_prepare_pv_unit_related_meters);
    TEST_ADD(METER_SETTER_TEST::test_prepare_energy_storage_related_meters);
    TEST_ADD(METER_SETTER_TEST::test_prepare_hvdc_related_meters);
    TEST_ADD(METER_SETTER_TEST::test_prepare_equivalent_device_related_meters);
}

void METER_SETTER_TEST::setup()
{
    setter.set_toolkit(default_toolkit);
}

void METER_SETTER_TEST::tear_down()
{
    show_test_end_information();
}

void METER_SETTER_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_SETTER_TEST");

}

void METER_SETTER_TEST::test_prepare_bus_related_meters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_SETTER_TEST");

    prepare_basic_buses();

    METER meter;
    meter.set_toolkit(default_toolkit);

    DEVICE_ID did;
    TERMINAL terminal;

    did.set_device_type("BUS");
    terminal.append_bus(1);
    did.set_device_terminal(terminal);

    meter = setter.prepare_bus_voltage_in_pu_meter(1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="VOLTAGE IN PU");

    meter = setter.prepare_bus_voltage_in_kV_meter(1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="VOLTAGE IN KV");

    meter = setter.prepare_bus_angle_in_deg_meter(1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ANGLE IN DEG");

    meter = setter.prepare_bus_angle_in_rad_meter(1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ANGLE IN RAD");

    meter = setter.prepare_bus_frequency_in_pu_meter(1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="FREQUENCY IN PU");

    meter = setter.prepare_bus_frequency_in_Hz_meter(1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="FREQUENCY IN HZ");

    meter = setter.prepare_bus_frequency_deviation_in_pu_meter(1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="FREQUENCY DEVIATION IN PU");

    meter = setter.prepare_bus_frequency_deviation_in_Hz_meter(1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="FREQUENCY DEVIATION IN HZ");

    meter = setter.prepare_bus_ROCOV_in_pu_per_s_meter(1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ROCOV IN PU/S");

    meter = setter.prepare_bus_ROCOV_in_kV_per_s_meter(1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ROCOV IN KV/S");

    meter = setter.prepare_bus_ROCOF_in_pu_per_s_meter(1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ROCOF IN PU/S");

    meter = setter.prepare_bus_ROCOF_in_Hz_per_s_meter(1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ROCOF IN HZ/S");
}

void METER_SETTER_TEST::test_prepare_line_related_meters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_SETTER_TEST");

    prepare_basic_lines();

    METER meter;
    meter.set_toolkit(default_toolkit);

    DEVICE_ID did;
    TERMINAL terminal;

    did.set_device_type("LINE");
    terminal.append_bus(1);
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1#");

    meter = setter.prepare_line_current_in_kA_meter(did,1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="CURRENT IN KA");
    TEST_ASSERT(meter.get_meter_side_bus()==1);

    meter = setter.prepare_line_current_in_kA_meter(did,2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="CURRENT IN KA");
    TEST_ASSERT(meter.get_meter_side_bus()==2);

    meter = setter.prepare_line_active_power_in_MW_meter(did,1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ACTIVE POWER IN MW");
    TEST_ASSERT(meter.get_meter_side_bus()==1);

    meter = setter.prepare_line_active_power_in_MW_meter(did,2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ACTIVE POWER IN MW");
    TEST_ASSERT(meter.get_meter_side_bus()==2);

    meter = setter.prepare_line_reactive_power_in_MVar_meter(did,1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="REACTIVE POWER IN MVAR");
    TEST_ASSERT(meter.get_meter_side_bus()==1);

    meter = setter.prepare_line_reactive_power_in_MVar_meter(did,2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="REACTIVE POWER IN MVAR");
    TEST_ASSERT(meter.get_meter_side_bus()==2);

    meter = setter.prepare_line_apparent_impedance_in_Ohm_meter(did,1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="APPARENT IMPEDANCE IN OHM");
    TEST_ASSERT(meter.get_meter_side_bus()==1);

    meter = setter.prepare_line_apparent_impedance_in_Ohm_meter(did,2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="APPARENT IMPEDANCE IN OHM");
    TEST_ASSERT(meter.get_meter_side_bus()==2);

    meter = setter.prepare_line_apparent_impedance_angle_in_deg_meter(did,1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="APPARENT IMPEDANCE ANGLE IN DEG");
    TEST_ASSERT(meter.get_meter_side_bus()==1);

    meter = setter.prepare_line_apparent_impedance_angle_in_deg_meter(did,2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="APPARENT IMPEDANCE ANGLE IN DEG");
    TEST_ASSERT(meter.get_meter_side_bus()==2);

    meter = setter.prepare_line_current_in_pu_meter(did,1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="CURRENT IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==1);

    meter = setter.prepare_line_current_in_pu_meter(did,2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="CURRENT IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==2);

    meter = setter.prepare_line_active_power_in_pu_meter(did,1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ACTIVE POWER IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==1);

    meter = setter.prepare_line_active_power_in_pu_meter(did,2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ACTIVE POWER IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==2);

    meter = setter.prepare_line_reactive_power_in_pu_meter(did,1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="REACTIVE POWER IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==1);

    meter = setter.prepare_line_reactive_power_in_pu_meter(did,2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="REACTIVE POWER IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==2);

    meter = setter.prepare_line_apparent_impedance_in_pu_meter(did,1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="APPARENT IMPEDANCE IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==1);

    meter = setter.prepare_line_apparent_impedance_in_pu_meter(did,2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="APPARENT IMPEDANCE IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==2);

    meter = setter.prepare_line_apparent_impedance_angle_in_rad_meter(did,1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="APPARENT IMPEDANCE ANGLE IN RAD");
    TEST_ASSERT(meter.get_meter_side_bus()==1);

    meter = setter.prepare_line_apparent_impedance_angle_in_rad_meter(did,2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="APPARENT IMPEDANCE ANGLE IN RAD");
    TEST_ASSERT(meter.get_meter_side_bus()==2);
}

void METER_SETTER_TEST::test_prepare_transformer_related_meters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_SETTER_TEST");

    prepare_basic_transformers();

    METER meter;
    meter.set_toolkit(default_toolkit);

    DEVICE_ID did;
    TERMINAL terminal;

    did.set_device_type("TRANSFORMER");
    terminal.append_bus(1);
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1#");

    meter = setter.prepare_transformer_current_in_kA_meter(did,1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="CURRENT IN KA");
    TEST_ASSERT(meter.get_meter_side_bus()==1);

    meter = setter.prepare_transformer_current_in_kA_meter(did,2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="CURRENT IN KA");
    TEST_ASSERT(meter.get_meter_side_bus()==2);

    meter = setter.prepare_transformer_active_power_in_MW_meter(did,1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ACTIVE POWER IN MW");
    TEST_ASSERT(meter.get_meter_side_bus()==1);

    meter = setter.prepare_transformer_active_power_in_MW_meter(did,2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ACTIVE POWER IN MW");
    TEST_ASSERT(meter.get_meter_side_bus()==2);

    meter = setter.prepare_transformer_reactive_power_in_MVar_meter(did,1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="REACTIVE POWER IN MVAR");
    TEST_ASSERT(meter.get_meter_side_bus()==1);

    meter = setter.prepare_transformer_reactive_power_in_MVar_meter(did,2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="REACTIVE POWER IN MVAR");
    TEST_ASSERT(meter.get_meter_side_bus()==2);

    meter = setter.prepare_transformer_current_in_pu_meter(did,1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="CURRENT IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==1);

    meter = setter.prepare_transformer_current_in_pu_meter(did,2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="CURRENT IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==2);

    meter = setter.prepare_transformer_active_power_in_pu_meter(did,1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ACTIVE POWER IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==1);

    meter = setter.prepare_transformer_active_power_in_pu_meter(did,2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ACTIVE POWER IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==2);

    meter = setter.prepare_transformer_reactive_power_in_pu_meter(did,1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="REACTIVE POWER IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==1);

    meter = setter.prepare_transformer_reactive_power_in_pu_meter(did,2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="REACTIVE POWER IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==2);

    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1#");

    meter = setter.prepare_transformer_current_in_kA_meter(did,1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="CURRENT IN KA");
    TEST_ASSERT(meter.get_meter_side_bus()==1);

    meter = setter.prepare_transformer_current_in_kA_meter(did,2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="CURRENT IN KA");
    TEST_ASSERT(meter.get_meter_side_bus()==2);

    meter = setter.prepare_transformer_current_in_kA_meter(did,3);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="CURRENT IN KA");
    TEST_ASSERT(meter.get_meter_side_bus()==3);

    meter = setter.prepare_transformer_active_power_in_MW_meter(did,1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ACTIVE POWER IN MW");
    TEST_ASSERT(meter.get_meter_side_bus()==1);

    meter = setter.prepare_transformer_active_power_in_MW_meter(did,2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ACTIVE POWER IN MW");
    TEST_ASSERT(meter.get_meter_side_bus()==2);

    meter = setter.prepare_transformer_active_power_in_MW_meter(did,3);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ACTIVE POWER IN MW");
    TEST_ASSERT(meter.get_meter_side_bus()==3);

    meter = setter.prepare_transformer_reactive_power_in_MVar_meter(did,1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="REACTIVE POWER IN MVAR");
    TEST_ASSERT(meter.get_meter_side_bus()==1);

    meter = setter.prepare_transformer_reactive_power_in_MVar_meter(did,2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="REACTIVE POWER IN MVAR");
    TEST_ASSERT(meter.get_meter_side_bus()==2);

    meter = setter.prepare_transformer_reactive_power_in_MVar_meter(did,3);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="REACTIVE POWER IN MVAR");
    TEST_ASSERT(meter.get_meter_side_bus()==3);

    meter = setter.prepare_transformer_current_in_pu_meter(did,1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="CURRENT IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==1);

    meter = setter.prepare_transformer_current_in_pu_meter(did,2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="CURRENT IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==2);

    meter = setter.prepare_transformer_current_in_pu_meter(did,3);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="CURRENT IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==3);

    meter = setter.prepare_transformer_active_power_in_pu_meter(did,1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ACTIVE POWER IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==1);

    meter = setter.prepare_transformer_active_power_in_pu_meter(did,2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ACTIVE POWER IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==2);

    meter = setter.prepare_transformer_active_power_in_pu_meter(did,3);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ACTIVE POWER IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==3);

    meter = setter.prepare_transformer_reactive_power_in_pu_meter(did,1);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="REACTIVE POWER IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==1);

    meter = setter.prepare_transformer_reactive_power_in_pu_meter(did,2);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="REACTIVE POWER IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==2);

    meter = setter.prepare_transformer_reactive_power_in_pu_meter(did,3);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="REACTIVE POWER IN PU");
    TEST_ASSERT(meter.get_meter_side_bus()==3);
}

void METER_SETTER_TEST::test_prepare_load_related_meters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_SETTER_TEST");

    prepare_basic_loads();

    METER meter;
    meter.set_toolkit(default_toolkit);

    DEVICE_ID did;
    TERMINAL terminal;

    did.set_device_type("LOAD");
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1#");

    meter = setter.prepare_load_active_power_in_MW_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ACTIVE POWER IN MW");

    meter = setter.prepare_load_reactive_power_in_MVar_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="REACTIVE POWER IN MVAR");

    meter = setter.prepare_load_current_in_kA_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="CURRENT IN KA");

    meter = setter.prepare_load_active_power_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ACTIVE POWER IN PU");

    meter = setter.prepare_load_reactive_power_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="REACTIVE POWER IN PU");

    meter = setter.prepare_load_current_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="CURRENT IN PU");

    meter = setter.prepare_load_total_scale_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TOTAL SCALE IN PU");

    meter = setter.prepare_load_manually_scale_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="MANUALLY SCALE IN PU");

    meter = setter.prepare_load_relay_shed_scale_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="RELAY SHED SCALE IN PU");

    meter = setter.prepare_load_load_model_internal_variable_meter(did,"TOTAL ACTIVE POWER LOAD IN MW");
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="LOAD MODEL INTERNAL VARIABLE");
    TEST_ASSERT(meter.get_internal_variable_name()=="TOTAL ACTIVE POWER LOAD IN MW");

    meter = setter.prepare_load_frequency_relay_model_internal_variable_meter(did,"SHED SCALE IN PU");
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="FREQUENCY RELAY MODEL INTERNAL VARIABLE");
    TEST_ASSERT(meter.get_internal_variable_name()=="SHED SCALE IN PU");

    meter = setter.prepare_load_voltage_relay_model_internal_variable_meter(did,"SHED SCALE IN PU");
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="VOLTAGE RELAY MODEL INTERNAL VARIABLE");
    TEST_ASSERT(meter.get_internal_variable_name()=="SHED SCALE IN PU");
}

void METER_SETTER_TEST::test_prepare_generator_related_meters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_SETTER_TEST");

    prepare_basic_generators();

    METER meter;
    meter.set_toolkit(default_toolkit);

    DEVICE_ID did;
    TERMINAL terminal;

    did.set_device_type("GENERATOR");
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1#");

    meter = setter.prepare_generator_rotor_angle_in_deg_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ROTOR ANGLE IN DEG");

    meter = setter.prepare_generator_rotor_speed_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ROTOR SPEED IN PU");

    meter = setter.prepare_generator_rotor_speed_in_Hz_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ROTOR SPEED IN HZ");

    meter = setter.prepare_generator_rotor_speed_deviation_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ROTOR SPEED DEVIATION IN PU");

    meter = setter.prepare_generator_rotor_speed_deviation_in_Hz_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ROTOR SPEED DEVIATION IN HZ");

    meter = setter.prepare_generator_terminal_voltage_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TERMINAL VOLTAGE IN PU");

    meter = setter.prepare_generator_internal_voltage_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="INTERNAL VOLTAGE IN PU");

    meter = setter.prepare_generator_terminal_current_in_pu_on_mbase_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TERMINAL CURRENT IN PU ON MBASE");

    meter = setter.prepare_generator_terminal_current_in_pu_on_sbase_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TERMINAL CURRENT IN PU ON SBASE");

    meter = setter.prepare_generator_terminal_current_in_kA_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TERMINAL CURRENT IN KA");

    meter = setter.prepare_generator_terminal_active_power_in_pu_on_mbase_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TERMINAL ACTIVE POWER IN PU ON MBASE");

    meter = setter.prepare_generator_terminal_active_power_in_pu_on_sbase_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TERMINAL ACTIVE POWER IN PU ON SBASE");

    meter = setter.prepare_generator_terminal_active_power_in_MW_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TERMINAL ACTIVE POWER IN MW");

    meter = setter.prepare_generator_terminal_reactive_power_in_pu_on_mbase_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TERMINAL REACTIVE POWER IN PU ON MBASE");

    meter = setter.prepare_generator_terminal_reactive_power_in_pu_on_sbase_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TERMINAL REACTIVE POWER IN PU ON SBASE");

    meter = setter.prepare_generator_terminal_reactive_power_in_MVar_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TERMINAL REACTIVE POWER IN MVAR");

    meter = setter.prepare_generator_airgap_power_in_pu_on_mbase_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="AIRGAP POWER IN PU ON MBASE");

    meter = setter.prepare_generator_airgap_power_in_pu_on_sbase_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="AIRGAP POWER IN PU ON SBASE");

    meter = setter.prepare_generator_airgap_power_in_MW_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="AIRGAP POWER IN MW");

    meter = setter.prepare_generator_accelerating_power_in_pu_on_mbase_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ACCELERATING POWER IN PU ON MBASE");

    meter = setter.prepare_generator_accelerating_power_in_pu_on_sbase_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ACCELERATING POWER IN PU ON SBASE");

    meter = setter.prepare_generator_accelerating_power_in_MW_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ACCELERATING POWER IN MW");

    meter = setter.prepare_generator_mechanical_power_in_pu_on_mbase_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="MECHANICAL POWER IN PU ON MBASE");

    meter = setter.prepare_generator_mechanical_power_in_pu_on_sbase_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="MECHANICAL POWER IN PU ON SBASE");

    meter = setter.prepare_generator_mechanical_power_in_MW_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="MECHANICAL POWER IN MW");

    meter = setter.prepare_generator_excitation_voltage_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="EXCITATION VOLTAGE IN PU");

    meter = setter.prepare_generator_compensated_voltage_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="COMPENSATED VOLTAGE IN PU");

    meter = setter.prepare_generator_stabilizing_signal_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="STABILIZING SIGNAL IN PU");

    meter = setter.prepare_generator_sync_generator_model_internal_variable_meter(did, "STATE@ROTOR SPEED BLOCK");
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="SYNC GENERATOR MODEL INTERNAL VARIABLE");
    TEST_ASSERT(meter.get_internal_variable_name()=="STATE@ROTOR SPEED BLOCK");

    meter = setter.prepare_generator_compensator_model_internal_variable_meter(did, "COMPENSATED VOLTAGE IN PU");
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="COMPENSATOR MODEL INTERNAL VARIABLE");
    TEST_ASSERT(meter.get_internal_variable_name()=="");

    meter = setter.prepare_generator_stabilizer_model_internal_variable_meter(did, "STATE@PHASE TUNER 3");
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="STABILIZER MODEL INTERNAL VARIABLE");
    TEST_ASSERT(meter.get_internal_variable_name()=="STATE@PHASE TUNER 3");

    meter = setter.prepare_generator_exciter_model_internal_variable_meter(did, "STATE@EXCITER");
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="EXCITER MODEL INTERNAL VARIABLE");
    TEST_ASSERT(meter.get_internal_variable_name()=="STATE@EXCITER");

    meter = setter.prepare_generator_turbine_governor_model_internal_variable_meter(did, "STATE@DELAYER 4");
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TURBINE GOVERNOR MODEL INTERNAL VARIABLE");
    TEST_ASSERT(meter.get_internal_variable_name()=="STATE@DELAYER 4");

    meter = setter.prepare_generator_turbine_load_controller_model_internal_variable_meter(did, "STATE@POWER SENSOR");
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TURBINE LOAD CONTROLLER MODEL INTERNAL VARIABLE");
    TEST_ASSERT(meter.get_internal_variable_name()=="STATE@POWER SENSOR");
}

void METER_SETTER_TEST::test_prepare_wt_generator_related_meters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_SETTER_TEST");

    prepare_basic_wt_generators();

    METER meter;
    meter.set_toolkit(default_toolkit);

    DEVICE_ID did;
    TERMINAL terminal;

    did.set_device_type("WT GENERATOR");
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1#");

    meter = setter.prepare_wt_generator_terminal_current_in_pu_on_mbase_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TERMINAL CURRENT IN PU ON MBASE");

    meter = setter.prepare_wt_generator_terminal_current_in_pu_on_sbase_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TERMINAL CURRENT IN PU ON SBASE");

    meter = setter.prepare_wt_generator_terminal_current_in_kA_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TERMINAL CURRENT IN KA");

    meter = setter.prepare_wt_generator_terminal_active_power_in_MW_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TERMINAL ACTIVE POWER IN MW");

    meter = setter.prepare_wt_generator_terminal_reactive_power_in_MVar_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TERMINAL REACTIVE POWER IN MVAR");

    meter = setter.prepare_wt_generator_mechanical_power_in_MW_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="MECHANICAL POWER IN MW");

    meter = setter.prepare_wt_generator_max_available_mechanical_power_in_MW_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="MAX AVAILABLE MECHANICAL POWER IN MW");

    meter = setter.prepare_wt_generator_speed_reference_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="SPEED REFERENCE IN PU");

    meter = setter.prepare_wt_generator_speed_reference_in_rad_per_s_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="SPEED REFERENCE IN RAD/S");

    meter = setter.prepare_wt_generator_turbine_speed_deviation_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TURBINE SPEED DEVIATION IN PU");

    meter = setter.prepare_wt_generator_turbine_speed_deviation_in_Hz_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TURBINE SPEED DEVIATION IN HZ");

    meter = setter.prepare_wt_generator_turbine_speed_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TURBINE SPEED IN PU");

    meter = setter.prepare_wt_generator_turbine_speed_in_Hz_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TURBINE SPEED IN HZ");

    meter = setter.prepare_wt_generator_rotor_speed_deviation_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ROTOR SPEED DEVIATION IN PU");

    meter = setter.prepare_wt_generator_rotor_speed_deviation_in_Hz_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ROTOR SPEED DEVIATION IN HZ");

    meter = setter.prepare_wt_generator_rotor_speed_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ROTOR SPEED IN PU");

    meter = setter.prepare_wt_generator_rotor_speed_in_Hz_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ROTOR SPEED IN HZ");

    meter = setter.prepare_wt_generator_rotor_angle_in_deg_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ROTOR ANGLE IN DEG");

    meter = setter.prepare_wt_generator_rotor_angle_in_rad_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ROTOR ANGLE IN RAD");

    meter = setter.prepare_wt_generator_active_current_command_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ACTIVE CURRENT COMMAND IN PU");

    meter = setter.prepare_wt_generator_reactive_current_command_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="REACTIVE CURRENT COMMAND IN PU");

    meter = setter.prepare_wt_generator_active_power_command_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ACTIVE POWER COMMAND IN PU");

    meter = setter.prepare_wt_generator_reactive_power_command_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="REACTIVE POWER COMMAND IN PU");

    meter = setter.prepare_wt_generator_reactive_voltage_command_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="REACTIVE VOLTAGE COMMAND IN PU");

    meter = setter.prepare_wt_generator_pitch_angle_in_deg_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="PITCH ANGLE IN DEG");

    meter = setter.prepare_wt_generator_wind_speed_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="WIND SPEED IN PU");

    meter = setter.prepare_wt_generator_wind_speed_in_mps_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="WIND SPEED IN MPS");

    meter = setter.prepare_wt_generator_model_internal_variable_meter(did, "STATE@LVPL VOLTAGE SENSOR");
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="WT GENERATOR MODEL INTERNAL VARIABLE");
    TEST_ASSERT(meter.get_internal_variable_name()=="STATE@LVPL VOLTAGE SENSOR");

    meter = setter.prepare_wt_aerodynamic_model_internal_variable_meter(did, "BLADE RADIUS IN M");
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="WT AERODYNAMIC MODEL INTERNAL VARIABLE");
    TEST_ASSERT(meter.get_internal_variable_name()=="BLADE RADIUS IN M");

    meter = setter.prepare_wt_turbine_model_internal_variable_meter(did, "STATE@TURBINE SPEED BLOCK");
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="WT TURBINE MODEL INTERNAL VARIABLE");
    TEST_ASSERT(meter.get_internal_variable_name()=="STATE@TURBINE SPEED BLOCK");

    meter = setter.prepare_wt_electrical_model_internal_variable_meter(did, "STATE@PRIMARY FREQUENCY CONTROL");
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="WT ELECTRICAL MODEL INTERNAL VARIABLE");
    TEST_ASSERT(meter.get_internal_variable_name()=="STATE@PRIMARY FREQUENCY CONTROL");

    meter = setter.prepare_wt_pitch_model_internal_variable_meter(did, "STATE@PITCH INTEGRATOR");
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="WT PITCH MODEL INTERNAL VARIABLE");
    TEST_ASSERT(meter.get_internal_variable_name()=="STATE@PITCH INTEGRATOR");

    meter = setter.prepare_wind_speed_model_internal_variable_meter(did, "WIND SPEED IN M/S");
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="WIND SPEED MODEL INTERNAL VARIABLE");
    TEST_ASSERT(meter.get_internal_variable_name()=="WIND SPEED IN M/S");
}

void METER_SETTER_TEST::test_prepare_pv_unit_related_meters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_SETTER_TEST");

    prepare_basic_pv_units();

    METER meter;
    meter.set_toolkit(default_toolkit);

    DEVICE_ID did;
    TERMINAL terminal;

    did.set_device_type("PV UNIT");
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1#");

    meter = setter.prepare_pv_unit_terminal_current_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TERMINAL CURRENT IN PU");

    meter = setter.prepare_pv_unit_terminal_current_in_kA_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TERMINAL CURRENT IN KA");

    meter = setter.prepare_pv_unit_terminal_active_power_in_MW_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TERMINAL ACTIVE POWER IN MW");

    meter = setter.prepare_pv_unit_terminal_reactive_power_in_MVar_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TERMINAL REACTIVE POWER IN MVAR");

    meter = setter.prepare_pv_unit_active_current_command_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ACTIVE CURRENT COMMAND IN PU");

    meter = setter.prepare_pv_unit_reactive_current_command_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="REACTIVE CURRENT COMMAND IN PU");

    meter = setter.prepare_pv_unit_active_power_command_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ACTIVE POWER COMMAND IN PU");

    meter = setter.prepare_pv_unit_reactive_power_command_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="REACTIVE POWER COMMAND IN PU");

    meter = setter.prepare_pv_unit_reactive_voltage_command_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="REACTIVE VOLTAGE COMMAND IN PU");

    /*meter = setter.prepare_pv_panel_model_internal_variable_meter(did, "STATE@LVPL VOLTAGE SENSOR");
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="PV PANEL MODEL INTERNAL VARIABLE");
    TEST_ASSERT(meter.get_internal_variable_name()=="STATE@LVPL VOLTAGE SENSOR");

    meter = setter.prepare_pv_converter_model_internal_variable_meter(did, "BLADE RADIUS IN M");
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="PV CONVERTER MODEL INTERNAL VARIABLE");
    TEST_ASSERT(meter.get_internal_variable_name()=="BLADE RADIUS IN M");

    meter = setter.prepare_pv_electrical_model_internal_variable_meter(did, "STATE@TURBINE");
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="PV ELECTRICAL MODEL INTERNAL VARIABLE");
    TEST_ASSERT(meter.get_internal_variable_name()=="STATE@TURBINE");

    meter = setter.prepare_pv_irradiance_model_internal_variable_meter(did, "STATE@PRIMARY FREQUENCY CONTROL");
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="PV IRRADIANCE MODEL INTERNAL VARIABLE");
    TEST_ASSERT(meter.get_internal_variable_name()=="STATE@PRIMARY FREQUENCY CONTROL");*/
}

void METER_SETTER_TEST::test_prepare_energy_storage_related_meters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_SETTER_TEST");

    prepare_basic_energy_strorages();

    METER meter;
    meter.set_toolkit(default_toolkit);

    DEVICE_ID did;
    TERMINAL terminal;

    did.set_device_type("ENERGY STORAGE");
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    meter = setter.prepare_energy_storage_state_of_energy_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="STATE OF ENERGY IN PU");

    meter = setter.prepare_energy_storage_active_power_in_MW_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ACTIVE POWER IN MW");

    meter = setter.prepare_energy_storage_active_power_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ACTIVE POWER IN PU");

    meter = setter.prepare_energy_storage_reactive_power_in_MVar_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="REACTIVE POWER IN MVAR");

    meter = setter.prepare_energy_storage_reactive_power_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="REACTIVE POWER IN PU");

    meter = setter.prepare_energy_storage_terminal_current_in_kA_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TERMINAL CURRENT IN KA");

    meter = setter.prepare_energy_storage_terminal_current_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="TERMINAL CURRENT IN PU");

    meter = setter.prepare_energy_storage_model_internal_variable_meter(did, "STATE OF ENERGY");
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ENERGY STORAGE MODEL INTERNAL VARIABLE");
    TEST_ASSERT(meter.get_internal_variable_name()=="STATE OF ENERGY");
}

void METER_SETTER_TEST::test_prepare_hvdc_related_meters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_SETTER_TEST");

    prepare_basic_hvdcs();

    METER meter;
    meter.set_toolkit(default_toolkit);

    DEVICE_ID did;
    TERMINAL terminal;

    did.set_device_type("HVDC");
    terminal.append_bus(1);
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("P");

    meter = setter.prepare_hvdc_dc_current_in_kA_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="DC CURRENT IN KA");

    meter = setter.prepare_hvdc_rectifier_dc_current_in_kA_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="RECTIFIER DC CURRENT IN KA");

    meter = setter.prepare_hvdc_inverter_dc_current_in_kA_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="INVERTER DC CURRENT IN KA");

    meter = setter.prepare_hvdc_rectifier_alpha_in_deg_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="RECTIFIER ALPHA IN DEG");

    meter = setter.prepare_hvdc_inverter_gamma_in_deg_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="INVERTER GAMMA IN DEG");

    meter = setter.prepare_hvdc_rectifier_mu_in_deg_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="RECTIFIER MU IN DEG");

    meter = setter.prepare_hvdc_inverter_mu_in_deg_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="INVERTER MU IN DEG");

    meter = setter.prepare_hvdc_rectifier_dc_voltage_in_kV_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="RECTIFIER DC VOLTAGE IN KV");

    meter = setter.prepare_hvdc_inverter_dc_voltage_in_kV_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="INVERTER DC VOLTAGE IN KV");

    meter = setter.prepare_hvdc_rectifier_ac_voltage_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="RECTIFIER AC VOLTAGE IN PU");

    meter = setter.prepare_hvdc_inverter_ac_voltage_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="INVERTER AC VOLTAGE IN PU");

    meter = setter.prepare_hvdc_rectifier_dc_power_in_MW_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="RECTIFIER DC POWER IN MW");

    meter = setter.prepare_hvdc_inverter_dc_power_in_MW_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="INVERTER DC POWER IN MW");

    meter = setter.prepare_hvdc_rectifier_ac_active_power_in_MW_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="RECTIFIER AC ACTIVE POWER IN MW");

    meter = setter.prepare_hvdc_inverter_ac_active_power_in_MW_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="INVERTER AC ACTIVE POWER IN MW");

    meter = setter.prepare_hvdc_rectifier_ac_reactive_power_in_MVar_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="RECTIFIER AC REACTIVE POWER IN MVAR");

    meter = setter.prepare_hvdc_inverter_ac_reactive_power_in_MVar_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="INVERTER AC REACTIVE POWER IN MVAR");

    meter = setter.prepare_hvdc_model_internal_variable_meter(did, "STATE@INVERTER DC VOLTAGE SENSOR");
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="HVDC MODEL INTERNAL VARIABLE");
    TEST_ASSERT(meter.get_internal_variable_name()=="STATE@INVERTER DC VOLTAGE SENSOR");
}

void METER_SETTER_TEST::test_prepare_equivalent_device_related_meters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_SETTER_TEST");

    prepare_basic_equivalent_devices();

    METER meter;
    meter.set_toolkit(default_toolkit);

    DEVICE_ID did;
    TERMINAL terminal;

    did.set_device_type("EQUIVALENT DEVICE");
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("E1");

    meter = setter.prepare_equivalent_device_voltage_source_voltage_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="VOLTAGE SOURCE VOLTAGE IN PU");

    meter = setter.prepare_equivalent_device_voltage_source_voltage_angle_in_deg_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="VOLTAGE SOURCE VOLTAGE ANGLE IN DEG");

    meter = setter.prepare_equivalent_device_voltage_source_resistance_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="VOLTAGE SOURCE RESISTANCE IN PU");

    meter = setter.prepare_equivalent_device_voltage_source_reactance_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="VOLTAGE SOURCE REACTANCE IN PU");

    meter = setter.prepare_equivalent_device_nominal_active_constant_power_load_in_MW_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ACTIVE CONSTANT POWER LOAD IN MW");

    meter = setter.prepare_equivalent_device_nominal_active_constant_current_load_in_MW_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ACTIVE CONSTANT CURRENT LOAD IN MW");

    meter = setter.prepare_equivalent_device_nominal_active_constant_impedance_load_in_MW_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ACTIVE CONSTANT IMPEDANCE LOAD IN MW");

    meter = setter.prepare_equivalent_device_nominal_reactive_constant_power_load_in_MVar_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="REACTIVE CONSTANT POWER LOAD IN MVAR");

    meter = setter.prepare_equivalent_device_nominal_reactive_constant_current_load_in_MVar_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="REACTIVE CONSTANT CURRENT LOAD IN MVAR");

    meter = setter.prepare_equivalent_device_nominal_reactive_constant_impedance_load_in_MVar_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="REACTIVE CONSTANT IMPEDANCE LOAD IN MVAR");

    meter = setter.prepare_equivalent_device_active_power_generation_in_MW_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ACTIVE POWER GENERATION IN MW");

    meter = setter.prepare_equivalent_device_reactive_power_generation_in_MVar_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="REACTIVE POWER GENERATION IN MVAR");

    meter = setter.prepare_equivalent_device_active_power_load_in_MW_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ACTIVE POWER LOAD IN MW");

    meter = setter.prepare_equivalent_device_reactive_power_load_in_MVar_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="REACTIVE POWER LOAD IN MVAR");

    meter = setter.prepare_equivalent_device_active_power_net_load_in_MW_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ACTIVE POWER NET LOAD IN MW");

    meter = setter.prepare_equivalent_device_reactive_power_net_load_in_MVar_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="REACTIVE POWER NET LOAD IN MVAR");

    meter = setter.prepare_equivalent_device_nominal_active_constant_power_load_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ACTIVE CONSTANT POWER LOAD IN PU");

    meter = setter.prepare_equivalent_device_nominal_active_constant_current_load_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ACTIVE CONSTANT CURRENT LOAD IN PU");

    meter = setter.prepare_equivalent_device_nominal_active_constant_impedance_load_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ACTIVE CONSTANT IMPEDANCE LOAD IN PU");

    meter = setter.prepare_equivalent_device_nominal_reactive_constant_power_load_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="REACTIVE CONSTANT POWER LOAD IN PU");

    meter = setter.prepare_equivalent_device_nominal_reactive_constant_current_load_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="REACTIVE CONSTANT CURRENT LOAD IN PU");

    meter = setter.prepare_equivalent_device_nominal_reactive_constant_impedance_load_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="REACTIVE CONSTANT IMPEDANCE LOAD IN PU");

    meter = setter.prepare_equivalent_device_active_power_generation_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ACTIVE POWER GENERATION IN PU");

    meter = setter.prepare_equivalent_device_reactive_power_generation_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="REACTIVE POWER GENERATION IN PU");

    meter = setter.prepare_equivalent_device_active_power_load_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ACTIVE POWER LOAD IN PU");

    meter = setter.prepare_equivalent_device_reactive_power_load_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="REACTIVE POWER LOAD IN PU");

    meter = setter.prepare_equivalent_device_active_power_net_load_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="ACTIVE POWER NET LOAD IN PU");

    meter = setter.prepare_equivalent_device_reactive_power_net_load_in_pu_meter(did);
    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="REACTIVE POWER NET LOAD IN PU");
}


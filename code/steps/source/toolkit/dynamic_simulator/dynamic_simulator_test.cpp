#include "header/basic/test_macro.h"
#include "header/toolkit/dynamic_simulator/dynamic_simulator_test.h"
#include "header/basic/utility.h"
#include "header/data_imexporter/psse_imexporter.h"
#include "header/prepare_for_tests/prepare_models_for_test.h"
#include "header/meter/meter_setter.h"
#include "header/steps_namespace.h"
#include "header/toolkit/powerflow_solver/powerflow_solver.h"
#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <cstdio>
#include <cmath>

#ifdef ENABLE_STEPS_TEST
using namespace std;

DYNAMICS_SIMULATOR_TEST::DYNAMICS_SIMULATOR_TEST()
{
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_constructor);

    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_set_get_bin_file_export_enable_flag);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_set_get_csv_file_export_enable_flag);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_set_get_json_file_export_enable_flag);

    //TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_set_get_current_simulation_time);

    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_set_get_DELT);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_set_get_TIME);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_set_get_max_DAE_iteration);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_set_get_min_DAE_iteration);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_set_get_max_network_iteration);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_set_get_max_update_iteration);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_set_get_max_event_update_iteration);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_set_get_max_network_solution_divergent_threshold);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_set_get_allowed_max_power_imbalance_in_MVA);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_set_get_iteration_accelerator);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_set_get_rotor_angle_stability_surveillance_flag);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_set_get_rotor_angle_stability_threshold);

    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_append_and_get_meter);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_get_meter_count);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_get_all_meters_value);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_clear_meters);

    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_set_get_output_file);

    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_run_single_machine_model);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_run_IEEE_9_bus_classical_model);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_run_IEEE_39_bus_model);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_run_SD_133_bus_model);

    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_run_bench_shandong_100_bus_model_with_dc_GENROU_CDC4T);
}

void DYNAMICS_SIMULATOR_TEST::setup()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.clear();
    psdb.set_allowed_max_bus_number(10000);
    DYNAMICS_SIMULATOR& simulator = default_toolkit.get_dynamic_simulator();
    default_toolkit.set_dynamic_simulation_time_step_in_s(0.01);
    simulator.set_allowed_max_power_imbalance_in_MVA(0.001);
    simulator.set_csv_file_export_enable_flag(true);
}

void DYNAMICS_SIMULATOR_TEST::tear_down()
{
    default_toolkit.clear();

    show_test_end_information();
}

void DYNAMICS_SIMULATOR_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = default_toolkit.get_dynamic_simulator();
    TEST_ASSERT(simulator.get_meter_count()==0);
    TEST_ASSERT(simulator.get_max_DAE_iteration()==100);
    TEST_ASSERT(simulator.get_min_DAE_iteration()==3);
    TEST_ASSERT(simulator.get_max_network_iteration()==50);
    TEST_ASSERT(simulator.get_max_update_iteration()==10);
    TEST_ASSERT(simulator.get_max_network_solution_divergent_threshold()==3);
    TEST_ASSERT(fabs(simulator.get_allowed_max_power_imbalance_in_MVA()-0.001)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(simulator.get_iteration_accelerator()-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(simulator.is_bin_file_export_enabled()==false);
    TEST_ASSERT(simulator.is_csv_file_export_enabled()==true);
    TEST_ASSERT(simulator.is_json_file_export_enabled()==false);
}

void DYNAMICS_SIMULATOR_TEST::test_set_get_bin_file_export_enable_flag()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = default_toolkit.get_dynamic_simulator();
    simulator.set_bin_file_export_enable_flag(true);
    TEST_ASSERT(simulator.is_bin_file_export_enabled()==true);
    simulator.set_bin_file_export_enable_flag(false);
    TEST_ASSERT(simulator.is_bin_file_export_enabled()==false);
}

void DYNAMICS_SIMULATOR_TEST::test_set_get_csv_file_export_enable_flag()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = default_toolkit.get_dynamic_simulator();
    simulator.set_csv_file_export_enable_flag(true);
    TEST_ASSERT(simulator.is_csv_file_export_enabled()==true);
    simulator.set_csv_file_export_enable_flag(false);
    TEST_ASSERT(simulator.is_csv_file_export_enabled()==false);
}

void DYNAMICS_SIMULATOR_TEST::test_set_get_json_file_export_enable_flag()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = default_toolkit.get_dynamic_simulator();
    simulator.set_json_file_export_enable_flag(true);
    TEST_ASSERT(simulator.is_json_file_export_enabled()==true);
    simulator.set_json_file_export_enable_flag(false);
    TEST_ASSERT(simulator.is_json_file_export_enabled()==false);
}

/*
void DYNAMICS_SIMULATOR_TEST::test_set_get_current_simulation_time()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    simulator.set_current_simulation_time_in_s(2.0);
    TEST_ASSERT(fabs(simulator.get_current_simulation_time_in_s()-2.0)<FLOAT_EPSILON);
    simulator.set_current_simulation_time_in_s(3.0);
    TEST_ASSERT(fabs(simulator.get_current_simulation_time_in_s()-3.0)<FLOAT_EPSILON);
}
*/
void DYNAMICS_SIMULATOR_TEST::test_set_get_DELT()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = default_toolkit.get_dynamic_simulator();
    simulator.set_dynamic_simulation_time_step_in_s(0.1);
    TEST_ASSERT(fabs(simulator.get_dynamic_simulation_time_step_in_s()-0.1)<FLOAT_EPSILON);
    simulator.set_dynamic_simulation_time_step_in_s(0.2);
    TEST_ASSERT(fabs(simulator.get_dynamic_simulation_time_step_in_s()-0.2)<FLOAT_EPSILON);
}
void DYNAMICS_SIMULATOR_TEST::test_set_get_TIME()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = default_toolkit.get_dynamic_simulator();
    simulator.set_dynamic_simulation_time_in_s(0.1);
    TEST_ASSERT(fabs(simulator.get_dynamic_simulation_time_in_s()-0.1)<FLOAT_EPSILON);
    simulator.set_dynamic_simulation_time_in_s(0.3);
    TEST_ASSERT(fabs(simulator.get_dynamic_simulation_time_in_s()-0.3)<FLOAT_EPSILON);
}
void DYNAMICS_SIMULATOR_TEST::test_set_get_max_DAE_iteration()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = default_toolkit.get_dynamic_simulator();
    simulator.set_max_DAE_iteration(200);
    TEST_ASSERT(simulator.get_max_DAE_iteration()==200);
    simulator.set_max_DAE_iteration(100);
    TEST_ASSERT(simulator.get_max_DAE_iteration()==100);
}

void DYNAMICS_SIMULATOR_TEST::test_set_get_min_DAE_iteration()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = default_toolkit.get_dynamic_simulator();
    simulator.set_min_DAE_iteration(20);
    TEST_ASSERT(simulator.get_min_DAE_iteration()==20);
    simulator.set_min_DAE_iteration(100);
    TEST_ASSERT(simulator.get_min_DAE_iteration()==100);
}

void DYNAMICS_SIMULATOR_TEST::test_set_get_max_network_iteration()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = default_toolkit.get_dynamic_simulator();
    simulator.set_max_network_iteration(200);
    TEST_ASSERT(simulator.get_max_network_iteration()==200);
    simulator.set_max_network_iteration(100);
    TEST_ASSERT(simulator.get_max_network_iteration()==100);
}

void DYNAMICS_SIMULATOR_TEST::test_set_get_max_update_iteration()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = default_toolkit.get_dynamic_simulator();
    simulator.set_max_update_iteration(200);
    TEST_ASSERT(simulator.get_max_update_iteration()==200);
    simulator.set_max_update_iteration(100);
    TEST_ASSERT(simulator.get_max_update_iteration()==100);
}

void DYNAMICS_SIMULATOR_TEST::test_set_get_max_event_update_iteration()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = default_toolkit.get_dynamic_simulator();
    simulator.set_max_event_update_iteration(200);
    TEST_ASSERT(simulator.get_max_event_update_iteration()==200);
    simulator.set_max_event_update_iteration(100);
    TEST_ASSERT(simulator.get_max_event_update_iteration()==100);
}

void DYNAMICS_SIMULATOR_TEST::test_set_get_max_network_solution_divergent_threshold()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = default_toolkit.get_dynamic_simulator();
    simulator.set_max_network_solution_divergent_threshold(2);
    TEST_ASSERT(simulator.get_max_network_solution_divergent_threshold()==2);
    simulator.set_max_network_solution_divergent_threshold(100);
    TEST_ASSERT(simulator.get_max_network_solution_divergent_threshold()==100);
    simulator.set_max_network_solution_divergent_threshold(1);
    TEST_ASSERT(simulator.get_max_network_solution_divergent_threshold()==1);
}

void DYNAMICS_SIMULATOR_TEST::test_set_get_allowed_max_power_imbalance_in_MVA()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = default_toolkit.get_dynamic_simulator();
    simulator.set_allowed_max_power_imbalance_in_MVA(0.001);
    TEST_ASSERT(fabs(simulator.get_allowed_max_power_imbalance_in_MVA()-0.001)<FLOAT_EPSILON);
}

void DYNAMICS_SIMULATOR_TEST::test_set_get_iteration_accelerator()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = default_toolkit.get_dynamic_simulator();
    simulator.set_iteration_accelerator(1.2);
    TEST_ASSERT(fabs(simulator.get_iteration_accelerator()-1.2)<FLOAT_EPSILON);
}

void DYNAMICS_SIMULATOR_TEST::test_set_get_rotor_angle_stability_surveillance_flag()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = default_toolkit.get_dynamic_simulator();
    TEST_ASSERT(simulator.get_rotor_angle_stability_surveillance_flag()==false);
    simulator.set_rotor_angle_stability_surveillance_flag(true);
    TEST_ASSERT(simulator.get_rotor_angle_stability_surveillance_flag()==true);
    simulator.set_rotor_angle_stability_surveillance_flag(false);
    TEST_ASSERT(simulator.get_rotor_angle_stability_surveillance_flag()==false);
}


void DYNAMICS_SIMULATOR_TEST::test_set_get_rotor_angle_stability_threshold()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = default_toolkit.get_dynamic_simulator();
    TEST_ASSERT(fabs(simulator.get_rotor_angle_stability_threshold_in_deg()-360.0)<FLOAT_EPSILON);
    simulator.set_rotor_angle_stability_threshold_in_deg(180.0);
    TEST_ASSERT(fabs(simulator.get_rotor_angle_stability_threshold_in_deg()-180.0)<FLOAT_EPSILON);
    simulator.set_rotor_angle_stability_threshold_in_deg(-170);
    TEST_ASSERT(fabs(simulator.get_rotor_angle_stability_threshold_in_deg()-170.0)<FLOAT_EPSILON);
}

void DYNAMICS_SIMULATOR_TEST::test_append_and_get_meter()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = default_toolkit.get_dynamic_simulator();
    prepare_IEEE_9_bus_model();
    prepare_IEEE_9_bus_model_classical_dynamic_model();

    METER meter(default_toolkit);
    DEVICE_ID did;
    TERMINAL terminal;

    terminal.clear();
    did.set_device_type(STEPS_BUS);
    terminal.append_bus(1);
    did.set_device_terminal(terminal);

    meter.set_device_id(did);
    meter.set_meter_type("VOLTAGE IN PU");

    simulator.append_meter(meter);

    TEST_ASSERT(simulator.get_meter_count()==1);

    meter = simulator.get_meter(0);

    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="VOLTAGE IN PU");

    terminal.clear();
    did.set_device_type(STEPS_BUS);
    terminal.append_bus(2);
    did.set_device_terminal(terminal);

    meter.set_device_id(did);
    meter.set_meter_type("VOLTAGE IN PU");

    simulator.append_meter(meter);

    TEST_ASSERT(simulator.get_meter_count()==2);

    meter = simulator.get_meter(1);

    TEST_ASSERT(meter.get_device_id()==did);
    TEST_ASSERT(meter.get_meter_type()=="VOLTAGE IN PU");
}

void DYNAMICS_SIMULATOR_TEST::test_get_meter_count()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = default_toolkit.get_dynamic_simulator();

    prepare_IEEE_9_bus_model();
    prepare_IEEE_9_bus_model_classical_dynamic_model();

    METER meter(default_toolkit);
    DEVICE_ID did;
    TERMINAL terminal;

    terminal.clear();
    did.set_device_type(STEPS_BUS);
    terminal.append_bus(1);
    did.set_device_terminal(terminal);

    meter.set_device_id(did);
    meter.set_meter_type("VOLTAGE IN PU");

    simulator.append_meter(meter);
    TEST_ASSERT(simulator.get_meter_count()==1);

    simulator.append_meter(meter);
    TEST_ASSERT(simulator.get_meter_count()==2);

    simulator.append_meter(meter);
    TEST_ASSERT(simulator.get_meter_count()==3);
}

void DYNAMICS_SIMULATOR_TEST::test_get_all_meters_value()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = default_toolkit.get_dynamic_simulator();
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    prepare_IEEE_9_bus_model();
    prepare_IEEE_9_bus_model_classical_dynamic_model();

    METER meter(default_toolkit);
    DEVICE_ID did;
    TERMINAL terminal;

    terminal.clear();
    did.set_device_type(STEPS_BUS);
    terminal.append_bus(1);
    did.set_device_terminal(terminal);

    meter.set_device_id(did);
    meter.set_meter_type("VOLTAGE IN PU");

    simulator.append_meter(meter);

    terminal.clear();
    did.set_device_type(STEPS_BUS);
    terminal.append_bus(2);
    did.set_device_terminal(terminal);

    meter.set_device_id(did);
    meter.set_meter_type("VOLTAGE IN PU");

    simulator.append_meter(meter);

    terminal.clear();
    did.set_device_type(STEPS_BUS);
    terminal.append_bus(3);
    did.set_device_terminal(terminal);

    meter.set_device_id(did);
    meter.set_meter_type("VOLTAGE IN PU");

    simulator.append_meter(meter);


    TEST_ASSERT(simulator.get_meter_count()==3);

    BUS* busptr = psdb.get_bus(1);
    busptr->set_positive_sequence_voltage_in_pu(1.0);
    busptr = psdb.get_bus(2);
    busptr->set_positive_sequence_voltage_in_pu(1.1);
    busptr = psdb.get_bus(3);
    busptr->set_positive_sequence_voltage_in_pu(0.9);

    vector<double> values = simulator.get_all_meters_value();

    TEST_ASSERT(values.size()==3)
    TEST_ASSERT(fabs(values[0]-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(values[1]-1.1)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(values[2]-0.9)<FLOAT_EPSILON);
}

void DYNAMICS_SIMULATOR_TEST::test_clear_meters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = default_toolkit.get_dynamic_simulator();

    prepare_IEEE_9_bus_model();
    prepare_IEEE_9_bus_model_classical_dynamic_model();

    METER meter(default_toolkit);
    DEVICE_ID did;
    TERMINAL terminal;

    terminal.clear();
    did.set_device_type(STEPS_BUS);
    terminal.append_bus(1);
    did.set_device_terminal(terminal);

    meter.set_device_id(did);
    meter.set_meter_type("VOLTAGE IN PU");

    simulator.append_meter(meter);
    simulator.append_meter(meter);
    simulator.append_meter(meter);
    TEST_ASSERT(simulator.get_meter_count()==3);

    simulator.clear_meters();
    TEST_ASSERT(simulator.get_meter_count()==0);
}

void DYNAMICS_SIMULATOR_TEST::test_set_get_output_file()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = default_toolkit.get_dynamic_simulator();
    simulator.set_output_file("steps out file 1");
    TEST_ASSERT(simulator.get_output_file()=="steps out file 1");
}


void DYNAMICS_SIMULATOR_TEST::test_run_single_machine_model()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = default_toolkit.get_dynamic_simulator();

    string file = "test_log/";
    file += __FUNCTION__;
    file += ".txt";
    default_toolkit.open_log_file(file);

    PSSE_IMEXPORTER importer(default_toolkit);

    importer.load_powerflow_data("../../../bench/single_machine_model.raw");
    importer.load_dynamic_data("../../../bench/single_machine_model.dyr");

    string output_file = "test_log/";
    output_file += __FUNCTION__;
    simulator.set_output_file(output_file);

    POWERFLOW_SOLVER& powerflow_solver = default_toolkit.get_powerflow_solver();

    powerflow_solver.set_max_iteration(30);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.00001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.00001);
    powerflow_solver.set_flat_start_logic(false);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_fast_decoupled_solution();

    default_toolkit.set_dynamic_simulation_time_step_in_s(0.01);
    simulator.set_output_file("test_log/test_run_single_machine_model");

    simulator.prepare_meters();

    simulator.start();
    simulator.run_to(1.0);

    DEVICE_ID  did;
    did.set_device_type(STEPS_AC_LINE);
    TERMINAL terminal;
    terminal.append_bus(3);
    terminal.append_bus(4);
    did.set_device_terminal(terminal);
    did.set_device_identifier_index(get_index_of_string("1"));

    simulator.set_ac_line_fault(did, 3, 0.0, complex<double>(0.0, -2e5));

    simulator.run_to(1.05);

    simulator.clear_ac_line_fault(did, 3, 0.0);

    simulator.run_to(5.0);

    default_toolkit.close_log_file();
}


void DYNAMICS_SIMULATOR_TEST::test_run_IEEE_9_bus_classical_model()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = default_toolkit.get_dynamic_simulator();

    string file = "test_log/";
    file += __FUNCTION__;
    file += ".txt";
    default_toolkit.open_log_file(file);

    PSSE_IMEXPORTER importer(default_toolkit);

    importer.load_powerflow_data("../../../bench/IEEE9_classical.raw");
    importer.load_dynamic_data("../../../bench/IEEE9_classical.dyr");

    POWERFLOW_SOLVER& powerflow_solver = default_toolkit.get_powerflow_solver();

    powerflow_solver.set_max_iteration(30);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.00001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.00001);
    powerflow_solver.set_flat_start_logic(false);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_fast_decoupled_solution();

    default_toolkit.set_dynamic_simulation_time_step_in_s(0.0083333);
    simulator.set_output_file("test_log/test_run_IEEE_9_bus_classical_model");

    simulator.prepare_meters();


    simulator.start();
    simulator.run_to(0.0);

    DEVICE_ID did;
    did.set_device_type(STEPS_AC_LINE);
    TERMINAL terminal;
    terminal.append_bus(5);
    terminal.append_bus(7);
    did.set_device_terminal(terminal);
    did.set_device_identifier_index(get_index_of_string("1"));

    simulator.set_ac_line_fault(did, 7, 0.0, complex<double>(0.0, -2e5));

    simulator.run_to(0.08333);

    simulator.clear_ac_line_fault(did, 7, 0.0);
    simulator.trip_ac_line(did);

    simulator.run_to(5.0);

    default_toolkit.close_log_file();
}



void DYNAMICS_SIMULATOR_TEST::test_run_IEEE_39_bus_model()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = default_toolkit.get_dynamic_simulator();

    ostringstream osstream;
    string file = "test_log/";
    file += __FUNCTION__;
    file += ".txt";
    default_toolkit.open_log_file(file);

    PSSE_IMEXPORTER importer(default_toolkit);

    importer.load_powerflow_data("../../../bench/IEEE39_model.raw");
    importer.load_dynamic_data("../../../bench/IEEE39_model.dyr");

    POWERFLOW_SOLVER& powerflow_solver = default_toolkit.get_powerflow_solver();

    powerflow_solver.set_max_iteration(30);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.00001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.00001);
    powerflow_solver.set_flat_start_logic(false);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_fast_decoupled_solution();

    simulator.prepare_meters();
    default_toolkit.set_dynamic_simulation_time_step_in_s(0.01);

    simulator.set_output_file("test_log/test_run_IEEE_39_bus_model");

    simulator.start();
    simulator.run_to(1.0);

    DEVICE_ID did;
    did.set_device_type(STEPS_GENERATOR);
    TERMINAL terminal;
    terminal.append_bus(30);
    did.set_device_terminal(terminal);
    did.set_device_identifier_index(get_index_of_string("1"));

    simulator.trip_generator(did);

    simulator.run_to(10);

    default_toolkit.close_log_file();
}

void DYNAMICS_SIMULATOR_TEST::test_run_SD_133_bus_model()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = default_toolkit.get_dynamic_simulator();
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    string file = "test_log/";
    file += __FUNCTION__;
    file += ".txt";
    default_toolkit.open_log_file(file);

    PSSE_IMEXPORTER importer(default_toolkit);

    psdb.set_allowed_max_bus_number(1000);

    importer.load_powerflow_data("../../../bench/SD133_model.raw");
    importer.load_dynamic_data("../../../bench/SD133_model.dyr");

    vector<LCC_HVDC2T*> hvdcs = psdb.get_all_2t_lcc_hvdcs();
    unsigned int n = hvdcs.size();
    for(unsigned int i=0; i!=n; ++i)
        hvdcs[i]->turn_rectifier_constant_power_mode_into_constant_current_mode();

    POWERFLOW_SOLVER& powerflow_solver = default_toolkit.get_powerflow_solver();

    powerflow_solver.set_max_iteration(30);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.00001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.00001);
    powerflow_solver.set_flat_start_logic(false);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_fast_decoupled_solution();

    simulator.prepare_meters();
    simulator.set_output_file("test_log/test_run_SD_133_bus_model");

    simulator.set_allowed_max_power_imbalance_in_MVA(0.001);
    default_toolkit.set_dynamic_simulation_time_step_in_s(0.01);

    simulator.start();
    simulator.run_to(1.0);

    DEVICE_ID did;
    did.set_device_type(STEPS_AC_LINE);
    TERMINAL terminal;
    terminal.append_bus(60);
    terminal.append_bus(62);
    did.set_device_terminal(terminal);
    did.set_device_identifier_index(get_index_of_string("1"));

    simulator.set_ac_line_fault(did, 60, 0.0, complex<double>(0.0, -1e6));

    simulator.run_to(1.1);

    simulator.clear_ac_line_fault(did, 60, 0.0);
    simulator.trip_ac_line(did);

    simulator.run_to(5.0);

    default_toolkit.close_log_file();
}


void DYNAMICS_SIMULATOR_TEST::test_run_IEEE_39_bus_model_with_wind()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = default_toolkit.get_dynamic_simulator();

    ostringstream osstream;
    string file = "test_log/";
    file += __FUNCTION__;
    file += ".txt";
    default_toolkit.open_log_file(file);

    PSSE_IMEXPORTER importer(default_toolkit);

    importer.load_powerflow_data("../../../bench/IEEE39_wind.raw");
    importer.load_dynamic_data("../../../bench/IEEE39_wind.dyr");

    POWERFLOW_SOLVER& powerflow_solver = default_toolkit.get_powerflow_solver();

    powerflow_solver.set_max_iteration(30);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.00001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.00001);
    powerflow_solver.set_flat_start_logic(false);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_fast_decoupled_solution();

    simulator.prepare_meters();
    default_toolkit.set_dynamic_simulation_time_step_in_s(0.01);

    simulator.set_output_file("test_log/IEEE_39_bus_model_with_wind");

    simulator.start();
    simulator.run_to(1.0);

    DEVICE_ID did;
    did.set_device_type(STEPS_GENERATOR);
    TERMINAL terminal;
    terminal.append_bus(30);
    did.set_device_terminal(terminal);
    did.set_device_identifier_index(get_index_of_string("1"));

    simulator.trip_generator(did);

    simulator.run_to(10);

    default_toolkit.close_log_file();
}

void DYNAMICS_SIMULATOR_TEST::test_run_bench_shandong_100_bus_model_with_dc_GENROU_CDC4T()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = default_toolkit.get_dynamic_simulator();
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    string file = "test_log/";
    file += __FUNCTION__;
    file += ".txt";
    default_toolkit.open_log_file(file);

    PSSE_IMEXPORTER importer(default_toolkit);

    psdb.set_allowed_max_bus_number(1000);

    importer.load_powerflow_data("../../../bench/bench_shandong_change.raw");
    importer.load_dynamic_data("../../../bench/bench_shandong_change_with_gov.dyr");

    vector<LCC_HVDC2T*> hvdcs = psdb.get_all_2t_lcc_hvdcs();
    unsigned int n = hvdcs.size();
    for(unsigned int i=0; i!=n; ++i)
        hvdcs[i]->turn_rectifier_constant_power_mode_into_constant_current_mode();

    POWERFLOW_SOLVER& powerflow_solver = default_toolkit.get_powerflow_solver();

    powerflow_solver.set_max_iteration(30);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.00001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.00001);
    powerflow_solver.set_flat_start_logic(false);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_fast_decoupled_solution();

    simulator.prepare_meters();
    simulator.set_output_file("test_log/bench_shandong_100_bus_model_dynamic_test_result_GENROU_CDC4T");

    simulator.set_output_file("test_log/bench_shandong_100_bus_model_dynamic_test_result_GENROU_CDC4T");
    simulator.set_allowed_max_power_imbalance_in_MVA(0.001);
    //simulator.set_max_DAE_iteration(20);
    //simulator.set_max_network_iteration(200);
    default_toolkit.set_dynamic_simulation_time_step_in_s(0.01);

    simulator.start();
    simulator.run_to(1.0);

    DEVICE_ID did;
    did.set_device_type(STEPS_AC_LINE);
    TERMINAL terminal;
    terminal.append_bus(60);
    terminal.append_bus(62);
    did.set_device_terminal(terminal);
    did.set_device_identifier_index(get_index_of_string("1"));

    simulator.set_ac_line_fault(did, 60, 0.0, complex<double>(0.0, -1e6));

    simulator.run_to(1.1);

    simulator.clear_ac_line_fault(did, 60, 0.0);
    simulator.trip_ac_line(did);

    simulator.run_to(5.0);

    default_toolkit.close_log_file();
}


void DYNAMICS_SIMULATOR_TEST::test_run_IEEE_9_bus_model_with_WT3_models()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = default_toolkit.get_dynamic_simulator();
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    string file = "test_log/";
    file += __FUNCTION__;
    file += ".txt";
    default_toolkit.open_log_file(file);


    PSSE_IMEXPORTER importer(default_toolkit);

    importer.load_powerflow_data("../../../bench/IEEE9_wind.raw");
    importer.load_dynamic_data("../../../bench/IEEE9_wt3_models.dyr");

    DEVICE_ID gendid = get_wt_generator_device_id(3, "1");
    WT_GENERATOR* gen = psdb.get_wt_generator(gendid);
    if(gen->get_wt_generator_model()!=NULL)
        cout<<gen->get_wt_generator_model()->get_standard_psse_string()<<endl;
    if(gen->get_wt_aerodynamic_model()!=NULL)
        cout<<gen->get_wt_aerodynamic_model()->get_standard_psse_string()<<endl;
    if(gen->get_wt_turbine_model()!=NULL)
        cout<<gen->get_wt_turbine_model()->get_standard_psse_string()<<endl;
    if(gen->get_wt_electrical_model()!=NULL)
        cout<<gen->get_wt_electrical_model()->get_standard_psse_string()<<endl;
    if(gen->get_wt_pitch_model()!=NULL)
        cout<<gen->get_wt_pitch_model()->get_standard_psse_string()<<endl;
    if(gen->get_wind_speed_model()!=NULL)
        cout<<gen->get_wind_speed_model()->get_standard_psse_string()<<endl;

    //prepare_IEEE_9_bus_model();
    //prepare_IEEE_9_bus_model_classical_dynamic_model();

    POWERFLOW_SOLVER& powerflow_solver = default_toolkit.get_powerflow_solver();

    powerflow_solver.set_max_iteration(30);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.00001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.00001);
    powerflow_solver.set_flat_start_logic(false);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_fast_decoupled_solution();

    /*METER meter(default_toolkit);
    METER_SETTER setter(default_toolkit);
    setter.set_power_system_database;
    meter = setter.prepare_bus_voltage_in_pu_meter(3);
    simulator.append_meter(meter);
    meter = setter.prepare_bus_angle_in_deg_meter(3);
    simulator.append_meter(meter);
    meter = setter.prepare_wt_generator_model_internal_variable_meter(gendid,0);
    simulator.append_meter(meter);
    meter = setter.prepare_wt_generator_model_internal_variable_meter(gendid,13);
    simulator.append_meter(meter);
    meter = setter.prepare_wt_generator_model_internal_variable_meter(gendid,14);
    simulator.append_meter(meter);
    meter = setter.prepare_wt_generator_model_internal_variable_meter(gendid,15);
    simulator.append_meter(meter);
    meter = setter.prepare_wt_generator_model_internal_variable_meter(gendid,16);
    simulator.append_meter(meter);
    meter = setter.prepare_wt_generator_model_internal_variable_meter(gendid,17);
    simulator.append_meter(meter);
    simulator.prepare_meters();
    */
    simulator.prepare_bus_related_meter(3, "Frequency in Hz");
    simulator.prepare_bus_related_meter(3, "Voltage in pu");
    simulator.prepare_wt_generator_related_meters();

    simulator.set_output_file("test_log/IEEE9_test_with_wt3_models");

    //simulator.set_max_DAE_iteration(20);
    //simulator.set_max_network_iteration(200);
    simulator.set_allowed_max_power_imbalance_in_MVA(0.001);
    default_toolkit.set_dynamic_simulation_time_step_in_s(0.01);
    simulator.set_max_DAE_iteration(200);
    simulator.set_max_network_iteration(20);
    simulator.start();
    simulator.run_to(1.0);

    DEVICE_ID did;
    did.set_device_type(STEPS_AC_LINE);
    TERMINAL terminal;
    terminal.append_bus(5);
    terminal.append_bus(7);
    did.set_device_terminal(terminal);
    did.set_device_identifier_index(get_index_of_string("1"));


    DEVICE_ID gendid_1;
    gendid_1.set_device_type(STEPS_GENERATOR);
    terminal.clear();
    terminal.append_bus(1);
    gendid_1.set_device_terminal(terminal);
    gendid_1.set_device_identifier_index(get_index_of_string("1"));

    /*simulator.set_ac_line_fault(did, 7, 0.0, complex<double>(0.0, -2e5));
    simulator.run_to(1.1);
    simulator.clear_ac_line_fault(did, 7, 0.0);
    simulator.trip_ac_line(did);*/
    simulator.shed_generator(gendid_1, 0.2);

    simulator.run_to(10.0);

    default_toolkit.close_log_file();
}

void DYNAMICS_SIMULATOR_TEST::test_run_IEEE_9_bus_model_with_all_WT3_models()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = default_toolkit.get_dynamic_simulator();
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    string file = "test_log/";
    file += __FUNCTION__;
    file += ".txt";
    default_toolkit.open_log_file(file);


    PSSE_IMEXPORTER importer(default_toolkit);

    importer.load_powerflow_data("../../../bench/IEEE9_all_wind.raw");
    importer.load_dynamic_data("../../../bench/IEEE9_all_wt3_models.dyr");

    vector<WT_GENERATOR*> gens = psdb.get_all_wt_generators();
    for(unsigned int i=0; i<gens.size(); ++i)
    {
        WT_GENERATOR* gen = gens[i];
        if(gen->get_wt_generator_model()!=NULL)
            cout<<gen->get_wt_generator_model()->get_standard_psse_string()<<endl;
        if(gen->get_wt_aerodynamic_model()!=NULL)
            cout<<gen->get_wt_aerodynamic_model()->get_standard_psse_string()<<endl;
        if(gen->get_wt_turbine_model()!=NULL)
            cout<<gen->get_wt_turbine_model()->get_standard_psse_string()<<endl;
        if(gen->get_wt_electrical_model()!=NULL)
            cout<<gen->get_wt_electrical_model()->get_standard_psse_string()<<endl;
        if(gen->get_wt_pitch_model()!=NULL)
            cout<<gen->get_wt_pitch_model()->get_standard_psse_string()<<endl;
        if(gen->get_wind_speed_model()!=NULL)
            cout<<gen->get_wind_speed_model()->get_standard_psse_string()<<endl;
    }

    //prepare_IEEE_9_bus_model();
    //prepare_IEEE_9_bus_model_classical_dynamic_model();

    POWERFLOW_SOLVER& powerflow_solver = default_toolkit.get_powerflow_solver();

    powerflow_solver.set_max_iteration(30);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.00001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.00001);
    powerflow_solver.set_flat_start_logic(false);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_fast_decoupled_solution();
    powerflow_solver.show_powerflow_result();

    /*METER meter(default_toolkit);
    METER_SETTER setter(default_toolkit);
    setter.set_power_system_database;
    meter = setter.prepare_bus_voltage_in_pu_meter(3);
    simulator.append_meter(meter);
    meter = setter.prepare_bus_angle_in_deg_meter(3);
    simulator.append_meter(meter);
    meter = setter.prepare_wt_generator_model_internal_variable_meter(gendid,0);
    simulator.append_meter(meter);
    meter = setter.prepare_wt_generator_model_internal_variable_meter(gendid,13);
    simulator.append_meter(meter);
    meter = setter.prepare_wt_generator_model_internal_variable_meter(gendid,14);
    simulator.append_meter(meter);
    meter = setter.prepare_wt_generator_model_internal_variable_meter(gendid,15);
    simulator.append_meter(meter);
    meter = setter.prepare_wt_generator_model_internal_variable_meter(gendid,16);
    simulator.append_meter(meter);
    meter = setter.prepare_wt_generator_model_internal_variable_meter(gendid,17);
    simulator.append_meter(meter);
    simulator.prepare_meters();
    */
    simulator.prepare_bus_related_meter(1, "Frequency in Hz");
    simulator.prepare_bus_related_meter(2, "Frequency in Hz");
    simulator.prepare_bus_related_meter(3, "Frequency in Hz");
    simulator.prepare_bus_related_meter(1, "Voltage in pu");
    simulator.prepare_bus_related_meter(2, "Voltage in pu");
    simulator.prepare_bus_related_meter(3, "Voltage in pu");
    simulator.prepare_bus_related_meter(1, "Angle in deg");
    simulator.prepare_bus_related_meter(2, "Angle in deg");
    simulator.prepare_bus_related_meter(3, "Angle in deg");
    simulator.prepare_wt_generator_related_meters();

    simulator.set_output_file("test_log/IEEE9_test_with_all_wt3_models");

    //simulator.set_max_DAE_iteration(20);
    //simulator.set_max_network_iteration(200);
    simulator.set_allowed_max_power_imbalance_in_MVA(0.001);
    default_toolkit.set_dynamic_simulation_time_step_in_s(0.01);
    simulator.start();
    simulator.run_to(1.0);

    DEVICE_ID did;
    did.set_device_type(STEPS_AC_LINE);
    TERMINAL terminal;
    terminal.append_bus(5);
    terminal.append_bus(7);
    did.set_device_terminal(terminal);
    did.set_device_identifier_index(get_index_of_string("1"));


    DEVICE_ID gendid_1;
    gendid_1.set_device_type(STEPS_GENERATOR);
    terminal.clear();
    terminal.append_bus(1);
    gendid_1.set_device_terminal(terminal);
    gendid_1.set_device_identifier_index(get_index_of_string("1"));

    /*simulator.set_ac_line_fault(did, 7, 0.0, complex<double>(0.0, -2e5));
    simulator.run_to(1.1);
    simulator.clear_ac_line_fault(did, 7, 0.0);
    simulator.trip_ac_line(did);*/
    simulator.shed_generator(gendid_1, 0.2);

    simulator.run_to(20.0);

    default_toolkit.close_log_file();
}

#endif

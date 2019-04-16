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

using namespace std;

DYNAMICS_SIMULATOR_TEST::DYNAMICS_SIMULATOR_TEST()
{

    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_constructor);

    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_set_get_bin_file_export_enable_flag);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_set_get_csv_file_export_enable_flag);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_set_get_json_file_export_enable_flag);

    //TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_set_get_current_simulation_time);

    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_set_get_max_DAE_iteration);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_set_get_max_network_iteration);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_set_get_max_update_event_iteration);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_set_get_allowed_max_power_imbalance_in_MVA);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_set_get_iteration_accelerator);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_set_get_rotor_angle_stability_survilliance_flag);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_set_get_rotor_angle_stability_threshold);

    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_append_and_get_meter);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_get_meter_count);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_get_all_meters_value);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_clear_meters);

    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_set_get_output_file);

    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_start);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_single_machine_model_GENCLS_IEEL);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_single_machine_model_GENROU);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_single_machine_model_IEEET1);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_single_machine_model_IEEEG1);


    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_run_IEEE_9_bus_model_classic_trip_bus);

    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_run_IEEE_9_bus_model_classic);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_run_IEEE_9_bus_model_classic_with_rotor_angle_survilliance);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_run_IEEE_39_bus_model_GENROU);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_run_IEEE_39_bus_model_GENSAL);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_run_IEEE_39_bus_model_GENROU_IEEET1);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_run_IEEE_39_bus_model_GENROU_IEEEG1);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_run_IEEE_39_bus_model_GENROU_IEEET1_IEEEG1);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_run_IEEE_39_bus_model_GENROU_SEXS_IEEEG1);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_run_IEEE_39_bus_model_GENROU_SEXS_IEEEG1_LCFB1);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_run_IEEE_39_bus_model_with_wind);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_run_IEEE_39_bus_model_GENROU_SEXS_IEEEG1_without_UFLS);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_run_IEEE_39_bus_model_GENROU_SEXS_IEEEG1_UFLS);
    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_run_IEEE_39_bus_model_GENROU_SEXS_IEEEG1_PUFLS);

    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_run_bench_shandong_100_bus_model_with_dc_GENROU_CDC4T);

    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_run_IEEE_9_bus_model_with_WT3_models);

    TEST_ADD(DYNAMICS_SIMULATOR_TEST::test_run_IEEE_9_bus_model_with_all_WT3_models);


}

void DYNAMICS_SIMULATOR_TEST::setup()
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    psdb.set_allowed_max_bus_number(10000);
    DYNAMICS_SIMULATOR& simulator = get_default_dynamic_simulator();
    set_dynamic_simulation_time_step_in_s(0.01);
    simulator.set_allowed_max_power_imbalance_in_MVA(0.00001);
}

void DYNAMICS_SIMULATOR_TEST::tear_down()
{
    DYNAMICS_SIMULATOR& simulator = get_default_dynamic_simulator();
    simulator.clear();

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    psdb.clear_database();

    show_test_end_information();
}

void DYNAMICS_SIMULATOR_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = get_default_dynamic_simulator();
    TEST_ASSERT(simulator.get_meter_count()==0);
    TEST_ASSERT(simulator.get_max_DAE_iteration()==200);
    TEST_ASSERT(simulator.get_max_network_iteration()==1);
    TEST_ASSERT(fabs(simulator.get_allowed_max_power_imbalance_in_MVA()-0.00001)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(simulator.get_iteration_accelerator()-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(simulator.is_bin_file_export_enabled()==true);
    TEST_ASSERT(simulator.is_csv_file_export_enabled()==false);
    TEST_ASSERT(simulator.is_json_file_export_enabled()==false);
}

void DYNAMICS_SIMULATOR_TEST::test_set_get_bin_file_export_enable_flag()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = get_default_dynamic_simulator();
    simulator.set_bin_file_export_enable_flag(true);
    TEST_ASSERT(simulator.is_bin_file_export_enabled()==true);
    simulator.set_bin_file_export_enable_flag(false);
    TEST_ASSERT(simulator.is_bin_file_export_enabled()==false);
}

void DYNAMICS_SIMULATOR_TEST::test_set_get_csv_file_export_enable_flag()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = get_default_dynamic_simulator();
    simulator.set_csv_file_export_enable_flag(true);
    TEST_ASSERT(simulator.is_csv_file_export_enabled()==true);
    simulator.set_csv_file_export_enable_flag(false);
    TEST_ASSERT(simulator.is_csv_file_export_enabled()==false);
}

void DYNAMICS_SIMULATOR_TEST::test_set_get_json_file_export_enable_flag()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = get_default_dynamic_simulator();
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
void DYNAMICS_SIMULATOR_TEST::test_set_get_max_DAE_iteration()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = get_default_dynamic_simulator();
    simulator.set_max_DAE_iteration(200);
    TEST_ASSERT(simulator.get_max_DAE_iteration()==200);
    simulator.set_max_DAE_iteration(100);
    TEST_ASSERT(simulator.get_max_DAE_iteration()==100);
}

void DYNAMICS_SIMULATOR_TEST::test_set_get_max_network_iteration()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = get_default_dynamic_simulator();
    simulator.set_max_network_iteration(200);
    TEST_ASSERT(simulator.get_max_network_iteration()==200);
    simulator.set_max_network_iteration(100);
    TEST_ASSERT(simulator.get_max_network_iteration()==100);
}

void DYNAMICS_SIMULATOR_TEST::test_set_get_max_update_event_iteration()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = get_default_dynamic_simulator();
    simulator.set_max_update_event_iteration(200);
    TEST_ASSERT(simulator.get_max_update_event_iteration()==200);
    simulator.set_max_update_event_iteration(100);
    TEST_ASSERT(simulator.get_max_update_event_iteration()==100);
}

void DYNAMICS_SIMULATOR_TEST::test_set_get_allowed_max_power_imbalance_in_MVA()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = get_default_dynamic_simulator();
    simulator.set_allowed_max_power_imbalance_in_MVA(0.001);
    TEST_ASSERT(fabs(simulator.get_allowed_max_power_imbalance_in_MVA()-0.001)<FLOAT_EPSILON);
}

void DYNAMICS_SIMULATOR_TEST::test_set_get_iteration_accelerator()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = get_default_dynamic_simulator();
    simulator.set_iteration_accelerator(1.2);
    TEST_ASSERT(fabs(simulator.get_iteration_accelerator()-1.2)<FLOAT_EPSILON);
}

void DYNAMICS_SIMULATOR_TEST::test_set_get_rotor_angle_stability_survilliance_flag()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = get_default_dynamic_simulator();
    TEST_ASSERT(simulator.get_rotor_angle_stability_survilliance_flag()==false);
    simulator.set_rotor_angle_stability_survilliance_flag(true);
    TEST_ASSERT(simulator.get_rotor_angle_stability_survilliance_flag()==true);
    simulator.set_rotor_angle_stability_survilliance_flag(false);
    TEST_ASSERT(simulator.get_rotor_angle_stability_survilliance_flag()==false);
}


void DYNAMICS_SIMULATOR_TEST::test_set_get_rotor_angle_stability_threshold()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = get_default_dynamic_simulator();
    TEST_ASSERT(fabs(simulator.get_rotor_angle_stability_threshold_in_deg()-360.0)<FLOAT_EPSILON);
    simulator.set_rotor_angle_stability_threshold_in_deg(180.0);
    TEST_ASSERT(fabs(simulator.get_rotor_angle_stability_threshold_in_deg()-180.0)<FLOAT_EPSILON);
    simulator.set_rotor_angle_stability_threshold_in_deg(-170);
    TEST_ASSERT(fabs(simulator.get_rotor_angle_stability_threshold_in_deg()-170.0)<FLOAT_EPSILON);
}

void DYNAMICS_SIMULATOR_TEST::test_append_and_get_meter()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = get_default_dynamic_simulator();
    prepare_IEEE_9_bus_model();
    prepare_IEEE_9_bus_model_classical_dynamic_model();

    METER meter;
    DEVICE_ID did;
    TERMINAL terminal;

    terminal.clear();
    did.set_device_type("BUS");
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
    did.set_device_type("BUS");
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

    DYNAMICS_SIMULATOR& simulator = get_default_dynamic_simulator();

    prepare_IEEE_9_bus_model();
    prepare_IEEE_9_bus_model_classical_dynamic_model();

    METER meter;
    DEVICE_ID did;
    TERMINAL terminal;

    terminal.clear();
    did.set_device_type("BUS");
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

    DYNAMICS_SIMULATOR& simulator = get_default_dynamic_simulator();
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    prepare_IEEE_9_bus_model();
    prepare_IEEE_9_bus_model_classical_dynamic_model();

    METER meter;
    DEVICE_ID did;
    TERMINAL terminal;

    terminal.clear();
    did.set_device_type("BUS");
    terminal.append_bus(1);
    did.set_device_terminal(terminal);

    meter.set_device_id(did);
    meter.set_meter_type("VOLTAGE IN PU");

    simulator.append_meter(meter);

    terminal.clear();
    did.set_device_type("BUS");
    terminal.append_bus(2);
    did.set_device_terminal(terminal);

    meter.set_device_id(did);
    meter.set_meter_type("VOLTAGE IN PU");

    simulator.append_meter(meter);

    terminal.clear();
    did.set_device_type("BUS");
    terminal.append_bus(3);
    did.set_device_terminal(terminal);

    meter.set_device_id(did);
    meter.set_meter_type("VOLTAGE IN PU");

    simulator.append_meter(meter);


    TEST_ASSERT(simulator.get_meter_count()==3);

    BUS* busptr = psdb.get_bus(1);
    busptr->set_voltage_in_pu(1.0);
    busptr = psdb.get_bus(2);
    busptr->set_voltage_in_pu(1.1);
    busptr = psdb.get_bus(3);
    busptr->set_voltage_in_pu(0.9);

    vector<double> values = simulator.get_all_meters_value();

    TEST_ASSERT(values.size()==3)
    TEST_ASSERT(fabs(values[0]-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(values[1]-1.1)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(values[2]-0.9)<FLOAT_EPSILON);
}

void DYNAMICS_SIMULATOR_TEST::test_clear_meters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = get_default_dynamic_simulator();

    prepare_IEEE_9_bus_model();
    prepare_IEEE_9_bus_model_classical_dynamic_model();

    METER meter;
    DEVICE_ID did;
    TERMINAL terminal;

    terminal.clear();
    did.set_device_type("BUS");
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

    DYNAMICS_SIMULATOR& simulator = get_default_dynamic_simulator();
    simulator.set_output_file("steps out file 1");
    TEST_ASSERT(simulator.get_output_file()=="steps out file 1");
}

void DYNAMICS_SIMULATOR_TEST::test_start()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = get_default_dynamic_simulator();
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    string file = "test_log/";
    file += __FUNCTION__;
    file += ".txt";
    redirect_stdout_to_file(file);

    prepare_IEEE_9_bus_model();
    prepare_IEEE_9_bus_model_classical_dynamic_model();

    POWERFLOW_SOLVER powerflow_solver;

    powerflow_solver.set_max_iteration(30);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.00001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.00001);
    powerflow_solver.set_flat_start_logic(false);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_fast_decoupled_solution();

    simulator.set_output_file("test_log/test_dynamic_simulator_initialize");

    simulator.start();

    TEST_ASSERT(fabs(get_dynamic_simulation_time_in_s()-(-2.0)*get_dynamic_simulation_time_step_in_s())<FLOAT_EPSILON);

    size_t n = psdb.get_generator_count();
    vector<GENERATOR*> generators = psdb.get_all_generators();
    GENERATOR* generator;
    ostringstream osstream;
    for(size_t i=0; i!=n; ++i)
    {
        generator = generators[i];
        SYNC_GENERATOR_MODEL* genmodel = generator->get_sync_generator_model();
        osstream<<genmodel->get_device_name()<<" initialized : "<<genmodel->get_rotor_angle_in_deg()<<" deg"<<endl;
    }
    show_information_with_leading_time_stamp(osstream);
    recover_stdout();
}

void DYNAMICS_SIMULATOR_TEST::run_single_machine_model_for_model_test()
{
    DYNAMICS_SIMULATOR& simulator = get_default_dynamic_simulator();

    POWERFLOW_SOLVER powerflow_solver;

    powerflow_solver.set_max_iteration(30);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.00001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.00001);
    powerflow_solver.set_flat_start_logic(false);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_fast_decoupled_solution();

    set_dynamic_simulation_time_step_in_s(0.001);


    DEVICE_ID did;
    did.set_device_type("GENERATOR");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");

    //cout<<psdb.get_generator(did)->get_sync_generator_model()->get_standard_model_string()<<endl;

    METER_SETTER setter;

    METER meter;
    meter = setter.prepare_bus_voltage_in_pu_meter(1);
    simulator.append_meter(meter);
    meter = setter.prepare_bus_angle_in_deg_meter(1);
    simulator.append_meter(meter);
    meter = setter.prepare_bus_frequency_deviation_in_pu_meter(1);
    simulator.append_meter(meter);


    did.set_device_type("GENERATOR");
    terminal.clear();
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");

    meter = setter.prepare_generator_rotor_angle_in_deg_meter(did);
    simulator.append_meter(meter);
    meter = setter.prepare_generator_rotor_speed_deviation_in_pu_meter(did);
    simulator.append_meter(meter);
    meter = setter.prepare_generator_excitation_voltage_in_pu_meter(did);
    simulator.append_meter(meter);
    meter = setter.prepare_generator_mechanical_power_in_pu_on_mbase_meter(did);
    simulator.append_meter(meter);
    meter = setter.prepare_generator_terminal_active_power_in_pu_on_sbase_meter(did);
    simulator.append_meter(meter);
    meter = setter.prepare_generator_terminal_reactive_power_in_pu_on_sbase_meter(did);
    simulator.append_meter(meter);

    simulator.clear_meters();

    simulator.start();
    simulator.run_to(1.0);

    did.set_device_type("LINE");
    terminal.clear();
    terminal.append_bus(3);
    terminal.append_bus(4);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");

    simulator.set_line_fault(did, 3, 0.0, complex<double>(0.0, -2e10));

    DEVICE_ID load_did;
    load_did.set_device_type("LOAD");
    terminal.clear();
    terminal.append_bus(2);
    load_did.set_device_terminal(terminal);
    load_did.set_device_identifier("1");

    //simulator.scale_load(load_did, 0.1);

    simulator.run_to(1.1);

    simulator.clear_line_fault(did, 3, 0.0);

    simulator.run_to(5.0);
}

void DYNAMICS_SIMULATOR_TEST::test_single_machine_model_GENCLS_IEEL()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = get_default_dynamic_simulator();

    string file = "test_log/";
    file += __FUNCTION__;
    file += ".txt";
    redirect_stdout_to_file(file);

    PSSE_IMEXPORTER importer;

    importer.load_powerflow_data("../../../bench/sm_test_model.raw");
    importer.load_dynamic_data("../../../bench/sm_test_model_GENCLS_IEELAL.dyr");

    string output_file = "test_log/";
    output_file += __FUNCTION__;
    simulator.set_output_file(output_file);

    run_single_machine_model_for_model_test();

    recover_stdout();
}

void DYNAMICS_SIMULATOR_TEST::test_single_machine_model_GENROU()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = get_default_dynamic_simulator();

    string file = "test_log/";
    file += __FUNCTION__;
    file += ".txt";
    redirect_stdout_to_file(file);

    PSSE_IMEXPORTER importer;

    importer.load_powerflow_data("../../../bench/sm_test_model.raw");
    importer.load_dynamic_data("../../../bench/sm_test_model_GENROU_GENCLS_IEELAL.dyr");

    string output_file = "test_log/";
    output_file += __FUNCTION__;
    simulator.set_output_file(output_file);

    run_single_machine_model_for_model_test();

    recover_stdout();
}

void DYNAMICS_SIMULATOR_TEST::test_single_machine_model_IEEET1()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = get_default_dynamic_simulator();

    string file = "test_log/";
    file += __FUNCTION__;
    file += ".txt";
    redirect_stdout_to_file(file);

    PSSE_IMEXPORTER importer;

    importer.load_powerflow_data("../../../bench/sm_test_model.raw");
    importer.load_dynamic_data("../../../bench/sm_test_model_GENROU_GENCLS_IEELAL_IEEET1.dyr");

    string output_file = "test_log/";
    output_file += __FUNCTION__;
    simulator.set_output_file(output_file);

    run_single_machine_model_for_model_test();

    recover_stdout();
}

void DYNAMICS_SIMULATOR_TEST::test_single_machine_model_IEEEG1()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = get_default_dynamic_simulator();

    string file = "test_log/";
    file += __FUNCTION__;
    file += ".txt";
    redirect_stdout_to_file(file);

    PSSE_IMEXPORTER importer;

    importer.load_powerflow_data("../../../bench/sm_test_model.raw");
    importer.load_dynamic_data("../../../bench/sm_test_model_GENROU_GENCLS_IEELAL_IEEET1_IEEEG1.dyr");

    string output_file = "test_log/";
    output_file += __FUNCTION__;
    simulator.set_output_file(output_file);

    run_single_machine_model_for_model_test();

    recover_stdout();
}


void DYNAMICS_SIMULATOR_TEST::test_run_IEEE_9_bus_model_classic_trip_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = get_default_dynamic_simulator();

    string file = "test_log/";
    file += __FUNCTION__;
    file += ".txt";
    redirect_stdout_to_file(file);

    PSSE_IMEXPORTER importer;

    importer.load_powerflow_data("../../../bench/IEEE9_classical.raw");
    importer.load_dynamic_data("../../../bench/IEEE9_classical.dyr");

    //prepare_IEEE_9_bus_model();
    //prepare_IEEE_9_bus_model_classical_dynamic_model();

    POWERFLOW_SOLVER powerflow_solver;

    powerflow_solver.set_max_iteration(30);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.00001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.00001);
    powerflow_solver.set_flat_start_logic(false);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_fast_decoupled_solution();

    set_dynamic_simulation_time_step_in_s(0.0083333);

    simulator.prepare_meters();

    simulator.set_output_file("test_log/IEEE9_test_with_classical_model_trip_bus_5");

    simulator.start();
    simulator.run_to(0.0);

    simulator.trip_bus(5);

    simulator.run_to(5.0);

    recover_stdout();
}


void DYNAMICS_SIMULATOR_TEST::test_run_IEEE_9_bus_model_classic()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = get_default_dynamic_simulator();

    string file = "test_log/";
    file += __FUNCTION__;
    file += ".txt";
    redirect_stdout_to_file(file);

    PSSE_IMEXPORTER importer;

    importer.load_powerflow_data("../../../bench/IEEE9_classical.raw");
    importer.load_dynamic_data("../../../bench/IEEE9_classical.dyr");

    //prepare_IEEE_9_bus_model();
    //prepare_IEEE_9_bus_model_classical_dynamic_model();

    POWERFLOW_SOLVER powerflow_solver;

    powerflow_solver.set_max_iteration(30);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.00001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.00001);
    powerflow_solver.set_flat_start_logic(false);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_fast_decoupled_solution();

    set_dynamic_simulation_time_step_in_s(0.0083333);

    simulator.prepare_meters();

    simulator.set_output_file("test_log/IEEE9_test_with_classical_model");

    simulator.start();
    simulator.run_to(0.0);

    DEVICE_ID did;
    did.set_device_type("LINE");
    TERMINAL terminal;
    terminal.append_bus(5);
    terminal.append_bus(7);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");

    simulator.set_line_fault(did, 7, 0.0, complex<double>(0.0, -2e10));

    simulator.run_to(0.08333);

    simulator.clear_line_fault(did, 7, 0.0);
    simulator.trip_line(did);

    simulator.run_to(5.0);

    recover_stdout();
}



void DYNAMICS_SIMULATOR_TEST::test_run_IEEE_9_bus_model_classic_with_rotor_angle_survilliance()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = get_default_dynamic_simulator();

    string file = "test_log/";
    file += __FUNCTION__;
    file += ".txt";
    redirect_stdout_to_file(file);

    PSSE_IMEXPORTER importer;

    importer.load_powerflow_data("../../../bench/IEEE9_classical.raw");
    importer.load_dynamic_data("../../../bench/IEEE9_classical.dyr");

    //prepare_IEEE_9_bus_model();
    //prepare_IEEE_9_bus_model_classical_dynamic_model();

    POWERFLOW_SOLVER powerflow_solver;

    powerflow_solver.set_max_iteration(30);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.00001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.00001);
    powerflow_solver.set_flat_start_logic(false);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_fast_decoupled_solution();

    set_dynamic_simulation_time_step_in_s(0.0083333);

    simulator.prepare_meters();
    simulator.set_rotor_angle_stability_survilliance_flag(true);
    simulator.set_rotor_angle_stability_threshold_in_deg(360.0);

    simulator.set_output_file("test_log/IEEE9_test_with_classical_model_with_rotor_angle_survilliance");

    simulator.start();
    simulator.run_to(0.0);

    DEVICE_ID did;
    did.set_device_type("LINE");
    TERMINAL terminal;
    terminal.append_bus(5);
    terminal.append_bus(7);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");

    simulator.set_line_fault(did, 7, 0.0, complex<double>(0.0, -2e10));

    simulator.run_to(0.3);

    simulator.clear_line_fault(did, 7, 0.0);
    simulator.trip_line(did);

    simulator.run_to(5.0);

    recover_stdout();
}



void DYNAMICS_SIMULATOR_TEST::test_run_IEEE_39_bus_model_GENROU()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = get_default_dynamic_simulator();

    string file = "test_log/";
    file += __FUNCTION__;
    file += ".txt";
    redirect_stdout_to_file(file);

    PSSE_IMEXPORTER importer;

    importer.load_powerflow_data("../../../bench/IEEE39.raw");
    importer.load_dynamic_data("../../../bench/IEEE39_GENROU.dyr");

    POWERFLOW_SOLVER powerflow_solver;

    powerflow_solver.set_max_iteration(30);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.00001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.00001);
    powerflow_solver.set_flat_start_logic(false);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_fast_decoupled_solution();

    simulator.prepare_meters();

    simulator.set_output_file("test_log/IEEE_39_bus_model_dynamic_test_result_GENROU");

    simulator.start();
    simulator.run_to(0.0);

    DEVICE_ID did;
    did.set_device_type("LINE");
    TERMINAL terminal;
    terminal.append_bus(17);
    terminal.append_bus(18);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");

    simulator.set_line_fault(did, 17, 0.0, complex<double>(0.0, -2e10));

    simulator.run_to(0.5);

    simulator.clear_line_fault(did, 17, 0.0);
    simulator.trip_line(did);

    simulator.run_to(5.0);

    recover_stdout();
}


void DYNAMICS_SIMULATOR_TEST::test_run_IEEE_39_bus_model_GENSAL()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = get_default_dynamic_simulator();

    string file = "test_log/";
    file += __FUNCTION__;
    file += ".txt";
    redirect_stdout_to_file(file);

    PSSE_IMEXPORTER importer;

    importer.load_powerflow_data("../../../bench/IEEE39.raw");
    importer.load_dynamic_data("../../../bench/IEEE39_GENSAL.dyr");

    POWERFLOW_SOLVER powerflow_solver;

    powerflow_solver.set_max_iteration(30);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.00001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.00001);
    powerflow_solver.set_flat_start_logic(false);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_fast_decoupled_solution();

    simulator.prepare_meters();

    simulator.set_output_file("test_log/IEEE_39_bus_model_dynamic_test_result_GENSAL");

    simulator.start();
    simulator.run_to(0.0);

    DEVICE_ID did;
    did.set_device_type("LINE");
    TERMINAL terminal;
    terminal.append_bus(17);
    terminal.append_bus(18);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");

    simulator.set_line_fault(did, 17, 0.0, complex<double>(0.0, -2e10));

    simulator.run_to(0.5);

    simulator.clear_line_fault(did, 17, 0.0);
    simulator.trip_line(did);

    simulator.run_to(5.0);

    recover_stdout();
}




void DYNAMICS_SIMULATOR_TEST::test_run_IEEE_39_bus_model_GENROU_IEEET1()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = get_default_dynamic_simulator();

    string file = "test_log/";
    file += __FUNCTION__;
    file += ".txt";
    redirect_stdout_to_file(file);

    PSSE_IMEXPORTER importer;

    importer.load_powerflow_data("../../../bench/IEEE39.raw");
    importer.load_dynamic_data("../../../bench/IEEE39_GENROU_IEEET1.dyr");

    POWERFLOW_SOLVER powerflow_solver;

    powerflow_solver.set_max_iteration(30);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.00001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.00001);
    powerflow_solver.set_flat_start_logic(false);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_fast_decoupled_solution();

    simulator.prepare_meters();

    simulator.set_output_file("test_log/IEEE_39_bus_model_dynamic_test_result_GENROU_IEEET1");

    simulator.start();
    simulator.run_to(0.0);

    DEVICE_ID did;
    did.set_device_type("LINE");
    TERMINAL terminal;
    terminal.append_bus(17);
    terminal.append_bus(18);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");

    simulator.set_line_fault(did, 17, 0.0, complex<double>(0.0, -2e10));

    simulator.run_to(0.5);

    simulator.clear_line_fault(did, 17, 0.0);
    simulator.trip_line(did);

    simulator.run_to(5.0);

    recover_stdout();
}



void DYNAMICS_SIMULATOR_TEST::test_run_IEEE_39_bus_model_GENROU_IEEEG1()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = get_default_dynamic_simulator();

    string file = "test_log/";
    file += __FUNCTION__;
    file += ".txt";
    redirect_stdout_to_file(file);

    PSSE_IMEXPORTER importer;

    importer.load_powerflow_data("../../../bench/IEEE39.raw");
    importer.load_dynamic_data("../../../bench/IEEE39_GENROU_IEEEG1.dyr");

    POWERFLOW_SOLVER powerflow_solver;

    powerflow_solver.set_max_iteration(30);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.00001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.00001);
    powerflow_solver.set_flat_start_logic(false);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_fast_decoupled_solution();

    simulator.prepare_meters();

    simulator.set_output_file("test_log/IEEE_39_bus_model_dynamic_test_result_GENROU_IEEEG1");

    simulator.start();
    simulator.run_to(0.0);

    DEVICE_ID did;
    did.set_device_type("LINE");
    TERMINAL terminal;
    terminal.append_bus(17);
    terminal.append_bus(18);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");

    simulator.set_line_fault(did, 17, 0.0, complex<double>(0.0, -2e10));

    simulator.run_to(0.5);

    simulator.clear_line_fault(did, 17, 0.0);
    simulator.trip_line(did);

    simulator.run_to(5.0);

    recover_stdout();
}



void DYNAMICS_SIMULATOR_TEST::test_run_IEEE_39_bus_model_GENROU_IEEET1_IEEEG1()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = get_default_dynamic_simulator();

    string file = "test_log/";
    file += __FUNCTION__;
    file += ".txt";
    redirect_stdout_to_file(file);

    PSSE_IMEXPORTER importer;

    importer.load_powerflow_data("../../../bench/IEEE39.raw");
    importer.load_dynamic_data("../../../bench/IEEE39.dyr");

    POWERFLOW_SOLVER powerflow_solver;

    powerflow_solver.set_max_iteration(30);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.00001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.00001);
    powerflow_solver.set_flat_start_logic(false);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_fast_decoupled_solution();

    simulator.prepare_meters();

    simulator.set_output_file("test_log/IEEE_39_bus_model_dynamic_test_result_GENROU_IEEET1_IEEEG1");

    simulator.start();
    simulator.run_to(0.0);

    DEVICE_ID did;
    did.set_device_type("LINE");
    TERMINAL terminal;
    terminal.append_bus(17);
    terminal.append_bus(18);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");

    simulator.set_line_fault(did, 17, 0.0, complex<double>(0.0, -2e10));

    simulator.run_to(0.5);

    simulator.clear_line_fault(did, 17, 0.0);
    simulator.trip_line(did);

    simulator.run_to(5.0);

    recover_stdout();
}


void DYNAMICS_SIMULATOR_TEST::test_run_IEEE_39_bus_model_GENROU_SEXS_IEEEG1()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = get_default_dynamic_simulator();

    string file = "test_log/";
    file += __FUNCTION__;
    file += ".txt";
    redirect_stdout_to_file(file);

    PSSE_IMEXPORTER importer;

    importer.load_powerflow_data("../../../bench/IEEE39.raw");
    importer.load_dynamic_data("../../../bench/IEEE39_GENROU_SEXS_IEEEG1.dyr");

    POWERFLOW_SOLVER powerflow_solver;

    powerflow_solver.set_max_iteration(30);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.00001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.00001);
    powerflow_solver.set_flat_start_logic(false);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_fast_decoupled_solution();

    simulator.prepare_meters();

    simulator.set_output_file("test_log/IEEE_39_bus_model_dynamic_test_result_GENROU_SEXS_IEEEG1");

    simulator.start();
    simulator.run_to(0.0);

    DEVICE_ID did;
    did.set_device_type("LINE");
    TERMINAL terminal;
    terminal.append_bus(17);
    terminal.append_bus(18);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");

    simulator.set_line_fault(did, 17, 0.0, complex<double>(0.0, -2e10));

    simulator.run_to(0.5);

    simulator.clear_line_fault(did, 17, 0.0);
    simulator.trip_line(did);

    simulator.run_to(5.0);

    recover_stdout();
}

void DYNAMICS_SIMULATOR_TEST::test_run_IEEE_39_bus_model_GENROU_SEXS_IEEEG1_LCFB1()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = get_default_dynamic_simulator();

    string file = "test_log/";
    file += __FUNCTION__;
    file += ".txt";
    redirect_stdout_to_file(file);

    PSSE_IMEXPORTER importer;

    importer.load_powerflow_data("../../../bench/IEEE39.raw");
    importer.load_dynamic_data("../../../bench/IEEE39_GENROU_SEXS_IEEEG1_LCFB1.dyr");

    POWERFLOW_SOLVER powerflow_solver;

    powerflow_solver.set_max_iteration(30);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.00001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.00001);
    powerflow_solver.set_flat_start_logic(false);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_fast_decoupled_solution();

    //simulator.prepare_meters();

    simulator.set_output_file("test_log/IEEE_39_bus_model_dynamic_test_result_GENROU_SEXS_IEEEG1_LCFB1");

    simulator.start();
    simulator.run_to(1.0);

    DEVICE_ID did;
    did.set_device_type("GENERATOR");
    TERMINAL terminal;
    terminal.append_bus(39);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");

    simulator.trip_generator(did);

    terminal.clear();
    terminal.append_bus(38);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");

    simulator.trip_generator(did);

    terminal.clear();
    terminal.append_bus(37);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");

    simulator.trip_generator(did);

    simulator.run_to(50);

    recover_stdout();
}


void DYNAMICS_SIMULATOR_TEST::test_run_IEEE_39_bus_model_GENROU_SEXS_IEEEG1_without_UFLS()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = get_default_dynamic_simulator();

    string file = "test_log/";
    file += __FUNCTION__;
    file += ".txt";
    redirect_stdout_to_file(file);

    PSSE_IMEXPORTER importer;

    importer.load_powerflow_data("../../../bench/IEEE39.raw");
    importer.load_dynamic_data("../../../bench/IEEE39_GENROU_SEXS_IEEEG1.dyr");

    POWERFLOW_SOLVER powerflow_solver;

    powerflow_solver.set_max_iteration(30);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.00001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.00001);
    powerflow_solver.set_flat_start_logic(false);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_fast_decoupled_solution();

    simulator.prepare_meters();

    simulator.set_output_file("test_log/IEEE_39_bus_model_dynamic_test_result_GENROU_SEXS_IEEEG1_without_UFLS");

    simulator.start();
    simulator.run_to(1.0);

    DEVICE_ID did;
    did.set_device_type("GENERATOR");
    TERMINAL terminal;
    terminal.append_bus(39);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");

    simulator.trip_generator(did);

    terminal.clear();
    terminal.append_bus(38);
    did.set_device_terminal(terminal);
    simulator.trip_generator(did);

    terminal.clear();
    terminal.append_bus(37);
    did.set_device_terminal(terminal);
    simulator.trip_generator(did);

    simulator.run_to(20);

    recover_stdout();
}

void DYNAMICS_SIMULATOR_TEST::test_run_IEEE_39_bus_model_GENROU_SEXS_IEEEG1_UFLS()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = get_default_dynamic_simulator();

    string file = "test_log/";
    file += __FUNCTION__;
    file += ".txt";
    redirect_stdout_to_file(file);

    PSSE_IMEXPORTER importer;

    importer.load_powerflow_data("../../../bench/IEEE39.raw");
    importer.load_dynamic_data("../../../bench/IEEE39_GENROU_SEXS_IEEEG1_UFLS.dyr");

    POWERFLOW_SOLVER powerflow_solver;

    powerflow_solver.set_max_iteration(30);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.00001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.00001);
    powerflow_solver.set_flat_start_logic(false);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_fast_decoupled_solution();

    simulator.prepare_meters();

    simulator.set_output_file("test_log/IEEE_39_bus_model_dynamic_test_result_GENROU_SEXS_IEEEG1_UFLS");

    simulator.start();
    simulator.run_to(1.0);

    DEVICE_ID did;
    did.set_device_type("GENERATOR");
    TERMINAL terminal;
    terminal.append_bus(39);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");

    simulator.trip_generator(did);

    terminal.clear();
    terminal.append_bus(38);
    did.set_device_terminal(terminal);
    simulator.trip_generator(did);

    terminal.clear();
    terminal.append_bus(37);
    did.set_device_terminal(terminal);
    simulator.trip_generator(did);

    simulator.run_to(20);

    recover_stdout();
}


void DYNAMICS_SIMULATOR_TEST::test_run_IEEE_39_bus_model_GENROU_SEXS_IEEEG1_PUFLS()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = get_default_dynamic_simulator();

    string file = "test_log/";
    file += __FUNCTION__;
    file += ".txt";
    redirect_stdout_to_file(file);

    PSSE_IMEXPORTER importer;

    importer.load_powerflow_data("../../../bench/IEEE39.raw");
    importer.load_dynamic_data("../../../bench/IEEE39_GENROU_SEXS_IEEEG1_PUFLS.dyr");

    POWERFLOW_SOLVER powerflow_solver;

    powerflow_solver.set_max_iteration(30);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.00001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.00001);
    powerflow_solver.set_flat_start_logic(false);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_fast_decoupled_solution();

    simulator.prepare_meters();

    simulator.set_output_file("test_log/IEEE_39_bus_model_dynamic_test_result_GENROU_SEXS_IEEEG1_PUFLS");

    simulator.start();
    simulator.run_to(1.0);

    DEVICE_ID did;
    did.set_device_type("GENERATOR");
    TERMINAL terminal;
    terminal.append_bus(39);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");

    simulator.trip_generator(did);

    terminal.clear();
    terminal.append_bus(38);
    did.set_device_terminal(terminal);
    simulator.trip_generator(did);

    terminal.clear();
    terminal.append_bus(37);
    did.set_device_terminal(terminal);
    simulator.trip_generator(did);

    simulator.run_to(20);

    recover_stdout();
}


void DYNAMICS_SIMULATOR_TEST::test_run_IEEE_39_bus_model_with_wind()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = get_default_dynamic_simulator();

    string file = "test_log/";
    file += __FUNCTION__;
    file += ".txt";
    redirect_stdout_to_file(file);

    PSSE_IMEXPORTER importer;

    importer.load_powerflow_data("../../../bench/IEEE39_wind.raw");
    importer.load_dynamic_data("../../../bench/IEEE39_wind.dyr");

    POWERFLOW_SOLVER powerflow_solver;

    powerflow_solver.set_max_iteration(30);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.00001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.00001);
    powerflow_solver.set_flat_start_logic(false);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_fast_decoupled_solution();

    simulator.prepare_meters();
    set_dynamic_simulation_time_step_in_s(0.001);

    simulator.set_output_file("test_log/IEEE_39_bus_model_with_wind");

    simulator.start();
    simulator.run_to(1.0);


    recover_stdout();
}

void DYNAMICS_SIMULATOR_TEST::test_run_bench_shandong_100_bus_model_with_dc_GENROU_CDC4T()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = get_default_dynamic_simulator();
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    string file = "test_log/";
    file += __FUNCTION__;
    file += ".txt";
    redirect_stdout_to_file(file);

    PSSE_IMEXPORTER importer;

    psdb.set_allowed_max_bus_number(1000);

    importer.load_powerflow_data("../../../bench/bench_shandong_change.raw");
    importer.load_dynamic_data("../../../bench/bench_shandong_change_with_gov.dyr");

    vector<HVDC*> hvdcs = psdb.get_all_hvdcs();
    size_t n = hvdcs.size();
    for(size_t i=0; i!=n; ++i)
        hvdcs[i]->turn_rectifier_constant_power_mode_into_constant_current_mode();

    POWERFLOW_SOLVER powerflow_solver;

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
    set_dynamic_simulation_time_step_in_s(0.01);

    simulator.start();
    simulator.run_to(1.0);

    DEVICE_ID did;
    did.set_device_type("LINE");
    TERMINAL terminal;
    terminal.append_bus(60);
    terminal.append_bus(62);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");

    simulator.set_line_fault(did, 60, 0.0, complex<double>(0.0, -1e6));

    simulator.run_to(1.1);

    simulator.clear_line_fault(did, 60, 0.0);
    simulator.trip_line(did);

    simulator.run_to(5.0);

    recover_stdout();
}


void DYNAMICS_SIMULATOR_TEST::test_run_IEEE_9_bus_model_with_WT3_models()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = get_default_dynamic_simulator();
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    string file = "test_log/";
    file += __FUNCTION__;
    file += ".txt";
    redirect_stdout_to_file(file);


    PSSE_IMEXPORTER importer;

    importer.load_powerflow_data("../../../bench/IEEE9_wind.raw");
    importer.load_dynamic_data("../../../bench/IEEE9_wt3_models.dyr");

    DEVICE_ID gendid = get_wt_generator_device_id(3, "1");
    WT_GENERATOR* gen = psdb.get_wt_generator(gendid);
    if(gen->get_wt_generator_model()!=NULL)
        cout<<gen->get_wt_generator_model()->get_standard_model_string()<<endl;
    if(gen->get_wt_aerodynamic_model()!=NULL)
        cout<<gen->get_wt_aerodynamic_model()->get_standard_model_string()<<endl;
    if(gen->get_wt_turbine_model()!=NULL)
        cout<<gen->get_wt_turbine_model()->get_standard_model_string()<<endl;
    if(gen->get_wt_electrical_model()!=NULL)
        cout<<gen->get_wt_electrical_model()->get_standard_model_string()<<endl;
    if(gen->get_wt_pitch_model()!=NULL)
        cout<<gen->get_wt_pitch_model()->get_standard_model_string()<<endl;
    if(gen->get_wind_speed_model()!=NULL)
        cout<<gen->get_wind_speed_model()->get_standard_model_string()<<endl;

    //prepare_IEEE_9_bus_model();
    //prepare_IEEE_9_bus_model_classical_dynamic_model();

    POWERFLOW_SOLVER powerflow_solver;

    powerflow_solver.set_max_iteration(30);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.00001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.00001);
    powerflow_solver.set_flat_start_logic(false);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_fast_decoupled_solution();

    /*METER meter;
    METER_SETTER setter;
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
    set_dynamic_simulation_time_step_in_s(0.01);
    simulator.set_max_DAE_iteration(200);
    simulator.set_max_network_iteration(1);
    simulator.start();
    simulator.run_to(1.0);

    DEVICE_ID did;
    did.set_device_type("LINE");
    TERMINAL terminal;
    terminal.append_bus(5);
    terminal.append_bus(7);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");


    DEVICE_ID gendid_1;
    gendid_1.set_device_type("GENERATOR");
    terminal.clear();
    terminal.append_bus(1);
    gendid_1.set_device_terminal(terminal);
    gendid_1.set_device_identifier("1");

    /*simulator.set_line_fault(did, 7, 0.0, complex<double>(0.0, -2e8));
    simulator.run_to(1.1);
    simulator.clear_line_fault(did, 7, 0.0);
    simulator.trip_line(did);*/
    simulator.shed_generator(gendid_1, 0.2);

    simulator.run_to(10.0);

    recover_stdout();
}

void DYNAMICS_SIMULATOR_TEST::test_run_IEEE_9_bus_model_with_all_WT3_models()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DYNAMICS_SIMULATOR_TEST");

    DYNAMICS_SIMULATOR& simulator = get_default_dynamic_simulator();
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    string file = "test_log/";
    file += __FUNCTION__;
    file += ".txt";
    redirect_stdout_to_file(file);


    PSSE_IMEXPORTER importer;

    importer.load_powerflow_data("../../../bench/IEEE9_all_wind.raw");
    importer.load_dynamic_data("../../../bench/IEEE9_all_wt3_models.dyr");

    vector<WT_GENERATOR*> gens = psdb.get_all_wt_generators();
    for(size_t i=0; i<gens.size(); ++i)
    {
        WT_GENERATOR* gen = gens[i];
        if(gen->get_wt_generator_model()!=NULL)
            cout<<gen->get_wt_generator_model()->get_standard_model_string()<<endl;
        if(gen->get_wt_aerodynamic_model()!=NULL)
            cout<<gen->get_wt_aerodynamic_model()->get_standard_model_string()<<endl;
        if(gen->get_wt_turbine_model()!=NULL)
            cout<<gen->get_wt_turbine_model()->get_standard_model_string()<<endl;
        if(gen->get_wt_electrical_model()!=NULL)
            cout<<gen->get_wt_electrical_model()->get_standard_model_string()<<endl;
        if(gen->get_wt_pitch_model()!=NULL)
            cout<<gen->get_wt_pitch_model()->get_standard_model_string()<<endl;
        if(gen->get_wind_speed_model()!=NULL)
            cout<<gen->get_wind_speed_model()->get_standard_model_string()<<endl;
    }

    //prepare_IEEE_9_bus_model();
    //prepare_IEEE_9_bus_model_classical_dynamic_model();

    POWERFLOW_SOLVER powerflow_solver;

    powerflow_solver.set_max_iteration(30);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.00001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.00001);
    powerflow_solver.set_flat_start_logic(false);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_fast_decoupled_solution();

    /*METER meter;
    METER_SETTER setter;
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
    set_dynamic_simulation_time_step_in_s(0.01);
    simulator.start();
    simulator.run_to(1.0);

    DEVICE_ID did;
    did.set_device_type("LINE");
    TERMINAL terminal;
    terminal.append_bus(5);
    terminal.append_bus(7);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");


    DEVICE_ID gendid_1;
    gendid_1.set_device_type("GENERATOR");
    terminal.clear();
    terminal.append_bus(1);
    gendid_1.set_device_terminal(terminal);
    gendid_1.set_device_identifier("1");

    /*simulator.set_line_fault(did, 7, 0.0, complex<double>(0.0, -2e8));
    simulator.run_to(1.1);
    simulator.clear_line_fault(did, 7, 0.0);
    simulator.trip_line(did);*/
    simulator.shed_generator(gendid_1, 0.2);

    simulator.run_to(100.0);

    recover_stdout();
}

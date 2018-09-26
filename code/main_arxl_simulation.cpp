#include <istream>
#include <iostream>
#include "header/power_system_database.h"
#include "header/toolkit/dynamic_simulator/dynamic_simulator.h"
#include "header/toolkit/powerflow_solver/powerflow_solver.h"
#include "header/basic/utility.h"
#include "header/data_imexporter/psse_imexporter.h"
#include "header/prepare_for_tests/prepare_models_for_test.h"
#include "header/data_imexporter/equivalent_model_imexporter.h"
using namespace std;

void prepare_training_load_scale_case();
void prepare_training_generation_shed_case();

void prepare_training_load_scale_case()
{
    POWER_SYSTEM_DATABASE* db;
    DYNAMICS_SIMULATOR* simulator;

    db = get_default_power_system_database_pointer();
    simulator = new DYNAMICS_SIMULATOR(db);

    db->set_allowed_max_bus_number(1000);
    db->set_system_base_power_in_MVA(100.0);

    simulator->set_allowed_max_power_imbalance_in_MVA(0.0001);
    set_dynamic_simulation_time_step_in_s(0.001);

    PSSE_IMEXPORTER importer;

    importer.load_powerflow_data("IEEE39.raw");
    importer.load_dynamic_data("IEEE39_GENROU_SEXS_IEEEG1.dyr");

    POWERFLOW_SOLVER powerflow_solver;

    powerflow_solver.set_power_system_database(db);

    powerflow_solver.set_max_iteration(30);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.00001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.00001);
    powerflow_solver.set_flat_start_logic(false);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_fast_decoupled_solution();

    simulator->prepare_meters();

    simulator->set_output_file("test_log/IEEE_39_bus_model_GENROU_SEXS_IEEEG1_load_scale_down_at_bus_3_by_10%");

    simulator->start();
    simulator->run_to(1.0);

    DEVICE_ID did;
    did.set_device_type("LOAD");
    TERMINAL terminal;
    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");

    simulator->scale_load(did, -0.1);

    simulator->run_to(20.0);

    delete simulator;

    db->clear_database();
}

void prepare_training_generation_shed_case()
{
    POWER_SYSTEM_DATABASE* db;
    DYNAMICS_SIMULATOR* simulator;

    db = get_default_power_system_database_pointer();
    simulator = new DYNAMICS_SIMULATOR(db);

    db->set_allowed_max_bus_number(1000);
    db->set_system_base_power_in_MVA(100.0);

    simulator->set_allowed_max_power_imbalance_in_MVA(0.0001);
    set_dynamic_simulation_time_step_in_s(0.001);

    PSSE_IMEXPORTER importer;

    importer.load_powerflow_data("IEEE39.raw");
    importer.load_dynamic_data("IEEE39_GENROU_SEXS_IEEEG1.dyr");

    POWERFLOW_SOLVER powerflow_solver;

    powerflow_solver.set_power_system_database(db);

    powerflow_solver.set_max_iteration(30);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.00001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.00001);
    powerflow_solver.set_flat_start_logic(false);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_fast_decoupled_solution();

    simulator->prepare_meters();

    simulator->set_output_file("test_log/IEEE_39_bus_model_GENROU_SEXS_IEEEG1_generation_shed_down_at_bus_38_by_10%");

    simulator->start();
    simulator->run_to(1.0);

    DEVICE_ID did;
    did.set_device_type("GENERATOR");
    TERMINAL terminal;
    terminal.append_bus(38);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");

    simulator->shed_generator(did, 0.1);

    simulator->run_to(20.0);

    delete simulator;

    db->clear_database();
}


int main()
{
    initialize_simulator(); // this function should be called first

    //prepare_training_load_scale_case();
    //prepare_training_generation_shed_case();


    POWER_SYSTEM_DATABASE* db;
    DYNAMICS_SIMULATOR* simulator;

    db = get_default_power_system_database_pointer();
    simulator = new DYNAMICS_SIMULATOR(db);

    db->set_allowed_max_bus_number(1000);
    db->set_system_base_power_in_MVA(100.0);

    set_dynamic_simulation_time_step_in_s(0.001);
    simulator->set_allowed_max_power_imbalance_in_MVA(0.01);
    simulator->set_max_DAE_iteration(10);
    simulator->set_max_network_iteration(200);
    simulator->set_iteration_accelerator(0.7);


    PSSE_IMEXPORTER importer;

    importer.load_powerflow_data("IEEE39.raw");
    importer.load_dynamic_data("IEEE39_GENROU_SEXS_IEEEG1.dyr");

    EQUIVALENT_MODEL_IMEXPORTER eqimporter;
    eqimporter.set_power_system_database(db);
    eqimporter.load_equivalent_model("C:/Users/charles/Desktop/arx/arx_model.eqv");

    POWERFLOW_SOLVER powerflow_solver;

    powerflow_solver.set_power_system_database(db);

    powerflow_solver.set_max_iteration(30);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.00001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.00001);
    powerflow_solver.set_flat_start_logic(false);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_fast_decoupled_solution();

    simulator->prepare_meters();

    simulator->set_output_file("test_log/IEEE_39_bus_model_GENROU_SEXS_IEEEG1_with_ARXL");

    simulator->start();
    simulator->run_to(1.0);

    DEVICE_ID did;
    did.set_device_type("LOAD");
    TERMINAL terminal;
    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");

    simulator->scale_load(did, -0.1);

    simulator->run_to(1.1);

    simulator->switch_on_equivalent_device();
    vector<size_t> buses_to_clear;
    buses_to_clear.push_back(19);
    buses_to_clear.push_back(20);
    buses_to_clear.push_back(33);
    buses_to_clear.push_back(34);
    buses_to_clear.push_back(21);
    buses_to_clear.push_back(22);
    buses_to_clear.push_back(23);
    buses_to_clear.push_back(24);
    buses_to_clear.push_back(35);
    buses_to_clear.push_back(36);
    simulator->trip_buses(buses_to_clear);

    simulator->run_to(5.0);

    delete simulator;

    db->clear_database();

    terminate_simulator();

    return 0;
}

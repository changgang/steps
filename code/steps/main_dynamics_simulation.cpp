#include <istream>
#include <iostream>
#include "header/power_system_database.h"
#include "header/steps_namespace.h"
#include "header/toolkit/dynamic_simulator/dynamic_simulator.h"
#include "header/toolkit/powerflow_solver/powerflow_solver.h"
#include "header/basic/utility.h"
#include "header/data_imexporter/psse_imexporter.h"
#include "header/prepare_for_tests/prepare_models_for_test.h"
using namespace std;

int main()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    DYNAMICS_SIMULATOR& simulator = default_toolkit.get_dynamic_simulator();

    psdb.set_allowed_max_bus_number(1000);

    default_toolkit.set_dynamic_simulation_time_step_in_s(0.01);
    simulator.set_allowed_max_power_imbalance_in_MVA(0.01);
    simulator.set_max_DAE_iteration(200);
    simulator.set_max_network_iteration(1);
    simulator.set_iteration_accelerator(0.8);

    PSSE_IMEXPORTER importer(default_toolkit);

    //importer.load_powerflow_data("../../../bench/IEEE39.raw");
    //importer.load_dynamic_data("IEEE39_GENROU_SEXS_IEEEG1.dyr");
    //importer.load_powerflow_data("bench_shandong_v2.raw");
    //importer.load_dynamic_data("bench_shandong_v2.dyr");
    importer.load_powerflow_data("IEEE9_wind.raw");
    importer.load_dynamic_data("IEEE9_wind.dyr");
    //importer.load_dynamic_data("bench_shandong_with_avr.dyr");

    POWERFLOW_SOLVER powerflow_solver(default_toolkit);

    powerflow_solver.set_max_iteration(30);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.00001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.00001);
    powerflow_solver.set_flat_start_logic(false);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_fast_decoupled_solution();

    simulator.prepare_meters();
    //simulator.prepare_hvdc_related_meters();

    //simulator.set_output_file("test_log/IEEE_39_bus_model_GENROU_SEXS_IEEEG1_load_scale_down_at_bus_3_by_10%");
    //simulator.set_output_file("test_log/IEEE_39_bus_model_GENROU_SEXS_IEEEG1_generation_scale_down_at_bus_38_by_10%");
    //simulator.set_output_file("test_log/IEEE_39_bus_model_GENROU_SEXS_IEEEG1_trip_bus_");
    //simulator.set_output_file("test_log/bench_shandong_with_avr_fault_at_line_82_60");
    //simulator.set_output_file("test_log/bench_shandong_with_avr_fault_at_line_82_80");
    //simulator.set_output_file("test_log/bench_shandong_with_avr_trip_mac_140");
    simulator.set_output_file("test_log/IEEE9_wind");

    simulator.start();
    simulator.run_to(1.0);

    /*DEVICE_ID did;
    did.set_device_type(STEPS_LOAD);
    TERMINAL terminal;
    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    did.set_device_identifier_index(get_index_of_string("1"));

    simulator.scale_load(did, -0.1);
    */

    /*did.set_device_type(STEPS_GENERATOR);
    terminal.clear();
    terminal.append_bus(38);
    did.set_device_terminal(terminal);
    did.set_device_identifier_index(get_index_of_string("1"));

    simulator.shed_generator(did, 0.1);
    */

/*
    DEVICE_ID did;
    did.set_device_type(STEPS_LINE);
    TERMINAL terminal;
    terminal.append_bus(82);
    terminal.append_bus(80);
    did.set_device_terminal(terminal);
    did.set_device_identifier_index(get_index_of_string("1"));

    simulator.set_line_fault(did, 82, 0.0, complex<double>(0.0, -1e4));

    simulator.run_to(1.35);

    simulator.clear_line_fault(did, 82, 0.0);*/
/*
    DEVICE_ID did;
    did.set_device_type(STEPS_GENERATOR);
    TERMINAL terminal;
    terminal.append_bus(140);
    did.set_device_terminal(terminal);
    did.set_device_identifier_index(get_index_of_string("1"));

    simulator.trip_generator(did);
*/

    DEVICE_ID did;
    did.set_device_type(STEPS_GENERATOR);
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier_index(get_index_of_string("1"));

    simulator.shed_generator(did, 0.2);

    simulator.run_to(20.0);

    psdb.clear();
    simulator.clear();

    return 0;
}

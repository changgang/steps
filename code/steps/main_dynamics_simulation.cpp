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
    default_toolkit.disable_use_steps_fast_math_logic();

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    POWERFLOW_SOLVER& powerflow_solver = default_toolkit.get_powerflow_solver();
    DYNAMICS_SIMULATOR& simulator = default_toolkit.get_dynamic_simulator();

    psdb.set_allowed_max_bus_number(1000);
    PSSE_IMEXPORTER importer(default_toolkit);

    //importer.load_powerflow_data("../../../bench/IEEE39.raw");
    //importer.load_dynamic_data("IEEE39_GENROU_SEXS_IEEEG1.dyr");
    //importer.load_powerflow_data("bench_shandong_v2.raw");
    //importer.load_dynamic_data("bench_shandong_v2.dyr");

    importer.load_powerflow_data("../../../bench/IEEE39_with_four_vsc.raw");
    cout<<"Done loading powerflow file"<<endl;
    importer.load_vsc_powerflow_data("../../../bench/4_terminal_vsc_hvdc.vscraw");


    powerflow_solver.set_max_iteration(30);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.00001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.00001);
    powerflow_solver.set_flat_start_logic(false);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_full_Newton_Raphson_solution();
    powerflow_solver.save_extended_powerflow_result_to_file("ieee39_vsc4_pf.csv");

    importer.load_dynamic_data("../../../bench/IEEE39_with_four_terminal_vsc.dyr");
    //VSC_HVDC* vsc_hvdc = psdb.get_vsc_hvdc("Vsc-Project1");
    //VSC_HVDC_CONVERTER_MODEL* model = vsc_hvdc->get_vsc_hvdc_converter_model(0);
    //model->report();

    default_toolkit.set_dynamic_simulation_time_step_in_s(0.001);
    simulator.set_allowed_max_power_imbalance_in_MVA(0.01);
    simulator.set_max_DAE_iteration(3);
    simulator.set_min_DAE_iteration(3);
    simulator.set_max_network_iteration(50);
    simulator.set_iteration_accelerator(0.8);
    simulator.set_allowed_max_power_imbalance_in_MVA(0.0001);

    simulator.prepare_vsc_hvdc_related_meters();
    simulator.set_output_file("IEEE39_vsc");
    //simulator.show_dynamic_simulator_configuration();


    //simulator.set_output_file("test_log/IEEE_39_bus_model_GENROU_SEXS_IEEEG1_load_scale_down_at_bus_3_by_10%");
    //simulator.set_output_file("test_log/IEEE_39_bus_model_GENROU_SEXS_IEEEG1_generation_scale_down_at_bus_38_by_10%");
    //simulator.set_output_file("test_log/IEEE_39_bus_model_GENROU_SEXS_IEEEG1_trip_bus_");
    //simulator.set_output_file("test_log/bench_shandong_with_avr_fault_at_line_82_60");
    //simulator.set_output_file("test_log/bench_shandong_with_avr_fault_at_line_82_80");
    //simulator.set_output_file("test_log/bench_shandong_with_avr_trip_mac_140");

    simulator.start();
    simulator.run_to(0.5);

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
*/
    return 0;
}

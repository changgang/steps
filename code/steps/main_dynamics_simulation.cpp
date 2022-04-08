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

    psdb.set_allowed_max_bus_number(10000);
    PSSE_IMEXPORTER importer(default_toolkit);

    //importer.load_powerflow_data("../../../bench/IEEE39.raw");
    //importer.load_dynamic_data("IEEE39_GENROU_SEXS_IEEEG1.dyr");
    //importer.load_powerflow_data("bench_shandong_v2.raw");
    //importer.load_dynamic_data("bench_shandong_v2.dyr");

    //importer.load_powerflow_data("../../../bench/IEEE39_with_four_vsc.raw");
    //cout<<"Done loading powerflow file"<<endl;
    //importer.load_vsc_powerflow_data("../../../bench/4_terminal_vsc_hvdc.vscraw");

    //importer.load_powerflow_data("../../../bench/shandong_original/bench_shandong.raw");
    cout<<"Done loading powerflow file"<<endl;
    //importer.load_vsc_powerflow_data("../../../bench/shandong/4_terminal_vsc_hvdc.vscraw");

    importer.load_powerflow_data("../../../bench/39_bus/IEEE39.raw");
    cout<<"Done loading powerflow file"<<endl;
    importer.load_vsc_powerflow_data("../../../bench/39_bus/2_VSC.vscraw");

    powerflow_solver.set_max_iteration(30);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.00001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.00001);
    powerflow_solver.set_flat_start_logic(false);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_full_Newton_Raphson_solution();
    powerflow_solver.save_network_Y_matrix_to_file("39_bus_Y_matrix.csv");
    powerflow_solver.save_jacobian_matrix_to_file("39_bus_jacobian.csv");
    powerflow_solver.save_extended_powerflow_result_to_file("39_bus.csv");

    //importer.load_dynamic_data("../../../bench/shandong_load/5_bus.dyr");
    importer.load_dynamic_data("../../../bench/39_bus/IEEE39.dyr");

    //vector<GENERATOR*> gens = psdb.get_all_generators();
    //unsigned int n = gens.size();
    //for(unsigned int i=0; i!=n; ++i)
    //{
    //    GENERATOR* gen = gens[i];
    //    SYNC_GENERATOR_MODEL* model = (SYNC_GENERATOR_MODEL*) gen->get_model_of_type("SYNC GENERATOR");
    //    cout<<gen->get_compound_device_name()<<", gen model = "<<model<<endl;
    //}

    //psdb.check_dynamic_data();

    //VSC_HVDC* vsc_hvdc = psdb.get_vsc_hvdc("Vsc-Project1");
    //VSC_HVDC_CONVERTER_MODEL* model = vsc_hvdc->get_vsc_hvdc_converter_model(0);
    //model->report();

    default_toolkit.set_dynamic_simulation_time_step_in_s(0.001);
    simulator.set_allowed_max_power_imbalance_in_MVA(0.01);
    simulator.set_max_DAE_iteration(3);
    simulator.set_min_DAE_iteration(3);
    simulator.set_max_network_iteration(50);
    simulator.set_iteration_accelerator(0.8);
    simulator.set_allowed_max_power_imbalance_in_MVA(0.00001);

/*
    simulator.prepare_bus_related_meter(9101, "voltage in pu");
    simulator.prepare_bus_related_meter(9102, "voltage in pu");
    simulator.prepare_bus_related_meter(9103, "voltage in pu");
    simulator.prepare_bus_related_meter(9104, "voltage in pu");
    simulator.prepare_bus_related_meter(9101, "frequency in pu");
    simulator.prepare_bus_related_meter(9102, "frequency in pu");
    simulator.prepare_bus_related_meter(9103, "frequency in pu");
    simulator.prepare_bus_related_meter(9104, "frequency in pu");
    simulator.prepare_bus_related_meter(9101, "ANGLE IN DEG");
    simulator.prepare_bus_related_meter(9102, "ANGLE IN DEG");
    simulator.prepare_bus_related_meter(9103, "ANGLE IN DEG");
    simulator.prepare_bus_related_meter(9104, "ANGLE IN DEG");
    simulator.prepare_bus_related_meter(9101, "FREQUENCY DEVIATION IN PU");
    simulator.prepare_bus_related_meter(9102, "FREQUENCY DEVIATION IN PU");
    simulator.prepare_bus_related_meter(9103, "FREQUENCY DEVIATION IN PU");
    simulator.prepare_bus_related_meter(9104, "FREQUENCY DEVIATION IN PU");
    simulator.prepare_bus_related_meter(9101, "FREQUENCY DEVIATION IN HZ");
    simulator.prepare_bus_related_meter(9102, "FREQUENCY DEVIATION IN HZ");
    simulator.prepare_bus_related_meter(9103, "FREQUENCY DEVIATION IN HZ");
    simulator.prepare_bus_related_meter(9104, "FREQUENCY DEVIATION IN HZ");
*/

    simulator.prepare_bus_related_meters();
    simulator.prepare_vsc_hvdc_related_meters();
    simulator.prepare_generator_related_meters();
    //simulator.prepare_load_related_meters();
    //simulator.prepare_wt_generator_related_meters();

    simulator.set_output_file("IEEE39_vsc");

    //simulator.set_output_file("IEEE39_2VSC");


    //simulator.show_dynamic_simulator_configuration();


    //simulator.set_output_file("test_log/IEEE_39_bus_model_GENROU_SEXS_IEEEG1_load_scale_down_at_bus_3_by_10%");
    //simulator.set_output_file("test_log/IEEE_39_bus_model_GENROU_SEXS_IEEEG1_generation_scale_down_at_bus_38_by_10%");
    //simulator.set_output_file("test_log/IEEE_39_bus_model_GENROU_SEXS_IEEEG1_trip_bus_");
    //simulator.set_output_file("test_log/bench_shandong_with_avr_fault_at_line_82_60");
    //simulator.set_output_file("test_log/bench_shandong_with_avr_fault_at_line_82_80");
    //simulator.set_output_file("test_log/bench_shandong_with_avr_trip_mac_140");


    simulator.start();
    simulator.run_to(0.01);
/*
    DEVICE_ID did;
    did.set_device_type(STEPS_LOAD);
    TERMINAL terminal;
    terminal.append_bus(103);
    did.set_device_terminal(terminal);
    did.set_device_identifier_index(get_index_of_string("1"));

    simulator.scale_load(did, 0.05);
    */
    /*
    DEVICE_ID did;
    did.set_device_type(STEPS_LINE);
    TERMINAL terminal;
    terminal.append_bus(16);
    terminal.append_bus(21);
    did.set_device_terminal(terminal);
    did.set_device_identifier_index(get_index_of_string("1"));
    simulator.set_line_fault(did, 16, 0.0, complex<double>(0, -100));
    simulator.run_to(1.1);
    simulator.clear_line_fault(did, 16, 0.0);
    */

    DEVICE_ID did;
    did.set_device_type(STEPS_LOAD);
    TERMINAL terminal;
    terminal.append_bus(23);
    did.set_device_terminal(terminal);
    did.set_device_identifier_index(get_index_of_string("1"));
    simulator.scale_load(did, 0.5);

/*
    did.set_device_type(STEPS_WT_GENERATOR);
    TERMINAL terminal;
    terminal.append_bus(103);
    did.set_device_terminal(terminal);
    did.set_device_identifier_index(get_index_of_string("1"));

    simulator.shed_wt_generator(did, 0.01);
*/
    /*
    DEVICE_ID did;
    did.set_device_type(STEPS_GENERATOR);
    TERMINAL terminal;
    terminal.append_bus(104);
    did.set_device_terminal(terminal);
    did.set_device_identifier_index(get_index_of_string("1"));

    simulator.trip_generator(did);
    */

    /*DEVICE_ID did;
    did = get_hvdc_device_id(57,201,"ZaQingDi");
    simulator.manual_block_hvdc(did);*/
    //simulator.run_to(5);


    //simulator.scale_all_load(-0.05);
    simulator.run_to(5);


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

#include "header/basic/utility.h"
#include "header/data_imexporter/psse_imexporter.h"
#include "header/prepare_for_tests/prepare_models_for_test.h"
#include "header/meter/meter_setter.h"
#include "header/steps_namespace.h"
#include "header/toolkit/powerflow_solver/powerflow_solver.h"
#include "header/toolkit/dynamic_simulator/dynamic_simulator.h"
#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <cstdio>
#include <cmath>
#include <ctime>

int main()
{
    clock_t start = clock();
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

    //simulator.prepare_meters();

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

    simulator.set_line_fault(did, 60, 0.0, complex<double>(0.0, -1e6));

    simulator.run_to(1.1);

    simulator.clear_line_fault(did, 60, 0.0);
    simulator.trip_line(did);

    simulator.run_to(5.0);
    default_toolkit.close_log_file();

    clock_t stop = clock();

    cout<<"time elapse: "<<double(stop-start)*1e-6<<" s"<<endl;
    return 0;
}

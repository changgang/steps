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

    ostringstream osstream;

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    POWERFLOW_SOLVER& powerflow_solver = default_toolkit.get_powerflow_solver();
    DYNAMICS_SIMULATOR& simulator = default_toolkit.get_dynamic_simulator();

    psdb.set_allowed_max_bus_number(10000);
    PSSE_IMEXPORTER importer(default_toolkit);

    importer.load_powerflow_data("../../../bench/IEEE9.raw");
    cout<<"Done loading powerflow file"<<endl;

    powerflow_solver.set_max_iteration(30);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.00001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.00001);
    powerflow_solver.set_flat_start_logic(false);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_full_Newton_Raphson_solution();
    powerflow_solver.save_network_Y_matrix_to_file("9_bus_Y_matrix.csv");
    powerflow_solver.save_jacobian_matrix_to_file("9_bus_jacobian.csv");
    powerflow_solver.save_extended_powerflow_result_to_file("9_bus.csv");

    importer.load_dynamic_data("IEEE9_ssa.dyr");

    default_toolkit.set_dynamic_simulation_time_step_in_s(0.001);
    simulator.set_allowed_max_power_imbalance_in_MVA(0.01);
    simulator.set_max_DAE_iteration(3);
    simulator.set_min_DAE_iteration(3);
    simulator.set_max_network_iteration(50);
    simulator.set_iteration_accelerator(0.8);
    simulator.set_allowed_max_power_imbalance_in_MVA(0.00001);

    simulator.start(); // initialize all devices

    cout<<"Done initialization"<<endl;

    vector<GENERATOR*> gens = psdb.get_all_generators();
    unsigned int n = gens.size();
    for(unsigned int i = 0; i<n; ++i)
    {
        GENERATOR* gen = gens[i];
        osstream<<"Show ABCD of "<<gen->get_compound_device_name();
        default_toolkit.show_information_with_leading_time_stamp(osstream);

        gen->build_linearized_matrix_ABCD();
    }

    return 0;
}

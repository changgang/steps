#include <iostream>
#include "header/power_system_database.h"
#include "header/toolkit/powerflow_solver/powerflow_solver.h"
#include "header/basic/utility.h"
#include "header/data_imexporter/psse_imexporter.h"
using namespace std;

int main()
{
    initialize_simulator(); // this function should be called first

    POWER_SYSTEM_DATABASE psdb;
    POWER_SYSTEM_DATABASE* psdb_ptr = (&psdb);

    PSSE_IMEXPORTER importer;
    importer.set_power_system_database(psdb_ptr);
    importer.load_powerflow_data("IEEE39.raw");

    POWERFLOW_SOLVER solver;
    solver.set_power_system_database(psdb_ptr);
    solver.set_max_iteration(30);
    solver.set_allowed_max_active_power_imbalance_in_MW(0.00001);
    solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.00001);
    solver.set_flat_start_logic(false);
    solver.set_transformer_tap_adjustment_logic(true);

    solver.solve_with_fast_decoupled_solution();
    solver.solve_with_full_Newton_Raphson_solution();

    solver.show_powerflow_result();
    solver.save_network_matrix_to_file("IEEE39_network_matrix.csv");
    solver.save_jacobian_matrix_to_file("IEEE39_jacobian_matrix.csv");
    solver.save_bus_powerflow_result_to_file("IEEE39_bus_powerflow_result.csv");

    terminate_simulator();
    return 0;
}

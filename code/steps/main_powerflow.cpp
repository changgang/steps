#include <istream>
#include <iostream>
#include "header/power_system_database.h"
#include "header/steps_namespace.h"
#include "header/toolkit/powerflow_solver/powerflow_solver.h"
#include "header/basic/utility.h"
#include "header/data_imexporter/psse_imexporter.h"
using namespace std;

int main()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.set_allowed_max_bus_number(100000);
    PSSE_IMEXPORTER importer(default_toolkit);
    importer.load_powerflow_data("../../../bench/sample.raw");
    /*
    POWERFLOW_SOLVER solver(default_toolkit);
    solver.set_max_iteration(30);
    solver.set_allowed_max_active_power_imbalance_in_MW(0.00001);
    solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.00001);
    solver.set_flat_start_logic(false);
    solver.set_transformer_tap_adjustment_logic(true);

    solver.solve_with_fast_decoupled_solution();
    solver.solve_with_full_Newton_Raphson_solution();

    solver.save_network_Y_matrix_to_file("IEEE39_network_matrix.csv");
    solver.save_jacobian_matrix_to_file("IEEE39_jacobian_matrix.csv");
    solver.save_bus_powerflow_result_to_file("IEEE39_bus_powerflow_result.csv");
    */
    return 0;
}

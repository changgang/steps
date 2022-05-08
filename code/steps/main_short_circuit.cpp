#include <istream>
#include <iostream>
#include "header/power_system_database.h"
#include "header/steps_namespace.h"
#include "header/toolkit/short_circuit_solver/short_circuit_solver.h"
#include "header/toolkit/powerflow_solver/powerflow_solver.h"
#include "header/basic/utility.h"
#include "header/data_imexporter/psse_imexporter.h"
using namespace std;


int main()
{
    PSSE_IMEXPORTER importer(default_toolkit);
    importer.load_powerflow_data("../../../bench/IEEE9.raw");
    importer.load_sequence_data("../../../bench/IEEE9.seq");

//    importer.load_powerflow_data("../../../bench/IEEE39.raw");
//    importer.load_sequence_data("../../../bench/IEEE39.seq");


    POWERFLOW_SOLVER solver(default_toolkit);
    solver.set_max_iteration(30);
    solver.set_allowed_max_active_power_imbalance_in_MW(0.00001);
    solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.00001);
    solver.set_flat_start_logic(false);
    solver.set_transformer_tap_adjustment_logic(true);

    solver.solve_with_fast_decoupled_solution();
    solver.show_powerflow_result();

    SHORT_CIRCUIT_SOLVER sc_solver(default_toolkit);
    sc_solver.set_generator_reactance_option(SUBTRANSIENT_REACTANCE);
    sc_solver.set_units_of_currents_and_voltages(PU);
    sc_solver.set_coordinates_of_currents_and_voltages(RECTANGULAR);
    sc_solver.set_consider_load_logic(true);
    sc_solver.set_consider_motor_load_logic(true);

    sc_solver.set_bus_fault(3, SINGLE_PHASE_GROUNDED_FAULT, complex<double>(0.0, -1e3));

 //   DEVICE_ID did = get_line_device_id(9, 8, "1");
 //   sc_solver.set_line_fault(did, 9, 0.5, SINGLE_PHASE_GROUNDED_FAULT, complex<double>(0.0, -1e3));

    sc_solver.solve();
    sc_solver.show_short_circuit_result();

    importer.export_sequence_data("IEEE9.seq");

    sc_solver.save_positive_sequence_Y_matrix_to_file("positive_sequence_Y.csv");
    sc_solver.save_short_circuit_result_to_file("short_circuit_result.csv");
    sc_solver.save_extended_short_circuit_result_to_file("extended_short_circuit_result.csv");

    return 0;
}

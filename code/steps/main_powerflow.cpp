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

    /*
    cout<<"Start load IEEE39_with_4_terminal_vsc data"<<endl;
    importer.load_powerflow_data("../../../bench/IEEE39_with_four_vsc.raw");
    importer.load_vsc_powerflow_data("../../../bench/4_terminal_vsc_hvdc.vscraw");
    cout<<"Done loading IEEE39_with_4_terminal_vsc data"<<endl;
    */

    cout<<"Start load IEEE57_with_5_terminal_vsc data"<<endl;
    importer.load_powerflow_data("../../../bench/IEEE57_with_five_vsc.raw");
    importer.load_vsc_powerflow_data("../../../bench/5_terminal_vsc_hvdc.vscraw");
    cout<<"Done loading IEEE57_with_5_terminal_vsc data"<<endl;

    /*
    cout<<"Start load IEEE57_with_7_terminal_vsc data"<<endl;
    importer.load_powerflow_data("../../../bench/IEEE57_with_seven_vsc.raw");
    importer.load_vsc_powerflow_data("../../../bench/7_terminal_vsc_hvdc.vscraw");
    cout<<"Done loading IEEE57_with_7_terminal_vsc data"<<endl;
    */

    cout<<"Done loading powerflow file"<<endl;

    POWERFLOW_SOLVER solver(default_toolkit);
    solver.set_max_iteration(20);
    solver.set_non_divergent_solution_logic(false);
    solver.set_allowed_max_active_power_imbalance_in_MW(0.0001);
    solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.0001);
    solver.set_flat_start_logic(false);
    solver.set_transformer_tap_adjustment_logic(true);

    //solver.solve_with_fast_decoupled_solution();
    solver.solve_with_full_Newton_Raphson_solution();

    /*
    solver.save_network_Y_matrix_to_file("IEEE39_with_four_terminal_vsc_network_matrix.csv");
    solver.save_jacobian_matrix_to_file("IEEE39_with_four_terminal_vsc_jacobian_matrix.csv");
    solver.save_powerflow_result_to_file("IEEE39_with_four_terminal_vsc_bus_powerflow_result.csv");
    solver.save_extended_powerflow_result_to_file("IEEE39_with_four_terminal_vsc_detailed_result.csv");
    */

    solver.save_network_Y_matrix_to_file("IEEE57_with_five_terminal_vsc_network_matrix.csv");
    solver.save_jacobian_matrix_to_file("IEEE57_with_five_terminal_vsc_jacobian_matrix.csv");
    solver.save_powerflow_result_to_file("IEEE57_with_five_terminal_vsc_bus_powerflow_result.csv");
    solver.save_extended_powerflow_result_to_file("IEEE57_with_five_terminal_vsc_detailed_result.csv");

    /*
    solver.save_network_Y_matrix_to_file("IEEE57_with_seven_terminal_vsc_network_matrix.csv");
    solver.save_jacobian_matrix_to_file("IEEE57_with_seven_terminal_vsc_jacobian_matrix.csv");
    solver.save_powerflow_result_to_file("IEEE57_with_seven_terminal_vsc_bus_powerflow_result.csv");
    solver.save_extended_powerflow_result_to_file("IEEE57_with_seven_terminal_vsc_detailed_result.csv");
    */
    vector<DEVICE_ID> dids  = psdb.get_all_devices_device_id_connecting_to_bus(1);
    unsigned int n = dids.size();
    for(unsigned int i=0; i!=n; ++i)
    {
        cout<<dids[i].get_compound_device_name()<<endl;
    }

    return 0;
}

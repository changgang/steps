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

int main(int argc, char* argv[])
{
    clock_t start = clock();
    int nthread = 1;
    if(argc>0) nthread = str2int(argv[0]);
    default_toolkit.set_thread_number(nthread);

    DYNAMICS_SIMULATOR& simulator = default_toolkit.get_dynamic_simulator();
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    default_toolkit.set_dynamic_model_database_size_in_bytes(1000000000);

    string file = "log.txt";
    default_toolkit.open_log_file(file);

    PSSE_IMEXPORTER importer(default_toolkit);

    psdb.set_allowed_max_bus_number(25000);
    psdb.set_bus_capacity(11000);
    psdb.set_generator_capacity(1200);
    psdb.set_wt_generator_capacity(3000);
    psdb.set_pv_unit_capacity(1900);
    psdb.set_energy_storage_capacity(300);
    psdb.set_ac_line_capacity(10000);
    psdb.set_transformer_capacity(4500);
    psdb.set_fixed_shunt_capacity(3100);
    psdb.set_2t_lcc_hvdc_capacity(50);
    psdb.set_load_capacity(14000);

    importer.load_powerflow_data("nccc10k.raw");

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

    importer.load_dynamic_data("nccc10k.dyr");

    simulator.prepare_bus_related_meters(306);

    vector<GENERATOR*> gens = psdb.get_all_generators();
    for(unsigned int i=0; i<1; ++i)
    {
        GENERATOR* gen = gens[i];
        simulator.prepare_generator_related_meters(gen->get_device_id());
    }
    vector<WT_GENERATOR*> wt_gens = psdb.get_all_wt_generators();
    for(unsigned int i=0; i<1; ++i)
    {
        WT_GENERATOR* wtgen = wt_gens[i];
        simulator.prepare_wt_generator_related_meters(wtgen->get_device_id());
    }
    vector<PV_UNIT*> pvus = psdb.get_all_pv_units();
    for(unsigned int i=0; i<1; ++i)
    {
        PV_UNIT* pvu = pvus[i];
        simulator.prepare_pv_unit_related_meters(pvu->get_device_id());
    }

    simulator.set_output_file("bench_NCCC_10k_bus_model_dynamic_test");
    simulator.set_allowed_max_power_imbalance_in_MVA(0.001);
    //simulator.set_max_DAE_iteration(20);
    //simulator.set_max_network_iteration(200);
    default_toolkit.set_dynamic_simulation_time_step_in_s(0.0025);

    simulator.start();
    simulator.run_to(0.1);

    vector<DEVICE_ID> lines = psdb.get_ac_lines_device_id_connecting_to_bus(306);

    DEVICE_ID did;
    for(unsigned int i=0; i<lines.size(); ++i)
    {
        did = lines[i];
        TERMINAL terminal = did.get_device_terminal();
        unsigned int ibus = terminal[0];
        unsigned int jbus = terminal[1];
        if(ibus == 4818 or jbus == 4818)
            break;
    }

    simulator.set_ac_line_fault(did, 306, 0.0, complex<double>(0.0, -1e6));

    simulator.run_to(0.2);

    simulator.clear_ac_line_fault(did, 306, 0.0);
    simulator.trip_ac_line(did);

    simulator.run_to(0.5);

    clock_t stop = clock();

    cout<<"time elapse: "<<double(stop-start)*1e-6<<" s"<<endl;
    return 0;
}

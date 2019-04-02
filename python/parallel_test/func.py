import pysteps2
def run(fault_bus):
    simulator = pysteps2.STEPS()
    simulator.clear_package()
    
    simulator.set_allowed_maximum_bus_number(10000)

    simulator.load_powerflow_data('IEEE9.raw','PSS/E')

    simulator.set_powerflow_solver_parameter('bool','flat start logic', True)

    simulator.solve_powerflow('NR')
    #simulator.show_powerflow_result()

    simulator.load_dynamic_data('IEEE9.dyr','PSS/E')
    
    simulator.set_dynamic_simulator_parameter('i', 'max_DAE_iter', 100)
    simulator.set_dynamic_simulator_parameter('i', 'max_NET_iter', 10)
    simulator.set_dynamic_simulation_time_step(0.005)

    outfile = 'shandong_bus_'+str(fault_bus)+'_fault'

    simulator.set_dynamic_simulator_output_file(outfile)

    buses = simulator.get_all_buses()
    for bus in buses:
        simulator.prepare_bus_meter(bus, "voltage in pu")
        
    simulator.start_dynamic_simulation()
    simulator.run_dynamic_simulation_to_time(0.0)
    
    fault_bus = fault_bus%10
    if fault_bus==0:
        fault_bus =1
    simulator.set_bus_fault(fault_bus, "THREE PHASE FAULT", (0, -1e6))
    simulator.run_dynamic_simulation_to_time(0.1)
    
    simulator.clear_bus_fault(fault_bus,"THREE PHASE FAULT")
    simulator.run_dynamic_simulation_to_time(10.0)

    simulator.stop_dynamic_simulation()
    
    simulator.terminate_package()
import pysteps

pysteps.load_powerflow_data('ieee39.raw','PSS/E')

pysteps.set_powerflow_solver_parameter('integer','Max iteration', 100)
pysteps.set_powerflow_solver_parameter('bool','flat start logic', True)
pysteps.set_powerflow_solver_parameter('double','Max active power imbalance in MW', 0.001)
pysteps.set_powerflow_solver_parameter('double','Max reactive power imbalance in MVar', 0.001)

pysteps.solve_powerflow('NR')
pysteps.save_powerflow_result('powerflow_result_initial.csv')
pysteps.save_network_matrix('y_matrix.csv')
pysteps.save_jacobian_matrix('jacobian_matrix.csv')


pysteps.set_powerflow_solver_parameter('bool','flat start logic', False)

deltP =  1.0



buses = pysteps.get_all_buses()
for bus in buses:
    bus_type = pysteps.get_bus_data(bus, 'integer', 'bus type')
    if bus_type == 3:
        pysteps.set_bus_data(bus, 'integer', 'bus type', 2)
        break
        
generators = pysteps.get_generators_at_bus(0)
loads = pysteps.get_loads_at_bus(0)

for generator in generators:
    gen_bus = generator[0]
    id = generator[1]    
    pysteps.set_bus_data(gen_bus, 'integer', 'bus type', 3)
    Pgen0 = pysteps.get_generator_data(generator,'double', "PGen_MW")
    for load in loads:
        load_bus = load[0]
        P0 = pysteps.get_load_data(load, 'double', 'PP0_MW')
        pysteps.set_load_data(load, 'double', 'PP0_MW', P0+deltP)
        pysteps.solve_powerflow('NR')
        powerflow_result_file = "powerflow_result_"+str(gen_bus)+"_"+str(load_bus)
        converged = pysteps.is_powerflow_converged()
        if converged:
            powerflow_result_file = powerflow_result_file+".csv"
        else:
            powerflow_result_file = powerflow_result_file+"_not_converged.csv"
        pysteps.save_powerflow_result(powerflow_result_file)
        pysteps.set_load_data(load, 'double', 'PP0_MW', P0)
        
    pysteps.set_bus_data(gen_bus, 'integer', 'bus type', 2)
    pysteps.set_generator_data(generator,'double', "PGen_MW", Pgen0)
        




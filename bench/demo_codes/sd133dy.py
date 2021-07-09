# this case in section 2.10 of Power System Control and Stability by P.M. Anderson
# April 1, 2021
# Changgang Li
from stepspy import STEPS # import modules

simulator = STEPS(is_default=True, log_file="sd133.log") # create simulator, use default simulator, and redirect logs to file ieee9.log
simulator.set_allowed_maximum_bus_number(10000) # set allowed maximum bus number, no bus with bus number greater than this number is allowed

simulator.load_powerflow_data('../sd133.raw','PSS/E') # load powerflow data
simulator.set_powerflow_solver_parameter('b','FLAT START Logic', True) # enable flat start logic
simulator.set_powerflow_solver_parameter('i','MAX ITERATION', 50) # set up maximum iteration
simulator.set_powerflow_solver_parameter('b','EXPORT JACOBIAN LOGIC', False) # disable exporting Jacobian matrix
simulator.set_powerflow_solver_parameter('b','NON DIVERGENT SOLUTION LOGIC', True) # enable non-divergent solution
simulator.set_powerflow_solver_parameter('d','MAX VOLTAGE CHANGE IN PU', 0.3) # set up maximum voltage change in each iteration
simulator.set_powerflow_solver_parameter('d','MAX ANGLE CHANGE IN DEG', 60.0) # set up maximum angle change in each iteration
simulator.set_powerflow_solver_parameter('d','MAX ACTIVE POWER IMBALANCE IN MW', 0.001) # maximum active power mismatch
simulator.set_powerflow_solver_parameter('d','MAX REACTIVE POWER IMBALANCE IN MVAR', 0.001) # maximum reactive power mismatch
simulator.solve_powerflow('NR') # solve with Newton-Raphson method
simulator.show_powerflow_result() # show powerflow result
simulator.save_network_Y_matrix("y.csv",export_full=True) # export Y matrix, both sparse and full version

simulator.set_dynamic_model_database_capacity(1000000) # set dynamic model database capacity, in bytes
simulator.load_dynamic_data('../sd133.dyr','PSS/E') # load dynamic data

buses = simulator.get_all_buses() # get all buses
gens = simulator.get_all_generators() # get all generators

for bus in buses: # prepare bus voltage and frequency meter for all buses
    simulator.prepare_bus_meter(bus, "voltage in pu")
    simulator.prepare_bus_meter(bus, "frequency in Hz")
for gen in gens: # prepare rotor angle meter for all generators
    simulator.prepare_generator_meter(gen, "rotor angle in deg")

simulator.set_dynamic_simulator_output_file("sd133dc") # set dynamic simulation output file for storing the meters
simulator.set_dynamic_simulation_time_step(0.001) # set dynamic simulation time step in seconds

simulator.start_dynamic_simulation() # start dynamic simulation by performing initialization
simulator.run_dynamic_simulation_to_time(0.0) # run dynamic simulation to 1.0s without events
simulator.manually_block_hvdc((704, 125, "MD2SD-DC-HIG")) # trip line 1 between bus 5 and 7
simulator.run_dynamic_simulation_to_time(10.0) # run dynamic simulation to 10.0 with fault cleared
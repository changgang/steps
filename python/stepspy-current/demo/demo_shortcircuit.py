from cmath import pi, sqrt
from stepspy import STEPS # import modules
import os
import math

simulator = STEPS(is_default=True, log_file="ieee39.log") # create simulator, use default simulator, and redirect logs to file ieee9.log
simulator.set_allowed_maximum_bus_number(10000) # set allowed maximum bus number, no bus with bus number greater than this number is allowed

simulator.load_powerflow_data('ieee39.raw','PSS/E') # load powerflow data
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
#simulator.save_network_Y_matrix("y.csv",export_full=True) # export Y matrix, both sparse and full version

simulator.load_sequence_data('ieee39.seq','PSS/E')

simulator.set_short_circuit_solver_parameter('i', 'GENERATOR REACTANCE', 0)
simulator.set_short_circuit_solver_parameter('i', 'UNITS', 0)
simulator.set_short_circuit_solver_parameter('i', 'COORDINATES', 0)
simulator.set_short_circuit_solver_parameter('b', 'CONSIDER LOAD LOGIC', True)
simulator.set_short_circuit_solver_parameter('b', 'CONSIDER MOTOR LOAD LOGIC', False)
simulator.set_short_circuit_solver_parameter('i', 'OPTION OF DC LINES', 1)
simulator.set_short_circuit_solver_parameter('i', 'MAX ITER COUNT', 50)
simulator.set_short_circuit_solver_parameter('F', 'CURRENT THRESHOLD', 0.01)
simulator.set_short_circuit_solver_parameter('B', "EXPORT DETAILED ITERATION LOGIC", True)
simulator.set_short_circuit_solver_parameter('B', "Consider negative sequence support of RES logic", False)

# simulator.set_short_circuit_bus_fault(7, "three phases fault", (0.0,-1e3))
simulator.set_short_circuit_bus_fault(19, "single phase grounded fault", (0.0,-1e3))
simulator.solve_short_circuit_with_iteration()
simulator.show_short_circuit_result()
simulator.save_short_circuit_result('result.csv')
simulator.save_extended_short_circuit_result('extended_result.csv')
simulator.save_positive_sequence_network_Y_matrix("ieee_pos_Y.csv")
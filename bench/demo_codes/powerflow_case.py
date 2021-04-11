# this case in section 2.10 of Power System Control and Stability by P.M. Anderson
# April 1, 2021
# Changgang Li
from stepspy import STEPS # import modules

simulator = STEPS(is_default=True, log_file="ieee9_pf.log") # create simulator, use default simulator, and redirect logs to file ieee9.log
simulator.set_allowed_maximum_bus_number(10000) # set allowed maximum bus number, no bus with bus number greater than this number is allowed

simulator.load_powerflow_data('../ieee9.raw','PSS/E') # load powerflow data
simulator.set_powerflow_solver_parameter('b','FLAT START Logic', True) # enable flat start logic
simulator.set_powerflow_solver_parameter('b','NON DIVERGENT SOLUTION LOGIC', False) # enable non-divergent solution
simulator.set_powerflow_solver_parameter('d','MAX ACTIVE POWER IMBALANCE IN MW', 0.001) # maximum active power mismatch
simulator.set_powerflow_solver_parameter('d','MAX REACTIVE POWER IMBALANCE IN MVAR', 0.001) # maximum reactive power mismatch
simulator.solve_powerflow('NR') # solve with Newton-Raphson method
simulator.save_extended_powerflow_result("ieee9_pf.csv") # show powerflow result


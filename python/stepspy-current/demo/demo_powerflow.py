#coding = utf-8
'''
Here is a demo of showing how to slove powerflow with stepspy.
Changgang Li, 2019/08/25
'''

from stepspy import STEPS  # import the class 'STEPS'

simulator = STEPS(is_default=True) # create a STEPS simulator instance
simulator.info()

powerflow_data_file = 'IEEE9.raw' # file name of powerflow data. Use absolute path if necessary
powerflow_data_type = 'PSS/E' # powerflow data type. Currently, use 'PSS/E' only

simulator.load_powerflow_data(powerflow_data_file, powerflow_data_type) # load powerflow data into the simulator

data_type = 'D' # if you want to set or get doubule data, set data_type as 'F' or 'D'.
data_name = 'MAX ACTIVE POWER IMBALANCE IN MW' # the data name in the powerflow solver of the simulator
# the data_type and data_name should be consistent. make sure the data_type is correct. 
# If the data is double, use 'F' or 'D'. If the data is integer, use 'I'. If the data is boolean, use 'B'. If the data is string, use 'S'
'''
(1) when data_type is 'D' or 'F' you can set/get the following data
 'MAX ACTIVE POWER IMBALANCE IN MW': maximum allowed active power mismatch at each bus, in MW. This is the powerflow convergence threshold of P equations.
 'MAX REACTIVE POWER IMBALANCE IN MVAR': maximum allowed reactive power mismatch at each bus, in MVar. This is the powerflow convergence threshold of Q equations.
 'ITERATION ACCELERATOR': acceleration factor for iteration. by default it is 1.0. if >1.0, then the powerflow solver is accelerated. if <1.0, the powerflow solver is decellerated.
 
 (2) when data_type is 'I', you can set/get the following data
 'MAX ITERATION': maximum iteration count allowed for solving powerflow. If set as 1, you can get the solution step by step.
 
 (3)when data_type is 'B', you can set/get the following data
 'FLAT START LOGIC': if true, powerflow will be solved with unity voltage profile (1.0pu, 0.0deg), if false, poewrflow will be solved from the current voltage profile. 
'''

# here goes get and set maximum active power imbalance in MW
data_type = 'D'
data_name = 'MAX ACTIVE POWER IMBALANCE IN MW'
P_error_MW = simulator.get_powerflow_solver_parameter(data_type, data_name)

value = 0.001
simulator.set_powerflow_solver_parameter(data_type, data_name, value)

# here goes get and set maximum reactive power imbalance in MVAR
data_type = 'D'
data_name = 'MAX REACTIVE POWER IMBALANCE IN MVAR'
Q_error_MVar = simulator.get_powerflow_solver_parameter(data_type, data_name)

value = 0.001
simulator.set_powerflow_solver_parameter(data_type, data_name, value)

# here goes get and set maximum iteration
data_type = 'I'
data_name = 'MAX ITERATION'
Iter_max = simulator.get_powerflow_solver_parameter(data_type, data_name)

value = 50
simulator.set_powerflow_solver_parameter(data_type, data_name, value)

# here goes get and set flat start logic
data_type = 'B'
data_name = 'FLAT START LOGIC'
flat_flag = simulator.get_powerflow_solver_parameter(data_type, data_name)

value = False
simulator.set_powerflow_solver_parameter(data_type, data_name, value)

# now assuming that maximum active and reactive power imbalance are already set.
# show how to solve powerflow

# solve powerflow with flat start logic disabled
data_type = 'B'
data_name = 'FLAT START LOGIC'
value = False
simulator.set_powerflow_solver_parameter(data_type, data_name, value)

simulator.solve_powerflow('NR') # use 'NR' for Newton-Raphson solution, use 'PQ' for PQ decoupled solution

# solve powerflow with flat start logic enabled
data_type = 'B'
data_name = 'FLAT START LOGIC'
value = True
simulator.set_powerflow_solver_parameter(data_type, data_name, value)

simulator.solve_powerflow('PQ')

# if you want to solve powerflow step by step to get the solution process,
# you can set MAX ITERATION as 1, and Flat start logic as false
data_type = 'I'
data_name = 'MAX ITERATION'
value = 1
simulator.set_powerflow_solver_parameter(data_type, data_name, value)

data_type = 'B'
data_name = 'FLAT START LOGIC'
value = True
simulator.set_powerflow_solver_parameter(data_type, data_name, value)

simulator.solve_powerflow('NR') # first slove it with flat start enable

data_type = 'B'
data_name = 'FLAT START LOGIC'
value = False
simulator.set_powerflow_solver_parameter(data_type, data_name, value) # from now on, disable flat start

while not simulator.is_powerflow_converged(): # use is_powerflow_converged() to check if powerflow is converged
    simulator.solve_powerflow('NR')
    simulator.save_jacobian_matrix('jacobian.txt') # if you are solving with NR method, you can get jacobian matrix of each iteration in the file

# once powerflow is converged, you can export powerflow result to file
powerflow_result_file = 'pf_result.txt'
simulator.save_powerflow_result(powerflow_result_file) # you can check the file's contents

# you can get power loss of a solved powerflow case
ploss_MW = simulator.get_powerflow_loss() # in MW
print('Loss is:', ploss_MW)

# if you want to get the voltage of each bus, you can try the following codes
buses = simulator.get_all_buses()
for bus in buses:
    bus_name = simulator.get_bus_data(bus, 'S', 'Name')
    voltage = simulator.get_bus_data(bus, 'D', 'Voltage in PU')
    angle = simulator.get_bus_data(bus, 'D', 'Angle in deg')
    print(bus, bus_name, voltage, angle)
    
# if you want to get the generation of each generator, you can try the following codes
generators = simulator.get_generators_at_bus(0) # 0 indicate all generators will be returned
for generator in generators:
    P = simulator.get_generator_data(generator, 'D', 'PGEN_MW')
    Q = simulator.get_generator_data(generator, 'D', 'QGEN_MVAR')
    print(generator, P, Q)
    
# if you want to get the load of each load, you can try the following codes
loads = simulator.get_loads_at_bus(0) # 0 indicate all loads will be returned
for load in loads:
    P = simulator.get_load_data(load, 'D', 'P_MW')
    Q = simulator.get_load_data(load, 'D', 'Q_MVAR')
    print(load, P, Q)
    
# if you want to get the power of each line, you can try the following codes
lines = simulator.get_lines_at_bus(0) # 0 indicate all lines will be returned
for line in lines:
    bus_send = simulator.get_line_data(line, 'I', 'BUS_SEND') # get the bus number of sending side
    bus_recv = simulator.get_line_data(line, 'I', 'BUS_RECV') # get the bus number of receiving side
    Psend = simulator.get_line_data(line, 'D', 'PSEND_MW') # active power at sending side
    Qsend = simulator.get_line_data(line, 'D', 'QSEND_MVAR') # reactive power at sending side
    Precv = simulator.get_line_data(line, 'D', 'PRECV_MW') # active power at receiving side
    Qrecv = simulator.get_line_data(line, 'D', 'QRECV_MVAR') # reactive power at receiving side
    print(line, bus_send, (Psend, Qsend), bus_recv, (Precv, Qrecv))
    
# if you want to get the power of each transformer, you can try the following codes
transformers = simulator.get_transformers_at_bus(0) # 0 indicate all transformers will be returned
for transformer in transformers:
    bus_pri = simulator.get_transformer_data(transformer, 'I', 'Primary', 'BUS') # get the bus number of primary side
    bus_sec = simulator.get_transformer_data(transformer, 'I', 'Secondary', 'BUS') # get the bus number of secondary side

    P_pri = simulator.get_transformer_data(transformer, 'D', 'Primary', 'P_MW') # active power at primary side
    Q_pri = simulator.get_transformer_data(transformer, 'D', 'Primary', 'Q_MVAR') # reactive power at primary side
    P_sec = simulator.get_transformer_data(transformer, 'D', 'Secondary', 'P_MW') # active power at secondary side
    Q_sec = simulator.get_transformer_data(transformer, 'D', 'Secondary', 'Q_MVAR') # reactive power at secondary side
    print(transformer, bus_pri, (P_pri, Q_pri), bus_sec, (P_sec, Q_sec))
    


# if you want to change generation of each generaor, trye the following codes
generator = (2,'1') # generator bus, and generator ID, check generator line of raw file
simulator.set_generator_data(generator, 'D', 'PGEN_MW', 50.0) # remember, only P of generator at bus of type 2 can be changed

data_type = 'I'
data_name = 'MAX ITERATION'
value = 10
simulator.set_powerflow_solver_parameter(data_type, data_name, value)

data_type = 'B'
data_name = 'FLAT START LOGIC'
value = True
simulator.set_powerflow_solver_parameter(data_type, data_name, value)

simulator.solve_powerflow('NR')

newfile = "IEEE9.new.raw"
file_type = "PSS/E"
export_mode = 0 # keep as original
export_mode = 1 # order with bus number
export_mode = 2 # order with bus name
export_mode = 3 # order for dynamic simulation
simulator.save_powerflow_data(newfile, file_type, export_mode)


simulator.build_network_Y_matrix()
simulator.save_network_Y_matrix('ymatrix_pf.csv')

simulator.build_decoupled_network_B_matrix()
simulator.save_decoupled_network_B_matrix('bmatrix_pf.csv')

simulator.build_dc_network_B_matrix()
simulator.save_dc_network_B_matrix('bmatrix_dc_pf.csv')

simulator.build_network_Z_matrix()
simulator.save_network_Z_matrix('zmatrix_pf.csv')
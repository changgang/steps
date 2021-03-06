from stepspy import STEPS, POUCH_CSV

simulator = STEPS(is_default = False, log_file = 'test.log')
simulator.info()

simulator.set_toolkit_log_file("newtest.log", log_file_append_mode=False)

simulator.set_parallel_thread_number(1)

simulator.set_dynamic_model_database_capacity(10000000)

max_bus = simulator.get_allowed_maximum_bus_number()
info = "The default maximum bus number is: "+str(max_bus)
print(info)
simulator.set_allowed_maximum_bus_number(10000)
max_bus = simulator.get_allowed_maximum_bus_number()
info = "The default maximum bus number is changed to: "+str(max_bus)
print(info)

simulator.load_powerflow_data('IEEE39.raw','PSS/E')
simulator.check_powerflow_data()
simulator.check_network_connectivity()


simulator.build_dynamic_network_Y_matrix()
simulator.save_dynamic_network_Y_matrix('ymatrix_dyn.csv')

simulator.build_network_Z_matrix()
simulator.save_network_Z_matrix('zmatrix_dyn.csv')

nbus = simulator.get_bus_count()
print(nbus)
nline = simulator.get_line_count()
print(nline)
ntrans = simulator.get_transformer_count()
print(ntrans)
nload = simulator.get_load_count()
print(nload)

print("here goes all buses")
buses = simulator.get_all_buses()
for bus in buses:
    busname = simulator.get_bus_data(bus, "string", "bus name")
    basevoltage = simulator.get_bus_data(bus, "double", "base voltage in kV")
    print(bus, busname, basevoltage)

print("here goes all lines")
lines = simulator.get_lines_at_bus(0)
for line in lines:
    status_send = simulator.get_line_data(line, "bool", "sending side breaker status")
    status_recv = simulator.get_line_data(line, "bool", "receiving side breaker status")
    r1 = simulator.get_line_data(line, "double", "r1_pu")
    x1 = simulator.get_line_data(line, "double", "x1_pu")
    g1 = simulator.get_line_data(line, "double", "g1_pu")
    b1 = simulator.get_line_data(line, "double", "b1_pu")
    print(line, status_send, status_recv, r1, x1, g1, b1)

print("here goes all transformer")
transes = simulator.get_transformers_at_bus(0)
for trans in transes:
    status_primary = simulator.get_transformer_data(trans, "bool", "primary", "status")
    status_secondary = simulator.get_transformer_data(trans, "bool", "secondary", "status")
    status_tertiary = simulator.get_transformer_data(trans, "bool", "tertiary", "status")
    gm = simulator.get_transformer_data(trans, "double", "transformer", "gm_pu")
    bm = simulator.get_transformer_data(trans, "double", "transformer", "bm_pu")
    print(trans, status_primary, status_secondary, status_tertiary, gm, bm)

print("here goes solving powerflow")

simulator.set_powerflow_solver_parameter('bool','flat start logic', True)

simulator.solve_powerflow('NR')
simulator.save_powerflow_result('pfresult.csv')
simulator.save_network_matrix('ymatrix.csv')
simulator.save_jacobian_matrix('jacobian.csv')


print("here goes running dynamic simulation")

simulator.set_dynamic_model_database_capacity(1000000)
simulator.load_dynamic_data('IEEE39.dyr','psse')
simulator.check_missing_models()
simulator.check_dynamic_data()
simulator.check_least_dynamic_time_constants()


print("here goes generator dynamic data")
gens = simulator.get_generators_at_bus(0)
for gen in gens:
    gen_model = simulator.get_generator_related_model_name(gen, "GEN")
    avr_model = simulator.get_generator_related_model_name(gen, "avr")
    pss_model = simulator.get_generator_related_model_name(gen, "pss")
    gov_model = simulator.get_generator_related_model_name(gen, "gov")
    pmax = simulator.get_generator_related_model_data(gen, "gov",  'pmax')
    pmin = simulator.get_generator_related_model_data(gen, "gov",  'pmin')
    mbase = simulator.get_generator_data(gen, 'd', "mbase_MVA")
    print(gen, mbase, gen_model, avr_model, pss_model, gov_model, pmax, pmin)
    
    data = simulator.get_generator_related_model_parameter_pair(gen, "gen")
    print(gen_model, data)

simulator.set_dynamic_simulator_parameter('b','bin export logic',False)
simulator.set_dynamic_simulator_parameter('b','csv export logic',True)
simulator.set_dynamic_simulator_parameter('d','ITERATION ACCELERATOR',1.0)
simulator.set_dynamic_simulator_parameter('d','MAX POWER IMBALANCE IN MVA',0.1)
simulator.set_dynamic_simulator_parameter('i','MAX DAE ITERATION',3)
simulator.set_dynamic_simulator_parameter('i','MIN DAE ITERATION',3)
simulator.set_dynamic_simulator_parameter('i','MAX NETWORK ITERATION',100)
simulator.set_dynamic_simulator_parameter('i','MAX UPDATE ITERATION',3)
simulator.set_dynamic_simulator_parameter('b','AUTOMATIC ACCELERATOR TUNE LOGIC',False)
simulator.set_dynamic_simulator_parameter('b','ANGLE STABILITY SURVEILLANCE LOGIC',False)
simulator.set_dynamic_simulator_parameter('d','ANGLE STABILITY THRESHOLD IN DEG',360.0)
simulator.set_dynamic_simulation_time_step(0.01)
simulator.set_dynamic_simulator_output_file('ieee39')

simulator.prepare_meters('all')

simulator.start_dynamic_simulation()
simulator.run_dynamic_simulation_to_time(1.0)

simulator.set_bus_fault(15, 'three phase fault',[0.0, -2e2])
simulator.run_dynamic_simulation_to_time(1.1)
simulator.clear_bus_fault(15, 'three phase fault')
simulator.run_dynamic_simulation_to_time(5.0)

simulator.stop_dynamic_simulation()

time, value, dychannel = POUCH_CSV('ieee39.csv')
from pysteps import STEPS

simulator = STEPS()

max_bus = simulator.get_allowed_maximum_bus_number()
print(max_bus)
simulator.set_allowed_maximum_bus_number(10000)
max_bus = simulator.get_allowed_maximum_bus_number()
print(max_bus)

#simulator.load_powerflow_data('IEEE9_wind.raw','PSS/E')
#simulator.load_dynamic_data('IEEE9_wt3_models.dyr','PSS/E')
simulator.load_powerflow_data('IEEE39_wind.raw','PSS/E')
simulator.load_dynamic_data('IEEE39_wind.dyr','PSS/E')

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

print("here goes generator dynamic data")
gens = simulator.get_generators_at_bus(0)
for gen in gens:
    gen_model = simulator.get_generator_related_model_name(gen, "gen")
    avr_model = simulator.get_generator_related_model_name(gen, "avr")
    pss_model = simulator.get_generator_related_model_name(gen, "pss")
    gov_model = simulator.get_generator_related_model_name(gen, "gov")
    pmax = simulator.get_generator_related_model_data(gen, "gov", 'pmax')
    pmin = simulator.get_generator_related_model_data(gen, "gov", 'pmin')
    mbase = simulator.get_generator_data(gen, 'd', "mbase_MVA")
    print(gen, mbase, gen_model, avr_model, pss_model, gov_model, pmax, pmin)


print("here goes solving powerflow")

simulator.set_powerflow_solver_parameter('bool','flat start logic', True)
simulator.set_powerflow_solver_parameter('double','max active power imbalance in MW', 0.0001)

simulator.solve_powerflow('NR')
simulator.save_powerflow_result('pfresult.csv')
simulator.save_network_matrix('ymatrix.csv')
simulator.save_jacobian_matrix('jacobian.csv')



print("here goes running dynamic simulation")

#simulator.load_dynamic_data('ieee39.dyr','psse')

simulator.set_dynamic_simulator_parameter('i', 'max DAE iteration', 200)
simulator.set_dynamic_simulator_parameter('i', 'max NETWORK iteration', 10)
simulator.set_dynamic_simulator_parameter('d', 'max power imbalance in MVA', 0.0001)
simulator.set_dynamic_simulation_time_step(0.005)
simulator.set_dynamic_simulator_output_file('ieee39_simulator')

simulator.prepare_meters('all')


simulator.start_dynamic_simulation()
simulator.run_dynamic_simulation_to_time(1.0)

#simulator.set_bus_fault(15, 'three phase fault',[0.0, -2e4])
#simulator.run_dynamic_simulation_to_time(1.1)
#simulator.clear_bus_fault(15, 'three phase fault')
#simulator.run_dynamic_simulation_to_time(5.0)

simulator.trip_generator((30, "1"))
simulator.trip_generator((31, "1"))
simulator.trip_generator((32, "1"))
simulator.trip_generator((33, "1"))
#simulator.trip_generator((32, "1"))
#simulator.trip_generator((33, "1"))

simulator.run_dynamic_simulation_to_time(10.0)

simulator.stop_dynamic_simulation()





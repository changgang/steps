import pysteps

max_bus = pysteps.get_allowed_maximum_bus_number()
print(max_bus)
pysteps.set_allowed_maximum_bus_number(10000)
max_bus = pysteps.get_allowed_maximum_bus_number()
print(max_bus)

pysteps.load_powerflow_data('IEEE39.raw','PSS/E')
pysteps.load_dynamic_data('IEEE39_GENROU_SEXS_IEEEG1_PUFLS.dyr','PSS/E')

nbus = pysteps.get_bus_count()
print(nbus)
nline = pysteps.get_line_count()
print(nline)
ntrans = pysteps.get_transformer_count()
print(ntrans)
nload = pysteps.get_load_count()
print(nload)

print("here goes all buses")
buses = pysteps.get_all_buses()
for bus in buses:
    busname = pysteps.get_bus_data(bus, "string", "bus name")
    basevoltage = pysteps.get_bus_data(bus, "double", "base voltage in kV")
    print(bus, busname, basevoltage)

print("here goes all loads")
loads = pysteps.get_loads_at_bus(0)
sump = 0.0
for load in loads:
    status = pysteps.get_load_data(load, "bool", "status")
    p  = pysteps.get_load_data(load, "double", "P_MW")
    print(load, status, p)
    sump = sump+p
print("total load are: "+str(sump)+" MW")

print("here goes all lines")
lines = pysteps.get_lines_at_bus(0)
for line in lines:
    status_send = pysteps.get_line_data(line, "bool", "sending side breaker status")
    status_recv = pysteps.get_line_data(line, "bool", "receiving side breaker status")
    r1 = pysteps.get_line_data(line, "double", "r1_pu")
    x1 = pysteps.get_line_data(line, "double", "x1_pu")
    g1 = pysteps.get_line_data(line, "double", "g1_pu")
    b1 = pysteps.get_line_data(line, "double", "b1_pu")
    print(line, status_send, status_recv, r1, x1, g1, b1)

print("here goes all transformer")
transes = pysteps.get_transformers_at_bus(0)
for trans in transes:
    status_primary = pysteps.get_transformer_data(trans, "bool", "primary", "status")
    status_secondary = pysteps.get_transformer_data(trans, "bool", "secondary", "status")
    status_tertiary = pysteps.get_transformer_data(trans, "bool", "tertiary", "status")
    gm = pysteps.get_transformer_data(trans, "double", "transformer", "gm_pu")
    bm = pysteps.get_transformer_data(trans, "double", "transformer", "bm_pu")
    print(trans, status_primary, status_secondary, status_tertiary, gm, bm)

print("here goes generator dynamic data")
gens = pysteps.get_generators_at_bus(0)
for gen in gens:
    gen_model = pysteps.get_generator_related_model_name(gen, "gen")
    avr_model = pysteps.get_generator_related_model_name(gen, "avr")
    pss_model = pysteps.get_generator_related_model_name(gen, "pss")
    gov_model = pysteps.get_generator_related_model_name(gen, "gov")
    pmax = pysteps.get_generator_related_model_data(gen, "gov", 'd', 'pmax')
    pmin = pysteps.get_generator_related_model_data(gen, "gov", 'd', 'pmin')
    mbase = pysteps.get_generator_data(gen, 'd', "mbase_MVA")
    print(gen, mbase, gen_model, avr_model, pss_model, gov_model, pmax, pmin)


print("here goes solving powerflow")

pysteps.set_powerflow_solver_parameter('bool','flat start logic', False)

pysteps.solve_powerflow('NR')
#pysteps.save_powerflow_result('pfresult.csv')
#pysteps.save_network_matrix('ymatrix.csv')
#pysteps.save_jacobian_matrix('jacobian.csv')



print("here goes running dynamic simulation")


pysteps.set_dynamic_simulator_parameter('i', 'max_DAE_iter', 200)
pysteps.set_dynamic_simulator_parameter('i', 'max_NET_iter', 200)
pysteps.set_dynamic_simulation_time_step(0.001)
pysteps.set_dynamic_simulator_output_file('ieee39_model_PUFLS_trip_generator_39')

pysteps.prepare_meters('bus')

pysteps.start_dynamic_simulation()
pysteps.run_dynamic_simulation_to_time(1.0)

pysteps.trip_generator([39, '1'])
pysteps.run_dynamic_simulation_to_time(20.0)

pysteps.stop_dynamic_simulation()





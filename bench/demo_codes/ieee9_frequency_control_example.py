from stepspy import STEPS # import modules
from stepspy import POUCH_CSV # import modules
import matplotlib.pyplot as plt

simulator = STEPS(is_default=True, log_file="ieee9.log") # create simulator, use default simulator, and redirect logs to file ieee9.log
simulator.set_allowed_maximum_bus_number(10000) # set allowed maximum bus number, no bus with bus number greater than this number is allowed

simulator.load_powerflow_data('../IEEE9.raw','PSS/E') # load powerflow data
simulator.solve_powerflow('NR') # solve with Newton-Raphson method
simulator.show_powerflow_result() # show powerflow result

simulator.set_dynamic_model_database_capacity(1000000) # set dynamic model database capacity, in bytes
simulator.load_dynamic_data('../IEEE9_detail.dyr','PSS/E') # load dynamic data

buses = simulator.get_all_buses() # get all buses
gens = simulator.get_all_generators() # get all generators

for bus in buses: # prepare bus voltage meter for all buses
    simulator.prepare_bus_meter(bus, "voltage in pu")
for bus in buses: # prepare bus frequency meter for all buses
    simulator.prepare_bus_meter(bus, "frequency in Hz")
for gen in gens: # prepare rotor angle meter for all generators
    simulator.prepare_generator_meter(gen, "rotor angle in deg")
for gen in gens: # prepare rotor speed deviation meter for all generators
    simulator.prepare_generator_meter(gen, "rotor speed deviation in pu")
for gen in gens: # prepare rotor speed deviation meter for all generators
    simulator.prepare_generator_meter(gen, "terminal active power in MW")

simulator.set_dynamic_simulator_output_file("ieee9_frq") # set dynamic simulation output file for storing the meters
simulator.set_dynamic_simulation_time_step(0.01) # set dynamic simulation time step in seconds

load = (5,"1")
pload5 = simulator.get_load_data(load, "d", "P0_MW")
print(pload5)

simulator.start_dynamic_simulation() # start dynamic simulation by performing initialization
simulator.run_dynamic_simulation_to_time(1.0) # run dynamic simulation to 1.0s without events
simulator.scale_load(load, 0.2) # scale load at bus 5 by 20%
simulator.run_dynamic_simulation_to_time(30) # run dynamic simulation to 30s

if True:
    gen = (2, "1")
    pmech0 = simulator.get_generator_mechanical_power_reference_in_MW(gen) # get Pref
    simulator.set_generator_mechanical_power_reference_in_MW(gen, pmech0+pload5*0.2*0.5) # get Pref
    simulator.run_dynamic_simulation_to_time(60) # run dynamic simulation to 60s
    simulator.set_generator_mechanical_power_reference_in_MW(gen, pmech0+pload5*0.2) # get Pref
    simulator.run_dynamic_simulation_to_time(120) # run dynamic simulation to 90s

time, values, channels = POUCH_CSV("ieee9_frq.csv")
index_frq = []
for i, channel in enumerate(channels):
    if "FREQUENCY" in channel:
        index_frq.append(i)
for index in index_frq:
    plt.plot(time, values[:,index])
plt.xlim(0, 120.0)
plt.savefig("fig.png",dpi=300)
plt.show()
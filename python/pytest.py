from pysteps import *

libsteps.api_initialize_package()

libsteps.api_load_powerflow_data_from_file("IEEE39.raw","psse")

print(get_all_buses())

libsteps.api_initialize_powerflow_solver()

libsteps.api_set_powerflow_solver_boolean_parameter("Flat start logic", False)
libsteps.api_set_powerflow_solver_float_parameter("max active power imbalance in MW", 0.01)
libsteps.api_set_powerflow_solver_float_parameter("max reactive power imbalance in MVar", 0.01)

libsteps.api_solve_powerflow("NR")

libsteps.api_show_powerflow_result()
libsteps.api_save_powerflow_result('ieee39_pf.txt')

libsteps.api_load_dynamic_data_from_file("ieee39_genrou.dyr", "pss/e")

libsteps.api_set_dynamic_simulation_time_step(0.001)
libsteps.api_set_dynamic_simulator_float_parameter("MAX POWER IMBALANCE IN MVA", 0.01)
libsteps.api_set_dynamic_simulator_integer_parameter("MAX DAE ITERATION", 200)
libsteps.api_set_dynamic_simulator_integer_parameter("MAX NETWORK ITERATION", 1)

libsteps.api_prepare_meters()

libsteps.api_set_dynamic_simulator_output_file("ieee39_simu_with_py")

libsteps.api_start_dynamic_simulation()

libsteps.api_run_simulation_to_time(1.0)

libsteps.api_set_bus_fault(17, "three phase fault", 0.0, -2e4)
libsteps.api_update_with_event()

libsteps.api_run_simulation_to_time(1.1)

libsteps.api_clear_bus_fault(17, "three phase fault")
libsteps.api_trip_line(17, 18, "1")
libsteps.api_update_with_event()

libsteps.api_run_simulation_to_time(5)

libsteps.api_stop_dynamic_simulation()


num = libsteps.api_get_bus_integer_data(2, "bus number")
print(num)
V = libsteps.api_get_bus_float_data(2, "voltage in pu")
print(V)
name = libsteps.api_get_bus_string_data(2, "bus name")
print(name)

num = libsteps.api_get_load_integer_data(5, "1", "bus number")
print(num)
V = libsteps.api_get_load_float_data(5, "1", "nominal constant power active power in mw")
print(V)
name = libsteps.api_get_load_string_data(5, "1", "identifier")
print(name)
status = libsteps.api_get_load_boolean_data(5, "1", "status")
print(status)

libsteps.api_terminate_package();
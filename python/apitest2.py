from pysteps import libsteps as ls


ls.api_initialize_package()

ls.api_load_powerflow_data_from_file('IEEE9.raw','PSSE')
ls.api_show_device_data("ALL")
nbus = ls.api_get_device_count("BUS")
nload=ls.api_get_device_count("LOAD")
print(nbus)
print(nload)

ls.api_initialize_powerflow_solver()

ls.api_set_powerflow_solver_boolean_parameter("Flat start logic", True)
ls.api_set_powerflow_solver_float_parameter("max active power imbalance in MW", 0.01)
ls.api_set_powerflow_solver_float_parameter("max reactive power imbalance in MVar", 0.01)

ls.api_solve_powerflow("NR")


ls.api_is_powerflow_converged()


ls.api_show_powerflow_result()






#coding = utf-8
from stepspy import STEPS
import os
import csv

simulator = STEPS(is_default=False,log_file='dynamic.log')
simulator.load_powerflow_data('ieee39.raw','PSS/E')
simulator.load_mt_hvdc_powerflow_data("2vsc.hdcraw",'PSS/E') #load mt hvdc power flow data       
simulator.set_powerflow_solver_parameter('bool','flat start logic',True) 
simulator.set_powerflow_solver_parameter('d','max active power imbalance in MW',0.001) 
simulator.set_powerflow_solver_parameter('d','max reactive power imbalance in MVar',0.001) 

mthvdcs = simulator.get_all_mt_hvdcs()
for mtdc in mthvdcs:
    print(mtdc)
    nter = simulator.get_mt_hvdc_data(mtdc,"MTDC","DEVICE_NAME","I","NTERMINAL")
    print("DC terminal count:",nter)
    nsta = simulator.get_mt_hvdc_data(mtdc,"MTDC","DEVICE_NAME","I","NSTATION")
    print("DC station count:",nsta)
    nlcc = simulator.get_mt_hvdc_data(mtdc,"MTDC","DEVICE_NAME","I","NLCC")
    print("LCC converter count:",nlcc)
    nvsc = simulator.get_mt_hvdc_data(mtdc,"MTDC","DEVICE_NAME","I","NVSC")
    print("VSC converter count count:",nvsc)
    ndcbus = simulator.get_mt_hvdc_data(mtdc,"MTDC","DEVICE_NAME","I","NDCBUS")
    print("DC bus count:",ndcbus)
    ndcline = simulator.get_mt_hvdc_data(mtdc,"MTDC","DEVICE_NAME","I","NDCLINE")
    print("DC line count:",ndcline)
    udc_base = simulator.get_mt_hvdc_data(mtdc,"MTDC","DEVICE_NAME","F","UDC BASE")
    print("DC base voltage:",udc_base)

    bus = simulator.get_mt_hvdc_data(mtdc,"VSC","vsc1","I","AC BUS")
    print("CONVERTER AC BUS:",bus)

    pac= simulator.get_mt_hvdc_data(mtdc,"VSC","vsc1","F","PAC COMMAND IN MW")
    print("CONVERTER PAC COMMAND:",pac)

    udc = simulator.get_mt_hvdc_data(mtdc,"VSC","vsc2","F","UDC COMMAND IN KV")
    print("UAC COMMAND:",udc)

    status = simulator.get_mt_hvdc_data(mtdc,"VSC","vsc1","B","STATUS")
    print("CONVERTER STATUS:",status)



simulator.solve_powerflow('NR') #PQ
simulator.save_extended_powerflow_result("ieee39_2vsc.csv")

simulator.load_dynamic_data('ieee39.dyr','PSS/E')

out_file='output'  

simulator.set_dynamic_simulator_parameter('i','max_DAE_iter',3)
simulator.set_dynamic_simulator_parameter('i','max_NET_iter',50)
simulator.set_dynamic_simulator_parameter('i','max_UPDATE_iter',50)
simulator.set_dynamic_simulator_parameter('d','ITERATION ACCELERATOR',1.0)
simulator.set_dynamic_simulator_parameter('d','max power imbalance in MVA',0.01) 
simulator.set_dynamic_simulator_parameter('B','CSV EXPORT LOGIC',True)
simulator.set_dynamic_simulator_parameter('B','BIN EXPORT LOGIC',False)
simulator.set_dynamic_simulation_time_step(0.001)

simulator.prepare_meters("MT HVDC")#prepare mt hvdc meters       

simulator.set_dynamic_simulator_output_file(out_file)  
simulator.start_dynamic_simulation()
simulator.run_dynamic_simulation_to_time(0.5)
simulator.set_bus_fault(16,'THREE PHASE FAULT',[0.0,-1e6])
simulator.run_dynamic_simulation_to_time(0.6)
simulator.clear_bus_fault(16,'THREE PHASE FAULT')
simulator.run_dynamic_simulation_to_time(3.0)  
simulator.stop_dynamic_simulation()




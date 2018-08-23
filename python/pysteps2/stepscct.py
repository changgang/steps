from pysteps import STEPS
from math import exp, fabs

class CCT():
    def __init__(self):
        self.__raw_file = None
        self.__dyr_file = None
        self.__simulator = STEPS(True)
        self.__fault_type = None
        self.__fault_device = None
        self.__simulation_time_step = 0.01
        self.__simulation_time_span = 10.0

    def __del__(self):
        self.__simulator.clear_package()
        del self.__simulator
    
    def help(self):
        print("""
            To use the Critical Clearing Time module, do the following:
            cct = CCT() # creater a CCT searcher
            cct.set_powerflow_file("IEEE9.raw") # set powerlfow data
            cct.set_dynamic_file("IEEE9.dyr") # set dynamic data
            cct.set_fault("BUS",4) # set device to search
            cct.search() # go search
            """)
    def set_powerflow_file(self, raw_file):
        self.__raw_file = raw_file
        return
    
    def set_dynamic_file(self, dyr_file):
        self.__dyr_file = dyr_file
        return
    
    def set_fault(self, fault_type, fault_device):
        self.__fault_type = fault_type
        self.__fault_device = fault_device
        return
    
    def set_simulation_time_step(self, step):
        self.__simulation_time_step = step
        return

    def set_simulation_time_span(self, span):
        self.__simulation_time_span = span
        return
    
    def get_powerflow_file(self):
        return self.__raw_file
    
    def get_dynamic_file(self):
        return self.__dyr_file
    
    def get_fault_type(self):
        return self.__fault_type
    
    def get_fault_device(self):
        return self.__fault_device
    
    def get_simulation_time_step(self):
        return self.__simulation_time_step
        
    def set_simulation_time_span(self):
        return self.__simulation_time_span
    
    def search(self):
        ct_low = 0.1
        ct_high = 0.1
        
        while True:
            tend = self.run_simulation(ct_low)
            if fabs(tend-self.__simulation_time_span)<=2.0*self.__simulation_time_step:
                break
            else:
                ct_low *= 0.5
                ct_low = round(ct_low/self.__simulation_time_step)*self.__simulation_time_step
        
        while True:
            tend = self.run_simulation(ct_high)
            if fabs(tend-self.__simulation_time_span)<=2.0*self.__simulation_time_step:
                ct_high *= 2.0
                ct_high = round(ct_high/self.__simulation_time_step)*self.__simulation_time_step
            else:
                break
        
        while True:
            ct = 0.5*(ct_low+ct_high)
            ct = round(ct/self.__simulation_time_step)*self.__simulation_time_step
            tend = self.run_simulation(ct)
            if fabs(tend-self.__simulation_time_span)<=2.0*self.__simulation_time_step:
                ct_low = ct
            else:
                ct_high = ct
            
            if ct_high-ct_low<=self.__simulation_time_step*1.1:
                break
        self.run_simulation(ct_low, export=True) 
        self.run_simulation(ct_high, export=True) 
        return (ct_low, ct_high)
            
            
        
    def run_simulation(self, ct, export=False):
        sim = self.__simulator
        sim.clear_package()
        
        raw_file = self.get_powerflow_file()
        sim.load_powerflow_data(raw_file,"PSSE")
        sim.solve_powerflow("PQ")
        
        dyr_file = self.get_dynamic_file()
        sim.load_dynamic_data(dyr_file,"PSSE")
        
        sim.set_dynamic_simulator_parameter('b','ANGLE STABILITY SURVILLIANCE LOGIC', True)
        sim.set_dynamic_simulator_parameter('d','ANGLE STABILITY THRESHOLD IN DEG', 360.0)
        
        sim.set_dynamic_simulation_time_step(self.__simulation_time_step)
        
        
        ftype = self.get_fault_type()
        fdevice = self.get_fault_device()
        
        if export:
            sim.prepare_meters("ALL")
            filename = "CCT_"+ftype+"_"
            if ftype == "BUS":
                filename += str(fdevice)+"_"
            else:
                for d in fdevice:
                    filename = filename + str(d)+"_"
            filename = filename+str(ct)
            sim.set_dynamic_simulator_output_file(filename)
            
        sim.start_dynamic_simulation()
        sim.run_dynamic_simulation_to_time(0.0)
        
        if ftype == "BUS":
            sim.set_bus_fault(fdevice, "THREE PHASE FAULT", (0.0, -2e4))
        elif ftype == "LINE":
            sim.set_line_fault(fdevice, "THREE PHASE FAULT", 0.0, (0.0, -2e4))
            
        sim.run_dynamic_simulation_to_time(ct)
        
        if ftype == "BUS":
            sim.clear_bus_fault(fdevice, "THREE PHASE FAULT")
        elif ftype == "LINE":
            sim.clear_line_fault(fdevice, "THREE PHASE FAULT", 0.0)
        
        sim.run_dynamic_simulation_to_time(self.__simulation_time_span)
        
        return sim.get_dynamic_simulation_time()
        
        
        
        
        
    
            
            
        

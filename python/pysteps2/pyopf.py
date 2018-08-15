from pysteps import STEPS
from math import exp, fabs

class OPF():
    def __init__(self):
        self.__unit_config = dict() # store configuration of each unit, [pmax, pmin, cost_table_no]
        self.__cost_tables = dict() # cost table, [cost_type, cost_parameter]
        self.__simulator = STEPS() # simulator for solving powerflow

    def __del__(self):
        self.__simulator.clear_package()
        del self.__simulator

    def load_powerflow_case(self, pf_file): # load powerflow and solve it
        self.__simulator.load_powerflow_data(pf_file,"PSSE")
        self.__simulator.solve_powerflow("PQ")
        return
    
    def save_opf_powerflow(self, pf_file): # once OPF is converged, save it to new file
        self.__simulator.save_powerflow_data(pf_file, "PSSE")
        return
    
    def load_cost_table(self, cost_table_file): # load all cost table
        """ format of cost table file:
        table_no, table_type, other_parameters
        table_no is an unique integer
        table_type is a string, currently, only "POLY" and "POLYEXP" are supported
        other_parameters are parameter of the table.
        Demo1:  1,"POLY", a, b, c, d, e, ...
                cost = a+b*P+c*p^2+d*p^3+e*p^4+....
        Demo2:  2, "POLYEXP", a, b, c, d, e, f, g
                cost = a+b*P+c*p^2+d*p^3+e*p^4+f*exp^(g*P)
        """
        with open(cost_table_file,"r") as file:
            all_tables = file.readlines()
            
            self.__cost_tables = dict()
            for table in all_tables:
                table = table.strip()
                data = table.split(",")
                
                if len(data)<3:
                    continue
                
                table_no = int(data[0])
                
                cost_type = data[1]
                cost_type = cost_type.strip('\'')
                cost_type = cost_type.strip('\"')
                cost_type = cost_type.strip('\'')
                cost_type = cost_type.upper()
                if cost_type not in ("POLY", "POLYEXP"):
                    continue
                
                cost_para = []
                for para in data[2:]:
                    cost_para.append(float(para))
                
                self.__cost_tables[table_no]=(cost_type, cost_para)
        return
    
    def get_cost_table(self):
        return self.__cost_tables
    
    def load_unit_config(self, unit_config_file): # load units configuration for calculating cost
        """ format of unit configuration
        bus, id, status, pmax, pmin, table_no
        bus: bus number of generator
        id: generator id, string
        status: 1 if generator accounts for cost, 0 if generator is ignored
        pmax: maximum power
        pmin: minimum power
        table_no: table no of the generator. make sure the table no exists in the cost table
        """
        with open(unit_config_file,"r") as file:
            all_units = file.readlines()
            
            self.__unit_config = dict()
            for unit in all_units:
                unit = unit.strip()
                data = unit.split(",")
                if len(data)<6:
                    continue
                
                unit_no = int(data[0])
                
                unit_id = data[1]
                unit_id = unit_id.strip('\'')
                unit_id = unit_id.strip('\"')
                unit_id = unit_id.strip('\'')

                gen = (unit_no, unit_id)

                if not self.__simulator.is_generator_exist(gen):
                    print("Generator",gen, "does not exist in current database. Unit config is bypassed")
                    continue
                
                if self.__simulator.get_generator_data(gen, 'b', 'STATUS') == False:
                    print("Generator",gen, "is out of service. Unit config is bypassed")
                    continue
                
                status = (int(data[2])==1) 
                if status == False:
                    continue
                
                pmax = float(data[3])                
                pmin = float(data[4])                
                table_no = int(data[5])
                self.__unit_config[gen] = (pmax, pmin, table_no)
        return
    
    def get_unit_config(self):
        return self.__unit_config
    
    def get_cost_of_table(self, table_no, power): # get cost of table with given power
        # if table_no is invalid, return 0.0
        # make sure the unit of power is correct
        cost = 0.0
        if table_no in self.__cost_tables:
            table = self.__cost_tables[table_no]
            cost_type = table[0]
            cost_para = table[1]
            
            if cost_type == "POLY":
                cost += cost_para[0]
                p = power
                for para in cost_para[1:]:
                    cost += (para*p)
                    p *= power
            elif cost_type =="POLYEXP":
                cost += cost_para[0]
                p = power
                for para in cost_para[1:-2]:
                    cost += (para*p)
                    p *= power
                cost += cost_para[-2]*exp(cost_para[-1]*power)
        return cost
    
    def get_current_cost(self): # get cost of the current powerflow
        self.__simulator.solve_powerflow("PQ") # first solve powerflow
        total_cost = 0.0
        for unit in self.__unit_config: # count only units in unit_config
            cost = self.get_cost_of_generator(unit)
            total_cost += cost
        return total_cost

    def get_cost_of_generator(self, unit): # get cost of specific generator
        config = self.__unit_config[unit]
        table_no = config[-1]
        p = self.__simulator.get_generator_data(unit, 'd', 'PGEN_MW') # get P generation of the unit
        return self.get_cost_of_table(table_no, p)
        
    def get_current_generation_of_cost_units(self): # get current power generation of units
        # remember: generator at slack bus is not stored
        P = dict()
        for unit in self.__unit_config:
            bus = unit[0]
            btype = self.__simulator.get_bus_data(bus, 'i', 'bus type')
            if btype==3: # skip slack bus
                continue
            P[unit] = self.__simulator.get_generator_data(unit, 'd', 'PGEN_MW')
        return P

    def get_dcost_over_dp_of_all_unit(self, P0): # get derivative of system cost over power of each unit
        # remember: generator at slack bus is not calculated
        self.apply_generation(P0) # first set up system as initial status
        cost0 = self.get_current_cost() # get initial cost
        delta_p = 0.1 # step
        dcost_dp = dict()
        for (unit, p0) in P0.items():
            self.__simulator.set_generator_data(unit, 'd', 'PGEN_MW', p0+delta_p) # change generator power 
            cost = self.get_current_cost() # caculate new cost
            dcost_dp[unit] = (cost-cost0)/delta_p # get derivative
            self.__simulator.set_generator_data(unit, 'd', 'PGEN_MW', p0) # recover generator power
        return dcost_dp
        
    def apply_generation(self, P):
        for (unit, p) in P.items():
            self.__simulator.set_generator_data(unit, 'd', 'PGEN_MW', p)

    def optimize(self): # this is the core of the class. Optimize it
        cost0 = self.get_current_cost() # get initial cost
        print(cost0)
        while True: # iteration
            P0 = self.get_current_generation_of_cost_units() # save current power generation
            dcost = self.get_dcost_over_dp_of_all_unit(P0) # get derivative
            self.tune_unit_power(dcost, P0) # use the derivative to tune unit power
            cost = self.get_current_cost() # then get the new cost
            print("new cost is", cost)
            if cost0-cost>0.1: # criteria to exit the loop
                cost0 = cost
            else:
                break
            
            
    def tune_unit_power(self, dcost, P0):# key to tune unit power. need further modification
        # first: remove generators at the boundary
        unit_to_delete = []
        for (unit, cost) in dcost.items():
            p = P0[unit]
            pmax = self.__unit_config[unit][0]
            pmin = self.__unit_config[unit][1]
            if (cost<0.0 and p>=pmax) or (cost>0.0 and p<=pmin): # units should not to tune
                unit_to_delete.append(unit)
        
        for unit in unit_to_delete: # remove
            dcost.pop(unit)
        
        if len(dcost.keys())==0: # if no more units to tune, exit
            return
        
        print(dcost)
        unit_candidate = None
        dcost_max = 0.0
        for (unit, cost) in dcost.items(): # the unit with greatest derivative, aka, most sensitive
            if fabs(cost)>dcost_max:
                dcost_max = fabs(cost)
                unit_candidate = unit
        dcost_candidate = dcost[unit_candidate]
        current_p = P0[unit_candidate]
        pmax = self.__unit_config[unit_candidate][0]
        pmin = self.__unit_config[unit_candidate][1]
        if dcost_candidate>0.0 and current_p>pmin: # change unit power
            p = current_p - 1
            if p<pmin:
                p = pmin
            self.__simulator.set_generator_data(unit_candidate, 'd', 'PGEN_MW', p)
            print("generator", unit_candidate, " is changed to ", p,"dcost is ",dcost_candidate)
            return
        if dcost_candidate<0.0 and current_p<pmax:
            p = current_p + 1
            if p>pmax:
                p = pmax
            self.__simulator.set_generator_data(unit_candidate, 'd', 'PGEN_MW', p)
            print("generator", unit_candidate, " is changed to ", p,"dcost is ",dcost_candidate)
            return
            
            

        
            
            
        

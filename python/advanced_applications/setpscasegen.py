import pysteps

class CASE_GEN(self):
    def __init__(self):
        self.__maximum_bus_number = 10000
        self.__base_powerflow_file = None
        self.__powerflow_file_type = "PSSE"
        self.__load_uniform_scale = 0.0
        self.__load_random_scale = 0.0
    
    def set_maixmum_bus_number(self, n):
        self.__maximum_bus_number = n
        
    def get_maximum_bus_number(self):
        return self.__maximum_bus_number
    
    def set_base_powerflow_file(self, file):
        self.__base_powerflow_file = file
        
    def get_base_powerflow_file(self):
        return self.__base_powerflow_file
    
    def set_powerflow_file_type(self, pftype):
        self.__powerflow_file_type = pftype
    
    def get_powerflow_file_type(self):
        return self.__powerflow_file_type
    
    def set_load_uniform_scale(self, scale):
        self.__load_uniform_scale = scale
        
    def get_load_uniform_scale(self):
        return self.__load_uniform_scale
    
    def set_load_randow_scale(self, scale):
        self.__load_randow_scale = scale
        
    def get_load_randow_scale(self):
        return self.__load_randow_scale
        
    def generate(self):
        pass
        
    def generator_load_scale(self):
        pass
    
    def __generate_one_case_with_scale(self, scale):
        pass
        

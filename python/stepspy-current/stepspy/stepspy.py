from .libsteps import pylibsteps
from ctypes import c_char_p
import platform
import os
global STEPS_LIB

class STEPS():
    def info(self):
        version = "0.11.0"
        date = "2019/11/18"
        authors = (("Changgang Li", "lichangang@sdu.edu.cn"),("Yue Wu","sduyuewu2018@163.com"))
        info = 'STEPS '+version+', built on '+date+', by '
        for author in authors:
            info += author[0]+"("+author[1]+"), "
        info = info.rstrip(", ")
        print(info)
        
    def __init__(self, is_default=False, log_file=""):        
        global STEPS_LIB

        if 'STEPS_LIB' not in globals():
            STEPS_LIB = pylibsteps.load_library()

        if is_default==True:
            #STEPS_LIB.api_set_default_toolkit_log_file(log_file)
            self.toolkit_index = 1000000000
        else:
            log_file = self.__get_c_char_p_of_string(log_file)
            self.toolkit_index = STEPS_LIB.api_generate_new_toolkit(log_file)
        
    def __del__(self):
        global STEPS_LIB
        if 'STEPS_LIB' in globals():
            STEPS_LIB.api_delete_toolkit(self.toolkit_index)

    def __extract_single_bus_device_id(self, did):
        bus = did[0]
        ickt = did[1]
        return bus, ickt

    def __extract_double_bus_device_id(self, did):
        ibus = did[0]
        jbus = did[1]
        ickt = did[2]
        return ibus, jbus, ickt
    
    def __extract_triple_bus_device_id(self, did):
        ibus = did[0]
        jbus = did[1]
        kbus = 0
        ickt = did[-1]
        if len(did)>3:
            kbus = did[2]
        return ibus, jbus, kbus, ickt
        
    def __get_c_char_p_of_string(self, data):
        python_version = platform.python_version_tuple()
        python_version = python_version[0]
        if python_version is '3':
            return c_char_p(bytes(data, 'utf-8'))
        elif python_version is '2':
            return c_char_p(bytes(data))
        else:
            return None
            
    def __get_string_from_c_char_p(self, data):
        python_version = platform.python_version_tuple()
        python_version = python_version[0]
        if python_version is '3':
            return str(data, encoding='utf-8')
        elif python_version is '2':
            return str(data)
        else:
            return None

    def set_parallel_thread_number(self, num=1):
        global STEPS_LIB
        STEPS_LIB.api_set_toolkit_parallel_thread_number(num, self.toolkit_index)
        return
        
    def get_parallel_thread_number(self, num=1):
        global STEPS_LIB
        return STEPS_LIB.api_get_toolkit_parallel_thread_number(self.toolkit_index)
        
    def clear_toolkit(self):
        global STEPS_LIB
        STEPS_LIB.api_clear_toolkit(self.toolkit_index)
        return

    def terminate_toolkit(self):
        global STEPS_LIB
        STEPS_LIB = None
        #STEPS_LIB.api_terminate_toolkit(self.toolkit_index)
        return
    
    def get_toolkit_float_data(self, dataname):
        global STEPS_LIB
        dataname = self.__get_c_char_p_of_string(dataname)
        return STEPS_LIB.api_get_toolkit_float_data(dataname, self.toolkit_index)

    def set_toolkit_float_data(self, dataname, value):
        global STEPS_LIB
        dataname = self.__get_c_char_p_of_string(dataname)
        return STEPS_LIB.api_set_toolkit_float_data(dataname, value, self.toolkit_index)
    
    def get_system_base_power_in_MVA(self):
        global STEPS_LIB
        return self.get_toolkit_float_data("SBASE")
    
    def set_system_base_power_in_MVA(self, sbase):
        global STEPS_LIB
        return self.set_toolkit_float_data("SBASE", sbase)
    
    def get_toolkit_string_data(self, dataname):
        global STEPS_LIB
        dataname = self.__get_c_char_p_of_string(dataname)
        return self.__get_source_data(STEPS_LIB.api_get_toolkit_string_data(dataname, self.toolkit_index))

    def set_toolkit_string_data(self, dataname, value):
        global STEPS_LIB
        dataname = self.__get_c_char_p_of_string(dataname)
        value = self.__get_c_char_p_of_string(value)
        return self.__get_source_data(STEPS_LIB.api_set_toolkit_string_data(dataname, value, self.toolkit_index))
    
    def get_case_information(self):
        global STEPS_LIB
        return self.__get_source_data(self.get_toolkit_string_data("CASE INFORMATION"))
    
    def get_case_additional_information(self):
        global STEPS_LIB
        return self.__get_source_data(self.get_toolkit_string_data("CASE ADDITIONAL INFORMATION"))
    
    def set_case_information(self, value):
        global STEPS_LIB
        return self.set_toolkit_string_data("CASE INFORMATION", value)
    
    def set_case_additional_information(self, value):
        global STEPS_LIB
        return self.set_toolkit_string_data("CASE ADDITIONAL INFORMATION", value)    

    def get_toolkit_bool_data(self, dataname):
        global STEPS_LIB
        dataname = self.__get_c_char_p_of_string(dataname)
        return STEPS_LIB.api_get_toolkit_bool_data(dataname, self.toolkit_index)        

    def set_toolkit_bool_data(self, dataname, value):
        global STEPS_LIB
        dataname = self.__get_c_char_p_of_string(dataname)
        return STEPS_LIB.api_set_toolkit_bool_data(dataname, value, self.toolkit_index)
    
    def get_toolkit_detailed_log_logic(self):
        global STEPS_LIB
        return self.get_toolkit_bool_data("DETAILED LOG LOGIC")
    
    def set_toolkit_detailed_log_logic(self, logic):
        global STEPS_LIB
        return self.set_toolkit_bool_data("DETAILED LOG LOGIC", logic)
    

    def get_allowed_maximum_bus_number(self):
        global STEPS_LIB
        return int(STEPS_LIB.api_get_allowed_maximum_bus_number(self.toolkit_index))

    def set_allowed_maximum_bus_number(self, max_bus_number):
        global STEPS_LIB
        STEPS_LIB.api_set_allowed_maximum_bus_number(max_bus_number, self.toolkit_index)

    def load_powerflow_data(self, file, ftype):
        global STEPS_LIB
        file = self.__get_c_char_p_of_string(file)
        ftype = self.__get_c_char_p_of_string(ftype)
        STEPS_LIB.api_load_powerflow_data_from_file(file, ftype, self.toolkit_index)

    def save_powerflow_data(self, file, ftype, export_zero_line=True, export_out_of_service_bus=True):
        print("Warning. API save_powerflow_data() is updated as save_powerflow_data_in_keep_mode(). save_powerflow_data() will be deprecated later.")
        self.save_powerflow_data_in_keep_mode(file, ftype, export_zero_line, export_out_of_service_bus)
        
    def save_powerflow_data_in_keep_mode(self, file, ftype, export_zero_line=True, export_out_of_service_bus=True):
        global STEPS_LIB
        file = self.__get_c_char_p_of_string(file)
        ftype = self.__get_c_char_p_of_string(ftype)
        STEPS_LIB.api_save_powerflow_data_to_file(file, ftype, export_zero_line, export_out_of_service_bus, 0, self.toolkit_index)
        
    def save_powerflow_data_in_ordered_mode(self, file, ftype, export_zero_line=True, export_out_of_service_bus=True):
        global STEPS_LIB
        file = self.__get_c_char_p_of_string(file)
        ftype = self.__get_c_char_p_of_string(ftype)
        STEPS_LIB.api_save_powerflow_data_to_file(file, ftype, export_zero_line, export_out_of_service_bus, 1, self.toolkit_index)
        
    def save_powerflow_data_in_optimized_mode(self, file, ftype, export_zero_line=True, export_out_of_service_bus=True):
        global STEPS_LIB
        file = self.__get_c_char_p_of_string(file)
        ftype = self.__get_c_char_p_of_string(ftype)
        STEPS_LIB.api_save_powerflow_data_to_file(file, ftype, export_zero_line, export_out_of_service_bus, 2, self.toolkit_index)

    def load_powerflow_result(self, file, ftype):
        global STEPS_LIB
        file = self.__get_c_char_p_of_string(file)
        ftype = self.__get_c_char_p_of_string(ftype)
        STEPS_LIB.api_load_powerflow_result_from_file(file, ftype, self.toolkit_index)
        
    def load_dynamic_data(self, file, ftype):
        global STEPS_LIB
        file = self.__get_c_char_p_of_string(file)
        ftype = self.__get_c_char_p_of_string(ftype)
        STEPS_LIB.api_load_dynamic_data_from_file(file, ftype, self.toolkit_index)
        
    def save_dynamic_data(self, file, ftype):
        global STEPS_LIB
        file = self.__get_c_char_p_of_string(file)
        ftype = self.__get_c_char_p_of_string(ftype)
        STEPS_LIB.api_save_dynamic_data_to_file(file, ftype, self.toolkit_index)

    def check_powerflow_data(self):
        global STEPS_LIB
        STEPS_LIB.api_check_powerflow_data(self.toolkit_index)

    def check_database(self):
        print("Warning. API check_database() is obsolete, and will be deprecated later. Use check_powerflow_data() instead.")
        self.check_powerflow_data()
        
    def check_dynamic_data(self):
        global STEPS_LIB
        STEPS_LIB.api_check_dynamic_data(self.toolkit_index)
        
    def check_missing_models(self):
        global STEPS_LIB
        STEPS_LIB.api_check_missing_models(self.toolkit_index)
        
    def check_network_connectivity(self, remove_void_islands=False):
        global STEPS_LIB
        STEPS_LIB.api_check_network_connectivity(remove_void_islands, self.toolkit_index)
        
    def get_bus_capacity(self):
        global STEPS_LIB
        device = self.__get_c_char_p_of_string("BUS")
        return int(STEPS_LIB.api_get_device_capacity(device, self.toolkit_index))
    def get_generator_capacity(self):
        global STEPS_LIB
        device = self.__get_c_char_p_of_string("Generator")
        return int(STEPS_LIB.api_get_device_capacity(device, self.toolkit_index))
    def get_wt_generator_capacity(self):
        global STEPS_LIB
        device = self.__get_c_char_p_of_string("WT Generator")
        return int(STEPS_LIB.api_get_device_capacity(device, self.toolkit_index))
    def get_pv_unit_capacity(self):
        global STEPS_LIB
        device = self.__get_c_char_p_of_string("PV Unit")
        return int(STEPS_LIB.api_get_device_capacity(device, self.toolkit_index))
    def get_load_capacity(self):
        global STEPS_LIB
        device = self.__get_c_char_p_of_string("Load")
        return int(STEPS_LIB.api_get_device_capacity(device, self.toolkit_index))
    def get_fixed_shunt_capacity(self):
        global STEPS_LIB
        device = self.__get_c_char_p_of_string("Fixed Shunt")
        return int(STEPS_LIB.api_get_device_capacity(device, self.toolkit_index))
    def get_line_capacity(self):
        global STEPS_LIB
        device = self.__get_c_char_p_of_string("Line")
        return int(STEPS_LIB.api_get_device_capacity(device, self.toolkit_index))
    def get_transformer_capacity(self):
        global STEPS_LIB
        device = self.__get_c_char_p_of_string("Transformer")
        return int(STEPS_LIB.api_get_device_capacity(device, self.toolkit_index))
    def get_hvdc_capacity(self):
        global STEPS_LIB
        device = self.__get_c_char_p_of_string("HVDC")
        return int(STEPS_LIB.api_get_device_capacity(device, self.toolkit_index))
    def get_equivalent_device_capacity(self):
        global STEPS_LIB
        device = self.__get_c_char_p_of_string("Equivalent Device")
        return int(STEPS_LIB.api_get_device_capacity(device, self.toolkit_index))
    def get_energy_storage_capacity(self):
        global STEPS_LIB
        device = self.__get_c_char_p_of_string("Energy Storage")
        return int(STEPS_LIB.api_get_device_capacity(device, self.toolkit_index))
    def get_area_capacity(self):
        global STEPS_LIB
        return int(STEPS_LIB.api_get_area_capacity(self.toolkit_index))
    def get_zone_capacity(self):
        global STEPS_LIB
        return int(STEPS_LIB.api_get_zone_capacity(self.toolkit_index))
    def get_owner_capacity(self):
        global STEPS_LIB
        return int(STEPS_LIB.api_get_owner_capacity(self.toolkit_index))

    def set_bus_capacity(self, capacity):
        global STEPS_LIB
        device = self.__get_c_char_p_of_string("BUS")
        return STEPS_LIB.api_set_device_capacity("BUS", capacity, self.toolkit_index)
    def set_generator_capacity(self, capacity):
        global STEPS_LIB
        device = self.__get_c_char_p_of_string("Generator")
        return STEPS_LIB.api_set_device_capacity(device, capacity, self.toolkit_index)
    def set_wt_generator_capacity(self, capacity):
        global STEPS_LIB
        device = self.__get_c_char_p_of_string("Wt Generator")
        return STEPS_LIB.api_set_device_capacity(device, capacity, self.toolkit_index)
    def set_pv_unit_capacity(self, capacity):
        global STEPS_LIB
        device = self.__get_c_char_p_of_string("PV Unit")
        return STEPS_LIB.api_set_device_capacity(device, capacity, self.toolkit_index)
    def set_load_capacity(self, capacity):
        global STEPS_LIB
        device = self.__get_c_char_p_of_string("Load")
        return STEPS_LIB.api_set_device_capacity(device, capacity, self.toolkit_index)
    def set_fixed_shunt_capacity(self, capacity):
        global STEPS_LIB
        device = self.__get_c_char_p_of_string("Fixed Shunt")
        return STEPS_LIB.api_set_device_capacity(device, capacity, self.toolkit_index)
    def set_line_capacity(self, capacity):
        global STEPS_LIB
        device = self.__get_c_char_p_of_string("Line")
        return STEPS_LIB.api_set_device_capacity(device, capacity, self.toolkit_index)
    def set_transformer_capacity(self, capacity):
        global STEPS_LIB
        device = self.__get_c_char_p_of_string("Transformer")
        return STEPS_LIB.api_set_device_capacity(device, capacity, self.toolkit_index)
    def set_hvdc_capacity(self, capacity):
        global STEPS_LIB
        device = self.__get_c_char_p_of_string("HVDC")
        return STEPS_LIB.api_set_device_capacity(device, capacity, self.toolkit_index)
    def set_equivalent_device_capacity(self, capacity):
        global STEPS_LIB
        device = self.__get_c_char_p_of_string("Equivalent Device")
        return STEPS_LIB.api_set_device_capacity(device, capacity, self.toolkit_index)
    def set_energy_storage_capacity(self, capacity):
        global STEPS_LIB
        device = self.__get_c_char_p_of_string("Energy Storage")
        return STEPS_LIB.api_set_device_capacity(device, capacity, self.toolkit_index)
    def set_area_capacity(self, capacity):
        global STEPS_LIB
        return STEPS_LIB.api_set_area_capacity(capacity, self.toolkit_index)
    def set_zone_capacity(self, capacity):
        global STEPS_LIB
        return STEPS_LIB.api_set_zone_capacity(capacity, self.toolkit_index)
    def set_owner_capacity(self, capacity):
        global STEPS_LIB
        return STEPS_LIB.api_set_owner_capacity(capacity, self.toolkit_index)


    def add_bus(self, busnumber, busname, basevoltage):
        global STEPS_LIB
        busname = self.__get_c_char_p_of_string(busname)
        STEPS_LIB.api_add_bus(busnumber, busname, basevoltage, self.toolkit_index)
        return

    def add_generator(self, generator):
        global STEPS_LIB
        bus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        STEPS_LIB.api_add_generator(bus, ickt, self.toolkit_index)
        return

    def add_wt_generator(self, generator):
        global STEPS_LIB
        bus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        STEPS_LIB.api_add_wt_generator(bus, ickt, self.toolkit_index)

    def add_pv_unit(self, unit):
        global STEPS_LIB
        bus, ickt = self.__extract_single_bus_device_id(unit)
        ickt = self.__get_c_char_p_of_string(ickt)
        STEPS_LIB.api_add_pv_unit(bus, ickt, self.toolkit_index)
        return

    def add_load(self, load):
        global STEPS_LIB
        bus, ickt = self.__extract_single_bus_device_id(load)
        ickt = self.__get_c_char_p_of_string(ickt)
        STEPS_LIB.api_add_load(bus, ickt, self.toolkit_index)
        return

    def add_fixed_shunt(self, shunt):
        global STEPS_LIB
        bus, ickt = self.__extract_single_bus_device_id(shunt)
        ickt = self.__get_c_char_p_of_string(ickt)
        STEPS_LIB.api_add_fixed_shunt(bus, ickt, self.toolkit_index)
        return

    def add_line(self, line):
        global STEPS_LIB
        ibus, jbus, ickt = self.__extract_double_bus_device_id(line)
        ickt = self.__get_c_char_p_of_string(ickt)
        STEPS_LIB.api_add_line(ibus, jbus, ickt, self.toolkit_index)
        return

    def add_hvdc(self, hvdc):
        global STEPS_LIB
        ibus, jbus, ickt = self.__extract_double_bus_device_id(hvdc)
        ickt = self.__get_c_char_p_of_string(ickt)
        STEPS_LIB.api_add_hvdc(ibus, jbus, ickt, self.toolkit_index)
        return

    def add_transformer(self, transformer):
        global STEPS_LIB
        ibus, jbus, kbus, ickt = self.__extract_triple_bus_device_id(transformer)
        ickt = self.__get_c_char_p_of_string(ickt)
        STEPS_LIB.api_add_transformer(ibus, jbus, kbus, ickt, self.toolkit_index)
        return

    def add_equivalent_device(self, device):
        global STEPS_LIB
        bus, ickt = self.__extract_single_bus_device_id(device)
        ickt = self.__get_c_char_p_of_string(ickt)
        STEPS_LIB.api_add_equivalent_device(bus, ickt, self.toolkit_index)
        return

    def add_energy_storage(self, storage):
        global STEPS_LIB
        bus, ickt = self.__extract_single_bus_device_id(storage)
        ickt = self.__get_c_char_p_of_string(ickt)
        STEPS_LIB.api_add_energy_storage(bus, ickt, self.toolkit_index)
        return

    def add_area(self, areanumber, areaname):
        global STEPS_LIB
        areaname = self.__get_c_char_p_of_string(areaname)
        STEPS_LIB.api_add_area(areanumber, areaname, self.toolkit_index)
        return

    def add_zone(self, zonenumber, zonename):
        global STEPS_LIB
        zonename = self.__get_c_char_p_of_string(zonename)
        STEPS_LIB.api_add_zone(zonenumber, zonename, self.toolkit_index)
        return

    def add_owner(self, ownernumber, ownername):
        global STEPS_LIB
        ownername = self.__get_c_char_p_of_string(ownername)
        STEPS_LIB.api_add_owner(ownernumber, ownername, self.toolkit_index)
        return

    def remove_bus(self, busnumber):
        global STEPS_LIB
        STEPS_LIB.api_remove_bus(busnumber, self.toolkit_index)
        return

    def remove_generator(self, generator):
        global STEPS_LIB
        bus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        STEPS_LIB.api_remove_generator(bus, ickt, self.toolkit_index)
        return

    def remove_wt_generator(self, generator):
        global STEPS_LIB
        bus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        STEPS_LIB.api_remove_wt_generator(bus, ickt, self.toolkit_index)
        return

    def remove_pv_unit(self, unit):
        global STEPS_LIB
        bus, ickt = self.__extract_single_bus_device_id(unit)
        ickt = self.__get_c_char_p_of_string(ickt)
        STEPS_LIB.api_remove_pv_unit(bus, ickt, self.toolkit_index)
        return

    def remove_load(self, load):
        global STEPS_LIB
        bus, ickt = self.__extract_single_bus_device_id(load)
        ickt = self.__get_c_char_p_of_string(ickt)
        STEPS_LIB.api_remove_load(bus, ickt, self.toolkit_index)
        return

    def remove_fixed_shunt(self, shunt):
        global STEPS_LIB
        bus, ickt = self.__extract_single_bus_device_id(shunt)
        ickt = self.__get_c_char_p_of_string(ickt)
        STEPS_LIB.api_remove_fixed_shunt(bus, ickt, self.toolkit_index)
        return

    def remove_line(self, line):
        global STEPS_LIB
        ibus, jbus, ickt = self.__extract_double_bus_device_id(line)
        ickt = self.__get_c_char_p_of_string(ickt)
        STEPS_LIB.api_remove_line(ibus, jbus, ickt, self.toolkit_index)
        return

    def remove_hvdc(self, hvdc):
        global STEPS_LIB
        ibus, jbus, ickt = self.__extract_double_bus_device_id(hvdc)
        ickt = self.__get_c_char_p_of_string(ickt)
        STEPS_LIB.api_remove_hvdc(ibus, jbus, ickt, self.toolkit_index)
        return

    def remove_transformer(self, transformer):
        global STEPS_LIB
        ibus, jbus, kbus, ickt = self.__extract_triple_bus_device_id(transformer)
        ickt = self.__get_c_char_p_of_string(ickt)
        STEPS_LIB.api_remove_transformer(ibus, jbus, kbus, ickt, self.toolkit_index)
        return

    def remove_equivalent_device(self, device):
        global STEPS_LIB
        bus, ickt = self.__extract_single_bus_device_id(device)
        ickt = self.__get_c_char_p_of_string(ickt)
        STEPS_LIB.api_remove_equivalent_device(bus, ickt, self.toolkit_index)
        return

    def remove_energy_storage(self, storage):
        global STEPS_LIB
        bus, ickt = self.__extract_single_bus_device_id(storage)
        ickt = self.__get_c_char_p_of_string(ickt)
        STEPS_LIB.api_remove_energy_storage(bus, ickt, self.toolkit_index)
        return

    def remove_area(self, areanumber):
        global STEPS_LIB
        STEPS_LIB.api_remove_area(areanumber, self.toolkit_index)
        return

    def remove_zone(self, zonenumber):
        global STEPS_LIB
        STEPS_LIB.api_remove_zone(zonenumber, self.toolkit_index)
        return

    def remove_owner(self, ownernumber):
        global STEPS_LIB
        STEPS_LIB.api_remove_owner(ownernumber, self.toolkit_index)
        return
    
    def get_bus_count(self):
        global STEPS_LIB
        device = self.__get_c_char_p_of_string("BUS")
        return STEPS_LIB.api_get_device_count(device, self.toolkit_index)
    def get_generator_count(self):
        global STEPS_LIB
        device = self.__get_c_char_p_of_string("Generator")
        return STEPS_LIB.api_get_device_count(device, self.toolkit_index)
    def get_wt_generator_count(self):
        global STEPS_LIB
        device = self.__get_c_char_p_of_string("WT Generator")
        return STEPS_LIB.api_get_device_count(device, self.toolkit_index)
    def get_pv_unit_count(self):
        global STEPS_LIB
        device = self.__get_c_char_p_of_string("PV Unit")
        return STEPS_LIB.api_get_device_count(device, self.toolkit_index)
    def get_load_count(self):
        global STEPS_LIB
        device = self.__get_c_char_p_of_string("Load")
        return STEPS_LIB.api_get_device_count(device, self.toolkit_index)
    def get_fixed_shunt_count(self):
        global STEPS_LIB
        device = self.__get_c_char_p_of_string("Fixed Shunt")
        return STEPS_LIB.api_get_device_count(device, self.toolkit_index)
    def get_line_count(self):
        global STEPS_LIB
        device = self.__get_c_char_p_of_string("Line")
        return STEPS_LIB.api_get_device_count(device, self.toolkit_index)
    def get_transformer_count(self):
        global STEPS_LIB
        device = self.__get_c_char_p_of_string("Transformer")
        return STEPS_LIB.api_get_device_count(device, self.toolkit_index)
    def get_hvdc_count(self):
        global STEPS_LIB
        device = self.__get_c_char_p_of_string("HVDC")
        return STEPS_LIB.api_get_device_count(device, self.toolkit_index)
    def get_equivalent_device_count(self):
        global STEPS_LIB
        device = self.__get_c_char_p_of_string("Equivalent Device")
        return STEPS_LIB.api_get_device_count(device, self.toolkit_index)
    def get_energy_storage_count(self):
        global STEPS_LIB
        device = self.__get_c_char_p_of_string("Energy Storage")
        return STEPS_LIB.api_get_device_count(device, self.toolkit_index)
    def get_area_count(self):
        global STEPS_LIB
        return STEPS_LIB.api_get_area_count(self.toolkit_index)
    def get_zone_count(self):
        global STEPS_LIB
        return STEPS_LIB.api_get_zone_count(self.toolkit_index)
    def get_owner_count(self):
        global STEPS_LIB
        return STEPS_LIB.api_get_owner_count(self.toolkit_index)
    def get_in_service_bus_count(self):
        global STEPS_LIB
        STEPS_LIB.api_get_in_service_bus_count(self.toolkit_index)
        return
    def update_overshadowed_buses(self):
        global STEPS_LIB
        return STEPS_LIB.api_update_overshadowed_buses(self.toolkit_index)
    def set_all_buses_un_overshadowed(self):
        global STEPS_LIB
        STEPS_LIB.api_set_all_buses_un_overshadowed(self.toolkit_index)
        return
    def get_overshadowed_bus_count(self):
        global STEPS_LIB
        return STEPS_LIB.api_get_overshadowed_bus_count(self.toolkit_index)

    def is_bus_exist(self, bus):
        global STEPS_LIB
        return STEPS_LIB.api_is_bus_exist(bus, self.toolkit_index)

    def is_generator_exist(self, generator):
        global STEPS_LIB
        bus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        return STEPS_LIB.api_is_generator_exist(bus, ickt, self.toolkit_index)

    def is_wt_generator_exist(self, generator):
        global STEPS_LIB
        bus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        return STEPS_LIB.api_is_wt_generator_exist(bus, ickt, self.toolkit_index)

    def is_pv_unit_exist(self, pv_unit):
        global STEPS_LIB
        bus, ickt = self.__extract_single_bus_device_id(pv_unit)
        ickt = self.__get_c_char_p_of_string(ickt)
        return STEPS_LIB.api_is_pv_unit_exist(bus, ickt, self.toolkit_index)

    def is_load_exist(self, load):
        global STEPS_LIB
        bus, ickt = self.__extract_single_bus_device_id(load)
        ickt = self.__get_c_char_p_of_string(ickt)
        return STEPS_LIB.api_is_load_exist(bus, ickt, self.toolkit_index)

    def is_fixed_shunt_exist(self, shunt):
        global STEPS_LIB
        bus, ickt = self.__extract_single_bus_device_id(shunt)
        ickt = self.__get_c_char_p_of_string(ickt)
        return STEPS_LIB.api_is_fixed_shunt_exist(bus, ickt, self.toolkit_index)

    def is_line_exist(self, line):
        global STEPS_LIB
        ibus, jbus, ickt = self.__extract_double_bus_device_id(line)
        ickt = self.__get_c_char_p_of_string(ickt)
        return STEPS_LIB.api_is_line_exist(ibus, jbus, ickt, self.toolkit_index)

    def is_transformer_exist(self, transformer):
        global STEPS_LIB
        ibus, jbus, kbus, ickt = self.__extract_triple_bus_device_id(transformer)
        ickt = self.__get_c_char_p_of_string(ickt)
        return STEPS_LIB.api_is_transformer_exist(ibus, jbus, kbus, ickt, self.toolkit_index)

    def is_hvdc_exist(self, hvdc):
        global STEPS_LIB
        ibus, jbus, ickt = self.__extract_double_bus_device_id(hvdc)
        ickt = self.__get_c_char_p_of_string(ickt)
        return STEPS_LIB.api_is_hvdc_exist(ibus, jbus, ickt, self.toolkit_index)

    def is_equivalent_device_exist(self, equivalent_device):
        global STEPS_LIB
        bus, ickt = self.__extract_single_bus_device_id(equivalent_device)
        ickt = self.__get_c_char_p_of_string(ickt)
        return STEPS_LIB.api_is_equivalent_device_exist(bus, ickt, self.toolkit_index)

    def is_energy_storage_exist(self, energy_storage):
        global STEPS_LIB
        bus, ickt = self.__extract_single_bus_device_id(energy_storage)
        ickt = self.__get_c_char_p_of_string(ickt)
        return STEPS_LIB.api_is_energy_storage_exist(bus, ickt, self.toolkit_index)
    
    def bus_name2number(self, name):
        global STEPS_LIB
        name = self.__get_c_char_p_of_string(name)
        return int(STEPS_LIB.api_bus_name2bus_number(name, self.toolkit_index))

    def bus_number2name(self, bus):
        global STEPS_LIB
        return self.__get_string_from_c_char_p(STEPS_LIB.api_bus_number2bus_name(bus, self.toolkit_index))

    def get_all_buses(self):
        global STEPS_LIB
        STEPS_LIB.api_initialize_all_bus_search(self.toolkit_index)
        buses = []
        while True:
            bus = STEPS_LIB.api_get_current_bus_number(self.toolkit_index)
            if bus==0:
                break
            buses.append(int(bus))
            STEPS_LIB.api_goto_next_bus(self.toolkit_index)
        return tuple(buses)

    def __get_buses_with_constraints_deprecated(self, constraints):
        print("Warning. __get_buses_with_constraints_deprecated(constraints) will be removed later.")
        buses = []
        if len(constraints)<7:
            return buses
        vbase_min = constraints[0]
        vbase_max = constraints[1]
        v_min = constraints[2]
        v_max = constraints[3]
        area = constraints[4]
        zone = constraints[5]
        owner = constraints[6]  
        return self.get_buses_with_constraints(vbase_min, vbase_max, v_min, v_max, area, zone, owner)
        
    def get_buses_with_constraints(self, vbase_min=0.0, vbase_max=10000000.0, v_min=0.0, v_max=10000000.0, area=0, zone=0, owner=0):
        if isinstance(vbase_min, list) or isinstance(vbase_min, tuple):
            print("Warning. get_buses_with_constraints(constraints) is obsolete and has been updated to get_buses_with_constraints(vbase_min=0.0, vbase_max=10000000.0, v_min=0.0, v_max=10000000.0, area=0, zone=0, owner=0). Use __get_buses_with_constraints_deprecated(constraints) for compatibility.")
            constraints = tuple(vbase_min)
            return self.__get_buses_with_constraints_deprecated(constraints)
        else:
            global STEPS_LIB
            buses = []        
            STEPS_LIB.api_initialize_bus_search(vbase_min, vbase_max, v_min, v_max, area, zone, owner, self.toolkit_index)
            while True:
                bus = STEPS_LIB.api_get_current_bus_number(self.toolkit_index)
                if bus==0:
                    break
                buses.append(int(bus))
                STEPS_LIB.api_goto_next_bus(self.toolkit_index)
            return tuple(buses)
                
    def get_all_generators(self):
        return self.get_generators_at_bus(0)
        
    def get_generators_at_bus(self, bus):
        global STEPS_LIB
        device = "GENERATOR"
        device = self.__get_c_char_p_of_string(device)
        blank = self.__get_c_char_p_of_string("")
        STEPS_LIB.api_initialize_device_search(device, bus, self.toolkit_index)
        generators = []
        while True:
            bus = STEPS_LIB.api_get_current_device_bus_number(device, blank, self.toolkit_index)
            if bus==0:
                break
            id = STEPS_LIB.api_get_current_device_identifier(device, self.toolkit_index)
            id = self.__get_string_from_c_char_p(id)
            generators.append((int(bus), id))
            STEPS_LIB.api_goto_next_device(device, self.toolkit_index)
        return tuple(generators)
        
    def get_all_wt_generators(self):
        return self.get_wt_generators_at_bus(0)
        
    def get_wt_generators_at_bus(self, bus):
        global STEPS_LIB
        device = "WT GENERATOR"
        device = self.__get_c_char_p_of_string(device)
        blank = self.__get_c_char_p_of_string("")
        STEPS_LIB.api_initialize_device_search(device, bus, self.toolkit_index)
        wt_generators = []
        while True:
            bus = STEPS_LIB.api_get_current_device_bus_number(device, blank, self.toolkit_index)
            if bus==0:
                break
            id = STEPS_LIB.api_get_current_device_identifier(device, self.toolkit_index)
            id = self.__get_string_from_c_char_p(id)
            wt_generators.append((int(bus), id))
            STEPS_LIB.api_goto_next_device(device, self.toolkit_index)
        return tuple(wt_generators)
        
    def get_all_pv_units(self):
        return self.get_pv_units_at_bus(0)
        
    def get_pv_units_at_bus(self, bus):
        global STEPS_LIB
        device = "PV UNIT"
        device = self.__get_c_char_p_of_string(device)
        blank = self.__get_c_char_p_of_string("")
        STEPS_LIB.api_initialize_device_search(device, bus, self.toolkit_index)
        pv_units = []
        while True:
            bus = STEPS_LIB.api_get_current_device_bus_number(device, blank, self.toolkit_index)
            if bus==0:
                break
            id = STEPS_LIB.api_get_current_device_identifier(device, self.toolkit_index)
            id = self.__get_string_from_c_char_p(id)
            pv_units.append((int(bus), id))
            STEPS_LIB.api_goto_next_device(device, self.toolkit_index)
        return tuple(pv_units)
        
    def get_all_energy_storages(self):
        return self.get_energy_storages_at_bus(0)
        
    def get_energy_storages_at_bus(self, bus):
        global STEPS_LIB
        device = "ENERGY STORAGE"
        device = self.__get_c_char_p_of_string(device)
        blank = self.__get_c_char_p_of_string("")
        STEPS_LIB.api_initialize_device_search(device, bus, self.toolkit_index)
        energy_storages = []
        while True:
            bus = STEPS_LIB.api_get_current_device_bus_number(device, blank, self.toolkit_index)
            if bus==0:
                break
            id = STEPS_LIB.api_get_current_device_identifier(device, self.toolkit_index)
            id = self.__get_string_from_c_char_p(id)
            energy_storages.append((int(bus), id))
            STEPS_LIB.api_goto_next_device(device, self.toolkit_index)
        return tuple(energy_storages)
        
    def get_all_loads(self):
        return self.get_loads_at_bus(0)
        
    def get_loads_at_bus(self, bus):
        global STEPS_LIB
        device = "LOAD"
        device = self.__get_c_char_p_of_string(device)
        blank = self.__get_c_char_p_of_string("")
        STEPS_LIB.api_initialize_device_search(device, bus, self.toolkit_index)
        loads = []
        while True:
            bus = STEPS_LIB.api_get_current_device_bus_number(device, blank, self.toolkit_index)
            if bus==0:
                break
            id = STEPS_LIB.api_get_current_device_identifier(device, self.toolkit_index)
            id = self.__get_string_from_c_char_p(id)
            loads.append((int(bus), id))
            STEPS_LIB.api_goto_next_device(device, self.toolkit_index)
        return tuple(loads)
        
    def get_all_fixed_shunts(self):
        return self.get_fixed_shunts_at_bus(0)
        
    def get_fixed_shunts_at_bus(self, bus):
        global STEPS_LIB
        device = "FIXED SHUNT"
        device = self.__get_c_char_p_of_string(device)
        blank = self.__get_c_char_p_of_string("")
        STEPS_LIB.api_initialize_device_search(device, bus, self.toolkit_index)
        fixed_shunts = []
        while True:
            bus = STEPS_LIB.api_get_current_device_bus_number(device, blank, self.toolkit_index)
            if bus==0:
                break
            id = STEPS_LIB.api_get_current_device_identifier(device, self.toolkit_index)
            id = self.__get_string_from_c_char_p(id)
            fixed_shunts.append((int(bus), id))
            STEPS_LIB.api_goto_next_device(device, self.toolkit_index)
        return tuple(fixed_shunts)
                
    def get_all_equivalent_devices(self):
        return self.get_equivalent_devices_at_bus(0)
        
    def get_equivalent_devices_at_bus(self, bus):
        global STEPS_LIB
        device = "EQUIVALENT DEVICE"
        device = self.__get_c_char_p_of_string(device)
        blank = self.__get_c_char_p_of_string("")
        STEPS_LIB.api_initialize_device_search(device, bus, self.toolkit_index)
        equivalent_devices = []
        while True:
            bus = STEPS_LIB.api_get_current_device_bus_number(device, blank, self.toolkit_index)
            if bus==0:
                break
            id = STEPS_LIB.api_get_current_device_identifier(device, self.toolkit_index)
            id = self.__get_string_from_c_char_p(id)
            equivalent_devices.append((int(bus), id))
            STEPS_LIB.api_goto_next_device(device, self.toolkit_index)
        return tuple(equivalent_devices)

    def get_all_lines(self):
        return self.get_lines_at_bus(0)
        
    def get_lines_at_bus(self, bus):
        global STEPS_LIB
        device = "LINE"
        device = self.__get_c_char_p_of_string(device)
        send_side = self.__get_c_char_p_of_string("SEND")
        recv_side = self.__get_c_char_p_of_string("RECEIVE")
        STEPS_LIB.api_initialize_device_search(device, bus, self.toolkit_index)
        lines = []
        while True:
            ibus = STEPS_LIB.api_get_current_device_bus_number(device, send_side, self.toolkit_index)
            if ibus==0:
                break
            jbus = STEPS_LIB.api_get_current_device_bus_number(device, recv_side, self.toolkit_index)
            id = STEPS_LIB.api_get_current_device_identifier(device, self.toolkit_index)
            id = self.__get_string_from_c_char_p(id)
            lines.append((int(ibus), int(jbus), id))
            STEPS_LIB.api_goto_next_device(device, self.toolkit_index)
        return tuple(lines)

    def get_all_transformers(self):
        return self.get_transformers_at_bus(0)
        
    def get_transformers_at_bus(self, bus):
        global STEPS_LIB
        device = "TRANSFORMER"
        device = self.__get_c_char_p_of_string(device)
        pri_side = self.__get_c_char_p_of_string("PRIMARY")
        sec_side = self.__get_c_char_p_of_string("SECONDARY")
        ter_side = self.__get_c_char_p_of_string("TERTIARY")
        STEPS_LIB.api_initialize_device_search(device, bus, self.toolkit_index)
        transformers = []
        while True:
            ibus = STEPS_LIB.api_get_current_device_bus_number(device, pri_side, self.toolkit_index)
            if ibus==0:
                break
            jbus = STEPS_LIB.api_get_current_device_bus_number(device, sec_side, self.toolkit_index)
            kbus = STEPS_LIB.api_get_current_device_bus_number(device, ter_side, self.toolkit_index)
            id = STEPS_LIB.api_get_current_device_identifier(device, self.toolkit_index)
            id = self.__get_string_from_c_char_p(id)
            transformers.append((int(ibus), int(jbus), int(kbus), id))
            STEPS_LIB.api_goto_next_device(device, self.toolkit_index)
        return tuple(transformers)

    def get_all_hvdcs(self):
        return self.get_hvdcs_at_bus(0)
        
    def get_hvdcs_at_bus(self, bus):
        global STEPS_LIB
        device = "HVDC"
        device = self.__get_c_char_p_of_string(device)
        rec_side = self.__get_c_char_p_of_string("RECTIFIER")
        inv_side = self.__get_c_char_p_of_string("INVERTER")
        STEPS_LIB.api_initialize_device_search(device, bus, self.toolkit_index)
        hvdcs = []
        while True:
            ibus = STEPS_LIB.api_get_current_device_bus_number(device, rec_side, self.toolkit_index)
            if ibus==0:
                break
            jbus = STEPS_LIB.api_get_current_device_bus_number(device, inv_side, self.toolkit_index)
            id = STEPS_LIB.api_get_current_device_identifier(device, self.toolkit_index)
            id = self.__get_string_from_c_char_p(id)
            hvdcs.append((int(ibus), int(jbus), id))
            STEPS_LIB.api_goto_next_device(device, self.toolkit_index)
        return tuple(hvdcs)

    def get_generators_with_constraints(self, area=0, zone=0):
        all_gens = self.get_generators_at_bus(0)
        if area==0 and zone==0:
            return all_gens
        else:
            gens = []
            if area==0 and zone!=0:
                for gen in all_gens:
                    bus = gen[0]
                    gen_zone = self.get_bus_data(bus, "i", "zone")
                    if gen_zone==zone:
                        gens.append(gen)
            elif area!=0 and zone==0:
                for gen in all_gens:
                    bus = gen[0]
                    gen_area = self.get_bus_data(bus, "i", "area")
                    if gen_area==area:
                        gens.append(gen)
            else:
                for gen in all_gens:
                    bus = gen[0]
                    gen_area = self.get_bus_data(bus, "i", "area")
                    gen_zone = self.get_bus_data(bus, "i", "zone")
                    if gen_area==area and gen_zone==zone:
                        gens.append(gen)
        return tuple(gens)
        
    def get_wt_generators_with_constraints(self, area=0, zone=0):
        all_gens = self.get_wt_generators_at_bus(0)
        if area==0 and zone==0:
            return all_gens
        else:
            gens = []
            if area==0 and zone!=0:
                for gen in all_gens:
                    bus = gen[0]
                    gen_zone = self.get_bus_data(bus, "i", "zone")
                    if gen_zone==zone:
                        gens.append(gen)
            elif area!=0 and zone==0:
                for gen in all_gens:
                    bus = gen[0]
                    gen_area = self.get_bus_data(bus, "i", "area")
                    if gen_area==area:
                        gens.append(gen)
            else:
                for gen in all_gens:
                    bus = gen[0]
                    gen_area = self.get_bus_data(bus, "i", "area")
                    gen_zone = self.get_bus_data(bus, "i", "zone")
                    if gen_area==area and gen_zone==zone:
                        gens.append(gen)
        return tuple(gens)

    def get_pv_units_with_constraints(self, area=0, zone=0):
        all_pvs = self.get_pv_units_at_bus(0)
        if area==0 and zone==0:
            return all_pvs
        else:
            pvs = []
            if area==0 and zone!=0:
                for pv in all_pvs:
                    bus = pv[0]
                    pv_zone = self.get_bus_data(bus, "i", "zone")
                    if pv_zone==zone:
                        pvs.append(pv)
            elif area!=0 and zone==0:
                for pv in all_pvs:
                    bus = pv[0]
                    pv_area = self.get_bus_data(bus, "i", "area")
                    if pv_area==area:
                        pvs.append(pv)
            else:
                for pv in all_pvs:
                    bus = pv[0]
                    pv_area = self.get_bus_data(bus, "i", "area")
                    pv_zone = self.get_bus_data(bus, "i", "zone")
                    if pv_area==area and pv_zone==zone:
                        pvs.append(pv)
        return tuple(pvs)

    def get_loads_with_constraints(self, area=0, zone=0):
        all_loads = self.get_loads_at_bus(0)
        if area==0 and zone==0:
            return all_loads
        else:
            loads = []
            if area==0 and zone!=0:
                for load in all_loads:
                    bus = load[0]
                    load_zone = self.get_bus_data(bus, "i", "zone")
                    if load_zone==zone:
                        loads.append(load)
            elif area!=0 and zone==0:
                for load in all_loads:
                    bus = load[0]
                    load_area = self.get_bus_data(bus, "i", "area")
                    if load_area==area:
                        loads.append(load)
            else:
                for load in all_loads:
                    bus = load[0]
                    load_area = self.get_bus_data(bus, "i", "area")
                    load_zone = self.get_bus_data(bus, "i", "zone")
                    if load_area==area and load_zone==zone:
                        loads.append(load)
        return tuple(loads)
        
    def get_areas(self):
        return self.get_all_areas()
        
    def get_all_areas(self):
        global STEPS_LIB
        STEPS_LIB.api_initialize_area_search(self.toolkit_index)
        areas = []
        while True:
            area = STEPS_LIB.api_get_current_area_number(self.toolkit_index)
            if area==0:
                break
            areas.append(int(area))
            STEPS_LIB.api_goto_next_area(self.toolkit_index)
        return tuple(areas)

    def get_zones(self):
        return self.get_all_zones()
        
    def get_all_zones(self):
        global STEPS_LIB
        STEPS_LIB.api_initialize_zone_search(self.toolkit_index)
        zones = []
        while True:
            zone = STEPS_LIB.api_get_current_zone_number(self.toolkit_index)
            if zone==0:
                break
            zones.append(int(zone))
            STEPS_LIB.api_goto_next_zone(self.toolkit_index)
        return tuple(zones)

    def get_owners(self):
        return self.get_all_owners()
        
    def get_all_owners(self):
        global STEPS_LIB
        STEPS_LIB.api_initialize_owner_search(self.toolkit_index)
        owners = []
        while True:
            owner = STEPS_LIB.api_get_current_owner_number(self.toolkit_index)
            if owner==0:
                break
            owners.append(int(owner))
            STEPS_LIB.api_goto_next_owner(self.toolkit_index)
        return tuple(owners)

    def get_bus_data(self, bus, par_type, par_name):
        global STEPS_LIB
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return None
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return int(STEPS_LIB.api_get_bus_integer_data(bus, par_name, self.toolkit_index))
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return STEPS_LIB.api_get_bus_float_data(bus, par_name, self.toolkit_index)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return STEPS_LIB.api_get_bus_boolean_data(bus, par_name, self.toolkit_index)
        if par_type in ['S', 'STRING']:
            return self.__get_string_from_c_char_p(STEPS_LIB.api_get_bus_string_data(bus, par_name, self.toolkit_index))
        return None

    def __get_source_data(self, source, par_type, par_name):
        global STEPS_LIB
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return None
        bus, ickt = self.__extract_single_bus_device_id(source)
        ickt = self.__get_c_char_p_of_string(ickt)
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return int(STEPS_LIB.api_get_source_integer_data(bus, ickt, par_name, self.toolkit_index))
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return STEPS_LIB.api_get_source_float_data(bus, ickt, par_name, self.toolkit_index)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return STEPS_LIB.api_get_source_boolean_data(bus, ickt, par_name, self.toolkit_index)
        if par_type in ['S', 'STRING']:
            return self.__get_string_from_c_char_p(STEPS_LIB.api_get_source_string_data(bus, ickt, par_name, self.toolkit_index))
        return None

    def get_generator_data(self, generator, par_type, par_name):
        global STEPS_LIB
        return self.__get_source_data(generator, par_type, par_name)

    def get_wt_generator_data(self, wt_generator, par_type, par_name):
        global STEPS_LIB
        return self.__get_source_data(wt_generator, par_type, par_name)

    def get_pv_unit_data(self, pv_unit, par_type, par_name):
        global STEPS_LIB
        return self.__get_source_data(pv_unit, par_type, par_name)

    def get_energy_storage_data(self, energy_storage, par_type, par_name):
        global STEPS_LIB
        return self.__get_source_data(energy_storage, par_type, par_name)

    def get_load_data(self, load, par_type, par_name):
        global STEPS_LIB
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return None
        bus, ickt = self.__extract_single_bus_device_id(load)
        ickt = self.__get_c_char_p_of_string(ickt)
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return int(STEPS_LIB.api_get_load_integer_data(bus, ickt, par_name, self.toolkit_index))
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return STEPS_LIB.api_get_load_float_data(bus, ickt, par_name, self.toolkit_index)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return STEPS_LIB.api_get_load_boolean_data(bus, ickt, par_name, self.toolkit_index)
        if par_type in ['S', 'STRING']:
            return self.__get_string_from_c_char_p(STEPS_LIB.api_get_load_string_data(bus, ickt, par_name, self.toolkit_index))
        return None

    def get_fixed_shunt_data(self, fixed_shunt, par_type, par_name):
        global STEPS_LIB
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return None
        bus, ickt = self.__extract_single_bus_device_id(fixed_shunt)
        ickt = self.__get_c_char_p_of_string(ickt)
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return int(STEPS_LIB.api_get_fixed_shunt_integer_data(bus, ickt, par_name, self.toolkit_index))
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return STEPS_LIB.api_get_fixed_shunt_float_data(bus, ickt, par_name, self.toolkit_index)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return STEPS_LIB.api_get_fixed_shunt_boolean_data(bus, ickt, par_name, self.toolkit_index)
        if par_type in ['S', 'STRING']:
            return self.__get_string_from_c_char_p(STEPS_LIB.api_get_fixed_shunt_string_data(bus, ickt, par_name, self.toolkit_index))
        return None

    def get_equivalent_device_data(self, equivalent_device, par_type, par_name):
        global STEPS_LIB
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return None
        bus, ickt = self.__extract_single_bus_device_id(equivalent_device)
        ickt = self.__get_c_char_p_of_string(ickt)
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return int(STEPS_LIB.api_get_equivalent_device_integer_data(bus, ickt, par_name, self.toolkit_index))
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return STEPS_LIB.api_get_equivalent_device_float_data(bus, ickt, par_name, self.toolkit_index)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return STEPS_LIB.api_get_equivalent_device_boolean_data(bus, ickt, par_name, self.toolkit_index)
        if par_type in ['S', 'STRING']:
            return self.__get_string_from_c_char_p(STEPS_LIB.api_get_equivalent_device_string_data(bus, ickt, par_name, self.toolkit_index))
        return None

    def get_line_data(self, line, par_type, par_name):
        global STEPS_LIB
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return None
        ibus, jbus, ickt = self.__extract_double_bus_device_id(line)
        ickt = self.__get_c_char_p_of_string(ickt)
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return int(STEPS_LIB.api_get_line_integer_data(ibus, jbus, ickt, par_name, self.toolkit_index))
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return STEPS_LIB.api_get_line_float_data(ibus, jbus, ickt, par_name, self.toolkit_index)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return STEPS_LIB.api_get_line_boolean_data(ibus, jbus, ickt, par_name, self.toolkit_index)
        if par_type in ['S', 'STRING']:
            return self.__get_string_from_c_char_p(STEPS_LIB.api_get_line_string_data(ibus, jbus, ickt, par_name, self.toolkit_index))
        return None

    def get_transformer_data(self, transformer, par_type, side, par_name):
        global STEPS_LIB
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return None
        side = side.upper()
        if side not in ['PRIMARY', 'SECONDARY', 'TERTIARY', 'TRANSFORMER']:
            return None
        ibus, jbus, kbus, ickt = self.__extract_triple_bus_device_id(transformer)
        ickt = self.__get_c_char_p_of_string(ickt)
        side = self.__get_c_char_p_of_string(side)
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return int(STEPS_LIB.api_get_transformer_integer_data(ibus, jbus, kbus, ickt, side, par_name, self.toolkit_index))
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return STEPS_LIB.api_get_transformer_float_data(ibus, jbus, kbus, ickt, side, par_name, self.toolkit_index)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return STEPS_LIB.api_get_transformer_boolean_data(ibus, jbus, kbus, ickt, side, par_name, self.toolkit_index)
        if par_type in ['S', 'STRING']:
            return self.__get_string_from_c_char_p(STEPS_LIB.api_get_transformer_string_data(ibus, jbus, kbus, ickt, side, par_name, self.toolkit_index))
        return None

    def get_hvdc_data(self, hvdc, par_type, side, par_name):
        global STEPS_LIB
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return None
        side = side.upper()
        if side not in ['RECTIFIER', 'INVERTER', 'HVDC']:
            return None
        ibus, jbus, ickt = self.__extract_double_bus_device_id(hvdc)
        ickt = self.__get_c_char_p_of_string(ickt)
        side = self.__get_c_char_p_of_string(side)
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return int(STEPS_LIB.api_get_hvdc_integer_data(ibus, jbus, ickt, side, par_name, self.toolkit_index))
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return STEPS_LIB.api_get_hvdc_float_data(ibus, jbus, ickt, side, par_name, self.toolkit_index)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return STEPS_LIB.api_get_hvdc_boolean_data(ibus, jbus, ickt, side, par_name, self.toolkit_index)
        if par_type in ['S', 'STRING']:
            return self.__get_string_from_c_char_p(STEPS_LIB.api_get_hvdc_string_data(ibus, jbus, ickt, side, par_name, self.toolkit_index))
        return None

    def get_area_data(self, area, par_type, par_name):
        global STEPS_LIB
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return None
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return int(STEPS_LIB.api_get_area_integer_data(area, par_name, self.toolkit_index))
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return STEPS_LIB.api_get_area_float_data(area, par_name, self.toolkit_index)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return STEPS_LIB.api_get_area_boolean_data(area, par_name, self.toolkit_index)
        if par_type in ['S', 'STRING']:
            return self.__get_string_from_c_char_p(STEPS_LIB.api_get_area_string_data(area, par_name, self.toolkit_index))
        return None

    def get_zone_data(self, zone, par_type, par_name):
        global STEPS_LIB
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return None
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return int(STEPS_LIB.api_get_zone_integer_data(zone, par_name, self.toolkit_index))
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return STEPS_LIB.api_get_zone_float_data(zone, par_name, self.toolkit_index)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return STEPS_LIB.api_get_zone_boolean_data(zone, par_name, self.toolkit_index)
        if par_type in ['S', 'STRING']:
            return self.__get_string_from_c_char_p(STEPS_LIB.api_get_zone_string_data(zone, par_name, self.toolkit_index))
        return None
    
    def get_owner_data(self, owner, par_type, par_name):
        global STEPS_LIB
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return None
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return int(STEPS_LIB.api_get_owner_integer_data(owner, par_name, self.toolkit_index))
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return STEPS_LIB.api_get_owner_float_data(owner, par_name, self.toolkit_index)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return STEPS_LIB.api_get_owner_boolean_data(owner, par_name, self.toolkit_index)
        if par_type in ['S', 'STRING']:
            return self.__get_string_from_c_char_p(STEPS_LIB.api_get_owner_string_data(owner, par_name, self.toolkit_index))
        return None

        
    def set_bus_data(self, bus, par_type, par_name, value):
        global STEPS_LIB
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            STEPS_LIB.api_set_bus_integer_data(bus, par_name, value, self.toolkit_index)
            return
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return STEPS_LIB.api_set_bus_float_data(bus, par_name, value, self.toolkit_index)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return STEPS_LIB.api_set_bus_boolean_data(bus, par_name, value, self.toolkit_index)
        if par_type in ['S', 'STRING']:
            value = self.__get_c_char_p_of_string(value)
            return STEPS_LIB.api_set_bus_string_data(bus, par_name, value, self.toolkit_index)
        return

    def __set_source_data(self, source, par_type, par_name, value):
        global STEPS_LIB
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return        
        bus, ickt = self.__extract_single_bus_device_id(source)
        ickt = self.__get_c_char_p_of_string(ickt)
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return STEPS_LIB.api_set_source_integer_data(bus, ickt, par_name, value, self.toolkit_index)
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return STEPS_LIB.api_set_source_float_data(bus, ickt, par_name, value, self.toolkit_index)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return STEPS_LIB.api_set_source_boolean_data(bus, ickt, par_name, value, self.toolkit_index)
        if par_type in ['S', 'STRING']:
            value = self.__get_c_char_p_of_string(value)
            return STEPS_LIB.api_set_source_string_data(bus, ickt, par_name, value, self.toolkit_index)
        return

    def set_generator_data(self, generator, par_type, par_name, value):
        global STEPS_LIB
        return self.__set_source_data(generator, par_type, par_name, value)

    def set_wt_generator_data(self, wt_generator, par_type, par_name, value):
        global STEPS_LIB
        return self.__set_source_data(wt_generator, par_type, par_name, value)

    def set_pv_unit_data(self, pv_unit, par_type, par_name, value):
        global STEPS_LIB
        return self.__set_source_data(pv_unit, par_type, par_name, value)

    def set_energy_storage_data(self, energy_storage, par_type, par_name, value):
        global STEPS_LIB
        return self.__set_source_data(energy_storage, par_type, par_name, value)

    def set_load_data(self, load, par_type, par_name, value):
        global STEPS_LIB
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return
        bus, ickt = self.__extract_single_bus_device_id(load)
        ickt = self.__get_c_char_p_of_string(ickt)
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return STEPS_LIB.api_set_load_integer_data(bus, ickt, par_name, value, self.toolkit_index)
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return STEPS_LIB.api_set_load_float_data(bus, ickt, par_name, value, self.toolkit_index)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return STEPS_LIB.api_set_load_boolean_data(bus, ickt, par_name, value, self.toolkit_index)
        if par_type in ['S', 'STRING']:
            value = self.__get_c_char_p_of_string(value)
            return STEPS_LIB.api_set_load_string_data(bus, ickt, par_name, value, self.toolkit_index)
        return

    def set_fixed_shunt_data(self, fixed_shunt, par_type, par_name, value):
        global STEPS_LIB
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return
        bus, ickt = self.__extract_single_bus_device_id(fixed_shunt)
        ickt = self.__get_c_char_p_of_string(ickt)
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return STEPS_LIB.api_set_fixed_shunt_integer_data(bus, ickt, par_name, value, self.toolkit_index)
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return STEPS_LIB.api_set_fixed_shunt_float_data(bus, ickt, par_name, value, self.toolkit_index)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return STEPS_LIB.api_set_fixed_shunt_boolean_data(bus, ickt, par_name, value, self.toolkit_index)
        if par_type in ['S', 'STRING']:
            value = self.__get_c_char_p_of_string(value)
            return STEPS_LIB.api_set_fixed_shunt_string_data(bus, ickt, par_name, value, self.toolkit_index)
        return

    def set_equivalent_device_data(self, equivalent_device, par_type, par_name, value):
        global STEPS_LIB
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return
        bus, ickt = self.__extract_single_bus_device_id(equivalent_device)
        ickt = self.__get_c_char_p_of_string(ickt)
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return STEPS_LIB.api_set_equivalent_device_integer_data(bus, ickt, par_name, value, self.toolkit_index)
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return STEPS_LIB.api_set_equivalent_device_float_data(bus, ickt, par_name, value, self.toolkit_index)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return STEPS_LIB.api_set_equivalent_device_boolean_data(bus, ickt, par_name, value, self.toolkit_index)
        if par_type in ['S', 'STRING']:
            value = self.__get_c_char_p_of_string(value)
            return STEPS_LIB.api_set_equivalent_device_string_data(bus, ickt, par_name, value, self.toolkit_index)
        return

    def set_line_data(self, line, par_type, par_name, value):
        global STEPS_LIB
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return
        ibus, jbus, ickt = self.__extract_double_bus_device_id(line)
        ickt = self.__get_c_char_p_of_string(ickt)
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return STEPS_LIB.api_set_line_integer_data(ibus, jbus, ickt, par_name, value, self.toolkit_index)
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return STEPS_LIB.api_set_line_float_data(ibus, jbus, ickt, par_name, value, self.toolkit_index)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return STEPS_LIB.api_set_line_boolean_data(ibus, jbus, ickt, par_name, value, self.toolkit_index)
        if par_type in ['S', 'STRING']:
            value = self.__get_c_char_p_of_string(value)
            return STEPS_LIB.api_set_line_string_data(ibus, jbus, ickt, par_name, value, self.toolkit_index)
        return

    def set_transformer_data(self, transformer, par_type, side, par_name, value):
        global STEPS_LIB
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return
        ibus, jbus, kbus, ickt = self.__extract_triple_bus_device_id(transformer)
        ickt = self.__get_c_char_p_of_string(ickt)
        side = self.__get_c_char_p_of_string(side)
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return STEPS_LIB.api_set_transformer_integer_data(ibus, jbus, kbus, ickt, side, par_name, value, self.toolkit_index)
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return STEPS_LIB.api_set_transformer_float_data(ibus, jbus, kbus, ickt, side, par_name, value, self.toolkit_index)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return STEPS_LIB.api_set_transformer_boolean_data(ibus, jbus, kbus, ickt, side, par_name, value, self.toolkit_index)
        if par_type in ['S', 'STRING']:
            value = self.__get_c_char_p_of_string(value)
            return STEPS_LIB.api_set_transformer_string_data(ibus, jbus, kbus, ickt, side, par_name, value, self.toolkit_index)
        return

    def set_hvdc_data(self, hvdc, par_type, side, par_name, value):
        global STEPS_LIB
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return
        ibus, jbus, ickt = self.__extract_double_bus_device_id(hvdc)
        ickt = self.__get_c_char_p_of_string(ickt)
        side = self.__get_c_char_p_of_string(side)
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return STEPS_LIB.api_set_hvdc_integer_data(ibus, jbus, ickt, side, par_name, value, self.toolkit_index)
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return STEPS_LIB.api_set_hvdc_float_data(ibus, jbus, ickt, side, par_name, value, self.toolkit_index)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return STEPS_LIB.api_set_hvdc_boolean_data(ibus, jbus, ickt, side, par_name, value, self.toolkit_index)
        if par_type in ['S', 'STRING']:
            value = self.__get_c_char_p_of_string(value)
            return STEPS_LIB.api_set_hvdc_string_data(ibus, jbus, ickt, side, par_name, value, self.toolkit_index)
        return

    def set_area_data(self, area, par_type, par_name, value):
        global STEPS_LIB
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return STEPS_LIB.api_set_area_integer_data(area, par_name, value, self.toolkit_index)
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return STEPS_LIB.api_set_area_float_data(area, par_name, value, self.toolkit_index)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return STEPS_LIB.api_set_area_boolean_data(area, par_name, value, self.toolkit_index)
        if par_type in ['S', 'STRING']:
            value = self.__get_c_char_p_of_string(value)
            return STEPS_LIB.api_set_area_string_data(area, par_name, value, self.toolkit_index)
        return

    def set_zone_data(self, zone, par_type, par_name, value):
        global STEPS_LIB
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return STEPS_LIB.api_set_zone_integer_data(zone, par_name, value, self.toolkit_index)
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return STEPS_LIB.api_set_zone_float_data(zone, par_name, value, self.toolkit_index)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return STEPS_LIB.api_set_zone_boolean_data(zone, par_name, value, self.toolkit_index)
        if par_type in ['S', 'STRING']:
            value = self.__get_c_char_p_of_string(value)
            return STEPS_LIB.api_set_zone_string_data(zone, par_name, value, self.toolkit_index)
        return

    def set_owner_data(self, owner, par_type, par_name, value):
        global STEPS_LIB
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return STEPS_LIB.api_set_owner_integer_data(owner, par_name, value, self.toolkit_index)
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return STEPS_LIB.api_set_owner_float_data(owner, par_name, value, self.toolkit_index)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return STEPS_LIB.api_set_owner_boolean_data(owner, par_name, value, self.toolkit_index)
        if par_type in ['S', 'STRING']:
            value = self.__get_c_char_p_of_string(value)
            return STEPS_LIB.api_set_owner_string_data(owner, par_name, value, self.toolkit_index)
        return

    def set_dynamic_model(self, data, file_type):
        global STEPS_LIB
        data = self.__get_c_char_p_of_string(data)
        file_type = self.__get_c_char_p_of_string(file_type)        
        STEPS_LIB.api_set_dynamic_model(data, file_type, self.toolkit_index)
        return
    
    def get_generator_related_model_name(self, generator, model_type):
        global STEPS_LIB
        ibus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        model_type = self.__get_c_char_p_of_string(model_type)
        model_name = self.__get_string_from_c_char_p(STEPS_LIB.api_get_generator_related_model_name(ibus, ickt, model_type, self.toolkit_index))
        if model_name=="":
            model_name = None
        return model_name

    def get_generator_related_model_data(self, generator, model_type, par_name):
        global STEPS_LIB
        ibus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        model_type = self.__get_c_char_p_of_string(model_type)
        par_name = self.__get_c_char_p_of_string(par_name)
        return STEPS_LIB.api_get_generator_related_model_float_parameter(ibus, ickt, model_type, par_name, self.toolkit_index)
        
    def set_generator_related_model_data(self, generator, model_type, par_name, value):
        global STEPS_LIB
        ibus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        model_type = self.__get_c_char_p_of_string(model_type)
        par_name = self.__get_c_char_p_of_string(par_name)
        return STEPS_LIB.api_set_generator_related_model_float_parameter(ibus, ickt, model_type, par_name, value, self.toolkit_index)
    
    def get_generator_related_model_parameter_pair(self, generator, model_type):
        global STEPS_LIB
        ibus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        model_type = self.__get_c_char_p_of_string(model_type)
        parameters = []
        n = STEPS_LIB.api_get_generator_related_model_float_parameter_count(ibus, ickt, model_type, self.toolkit_index)
        for parameter_index in range(n):
            par_name = STEPS_LIB.api_get_generator_related_model_float_parameter_name(ibus, ickt, model_type, parameter_index, self.toolkit_index)
            par_value = STEPS_LIB.api_get_generator_related_model_float_parameter(ibus, ickt, model_type, par_name, self.toolkit_index);
            par_name = self.__get_string_from_c_char_p(par_name)
            parameters.append((par_name, par_value))
        return tuple(parameters)
    
    def get_wt_generator_related_model_name(self, generator, model_type):
        global STEPS_LIB
        ibus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        model_type = self.__get_c_char_p_of_string(model_type)
        model_name = self.__get_string_from_c_char_p(STEPS_LIB.api_get_wt_generator_related_model_name(ibus, ickt, model_type, self.toolkit_index))
        if model_name=="":
            model_name = None
        return model_name

    def get_wt_generator_related_model_data(self, generator, model_type, par_name):
        global STEPS_LIB
        ibus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        model_type = self.__get_c_char_p_of_string(model_type)
        par_name = self.__get_c_char_p_of_string(par_name)
        return STEPS_LIB.api_get_wt_generator_related_model_float_parameter(ibus, ickt, model_type, par_name, self.toolkit_index)
        
    def set_wt_generator_related_model_data(self, generator, model_type, par_name, value):
        global STEPS_LIB
        ibus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        model_type = self.__get_c_char_p_of_string(model_type)
        par_name = self.__get_c_char_p_of_string(par_name)
        return STEPS_LIB.api_set_wt_generator_related_model_float_parameter(ibus, ickt, model_type, par_name, value, self.toolkit_index)
        
    def get_wt_generator_related_model_parameter_pair(self, generator, model_type):
        global STEPS_LIB
        ibus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        model_type = self.__get_c_char_p_of_string(model_type)
        parameters = []
        n = STEPS_LIB.api_get_wt_generator_related_model_float_parameter_count(ibus, ickt, model_type, self.toolkit_index)
        for parameter_index in range(n):
            par_name = STEPS_LIB.api_get_wt_generator_related_model_float_parameter_name(ibus, ickt, model_type, parameter_index, self.toolkit_index)
            par_value = STEPS_LIB.api_get_wt_generator_related_model_float_parameter(ibus, ickt, model_type, par_name, self.toolkit_index);
            par_name = self.__get_string_from_c_char_p(par_name)
            parameters.append((par_name, par_value))
        return tuple(parameters)
        
    def get_pv_unit_related_model_name(self, pv_unit, model_type):
        global STEPS_LIB
        ibus, ickt = self.__extract_single_bus_device_id(pv_unit)
        ickt = self.__get_c_char_p_of_string(ickt)
        model_type = self.__get_c_char_p_of_string(model_type)
        model_name = self.__get_string_from_c_char_p(STEPS_LIB.api_get_pv_unit_related_model_name(ibus, ickt, model_type, self.toolkit_index))
        if model_name=="":
            model_name = None
        return model_name

    def get_pv_unit_related_model_data(self, pv_unit, model_type, par_name):
        global STEPS_LIB
        ibus, ickt = self.__extract_single_bus_device_id(pv_unit)
        ickt = self.__get_c_char_p_of_string(ickt)
        model_type = self.__get_c_char_p_of_string(model_type)
        par_name = self.__get_c_char_p_of_string(par_name)
        return STEPS_LIB.api_get_pv_unit_related_model_float_parameter(ibus, ickt, model_type, par_name, self.toolkit_index)
        
    def set_pv_unit_related_model_data(self, pv_unit, model_type, par_name, value):
        global STEPS_LIB
        ibus, ickt = self.__extract_single_bus_device_id(pv_unit)
        ickt = self.__get_c_char_p_of_string(ickt)
        model_type = self.__get_c_char_p_of_string(model_type)
        par_name = self.__get_c_char_p_of_string(par_name)
        return STEPS_LIB.api_set_pv_unit_related_model_float_parameter(ibus, ickt, model_type, par_name, value, self.toolkit_index)
        
    def get_pv_unit_related_model_parameter_pair(self, pv_unit, model_type):
        global STEPS_LIB
        ibus, ickt = self.__extract_single_bus_device_id(pv_unit)
        ickt = self.__get_c_char_p_of_string(ickt)
        model_type = self.__get_c_char_p_of_string(model_type)
        parameters = []
        n = STEPS_LIB.api_get_pv_unit_related_model_float_parameter_count(ibus, ickt, model_type, self.toolkit_index)
        for parameter_index in range(n):
            par_name = STEPS_LIB.api_get_pv_unit_related_model_float_parameter_name(ibus, ickt, model_type, parameter_index, self.toolkit_index)
            par_value = STEPS_LIB.api_get_pv_unit_related_model_float_parameter(ibus, ickt, model_type, par_name, self.toolkit_index);
            par_name = self.__get_string_from_c_char_p(par_name)
            parameters.append((par_name, par_value))
        return tuple(parameters)
         
    def get_load_related_model_name(self, load, model_type):
        global STEPS_LIB
        ibus, ickt = self.__extract_single_bus_device_id(load)
        ickt = self.__get_c_char_p_of_string(ickt)
        model_type = self.__get_c_char_p_of_string(model_type)
        model_name = self.__get_string_from_c_char_p(STEPS_LIB.api_get_load_related_model_name(ibus, ickt, model_type, self.toolkit_index))
        if model_name=="":
            model_name = None
        return model_name

    def get_load_related_model_data(self, load, model_type, par_name):
        global STEPS_LIB
        ibus, ickt = self.__extract_single_bus_device_id(load)
        ickt = self.__get_c_char_p_of_string(ickt)
        model_type = self.__get_c_char_p_of_string(model_type)
        par_name = self.__get_c_char_p_of_string(par_name)
        return STEPS_LIB.api_get_load_related_model_float_parameter(ibus, ickt, model_type, par_name, self.toolkit_index)
        
    def set_load_related_model_data(self, load, model_type, par_name, value):
        global STEPS_LIB
        ibus, ickt = self.__extract_single_bus_device_id(load)
        ickt = self.__get_c_char_p_of_string(ickt)
        model_type = self.__get_c_char_p_of_string(model_type)
        par_name = self.__get_c_char_p_of_string(par_name)
        return STEPS_LIB.api_set_load_related_model_float_parameter(ibus, ickt, model_type, par_name, value, self.toolkit_index)
        
    def get_load_related_model_parameter_pair(self, load, model_type):
        global STEPS_LIB
        ibus, ickt = self.__extract_single_bus_device_id(load)
        ickt = self.__get_c_char_p_of_string(ickt)
        model_type = self.__get_c_char_p_of_string(model_type)
        parameters = []
        n = STEPS_LIB.api_get_load_related_model_float_parameter_count(ibus, ickt, model_type, self.toolkit_index)
        for parameter_index in range(n):
            par_name = STEPS_LIB.api_get_load_related_model_float_parameter_name(ibus, ickt, model_type, parameter_index, self.toolkit_index)
            par_value = STEPS_LIB.api_get_load_related_model_float_parameter(ibus, ickt, model_type, par_name, self.toolkit_index);
            par_name = self.__get_string_from_c_char_p(par_name)
            parameters.append((par_name, par_value))
        return tuple(parameters)
        
    def get_line_related_model_name(self, line, model_type):
        global STEPS_LIB
        ibus, jbus, ickt = self.__extract_double_bus_device_id(line)
        ickt = self.__get_c_char_p_of_string(ickt)
        model_type = self.__get_c_char_p_of_string(model_type)
        model_name = self.__get_string_from_c_char_p(STEPS_LIB.api_get_line_related_model_name(ibus, jbus, ickt, model_type, self.toolkit_index))
        if model_name=="":
            model_name = None
        return model_name

    def get_line_related_model_data(self, line, model_type, par_name):
        global STEPS_LIB
        ibus, jbus, ickt = self.__extract_double_bus_device_id(line)
        ickt = self.__get_c_char_p_of_string(ickt)
        model_type = self.__get_c_char_p_of_string(model_type)
        par_name = self.__get_c_char_p_of_string(par_name)
        return STEPS_LIB.api_get_line_related_model_float_parameter(ibus, jbus, ickt, model_type, par_name, self.toolkit_index)
        
    def set_line_related_model_data(self, line, model_type, par_name, value):
        global STEPS_LIB
        ibus, jbus, ickt = self.__extract_double_bus_device_id(line)
        ickt = self.__get_c_char_p_of_string(ickt)
        model_type = self.__get_c_char_p_of_string(model_type)
        par_name = self.__get_c_char_p_of_string(par_name)
        return STEPS_LIB.api_set_line_related_model_float_parameter(ibus, jbus, ickt, model_type, par_name, value, self.toolkit_index)
        
    def get_line_related_model_parameter_pair(self, line, model_type):
        global STEPS_LIB
        ibus, jbus, ickt = self.__extract_double_bus_device_id(line)
        ickt = self.__get_c_char_p_of_string(ickt)
        model_type = self.__get_c_char_p_of_string(model_type)
        parameters = []
        n = STEPS_LIB.api_get_line_related_model_float_parameter_count(ibus, jbus, ickt, model_type, self.toolkit_index)
        for parameter_index in range(n):
            par_name = STEPS_LIB.api_get_line_related_model_float_parameter_name(ibus, jbus, ickt, model_type, parameter_index, self.toolkit_index)
            par_value = STEPS_LIB.api_get_line_related_model_float_parameter(ibus, jbus, ickt, model_type, par_name, self.toolkit_index);
            par_name = self.__get_string_from_c_char_p(par_name)
            parameters.append((par_name, par_value))
        return tuple(parameters)
        
    def get_hvdc_related_model_name(self, hvdc, model_type):
        global STEPS_LIB
        ibus, jbus, ickt = self.__extract_double_bus_device_id(hvdc)
        ickt = self.__get_c_char_p_of_string(ickt)
        model_type = self.__get_c_char_p_of_string(model_type)
        model_name = self.__get_string_from_c_char_p(STEPS_LIB.api_get_hvdc_related_model_name(ibus, jbus, ickt, model_type, self.toolkit_index))
        if model_name=="":
            model_name = None
        return model_name

    def get_hvdc_related_model_data(self, hvdc, model_type, par_name):
        global STEPS_LIB
        ibus, jbus, ickt = self.__extract_double_bus_device_id(hvdc)
        ickt = self.__get_c_char_p_of_string(ickt)
        model_type = self.__get_c_char_p_of_string(model_type)
        par_name = self.__get_c_char_p_of_string(par_name)
        return STEPS_LIB.api_get_hvdc_related_model_float_parameter(ibus, jbus, ickt, model_type, par_name, self.toolkit_index)
        
    def set_hvdc_related_model_data(self, hvdc, model_type, par_name, value):
        global STEPS_LIB
        ibus, jbus, ickt = self.__extract_double_bus_device_id(hvdc)
        ickt = self.__get_c_char_p_of_string(ickt)
        model_type = self.__get_c_char_p_of_string(model_type)
        par_name = self.__get_c_char_p_of_string(par_name)
        return STEPS_LIB.api_set_hvdc_related_model_float_parameter(ibus, jbus, ickt, model_type, par_name, value, self.toolkit_index)
        
    def get_hvdc_related_model_parameter_pair(self, hvdc, model_type):
        global STEPS_LIB
        ibus, jbus, ickt = self.__extract_double_bus_device_id(hvdc)
        ickt = self.__get_c_char_p_of_string(ickt)
        model_type = self.__get_c_char_p_of_string(model_type)
        parameters = []
        n = STEPS_LIB.api_get_hvdc_related_model_float_parameter_count(ibus, jbus, ickt, model_type, self.toolkit_index)
        for parameter_index in range(n):
            par_name = STEPS_LIB.api_get_hvdc_related_model_float_parameter_name(ibus, jbus, ickt, model_type, parameter_index, self.toolkit_index)
            par_value = STEPS_LIB.api_get_hvdc_related_model_float_parameter(ibus, jbus, ickt, model_type, par_name, self.toolkit_index);
            par_name = self.__get_string_from_c_char_p(par_name)
            parameters.append((par_name, par_value))
        return tuple(parameters)
        
    def get_powerflow_solver_parameter(self, par_type, par_name):
        global STEPS_LIB
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN']:
            return None
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return int(STEPS_LIB.api_get_powerflow_solver_integer_parameter(par_name, self.toolkit_index))
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return STEPS_LIB.api_get_powerflow_solver_float_parameter(par_name, self.toolkit_index)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return STEPS_LIB.api_get_powerflow_solver_boolean_parameter(par_name, self.toolkit_index)
        return None

    def set_powerflow_solver_parameter(self, par_type, par_name, value):
        global STEPS_LIB
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN']:
            return
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return STEPS_LIB.api_set_powerflow_solver_integer_parameter(par_name, value, self.toolkit_index)
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return STEPS_LIB.api_set_powerflow_solver_float_parameter(par_name, value, self.toolkit_index)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return STEPS_LIB.api_set_powerflow_solver_boolean_parameter(par_name, value, self.toolkit_index)
        return
    
    def show_powerflow_solver_configuration(self):
        global STEPS_LIB
        STEPS_LIB.api_show_powerflow_solver_configuration(self.toolkit_index)
        return
        
    def solve_powerflow(self, method):
        global STEPS_LIB
        method = self.__get_c_char_p_of_string(method)
        STEPS_LIB.api_solve_powerflow(method, self.toolkit_index)
        return

    def is_powerflow_converged(self):
        global STEPS_LIB
        return STEPS_LIB.api_is_powerflow_converged(self.toolkit_index)
    
    def get_powerflow_loss(self):
        global STEPS_LIB
        p_gen = 0.0
        gens = self.get_generators_at_bus(0)
        for gen in gens:
            p_gen += self.get_generator_data(gen, 'd', 'Pgen_MW')
            
        gens = self.get_wt_generators_at_bus(0)
        for gen in gens:
            p_gen += self.get_wt_generator_data(gen, 'd', 'Pgen_MW')
            
        pvs = self.get_pv_units_at_bus(0)
        for pv in pvs:
            p_gen += self.get_pv_unit_data(pv, 'd', 'Pgen_MW')
            
        eses = self.get_energy_storages_at_bus(0)
        for es in eses:
            p_gen += self.get_energy_storage_data(es, 'd', 'Pgen_MW')

        p_load = 0.0
        loads = self.get_loads_at_bus(0)
        for load in loads:
            p_load += self.get_load_data(load, 'd', 'P_MW')
            
        return p_gen-p_load
        
            
    def show_powerflow_result(self):
        global STEPS_LIB
        STEPS_LIB.api_show_powerflow_result(self.toolkit_index)
        return
        
    def save_powerflow_result(self, file):
        global STEPS_LIB
        file = self.__get_c_char_p_of_string(file)
        STEPS_LIB.api_save_powerflow_result(file, self.toolkit_index)
        return
    
    def save_extended_powerflow_result(self, file):
        global STEPS_LIB
        file = self.__get_c_char_p_of_string(file)
        STEPS_LIB.api_save_extended_powerflow_result(file, self.toolkit_index)
        return
        
    def save_jacobian_matrix(self, file):
        global STEPS_LIB
        file = self.__get_c_char_p_of_string(file)
        STEPS_LIB.api_save_jacobian_matrix(file, self.toolkit_index)
        return

    def build_network_Y_matrix(self):
        global STEPS_LIB
        STEPS_LIB.api_build_network_Y_matrix(self.toolkit_index)
        return

    def build_decoupled_network_B_matrix(self):
        global STEPS_LIB
        STEPS_LIB.api_build_decoupled_network_B_matrix(self.toolkit_index)
        return

    def build_dc_network_B_matrix(self):
        global STEPS_LIB
        STEPS_LIB.api_build_dc_network_B_matrix(self.toolkit_index)
        return

    def build_dynamic_network_Y_matrix(self):
        global STEPS_LIB
        STEPS_LIB.api_build_dynamic_network_Y_matrix(self.toolkit_index)
        return

    def build_network_Z_matrix(self):
        global STEPS_LIB
        STEPS_LIB.api_build_network_Z_matrix(self.toolkit_index)
        return
        
    def save_network_Y_matrix(self, file):
        global STEPS_LIB
        file = self.__get_c_char_p_of_string(file)
        STEPS_LIB.api_save_network_Y_matrix(file, self.toolkit_index)
        return
        
    def save_decoupled_network_B_matrix(self, file):
        global STEPS_LIB
        file = self.__get_c_char_p_of_string(file)
        STEPS_LIB.api_save_decoupled_network_B_matrix(file, self.toolkit_index)
        return
        
    def save_dc_network_B_matrix(self, file):
        global STEPS_LIB
        file = self.__get_c_char_p_of_string(file)
        STEPS_LIB.api_save_dc_network_B_matrix(file, self.toolkit_index)
        return
        
    def save_dynamic_network_Y_matrix(self, file):
        global STEPS_LIB
        file = self.__get_c_char_p_of_string(file)
        STEPS_LIB.api_save_dynamic_network_Y_matrix(file, self.toolkit_index)
        return
        
    def save_network_Z_matrix(self, file):
        global STEPS_LIB
        file = self.__get_c_char_p_of_string(file)
        STEPS_LIB.api_save_network_Z_matrix(file, self.toolkit_index)
        return
        
        
    def get_dynamic_simulator_parameter(self, par_type, par_name):
        global STEPS_LIB
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return None
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return int(STEPS_LIB.api_get_dynamic_simulator_integer_parameter(par_name, self.toolkit_index))
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return STEPS_LIB.api_get_dynamic_simulator_float_parameter(par_name, self.toolkit_index)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return STEPS_LIB.api_get_dynamic_simulator_boolean_parameter(par_name, self.toolkit_index)
        if par_type in ['S', 'STRING']:
            return self.__get_string_from_c_char_p(STEPS_LIB.api_get_dynamic_simulator_string_parameter(par_name, self.toolkit_index))
        return None

    def set_dynamic_simulator_parameter(self, par_type, par_name, value):
        global STEPS_LIB
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return None
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return STEPS_LIB.api_set_dynamic_simulator_integer_parameter(par_name, value, self.toolkit_index)
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return STEPS_LIB.api_set_dynamic_simulator_float_parameter(par_name, value, self.toolkit_index)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return STEPS_LIB.api_set_dynamic_simulator_boolean_parameter(par_name, value, self.toolkit_index)
        if par_type in ['S', 'STRING']:
            value = self.__get_c_char_p_of_string(value)
            return STEPS_LIB.api_set_dynamic_simulator_string_parameter(par_name, value, self.toolkit_index)
        return None
        
    def get_dynamic_simulator_output_file(self):
        global STEPS_LIB
        return self.__get_string_from_c_char_p(STEPS_LIB.api_get_dynamic_simulator_output_file(self.toolkit_index))
        
    def set_dynamic_simulator_output_file(self, file):
        global STEPS_LIB
        file = self.__get_c_char_p_of_string(file)
        return STEPS_LIB.api_set_dynamic_simulator_output_file(file, self.toolkit_index)
        
    def get_dynamic_simulation_time_step(self):
        global STEPS_LIB
        return STEPS_LIB.api_get_dynamic_simulation_time_step(self.toolkit_index)
        
    def set_dynamic_simulation_time_step(self, step):
        global STEPS_LIB
        return STEPS_LIB.api_set_dynamic_simulation_time_step(step, self.toolkit_index)

    def show_dynamic_simulation_configuration(self):
        global STEPS_LIB
        STEPS_LIB.api_show_dynamic_simulation_configuration(self.toolkit_index)
        return
    
    def get_dynamic_simulation_time(self):
        global STEPS_LIB
        return STEPS_LIB.api_get_dynamic_simulation_time(self.toolkit_index)
        
    def prepare_meters(self, device_type):
        global STEPS_LIB
        device_type = device_type.upper()
        if device_type not in ['ALL', 'BUS', 'GENERATOR', 'WT GENERATOR', 'LOAD', 'LINE', 'TRANSFORMER', 'HVDC', 'EQUIVALENT DEVICE']:
            return
        if device_type in ['ALL']:
            STEPS_LIB.api_prepare_meters(self.toolkit_index)
            return
        if device_type in ['BUS']:
            STEPS_LIB.api_prepare_bus_related_meters(self.toolkit_index)
            return
        if device_type in ['GENERATOR']:
            STEPS_LIB.api_prepare_generator_related_meters(self.toolkit_index)
            return
        if device_type in ['WT GENERATOR']:
            STEPS_LIB.api_prepare_wt_generator_related_meters(self.toolkit_index)
            return
        if device_type in ['PV UNIT']:
            STEPS_LIB.api_prepare_pv_unit_related_meters(self.toolkit_index)
            return
        if device_type in ['ENERGY STORAGE']:
            STEPS_LIB.api_prepare_energy_storage_related_meters(self.toolkit_index)
            return
        if device_type in ['LOAD']:
            STEPS_LIB.api_prepare_load_related_meters(self.toolkit_index)
            return
        if device_type in ['LINE']:
            STEPS_LIB.api_prepare_line_related_meters(self.toolkit_index)
            return
        if device_type in ['TRANSFORMER']:
            STEPS_LIB.api_prepare_transformer_related_meters(self.toolkit_index)
            return
        if device_type in ['HVDC']:
            STEPS_LIB.api_prepare_hvdc_related_meters(self.toolkit_index)
            return
        if device_type in ['EQUIVALENT DEVICE']:
            STEPS_LIB.api_prepare_equivalent_device_related_meters(self.toolkit_index)
            return
        return

    def prepare_bus_meter(self, bus, meter_type):
        global STEPS_LIB
        meter_type = self.__get_c_char_p_of_string(meter_type)
        STEPS_LIB.api_prepare_bus_related_meter(bus, meter_type, self.toolkit_index)
        return

    def prepare_generator_meter(self, generator, meter_type, var_name=""):
        global STEPS_LIB
        bus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        meter_type = self.__get_c_char_p_of_string(meter_type)
        var_name = self.__get_c_char_p_of_string(var_name)
        STEPS_LIB.api_prepare_generator_related_meter(bus, ickt, meter_type, var_name, self.toolkit_index)
        return

    def prepare_wt_generator_meter(self, generator, meter_type, var_name=""):
        global STEPS_LIB
        bus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        meter_type = self.__get_c_char_p_of_string(meter_type)
        var_name = self.__get_c_char_p_of_string(var_name)
        STEPS_LIB.api_prepare_wt_generator_related_meter(bus, ickt, meter_type, var_name, self.toolkit_index)
        return

    def prepare_pv_unit_meter(self, pvunit, meter_type, var_name=""):
        global STEPS_LIB
        bus, ickt = self.__extract_single_bus_device_id(pvunit)
        ickt = self.__get_c_char_p_of_string(ickt)
        meter_type = self.__get_c_char_p_of_string(meter_type)
        var_name = self.__get_c_char_p_of_string(var_name)
        STEPS_LIB.api_prepare_pv_unit_related_meter(bus, ickt, meter_type, var_name, self.toolkit_index)
        return

    def prepare_energy_storage_meter(self, estorage, meter_type, var_name=""):
        global STEPS_LIB
        bus, ickt = self.__extract_single_bus_device_id(estorage)
        ickt = self.__get_c_char_p_of_string(ickt)
        meter_type = self.__get_c_char_p_of_string(meter_type)
        var_name = self.__get_c_char_p_of_string(var_name)
        STEPS_LIB.api_prepare_energy_storage_related_meter(bus, ickt, meter_type, var_name, self.toolkit_index)
        return

    def prepare_load_meter(self, load, meter_type, var_name=""):
        global STEPS_LIB
        bus, ickt = self.__extract_single_bus_device_id(load)
        ickt = self.__get_c_char_p_of_string(ickt)
        meter_type = self.__get_c_char_p_of_string(meter_type)
        var_name = self.__get_c_char_p_of_string(var_name)
        STEPS_LIB.api_prepare_load_related_meter(bus, ickt, meter_type, var_name, self.toolkit_index)
        return

    def prepare_line_meter(self, line, meter_type, side, var_name=""):
        global STEPS_LIB
        ibus, jbus, ickt = self.__extract_double_bus_device_id(line)
        ickt = self.__get_c_char_p_of_string(ickt)
        meter_type = self.__get_c_char_p_of_string(meter_type)
        side = self.__get_c_char_p_of_string(side)
        var_name = self.__get_c_char_p_of_string(var_name)
        STEPS_LIB.api_prepare_line_related_meter(ibus, jbus, ickt, meter_type, side, var_name, self.toolkit_index)
        return

    def prepare_transformer_meter(self, trans, meter_type, side, var_name=""):
        global STEPS_LIB
        ibus, jbus, kbus, ickt = self.__extract_triple_bus_device_id(trans)
        ickt = self.__get_c_char_p_of_string(ickt)
        meter_type = self.__get_c_char_p_of_string(meter_type)
        side = self.__get_c_char_p_of_string(side)
        var_name = self.__get_c_char_p_of_string(var_name)
        STEPS_LIB.api_prepare_transformer_related_meter(ibus, jbus, kbus, ickt, meter_type, side, var_name, self.toolkit_index)
        return

    def prepare_hvdc_meter(self, hvdc, meter_type, side, var_name=""):
        global STEPS_LIB
        ibus, jbus, ickt = self.__extract_double_bus_device_id(hvdc)
        ickt = self.__get_c_char_p_of_string(ickt)
        meter_type = self.__get_c_char_p_of_string(meter_type)
        side = self.__get_c_char_p_of_string(side)
        var_name = self.__get_c_char_p_of_string(var_name)
        STEPS_LIB.api_prepare_hvdc_related_meter(ibus, jbus, ickt, meter_type, side, var_name, self.toolkit_index)
        return

    def prepare_equivalent_device_meter(self, edevice, meter_type, var_name=""):
        global STEPS_LIB
        bus, ickt = self.__extract_single_bus_device_id(equivalent_device)
        ickt = self.__get_c_char_p_of_string(ickt)
        meter_type = self.__get_c_char_p_of_string(meter_type)
        var_name = self.__get_c_char_p_of_string(var_name)
        STEPS_LIB.api_prepare_equivalent_device_related_meter(bus, ickt, meter_type, var_name, self.toolkit_index)
        return

    def start_dynamic_simulation(self):
        global STEPS_LIB
        STEPS_LIB.api_start_dynamic_simulation(self.toolkit_index)
        return

    def stop_dynamic_simulation(self):
        global STEPS_LIB
        STEPS_LIB.api_stop_dynamic_simulation(self.toolkit_index)
        return

    def run_dynamic_simulation_to_time(self, time):
        global STEPS_LIB
        STEPS_LIB.api_run_simulation_to_time(time, self.toolkit_index)
        return

    def run_a_step(self):
        global STEPS_LIB
        STEPS_LIB.api_run_a_step(self.toolkit_index)
        return

    def set_bus_fault(self, bus, fault_type, fault_shunt):
        global STEPS_LIB
        fault_type = self.__get_c_char_p_of_string(fault_type)
        STEPS_LIB.api_set_bus_fault(bus, fault_type, fault_shunt[0], fault_shunt[1], self.toolkit_index)
        return

    def clear_bus_fault(self, bus, fault_type):
        global STEPS_LIB
        fault_type = self.__get_c_char_p_of_string(fault_type)
        STEPS_LIB.api_clear_bus_fault(bus, fault_type, self.toolkit_index)
        return

    def trip_bus(self, bus):
        global STEPS_LIB
        STEPS_LIB.api_trip_bus(bus, self.toolkit_index)
        return

    def set_line_fault(self, line, fault_type, fault_location, fault_shunt):
        global STEPS_LIB
        ibus, jbus, ickt = self.__extract_double_bus_device_id(line)
        ickt = self.__get_c_char_p_of_string(ickt)
        fault_type = self.__get_c_char_p_of_string(fault_type)
        STEPS_LIB.api_set_line_fault(ibus, jbus, ickt, fault_type, fault_location, fault_shunt[0], fault_shunt[1], self.toolkit_index)
        return

    def clear_line_fault(self, line, fault_type, fault_location):
        global STEPS_LIB
        ibus, jbus, ickt = self.__extract_double_bus_device_id(line)
        ickt = self.__get_c_char_p_of_string(ickt)
        fault_type = self.__get_c_char_p_of_string(fault_type)
        STEPS_LIB.api_clear_line_fault(ibus, jbus, ickt, fault_type, fault_location, self.toolkit_index)
        return

    def trip_line(self, line):
        global STEPS_LIB
        ibus, jbus, ickt = self.__extract_double_bus_device_id(line)
        ickt = self.__get_c_char_p_of_string(ickt)
        STEPS_LIB.api_trip_line(ibus, jbus, ickt, self.toolkit_index)
        return

    def trip_line_breaker(self, line, side):
        global STEPS_LIB
        if side not in line:
            return
        else:
            ibus, jbus, ickt = self.__extract_single_bus_device_id(line)
            ickt = self.__get_c_char_p_of_string(ickt)
            
            if ibus==side:
                STEPS_LIB.api_trip_line_breaker(ibus, jbus, ickt, self.toolkit_index)
            else:
                STEPS_LIB.api_trip_line_breaker(jbus, ibus, ickt, self.toolkit_index)
        return
        

    def close_line(self, line):
        global STEPS_LIB
        ibus, jbus, ickt = self.__extract_double_bus_device_id(line)
        ickt = self.__get_c_char_p_of_string(ickt)
        STEPS_LIB.api_close_line(ibus, jbus, ickt, self.toolkit_index)
        return

    def close_line_breaker(self, line, side):
        global STEPS_LIB
        if side not in line:
            return
        else:
            ibus, jbus, ickt = self.__extract_single_bus_device_id(line)
            ickt = self.__get_c_char_p_of_string(ickt)
            
            if ibus==side:
                STEPS_LIB.api_close_line_breaker(ibus, jbus, ickt, self.toolkit_index)
            else:
                STEPS_LIB.api_close_line_breaker(jbus, ibus, ickt, self.toolkit_index)
        return
        

    def trip_transformer(self, transformer):
        global STEPS_LIB
        ibus, jbus, kbus, ickt = self.__extract_triple_bus_device_id(transformer)
        ickt = self.__get_c_char_p_of_string(ickt)
        STEPS_LIB.api_trip_transformer(ibus, jbus, kbus, ickt, self.toolkit_index)
        return

    def trip_transformer_breaker(self, transformer, side):
        global STEPS_LIB
        if side not in transformer or side==0:
            return
        else:
            ibus, jbus, kbus, ickt = self.__extract_triple_bus_device_id(transformer)
            ickt = self.__get_c_char_p_of_string(ickt)
            if ibus == side:
                STEPS_LIB.api_trip_transformer_breaker(ibus, jbus, kbus, ickt, self.toolkit_index)
            elif jbus == side:
                STEPS_LIB.api_trip_transformer_breaker(jbus, ibus, kbus, ickt, self.toolkit_index)
            else:
                STEPS_LIB.api_trip_transformer_breaker(kbus, ibus, jbus, ickt, self.toolkit_index)
        return
        

    def close_transformer(self, transformer):
        global STEPS_LIB
        ibus, jbus, kbus, ickt = self.__extract_triple_bus_device_id(transformer)
        ickt = self.__get_c_char_p_of_string(ickt)
        STEPS_LIB.api_close_transformer(ibus, jbus, kbus, ickt, self.toolkit_index)
        return

    def close_transformer_breaker(self, transformer, side):
        global STEPS_LIB
        if side not in transformer or side==0:
            return
        else:
            ibus, jbus, kbus, ickt = self.__extract_triple_bus_device_id(transformer)
            ickt = self.__get_c_char_p_of_string(ickt)
            if ibus == side:
                STEPS_LIB.api_close_transformer_breaker(ibus, jbus, kbus, ickt, self.toolkit_index)
            elif jbus == side:
                STEPS_LIB.api_close_transformer_breaker(jbus, ibus, kbus, ickt, self.toolkit_index)
            else:
                STEPS_LIB.api_trip_tapi_close_transformer_breakerransformer_breaker(kbus, ibus, jbus, ickt, self.toolkit_index)
        return
        
    def trip_generator(self, generator):
        global STEPS_LIB
        ibus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        STEPS_LIB.api_trip_generator(ibus, ickt, self.toolkit_index)
        return

    def shed_generator(self, generator, percent):
        global STEPS_LIB
        ibus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        STEPS_LIB.api_shed_generator(ibus, ickt, percent, self.toolkit_index)
        return

    def trip_load(self, load):
        global STEPS_LIB
        ibus, ickt = self.__extract_single_bus_device_id(load)
        ickt = self.__get_c_char_p_of_string(ickt)
        STEPS_LIB.api_trip_load(ibus, ickt, self.toolkit_index)
        return

    def close_load(self, load):
        global STEPS_LIB
        ibus, ickt = self.__extract_single_bus_device_id(load)
        ickt = self.__get_c_char_p_of_string(ickt)
        STEPS_LIB.api_close_load(ibus, ickt, self.toolkit_index)
        return

    def scale_load(self, load, percent):
        global STEPS_LIB
        ibus, ickt = self.__extract_single_bus_device_id(load)
        ickt = self.__get_c_char_p_of_string(ickt)
        STEPS_LIB.api_scale_load(ibus, ickt, percent, self.toolkit_index)
        return

    def scale_all_loads(self, percent):
        global STEPS_LIB
        STEPS_LIB.api_scale_all_loads(percent, self.toolkit_index)
        return

    def trip_fixed_shunt(self, shunt):
        global STEPS_LIB
        ibus, ickt = self.__extract_single_bus_device_id(shunt)
        ickt = self.__get_c_char_p_of_string(ickt)
        STEPS_LIB.api_trip_fixed_shunt(ibus, ickt, self.toolkit_index)
        return

    def close_fixed_shunt(self, shunt):
        global STEPS_LIB
        ibus, ickt = self.__extract_single_bus_device_id(shunt)
        ickt = self.__get_c_char_p_of_string(ickt)
        STEPS_LIB.api_close_fixed_shunt(ibus, ickt, self.toolkit_index)
        return

    def manually_bypass_hvdc(self, hvdc):
        global STEPS_LIB
        ibus, jbus, ickt = self.__extract_double_bus_device_id(hvdc)
        ickt = self.__get_c_char_p_of_string(ickt)
        STEPS_LIB.api_manually_bypass_hvdc(ibus, jbus, ickt, self.toolkit_index)
        return

    def manually_block_hvdc(self, hvdc):
        global STEPS_LIB
        ibus, jbus, ickt = self.__extract_double_bus_device_id(hvdc)
        ickt = self.__get_c_char_p_of_string(ickt)
        STEPS_LIB.api_manually_block_hvdc(ibus, jbus, ickt, self.toolkit_index)
        return

    def manually_unbypass_hvdc(self, hvdc):
        global STEPS_LIB
        ibus, jbus, ickt = self.__extract_double_bus_device_id(hvdc)
        ickt = self.__get_c_char_p_of_string(ickt)
        STEPS_LIB.api_manually_unbypass_hvdc(ibus, jbus, ickt, self.toolkit_index)
        return

    def manually_unblock_hvdc(self, hvdc):
        global STEPS_LIB
        ibus, jbus, ickt = self.__extract_double_bus_device_id(hvdc)
        ickt = self.__get_c_char_p_of_string(ickt)
        STEPS_LIB.api_manually_unblock_hvdc(ibus, jbus, ickt, self.toolkit_index)
        return

    def get_generator_voltage_reference_in_pu(self, generator):
        global STEPS_LIB
        ibus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        return STEPS_LIB.api_get_generator_voltage_reference_in_pu(ibus, ickt, self.toolkit_index)

    def get_generator_mechanical_power_reference_in_pu(self, generator):
        global STEPS_LIB
        ibus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        return STEPS_LIB.api_get_generator_mechanical_power_reference_in_pu_based_on_mbase(ibus, ickt, self.toolkit_index)

    def get_generator_mechanical_power_reference_in_MW(self, generator):
        global STEPS_LIB
        ibus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        return STEPS_LIB.api_get_generator_mechanical_power_reference_in_MW(ibus, ickt, self.toolkit_index)

    def set_generator_voltage_reference_in_pu(self, generator, value):
        global STEPS_LIB
        ibus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        return STEPS_LIB.api_set_generator_voltage_reference_in_pu(ibus, ickt, value, self.toolkit_index)

    def set_generator_mechanical_power_reference_in_pu(self, generator, value):
        global STEPS_LIB
        ibus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        return STEPS_LIB.api_set_generator_mechanical_power_reference_in_pu_based_on_mbase(ibus, ickt, value, self.toolkit_index)
        
    def set_generator_mechanical_power_reference_in_MW(self, generator, value):
        global STEPS_LIB
        ibus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        return STEPS_LIB.api_set_generator_mechanical_power_reference_in_MW(ibus, ickt, value, self.toolkit_index)
        
    def get_generator_excitation_voltage_in_pu(self, generator):
        global STEPS_LIB
        ibus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        return STEPS_LIB.api_get_generator_excitation_voltage_in_pu(ibus, ickt, self.toolkit_index)

    def get_generator_mechanical_power_in_pu(self, generator):
        global STEPS_LIB
        ibus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        return STEPS_LIB.api_get_generator_mechanical_power_in_pu_based_on_mbase(ibus, ickt, self.toolkit_index)

    def get_generator_mechanical_power_in_MW(self, generator):
        global STEPS_LIB
        ibus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        return STEPS_LIB.api_get_generator_mechanical_power_in_MW(ibus, ickt, self.toolkit_index)

    def set_generator_excitation_voltage_in_pu(self, generator, value):
        global STEPS_LIB
        ibus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        return STEPS_LIB.api_set_generator_excitation_voltage_in_pu(ibus, ickt, value, self.toolkit_index)

    def set_generator_mechanical_power_in_pu(self, generator, value):
        global STEPS_LIB
        ibus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        return STEPS_LIB.api_set_generator_mechanical_power_in_pu_based_on_mbase(ibus, ickt, value, self.toolkit_index)
        
    def set_generator_mechanical_power_in_MW(self, generator, value):
        global STEPS_LIB
        ibus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        return STEPS_LIB.api_set_generator_mechanical_power_in_MW(ibus, ickt, value, self.toolkit_index)

    def get_hvdc_power_order_in_MW(self, hvdc):
        global STEPS_LIB
        ibus, jbus, ickt = self.__extract_double_bus_device_id(hvdc)
        ickt = self.__get_c_char_p_of_string(ickt)
        return STEPS_LIB.api_get_hvdc_power_order_in_MW(ibus, jbus, ickt, self.toolkit_index)
        
    def set_hvdc_power_order_in_MW(self, hvdc, value):
        global STEPS_LIB
        ibus, jbus, ickt = self.__extract_double_bus_device_id(hvdc)
        ickt = self.__get_c_char_p_of_string(ickt)
        return STEPS_LIB.api_set_hvdc_power_order_in_MW(ibus, jbus, ickt, value, self.toolkit_index)
        
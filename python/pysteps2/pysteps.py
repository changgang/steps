pysteps_version = "0.5.1"
pysteps_date = "2019/01/29"
pysteps_author = (("Changgang Li", "lichangang@sdu.edu.cn"),("Yue Wu","sduyuewu2018@163.com"))

from .libsteps import pylibsteps
from ctypes import c_char_p
import platform

class STEPS():
    def __init__(self, parallel_flag = False):        
        self.libsteps = pylibsteps.load_library(parallel_flag)
        if self.libsteps is None:
            return None
        
        self.libsteps.api_initialize_package()
        #if parallel_flag==False:
        #    print("STEPS simulator is created:", self)
        
    def __del__(self):
        if self.libsteps is None:
            return None
        self.clear_package()
        #print("STEPS simulator is deleted")

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

    def clear_package(self):
        self.libsteps.api_clear_package()
        return

    def terminate_package(self):
        self.libsteps = None
        #self.libsteps.api_terminate_package()
        return

    def get_package_float_data(self, dataname):
        dataname = self.__get_c_char_p_of_string(dataname)
        return self.libsteps.api_get_package_float_data(dataname)        

    def set_package_float_data(self, dataname, value):
        dataname = self.__get_c_char_p_of_string(dataname)
        return self.libsteps.api_set_package_float_data(dataname, value)

    def get_allowed_maximum_bus_number(self):
        return int(self.libsteps.api_get_allowed_maximum_bus_number())

    def set_allowed_maximum_bus_number(self, max_bus_number):
        self.libsteps.api_set_allowed_maximum_bus_number(max_bus_number)

    def load_powerflow_data(self, file, ftype):
        file = self.__get_c_char_p_of_string(file)
        ftype = self.__get_c_char_p_of_string(ftype)
        self.libsteps.api_load_powerflow_data_from_file(file, ftype)

    def save_powerflow_data(self, file, ftype, export_zero_line=True):
        file = self.__get_c_char_p_of_string(file)
        ftype = self.__get_c_char_p_of_string(ftype)
        self.libsteps.api_save_powerflow_data_to_file(file, ftype, export_zero_line)
        
    def load_dynamic_data(self, file, ftype):
        file = self.__get_c_char_p_of_string(file)
        ftype = self.__get_c_char_p_of_string(ftype)
        self.libsteps.api_load_dynamic_data_from_file(file, ftype)
        
    def save_dynamic_data(self, file, ftype):
        file = self.__get_c_char_p_of_string(file)
        ftype = self.__get_c_char_p_of_string(ftype)
        self.libsteps.api_save_dynamic_data_from_file(file, ftype)

    def get_bus_capacity(self):
        device = self.__get_c_char_p_of_string("BUS")
        return int(self.libsteps.api_get_device_capacity(device))
    def get_generator_capacity(self):
        device = self.__get_c_char_p_of_string("Generator")
        return int(self.libsteps.api_get_device_capacity(device))
    def get_wt_generator_capacity(self):
        device = self.__get_c_char_p_of_string("WT Generator")
        return int(self.libsteps.api_get_device_capacity(device))
    def get_pv_unit_capacity(self):
        device = self.__get_c_char_p_of_string("PV Unit")
        return int(self.libsteps.api_get_device_capacity(device))
    def get_load_capacity(self):
        device = self.__get_c_char_p_of_string("Load")
        return int(self.libsteps.api_get_device_capacity(device))
    def get_fixed_shunt_capacity(self):
        device = self.__get_c_char_p_of_string("Fixed Shunt")
        return int(self.libsteps.api_get_device_capacity(device))
    def get_line_capacity(self):
        device = self.__get_c_char_p_of_string("Line")
        return int(self.libsteps.api_get_device_capacity(device))
    def get_transformer_capacity(self):
        device = self.__get_c_char_p_of_string("Transformer")
        return int(self.libsteps.api_get_device_capacity(device))
    def get_hvdc_capacity(self):
        device = self.__get_c_char_p_of_string("HVDC")
        return int(self.libsteps.api_get_device_capacity(device))
    def get_equivalent_device_capacity(self):
        device = self.__get_c_char_p_of_string("Equivalent Device")
        return int(self.libsteps.api_get_device_capacity(device))
    def get_energy_storage_capacity(self):
        device = self.__get_c_char_p_of_string("Energy Storage")
        return int(self.libsteps.api_get_device_capacity(device))
    def get_area_capacity(self):
        return int(self.libsteps.api_get_area_capacity())
    def get_zone_capacity(self):
        return int(self.libsteps.api_get_zone_capacity())
    def get_owner_capacity(self):
        return int(self.libsteps.api_get_owner_capacity())

    def set_bus_capacity(self, capacity):
        device = self.__get_c_char_p_of_string("BUS")
        return self.libsteps.api_set_device_capacity("BUS", capacity)
    def set_generator_capacity(self, capacity):
        device = self.__get_c_char_p_of_string("Generator")
        return self.libsteps.api_set_device_capacity(device, capacity)
    def set_wt_generator_capacity(self, capacity):
        device = self.__get_c_char_p_of_string("Wt Generator")
        return self.libsteps.api_set_device_capacity(device, capacity)
    def set_pv_unit_capacity(self, capacity):
        device = self.__get_c_char_p_of_string("PV Unit")
        return self.libsteps.api_set_device_capacity(device, capacity)
    def set_load_capacity(self, capacity):
        device = self.__get_c_char_p_of_string("Load")
        return self.libsteps.api_set_device_capacity(device, capacity)
    def set_fixed_shunt_capacity(self, capacity):
        device = self.__get_c_char_p_of_string("Fixed Shunt")
        return self.libsteps.api_set_device_capacity(device, capacity)
    def set_line_capacity(self, capacity):
        device = self.__get_c_char_p_of_string("Line")
        return self.libsteps.api_set_device_capacity(device, capacity)
    def set_transformer_capacity(self, capacity):
        device = self.__get_c_char_p_of_string("Transformer")
        return self.libsteps.api_set_device_capacity(device, capacity)
    def set_hvdc_capacity(self, capacity):
        device = self.__get_c_char_p_of_string("HVDC")
        return self.libsteps.api_set_device_capacity(device, capacity)
    def set_equivalent_device_capacity(self, capacity):
        device = self.__get_c_char_p_of_string("Equivalent Device")
        return self.libsteps.api_set_device_capacity(device, capacity)
    def set_energy_storage_capacity(self, capacity):
        device = self.__get_c_char_p_of_string("Energy Storage")
        return self.libsteps.api_set_device_capacity(device, capacity)
    def set_area_capacity(self, capacity):
        return self.libsteps.api_set_area_capacity(capacity)
    def set_zone_capacity(self, capacity):
        return self.libsteps.api_set_zone_capacity(capacity)
    def set_owner_capacity(self, capacity):
        return self.libsteps.api_set_owner_capacity(capacity)


    def add_bus(self, busnumber, busname, basevoltage):
        busname = self.__get_c_char_p_of_string(busname)
        self.libsteps.api_add_bus(busnumber, busname, basevoltage)
        return

    def add_generator(self, generator):
        bus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        self.libsteps.api_add_generator(bus, ickt)
        return

    def add_wt_generator(self, generator):
        bus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        self.libsteps.api_add_wt_generator(bus, ickt)

    def add_pv_unit(self, unit):
        bus, ickt = self.__extract_single_bus_device_id(unit)
        ickt = self.__get_c_char_p_of_string(ickt)
        self.libsteps.api_add_pv_unit(bus, ickt)
        return

    def add_load(self, load):
        bus, ickt = self.__extract_single_bus_device_id(load)
        ickt = self.__get_c_char_p_of_string(ickt)
        self.libsteps.api_add_load(bus, ickt)
        return

    def add_fixed_shunt(self, shunt):
        bus, ickt = self.__extract_single_bus_device_id(shunt)
        ickt = self.__get_c_char_p_of_string(ickt)
        self.libsteps.api_add_fixed_shunt(bus, ickt)
        return

    def add_line(self, line):
        ibus, jbus, ickt = self.__extract_double_bus_device_id(line)
        ickt = self.__get_c_char_p_of_string(ickt)
        self.libsteps.api_add_line(ibus, jbus, ickt)
        return

    def add_hvdc(self, hvdc):
        ibus, jbus, ickt = self.__extract_double_bus_device_id(hvdc)
        ickt = self.__get_c_char_p_of_string(ickt)
        self.libsteps.api_add_hvdc(ibus, jbus, ickt)
        return

    def add_transformer(self, transformer):
        ibus, jbus, kbus, ickt = self.__extract_triple_bus_device_id(transformer)
        ickt = self.__get_c_char_p_of_string(ickt)
        self.libsteps.api_add_transformer(ibus, jbus, kbus, ickt)
        return

    def add_equivalent_device(self, device):
        bus, ickt = self.__extract_single_bus_device_id(device)
        ickt = self.__get_c_char_p_of_string(ickt)
        self.libsteps.api_add_equivalent_device(bus, ickt)
        return

    def add_energy_storage(self, storage):
        bus, ickt = self.__extract_single_bus_device_id(storage)
        ickt = self.__get_c_char_p_of_string(ickt)
        self.libsteps.api_add_energy_storage(bus, ickt)
        return

    def add_area(self, areanumber, areaname):
        areaname = self.__get_c_char_p_of_string(areaname)
        self.libsteps.api_add_area(areanumber, areaname)
        return

    def add_zone(self, zonenumber, zonename):
        zonename = self.__get_c_char_p_of_string(zonename)
        self.libsteps.api_add_zone(zonenumber, zonename)
        return

    def add_owner(self, ownernumber, ownername):
        ownername = self.__get_c_char_p_of_string(ownername)
        self.libsteps.api_add_owner(ownernumber, ownername)
        return

    def remove_bus(self, busnumber):
        self.libsteps.api_remove_bus(busnumber)
        return

    def remove_generator(self, generator):
        bus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        self.libsteps.api_remove_generator(bus, ickt)
        return

    def remove_wt_generator(self, generator):
        bus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        self.libsteps.api_remove_wt_generator(bus, ickt)

    def remove_pv_unit(self, unit):
        bus, ickt = self.__extract_single_bus_device_id(unit)
        ickt = self.__get_c_char_p_of_string(ickt)
        self.libsteps.api_remove_pv_unit(bus, ickt)
        return

    def remove_load(self, load):
        bus, ickt = self.__extract_single_bus_device_id(load)
        ickt = self.__get_c_char_p_of_string(ickt)
        self.libsteps.api_remove_load(bus, ickt)
        return

    def remove_fixed_shunt(self, shunt):
        bus, ickt = self.__extract_single_bus_device_id(shunt)
        ickt = self.__get_c_char_p_of_string(ickt)
        self.libsteps.api_remove_fixed_shunt(bus, ickt)
        return

    def remove_line(self, line):
        ibus, jbus, ickt = self.__extract_double_bus_device_id(line)
        ickt = self.__get_c_char_p_of_string(ickt)
        self.libsteps.api_remove_line(ibus, jbus, ickt)
        return

    def remove_hvdc(self, hvdc):
        ibus, jbus, ickt = self.__extract_double_bus_device_id(hvdc)
        ickt = self.__get_c_char_p_of_string(ickt)
        self.libsteps.api_remove_hvdc(ibus, jbus, ickt)
        return

    def remove_transformer(self, transformer):
        ibus, jbus, kbus, ickt = self.__extract_triple_bus_device_id(transformer)
        ickt = self.__get_c_char_p_of_string(ickt)
        self.libsteps.api_remove_transformer(ibus, jbus, kbus, ickt)
        return

    def remove_equivalent_device(self, device):
        bus, ickt = self.__extract_single_bus_device_id(device)
        ickt = self.__get_c_char_p_of_string(ickt)
        self.libsteps.api_remove_equivalent_device(bus, ickt)
        return

    def remove_energy_storage(self, storage):
        bus, ickt = self.__extract_single_bus_device_id(storage)
        ickt = self.__get_c_char_p_of_string(ickt)
        self.libsteps.api_remove_energy_storage(bus, ickt)
        return

    def remove_area(self, areanumber):
        self.libsteps.api_remove_area(areanumber)
        return

    def remove_zone(self, zonenumber):
        self.libsteps.api_remove_zone(zonenumber)
        return

    def remove_owner(self, ownernumber):
        self.libsteps.api_remove_owner(ownernumber)
        return
    
    def get_bus_count(self):
        device = self.__get_c_char_p_of_string("BUS")
        return self.libsteps.api_get_device_count(device)
    def get_generator_count(self):
        device = self.__get_c_char_p_of_string("Generator")
        return self.libsteps.api_get_device_count(device)
    def get_wt_generator_count(self):
        device = self.__get_c_char_p_of_string("WT Generator")
        return self.libsteps.api_get_device_count(device)
    def get_pv_unit_count(self):
        device = self.__get_c_char_p_of_string("PV Unit")
        return self.libsteps.api_get_device_count(device)
    def get_load_count(self):
        device = self.__get_c_char_p_of_string("Load")
        return self.libsteps.api_get_device_count(device)
    def get_fixed_shunt_count(self):
        device = self.__get_c_char_p_of_string("Fixed Shunt")
        return self.libsteps.api_get_device_count(device)
    def get_line_count(self):
        device = self.__get_c_char_p_of_string("Line")
        return self.libsteps.api_get_device_count(device)
    def get_transformer_count(self):
        device = self.__get_c_char_p_of_string("Transformer")
        return self.libsteps.api_get_device_count(device)
    def get_hvdc_count(self):
        device = self.__get_c_char_p_of_string("HVDC")
        return self.libsteps.api_get_device_count(device)
    def get_equivalent_device_count(self):
        device = self.__get_c_char_p_of_string("Equivalent Device")
        return self.libsteps.api_get_device_count(device)
    def get_energy_storage_count(self):
        device = self.__get_c_char_p_of_string("Energy Storage")
        return self.libsteps.api_get_device_count(device)
    def get_area_count(self):
        return self.libsteps.api_get_area_count()
    def get_zone_count(self):
        return self.libsteps.api_get_zone_count()
    def get_owner_count(self):
        return self.libsteps.api_get_owner_count()
    def get_in_service_bus_count(self):
        self.libsteps.api_get_in_service_bus_count()
        return
    def update_overshadowed_buses(self):
        return self.libsteps.api_update_overshadowed_buses()
    def set_all_buses_un_overshadowed(self):
        self.libsteps.api_set_all_buses_un_overshadowed()
        return
    def get_overshadowed_bus_count(self):
        return self.libsteps.api_get_overshadowed_bus_count()

    def is_bus_exist(self, bus):
        return self.libsteps.api_is_bus_exist(bus)

    def is_generator_exist(self, generator):
        bus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        return self.libsteps.api_is_generator_exist(bus, ickt)

    def is_wt_generator_exist(self, generator):
        bus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        return self.libsteps.api_is_wt_generator_exist(bus, ickt)

    def is_pv_unit_exist(self, pv_unit):
        bus, ickt = self.__extract_single_bus_device_id(pv_unit)
        ickt = self.__get_c_char_p_of_string(ickt)
        return self.libsteps.api_is_pv_unit_exist(bus, ickt)

    def is_load_exist(self, load):
        bus, ickt = self.__extract_single_bus_device_id(load)
        ickt = self.__get_c_char_p_of_string(ickt)
        return self.libsteps.api_is_load_exist(bus, ickt)

    def is_fixed_shunt_exist(self, shunt):
        bus, ickt = self.__extract_single_bus_device_id(shunt)
        ickt = self.__get_c_char_p_of_string(ickt)
        return self.libsteps.api_is_fixed_shunt_exist(bus, ickt)

    def is_line_exist(self, line):
        ibus, jbus, ickt = self.__extract_double_bus_device_id(line)
        ickt = self.__get_c_char_p_of_string(ickt)
        return self.libsteps.api_is_line_exist(ibus, jbus, ickt)

    def is_transformer_exist(self, transformer):
        ibus, jbus, kbus, ickt = self.__extract_triple_bus_device_id(transformer)
        ickt = self.__get_c_char_p_of_string(ickt)
        return self.libsteps.api_is_transformer_exist(ibus, jbus, kbus, ickt)

    def is_hvdc_exist(self, hvdc):
        ibus, jbus, ickt = self.__extract_double_bus_device_id(hvdc)
        ickt = self.__get_c_char_p_of_string(ickt)
        return self.libsteps.api_is_hvdc_exist(ibus, jbus, ickt)

    def is_equivalent_device_exist(self, equivalent_device):
        bus, ickt = self.__extract_single_bus_device_id(equivalent_device)
        ickt = self.__get_c_char_p_of_string(ickt)
        return self.libsteps.api_is_equivalent_device_exist(bus, ickt)

    def is_energy_storage_exist(self, energy_storage):
        bus, ickt = self.__extract_single_bus_device_id(energy_storage)
        ickt = self.__get_c_char_p_of_string(ickt)
        return self.libsteps.api_is_energy_storage_exist(bus, ickt)
    
    def bus_name2number(self, name):
        name = self.__get_c_char_p_of_string(name)
        return int(self.libsteps.api_bus_name2bus_number(name))

    def bus_number2name(self, bus):
        return str(self.libsteps.api_bus_number2bus_name(bus))

    def get_all_buses(self):
        self.libsteps.api_initialize_all_bus_search()
        buses = []
        while True:
            bus = self.libsteps.api_get_current_bus_number()
            if bus==0:
                break
            buses.append(int(bus))
            self.libsteps.api_goto_next_bus()
        return tuple(buses)

    def get_buses_with_constraints(self, constraints):
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
        
        self.libsteps.api_initialize_bus_search(vbase_min, vbase_max, v_min, v_max, area, zone, owner)
        while True:
            bus = self.libsteps.api_get_current_bus_number()
            if bus==0:
                break
            buses.append(int(bus))
            self.libsteps.api_goto_next_bus()
        return tuple(buses)
        
    def get_generators_at_bus(self, bus):
        device = "GENERATOR"
        device = self.__get_c_char_p_of_string(device)
        blank = self.__get_c_char_p_of_string("")
        self.libsteps.api_initialize_device_search(device, bus)
        generators = []
        while True:
            bus = self.libsteps.api_get_current_device_bus_number(device, blank)
            if bus==0:
                break
            id = self.libsteps.api_get_current_device_identifier(device)
            id = str(id)
            generators.append((int(bus), id))
            self.libsteps.api_goto_next_device(device)
        return tuple(generators)
        
    def get_wt_generators_at_bus(self, bus):
        device = "WT GENERATOR"
        device = self.__get_c_char_p_of_string(device)
        blank = self.__get_c_char_p_of_string("")
        self.libsteps.api_initialize_device_search(device, bus)
        wt_generators = []
        while True:
            bus = self.libsteps.api_get_current_device_bus_number(device, blank)
            if bus==0:
                break
            id = self.libsteps.api_get_current_device_identifier(device)
            id = str(id)
            wt_generators.append((int(bus), id))
            self.libsteps.api_goto_next_device(device)
        return tuple(wt_generators)
        
    def get_pv_units_at_bus(self, bus):
        device = "PV UNIT"
        device = self.__get_c_char_p_of_string(device)
        blank = self.__get_c_char_p_of_string("")
        self.libsteps.api_initialize_device_search(device, bus)
        pv_units = []
        while True:
            bus = self.libsteps.api_get_current_device_bus_number(device, blank)
            if bus==0:
                break
            id = self.libsteps.api_get_current_device_identifier(device)
            id = str(id)
            pv_units.append((int(bus), id))
            self.libsteps.api_goto_next_device(device)
        return tuple(pv_units)
        
    def get_loads_at_bus(self, bus):
        device = "LOAD"
        device = self.__get_c_char_p_of_string(device)
        blank = self.__get_c_char_p_of_string("")
        self.libsteps.api_initialize_device_search(device, bus)
        loads = []
        while True:
            bus = self.libsteps.api_get_current_device_bus_number(device, blank)
            if bus==0:
                break
            id = self.libsteps.api_get_current_device_identifier(device)
            id = str(id)
            loads.append((int(bus), id))
            self.libsteps.api_goto_next_device(device)
        return tuple(loads)
        
    def get_fixed_shunts_at_bus(self, bus):
        device = "FIXED SHUNT"
        device = self.__get_c_char_p_of_string(device)
        blank = self.__get_c_char_p_of_string("")
        self.libsteps.api_initialize_device_search(device, bus)
        fixed_shunts = []
        while True:
            bus = self.libsteps.api_get_current_device_bus_number(device, blank)
            if bus==0:
                break
            id = self.libsteps.api_get_current_device_identifier(device)
            id = str(id)
            fixed_shunts.append((int(bus), id))
            self.libsteps.api_goto_next_device(device)
        return tuple(fixed_shunts)
        
    def get_equivalent_devices_at_bus(self, bus):
        device = "EQUIVALENT DEVICE"
        device = self.__get_c_char_p_of_string(device)
        blank = self.__get_c_char_p_of_string("")
        self.libsteps.api_initialize_device_search(device, bus)
        equivalent_devices = []
        while True:
            bus = self.libsteps.api_get_current_device_bus_number(device, blank)
            if bus==0:
                break
            id = self.libsteps.api_get_current_device_identifier(device)
            id = str(id)
            equivalent_devices.append((int(bus), id))
            self.libsteps.api_goto_next_device(device)
        return tuple(equivalent_devices)
        
    def get_energy_storages_at_bus(self, bus):
        device = "ENERGY STORAGE"
        device = self.__get_c_char_p_of_string(device)
        blank = self.__get_c_char_p_of_string("")
        self.libsteps.api_initialize_device_search(device, bus)
        energy_storages = []
        while True:
            bus = self.libsteps.api_get_current_device_bus_number(device, blank)
            if bus==0:
                break
            id = self.libsteps.api_get_current_device_identifier(device)
            id = str(id)
            energy_storages.append((int(bus), id))
            self.libsteps.api_goto_next_device(device)
        return tuple(energy_storages)

    def get_lines_at_bus(self, bus):
        device = "LINE"
        device = self.__get_c_char_p_of_string(device)
        send_side = self.__get_c_char_p_of_string("SEND")
        recv_side = self.__get_c_char_p_of_string("RECEIVE")
        self.libsteps.api_initialize_device_search(device, bus)
        lines = []
        while True:
            ibus = self.libsteps.api_get_current_device_bus_number(device, send_side)
            if ibus==0:
                break
            jbus = self.libsteps.api_get_current_device_bus_number(device, recv_side)
            id = self.libsteps.api_get_current_device_identifier(device)
            id = str(id)
            lines.append((int(ibus), int(jbus), id))
            self.libsteps.api_goto_next_device(device)
        return tuple(lines)

    def get_transformers_at_bus(self, bus):
        device = "TRANSFORMER"
        device = self.__get_c_char_p_of_string(device)
        pri_side = self.__get_c_char_p_of_string("PRIMARY")
        sec_side = self.__get_c_char_p_of_string("SECONDARY")
        ter_side = self.__get_c_char_p_of_string("TERTIARY")
        self.libsteps.api_initialize_device_search(device, bus)
        transformers = []
        while True:
            ibus = self.libsteps.api_get_current_device_bus_number(device, pri_side)
            if ibus==0:
                break
            jbus = self.libsteps.api_get_current_device_bus_number(device, sec_side)
            kbus = self.libsteps.api_get_current_device_bus_number(device, ter_side)
            id = self.libsteps.api_get_current_device_identifier(device)
            id = str(id)
            transformers.append((int(ibus), int(jbus), int(kbus), id))
            self.libsteps.api_goto_next_device(device)
        return tuple(transformers)

    def get_hvdcs_at_bus(self, bus):
        device = "HVDC"
        device = self.__get_c_char_p_of_string(device)
        rec_side = self.__get_c_char_p_of_string("RECTIFIER")
        inv_side = self.__get_c_char_p_of_string("INVERTER")
        self.libsteps.api_initialize_device_search(device, bus)
        hvdcs = []
        while True:
            ibus = self.libsteps.api_get_current_device_bus_number(device, rec_side)
            if ibus==0:
                break
            jbus = self.libsteps.api_get_current_device_bus_number(device, inv_side)
            id = self.libsteps.api_get_current_device_identifier(device)
            id = str(id)
            hvdcs.append((int(ibus), int(jbus), id))
            self.libsteps.api_goto_next_device(device)
        return tuple(hvdcs)

    def get_areas(self):
        self.libsteps.api_initialize_area_search()
        areas = []
        while True:
            area = self.libsteps.api_get_current_area_number()
            if area==0:
                break
            areas.append(int(area))
            self.libsteps.api_goto_next_area()
        return tuple(areas)

    def get_zones(self):
        self.libsteps.api_initialize_zone_search()
        zones = []
        while True:
            zone = self.libsteps.api_get_current_zone_number()
            if zone==0:
                break
            zones.append(int(zone))
            self.libsteps.api_goto_next_zone()
        return tuple(zones)

    def get_owners(self):
        self.libsteps.api_initialize_owner_search()
        owners = []
        while True:
            owner = self.libsteps.api_get_current_owner_number()
            if owner==0:
                break
            owners.append(int(owner))
            self.libsteps.api_goto_next_owner()
        return tuple(owners)

    def get_bus_data(self, bus, par_type, par_name):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return None
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return int(self.libsteps.api_get_bus_integer_data(bus, par_name))
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_get_bus_float_data(bus, par_name)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_get_bus_boolean_data(bus, par_name)
        if par_type in ['S', 'STRING']:
            return str(self.libsteps.api_get_bus_string_data(bus, par_name))
        return None

    def __get_source_data(self, source, par_type, par_name):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return None
        bus, ickt = self.__extract_single_bus_device_id(source)
        ickt = self.__get_c_char_p_of_string(ickt)
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return int(self.libsteps.api_get_source_integer_data(bus, ickt, par_name))
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_get_source_float_data(bus, ickt, par_name)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_get_source_boolean_data(bus, ickt, par_name)
        if par_type in ['S', 'STRING']:
            return str(self.libsteps.api_get_source_string_data(bus, ickt, par_name))
        return None

    def get_generator_data(self, generator, par_type, par_name):
        return self.__get_source_data(generator, par_type, par_name)

    def get_wt_generator_data(self, wt_generator, par_type, par_name):
        return self.__get_source_data(wt_generator, par_type, par_name)

    def get_pv_unit_data(self, pv_unit, par_type, par_name):
        return self.__get_source_data(pv_unit, par_type, par_name)

    def get_energy_storage_data(self, energy_storage, par_type, par_name):
        return self.__get_source_data(energy_storage, par_type, par_name)

    def get_load_data(self, load, par_type, par_name):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return None
        bus, ickt = self.__extract_single_bus_device_id(load)
        ickt = self.__get_c_char_p_of_string(ickt)
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return int(self.libsteps.api_get_load_integer_data(bus, ickt, par_name))
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_get_load_float_data(bus, ickt, par_name)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_get_load_boolean_data(bus, ickt, par_name)
        if par_type in ['S', 'STRING']:
            return str(self.libsteps.api_get_load_string_data(bus, ickt, par_name))
        return None

    def get_fixed_shunt_data(self, fixed_shunt, par_type, par_name):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return None
        bus, ickt = self.__extract_single_bus_device_id(fixed_shunt)
        ickt = self.__get_c_char_p_of_string(ickt)
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return int(self.libsteps.api_get_fixed_shunt_integer_data(bus, ickt, par_name))
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_get_fixed_shunt_float_data(bus, ickt, par_name)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_get_fixed_shunt_boolean_data(bus, ickt, par_name)
        if par_type in ['S', 'STRING']:
            return str(self.libsteps.api_get_fixed_shunt_string_data(bus, ickt, par_name))
        return None

    def get_equivalent_device_data(self, equivalent_device, par_type, par_name):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return None
        bus, ickt = self.__extract_single_bus_device_id(equivalent_device)
        ickt = self.__get_c_char_p_of_string(ickt)
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return int(self.libsteps.api_get_equivalent_device_integer_data(bus, ickt, par_name))
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_get_equivalent_device_float_data(bus, ickt, par_name)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_get_equivalent_device_boolean_data(bus, ickt, par_name)
        if par_type in ['S', 'STRING']:
            return str(self.libsteps.api_get_equivalent_device_string_data(bus, ickt, par_name))
        return None

    def get_line_data(self, line, par_type, par_name):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return None
        ibus, jbus, ickt = self.__extract_double_bus_device_id(line)
        ickt = self.__get_c_char_p_of_string(ickt)
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return int(self.libsteps.api_get_line_integer_data(ibus, jbus, ickt, par_name))
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_get_line_float_data(ibus, jbus, ickt, par_name)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_get_line_boolean_data(ibus, jbus, ickt, par_name)
        if par_type in ['S', 'STRING']:
            return str(self.libsteps.api_get_line_string_data(ibus, jbus, ickt, par_name))
        return None

    def get_transformer_data(self, transformer, par_type, side, par_name):
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
            return int(self.libsteps.api_get_transformer_integer_data(ibus, jbus, kbus, ickt, side, par_name))
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_get_transformer_float_data(ibus, jbus, kbus, ickt, side, par_name)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_get_transformer_boolean_data(ibus, jbus, kbus, ickt, side, par_name)
        if par_type in ['S', 'STRING']:
            return str(self.libsteps.api_get_transformer_string_data(ibus, jbus, kbus, ickt, side, par_name))
        return None

    def get_hvdc_data(self, hvdc, par_type, side, par_name):
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
            return int(self.libsteps.api_get_hvdc_integer_data(ibus, jbus, ickt, side, par_name))
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_get_hvdc_float_data(ibus, jbus, ickt, side, par_name)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_get_hvdc_boolean_data(ibus, jbus, ickt, side, par_name)
        if par_type in ['S', 'STRING']:
            return str(self.libsteps.api_get_hvdc_string_data(ibus, jbus, ickt, side, par_name))
        return None

    def get_area_data(self, area, par_type, par_name):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return None
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return int(self.libsteps.api_get_area_integer_data(area, par_name))
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_get_area_float_data(area, par_name)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_get_area_boolean_data(area, par_name)
        if par_type in ['S', 'STRING']:
            return str(self.libsteps.api_get_area_string_data(area, par_name))
        return None

    def get_zone_data(self, zone, par_type, par_name):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return None
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return int(self.libsteps.api_get_zone_integer_data(zone, par_name))
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_get_zone_float_data(zone, par_name)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_get_zone_boolean_data(zone, par_name)
        if par_type in ['S', 'STRING']:
            return str(self.libsteps.api_get_zone_string_data(zone, par_name))
        return None
    
    def get_owner_data(self, owner, par_type, par_name):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return None
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return int(self.libsteps.api_get_owner_integer_data(owner, par_name))
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_get_owner_float_data(owner, par_name)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_get_owner_boolean_data(owner, par_name)
        if par_type in ['S', 'STRING']:
            return str(self.libsteps.api_get_owner_string_data(owner, par_name))
        return None

        
    def set_bus_data(self, bus, par_type, par_name, value):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            self.libsteps.api_set_bus_integer_data(bus, par_name, value)
            return
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_set_bus_float_data(bus, par_name, value)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_set_bus_boolean_data(bus, par_name, value)
        if par_type in ['S', 'STRING']:
            value = self.__get_c_char_p_of_string(value)
            return self.libsteps.api_set_bus_string_data(bus, par_name, value)
        return

    def __set_source_data(self, source, par_type, par_name, value):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return        
        bus, ickt = self.__extract_single_bus_device_id(source)
        ickt = self.__get_c_char_p_of_string(ickt)
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return self.libsteps.api_set_source_integer_data(bus, ickt, par_name, value)
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_set_source_float_data(bus, ickt, par_name, value)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_set_source_boolean_data(bus, ickt, par_name, value)
        if par_type in ['S', 'STRING']:
            value = self.__get_c_char_p_of_string(value)
            return self.libsteps.api_set_source_string_data(bus, ickt, par_name, value)
        return

    def set_generator_data(self, generator, par_type, par_name, value):
        return self.__set_source_data(generator, par_type, par_name, value)

    def set_wt_generator_data(self, wt_generator, par_type, par_name, value):
        return self.__set_source_data(wt_generator, par_type, par_name, value)

    def set_pv_unit_data(self, pv_unit, par_type, par_name, value):
        return self.__set_source_data(pv_unit, par_type, par_name, value)

    def set_energy_storage_data(self, energy_storage, par_type, par_name, value):
        return self.__set_source_data(energy_storage, par_type, par_name, value)

    def set_load_data(self, load, par_type, par_name, value):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return
        bus, ickt = self.__extract_single_bus_device_id(load)
        ickt = self.__get_c_char_p_of_string(ickt)
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return self.libsteps.api_set_load_integer_data(bus, ickt, par_name, value)
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_set_load_float_data(bus, ickt, par_name, value)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_set_load_boolean_data(bus, ickt, par_name, value)
        if par_type in ['S', 'STRING']:
            value = self.__get_c_char_p_of_string(value)
            return self.libsteps.api_set_load_string_data(bus, ickt, par_name, value)
        return

    def set_fixed_shunt_data(self, fixed_shunt, par_type, par_name, value):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return
        bus, ickt = self.__extract_single_bus_device_id(fixed_shunt)
        ickt = self.__get_c_char_p_of_string(ickt)
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return self.libsteps.api_set_fixed_shunt_integer_data(bus, ickt, par_name, value)
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_set_fixed_shunt_float_data(bus, ickt, par_name, value)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_set_fixed_shunt_boolean_data(bus, ickt, par_name, value)
        if par_type in ['S', 'STRING']:
            value = self.__get_c_char_p_of_string(value)
            return self.libsteps.api_set_fixed_shunt_string_data(bus, ickt, par_name, value)
        return

    def set_equivalent_device_data(self, equivalent_device, par_type, par_name, value):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return
        bus, ickt = self.__extract_single_bus_device_id(equivalent_device)
        ickt = self.__get_c_char_p_of_string(ickt)
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return self.libsteps.api_set_equivalent_device_integer_data(bus, ickt, par_name, value)
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_set_equivalent_device_float_data(bus, ickt, par_name, value)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_set_equivalent_device_boolean_data(bus, ickt, par_name, value)
        if par_type in ['S', 'STRING']:
            value = self.__get_c_char_p_of_string(value)
            return self.libsteps.api_set_equivalent_device_string_data(bus, ickt, par_name, value)
        return

    def set_line_data(self, line, par_type, par_name, value):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return
        ibus, jbus, ickt = self.__extract_double_bus_device_id(line)
        ickt = self.__get_c_char_p_of_string(ickt)
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return self.libsteps.api_set_line_integer_data(ibus, jbus, ickt, par_name, value)
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_set_line_float_data(ibus, jbus, ickt, par_name, value)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_set_line_boolean_data(ibus, jbus, ickt, par_name, value)
        if par_type in ['S', 'STRING']:
            value = self.__get_c_char_p_of_string(value)
            return self.libsteps.api_set_line_string_data(ibus, jbus, ickt, par_name, value)
        return

    def set_transformer_data(self, transformer, par_type, side, par_name, value):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return
        ibus, jbus, kbus, ickt = self.__extract_triple_bus_device_id(transformer)
        ickt = self.__get_c_char_p_of_string(ickt)
        side = self.__get_c_char_p_of_string(side)
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return self.libsteps.api_set_transformer_integer_data(ibus, jbus, kbus, ickt, side, par_name, value)
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_set_transformer_float_data(ibus, jbus, kbus, ickt, side, par_name, value)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_set_transformer_boolean_data(ibus, jbus, kbus, ickt, side, par_name, value)
        if par_type in ['S', 'STRING']:
            value = self.__get_c_char_p_of_string(value)
            return self.libsteps.api_set_transformer_string_data(ibus, jbus, kbus, ickt, side, par_name, value)
        return

    def set_hvdc_data(self, hvdc, par_type, side, par_name, value):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return
        ibus, jbus, ickt = self.__extract_double_bus_device_id(hvdc)
        ickt = self.__get_c_char_p_of_string(ickt)
        side = self.__get_c_char_p_of_string(side)
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return self.libsteps.api_set_hvdc_integer_data(ibus, jbus, ickt, side, par_name, value)
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_set_hvdc_float_data(ibus, jbus, ickt, side, par_name, value)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_set_hvdc_boolean_data(ibus, jbus, ickt, side, par_name, value)
        if par_type in ['S', 'STRING']:
            value = self.__get_c_char_p_of_string(value)
            return self.libsteps.api_set_hvdc_string_data(ibus, jbus, ickt, side, par_name, value)
        return

    def set_area_data(self, area, par_type, par_name, value):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return self.libsteps.api_set_area_integer_data(area, par_name, value)
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_set_area_float_data(area, par_name, value)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_set_area_boolean_data(area, par_name, value)
        if par_type in ['S', 'STRING']:
            value = self.__get_c_char_p_of_string(value)
            return self.libsteps.api_set_area_string_data(area, par_name, value)
        return

    def set_zone_data(self, zone, par_type, par_name, value):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return self.libsteps.api_set_zone_integer_data(zone, par_name, value)
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_set_zone_float_data(zone, par_name, value)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_set_zone_boolean_data(zone, par_name, value)
        if par_type in ['S', 'STRING']:
            value = self.__get_c_char_p_of_string(value)
            return self.libsteps.api_set_zone_string_data(zone, par_name, value)
        return

    def set_owner_data(self, owner, par_type, par_name, value):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return self.libsteps.api_set_owner_integer_data(owner, par_name, value)
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_set_owner_float_data(owner, par_name, value)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_set_owner_boolean_data(owner, par_name, value)
        if par_type in ['S', 'STRING']:
            value = self.__get_c_char_p_of_string(value)
            return self.libsteps.api_set_owner_string_data(owner, par_name, value)
        return

    def set_dynamic_model(self, data, file_type):
        data = self.__get_c_char_p_of_string(data)
        file_type = self.__get_c_char_p_of_string(file_type)        
        self.libsteps.api_set_dynamic_model(data, file_type)
        return
    
    def get_generator_related_model_name(self, generator, model_type):
        ibus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        model_type = self.__get_c_char_p_of_string(model_type)
        return str(self.libsteps.api_get_generator_related_model_name(ibus, ickt, model_type))

    def get_generator_related_model_data(self, generator, model_type, par_name):
        ibus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        model_type = self.__get_c_char_p_of_string(model_type)
        par_name = self.__get_c_char_p_of_string(par_name)
        return self.libsteps.api_get_generator_related_model_float_parameter(ibus, ickt, model_type, par_name)
        
    def set_generator_related_model_data(self, generator, model_type, par_name, value):
        ibus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        model_type = self.__get_c_char_p_of_string(model_type)
        par_name = self.__get_c_char_p_of_string(par_name)
        return self.libsteps.api_set_generator_related_model_float_parameter(ibus, ickt, model_type, par_name, value)
        
    def get_wt_generator_related_model_name(self, generator, model_type):
        ibus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        model_type = self.__get_c_char_p_of_string(model_type)
        return str(self.libsteps.api_get_wt_generator_related_model_name(ibus, ickt, model_type))

    def get_wt_generator_related_model_data(self, generator, model_type, par_name):
        ibus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        model_type = self.__get_c_char_p_of_string(model_type)
        par_name = self.__get_c_char_p_of_string(par_name)
        return self.libsteps.api_get_wt_generator_related_model_float_parameter(ibus, ickt, model_type, par_name)
        
    def set_wt_generator_related_model_data(self, generator, model_type, par_name, value):
        ibus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        model_type = self.__get_c_char_p_of_string(model_type)
        par_name = self.__get_c_char_p_of_string(par_name)
        return self.libsteps.api_set_wt_generator_related_model_float_parameter(ibus, ickt, model_type, par_name, value)
        
    def get_pv_unit_related_model_name(self, pv_unit, model_type):
        ibus, ickt = self.__extract_single_bus_device_id(pv_unit)
        ickt = self.__get_c_char_p_of_string(ickt)
        model_type = self.__get_c_char_p_of_string(model_type)
        return str(self.libsteps.api_get_pv_unit_related_model_name(ibus, ickt, model_type))

    def get_pv_unit_related_model_data(self, pv_unit, model_type, par_name):
        ibus, ickt = self.__extract_single_bus_device_id(pv_unit)
        ickt = self.__get_c_char_p_of_string(ickt)
        model_type = self.__get_c_char_p_of_string(model_type)
        par_name = self.__get_c_char_p_of_string(par_name)
        return self.libsteps.api_get_pv_unit_related_model_float_parameter(ibus, ickt, model_type, par_name)
        
    def set_pv_unit_related_model_data(self, pv_unit, model_type, par_name, value):
        ibus, ickt = self.__extract_single_bus_device_id(pv_unit)
        ickt = self.__get_c_char_p_of_string(ickt)
        model_type = self.__get_c_char_p_of_string(model_type)
        par_name = self.__get_c_char_p_of_string(par_name)
        return self.libsteps.api_set_pv_unit_related_model_float_parameter(ibus, ickt, model_type, par_name, value)
        
    def get_powerflow_solver_parameter(self, par_type, par_name):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN']:
            return None
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return int(self.libsteps.api_get_powerflow_solver_integer_parameter(par_name))
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_get_powerflow_solver_float_parameter(par_name)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_get_powerflow_solver_boolean_parameter(par_name)
        return None

    def set_powerflow_solver_parameter(self, par_type, par_name, value):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN']:
            return
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return self.libsteps.api_set_powerflow_solver_integer_parameter(par_name, value)
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_set_powerflow_solver_float_parameter(par_name, value)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_set_powerflow_solver_boolean_parameter(par_name, value)
        return
        
    def solve_powerflow(self, method):
        method = self.__get_c_char_p_of_string(method)
        self.libsteps.api_solve_powerflow(method)
        return

    def is_powerflow_converged(self):
        return self.libsteps.api_is_powerflow_converged()
    
    def get_powerflow_loss(self):
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
        self.libsteps.api_show_powerflow_result()
        return
        
    def save_powerflow_result(self, file):
        file = self.__get_c_char_p_of_string(file)
        self.libsteps.api_save_powerflow_result(file)
        return
    
    def save_extended_powerflow_result(self, file):
        file = self.__get_c_char_p_of_string(file)
        self.libsteps.api_save_extended_powerflow_result(file)
        return
        
    def save_jacobian_matrix(self, file):
        file = self.__get_c_char_p_of_string(file)
        self.libsteps.api_save_jacobian_matrix(file)
        return

    def build_network_matrix(self):
        self.libsteps.api_build_network_matrix()
        return

    def build_decoupled_network_matrix(self):
        self.libsteps.api_build_decoupled_network_matrix()
        return

    def build_dc_network_matrix(self):
        self.libsteps.api_build_dc_network_matrix()
        return

    def build_dynamic_network_matrix(self):
        self.libsteps.api_build_dynamic_network_matrix()
        return

    def save_network_matrix(self, file):
        file = self.__get_c_char_p_of_string(file)
        self.libsteps.api_save_network_matrix(file)
        return
        
    def save_decoupled_network_matrix(self, file):
        file = self.__get_c_char_p_of_string(file)
        self.libsteps.api_save_decoupled_network_matrix(file)
        return
        
    def save_dc_network_matrix(self, file):
        file = self.__get_c_char_p_of_string(file)
        self.libsteps.api_save_dc_network_matrix(file)
        return
        
    def save_dynamic_network_matrix(self, file):
        file = self.__get_c_char_p_of_string(file)
        self.libsteps.api_save_dynamic_network_matrix(file)
        return
        
        
    def get_dynamic_simulator_parameter(self, par_type, par_name):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return None
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return int(self.libsteps.api_get_dynamic_simulator_integer_parameter(par_name))
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_get_dynamic_simulator_float_parameter(par_name)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_get_dynamic_simulator_boolean_parameter(par_name)
        if par_type in ['S', 'STRING']:
            return str(self.libsteps.api_get_dynamic_simulator_string_parameter(par_name))
        return None

    def set_dynamic_simulator_parameter(self, par_type, par_name, value):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return None
        par_name = self.__get_c_char_p_of_string(par_name)
        if par_type in ['I', 'INT', 'INTEGER']:
            return self.libsteps.api_set_dynamic_simulator_integer_parameter(par_name, value)
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_set_dynamic_simulator_float_parameter(par_name, value)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_set_dynamic_simulator_boolean_parameter(par_name, value)
        if par_type in ['S', 'STRING']:
            value = self.__get_c_char_p_of_string(value)
            return self.libsteps.api_set_dynamic_simulator_string_parameter(par_name, value)
        return None
        
    def get_dynamic_simulator_output_file(self):
        return str(self.libsteps.api_get_dynamic_simulator_output_file())
        
    def set_dynamic_simulator_output_file(self, file):
        file = self.__get_c_char_p_of_string(file)
        return self.libsteps.api_set_dynamic_simulator_output_file(file)
        
    def get_dynamic_simulation_time_step(self):
        return self.libsteps.api_get_dynamic_simulation_time_step()
        
    def set_dynamic_simulation_time_step(self, step):
        return self.libsteps.api_set_dynamic_simulation_time_step(step)
        
    def get_dynamic_simulation_time(self):
        return self.libsteps.api_get_dynamic_simulation_time()
        
    def prepare_meters(self, device_type):
        device_type = device_type.upper()
        if device_type not in ['ALL', 'BUS', 'GENERATOR', 'WT GENERATOR', 'LOAD', 'LINE', 'HVDC', 'EQUIVALENT DEVICE']:
            return
        if device_type in ['ALL']:
            self.libsteps.api_prepare_meters()
            return
        if device_type in ['BUS']:
            self.libsteps.api_prepare_bus_related_meters()
            return
        if device_type in ['GENERATOR']:
            self.libsteps.api_prepare_generator_related_meters()
            return
        if device_type in ['WT GENERATOR']:
            self.libsteps.api_prepare_wt_generator_related_meters()
            return
        if device_type in ['PV UNIT']:
            self.libsteps.api_prepare_pv_unit_related_meters()
            return
        if device_type in ['ENERGY STORAGE']:
            self.libsteps.api_prepare_energy_storage_related_meters()
            return
        if device_type in ['LOAD']:
            self.libsteps.api_prepare_load_related_meters()
            return
        if device_type in ['LINE']:
            self.libsteps.api_prepare_line_related_meters()
            return
        if device_type in ['HVDC']:
            self.libsteps.api_prepare_hvdc_related_meters()
            return
        if device_type in ['EQUIVALENT DEVICE']:
            self.libsteps.api_prepare_equivalent_device_related_meters()
            return
        return

    def prepare_bus_meter(self, bus, meter_type):
        meter_type = self.__get_c_char_p_of_string(meter_type)
        self.libsteps.api_prepare_bus_related_meter(bus, meter_type)
        return

    def prepare_generator_meter(self, generator, meter_type, var_name=""):
        bus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        meter_type = self.__get_c_char_p_of_string(meter_type)
        self.libsteps.api_prepare_generator_related_meter(bus, ickt, meter_type, var_name)
        return

    def prepare_wt_generator_meter(self, generator, meter_type, var_name=""):
        bus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        meter_type = self.__get_c_char_p_of_string(meter_type)
        self.libsteps.api_prepare_wt_generator_related_meter(bus, ickt, meter_type, var_name)
        return

    def prepare_pv_unit_meter(self, pvunit, meter_type, var_name=""):
        bus, ickt = self.__extract_single_bus_device_id(pvunit)
        ickt = self.__get_c_char_p_of_string(ickt)
        meter_type = self.__get_c_char_p_of_string(meter_type)
        self.libsteps.api_prepare_pv_unit_related_meter(bus, ickt, meter_type, var_name)
        return

    def prepare_energy_storage_meter(self, estorage, meter_type, var_name=""):
        bus, ickt = self.__extract_single_bus_device_id(estorage)
        ickt = self.__get_c_char_p_of_string(ickt)
        meter_type = self.__get_c_char_p_of_string(meter_type)
        self.libsteps.api_prepare_energy_storage_related_meter(bus, ickt, meter_type, var_name)
        return

    def prepare_load_meter(self, load, meter_type, var_name=""):
        bus, ickt = self.__extract_single_bus_device_id(load)
        ickt = self.__get_c_char_p_of_string(ickt)
        meter_type = self.__get_c_char_p_of_string(meter_type)
        self.libsteps.api_prepare_load_related_meter(bus, ickt, meter_type, var_name)
        return

    def prepare_line_meter(self, line, meter_type, side, var_name=""):
        ibus, jbus, ickt = self.__extract_double_bus_device_id(line)
        ickt = self.__get_c_char_p_of_string(ickt)
        meter_type = self.__get_c_char_p_of_string(meter_type)
        side = self.__get_c_char_p_of_string(side)
        self.libsteps.api_prepare_line_related_meter(ibus, jbus, ickt, meter_type, side, var_name)
        return

    def prepare_hvdc_meter(self, hvdc, meter_type, side, var_name=""):
        ibus, jbus, ickt = self.__extract_double_bus_device_id(hvdc)
        ickt = self.__get_c_char_p_of_string(ickt)
        meter_type = self.__get_c_char_p_of_string(meter_type)
        side = self.__get_c_char_p_of_string(side)
        self.libsteps.api_prepare_hvdc_related_meter(ibus, jbus, ickt, meter_type, side, var_name)
        return

    def prepare_equivalent_device_meter(self, edevice, meter_type, var_name=""):
        bus, ickt = self.__extract_single_bus_device_id(equivalent_device)
        ickt = self.__get_c_char_p_of_string(ickt)
        meter_type = self.__get_c_char_p_of_string(meter_type)
        self.libsteps.api_prepare_equivalent_device_related_meter(bus, ickt, meter_type, var_name)
        return

    def start_dynamic_simulation(self):
        self.libsteps.api_start_dynamic_simulation()
        return

    def stop_dynamic_simulation(self):
        self.libsteps.api_stop_dynamic_simulation()
        return

    def run_dynamic_simulation_to_time(self, time):
        self.libsteps.api_run_simulation_to_time(time)
        return

    def run_a_step(self):
        self.libsteps.api_run_a_step()
        return

    def set_bus_fault(self, bus, fault_type, fault_shunt):
        fault_type = self.__get_c_char_p_of_string(fault_type)
        self.libsteps.api_set_bus_fault(bus, fault_type, fault_shunt[0], fault_shunt[1])
        return

    def clear_bus_fault(self, bus, fault_type):
        fault_type = self.__get_c_char_p_of_string(fault_type)
        self.libsteps.api_clear_bus_fault(bus, fault_type)
        return

    def trip_bus(self, bus):
        self.libsteps.api_trip_bus(bus)
        return

    def set_line_fault(self, line, fault_type, fault_location, fault_shunt):
        ibus, jbus, ickt = self.__extract_double_bus_device_id(line)
        ickt = self.__get_c_char_p_of_string(ickt)
        fault_type = self.__get_c_char_p_of_string(fault_type)
        self.libsteps.api_set_line_fault(ibus, jbus, ickt, fault_type, fault_location, fault_shunt[0], fault_shunt[1])
        return

    def clear_line_fault(self, line, fault_type, fault_location):
        ibus, jbus, ickt = self.__extract_double_bus_device_id(line)
        ickt = self.__get_c_char_p_of_string(ickt)
        fault_type = self.__get_c_char_p_of_string(fault_type)
        self.libsteps.api_clear_line_fault(ibus, jbus, ickt, fault_type, fault_location)
        return

    def trip_line(self, line):
        ibus, jbus, ickt = self.__extract_double_bus_device_id(line)
        ickt = self.__get_c_char_p_of_string(ickt)
        self.libsteps.api_trip_line(ibus, jbus, ickt)
        return

    def trip_line_breaker(self, line):
        ibus, jbus, ickt = self.__extract_double_bus_device_id(line)
        ickt = self.__get_c_char_p_of_string(ickt)
        self.libsteps.api_trip_line_breaker(ibus, jbus, ickt)
        return
        

    def close_line(self, line):
        ibus, jbus, ickt = self.__extract_double_bus_device_id(line)
        ickt = self.__get_c_char_p_of_string(ickt)
        self.libsteps.api_close_line(ibus, jbus, ickt)
        return

    def close_line_breaker(self, line):
        ibus, jbus, ickt = self.__extract_single_bus_device_id(line)
        ickt = self.__get_c_char_p_of_string(ickt)
        self.libsteps.api_close_line_breaker(ibus, jbus, ickt)
        return
        

    def trip_transformer(self, transformer):
        ibus, jbus, kbus, ickt = self.__extract_triple_bus_device_id(transformer)
        ickt = self.__get_c_char_p_of_string(ickt)
        self.libsteps.api_trip_transformer(ibus, jbus, kbus, ickt)
        return

    def trip_transformer_breaker(self, transformer):
        ibus, jbus, kbus, ickt = self.__extract_triple_bus_device_id(transformer)
        ickt = self.__get_c_char_p_of_string(ickt)
        self.libsteps.api_trip_transformer_breaker(ibus, jbus, kbus, ickt)
        return
        

    def close_transformer(self, transformer):
        ibus, jbus, kbus, ickt = self.__extract_triple_bus_device_id(transformer)
        ickt = self.__get_c_char_p_of_string(ickt)
        self.libsteps.api_close_transformer(ibus, jbus, kbus, ickt)
        return

    def close_transformer_breaker(self, transformer):
        ibus, jbus, kbus, ickt = self.__extract_triple_bus_device_id(transformer)
        ickt = self.__get_c_char_p_of_string(ickt)
        self.libsteps.api_close_transformer_breaker(ibus, jbus, kbus, ickt)
        return

    def trip_generator(self, generator):
        ibus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        self.libsteps.api_trip_generator(ibus, ickt)
        return

    def shed_generator(self, generator, percent):
        ibus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        self.libsteps.api_shed_generator(ibus, ickt, percent)
        return

    def trip_load(self, load):
        ibus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        self.libsteps.api_trip_load(ibus, ickt)
        return

    def close_load(self, load):
        ibus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        self.libsteps.api_close_load(ibus, ickt)
        return

    def scale_load(self, load, percent):
        ibus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        self.libsteps.api_scale_load(ibus, ickt, percent)
        return

    def scale_all_loads(self, percent):
        self.libsteps.api_scale_all_loads(percent)
        return

    def trip_fixed_shunt(self, shunt):
        ibus, ickt = self.__extract_single_bus_device_id(shunt)
        ickt = self.__get_c_char_p_of_string(ickt)
        self.libsteps.api_trip_fixed_shunt(ibus, ickt)
        return

    def close_fixed_shunt(self, shunt):
        ibus, ickt = self.__extract_single_bus_device_id(shunt)
        ickt = self.__get_c_char_p_of_string(ickt)
        self.libsteps.api_close_fixed_shunt(ibus, ickt)
        return

    def manually_bypass_hvdc(self, hvdc):
        ibus, jbus, ickt = self.__extract_double_bus_device_id(hvdc)
        ickt = self.__get_c_char_p_of_string(ickt)
        self.libsteps.api_manually_bypass_hvdc(ibus, jbus, ickt)
        return

    def manually_block_hvdc(self, hvdc):
        ibus, jbus, ickt = self.__extract_double_bus_device_id(hvdc)
        ickt = self.__get_c_char_p_of_string(ickt)
        self.libsteps.api_manually_block_hvdc(ibus, jbus, ickt)
        return

    def manually_unbypass_hvdc(self, hvdc):
        ibus, jbus, ickt = self.__extract_double_bus_device_id(hvdc)
        ickt = self.__get_c_char_p_of_string(ickt)
        self.libsteps.api_manually_unbypass_hvdc(ibus, jbus, ickt)
        return

    def manually_unblock_hvdc(self, hvdc):
        ibus, jbus, ickt = self.__extract_double_bus_device_id(hvdc)
        ickt = self.__get_c_char_p_of_string(ickt)
        self.libsteps.api_manually_unblock_hvdc(ibus, jbus, ickt)
        return

    def get_generator_voltage_reference_in_pu(self, generator):
        ibus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        return self.libsteps.api_get_generator_voltage_reference_in_pu(ibus, ickt)

    def get_generator_power_reference_in_MW(self, generator):
        ibus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        return self.libsteps.api_get_generator_power_reference_in_MW(ibus, ickt)

    def set_generator_voltage_reference_in_pu(self, generator, value):
        ibus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        return self.libsteps.api_set_generator_voltage_reference_in_pu(ibus, ickt, value)

    def set_generator_power_reference_in_MW(self, generator, value):
        ibus, ickt = self.__extract_single_bus_device_id(generator)
        ickt = self.__get_c_char_p_of_string(ickt)
        return self.libsteps.api_set_generator_power_reference_in_MW(ibus, ickt, value)

class pSTEPS(STEPS):
    def __init__(self):
        STEPS.__init__(self, True)
        #print("Parallel STEPS simulator is created:", self)

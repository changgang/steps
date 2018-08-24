pysteps_version = "0.5"
pysteps_date = "2018/08/23"
pysteps_author = (("Changgang Li", "lichangang@sdu.edu.cn"),("Yue Wu","sduyuewu2018@163.com"))

import pylibsteps

class STEPS():
    def __init__(self, parallel_flag = False):        
        self.libsteps = pylibsteps.load_library(parallel_flag)
        if self.libsteps is None:
            return None
        
        self.libsteps.api_initialize_package()
        if parallel_flag==False:
            print("STEPS simulator is created:", self)
        
    def __del__(self):
        if self.libsteps is None:
            return None
        self.clear_package()
        print("STEPS simulator is deleted")

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

    def clear_package(self):
        self.libsteps.api_clear_package()
        return

    def terminate_package(self):
        self.libsteps.api_terminate_package()
        return

    def get_package_float_data(self, dataname):
        return self.libsteps.api_get_package_float_data(dataname)        

    def set_package_float_data(self, dataname, value):
        return self.libsteps.api_set_package_float_data(dataname, value)

    def get_allowed_maximum_bus_number(self):
        return int(self.libsteps.api_get_allowed_maximum_bus_number())

    def set_allowed_maximum_bus_number(self, max_bus_number):
        self.libsteps.api_set_allowed_maximum_bus_number(max_bus_number)

    def load_powerflow_data(self, file, type):
        self.libsteps.api_load_powerflow_data_from_file(file, type)

    def save_powerflow_data(self, file, type):
        self.libsteps.api_save_powerflow_data_to_file(file, type)
        
    def load_dynamic_data(self, file, type):
        self.libsteps.api_load_dynamic_data_from_file(file, type)
        
    def save_dynamic_data(self, file, type):
        self.libsteps.api_save_dynamic_data_from_file(file, type)

    def get_bus_capacity(self):
        return int(self.libsteps.api_get_device_capacity("BUS"))
    def get_generator_capacity(self):
        return int(self.libsteps.api_get_device_capacity("Generator"))
    def get_wt_generator_capacity(self):
        return int(self.libsteps.api_get_device_capacity("WT Generator"))
    def get_pv_unit_capacity(self):
        return int(self.libsteps.api_get_device_capacity("PV Unit"))
    def get_load_capacity(self):
        return int(self.libsteps.api_get_device_capacity("Load"))
    def get_fixed_shunt_capacity(self):
        return int(self.libsteps.api_get_device_capacity("Fixed Shunt"))
    def get_line_capacity(self):
        return int(self.libsteps.api_get_device_capacity("Line"))
    def get_transformer_capacity(self):
        return int(self.libsteps.api_get_device_capacity("Transformer"))
    def get_hvdc_capacity(self):
        return int(self.libsteps.api_get_device_capacity("HVDC"))
    def get_equivalent_device_capacity(self):
        return int(self.libsteps.api_get_device_capacity("Equivalent Device"))
    def get_energy_storage_capacity(self):
        return int(self.libsteps.api_get_device_capacity("Energy Storage"))
    def get_area_capacity(self):
        return int(self.libsteps.api_get_area_capacity())
    def get_zone_capacity(self):
        return int(self.libsteps.api_get_zone_capacity())
    def get_owner_capacity(self):
        return int(self.libsteps.api_get_owner_capacity())

    def set_bus_capacity(self, capacity):
        return self.libsteps.api_set_device_capacity("BUS", capacity)
    def set_generator_capacity(self, capacity):
        return self.libsteps.api_set_device_capacity("Generator", capacity)
    def set_wt_generator_capacity(self, capacity):
        return self.libsteps.api_set_device_capacity("Wt Generator", capacity)
    def set_pv_unit_capacity(self, capacity):
        return self.libsteps.api_set_device_capacity("PV Unit", capacity)
    def set_load_capacity(self, capacity):
        return self.libsteps.api_set_device_capacity("Load", capacity)
    def set_fixed_shunt_capacity(self, capacity):
        return self.libsteps.api_set_device_capacity("Fixed Shunt", capacity)
    def set_line_capacity(self, capacity):
        return self.libsteps.api_set_device_capacity("Line", capacity)
    def set_transformer_capacity(self, capacity):
        return self.libsteps.api_set_device_capacity("Transformer", capacity)
    def set_hvdc_capacity(self, capacity):
        return self.libsteps.api_set_device_capacity("HVDC", capacity)
    def set_equivalent_device_capacity(self, capacity):
        return self.libsteps.api_set_device_capacity("Equivalent Device", capacity)
    def set_energy_storage_capacity(self, capacity):
        return self.libsteps.api_set_device_capacity("Energy Storage", capacity)
    def set_area_capacity(self, capacity):
        return self.libsteps.api_set_area_capacity(capacity)
    def set_zone_capacity(self, capacity):
        return self.libsteps.api_set_zone_capacity(capacity)
    def set_owner_capacity(self, capacity):
        return self.libsteps.api_set_owner_capacity(capacity)


    def add_bus(self, busnumber, busname, basevoltage):
        self.libsteps.api_add_bus(busnumber, busname, basevoltage)
        return

    def add_generator(self, generator):
        bus, ickt = self.__extract_single_bus_device_id(generator)
        self.libsteps.api_add_generator(bus, ickt)
        return

    def add_wt_generator(self, generator):
        bus, ickt = self.__extract_single_bus_device_id(generator)
        self.libsteps.api_add_wt_generator(bus, ickt)

    def add_pv_unit(self, unit):
        bus, ickt = self.__extract_single_bus_device_id(unit)
        self.libsteps.api_add_pv_unit(bus, ickt)
        return

    def add_load(self, load):
        bus, ickt = self.__extract_single_bus_device_id(load)
        self.libsteps.api_add_load(bus, ickt)
        return

    def add_fixed_shunt(self, shunt):
        bus, ickt = self.__extract_single_bus_device_id(shunt)
        self.libsteps.api_add_fixed_shunt(bus, ickt)
        return

    def add_line(self, line):
        ibus, jbus, ickt = self.__extract_double_bus_device_id(line)
        self.libsteps.api_add_line(ibus, jbus, ickt)
        return

    def add_hvdc(self, hvdc):
        ibus, jbus, ickt = self.__extract_double_bus_device_id(hvdc)
        self.libsteps.api_add_hvdc(ibus, jbus, ickt)
        return

    def add_transformer(self, transformer):
        ibus, jbus, kbus, ickt = self.__extract_triple_bus_device_id(transformer)            
        self.libsteps.api_add_transformer(ibus, jbus, kbus, ickt)
        return

    def add_equivalent_device(self, device):
        bus, ickt = self.__extract_single_bus_device_id(device)
        self.libsteps.api_add_equivalent_device(bus, ickt)
        return

    def add_energy_storage(self, storage):
        bus, ickt = self.__extract_single_bus_device_id(storage)
        self.libsteps.api_add_energy_storage(bus, ickt)
        return

    def add_area(self, areanumber, areaname):
        self.libsteps.api_add_area(areanumber, areaname)
        return

    def add_zone(self, zonenumber, zonename):
        self.libsteps.api_add_zone(zonenumber, zonename)
        return

    def add_owner(self, ownernumber, ownername):
        self.libsteps.api_add_owner(ownernumber, ownername)
        return

    def get_bus_count(self):
        return self.libsteps.api_get_device_count("BUS")
    def get_generator_count(self):
        return self.libsteps.api_get_device_count("Generator")
    def get_wt_generator_count(self):
        return self.libsteps.api_get_device_count("WT Generator")
    def get_pv_unit_count(self):
        return self.libsteps.api_get_device_count("PV Unit")
    def get_load_count(self):
        return self.libsteps.api_get_device_count("Load")
    def get_fixed_shunt_count(self):
        return self.libsteps.api_get_device_count("Fixed Shunt")
    def get_line_count(self):
        return self.libsteps.api_get_device_count("Line")
    def get_transformer_count(self):
        return self.libsteps.api_get_device_count("Transformer")
    def get_hvdc_count(self):
        return self.libsteps.api_get_device_count("HVDC")
    def get_equivalent_device_count(self):
        return self.libsteps.api_get_device_count("Equivalent Device")
    def get_energy_storage_count(self):
        return self.libsteps.api_get_device_count("Energy Storage")
    def get_area_count(self):
        return self.libsteps.api_get_area_count()
    def get_zone_count(self):
        return self.libsteps.api_get_zone_count()
    def get_owner_count(self):
        return self.libsteps.api_get_owner_count()

    def is_bus_exist(self, bus):
        return self.libsteps.api_is_bus_exist(bus)

    def is_generator_exist(self, generator):
        bus, ickt = self.__extract_single_bus_device_id(generator)
        return self.libsteps.api_is_generator_exist(bus, ickt)

    def is_wt_generator_exist(self, generator):
        bus, ickt = self.__extract_single_bus_device_id(generator)
        return self.libsteps.api_is_wt_generator_exist(bus, ickt)

    def is_pv_unit_exist(self, pv_unit):
        bus, ickt = self.__extract_single_bus_device_id(pv_unit)
        return self.libsteps.api_is_pv_unit_exist(bus, ickt)

    def is_load_exist(self, load):
        bus, ickt = self.__extract_single_bus_device_id(load)
        return self.libsteps.api_is_load_exist(bus, ickt)

    def is_fixed_shunt_exist(self, shunt):
        bus, ickt = self.__extract_single_bus_device_id(shunt)
        return self.libsteps.api_is_fixed_shunt_exist(bus, ickt)

    def is_line_exist(self, line):
        ibus, jbus, ickt = self.__extract_double_bus_device_id(line)
        return self.libsteps.api_is_line_exist(ibus, jbus, ickt)

    def is_transformer_exist(self, transformer):
        ibus, jbus, kbus, ickt = self.__extract_triple_bus_device_id(transformer)
        return self.libsteps.api_is_transformer_exist(ibus, jbus, kbus, ickt)

    def is_hvdc_exist(self, hvdc):
        ibus, jbus, ickt = self.__extract_double_bus_device_id(hvdc)
        return self.libsteps.api_is_hvdc_exist(ibus, jbus, ickt)

    def is_equivalent_device_exist(self, equivalent_device):
        bus = equivalent_device[0]
        ickt = equivalent_device[1]
        return self.libsteps.api_is_equivalent_device_exist(bus, ickt)

    def is_energy_storage_exist(self, energy_storage):
        bus = energy_storage[0]
        ickt = energy_storage[1]
        return self.libsteps.api_is_energy_storage_exist(bus, ickt)
    
    def bus_name2number(self, name):
        return self.libsteps.api_bus_name2bus_number(name)

    def bus_number2nane(self, bus):
        return self.libsteps.api_bus_number2bus_name(bus)

    def get_all_buses(self):
        self.libsteps.api_initialize_all_bus_search()
        buses = []
        while True:
            bus = self.libsteps.api_get_current_bus_number()
            if bus==0:
                break
            buses.append(bus)
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
            buses.append(bus)
            self.libsteps.api_goto_next_bus()
        return tuple(buses)
        
    def get_generators_at_bus(self, bus):
        self.libsteps.api_initialize_device_search("GENERATOR", bus)
        generators = []
        while True:
            bus = self.libsteps.api_get_current_device_bus_number("GENERATOR", "")
            if bus==0:
                break
            id = self.libsteps.api_get_current_device_identifier("GENERATOR")
            generators.append((bus, id))
            self.libsteps.api_goto_next_device("GENERATOR")
        return tuple(generators)
        
    def get_wt_generators_at_bus(self, bus):
        self.libsteps.api_initialize_device_search("WT GENERATOR", bus)
        wt_generators = []
        while True:
            bus = self.libsteps.api_get_current_device_bus_number("WT GENERATOR", "")
            if bus==0:
                break
            id = self.libsteps.api_get_current_device_identifier("WT GENERATOR")
            wt_generators.append((bus, id))
            self.libsteps.api_goto_next_device("WT GENERATOR")
        return tuple(wt_generators)
        
    def get_pv_units_at_bus(self, bus):
        self.libsteps.api_initialize_device_search("PV UNIT", bus)
        pv_units = []
        while True:
            bus = self.libsteps.api_get_current_device_bus_number("PV UNIT", "")
            if bus==0:
                break
            id = self.libsteps.api_get_current_device_identifier("PV UNIT")
            pv_units.append((bus, id))
            self.libsteps.api_goto_next_device("PV UNIT")
        return tuple(pv_units)
        
    def get_loads_at_bus(self, bus):
        self.libsteps.api_initialize_device_search("LOAD", bus)
        loads = []
        while True:
            bus = self.libsteps.api_get_current_device_bus_number("LOAD", "")
            if bus==0:
                break
            id = self.libsteps.api_get_current_device_identifier("LOAD")
            loads.append((bus, id))
            self.libsteps.api_goto_next_device("LOAD")
        return tuple(loads)
        
    def get_fixed_shunts_at_bus(self, bus):
        self.libsteps.api_initialize_device_search("FIXED SHUNT", bus)
        fixed_shunts = []
        while True:
            bus = self.libsteps.api_get_current_device_bus_number("FIXED SHUNT", "")
            if bus==0:
                break
            id = self.libsteps.api_get_current_device_identifier("FIXED SHUNT")
            fixed_shunts.append((bus, id))
            self.libsteps.api_goto_next_device("FIXED SHUNT")
        return tuple(fixed_shunts)
        
    def get_equivalent_devices_at_bus(self, bus):
        self.libsteps.api_initialize_device_search("EQUIVALENT DEVICE", bus)
        equivalent_devices = []
        while True:
            bus = self.libsteps.api_get_current_device_bus_number("EQUIVALENT DEVICE", "")
            if bus==0:
                break
            id = self.libsteps.api_get_current_device_identifier("EQUIVALENT DEVICE")
            equivalent_devices.append((bus, id))
            self.libsteps.api_goto_next_device("EQUIVALENT DEVICE")
        return tuple(equivalent_devices)
        
    def get_energy_storages_at_bus(self, bus):
        self.libsteps.api_initialize_device_search("ENERGY STORAGE", bus)
        energy_storages = []
        while True:
            bus = self.libsteps.api_get_current_device_bus_number("ENERGY STORAGE", "")
            if bus==0:
                break
            id = self.libsteps.api_get_current_device_identifier("ENERGY STORAGE")
            energy_storages.append((bus, id))
            self.libsteps.api_goto_next_device("ENERGY STORAGE")
        return tuple(energy_storages)

    def get_lines_at_bus(self, bus):
        self.libsteps.api_initialize_device_search("LINE", bus)
        lines = []
        while True:
            ibus = self.libsteps.api_get_current_device_bus_number("LINE", "SEND")
            if ibus==0:
                break
            jbus = self.libsteps.api_get_current_device_bus_number("LINE", "RECEIVE")
            id = self.libsteps.api_get_current_device_identifier("LINE")
            lines.append((ibus, jbus, id))
            self.libsteps.api_goto_next_device("LINE")
        return tuple(lines)

    def get_transformers_at_bus(self, bus):
        self.libsteps.api_initialize_device_search("TRANSFORMER", bus)
        transformers = []
        while True:
            ibus = self.libsteps.api_get_current_device_bus_number("TRANSFORMER", "PRIMARY")
            if ibus==0:
                break
            jbus = self.libsteps.api_get_current_device_bus_number("TRANSFORMER", "SECONDARY")
            kbus = self.libsteps.api_get_current_device_bus_number("TRANSFORMER", "TERTIARY")
            id = self.libsteps.api_get_current_device_identifier("TRANSFORMER")
            transformers.append((ibus, jbus, kbus, id))
            self.libsteps.api_goto_next_device("TRANSFORMER")
        return tuple(transformers)

    def get_hvdcs_at_bus(self, bus):
        self.libsteps.api_initialize_device_search("HVDC", bus)
        hvdcs = []
        while True:
            ibus = self.libsteps.api_get_current_device_bus_number("HVDC", "RECTIFIER")
            if ibus==0:
                break
            jbus = self.libsteps.api_get_current_device_bus_number("HVDC", "INVERTER")
            id = self.libsteps.api_get_current_device_identifier("HVDC")
            hvdcs.append((ibus, jbus, id))
            self.libsteps.api_goto_next_device("HVDC")
        return tuple(hvdcs)

    def get_areas(self):
        self.libsteps.api_initialize_area_search()
        areas = []
        while True:
            area = self.libsteps.api_get_current_area_number()
            if area==0:
                break
            areas.append(area)
            self.libsteps.api_goto_next_area()
        return tuple(areas)

    def get_zones(self):
        self.libsteps.api_initialize_zone_search()
        zones = []
        while True:
            zone = self.libsteps.api_get_current_zone_number()
            if zone==0:
                break
            zones.append(zone)
            self.libsteps.api_goto_next_zone()
        return tuple(zones)

    def get_owners(self):
        self.libsteps.api_initialize_owner_search()
        owners = []
        while True:
            owner = self.libsteps.api_get_current_owner_number()
            if owner==0:
                break
            owners.append(owner)
            self.libsteps.api_goto_next_owner()
        return tuple(owners)

    def get_bus_data(self, bus, par_type, par_name):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return None
        if par_type in ['I', 'INT', 'INTEGER']:
            return int(self.libsteps.api_get_bus_integer_data(bus, par_name))
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_get_bus_float_data(bus, par_name)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_get_bus_boolean_data(bus, par_name)
        if par_type in ['S', 'STRING']:
            return self.libsteps.api_get_bus_string_data(bus, par_name)
        return None

    def __get_source_data(self, source, par_type, par_name):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return None
        bus, ickt = self.__extract_single_bus_device_id(source)
        if par_type in ['I', 'INT', 'INTEGER']:
            return int(self.libsteps.api_get_source_integer_data(bus, ickt, par_name))
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_get_source_float_data(bus, ickt, par_name)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_get_source_boolean_data(bus, ickt, par_name)
        if par_type in ['S', 'STRING']:
            return self.libsteps.api_get_source_string_data(bus, ickt, par_name)
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
        if par_type in ['I', 'INT', 'INTEGER']:
            return int(self.libsteps.api_get_load_integer_data(bus, ickt, par_name))
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_get_load_float_data(bus, ickt, par_name)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_get_load_boolean_data(bus, ickt, par_name)
        if par_type in ['S', 'STRING']:
            return self.libsteps.api_get_load_string_data(bus, ickt, par_name)
        return None

    def get_fixed_shunt_data(self, fixed_shunt, par_type, par_name):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return None
        bus, ickt = self.__extract_single_bus_device_id(fixed_shunt)
        if par_type in ['I', 'INT', 'INTEGER']:
            return int(self.libsteps.api_get_fixed_shunt_integer_data(bus, ickt, par_name))
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_get_fixed_shunt_float_data(bus, ickt, par_name)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_get_fixed_shunt_boolean_data(bus, ickt, par_name)
        if par_type in ['S', 'STRING']:
            return self.libsteps.api_get_fixed_shunt_string_data(bus, ickt, par_name)
        return None

    def get_equivalent_device_data(self, equivalent_device, par_type, par_name):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return None
        bus, ickt = self.__extract_single_bus_device_id(equivalent_device)
        if par_type in ['I', 'INT', 'INTEGER']:
            return int(self.libsteps.api_get_equivalent_device_integer_data(bus, ickt, par_name))
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_get_equivalent_device_float_data(bus, ickt, par_name)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_get_equivalent_device_boolean_data(bus, ickt, par_name)
        if par_type in ['S', 'STRING']:
            return self.libsteps.api_get_equivalent_device_string_data(bus, ickt, par_name)
        return None

    def get_line_data(self, line, par_type, par_name):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return None
        ibus, jbus, ickt = self.__extract_double_bus_device_id(line)
        if par_type in ['I', 'INT', 'INTEGER']:
            return int(self.libsteps.api_get_line_integer_data(ibus, jbus, ickt, par_name))
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_get_line_float_data(ibus, jbus, ickt, par_name)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_get_line_boolean_data(ibus, jbus, ickt, par_name)
        if par_type in ['S', 'STRING']:
            return self.libsteps.api_get_line_string_data(ibus, jbus, ickt, par_name)
        return None

    def get_transformer_data(self, transformer, par_type, side, par_name):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return None
        side = side.upper()
        if side not in ['PRIMARY', 'SECONDARY', 'TERTIARY', 'TRANSFORMER']:
            return None
        ibus, jbus, kbus, ickt = self.__extract_triple_bus_device_id(transformer)
        if par_type in ['I', 'INT', 'INTEGER']:
            return int(self.libsteps.api_get_transformer_integer_data(ibus, jbus, kbus, ickt, side, par_name))
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_get_transformer_float_data(ibus, jbus, kbus, ickt, side, par_name)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_get_transformer_boolean_data(ibus, jbus, kbus, ickt, side, par_name)
        if par_type in ['S', 'STRING']:
            return self.libsteps.api_get_transformer_string_data(ibus, jbus, kbus, ickt, side, par_name)
        return None

    def get_hvdc_data(self, hvdc, par_type, side, par_name):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return None
        side = side.upper()
        if side not in ['RECTIFIER', 'INVERTER', 'HVDC']:
            return None
        ibus, jbus, ickt = self.__extract_double_bus_device_id(hvdc)
        if par_type in ['I', 'INT', 'INTEGER']:
            return int(self.libsteps.api_get_hvdc_integer_data(ibus, jbus, ickt, side, par_name))
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_get_hvdc_float_data(ibus, jbus, ickt, side, par_name)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_get_hvdc_boolean_data(ibus, jbus, ickt, side, par_name)
        if par_type in ['S', 'STRING']:
            return self.libsteps.api_get_hvdc_string_data(ibus, jbus, ickt, side, par_name)
        return None

    def get_area_data(self, area, par_type, par_name):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return None
        if par_type in ['I', 'INT', 'INTEGER']:
            return int(self.libsteps.api_get_area_integer_data(area, par_name))
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_get_area_float_data(area, par_name)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_get_area_boolean_data(area, par_name)
        if par_type in ['S', 'STRING']:
            return self.libsteps.api_get_area_string_data(area, par_name)
        return None

    def get_zone_data(self, zone, par_type, par_name):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return None
        if par_type in ['I', 'INT', 'INTEGER']:
            return int(self.libsteps.api_get_zone_integer_data(zone, par_name))
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_get_zone_float_data(zone, par_name)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_get_zone_boolean_data(zone, par_name)
        if par_type in ['S', 'STRING']:
            return self.libsteps.api_get_zone_string_data(zone, par_name)
        return None
    
    def get_owner_data(self, owner, par_type, par_name):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return None
        if par_type in ['I', 'INT', 'INTEGER']:
            return int(self.libsteps.api_get_owner_integer_data(owner, par_name))
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_get_owner_float_data(owner, par_name)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_get_owner_boolean_data(owner, par_name)
        if par_type in ['S', 'STRING']:
            return self.libsteps.api_get_owner_string_data(owner, par_name)
        return None

        
    def set_bus_data(self, bus, par_type, par_name, value):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return
        if par_type in ['I', 'INT', 'INTEGER']:
            self.libsteps.api_set_bus_integer_data(bus, par_name, value)
            return
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_set_bus_float_data(bus, par_name, value)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_set_bus_boolean_data(bus, par_name, value)
        if par_type in ['S', 'STRING']:
            return self.libsteps.api_set_bus_string_data(bus, par_name, value)
        return

    def __set_source_data(self, source, par_type, par_name, value):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return
        
        bus, ickt = self.__extract_single_bus_device_id(source)
        if par_type in ['I', 'INT', 'INTEGER']:
            return self.libsteps.api_set_source_integer_data(bus, ickt, par_name, value)
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_set_source_float_data(bus, ickt, par_name, value)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_set_source_boolean_data(bus, ickt, par_name, value)
        if par_type in ['S', 'STRING']:
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
        if par_type in ['I', 'INT', 'INTEGER']:
            return self.libsteps.api_set_load_integer_data(bus, ickt, par_name, value)
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_set_load_float_data(bus, ickt, par_name, value)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_set_load_boolean_data(bus, ickt, par_name, value)
        if par_type in ['S', 'STRING']:
            return self.libsteps.api_set_load_string_data(bus, ickt, par_name, value)
        return

    def set_fixed_shunt_data(self, fixed_shunt, par_type, par_name, value):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return
        bus, ickt = self.__extract_single_bus_device_id(fixed_shunt)
        if par_type in ['I', 'INT', 'INTEGER']:
            return self.libsteps.api_set_fixed_shunt_integer_data(bus, ickt, par_name, value)
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_set_fixed_shunt_float_data(bus, ickt, par_name, value)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_set_fixed_shunt_boolean_data(bus, ickt, par_name, value)
        if par_type in ['S', 'STRING']:
            return self.libsteps.api_set_fixed_shunt_string_data(bus, ickt, par_name, value)
        return

    def set_equivalent_device_data(self, equivalent_device, par_type, par_name, value):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return
        bus, ickt = self.__extract_single_bus_device_id(equivalent_device)
        if par_type in ['I', 'INT', 'INTEGER']:
            return self.libsteps.api_set_equivalent_device_integer_data(bus, ickt, par_name, value)
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_set_equivalent_device_float_data(bus, ickt, par_name, value)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_set_equivalent_device_boolean_data(bus, ickt, par_name, value)
        if par_type in ['S', 'STRING']:
            return self.libsteps.api_set_equivalent_device_string_data(bus, ickt, par_name, value)
        return

    def set_line_data(self, line, par_type, par_name, value):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return
        ibus, jbus, ickt = self.__extract_double_bus_device_id(line)
        if par_type in ['I', 'INT', 'INTEGER']:
            return self.libsteps.api_set_line_integer_data(ibus, jbus, ickt, par_name, value)
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_set_line_float_data(ibus, jbus, ickt, par_name, value)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_set_line_boolean_data(ibus, jbus, ickt, par_name, value)
        if par_type in ['S', 'STRING']:
            return self.libsteps.api_set_line_string_data(ibus, jbus, ickt, par_name, value)
        return

    def set_transformer_data(self, transformer, par_type, par_name, value):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return
        ibus, jbus, kbus, ickt = self.__extract_triple_bus_device_id(line)
        if par_type in ['I', 'INT', 'INTEGER']:
            return self.libsteps.api_set_transformer_integer_data(ibus, jbus, kbus, ickt, par_name, value)
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_set_transformer_float_data(ibus, jbus, kbus, ickt, par_name, value)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_set_transformer_boolean_data(ibus, jbus, kbus, ickt, par_name, value)
        if par_type in ['S', 'STRING']:
            return self.libsteps.api_set_transformer_string_data(ibus, jbus, kbus, ickt, par_name, value)
        return

    def set_hvdc_data(self, hvdc, par_type, par_name, value):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return
        ibus, jbus, ickt = self.__extract_double_bus_device_id(hvdc)
        if par_type in ['I', 'INT', 'INTEGER']:
            return self.libsteps.api_set_hvdc_integer_data(ibus, jbus, ickt, par_name, value)
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_set_hvdc_float_data(ibus, jbus, ickt, par_name, value)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_set_hvdc_boolean_data(ibus, jbus, ickt, par_name, value)
        if par_type in ['S', 'STRING']:
            return self.libsteps.api_set_hvdc_string_data(ibus, jbus, ickt, par_name, value)
        return

    def set_area_data(self, area, par_type, par_name, value):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return
        if par_type in ['I', 'INT', 'INTEGER']:
            return self.libsteps.api_set_area_integer_data(area, par_name, value)
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_set_area_float_data(area, par_name, value)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_set_area_boolean_data(area, par_name, value)
        if par_type in ['S', 'STRING']:
            return self.libsteps.api_set_area_string_data(area, par_name, value)
        return

    def set_zone_data(self, zone, par_type, par_name, value):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return
        if par_type in ['I', 'INT', 'INTEGER']:
            return self.libsteps.api_set_zone_integer_data(zone, par_name, value)
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_set_zone_float_data(zone, par_name, value)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_set_zone_boolean_data(zone, par_name, value)
        if par_type in ['S', 'STRING']:
            return self.libsteps.api_set_zone_string_data(zone, par_name, value)
        return

    def set_owner_data(self, owner, par_type, par_name, value):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return
        if par_type in ['I', 'INT', 'INTEGER']:
            return self.libsteps.api_set_owner_integer_data(owner, par_name, value)
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_set_owner_float_data(owner, par_name, value)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_set_owner_boolean_data(owner, par_name, value)
        if par_type in ['S', 'STRING']:
            return self.libsteps.api_set_owner_string_data(owner, par_name, value)
        return

    def get_generator_related_model_name(self, generator, model_type):
        return self.libsteps.api_get_generator_related_model_name(generator[0], generator[1], model_type)

    def get_generator_related_model_data(self, generator, model_type, par_type, par_name):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN']:
            return None
        if par_type in ['I', 'INT', 'INTEGER']:
            return 0
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_get_generator_related_model_float_parameter(generator[0], generator[1], model_type, par_name)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return False
        return None
        
    def set_generator_related_model_data(self, generator, model_type, par_type, par_name, value):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN']:
            return
        if par_type in ['I', 'INT', 'INTEGER']:
            return
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_set_generator_related_model_float_parameter(generator[0], generator[1], model_type, par_name, value)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return
        return
        
    def get_wt_generator_related_model_name(self, generator, model_type):
        return self.libsteps.api_get_wt_generator_related_model_name(generator[0], generator[1], model_type)

    def get_wt_generator_related_model_data(self, generator, model_type, par_type, par_name):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN']:
            return None
        if par_type in ['I', 'INT', 'INTEGER']:
            return 0
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_get_wt_generator_related_model_float_parameter(generator[0], generator[1], model_type, par_name)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return False
        return None
        
    def set_wt_generator_related_model_data(self, generator, model_type, par_type, par_name, value):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN']:
            return
        if par_type in ['I', 'INT', 'INTEGER']:
            return
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_set_wt_generator_related_model_float_parameter(generator[0], generator[1], model_type, par_name, value)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return
        return
        
    def get_pv_unit_related_model_name(self, pv_unit, model_type):
        return self.libsteps.api_get_pv_unit_related_model_name(pv_unit[0], pv_unit[1], model_type)

    def get_pv_unit_related_model_data(self, pv_unit, model_type, par_type, par_name):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN']:
            return None
        if par_type in ['I', 'INT', 'INTEGER']:
            return 0
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_get_pv_unit_related_model_float_parameter(pv_unit[0], pv_unit[1], model_type, par_name)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return False
        return None
        
    def set_pv_unit_related_model_data(self, pv_unit, model_type, par_type, par_name, value):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN']:
            return
        if par_type in ['I', 'INT', 'INTEGER']:
            return
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_set_pv_unit_related_model_float_parameter(pv_unit[0], pv_unit[1], model_type, par_name, value)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return
        return
        
    def get_powerflow_solver_parameter(self, par_type, par_name):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN']:
            return None
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
        if par_type in ['I', 'INT', 'INTEGER']:
            return self.libsteps.api_set_powerflow_solver_integer_parameter(par_name, value)
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_set_powerflow_solver_float_parameter(par_name, value)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_set_powerflow_solver_boolean_parameter(par_name, value)
        return
        
    def solve_powerflow(self, method):
        self.libsteps.api_solve_powerflow(method)
        return

    def is_powerflow_converged(self):
        return self.libsteps.api_is_powerflow_converged()
        
    def show_powerflow_result(self):
        self.libsteps.api_show_powerflow_result()
        return
        
    def save_powerflow_result(self, file):
        self.libsteps.api_save_powerflow_result(file)
        return
    
    def save_extended_powerflow_result(self, file):
        self.libsteps.api_save_extended_powerflow_result(file)
        return
        
    def save_jacobian_matrix(self, file):
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
        self.libsteps.api_save_network_matrix(file)
        return
        
    def save_decoupled_network_matrix(self, file):
        self.libsteps.api_save_decoupled_network_matrix(file)
        return
        
    def save_dc_network_matrix(self, file):
        self.libsteps.api_save_dc_network_matrix(file)
        return
        
    def save_dynamic_network_matrix(self, file):
        self.libsteps.api_save_dynamic_network_matrix(file)
        return
        
        
    def get_dynamic_simulator_parameter(self, par_type, par_name):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return None
        if par_type in ['I', 'INT', 'INTEGER']:
            return int(self.libsteps.api_get_dynamic_simulator_integer_parameter(par_name))
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_get_dynamic_simulator_float_parameter(par_name)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_get_dynamic_simulator_boolean_parameter(par_name)
        if par_type in ['S', 'STRING']:
            return self.libsteps.api_get_dynamic_simulator_string_parameter(par_name)
        return None

    def set_dynamic_simulator_parameter(self, par_type, par_name, value):
        par_type = par_type.upper()
        if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
            return None
        if par_type in ['I', 'INT', 'INTEGER']:
            return self.libsteps.api_set_dynamic_simulator_integer_parameter(par_name, value)
        if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
            return self.libsteps.api_set_dynamic_simulator_float_parameter(par_name, value)
        if par_type in ['B', 'BOOL', 'BOOLEAN']:
            return self.libsteps.api_set_dynamic_simulator_boolean_parameter(par_name, value)
        if par_type in ['S', 'STRING']:
            return self.libsteps.api_set_dynamic_simulator_string_parameter(par_name, value)
        return None
        
    def get_dynamic_simulator_output_file(self):
        return self.libsteps.api_get_dynamic_simulator_output_file()
        
    def set_dynamic_simulator_output_file(self, file):
        return self.libsteps.api_set_dynamic_simulator_output_file(file)
        
    def get_dynamic_simulation_time_step(self):
        return self.libsteps.api_get_dynamic_simulation_time_step()
        
    def set_dynamic_simulation_time_step(self, step):
        return self.libsteps.api_set_dynamic_simulation_time_step(step)
        
    def get_dynamic_simulation_time(self):
        return self.libsteps.api_get_dynamic_simulation_time()
        
    def prepare_meters(self, device_type):
        device_type = device_type.upper()
        if device_type not in ['ALL', 'BUS', 'GENERATOR', 'LOAD', 'LINE', 'HVDC', 'EQUIVALENT DEVICE']:
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
        self.libsteps.api_prepare_bus_related_meter(bus, meter_type)
        return

    def prepare_generator_meter(self, generator, meter_type):
        bus, ickt = self.__extract_single_bus_device_id(generator)
        self.libsteps.api_prepare_generator_related_meter(bus, ickt, meter_type)
        return

    def prepare_wt_generator_meter(self, generator, meter_type):
        bus, ickt = self.__extract_single_bus_device_id(generator)
        self.libsteps.api_prepare_wt_generator_related_meter(bus, ickt, meter_type)
        return

    def prepare_load_meter(self, load, meter_type):
        bus, ickt = self.__extract_single_bus_device_id(load)
        self.libsteps.api_prepare_load_related_meter(bus, ickt, meter_type)
        return

    def prepare_line_meter(self, line, meter_type, side):
        ibus, jbus, ickt = self.__extract_double_bus_device_id(line)
        self.libsteps.api_prepare_line_related_meter(ibus, jbus, ickt, meter_type, side)
        return

    def prepare_hvdc_meter(self, hvdc, meter_type, side):
        ibus, jbus, ickt = self.__extract_double_bus_device_id(hvdc)
        self.libsteps.api_prepare_hvdc_related_meter(ibus, jbus, ickt, meter_type, side)
        return

    def prepare_equivalent_device_meter(self, edevice, meter_type):
        bus, ickt = self.__extract_single_bus_device_id(equivalent_device)
        self.libsteps.api_prepare_equivalent_device_related_meter(bus, ickt, meter_type)
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
        self.libsteps.api_set_bus_fault(bus, fault_type, fault_shunt[0], fault_shunt[1])
        return

    def clear_bus_fault(self, bus, fault_type):
        self.libsteps.api_clear_bus_fault(bus, fault_type)
        return

    def trip_bus(self, bus):
        self.libsteps.api_trip_bus(bus)
        return

    def set_line_fault(self, line, fault_type, fault_location, fault_shunt):
        self.libsteps.api_set_line_fault(line[0], line[1], line[2], fault_type, fault_location, fault_shunt[0], fault_shunt[1])
        return

    def clear_line_fault(self, line, fault_type, fault_location):
        self.libsteps.api_clear_line_fault(line[0], line[1], line[2], fault_type, fault_location)
        return

    def trip_line(self, line):
        self.libsteps.api_trip_line(line[0], line[1], line[2])
        return

    def trip_line_breaker(self, line):
        self.libsteps.api_trip_line_breaker(line[0], line[1], line[2])
        return
        

    def close_line(self, line):
        self.libsteps.api_close_line(line[0], line[1], line[2])
        return

    def close_line_breaker(self, line):
        self.libsteps.api_close_line_breaker(line[0], line[1], line[2])
        return
        

    def trip_transformer(self, transformer):
        ibus, jbus, kbus, ickt = self.__extract_triple_bus_device_id(transformer)
        self.libsteps.api_trip_transformer(ibus, jbus, kbus, ickt)
        return

    def trip_transformer_breaker(self, transformer):
        ibus, jbus, kbus, ickt = self.__extract_triple_bus_device_id(transformer)
        self.libsteps.api_trip_transformer_breaker(ibus, jbus, kbus, ickt)
        return
        

    def close_transformer(self, transformer):
        ibus, jbus, kbus, ickt = self.__extract_triple_bus_device_id(transformer)
        self.libsteps.api_close_transformer(ibus, jbus, kbus, ickt)
        return

    def close_transformer_breaker(self, transformer):
        ibus, jbus, kbus, ickt = self.__extract_triple_bus_device_id(transformer)
        self.libsteps.api_close_transformer_breaker(ibus, jbus, kbus, ickt)
        return

    def trip_generator(self, generator):
        self.libsteps.api_trip_generator(generator[0], generator[1])
        return

    def shed_generator(self, generator, percent):
        self.libsteps.api_shed_generator(generator[0], generator[1], percent)
        return

    def trip_load(self, load):
        self.libsteps.api_trip_load(load[0], load[1])
        return

    def close_load(self, load):
        self.libsteps.api_close_load(load[0], load[1])
        return

    def scale_load(self, load, percent):
        self.libsteps.api_scale_load(load[0], load[1], percent)
        return

    def scale_all_loads(self, percent):
        self.libsteps.api_scale_all_loads(percent)
        return

    def trip_fixed_shunt(self, shunt):
        self.libsteps.api_trip_fixed_shunt(shunt[0], shunt[1])
        return

    def close_fixed_shunt(self, shunt):
        self.libsteps.api_close_fixed_shunt(shunt[0], shunt[1])
        return

    def manually_bypass_hvdc(self, hvdc):
        self.libsteps.api_manually_bypass_hvdc(hvdc[0], hvdc[1], hvdc[2])
        return

    def manually_block_hvdc(self, hvdc):
        self.libsteps.api_manually_block_hvdc(hvdc[0], hvdc[1], hvdc[2])
        return

    def manually_unbypass_hvdc(self, hvdc):
        self.libsteps.api_manually_unbypass_hvdc(hvdc[0], hvdc[1], hvdc[2])
        return

    def manually_unblock_hvdc(self, hvdc):
        self.libsteps.api_manually_unblock_hvdc(hvdc[0], hvdc[1], hvdc[2])
        return

    def get_generator_voltage_reference_in_pu(self, generator):
        return self.libsteps.api_get_generator_voltage_reference_in_pu(generator[0], generator[1])

    def get_generator_power_reference_in_MW(self, generator):
        return self.libsteps.api_get_generator_power_reference_in_MW(generator[0], generator[1])

    def set_generator_voltage_reference_in_pu(self, generator, value):
        return self.libsteps.api_set_generator_voltage_reference_in_pu(generator[0], generator[1], value)

    def set_generator_power_reference_in_MW(self, generator, value):
        return self.libsteps.api_set_generator_power_reference_in_MW(generator[0], generator[1], value)

        
    def search_cct(self, pf_file, dy_file, line, side, trip_logic):
        return
        ibus = line[0]
        jbus = line[1]
        ickt = line[2]
        trip_flag = 0
        print(line)
        print(side)
        print(trip_logic)
        if trip_logic is True:
            trip_flag = 1
        return self.libsteps.api_search_cct(pf_file, dy_file, ibus, jbus, ickt, side, trip_flag)

class pSTEPS(STEPS):
    def __init__(self):
        STEPS.__init__(self, True)
        print("Parallel STEPS simulator is created:", self)

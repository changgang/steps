from libsteps import libsteps

libsteps.api_initialize_package()

def get_allowed_maximum_bus_number():
    return libsteps.api_get_allowed_maximum_bus_number()

def set_allowed_maximum_bus_number(max_bus_number):
    libsteps.api_set_allowed_maximum_bus_number(max_bus_number)

def load_powerflow_data(file, type):
    libsteps.api_load_powerflow_data_from_file(file, type)

def get_bus_capacity():
    return libsteps.api_get_device_capacity("BUS")
def get_generator_capacity():
    return libsteps.api_get_device_capacity("Generator")
def get_pe_source_capacity():
    return libsteps.api_get_device_capacity("PE Source")
def get_load_capacity():
    return libsteps.api_get_device_capacity("Load")
def get_fixed_shunt_capacity():
    return libsteps.api_get_device_capacity("Fixed Shunt")
def get_line_capacity():
    return libsteps.api_get_device_capacity("Line")
def get_transformer_capacity():
    return libsteps.api_get_device_capacity("Transformer")
def get_hvdc_capacity():
    return libsteps.api_get_device_capacity("HVDC")
def get_equivalent_device_capacity():
    return libsteps.api_get_device_capacity("Equivalent Device")
def get_area_capacity():
    return libsteps.api_get_area_capacity()
def get_zone_capacity():
    return libsteps.api_get_zone_capacity()
def get_owner_capacity():
    return libsteps.api_get_owner_capacity()

def set_bus_capacity(capacity):
    return libsteps.api_set_device_capacity("BUS", capacity)
def set_generator_capacity(capacity):
    return libsteps.api_set_device_capacity("Generator", capacity)
def set_pe_source_capacity(capacity):
    return libsteps.api_set_device_capacity("PE Source", capacity)
def set_load_capacity(capacity):
    return libsteps.api_set_device_capacity("Load", capacity)
def set_fixed_shunt_capacity(capacity):
    return libsteps.api_set_device_capacity("Fixed Shunt", capacity)
def set_line_capacity(capacity):
    return libsteps.api_set_device_capacity("Line", capacity)
def set_transformer_capacity(capacity):
    return libsteps.api_set_device_capacity("Transformer", capacity)
def set_hvdc_capacity(capacity):
    return libsteps.api_set_device_capacity("HVDC", capacity)
def set_equivalent_device_capacity(capacity):
    return libsteps.api_set_device_capacity("Equivalent Device", capacity)
def set_area_capacity(capacity):
    return libsteps.api_set_area_capacity(capacity)
def set_zone_capacity(capacity):
    return libsteps.api_set_zone_capacity(capacity)
def set_owner_capacity(capacity):
    return libsteps.api_set_owner_capacity(capacity)


def get_bus_count():
    return libsteps.api_get_device_count("BUS")
def get_generator_count():
    return libsteps.api_get_device_count("Generator")
def get_pe_source_count():
    return libsteps.api_get_device_count("PE Source")
def get_load_count():
    return libsteps.api_get_device_count("Load")
def get_fixed_shunt_count():
    return libsteps.api_get_device_count("Fixed Shunt")
def get_line_count():
    return libsteps.api_get_device_count("Line")
def get_transformer_count():
    return libsteps.api_get_device_count("Transformer")
def get_hvdc_count():
    return libsteps.api_get_device_count("HVDC")
def get_equivalent_device_count():
    return libsteps.api_get_device_count("Equivalent Device")
def get_area_count():
    return libsteps.api_get_area_count()
def get_zone_count():
    return libsteps.api_get_zone_count()
def get_owner_count():
    return libsteps.api_get_owner_count()

def get_all_buses():
    libsteps.api_initialize_bus_search(-1e10, 1e10, -1e10, 1e10, 0, 0 ,0)
    buses = []
    while True:
        bus = libsteps.api_get_current_bus_number()
        if bus==0:
            break
        buses.append(bus)
        libsteps.api_goto_next_bus()
    return buses

def get_buses_with_constraints(constraints):
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
    
    libsteps.api_initialize_bus_search(vbase_min, vbase_max, v_min, v_max, area, zone, owner)
    while True:
        bus = libsteps.api_get_current_bus_number()
        if bus==0:
            break
        buses.append(bus)
        libsteps.api_goto_next_bus()
    return buses
    
def get_generators_at_bus(bus):
    libsteps.api_initialize_device_search("GENERATOR", bus)
    generators = []
    while True:
        bus = libsteps.api_get_current_device_bus_number("GENERATOR", "")
        if bus==0:
            break
        id = libsteps.api_get_current_device_identifier("GENERATOR")
        generators.append([bus, id])
        libsteps.api_goto_next_device("GENERATOR")
    return generators
    
def get_pe_sources_at_bus(bus):
    libsteps.api_initialize_device_search("PE SOURCE", bus)
    pe_sources = []
    while True:
        bus = libsteps.api_get_current_device_bus_number("PE SOURCE", "")
        if bus==0:
            break
        id = libsteps.api_get_current_device_identifier("PE SOURCE")
        pe_sources.append([bus, id])
        libsteps.api_goto_next_device("PE SOURCE")
    return pe_sources
    
def get_loads_at_bus(bus):
    libsteps.api_initialize_device_search("LOAD", bus)
    loads = []
    while True:
        bus = libsteps.api_get_current_device_bus_number("LOAD", "")
        if bus==0:
            break
        id = libsteps.api_get_current_device_identifier("LOAD")
        loads.append([bus, id])
        libsteps.api_goto_next_device("LOAD")
    return loads
    
def get_fixed_shunts_at_bus(bus):
    libsteps.api_initialize_device_search("FIXED SHUNT", bus)
    fixed_shunts = []
    while True:
        bus = libsteps.api_get_current_device_bus_number("FIXED SHUNT", "")
        if bus==0:
            break
        id = libsteps.api_get_current_device_identifier("FIXED SHUNT")
        fixed_shunts.append([bus, id])
        libsteps.api_goto_next_device("FIXED SHUNT")
    return fixed_shunts
    
def get_equivalent_devices_at_bus(bus):
    libsteps.api_initialize_device_search("EQUIVALENT DEVICE", bus)
    equivalent_devices = []
    while True:
        bus = libsteps.api_get_current_device_bus_number("EQUIVALENT DEVICE", "")
        if bus==0:
            break
        id = libsteps.api_get_current_device_identifier("EQUIVALENT DEVICE")
        equivalent_devices.append([bus, id])
        libsteps.api_goto_next_device("EQUIVALENT DEVICE")
    return equivalent_devices

def get_lines_at_bus(bus):
    libsteps.api_initialize_device_search("LINE", bus)
    lines = []
    while True:
        ibus = libsteps.api_get_current_device_bus_number("LINE", "SEND")
        if ibus==0:
            break
        jbus = libsteps.api_get_current_device_bus_number("LINE", "RECEIVE")
        id = libsteps.api_get_current_device_identifier("LINE")
        lines.append([ibus, jbus, id])
        libsteps.api_goto_next_device("LINE")
    return lines

def get_transformers_at_bus(bus):
    libsteps.api_initialize_device_search("TRANSFORMER", bus)
    transformers = []
    while True:
        ibus = libsteps.api_get_current_device_bus_number("TRANSFORMER", "PRIMARY")
        if ibus==0:
            break
        jbus = libsteps.api_get_current_device_bus_number("TRANSFORMER", "SECONDARY")
        kbus = libsteps.api_get_current_device_bus_number("TRANSFORMER", "TERTIARY")
        id = libsteps.api_get_current_device_identifier("TRANSFORMER")
        transformers.append([ibus, jbus, kbus, id])
        libsteps.api_goto_next_device("TRANSFORMER")
    return transformers

def get_hvdcs_at_bus(bus):
    libsteps.api_initialize_device_search("HVDC", bus)
    hvdcs = []
    while True:
        ibus = libsteps.api_get_current_device_bus_number("HVDC", "RECTIFIER")
        if ibus==0:
            break
        jbus = libsteps.api_get_current_device_bus_number("HVDC", "INVERTER")
        id = libsteps.api_get_current_device_identifier("HVDC")
        hvdcs.append([ibus, jbus, id])
        libsteps.api_goto_next_device("HVDC")
    return hvdcs

def get_areas():
    libsteps.api_initialize_area_search()
    areas = []
    while True:
        area = libsteps.api_get_current_area_number()
        if area==0:
            break
        areas.append(area)
        libsteps.api_goto_next_area()
    return areas

def get_zones():
    libsteps.api_initialize_zone_search()
    zones = []
    while True:
        zone = libsteps.api_get_current_zone_number()
        if zone==0:
            break
        zones.append(zone)
        libsteps.api_goto_next_zone()
    return zones

def get_owners():
    libsteps.api_initialize_owner_search()
    owners = []
    while True:
        owner = libsteps.api_get_current_owner_number()
        if owner==0:
            break
        owners.append(owner)
        libsteps.api_goto_next_owner()
    return owners

def get_bus_data(bus, par_type, par_name):
    par_type = par_type.upper()
    if par_type not in ['INT', 'INTEGER', 'FLOAT', 'DOUBLE', 'BOOL', 'BOOLEAN', 'STRING']:
        return 0
    if par_type in ['INT', 'INTEGER']:
        return libsteps.api_get_bus_integer_data(bus, par_name)
    if par_type in ['FLOAT', 'DOIUBLE']:
        return libsteps.api_get_bus_float_data(bus, par_name)
    if par_type in ['BOOL', 'BOOLEAN']:
        return libsteps.api_get_bus_boolean_data(bus, par_name)
    if par_type in ['STRING']:
        return libsteps.api_get_bus_string_data(bus, par_name)
    return 0

def get_generator_data(bus, id, par_type, par_name):
    par_type = par_type.upper()
    if par_type not in ['INT', 'INTEGER', 'FLOAT', 'DOUBLE', 'BOOL', 'BOOLEAN', 'STRING']:
        return 0
    if par_type in ['INT', 'INTEGER']:
        return libsteps.api_get_generator_integer_data(bus, id, par_name)
    if par_type in ['FLOAT', 'DOIUBLE']:
        return libsteps.api_get_generator_float_data(bus, id, par_name)
    if par_type in ['BOOL', 'BOOLEAN']:
        return libsteps.api_get_generator_boolean_data(bus, id, par_name)
    if par_type in ['STRING']:
        return libsteps.api_get_generator_string_data(bus, id, par_name)
    return 0


    

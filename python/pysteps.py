from libsteps import libsteps

libsteps.api_initialize_package()

def get_allowed_maximum_bus_number():
    return libsteps.api_get_allowed_maximum_bus_number()

def set_allowed_maximum_bus_number(max_bus_number):
    libsteps.api_set_allowed_maximum_bus_number(max_bus_number)

def load_powerflow_data(file, type):
    libsteps.api_load_powerflow_data_from_file(file, type)
    
def load_dynamic_data(file, type):
    libsteps.api_load_dynamic_data_from_file(file, type)

def get_bus_capacity():
    return libsteps.api_get_device_capacity("BUS")
def get_generator_capacity():
    return libsteps.api_get_device_capacity("Generator")
def get_wt_generator_capacity():
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
def set_wt_generator_capacity(capacity):
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
def get_wt_generator_count():
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
    libsteps.api_initialize_all_bus_search()
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
    
def get_wt_generators_at_bus(bus):
    libsteps.api_initialize_device_search("PE SOURCE", bus)
    wt_generators = []
    while True:
        bus = libsteps.api_get_current_device_bus_number("PE SOURCE", "")
        if bus==0:
            break
        id = libsteps.api_get_current_device_identifier("PE SOURCE")
        wt_generators.append([bus, id])
        libsteps.api_goto_next_device("PE SOURCE")
    return wt_generators
    
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
    if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
        return None
    if par_type in ['I', 'INT', 'INTEGER']:
        return libsteps.api_get_bus_integer_data(bus, par_name)
    if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
        return libsteps.api_get_bus_float_data(bus, par_name)
    if par_type in ['B', 'BOOL', 'BOOLEAN']:
        return libsteps.api_get_bus_boolean_data(bus, par_name)
    if par_type in ['S', 'STRING']:
        return libsteps.api_get_bus_string_data(bus, par_name)
    return None

def get_generator_data(generator, par_type, par_name):
    par_type = par_type.upper()
    if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
        return None
    
    bus = generator[0]
    id = generator[1]
    if par_type in ['I', 'INT', 'INTEGER']:
        return libsteps.api_get_source_integer_data(bus, id, par_name)
    if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
        return libsteps.api_get_source_float_data(bus, id, par_name)
    if par_type in ['B', 'BOOL', 'BOOLEAN']:
        return libsteps.api_get_source_boolean_data(bus, id, par_name)
    if par_type in ['S', 'STRING']:
        return libsteps.api_get_source_string_data(bus, id, par_name)
    return None

def get_wt_generator_data(wt_generator, par_type, par_name):
    par_type = par_type.upper()
    if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
        return None
    
    bus = wt_generator[0]
    id = wt_generator[1]
    if par_type in ['I', 'INT', 'INTEGER']:
        return libsteps.api_get_source_integer_data(bus, id, par_name)
    if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
        return libsteps.api_get_source_float_data(bus, id, par_name)
    if par_type in ['B', 'BOOL', 'BOOLEAN']:
        return libsteps.api_get_source_boolean_data(bus, id, par_name)
    if par_type in ['S', 'STRING']:
        return libsteps.api_get_source_string_data(bus, id, par_name)
    return None

def get_load_data(load, par_type, par_name):
    par_type = par_type.upper()
    if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
        return None
    
    bus = load[0]
    id = load[1]
    if par_type in ['I', 'INT', 'INTEGER']:
        return libsteps.api_get_load_integer_data(bus, id, par_name)
    if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
        return libsteps.api_get_load_float_data(bus, id, par_name)
    if par_type in ['B', 'BOOL', 'BOOLEAN']:
        return libsteps.api_get_load_boolean_data(bus, id, par_name)
    if par_type in ['S', 'STRING']:
        return libsteps.api_get_load_string_data(bus, id, par_name)
    return None

def get_fixed_shunt_data(fixed_shunt, par_type, par_name):
    par_type = par_type.upper()
    if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
        return None
    
    bus = fixed_shunt[0]
    id = fixed_shunt[1]
    if par_type in ['I', 'INT', 'INTEGER']:
        return libsteps.api_get_fixed_shunt_integer_data(bus, id, par_name)
    if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
        return libsteps.api_get_fixed_shunt_float_data(bus, id, par_name)
    if par_type in ['B', 'BOOL', 'BOOLEAN']:
        return libsteps.api_get_fixed_shunt_boolean_data(bus, id, par_name)
    if par_type in ['S', 'STRING']:
        return libsteps.api_get_fixed_shunt_string_data(bus, id, par_name)
    return None

def get_equivalent_device_data(equivalent_device, par_type, par_name):
    par_type = par_type.upper()
    if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
        return None
    
    bus = equivalent_device[0]
    id = equivalent_device[1]
    if par_type in ['I', 'INT', 'INTEGER']:
        return libsteps.api_get_equivalent_device_integer_data(bus, id, par_name)
    if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
        return libsteps.api_get_equivalent_device_float_data(bus, id, par_name)
    if par_type in ['B', 'BOOL', 'BOOLEAN']:
        return libsteps.api_get_equivalent_device_boolean_data(bus, id, par_name)
    if par_type in ['S', 'STRING']:
        return libsteps.api_get_equivalent_device_string_data(bus, id, par_name)
    return None

def get_line_data(line, par_type, par_name):
    par_type = par_type.upper()
    if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
        return None
    
    ibus = line[0]
    jbus = line[1]
    id = line[2]
    if par_type in ['I', 'INT', 'INTEGER']:
        return libsteps.api_get_line_integer_data(ibus, jbus, id, par_name)
    if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
        return libsteps.api_get_line_float_data(ibus, jbus, id, par_name)
    if par_type in ['B', 'BOOL', 'BOOLEAN']:
        return libsteps.api_get_line_boolean_data(ibus, jbus, id, par_name)
    if par_type in ['S', 'STRING']:
        return libsteps.api_get_line_string_data(ibus, jbus, id, par_name)
    return None

def get_transformer_data(transformer, par_type, side, par_name):
    par_type = par_type.upper()
    if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
        return None
    side = side.upper()
    if side not in ['PRIMARY', 'SECONDARY', 'TERTIARY', 'TRANSFORMER']:
        return None

    ibus = transformer[0]
    jbus = transformer[1]
    kbus = transformer[2]
    id = transformer[3]
    if par_type in ['I', 'INT', 'INTEGER']:
        return libsteps.api_get_transformer_integer_data(ibus, jbus, kbus, id, side, par_name)
    if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
        return libsteps.api_get_transformer_float_data(ibus, jbus, kbus, id, side, par_name)
    if par_type in ['B', 'BOOL', 'BOOLEAN']:
        return libsteps.api_get_transformer_boolean_data(ibus, jbus, kbus, id, side, par_name)
    if par_type in ['S', 'STRING']:
        return libsteps.api_get_transformer_string_data(ibus, jbus, kbus, id, side, par_name)
    return None

def get_hvdc_data(hvdc, par_type, side, par_name):
    par_type = par_type.upper()
    if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
        return None
    side = side.upper()
    if side not in ['RECTIFIER', 'INVERTER', 'HVDC']:
        return None
    
    ibus = hvdc[0]
    jbus = hvdc[1]
    id = hvdc[2]
    if par_type in ['I', 'INT', 'INTEGER']:
        return libsteps.api_get_hvdc_integer_data(ibus, jbus, id, side, par_name)
    if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
        return libsteps.api_get_hvdc_float_data(ibus, jbus, id, side, par_name)
    if par_type in ['B', 'BOOL', 'BOOLEAN']:
        return libsteps.api_get_hvdc_boolean_data(ibus, jbus, id, side, par_name)
    if par_type in ['S', 'STRING']:
        return libsteps.api_get_hvdc_string_data(ibus, jbus, id, side, par_name)
    return None

def get_area_data(area, par_type, par_name):
    par_type = par_type.upper()
    if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
        return None
    if par_type in ['I', 'INT', 'INTEGER']:
        return libsteps.api_get_area_integer_data(area, par_name)
    if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
        return libsteps.api_get_area_float_data(area, par_name)
    if par_type in ['B', 'BOOL', 'BOOLEAN']:
        return libsteps.api_get_area_boolean_data(area, par_name)
    if par_type in ['S', 'STRING']:
        return libsteps.api_get_area_string_data(area, par_name)
    return None

def get_zone_data(zone, par_type, par_name):
    par_type = par_type.upper()
    if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
        return None
    if par_type in ['I', 'INT', 'INTEGER']:
        return libsteps.api_get_zone_integer_data(zone, par_name)
    if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
        return libsteps.api_get_zone_float_data(zone, par_name)
    if par_type in ['B', 'BOOL', 'BOOLEAN']:
        return libsteps.api_get_zone_boolean_data(zone, par_name)
    if par_type in ['S', 'STRING']:
        return libsteps.api_get_zone_string_data(zone, par_name)
    return None
 
def get_owner_data(owner, par_type, par_name):
    par_type = par_type.upper()
    if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
        return None
    if par_type in ['I', 'INT', 'INTEGER']:
        return libsteps.api_get_owner_integer_data(owner, par_name)
    if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
        return libsteps.api_get_owner_float_data(owner, par_name)
    if par_type in ['B', 'BOOL', 'BOOLEAN']:
        return libsteps.api_get_owner_boolean_data(owner, par_name)
    if par_type in ['S', 'STRING']:
        return libsteps.api_get_owner_string_data(owner, par_name)
    return None

    
def set_bus_data(bus, par_type, par_name, value):
    par_type = par_type.upper()
    if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
        return
    if par_type in ['I', 'INT', 'INTEGER']:
        libsteps.api_set_bus_integer_data(bus, par_name, value)
        return
    if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
        return libsteps.api_set_bus_float_data(bus, par_name, value)
    if par_type in ['B', 'BOOL', 'BOOLEAN']:
        return libsteps.api_set_bus_boolean_data(bus, par_name, value)
    if par_type in ['S', 'STRING']:
        return libsteps.api_set_bus_string_data(bus, par_name, value)
    return None

def set_generator_data(generator, par_type, par_name, value):
    par_type = par_type.upper()
    if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
        return None
    
    bus = generator[0]
    id = generator[1]
    if par_type in ['I', 'INT', 'INTEGER']:
        return libsteps.api_set_source_integer_data(bus, id, par_name, value)
    if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
        return libsteps.api_set_source_float_data(bus, id, par_name, value)
    if par_type in ['B', 'BOOL', 'BOOLEAN']:
        return libsteps.api_set_source_boolean_data(bus, id, par_name, value)
    if par_type in ['S', 'STRING']:
        return libsteps.api_set_source_string_data(bus, id, par_name, value)
    return None

def set_wt_generator_data(wt_generator, par_type, par_name, value):
    par_type = par_type.upper()
    if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
        return None
    
    bus = wt_generator[0]
    id = wt_generator[1]
    if par_type in ['I', 'INT', 'INTEGER']:
        return libsteps.api_set_source_integer_data(bus, id, par_name, value)
    if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
        return libsteps.api_set_source_float_data(bus, id, par_name, value)
    if par_type in ['B', 'BOOL', 'BOOLEAN']:
        return libsteps.api_set_source_boolean_data(bus, id, par_name, value)
    if par_type in ['S', 'STRING']:
        return libsteps.api_set_source_string_data(bus, id, par_name, value)
    return None

def set_load_data(load, par_type, par_name, value):
    par_type = par_type.upper()
    if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
        return None
    
    bus = load[0]
    id = load[1]
    if par_type in ['I', 'INT', 'INTEGER']:
        return libsteps.api_set_load_integer_data(bus, id, par_name, value)
    if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
        return libsteps.api_set_load_float_data(bus, id, par_name, value)
    if par_type in ['B', 'BOOL', 'BOOLEAN']:
        return libsteps.api_set_load_boolean_data(bus, id, par_name, value)
    if par_type in ['S', 'STRING']:
        return libsteps.api_set_load_string_data(bus, id, par_name, value)
    return None

def set_fixed_shunt_data(fixed_shunt, par_type, par_name, value):
    par_type = par_type.upper()
    if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
        return None
    
    bus = fixed_shunt[0]
    id = fixed_shunt[1]
    if par_type in ['I', 'INT', 'INTEGER']:
        return libsteps.api_set_fixed_shunt_integer_data(bus, id, par_name, value)
    if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
        return libsteps.api_set_fixed_shunt_float_data(bus, id, par_name, value)
    if par_type in ['B', 'BOOL', 'BOOLEAN']:
        return libsteps.api_set_fixed_shunt_boolean_data(bus, id, par_name, value)
    if par_type in ['S', 'STRING']:
        return libsteps.api_set_fixed_shunt_string_data(bus, id, par_name, value)
    return None

def set_equivalent_device_data(equivalent_device, par_type, par_name, value):
    par_type = par_type.upper()
    if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
        return None
    
    bus = equivalent_device[0]
    id = equivalent_device[1]
    if par_type in ['I', 'INT', 'INTEGER']:
        return libsteps.api_set_equivalent_device_integer_data(bus, id, par_name, value)
    if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
        return libsteps.api_set_equivalent_device_float_data(bus, id, par_name, value)
    if par_type in ['B', 'BOOL', 'BOOLEAN']:
        return libsteps.api_set_equivalent_device_boolean_data(bus, id, par_name, value)
    if par_type in ['S', 'STRING']:
        return libsteps.api_set_equivalent_device_string_data(bus, id, par_name, value)
    return None

def set_line_data(line, par_type, par_name, value):
    par_type = par_type.upper()
    if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
        return None
    
    ibus = line[0]
    jbus = line[1]
    id = line[2]
    if par_type in ['I', 'INT', 'INTEGER']:
        return libsteps.api_set_line_integer_data(ibus, jbus, id, par_name, value)
    if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
        return libsteps.api_set_line_float_data(ibus, jbus, id, par_name, value)
    if par_type in ['B', 'BOOL', 'BOOLEAN']:
        return libsteps.api_set_line_boolean_data(ibus, jbus, id, par_name, value)
    if par_type in ['S', 'STRING']:
        return libsteps.api_set_line_string_data(ibus, jbus, id, par_name, value)
    return None

def set_transformer_data(transformer, par_type, par_name, value):
    par_type = par_type.upper()
    if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
        return None
    
    ibus = transformer[0]
    jbus = transformer[1]
    kbus = transformer[2]
    id = transformer[3]
    if par_type in ['I', 'INT', 'INTEGER']:
        return libsteps.api_set_transformer_integer_data(ibus, jbus, kbus, id, par_name, value)
    if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
        return libsteps.api_set_transformer_float_data(ibus, jbus, kbus, id, par_name, value)
    if par_type in ['B', 'BOOL', 'BOOLEAN']:
        return libsteps.api_set_transformer_boolean_data(ibus, jbus, kbus, id, par_name, value)
    if par_type in ['S', 'STRING']:
        return libsteps.api_set_transformer_string_data(ibus, jbus, kbus, id, par_name, value)
    return None

def set_hvdc_data(hvdc, par_type, par_name, value):
    par_type = par_type.upper()
    if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
        return None
    
    ibus = hvdc[0]
    jbus = hvdc[1]
    id = hvdc[2]
    if par_type in ['I', 'INT', 'INTEGER']:
        return libsteps.api_set_hvdc_integer_data(ibus, jbus, id, par_name, value)
    if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
        return libsteps.api_set_hvdc_float_data(ibus, jbus, id, par_name, value)
    if par_type in ['B', 'BOOL', 'BOOLEAN']:
        return libsteps.api_set_hvdc_boolean_data(ibus, jbus, id, par_name, value)
    if par_type in ['S', 'STRING']:
        return libsteps.api_set_hvdc_string_data(ibus, jbus, id, par_name, value)
    return None

def set_area_data(area, par_type, par_name, value):
    par_type = par_type.upper()
    if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
        return None
    if par_type in ['I', 'INT', 'INTEGER']:
        return libsteps.api_set_area_integer_data(area, par_name, value)
    if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
        return libsteps.api_set_area_float_data(area, par_name, value)
    if par_type in ['B', 'BOOL', 'BOOLEAN']:
        return libsteps.api_set_area_boolean_data(area, par_name, value)
    if par_type in ['S', 'STRING']:
        return libsteps.api_set_area_string_data(area, par_name, value)
    return None

def set_zone_data(zone, par_type, par_name, value):
    par_type = par_type.upper()
    if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
        return None
    if par_type in ['I', 'INT', 'INTEGER']:
        return libsteps.api_set_zone_integer_data(zone, par_name, value)
    if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
        return libsteps.api_set_zone_float_data(zone, par_name, value)
    if par_type in ['B', 'BOOL', 'BOOLEAN']:
        return libsteps.api_set_zone_boolean_data(zone, par_name, value)
    if par_type in ['S', 'STRING']:
        return libsteps.api_set_zone_string_data(zone, par_name, value)
    return None

def set_owner_data(owner, par_type, par_name, value):
    par_type = par_type.upper()
    if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
        return None
    if par_type in ['I', 'INT', 'INTEGER']:
        return libsteps.api_set_owner_integer_data(owner, par_name, value)
    if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
        return libsteps.api_set_owner_float_data(owner, par_name, value)
    if par_type in ['B', 'BOOL', 'BOOLEAN']:
        return libsteps.api_set_owner_boolean_data(owner, par_name, value)
    if par_type in ['S', 'STRING']:
        return libsteps.api_set_owner_string_data(owner, par_name, value)
    return None

def get_generator_related_model_name(generator, model_type):
    return libsteps.api_get_generator_related_model_name(generator[0], generator[1], model_type)

def get_generator_related_model_data(generator, model_type, par_type, par_name):
    par_type = par_type.upper()
    if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN']:
        return None
    if par_type in ['I', 'INT', 'INTEGER']:
        return 0
    if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
        return libsteps.api_get_generator_related_model_float_parameter(generator[0], generator[1], model_type, par_name)
    if par_type in ['B', 'BOOL', 'BOOLEAN']:
        return False
    return None
    
def set_generator_related_model_data(generator, model_type, par_type, par_name, value):
    par_type = par_type.upper()
    if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN']:
        return
    if par_type in ['I', 'INT', 'INTEGER']:
        return
    if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
        return libsteps.api_set_generator_related_model_float_parameter(generator[0], generator[1], model_type, par_name, value)
    if par_type in ['B', 'BOOL', 'BOOLEAN']:
        return
    return
    
def get_powerflow_solver_parameter(par_type, par_name):
    par_type = par_type.upper()
    if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN']:
        return None
    if par_type in ['I', 'INT', 'INTEGER']:
        return libsteps.api_get_powerflow_solver_integer_parameter(par_name)
    if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
        return libsteps.api_get_powerflow_solver_float_parameter(par_name)
    if par_type in ['B', 'BOOL', 'BOOLEAN']:
        return libsteps.api_get_powerflow_solver_boolean_parameter(par_name)
    return None

def set_powerflow_solver_parameter(par_type, par_name, value):
    par_type = par_type.upper()
    if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN']:
        return None
    if par_type in ['I', 'INT', 'INTEGER']:
        return libsteps.api_set_powerflow_solver_integer_parameter(par_name, value)
    if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
        return libsteps.api_set_powerflow_solver_float_parameter(par_name, value)
    if par_type in ['B', 'BOOL', 'BOOLEAN']:
        return libsteps.api_set_powerflow_solver_boolean_parameter(par_name, value)
    return None
    
def solve_powerflow(method):
    libsteps.api_solve_powerflow(method)
    return

def is_powerflow_converged():
    return libsteps.api_is_powerflow_converged()
    
def show_powerflow_result():
    libsteps.api_show_powerflow_result()
    return
    
def save_powerflow_result(file):
    libsteps.api_save_powerflow_result(file)
    return
    
def save_network_matrix(file):
    libsteps.api_save_network_matrix(file)
    return
    
def save_jacobian_matrix(file):
    libsteps.api_save_jacobian_matrix(file)
    return
    
    
def get_dynamic_simulator_parameter(par_type, par_name):
    par_type = par_type.upper()
    if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
        return None
    if par_type in ['I', 'INT', 'INTEGER']:
        return libsteps.api_get_dynamic_simulator_integer_parameter(par_name)
    if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
        return libsteps.api_get_dynamic_simulator_float_parameter(par_name)
    if par_type in ['B', 'BOOL', 'BOOLEAN']:
        return libsteps.api_get_dynamic_simulator_boolean_parameter(par_name)
    if par_type in ['S', 'STRING']:
        return libsteps.api_get_dynamic_simulator_string_parameter(par_name)
    return None

def set_dynamic_simulator_parameter(par_type, par_name, value):
    par_type = par_type.upper()
    if par_type not in ['I', 'INT', 'INTEGER', 'F', 'D', 'FLOAT', 'DOUBLE', 'B', 'BOOL', 'BOOLEAN', 'S', 'STRING']:
        return None
    if par_type in ['I', 'INT', 'INTEGER']:
        return libsteps.api_set_dynamic_simulator_integer_parameter(par_name, value)
    if par_type in ['F', 'D', 'FLOAT', 'DOUBLE']:
        return libsteps.api_set_dynamic_simulator_float_parameter(par_name, value)
    if par_type in ['B', 'BOOL', 'BOOLEAN']:
        return libsteps.api_set_dynamic_simulator_boolean_parameter(par_name, value)
    if par_type in ['S', 'STRING']:
        return libsteps.api_set_dynamic_simulator_string_parameter(par_name, value)
    return None
    
def get_dynamic_simulator_output_file():
    return libsteps.api_get_dynamic_simulator_output_file()
    
def set_dynamic_simulator_output_file(file):
    return libsteps.api_set_dynamic_simulator_output_file(file)
    
def get_dynamic_simulation_time_step():
    return libsteps.api_get_dynamic_simulation_time_step()
    
def set_dynamic_simulation_time_step(step):
    return libsteps.api_set_dynamic_simulation_time_step(step)
    
def prepare_meters(device_type):
    device_type = device_type.upper()
    if device_type not in ['ALL', 'BUS', 'GENERATOR', 'LOAD', 'LINE', 'HVDC', 'EQUIVALENT DEVICE']:
        return
    if device_type in ['ALL']:
        libsteps.api_prepare_meters()
        return
    if device_type in ['BUS']:
        libsteps.api_prepare_bus_related_meters()
        return
    if device_type in ['GENERATOR']:
        libsteps.api_prepare_generator_related_meters()
        return
    if device_type in ['LOAD']:
        libsteps.api_prepare_load_related_meters()
        return
    if device_type in ['LINE']:
        libsteps.api_prepare_line_related_meters()
        return
    if device_type in ['HVDC']:
        libsteps.api_prepare_hvdc_related_meters()
        return
    if device_type in ['EQUIVALENT DEVICE']:
        libsteps.api_prepare_equivalent_device_related_meters()
        return
    return

def prepare_bus_meter(bus, meter_type):
    libsteps.api_prepare_bus_related_meter(bus, meter_type)
    return

def prepare_generator_meter(generator, meter_type):
    bus = generator[0]
    id = generator[1]
    libsteps.api_prepare_generator_related_meter(bus, id, meter_type)
    return

def prepare_wt_generator_meter(generator, meter_type):
    bus = generator[0]
    id = generator[1]
    libsteps.api_prepare_wt_generator_related_meter(bus, id, meter_type)
    return

def prepare_load_meter(load, meter_type):
    bus = load[0]
    id = load[1]
    libsteps.api_prepare_load_related_meter(bus, id, meter_type)
    return

def prepare_line_meter(line, meter_type, side):
    ibus = line[0]
    jbus = line[1]
    id = line[2]
    libsteps.api_prepare_line_related_meter(ibus, jbus, id, meter_type, side)
    return

def prepare_hvdc_meter(hvdc, meter_type, side):
    ibus = hvdc[0]
    jbus = hvdc[1]
    id = hvdc[2]
    libsteps.api_prepare_hvdc_related_meter(ibus, jbus, id, meter_type, side)
    return

def prepare_equivalent_device_meter(edevice, meter_type):
    bus = edevice[0]
    id = edevice[1]
    libsteps.api_prepare_equivalent_device_related_meter(bus, id, meter_type)
    return

def start_dynamic_simulation():
    libsteps.api_start_dynamic_simulation()
    return

def stop_dynamic_simulation():
    libsteps.api_stop_dynamic_simulation()
    return

def run_dynamic_simulation_to_time(time):
    libsteps.api_run_simulation_to_time(time)
    return

def run_a_step():
    libsteps.api_run_a_step()
    return

def set_bus_fault(bus, fault_type, fault_shunt):
    libsteps.api_set_bus_fault(bus, fault_type, fault_shunt[0], fault_shunt[1])
    return

def clear_bus_fault(bus, fault_type):
    libsteps.api_clear_bus_fault(bus, fault_type)
    return

def trip_bus(bus):
    libsteps.api_trip_bus(bus)
    return

def set_line_fault(line, fault_type, fault_location, fault_shunt):
    libsteps.api_set_line_fault(line[0], line[1], line[2], fault_type, fault_location, fault_shunt[0], fault_shunt[1])
    return

def clear_line_fault(line, fault_type, fault_location):
    libsteps.api_clear_line_fault(line[0], line[1], line[2], fault_type, fault_location)
    return

def trip_line(line):
    libsteps.api_trip_line(line[0], line[1], line[2])
    return

def trip_line_breaker(line):
    libsteps.api_trip_line_breaker(line[0], line[1], line[2])
    return
    

def close_line(line):
    libsteps.api_close_line(line[0], line[1], line[2])
    return

def close_line_breaker(line):
    libsteps.api_close_line_breaker(line[0], line[1], line[2])
    return
    

def trip_transformer(transformer):
    libsteps.api_trip_transformer(transformer[0], transformer[1], transformer[2], transformer[3])
    return

def trip_transformer_breaker(transformer):
    libsteps.api_trip_transformer_breaker(transformer[0], transformer[1], transformer[2], transformer[3])
    return
    

def close_transformer(transformer):
    libsteps.api_close_transformer(transformer[0], transformer[1], transformer[2], transformer[3])
    return

def close_transformer_breaker(transformer):
    libsteps.api_close_transformer_breaker(transformer[0], transformer[1], transformer[2], transformer[3])
    return

def trip_generator(generator):
    libsteps.api_trip_generator(generator[0], generator[1])
    return

def shed_generator(generator, percent):
    libsteps.api_shed_generator(generator[0], generator[1], percent)
    return

def trip_load(load):
    libsteps.api_trip_load(load[0], load[1])
    return

def close_load(load):
    libsteps.api_close_load(load[0], load[1])
    return

def scale_load(load, percent):
    libsteps.api_shed_load(load[0], load[1], percent)
    return

def scale_all_loads(ercent):
    libsteps.api_shed_all_loads(percent)
    return

def trip_fixed_shunt(shunt):
    libsteps.api_trip_fixed_shunt(shunt[0], shunt[1])
    return

def close_fixed_shunt(shunt):
    libsteps.api_close_fixed_shunt(shunt[0], shunt[1])
    return

def manually_bypass_hvdc(hvdc):
    libsteps.api_manually_bypass_hvdc(hvdc[0], hvdc[1], hvdc[2])
    return

def manually_block_hvdc(hvdc):
    libsteps.api_manually_block_hvdc(hvdc[0], hvdc[1], hvdc[2])
    return

def manually_unbypass_hvdc(hvdc):
    libsteps.api_manually_unbypass_hvdc(hvdc[0], hvdc[1], hvdc[2])
    return

def manually_unblock_hvdc(hvdc):
    libsteps.api_manually_unblock_hvdc(hvdc[0], hvdc[1], hvdc[2])
    return

def get_generator_voltage_reference_in_pu(generator):
    return libsteps.api_get_generator_voltage_reference_in_pu(generator[0], generator[1])

def get_generator_power_reference_in_MW(generator):
    return libsteps.api_get_generator_power_reference_in_MW(generator[0], generator[1])

def set_generator_voltage_reference_in_pu(generator, value):
    return libsteps.api_set_generator_voltage_reference_in_pu(generator[0], generator[1], value)

def set_generator_power_reference_in_MW(generator, value):
    return libsteps.api_set_generator_power_reference_in_MW(generator[0], generator[1], value)

    

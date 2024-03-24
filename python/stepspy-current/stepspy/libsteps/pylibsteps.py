from ctypes import *
import platform
import os

def get_base_library(libsteps_file):
    dirname, filename = os.path.split(os.path.abspath(__file__))
    dirname = dirname.replace('\\','/')
    if not dirname.endswith('/'):
        dirname = dirname + '/'
    
    libsteps_name = libsteps_file
    libsteps_extension = '.so'
    if platform.system()=="Linux" or platform.system()=="Unix":
        libsteps_extension = '.so'
    elif platform.system()=="Windows":
        libsteps_extension = '.dll'
        
    library = dirname+libsteps_name+libsteps_extension
    if libsteps_file != "libSTEPS":
        print("Attention. You are trying to load a base library other than default libSTEPS: ", library)
    if not os.path.exists(library):
        info = "Warning. Library "+library+" does not exist. No applications with stepspy will be enabled.\n"
        info += "Please go to https://github.com/changgang/steps to download the proper version or build it from scratch.\n"
        info += "After that, you should put the dynamic library to the exact location of "+library+".\n"
        info += "Remember, keep the file name exactly as libSTEPS.so or libSTEPS.dll"
        print(info)
        library = None

    return library
    
def load_library(libsteps_file):
    library = get_base_library(libsteps_file)
    if library is not None:
        libsteps = cdll.LoadLibrary(library)
    else:
        return None

    libsteps.api_get_api_major_version.restype = c_uint
    libsteps.api_get_api_major_version.argtype = None
    libsteps.api_get_api_minor_version.restype = c_uint
    libsteps.api_get_api_minor_version.argtype = None
    libsteps.api_get_api_patch_version.restype = c_uint
    libsteps.api_get_api_patch_version.argtype = None

    major_version = libsteps.api_get_api_major_version()
    minor_version = libsteps.api_get_api_minor_version()
    patch_version = libsteps.api_get_api_patch_version()

    required_major_version = 1
    required_minor_version = 5
    required_patch_version = 2

    valid_flag = major_version >= required_major_version
    if valid_flag==True and major_version==required_major_version:
        valid_flag = valid_flag and minor_version>=required_minor_version
    if valid_flag==True and major_version==required_major_version and minor_version==required_minor_version:
        valid_flag = valid_flag and patch_version>=required_patch_version
    
    if valid_flag==False:
        print("Invalid libsteps version {}.{}.{}. Version >= {}.{}.{} is required.".format(
            major_version, minor_version, patch_version, required_major_version, required_minor_version, required_patch_version))
        return None

    libsteps.api_get_const_INDEX_NOT_EXIST.restype = c_uint
    libsteps.api_get_const_INDEX_NOT_EXIST.argtype = None
    
    libsteps.api_set_toolkit_log_file.restype = None
    libsteps.api_set_toolkit_log_file.argtypes = (c_char_p, c_bool, c_uint)
        
    libsteps.api_generate_new_toolkit.restype = c_uint
    libsteps.api_generate_new_toolkit.argtypes = None
    
    libsteps.api_delete_toolkit.restype = None
    libsteps.api_delete_toolkit.argtypes = (c_uint, )
    
    libsteps.api_initialize_toolkit.restype = None
    libsteps.api_initialize_toolkit.argtypes = (c_uint, )
    
    libsteps.api_clear_toolkit.restype = None
    libsteps.api_clear_toolkit.argtypes = (c_uint, )
    
    libsteps.api_set_toolkit_parallel_thread_number.restype = None
    libsteps.api_set_toolkit_parallel_thread_number.argtypes = (c_uint, c_uint)
    
    libsteps.api_get_toolkit_parallel_thread_number.restype = c_uint
    libsteps.api_get_toolkit_parallel_thread_number.argtypes = (c_uint, )   
    
    libsteps.api_set_toolkit_dynamic_model_database_capacity.restype = None
    libsteps.api_set_toolkit_dynamic_model_database_capacity.argtypes = (c_uint, c_uint)
    
    libsteps.api_get_toolkit_dynamic_model_database_capacity.restype = c_uint
    libsteps.api_get_toolkit_dynamic_model_database_capacity.argtypes = (c_uint, )    
    
    libsteps.api_get_toolkit_float_data.restype = c_double
    libsteps.api_get_toolkit_float_data.argtypes = (c_char_p, c_uint)
    
    libsteps.api_set_toolkit_float_data.restype = None
    libsteps.api_set_toolkit_float_data.argtypes = (c_char_p, c_double, c_uint)
    
    libsteps.api_get_toolkit_string_data.restype = c_char_p
    libsteps.api_get_toolkit_string_data.argtypes = (c_char_p, c_uint)
    
    libsteps.api_set_toolkit_string_data.restype = None
    libsteps.api_set_toolkit_string_data.argtypes = (c_char_p, c_char_p, c_uint)
    
    libsteps.api_get_toolkit_bool_data.restype = c_bool
    libsteps.api_get_toolkit_bool_data.argtypes = (c_char_p, c_uint)
    
    libsteps.api_set_toolkit_bool_data.restype = None
    libsteps.api_set_toolkit_bool_data.argtypes = (c_char_p, c_bool, c_uint)

    libsteps.api_load_powerflow_data_from_file.restype = None
    libsteps.api_load_powerflow_data_from_file.argtypes = (c_char_p, c_char_p, c_uint)
    libsteps.api_load_vsc_hvdc_powerflow_data_from_file.restype = None
    libsteps.api_load_vsc_hvdc_powerflow_data_from_file.argtypes = (c_char_p, c_char_p, c_uint)   
    libsteps.api_load_powerflow_result_from_file.restype = None
    libsteps.api_load_powerflow_result_from_file.argtypes = (c_char_p, c_char_p, c_uint)
    libsteps.api_load_sequence_data_from_file.restype = None
    libsteps.api_load_sequence_data_from_file.argtypes = (c_char_p, c_char_p, c_uint)
    libsteps.api_load_dynamic_data_from_file.restype = None
    libsteps.api_load_dynamic_data_from_file.argtypes = (c_char_p, c_char_p, c_uint)

    libsteps.api_save_powerflow_data_to_file.restype = None
    libsteps.api_save_powerflow_data_to_file.argtypes = (c_char_p, c_char_p, c_bool, c_bool, c_bool, c_uint, c_uint)
    libsteps.api_save_vsc_hvdc_powerflow_data_to_file.restype = None
    libsteps.api_save_vsc_hvdc_powerflow_data_to_file.argtypes = (c_char_p, c_char_p, c_bool, c_bool, c_bool, c_uint, c_uint)
    libsteps.api_save_sequence_data_to_file.restype = None
    libsteps.api_save_sequence_data_to_file.argtypes = (c_char_p, c_char_p, c_uint)
    libsteps.api_save_dynamic_data_to_file.restype = None
    libsteps.api_save_dynamic_data_to_file.argtypes = (c_char_p, c_char_p, c_bool, c_uint)
    
    libsteps.api_check_powerflow_data.restype = None
    libsteps.api_check_powerflow_data.argtypes = (c_uint, )
    
    libsteps.api_check_dynamic_data.restype = None
    libsteps.api_check_dynamic_data.argtypes = (c_uint, )
    
    libsteps.api_check_missing_models.restype = None
    libsteps.api_check_missing_models.argtypes = (c_uint, )
    
    libsteps.api_check_least_dynamic_time_constants.restype = None
    libsteps.api_check_least_dynamic_time_constants.argtypes = (c_uint, )
    
    libsteps.api_check_network_connectivity.restype = None
    libsteps.api_check_network_connectivity.argtypes = (c_bool, c_uint)

    libsteps.api_get_allowed_maximum_bus_number.restype = c_uint
    libsteps.api_get_allowed_maximum_bus_number.argtypes = (c_uint, )

    libsteps.api_set_allowed_maximum_bus_number.restype = None
    libsteps.api_set_allowed_maximum_bus_number.argtypes = (c_uint, c_uint)

    libsteps.api_get_device_capacity.restype = c_uint
    libsteps.api_get_device_capacity.argtypes = (c_char_p, c_uint)
    libsteps.api_get_area_capacity.restype = c_uint
    libsteps.api_get_area_capacity.argtypes = (c_uint, )
    libsteps.api_get_zone_capacity.restype = c_uint
    libsteps.api_get_zone_capacity.argtypes = (c_uint, )
    libsteps.api_get_owner_capacity.restype = c_uint
    libsteps.api_get_owner_capacity.argtypes = (c_uint, )

    libsteps.api_set_device_capacity.restype = None
    libsteps.api_set_device_capacity.argtypes = (c_char_p, c_uint, c_uint)
    libsteps.api_set_area_capacity.restype = None
    libsteps.api_set_area_capacity.argtypes = (c_uint,  c_uint)
    libsteps.api_set_zone_capacity.restype = None
    libsteps.api_set_zone_capacity.argtypes = (c_uint,  c_uint)
    libsteps.api_set_owner_capacity.restype = None
    libsteps.api_set_owner_capacity.argtypes = (c_uint,  c_uint)

    libsteps.api_get_device_count.restype = c_uint
    libsteps.api_get_device_count.argtypes = (c_char_p, c_uint)
    libsteps.api_get_area_count.restype = c_uint
    libsteps.api_get_area_count.argtypes = (c_uint, )
    libsteps.api_get_zone_count.restype = c_uint
    libsteps.api_get_zone_count.argtypes = (c_uint, )
    libsteps.api_get_owner_count.restype = c_uint
    libsteps.api_get_owner_count.argtypes = (c_uint, )
    libsteps.api_get_in_service_bus_count.restype = c_uint
    libsteps.api_get_in_service_bus_count.argtypes = (c_uint, )
    libsteps.api_update_overshadowed_buses.restype = None
    libsteps.api_update_overshadowed_buses.argtypes = (c_uint, )
    libsteps.api_set_all_buses_un_overshadowed.restype = None
    libsteps.api_set_all_buses_un_overshadowed.argtypes = (c_uint, )
    libsteps.api_get_overshadowed_bus_count.restype = c_uint
    libsteps.api_get_overshadowed_bus_count.argtypes = (c_uint, )

    libsteps.api_is_bus_exist.restype = c_bool
    libsteps.api_is_bus_exist.argtypes = (c_uint, c_uint)
    libsteps.api_is_generator_exist.restype = c_bool
    libsteps.api_is_generator_exist.argtypes = (c_uint, c_char_p, c_uint)
    libsteps.api_is_wt_generator_exist.restype = c_bool
    libsteps.api_is_wt_generator_exist.argtypes = (c_uint, c_char_p, c_uint)
    libsteps.api_is_pv_unit_exist.restype = c_bool
    libsteps.api_is_pv_unit_exist.argtypes = (c_uint, c_char_p, c_uint)
    libsteps.api_is_energy_storage_exist.restype = c_bool
    libsteps.api_is_energy_storage_exist.argtypes = (c_uint, c_char_p, c_uint)
    libsteps.api_is_load_exist.restype = c_bool
    libsteps.api_is_load_exist.argtypes = (c_uint, c_char_p, c_uint)
    libsteps.api_is_fixed_shunt_exist.restype = c_bool
    libsteps.api_is_fixed_shunt_exist.argtypes = (c_uint, c_char_p, c_uint)
    libsteps.api_is_line_exist.restype = c_bool
    libsteps.api_is_line_exist.argtypes = (c_uint, c_uint, c_char_p, c_uint)
    libsteps.api_is_transformer_exist.restype = c_bool
    libsteps.api_is_transformer_exist.argtypes = (c_uint, c_uint, c_uint, c_char_p, c_uint)
    libsteps.api_is_hvdc_exist.restype = c_bool
    libsteps.api_is_hvdc_exist.argtypes = (c_uint, c_uint, c_char_p, c_uint)
    libsteps.api_is_vsc_hvdc_exist.restype = c_bool
    libsteps.api_is_vsc_hvdc_exist.argtypes = (c_char_p, c_uint)    
    libsteps.api_is_equivalent_device_exist.restype = c_bool
    libsteps.api_is_equivalent_device_exist.argtypes = (c_uint, c_char_p, c_uint)

    libsteps.api_show_device_data.restype = None
    libsteps.api_show_device_data.argtypes = (c_char_p, c_uint)

    libsteps.api_bus_number2bus_name.restype = c_char_p
    libsteps.api_bus_number2bus_name.argtypes = (c_uint, c_uint)
    libsteps.api_bus_name2bus_number.restype = c_uint
    libsteps.api_bus_name2bus_number.argtypes = (c_char_p, c_double, c_uint)

    libsteps.api_initialize_bus_search.restype = None
    libsteps.api_initialize_bus_search.argtypes = (c_double, c_double, c_double, c_double, c_uint, c_uint, c_uint, c_uint)
    libsteps.api_initialize_all_bus_search.restype = None
    libsteps.api_initialize_all_bus_search.argtypes = (c_uint, )

    libsteps.api_get_current_bus_number.restype = c_uint
    libsteps.api_get_current_bus_number.argtypes = (c_uint, )

    libsteps.api_goto_next_bus.restype = None
    libsteps.api_goto_next_bus.argtypes = (c_uint, )

    libsteps.api_initialize_device_search.restype = None
    libsteps.api_initialize_device_search.argtypes = (c_char_p, c_uint, c_uint)

    libsteps.api_get_current_device_bus_number.restype = c_uint
    libsteps.api_get_current_device_bus_number.argtypes = (c_char_p, c_char_p, c_uint)

    libsteps.api_get_current_device_identifier.restype = c_char_p
    libsteps.api_get_current_device_identifier.argtypes = (c_char_p, c_uint)

    libsteps.api_goto_next_device.restype = None
    libsteps.api_goto_next_device.argtypes = (c_char_p, c_uint)

    libsteps.api_initialize_area_search.restype = None
    libsteps.api_initialize_area_search.argtypes = (c_uint, )

    libsteps.api_get_current_area_number.restype = c_uint
    libsteps.api_get_current_area_number.argtypes = (c_uint, )

    libsteps.api_goto_next_area.restype = None
    libsteps.api_goto_next_area.argtypes = (c_uint, )

    libsteps.api_initialize_zone_search.restype = None
    libsteps.api_initialize_zone_search.argtypes = (c_uint, )

    libsteps.api_get_current_zone_number.restype = c_uint
    libsteps.api_get_current_zone_number.argtypes = (c_uint, )

    libsteps.api_goto_next_zone.restype = None
    libsteps.api_goto_next_zone.argtypes = (c_uint, )

    libsteps.api_initialize_owner_search.restype = None
    libsteps.api_initialize_owner_search.argtypes = (c_uint, )

    libsteps.api_get_current_owner_number.restype = c_uint
    libsteps.api_get_current_owner_number.argtypes = (c_uint, )

    libsteps.api_goto_next_owner.restype = None
    libsteps.api_goto_next_owner.argtypes = (c_uint, )

    libsteps.api_add_bus.restype = None
    libsteps.api_add_bus.argtypes = (c_uint, c_char_p, c_double, c_uint)
    libsteps.api_add_generator.restype = None
    libsteps.api_add_generator.argtypes = (c_uint, c_char_p, c_uint)
    libsteps.api_add_wt_generator.restype = None
    libsteps.api_add_wt_generator.argtypes = (c_uint, c_char_p, c_uint)
    libsteps.api_add_pv_unit.restype = None
    libsteps.api_add_pv_unit.argtypes = (c_uint, c_char_p, c_uint)
    libsteps.api_add_energy_storage.restype = None
    libsteps.api_add_energy_storage.argtypes = (c_uint, c_char_p, c_uint)
    libsteps.api_add_load.restype = None
    libsteps.api_add_load.argtypes = (c_uint, c_char_p, c_uint)
    libsteps.api_add_fixed_shunt.restype = None
    libsteps.api_add_fixed_shunt.argtypes = (c_uint, c_char_p, c_uint)
    libsteps.api_add_line.restype = None
    libsteps.api_add_line.argtypes = (c_uint, c_uint, c_char_p, c_uint)
    libsteps.api_add_hvdc.restype = None
    libsteps.api_add_hvdc.argtypes = (c_uint, c_uint, c_char_p, c_uint)
    libsteps.api_add_vsc_hvdc.restype = None
    libsteps.api_add_vsc_hvdc.argtypes = (c_char_p, c_uint)   
    libsteps.api_add_transformer.restype = None
    libsteps.api_add_transformer.argtypes = (c_uint, c_uint, c_uint, c_char_p, c_uint)
    libsteps.api_add_equivalent_device.restype = None
    libsteps.api_add_equivalent_device.argtypes = (c_uint, c_char_p, c_uint)
    libsteps.api_add_area.restype = None
    libsteps.api_add_area.argtypes = (c_uint, c_char_p, c_uint)
    libsteps.api_add_zone.restype = None
    libsteps.api_add_zone.argtypes = (c_uint, c_char_p, c_uint)
    libsteps.api_add_owner.restype = None
    libsteps.api_add_owner.argtypes = (c_uint, c_char_p, c_uint)

    libsteps.api_remove_bus.restype = None
    libsteps.api_remove_bus.argtypes = (c_uint, c_uint)
    libsteps.api_remove_generator.restype = None
    libsteps.api_remove_generator.argtypes = (c_uint, c_char_p, c_uint)
    libsteps.api_remove_wt_generator.restype = None
    libsteps.api_remove_wt_generator.argtypes = (c_uint, c_char_p, c_uint)
    libsteps.api_remove_pv_unit.restype = None
    libsteps.api_remove_pv_unit.argtypes = (c_uint, c_char_p, c_uint)
    libsteps.api_remove_energy_storage.restype = None
    libsteps.api_remove_energy_storage.argtypes = (c_uint, c_char_p, c_uint)
    libsteps.api_remove_load.restype = None
    libsteps.api_remove_load.argtypes = (c_uint, c_char_p, c_uint)
    libsteps.api_remove_fixed_shunt.restype = None
    libsteps.api_remove_fixed_shunt.argtypes = (c_uint, c_char_p, c_uint)
    libsteps.api_remove_line.restype = None
    libsteps.api_remove_line.argtypes = (c_uint, c_uint, c_char_p, c_uint)
    libsteps.api_remove_hvdc.restype = None
    libsteps.api_remove_hvdc.argtypes = (c_uint, c_uint, c_char_p, c_uint)
    libsteps.api_remove_vsc_hvdc.restype = None
    libsteps.api_remove_vsc_hvdc.argtypes = (c_char_p, c_uint)  
    libsteps.api_remove_transformer.restype = None
    libsteps.api_remove_transformer.argtypes = (c_uint, c_uint, c_uint, c_char_p, c_uint)
    libsteps.api_remove_equivalent_device.restype = None
    libsteps.api_remove_equivalent_device.argtypes = (c_uint, c_char_p, c_uint)
    libsteps.api_remove_area.restype = None
    libsteps.api_remove_area.argtypes = (c_uint, c_uint)
    libsteps.api_remove_zone.restype = None
    libsteps.api_remove_zone.argtypes = (c_uint, c_uint)
    libsteps.api_remove_owner.restype = None
    libsteps.api_remove_owner.argtypes = (c_uint, c_uint)
    
    
    libsteps.api_change_bus_number.restype = None
    libsteps.api_change_bus_number.argtypes = (c_uint, c_uint, c_uint)    
    libsteps.api_change_bus_number_with_file.restype = None
    libsteps.api_change_bus_number_with_file.argtypes = (c_char_p, c_uint)


    libsteps.api_get_bus_integer_data.restype = (c_int)
    libsteps.api_get_bus_integer_data.argtypes = (c_uint, c_char_p, c_uint)
    libsteps.api_get_bus_float_data.restype = (c_double)
    libsteps.api_get_bus_float_data.argtypes = (c_uint, c_char_p, c_uint)
    libsteps.api_get_bus_string_data.restype = (c_char_p)
    libsteps.api_get_bus_string_data.argtypes = (c_uint, c_char_p, c_uint)
    libsteps.api_set_bus_integer_data.restype = None
    libsteps.api_set_bus_integer_data.argtypes = (c_uint, c_char_p, c_int, c_uint)
    libsteps.api_set_bus_float_data.restype = None
    libsteps.api_set_bus_float_data.argtypes = (c_uint, c_char_p, c_double, c_uint)
    libsteps.api_set_bus_string_data.restype = None
    libsteps.api_set_bus_string_data.argtypes = (c_uint, c_char_p, c_char_p, c_uint)

    libsteps.api_get_load_integer_data.restype = (c_int)
    libsteps.api_get_load_integer_data.argtypes = (c_uint, c_char_p, c_char_p, c_uint)
    libsteps.api_get_load_float_data.restype = (c_double)
    libsteps.api_get_load_float_data.argtypes = (c_uint, c_char_p, c_char_p, c_uint)
    libsteps.api_get_load_string_data.restype = (c_char_p)
    libsteps.api_get_load_string_data.argtypes = (c_uint, c_char_p, c_char_p, c_uint)
    libsteps.api_get_load_boolean_data.restype = (c_bool)
    libsteps.api_get_load_boolean_data.argtypes = (c_uint, c_char_p, c_char_p, c_uint)
    libsteps.api_set_load_integer_data.restype = None
    libsteps.api_set_load_integer_data.argtypes = (c_uint, c_char_p, c_char_p, c_int, c_uint)
    libsteps.api_set_load_float_data.restype = None
    libsteps.api_set_load_float_data.argtypes = (c_uint, c_char_p, c_char_p, c_double, c_uint)
    libsteps.api_set_load_string_data.restype = None
    libsteps.api_set_load_string_data.argtypes = (c_uint, c_char_p, c_char_p, c_char_p, c_uint)
    libsteps.api_set_load_boolean_data.restype = None
    libsteps.api_set_load_boolean_data.argtypes = (c_uint, c_char_p, c_char_p, c_bool, c_uint)

    libsteps.api_get_source_integer_data.restype = (c_int)
    libsteps.api_get_source_integer_data.argtypes = (c_uint, c_char_p, c_char_p, c_uint)
    libsteps.api_get_source_float_data.restype = (c_double)
    libsteps.api_get_source_float_data.argtypes = (c_uint, c_char_p, c_char_p, c_uint)
    libsteps.api_get_source_string_data.restype = (c_char_p)
    libsteps.api_get_source_string_data.argtypes = (c_uint, c_char_p, c_char_p, c_uint)
    libsteps.api_get_source_boolean_data.restype = (c_bool)
    libsteps.api_get_source_boolean_data.argtypes = (c_uint, c_char_p, c_char_p, c_uint)
    libsteps.api_set_source_integer_data.restype = None
    libsteps.api_set_source_integer_data.argtypes = (c_uint, c_char_p, c_char_p, c_int, c_uint)
    libsteps.api_set_source_float_data.restype = None
    libsteps.api_set_source_float_data.argtypes = (c_uint, c_char_p, c_char_p, c_double, c_uint)
    libsteps.api_set_source_string_data.restype = None
    libsteps.api_set_source_string_data.argtypes = (c_uint, c_char_p, c_char_p, c_char_p, c_uint)
    libsteps.api_set_source_boolean_data.restype = None
    libsteps.api_set_source_boolean_data.argtypes = (c_uint, c_char_p, c_char_p, c_bool, c_uint)

    libsteps.api_get_generator_sequence_float_data.restype = (c_double)
    libsteps.api_get_generator_sequence_float_data.argtypes = (c_uint, c_char_p, c_char_p, c_uint)
    libsteps.api_get_wt_generator_sequence_float_data.restype = (c_double)
    libsteps.api_get_wt_generator_sequence_float_data.argtypes = (c_uint, c_char_p, c_char_p, c_uint)
    libsteps.api_get_pv_unit_sequence_float_data.restype = (c_double)
    libsteps.api_get_pv_unit_sequence_float_data.argtypes = (c_uint, c_char_p, c_char_p, c_uint)
    libsteps.api_set_generator_sequence_float_data.restype = None
    libsteps.api_set_generator_sequence_float_data.argtypes = (c_uint, c_char_p, c_char_p, c_double, c_uint)
    libsteps.api_set_wt_generator_sequence_float_data.restype = None
    libsteps.api_set_wt_generator_sequence_float_data.argtypes = (c_uint, c_char_p, c_char_p, c_double, c_uint)
    libsteps.api_set_pv_unit_sequence_float_data.restype = None
    libsteps.api_set_pv_unit_sequence_float_data.argtypes = (c_uint, c_char_p, c_char_p, c_double, c_uint)

    libsteps.api_get_fixed_shunt_integer_data.restype = (c_int)
    libsteps.api_get_fixed_shunt_integer_data.argtypes = (c_uint, c_char_p, c_char_p, c_uint)
    libsteps.api_get_fixed_shunt_float_data.restype = (c_double)
    libsteps.api_get_fixed_shunt_float_data.argtypes = (c_uint, c_char_p, c_char_p, c_uint)
    libsteps.api_get_fixed_shunt_string_data.restype = (c_char_p)
    libsteps.api_get_fixed_shunt_string_data.argtypes = (c_uint, c_char_p, c_char_p, c_uint)
    libsteps.api_get_fixed_shunt_boolean_data.restype = (c_bool)
    libsteps.api_get_fixed_shunt_boolean_data.argtypes = (c_uint, c_char_p, c_char_p, c_uint)
    libsteps.api_set_fixed_shunt_integer_data.restype = None
    libsteps.api_set_fixed_shunt_integer_data.argtypes = (c_uint, c_char_p, c_char_p, c_int, c_uint)
    libsteps.api_set_fixed_shunt_float_data.restype = None
    libsteps.api_set_fixed_shunt_float_data.argtypes = (c_uint, c_char_p, c_char_p, c_double, c_uint)
    libsteps.api_set_fixed_shunt_string_data.restype = None
    libsteps.api_set_fixed_shunt_string_data.argtypes = (c_uint, c_char_p, c_char_p, c_char_p, c_uint)
    libsteps.api_set_fixed_shunt_boolean_data.restype = None
    libsteps.api_set_fixed_shunt_boolean_data.argtypes = (c_uint, c_char_p, c_char_p, c_bool, c_uint)

    libsteps.api_get_line_integer_data.restype = (c_int)
    libsteps.api_get_line_integer_data.argtypes = (c_uint, c_uint, c_char_p, c_char_p, c_uint)
    libsteps.api_get_line_float_data.restype = (c_double)
    libsteps.api_get_line_float_data.argtypes = (c_uint, c_uint, c_char_p, c_char_p, c_uint)
    libsteps.api_get_line_string_data.restype = (c_char_p)
    libsteps.api_get_line_string_data.argtypes = (c_uint, c_uint, c_char_p, c_char_p, c_uint)
    libsteps.api_get_line_boolean_data.restype = (c_bool)
    libsteps.api_get_line_boolean_data.argtypes = (c_uint, c_uint, c_char_p, c_char_p, c_uint)
    libsteps.api_set_line_integer_data.restype = None
    libsteps.api_set_line_integer_data.argtypes = (c_uint, c_uint, c_char_p, c_char_p, c_int, c_uint)
    libsteps.api_set_line_float_data.restype = None
    libsteps.api_set_line_float_data.argtypes = (c_uint, c_uint, c_char_p, c_char_p, c_double, c_uint)
    libsteps.api_set_line_string_data.restype = None
    libsteps.api_set_line_string_data.argtypes = (c_uint, c_uint, c_char_p, c_char_p, c_char_p, c_uint)
    libsteps.api_set_line_boolean_data.restype = None
    libsteps.api_set_line_boolean_data.argtypes = (c_uint, c_uint, c_char_p, c_char_p, c_bool, c_uint)

    libsteps.api_get_transformer_integer_data.restype = (c_int)
    libsteps.api_get_transformer_integer_data.argtypes = (c_uint, c_uint, c_uint, c_char_p, c_char_p, c_char_p, c_uint)
    libsteps.api_get_transformer_float_data.restype = (c_double)
    libsteps.api_get_transformer_float_data.argtypes = (c_uint, c_uint, c_uint, c_char_p, c_char_p, c_char_p, c_uint)
    libsteps.api_get_transformer_string_data.restype = (c_char_p)
    libsteps.api_get_transformer_string_data.argtypes = (c_uint, c_uint, c_uint, c_char_p, c_char_p, c_char_p, c_uint)
    libsteps.api_get_transformer_boolean_data.restype = (c_bool)
    libsteps.api_get_transformer_boolean_data.argtypes = (c_uint, c_uint, c_uint, c_char_p, c_char_p, c_char_p, c_uint)
    libsteps.api_set_transformer_integer_data.restype = None
    libsteps.api_set_transformer_integer_data.argtypes = (c_uint, c_uint, c_uint, c_char_p, c_char_p, c_char_p, c_int, c_uint)
    libsteps.api_set_transformer_float_data.restype = None
    libsteps.api_set_transformer_float_data.argtypes = (c_uint, c_uint, c_uint, c_char_p, c_char_p, c_char_p, c_double, c_uint)
    libsteps.api_set_transformer_string_data.restype = None
    libsteps.api_set_transformer_string_data.argtypes = (c_uint, c_uint, c_uint, c_char_p, c_char_p, c_char_p, c_char_p, c_uint)
    libsteps.api_set_transformer_boolean_data.restype = None
    libsteps.api_set_transformer_boolean_data.argtypes = (c_uint, c_uint, c_uint, c_char_p, c_char_p, c_char_p, c_bool, c_uint)

    libsteps.api_get_hvdc_integer_data.restype = (c_int)
    libsteps.api_get_hvdc_integer_data.argtypes = (c_uint, c_uint, c_char_p, c_char_p, c_char_p, c_uint)
    libsteps.api_get_hvdc_float_data.restype = (c_double)
    libsteps.api_get_hvdc_float_data.argtypes = (c_uint, c_uint, c_char_p, c_char_p, c_char_p, c_uint)
    libsteps.api_get_hvdc_string_data.restype = (c_char_p)
    libsteps.api_get_hvdc_string_data.argtypes = (c_uint, c_uint, c_char_p, c_char_p, c_char_p, c_uint)
    libsteps.api_get_hvdc_boolean_data.restype = (c_bool)
    libsteps.api_get_hvdc_boolean_data.argtypes = (c_uint, c_uint, c_char_p, c_char_p, c_char_p, c_uint)
    libsteps.api_set_hvdc_integer_data.restype = None
    libsteps.api_set_hvdc_integer_data.argtypes = (c_uint, c_uint, c_char_p, c_char_p, c_char_p, c_int, c_uint)
    libsteps.api_set_hvdc_float_data.restype = None
    libsteps.api_set_hvdc_float_data.argtypes = (c_uint, c_uint, c_char_p, c_char_p, c_char_p, c_double, c_uint)
    libsteps.api_set_hvdc_string_data.restype = None
    libsteps.api_set_hvdc_string_data.argtypes = (c_uint, c_uint, c_char_p, c_char_p, c_char_p, c_char_p, c_uint)
    libsteps.api_set_hvdc_boolean_data.restype = None
    libsteps.api_set_hvdc_boolean_data.argtypes = (c_uint, c_uint, c_char_p, c_char_p, c_char_p, c_bool, c_uint)

    libsteps.api_get_vsc_hvdc_integer_data.restype = (c_int)
    libsteps.api_get_vsc_hvdc_integer_data.argtypes = (c_char_p, c_char_p, c_char_p, c_uint, c_uint)
    libsteps.api_get_vsc_hvdc_float_data.restype = (c_double)
    libsteps.api_get_vsc_hvdc_float_data.argtypes = (c_char_p, c_char_p, c_char_p, c_uint, c_uint)
    libsteps.api_get_vsc_hvdc_string_data.restype = (c_char_p)
    libsteps.api_get_vsc_hvdc_string_data.argtypes = (c_char_p, c_char_p, c_char_p, c_uint, c_uint)
    libsteps.api_get_vsc_hvdc_boolean_data.restype = (c_bool)
    libsteps.api_get_vsc_hvdc_boolean_data.argtypes = (c_char_p, c_char_p, c_char_p, c_uint, c_uint)
    libsteps.api_set_vsc_hvdc_integer_data.restype = None
    libsteps.api_set_vsc_hvdc_integer_data.argtypes = (c_char_p, c_char_p, c_char_p, c_int, c_uint, c_uint)
    libsteps.api_set_vsc_hvdc_float_data.restype = None
    libsteps.api_set_vsc_hvdc_float_data.argtypes = (c_char_p, c_char_p, c_char_p, c_double, c_uint, c_uint)
    libsteps.api_set_vsc_hvdc_string_data.restype = None
    libsteps.api_set_vsc_hvdc_string_data.argtypes = (c_char_p, c_char_p, c_char_p, c_char_p, c_uint, c_uint)
    libsteps.api_set_vsc_hvdc_boolean_data.restype = None
    libsteps.api_set_vsc_hvdc_boolean_data.argtypes = (c_char_p, c_char_p, c_char_p, c_bool, c_uint, c_uint)

    libsteps.api_get_area_integer_data.restype = (c_int)
    libsteps.api_get_area_integer_data.argtypes = (c_uint, c_char_p, c_uint)
    libsteps.api_get_area_float_data.restype = (c_double)
    libsteps.api_get_area_float_data.argtypes = (c_uint, c_char_p, c_uint)
    libsteps.api_get_area_string_data.restype = (c_char_p)
    libsteps.api_get_area_string_data.argtypes = (c_uint, c_char_p, c_uint)
    libsteps.api_set_area_integer_data.restype = None
    libsteps.api_set_area_integer_data.argtypes = (c_uint, c_char_p, c_int, c_uint)
    libsteps.api_set_area_float_data.restype = None
    libsteps.api_set_area_float_data.argtypes = (c_uint, c_char_p, c_double, c_uint)
    libsteps.api_set_area_string_data.restype = None
    libsteps.api_set_area_string_data.argtypes = (c_uint, c_char_p, c_char_p, c_uint)

    libsteps.api_get_zone_integer_data.restype = (c_int)
    libsteps.api_get_zone_integer_data.argtypes = (c_uint, c_char_p, c_uint)
    libsteps.api_get_zone_string_data.restype = (c_char_p)
    libsteps.api_get_zone_string_data.argtypes = (c_uint, c_char_p, c_uint)
    libsteps.api_set_zone_integer_data.restype = None
    libsteps.api_set_zone_integer_data.argtypes = (c_uint, c_char_p, c_int, c_uint)
    libsteps.api_set_zone_string_data.restype = None
    libsteps.api_set_zone_string_data.argtypes = (c_uint, c_char_p, c_char_p, c_uint)

    libsteps.api_get_owner_integer_data.restype = (c_int)
    libsteps.api_get_owner_integer_data.argtypes = (c_uint, c_char_p, c_uint)
    libsteps.api_get_owner_string_data.restype = (c_char_p)
    libsteps.api_get_owner_string_data.argtypes = (c_uint, c_char_p, c_uint)
    libsteps.api_set_owner_integer_data.restype = None
    libsteps.api_set_owner_integer_data.argtypes = (c_uint, c_char_p, c_int, c_uint)
    libsteps.api_set_owner_string_data.restype = None
    libsteps.api_set_owner_string_data.argtypes = (c_uint, c_char_p, c_char_p, c_uint)

    libsteps.api_set_dynamic_model.restype = None
    libsteps.api_set_dynamic_model.argtypes = (c_char_p, c_char_p, c_uint)
    
    libsteps.api_disable_generator_related_model.restype = None
    libsteps.api_disable_generator_related_model.argtypes = (c_uint, c_char_p, c_char_p, c_uint)
    libsteps.api_enable_generator_related_model.restype = None
    libsteps.api_enable_generator_related_model.argtypes = (c_uint, c_char_p, c_char_p, c_uint)
    
    libsteps.api_get_generator_related_model_name.restype = c_char_p
    libsteps.api_get_generator_related_model_name.argtypes = (c_uint, c_char_p, c_char_p, c_uint)
    libsteps.api_get_generator_related_model_float_parameter.restype = c_double
    libsteps.api_get_generator_related_model_float_parameter.argtypes = (c_uint, c_char_p, c_char_p, c_char_p, c_uint)
    libsteps.api_set_generator_related_model_float_parameter.restype = None
    libsteps.api_set_generator_related_model_float_parameter.argtypes = (c_uint, c_char_p, c_char_p, c_char_p, c_double, c_uint)
    libsteps.api_get_generator_related_model_float_parameter_count.restype = c_uint
    libsteps.api_get_generator_related_model_float_parameter_count.argtypes = (c_uint, c_char_p, c_char_p, c_uint)
    libsteps.api_get_generator_related_model_float_parameter_name.restype = c_char_p
    libsteps.api_get_generator_related_model_float_parameter_name.argtypes = (c_uint, c_char_p, c_char_p, c_uint, c_uint)
    libsteps.api_get_generator_related_model_internal_variable_count.restype = c_uint
    libsteps.api_get_generator_related_model_internal_variable_count.argtypes = (c_uint, c_char_p, c_char_p, c_uint)
    libsteps.api_get_generator_related_model_internal_variable_with_name.restype = c_double
    libsteps.api_get_generator_related_model_internal_variable_with_name.argtypes = (c_uint, c_char_p, c_char_p, c_char_p, c_uint)
    libsteps.api_get_generator_related_model_internal_variable_with_index.restype = c_double
    libsteps.api_get_generator_related_model_internal_variable_with_index.argtypes = (c_uint, c_char_p, c_char_p, c_uint, c_uint)
    libsteps.api_get_generator_related_model_internal_variable_name.restype = c_char_p
    libsteps.api_get_generator_related_model_internal_variable_name.argtypes = (c_uint, c_char_p, c_char_p, c_uint, c_uint)

    libsteps.api_get_wt_generator_related_model_name.restype = c_char_p
    libsteps.api_get_wt_generator_related_model_name.argtypes = (c_uint, c_char_p, c_char_p, c_uint)
    libsteps.api_get_wt_generator_related_model_float_parameter.restype = c_double
    libsteps.api_get_wt_generator_related_model_float_parameter.argtypes = (c_uint, c_char_p, c_char_p, c_char_p, c_uint)
    libsteps.api_set_wt_generator_related_model_float_parameter.restype = None
    libsteps.api_set_wt_generator_related_model_float_parameter.argtypes = (c_uint, c_char_p, c_char_p, c_char_p, c_double, c_uint)
    libsteps.api_get_wt_generator_related_model_float_parameter_count.restype = c_uint
    libsteps.api_get_wt_generator_related_model_float_parameter_count.argtypes = (c_uint, c_char_p, c_char_p, c_uint)
    libsteps.api_get_wt_generator_related_model_float_parameter_name.restype = c_char_p
    libsteps.api_get_wt_generator_related_model_float_parameter_name.argtypes = (c_uint, c_char_p, c_char_p, c_uint, c_uint)

    libsteps.api_get_pv_unit_related_model_name.restype = c_char_p
    libsteps.api_get_pv_unit_related_model_name.argtypes = (c_uint, c_char_p, c_char_p, c_uint)
    libsteps.api_get_pv_unit_related_model_float_parameter.restype = c_double
    libsteps.api_get_pv_unit_related_model_float_parameter.argtypes = (c_uint, c_char_p, c_char_p, c_char_p, c_uint)
    libsteps.api_set_pv_unit_related_model_float_parameter.restype = None
    libsteps.api_set_pv_unit_related_model_float_parameter.argtypes = (c_uint, c_char_p, c_char_p, c_char_p, c_double, c_uint)
    libsteps.api_get_pv_unit_related_model_float_parameter_count.restype = c_uint
    libsteps.api_get_pv_unit_related_model_float_parameter_count.argtypes = (c_uint, c_char_p, c_char_p, c_uint)
    libsteps.api_get_pv_unit_related_model_float_parameter_name.restype = c_char_p
    libsteps.api_get_pv_unit_related_model_float_parameter_name.argtypes = (c_uint, c_char_p, c_char_p, c_uint, c_uint)


    libsteps.api_get_load_related_model_name.restype = c_char_p
    libsteps.api_get_load_related_model_name.argtypes = (c_uint, c_char_p, c_char_p, c_uint)
    libsteps.api_get_load_related_model_float_parameter.restype = c_double
    libsteps.api_get_load_related_model_float_parameter.argtypes = (c_uint, c_char_p, c_char_p, c_char_p, c_uint)
    libsteps.api_set_load_related_model_float_parameter.restype = None
    libsteps.api_set_load_related_model_float_parameter.argtypes = (c_uint, c_char_p, c_char_p, c_char_p, c_double, c_uint)
    libsteps.api_get_load_related_model_float_parameter_count.restype = c_uint
    libsteps.api_get_load_related_model_float_parameter_count.argtypes = (c_uint, c_char_p, c_char_p, c_uint)
    libsteps.api_get_load_related_model_float_parameter_name.restype = c_char_p
    libsteps.api_get_load_related_model_float_parameter_name.argtypes = (c_uint, c_char_p, c_char_p, c_uint, c_uint)

    libsteps.api_get_line_related_model_name.restype = c_char_p
    libsteps.api_get_line_related_model_name.argtypes = (c_uint, c_uint, c_char_p, c_char_p, c_uint)
    libsteps.api_get_line_related_model_float_parameter.restype = c_double
    libsteps.api_get_line_related_model_float_parameter.argtypes = (c_uint, c_uint, c_char_p, c_char_p, c_char_p, c_uint)
    libsteps.api_set_line_related_model_float_parameter.restype = None
    libsteps.api_set_line_related_model_float_parameter.argtypes = (c_uint, c_uint, c_char_p, c_char_p, c_char_p, c_double, c_uint)
    libsteps.api_get_line_related_model_float_parameter_count.restype = c_uint
    libsteps.api_get_line_related_model_float_parameter_count.argtypes = (c_uint, c_uint, c_char_p, c_char_p, c_uint)
    libsteps.api_get_line_related_model_float_parameter_name.restype = c_char_p
    libsteps.api_get_line_related_model_float_parameter_name.argtypes = (c_uint, c_uint, c_char_p, c_char_p, c_uint, c_uint)

    libsteps.api_get_hvdc_related_model_name.restype = c_char_p
    libsteps.api_get_hvdc_related_model_name.argtypes = (c_uint, c_uint, c_char_p, c_char_p, c_uint)
    libsteps.api_get_hvdc_related_model_float_parameter.restype = c_double
    libsteps.api_get_hvdc_related_model_float_parameter.argtypes = (c_uint, c_uint, c_char_p, c_char_p, c_char_p, c_uint)
    libsteps.api_set_hvdc_related_model_float_parameter.restype = None
    libsteps.api_set_hvdc_related_model_float_parameter.argtypes = (c_uint, c_uint, c_char_p, c_char_p, c_char_p, c_double, c_uint)
    libsteps.api_get_hvdc_related_model_float_parameter_count.restype = c_uint
    libsteps.api_get_hvdc_related_model_float_parameter_count.argtypes = (c_uint, c_uint, c_char_p, c_char_p, c_uint)
    libsteps.api_get_hvdc_related_model_float_parameter_name.restype = c_char_p
    libsteps.api_get_hvdc_related_model_float_parameter_name.argtypes = (c_uint, c_uint, c_char_p, c_char_p, c_uint, c_uint)

    libsteps.api_get_vsc_hvdc_related_model_name.restype = c_char_p
    libsteps.api_get_vsc_hvdc_related_model_name.argtypes = (c_char_p, c_char_p, c_uint, c_uint)
    libsteps.api_get_vsc_hvdc_related_model_float_parameter.restype = c_double
    libsteps.api_get_vsc_hvdc_related_model_float_parameter.argtypes = (c_char_p, c_char_p, c_char_p, c_uint, c_uint)
    libsteps.api_set_vsc_hvdc_related_model_float_parameter.restype = None
    libsteps.api_set_vsc_hvdc_related_model_float_parameter.argtypes = (c_char_p, c_char_p, c_char_p, c_double, c_uint, c_uint)
    libsteps.api_get_vsc_hvdc_related_model_float_parameter_count.restype = c_uint
    libsteps.api_get_vsc_hvdc_related_model_float_parameter_count.argtypes = (c_char_p, c_char_p, c_uint, c_uint)
    libsteps.api_get_vsc_hvdc_related_model_float_parameter_name.restype = c_char_p
    libsteps.api_get_vsc_hvdc_related_model_float_parameter_name.argtypes = (c_char_p, c_char_p, c_uint, c_uint, c_uint)

    libsteps.api_get_powerflow_solver_integer_parameter.restype = (c_uint)
    libsteps.api_get_powerflow_solver_integer_parameter.argtypes = (c_char_p, c_uint)
    libsteps.api_get_powerflow_solver_float_parameter.restype = (c_double)
    libsteps.api_get_powerflow_solver_float_parameter.argtypes = (c_char_p, c_uint)
    libsteps.api_get_powerflow_solver_boolean_parameter.restype = (c_bool)
    libsteps.api_get_powerflow_solver_boolean_parameter.argtypes = (c_char_p, c_uint)
    libsteps.api_set_powerflow_solver_integer_parameter.restype = None
    libsteps.api_set_powerflow_solver_integer_parameter.argtypes = (c_char_p, c_int, c_uint)
    libsteps.api_set_powerflow_solver_float_parameter.restype = None
    libsteps.api_set_powerflow_solver_float_parameter.argtypes = (c_char_p, c_double, c_uint)
    libsteps.api_set_powerflow_solver_boolean_parameter.restype = None
    libsteps.api_set_powerflow_solver_boolean_parameter.argtypes = (c_char_p, c_bool, c_uint)
    
    libsteps.api_show_powerflow_solver_configuration.restype = None
    libsteps.api_show_powerflow_solver_configuration.argtypes = (c_uint, )

    libsteps.api_solve_powerflow.restype = None
    libsteps.api_solve_powerflow.argtypes = (c_char_p, c_uint)
    libsteps.api_is_powerflow_converged.restype = (c_bool)
    libsteps.api_is_powerflow_converged.argtypes = (c_uint, )
    libsteps.api_is_nan_detected_in_powerflow_solution.restype = (c_bool)
    libsteps.api_is_nan_detected_in_powerflow_solution.argtypes = (c_uint, )
    libsteps.api_show_powerflow_result.restype = None
    libsteps.api_show_powerflow_result.argtypes = (c_uint, )
    libsteps.api_save_powerflow_result.restype = None
    libsteps.api_save_powerflow_result.argtypes = (c_char_p, c_uint)
    libsteps.api_save_extended_powerflow_result.restype = None
    libsteps.api_save_extended_powerflow_result.argtypes = (c_char_p, c_uint)
    libsteps.api_save_jacobian_matrix.restype = None
    libsteps.api_save_jacobian_matrix.argtypes = (c_char_p, c_uint)

    libsteps.api_build_network_Y_matrix.restype = None
    libsteps.api_build_network_Y_matrix.argtypes = (c_uint, )
    libsteps.api_build_decoupled_network_B_matrix.restype = None
    libsteps.api_build_decoupled_network_B_matrix.argtypes = (c_uint, )
    libsteps.api_build_dc_network_B_matrix.restype = None
    libsteps.api_build_dc_network_B_matrix.argtypes = (c_uint, )
    libsteps.api_build_network_Z_matrix.restype = None
    libsteps.api_build_network_Z_matrix.argtypes = (c_uint, )
    libsteps.api_save_network_Y_matrix.restype = None
    libsteps.api_save_network_Y_matrix.argtypes = (c_char_p, c_bool, c_uint)
    libsteps.api_save_decoupled_network_B_matrix.restype = None
    libsteps.api_save_decoupled_network_B_matrix.argtypes = (c_char_p, c_uint)
    libsteps.api_save_dc_network_B_matrix.restype = None
    libsteps.api_save_dc_network_B_matrix.argtypes = (c_char_p, c_uint)
    libsteps.api_save_network_Z_matrix.restype = None
    libsteps.api_save_network_Z_matrix.argtypes = (c_char_p, c_uint)

    libsteps.api_build_sequence_network_Y_matrix.restype = None
    libsteps.api_build_sequence_network_Y_matrix.argtypes = (c_uint, )
    libsteps.api_save_positive_sequence_network_Y_matrix.restype = None
    libsteps.api_save_positive_sequence_network_Y_matrix.argtypes = (c_char_p, c_uint)
    libsteps.api_save_negative_sequence_network_Y_matrix.restype = None
    libsteps.api_save_negative_sequence_network_Y_matrix.argtypes = (c_char_p, c_uint)
    libsteps.api_save_zero_sequence_network_Y_matrix.restype = None
    libsteps.api_save_zero_sequence_network_Y_matrix.argtypes = (c_char_p, c_uint)

    libsteps.api_build_dynamic_network_Y_matrix.restype = None
    libsteps.api_build_dynamic_network_Y_matrix.argtypes = (c_uint, )
    libsteps.api_save_dynamic_network_Y_matrix.restype = None
    libsteps.api_save_dynamic_network_Y_matrix.argtypes = (c_char_p, c_uint)

    libsteps.api_get_short_circuit_solver_integer_parameter.restype = (c_uint)
    libsteps.api_get_short_circuit_solver_integer_parameter.argtypes = (c_char_p, c_uint)
    libsteps.api_get_short_circuit_solver_float_parameter.restype = (c_double)
    libsteps.api_get_short_circuit_solver_float_parameter.argtypes = (c_char_p, c_uint)
    libsteps.api_get_short_circuit_solver_boolean_parameter.restype = (c_bool)
    libsteps.api_get_short_circuit_solver_boolean_parameter.argtypes = (c_char_p, c_uint)
    libsteps.api_set_short_circuit_solver_integer_parameter.restype = None
    libsteps.api_set_short_circuit_solver_integer_parameter.argtypes = (c_char_p, c_int, c_uint)
    libsteps.api_set_short_circuit_solver_float_parameter.restype = None
    libsteps.api_set_short_circuit_solver_float_parameter.argtypes = (c_char_p, c_double, c_uint)
    libsteps.api_set_short_circuit_solver_boolean_parameter.restype = None
    libsteps.api_set_short_circuit_solver_boolean_parameter.argtypes = (c_char_p, c_bool, c_uint)

    libsteps.api_solve_short_circuit.restype = None
    libsteps.api_solve_short_circuit.argtypes = (c_uint, )
    libsteps.api_get_short_circuit_result_float_data.restype = (c_double)
    libsteps.api_get_short_circuit_result_float_data.argtypes = (c_char_p, c_uint)
    libsteps.api_show_short_circuit_result.restype = None
    libsteps.api_show_short_circuit_result.argtypes = (c_uint, )
    libsteps.api_save_short_circuit_result_to_file.restype = None
    libsteps.api_save_short_circuit_result_to_file.argtypes = (c_char_p, c_uint)
    libsteps.api_save_extended_short_circuit_result_to_file.restype = None
    libsteps.api_save_extended_short_circuit_result_to_file.argtypes = (c_char_p, c_uint)

    libsteps.api_short_circuit_set_bus_fault.restype = None
    libsteps.api_short_circuit_set_bus_fault.argtypes = (c_uint, c_char_p, c_double, c_double, c_uint)
    libsteps.api_short_circuit_set_line_fault.restype = None
    libsteps.api_short_circuit_set_line_fault.argtypes = (c_uint, c_uint, c_char_p, c_char_p, c_double, c_double, c_double, c_uint)
    libsteps.api_short_circuit_clear_fault.restype = None	
    libsteps.api_short_circuit_clear_fault.argtype = (c_uint)

    libsteps.api_get_dynamic_simulator_integer_parameter.restype = (c_uint)
    libsteps.api_get_dynamic_simulator_integer_parameter.argtypes = (c_char_p, c_uint)
    libsteps.api_get_dynamic_simulator_float_parameter.restype = (c_double)
    libsteps.api_get_dynamic_simulator_float_parameter.argtypes = (c_char_p, c_uint)
    libsteps.api_get_dynamic_simulator_string_parameter.restype = (c_char_p)
    libsteps.api_get_dynamic_simulator_string_parameter.argtypes = (c_char_p, c_uint)
    libsteps.api_get_dynamic_simulator_boolean_parameter.restype = (c_bool)
    libsteps.api_get_dynamic_simulator_boolean_parameter.argtypes = (c_char_p, c_uint)
    libsteps.api_set_dynamic_simulator_integer_parameter.restype = None
    libsteps.api_set_dynamic_simulator_integer_parameter.argtypes = (c_char_p, c_int, c_uint)
    libsteps.api_set_dynamic_simulator_float_parameter.restype = None
    libsteps.api_set_dynamic_simulator_float_parameter.argtypes = (c_char_p, c_double, c_uint)
    libsteps.api_set_dynamic_simulator_string_parameter.restype = None
    libsteps.api_set_dynamic_simulator_string_parameter.argtypes = (c_char_p, c_char_p, c_uint)
    libsteps.api_set_dynamic_simulator_boolean_parameter.restype = None
    libsteps.api_set_dynamic_simulator_boolean_parameter.argtypes = (c_char_p, c_bool, c_uint)
    libsteps.api_get_dynamic_simulator_output_file.restype = (c_char_p)
    libsteps.api_get_dynamic_simulator_output_file.argtypes = (c_uint, )
    libsteps.api_set_dynamic_simulator_output_file.restype = None
    libsteps.api_set_dynamic_simulator_output_file.argtypes = (c_char_p, c_uint)

    libsteps.api_get_dynamic_simulation_time_step.restype = (c_double)
    libsteps.api_get_dynamic_simulation_time_step.argtypes = (c_uint, )
    libsteps.api_set_dynamic_simulation_time_step.restype = None
    libsteps.api_set_dynamic_simulation_time_step.argtypes = (c_double, c_uint)
    libsteps.api_get_dynamic_simulation_time.restype = (c_double)
    libsteps.api_get_dynamic_simulation_time.argtypes = (c_uint, )
    
    libsteps.api_show_dynamic_simulation_configuration.restype = None
    libsteps.api_show_dynamic_simulation_configuration.argtypes = (c_uint, )

    libsteps.api_clear_meters.restype = None
    libsteps.api_clear_meters.argtypes = (c_uint, )
    libsteps.api_prepare_meters.restype = None
    libsteps.api_prepare_meters.argtypes = (c_uint, )
    libsteps.api_prepare_bus_related_meters.restype = None
    libsteps.api_prepare_bus_related_meters.argtypes = (c_uint, )
    libsteps.api_prepare_generator_related_meters.restype = None
    libsteps.api_prepare_generator_related_meters.argtypes = (c_uint, )
    libsteps.api_prepare_wt_generator_related_meters.restype = None
    libsteps.api_prepare_wt_generator_related_meters.argtypes = (c_uint, )
    libsteps.api_prepare_pv_unit_related_meters.restype = None
    libsteps.api_prepare_pv_unit_related_meters.argtypes = (c_uint, )
    libsteps.api_prepare_energy_storage_related_meters.restype = None
    libsteps.api_prepare_energy_storage_related_meters.argtypes = (c_uint, )
    libsteps.api_prepare_load_related_meters.restype = None
    libsteps.api_prepare_load_related_meters.argtypes = (c_uint, )
    libsteps.api_prepare_line_related_meters.restype = None
    libsteps.api_prepare_line_related_meters.argtypes = (c_uint, )
    libsteps.api_prepare_transformer_related_meters.restype = None
    libsteps.api_prepare_transformer_related_meters.argtypes = (c_uint, )
    libsteps.api_prepare_hvdc_related_meters.restype = None
    libsteps.api_prepare_hvdc_related_meters.argtypes = (c_uint, )
    libsteps.api_prepare_vsc_hvdc_related_meters.restype = None
    libsteps.api_prepare_vsc_hvdc_related_meters.argtypes = (c_uint, )   
    libsteps.api_prepare_equivalent_device_related_meters.restype = None
    libsteps.api_prepare_equivalent_device_related_meters.argtypes = (c_uint, )


    libsteps.api_prepare_bus_related_meter.restype = None
    libsteps.api_prepare_bus_related_meter.argtypes = (c_uint, c_char_p, c_uint)
    libsteps.api_prepare_generator_related_meter.restype = None
    libsteps.api_prepare_generator_related_meter.argtypes = (c_uint, c_char_p, c_char_p, c_char_p, c_uint)
    libsteps.api_prepare_wt_generator_related_meter.restype = None
    libsteps.api_prepare_wt_generator_related_meter.argtypes = (c_uint, c_char_p, c_char_p, c_char_p, c_uint)
    libsteps.api_prepare_pv_unit_related_meter.restype = None
    libsteps.api_prepare_pv_unit_related_meter.argtypes = (c_uint, c_char_p, c_char_p, c_char_p, c_uint)
    libsteps.api_prepare_energy_storage_related_meter.restype = None
    libsteps.api_prepare_energy_storage_related_meter.argtypes = (c_uint, c_char_p, c_char_p, c_char_p, c_uint)
    libsteps.api_prepare_load_related_meter.restype = None
    libsteps.api_prepare_load_related_meter.argtypes = (c_uint, c_char_p, c_char_p, c_char_p, c_uint)
    libsteps.api_prepare_line_related_meter.restype = None
    libsteps.api_prepare_line_related_meter.argtypes = (c_uint, c_uint, c_char_p, c_char_p, c_char_p, c_char_p, c_uint)
    libsteps.api_prepare_transformer_related_meter.restype = None
    libsteps.api_prepare_transformer_related_meter.argtypes = (c_uint, c_uint, c_uint, c_char_p, c_char_p, c_char_p, c_char_p, c_uint)
    libsteps.api_prepare_hvdc_related_meter.restype = None
    libsteps.api_prepare_hvdc_related_meter.argtypes = (c_uint, c_uint, c_char_p, c_char_p, c_char_p, c_char_p, c_uint)
    libsteps.api_prepare_vsc_hvdc_related_meter.restype = None
    libsteps.api_prepare_vsc_hvdc_related_meter.argtypes = (c_char_p, c_char_p, c_uint,c_char_p, c_uint)    
    libsteps.api_prepare_equivalent_device_related_meter.restype = None
    libsteps.api_prepare_equivalent_device_related_meter.argtypes = (c_uint, c_char_p, c_char_p, c_char_p, c_uint)

    libsteps.api_start_dynamic_simulation.restype = None
    libsteps.api_start_dynamic_simulation.argtypes = (c_uint, )
    libsteps.api_stop_dynamic_simulation.restype = None
    libsteps.api_stop_dynamic_simulation.argtypes = (c_uint, )
    libsteps.api_run_simulation_to_time.restype = None
    libsteps.api_run_simulation_to_time.argtypes = (c_double, c_uint)
    libsteps.api_run_a_step.restype = None
    libsteps.api_run_a_step.argtypes = (c_uint, )
    libsteps.api_get_system_angular_stable_flag.restype = c_bool
    libsteps.api_get_system_angular_stable_flag.argtypes = (c_uint, )

    libsteps.api_get_user_meter_value.restype = c_double
    libsteps.api_get_user_meter_value.argtypes = (c_uint, c_uint)
    libsteps.api_get_basic_meter_value.restype = c_double
    libsteps.api_get_basic_meter_value.argtypes = (c_char_p, c_uint)

    libsteps.api_change_dynamic_simulation_time_step.restype = None
    libsteps.api_change_dynamic_simulation_time_step.argtypes = (c_double, c_uint)

    libsteps.api_set_bus_fault.restype = None
    libsteps.api_set_bus_fault.argtypes = (c_uint, c_char_p, c_double, c_double, c_uint)
    libsteps.api_clear_bus_fault.restype = None
    libsteps.api_clear_bus_fault.argtypes = (c_uint, c_char_p, c_uint)
    libsteps.api_trip_bus.restype = None
    libsteps.api_trip_bus.argtypes = (c_uint, c_uint)

    libsteps.api_set_line_fault.restype = None
    libsteps.api_set_line_fault.argtypes = (c_uint, c_uint, c_char_p, c_char_p, c_double, c_double, c_double, c_uint)
    libsteps.api_clear_line_fault.restype = None
    libsteps.api_clear_line_fault.argtypes = (c_uint, c_uint, c_char_p, c_char_p, c_double, c_uint)
    libsteps.api_trip_line.restype = None
    libsteps.api_trip_line.argtypes = (c_uint, c_uint, c_char_p, c_uint)
    libsteps.api_trip_line_breaker.restype = None
    libsteps.api_trip_line_breaker.argtypes = (c_uint, c_uint, c_char_p, c_uint)
    libsteps.api_close_line.restype = None
    libsteps.api_close_line.argtypes = (c_uint, c_uint, c_char_p, c_uint)
    libsteps.api_close_line_breaker.restype = None
    libsteps.api_close_line_breaker.argtypes = (c_uint, c_uint, c_char_p, c_uint)

    libsteps.api_trip_transformer.restype = None
    libsteps.api_trip_transformer.argtypes = (c_uint, c_uint, c_uint, c_char_p, c_uint)
    libsteps.api_trip_transformer_breaker.restype = None
    libsteps.api_trip_transformer_breaker.argtypes = (c_uint, c_uint, c_uint, c_char_p, c_uint)
    libsteps.api_close_transformer.restype = None
    libsteps.api_close_transformer.argtypes = (c_uint, c_uint, c_uint, c_char_p, c_uint)
    libsteps.api_close_transformer_breaker.restype = None
    libsteps.api_close_transformer_breaker.argtypes = (c_uint, c_uint, c_uint, c_char_p, c_uint)

    libsteps.api_trip_generator.restype = None
    libsteps.api_trip_generator.argtypes = (c_uint, c_char_p, c_uint)
    libsteps.api_shed_generator.restype = None
    libsteps.api_shed_generator.argtypes = (c_uint, c_char_p, c_double, c_uint)

    libsteps.api_trip_wt_generator.restype = None
    libsteps.api_trip_wt_generator.argtypes = (c_uint, c_char_p, c_uint)
    libsteps.api_shed_wt_generator.restype = None
    libsteps.api_shed_wt_generator.argtypes = (c_uint, c_char_p, c_double, c_uint)

    libsteps.api_trip_load.restype = None
    libsteps.api_trip_load.argtypes = (c_uint, c_char_p, c_uint)
    libsteps.api_close_load.restype = None
    libsteps.api_close_load.argtypes = (c_uint, c_char_p, c_uint)
    libsteps.api_scale_load.restype = None
    libsteps.api_scale_load.argtypes = (c_uint, c_char_p, c_double, c_uint)
    libsteps.api_scale_all_loads.restype = None
    libsteps.api_scale_all_loads.argtypes = (c_double, c_uint)

    libsteps.api_trip_fixed_shunt.restype = None
    libsteps.api_trip_fixed_shunt.argtypes = (c_uint, c_char_p, c_uint)
    libsteps.api_close_fixed_shunt.restype = None
    libsteps.api_close_fixed_shunt.argtypes = (c_uint, c_char_p, c_uint)

    libsteps.api_manually_bypass_hvdc.restype = None
    libsteps.api_manually_bypass_hvdc.argtypes = (c_uint, c_uint, c_char_p, c_uint)
    libsteps.api_manually_unbypass_hvdc.restype = None
    libsteps.api_manually_unbypass_hvdc.argtypes = (c_uint, c_uint, c_char_p, c_uint)
    libsteps.api_manually_block_hvdc.restype = None
    libsteps.api_manually_block_hvdc.argtypes = (c_uint, c_uint, c_char_p, c_uint)
    libsteps.api_manually_unblock_hvdc.restype = None
    libsteps.api_manually_unblock_hvdc.argtypes = (c_uint, c_uint, c_char_p, c_uint)

    libsteps.api_get_generator_voltage_reference_in_pu.restype = (c_double)
    libsteps.api_get_generator_voltage_reference_in_pu.argtypes = (c_uint, c_char_p, c_uint)
    libsteps.api_set_generator_voltage_reference_in_pu.restype = None
    libsteps.api_set_generator_voltage_reference_in_pu.argtypes = (c_uint, c_char_p, c_double, c_uint)

    libsteps.api_get_generator_mechanical_power_reference_in_pu_based_on_mbase.restype = (c_double)
    libsteps.api_get_generator_mechanical_power_reference_in_pu_based_on_mbase.argtypes = (c_uint, c_char_p, c_uint)
    libsteps.api_set_generator_mechanical_power_reference_in_pu_based_on_mbase.restype = None
    libsteps.api_set_generator_mechanical_power_reference_in_pu_based_on_mbase.argtypes = (c_uint, c_char_p, c_double, c_uint)

    libsteps.api_get_generator_mechanical_power_reference_in_MW.restype = (c_double)
    libsteps.api_get_generator_mechanical_power_reference_in_MW.argtypes = (c_uint, c_char_p, c_uint)
    libsteps.api_set_generator_mechanical_power_reference_in_MW.restype = None
    libsteps.api_set_generator_mechanical_power_reference_in_MW.argtypes = (c_uint, c_char_p, c_double, c_uint)

    libsteps.api_get_generator_excitation_voltage_in_pu.restype = (c_double)
    libsteps.api_get_generator_excitation_voltage_in_pu.argtypes = (c_uint, c_char_p, c_uint)
    libsteps.api_set_generator_excitation_voltage_in_pu.restype = None
    libsteps.api_set_generator_excitation_voltage_in_pu.argtypes = (c_uint, c_char_p, c_double, c_uint)

    libsteps.api_get_generator_mechanical_power_in_pu_based_on_mbase.restype = (c_double)
    libsteps.api_get_generator_mechanical_power_in_pu_based_on_mbase.argtypes = (c_uint, c_char_p, c_uint)
    libsteps.api_set_generator_mechanical_power_in_pu_based_on_mbase.restype = None
    libsteps.api_set_generator_mechanical_power_in_pu_based_on_mbase.argtypes = (c_uint, c_char_p, c_double, c_uint)

    libsteps.api_get_generator_mechanical_power_in_MW.restype = (c_double)
    libsteps.api_get_generator_mechanical_power_in_MW.argtypes = (c_uint, c_char_p, c_uint)
    libsteps.api_set_generator_mechanical_power_in_MW.restype = None
    libsteps.api_set_generator_mechanical_power_in_MW.argtypes = (c_uint, c_char_p, c_double, c_uint)
    
    libsteps.api_get_hvdc_power_order_in_MW.restype = (c_double)
    libsteps.api_get_hvdc_power_order_in_MW.argtypes = (c_uint, c_uint, c_char_p, c_uint)
    libsteps.api_set_hvdc_power_order_in_MW.restype = None
    libsteps.api_set_hvdc_power_order_in_MW.argtypes = (c_uint, c_uint, c_char_p, c_double, c_uint)

    libsteps.api_set_vsc_hvdc_dc_line_fault.restype = None
    libsteps.api_set_vsc_hvdc_dc_line_fault.argtypes = (c_char_p, c_uint, c_uint, c_char_p, c_double,c_double, c_uint)
    libsteps.api_clear_vsc_hvdc_dc_line_fault.restype = None
    libsteps.api_clear_vsc_hvdc_dc_line_fault.argtypes = (c_char_p, c_uint, c_uint, c_char_p, c_uint)
    libsteps.api_trip_vsc_hvdc_dc_line.restype = None
    libsteps.api_trip_vsc_hvdc_dc_line.argtypes = (c_char_p, c_uint, c_uint, c_char_p, c_uint)
    libsteps.api_close_vsc_hvdc_dc_line.restype = None
    libsteps.api_close_vsc_hvdc_dc_line.argtypes = (c_char_p, c_uint, c_uint, c_char_p, c_uint)

    libsteps.api_search_cct.restype = (c_double)
    libsteps.api_search_cct.argtypes = (c_char_p, c_char_p, c_uint, c_uint, c_char_p, c_uint, c_uint, c_uint)

    return libsteps

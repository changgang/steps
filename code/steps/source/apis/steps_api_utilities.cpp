#include "header/apis/steps_api.h"
#include "header/apis/steps_api_common.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

unsigned int api_get_api_major_version()
{
    return 1;
}

unsigned int api_get_api_minor_version()
{
    return 5;
}

unsigned int api_get_api_patch_version()
{
    return 2;
}

unsigned int api_get_const_INDEX_NOT_EXIST()
{
    return INDEX_NOT_EXIST;
}

void api_set_toolkit_log_file(char* log_file, bool log_file_append_mode, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    return toolkit.open_log_file(log_file, log_file_append_mode);
}

unsigned int api_generate_new_toolkit(char* log_fie)
{
    string log_file_name = "";
    if(log_fie!=NULL)
        log_file_name = log_fie;
    unsigned int index = generate_new_toolkit(log_file_name);
    return index;
}

void api_delete_toolkit(unsigned int toolkit_index)
{
    delete_toolkit(toolkit_index);
}

void api_initialize_toolkit(unsigned int toolkit_index)
{
    api_clear_toolkit(toolkit_index);
}

void api_clear_toolkit(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    toolkit.clear();
}

void api_set_toolkit_parallel_thread_number(unsigned int n, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    toolkit.set_thread_number(n);
}

unsigned int api_get_toolkit_parallel_thread_number(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    return toolkit.get_thread_number();
}


void api_set_toolkit_dynamic_model_database_capacity(unsigned int n, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    toolkit.set_dynamic_model_database_size_in_bytes(n);
}

unsigned int api_get_toolkit_dynamic_model_database_capacity(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    return toolkit.get_dynamic_model_database_size_in_bytes();
}

double api_get_toolkit_float_data(char* parameter_name, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    string PARAMETER_NAME = string2upper(parameter_name);
    if(PARAMETER_NAME=="SBASE")
        return psdb.get_system_base_power_in_MVA();
    if(PARAMETER_NAME=="ZERO IMPEDANCE THRESHOLD IN PU")
        return psdb.get_zero_impedance_threshold_in_pu();

    show_parameter_not_supported_with_api(PARAMETER_NAME, __FUNCTION__);
    return 0.0;
}

void api_set_toolkit_float_data(char* parameter_name, double value, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    string PARAMETER_NAME = string2upper(parameter_name);
    if(PARAMETER_NAME=="SBASE")
        return psdb.set_system_base_power_in_MVA(value);
    if(PARAMETER_NAME=="ZERO IMPEDANCE THRESHOLD IN PU")
        return psdb.set_zero_impedance_threshold_in_pu(value);

    show_parameter_not_supported_with_api(PARAMETER_NAME, __FUNCTION__);
}


const char* api_get_toolkit_string_data(char* parameter_name, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s","");
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    string PARAMETER_NAME = string2upper(parameter_name);
    if(PARAMETER_NAME=="TOOLKIT NAME")
    {
        snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (toolkit.get_toolkit_name()).c_str());
        return toolkit.steps_char_buffer;
    }
    if(PARAMETER_NAME=="CASE INFORMATION")
    {
        snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (psdb.get_case_information()).c_str());
        return toolkit.steps_char_buffer;
    }
    if(PARAMETER_NAME=="CASE ADDITIONAL INFORMATION")
    {
        snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (psdb.get_case_additional_information()).c_str());
        return toolkit.steps_char_buffer;
    }

    show_parameter_not_supported_with_api(PARAMETER_NAME, __FUNCTION__);
    return toolkit.steps_char_buffer;
}

void api_set_toolkit_string_data(char* parameter_name, char* value, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    string PARAMETER_NAME = string2upper(parameter_name);
    if(PARAMETER_NAME=="TOOLKIT NAME")
        return toolkit.set_toolkit_name(value);
    if(PARAMETER_NAME=="CASE INFORMATION")
        return psdb.set_case_information(value);
    if(PARAMETER_NAME=="CASE ADDITIONAL INFORMATION")
        return psdb.set_case_additional_information(value);

    show_parameter_not_supported_with_api(PARAMETER_NAME, __FUNCTION__);
    return;
}

bool api_get_toolkit_bool_data(char* parameter_name, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);

    string PARAMETER_NAME = string2upper(parameter_name);
    if(PARAMETER_NAME=="DETAILED LOG LOGIC")
    {
        return toolkit.is_detailed_log_enabled();
    }
    if(PARAMETER_NAME=="OPTIMIZE NETWORK LOGIC")
    {
        return toolkit.is_optimize_network_enabled();
    }
    if(PARAMETER_NAME=="USE STEPS FAST MATH LOGIC")
    {
        return toolkit.get_use_steps_fast_math_logic();
    }
    if(PARAMETER_NAME=="CORRECT THREE WINDING TRANSFORMER IMPEDANCE LOGIC")
    {
        return toolkit.get_correct_three_winding_transformer_impedance_logic();
    }
    if(PARAMETER_NAME=="BLOCK AUTOMATIC LARGE STEP LOGIC")
    {
        return toolkit.get_dynamic_blocks_automatic_large_step_logic();
    }

    show_parameter_not_supported_with_api(PARAMETER_NAME, __FUNCTION__);
    return false;
}

void api_set_toolkit_bool_data(char* parameter_name, bool value, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);

    string PARAMETER_NAME = string2upper(parameter_name);
    if(PARAMETER_NAME=="DETAILED LOG LOGIC")
    {
        if(value==true)
            return toolkit.enable_detailed_log();
        else
            return toolkit.disable_detailed_log();
    }
    if(PARAMETER_NAME=="OPTIMIZE NETWORK LOGIC")
    {
        if(value==true)
            return toolkit.enable_optimize_network();
        else
            return toolkit.disable_optimize_network();
    }
    if(PARAMETER_NAME=="USE STEPS FAST MATH LOGIC")
    {
        if(value==true)
            return toolkit.enable_use_steps_fast_math_logic();
        else
            return toolkit.disable_use_steps_fast_math_logic();
    }
    if(PARAMETER_NAME=="CORRECT THREE WINDING TRANSFORMER IMPEDANCE LOGIC")
    {
        if(value==true)
            return toolkit.enable_correct_three_winding_transformer_impedance_logic();
        else
            return toolkit.disable_correct_three_winding_transformer_impedance_logic();
    }
    if(PARAMETER_NAME=="BLOCK AUTOMATIC LARGE STEP LOGIC")
    {
        if(value==true)
            return toolkit.enable_dynamic_blocks_automatic_large_step_logic();
        else
            return toolkit.disable_dynamic_blocks_automatic_large_step_logic();
    }

    show_parameter_not_supported_with_api(PARAMETER_NAME, __FUNCTION__);
}

unsigned int api_get_allowed_maximum_bus_number(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    return psdb.get_allowed_max_bus_number();
}

void api_set_allowed_maximum_bus_number(unsigned int max_bus, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    psdb.set_allowed_max_bus_number(max_bus);
}


unsigned int api_get_device_capacity(const char* device_type, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    string DEVICE_TYPE = string2upper(device_type);

    if(DEVICE_TYPE=="BUS")
        return psdb.get_bus_capacity();
    if(DEVICE_TYPE=="GENERATOR")
        return psdb.get_generator_capacity();
    if(DEVICE_TYPE=="WT GENERATOR")
        return psdb.get_wt_generator_capacity();
    if(DEVICE_TYPE=="PV UNIT")
        return psdb.get_pv_unit_capacity();
    if(DEVICE_TYPE=="LOAD")
        return psdb.get_load_capacity();
    if(DEVICE_TYPE=="FIXED SHUNT")
        return psdb.get_fixed_shunt_capacity();
    if(DEVICE_TYPE=="LINE")
        return psdb.get_line_capacity();
    if(DEVICE_TYPE=="TRANSFORMER")
        return psdb.get_transformer_capacity();
    if(DEVICE_TYPE=="HVDC")
        return psdb.get_hvdc_capacity();
    if(DEVICE_TYPE=="VSC HVDC")
        return psdb.get_vsc_hvdc_capacity();
    if(DEVICE_TYPE=="EQUIVALENT DEVICE")
        return psdb.get_equivalent_device_capacity();
    if(DEVICE_TYPE=="ENERGY STORAGE")
        return psdb.get_energy_storage_capacity();

    show_parameter_not_supported_with_api(DEVICE_TYPE, __FUNCTION__);
    return 0;
}


unsigned int api_get_area_capacity(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    return psdb.get_area_capacity();
}

unsigned int api_get_zone_capacity(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    return psdb.get_zone_capacity();
}

unsigned int api_get_owner_capacity(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    return psdb.get_owner_capacity();
}


void api_set_device_capacity(const char* device_type, unsigned int cap, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    string DEVICE_TYPE = string2upper(device_type);

    if(DEVICE_TYPE=="BUS")
        return psdb.set_bus_capacity(cap);
    if(DEVICE_TYPE=="GENERATOR")
        return psdb.set_generator_capacity(cap);
    if(DEVICE_TYPE=="WT GENERATOR")
        return psdb.set_wt_generator_capacity(cap);
    if(DEVICE_TYPE=="PV UNIT")
        return psdb.set_pv_unit_capacity(cap);
    if(DEVICE_TYPE=="LOAD")
        return psdb.set_load_capacity(cap);
    if(DEVICE_TYPE=="FIXED SHUNT")
        return psdb.set_fixed_shunt_capacity(cap);
    if(DEVICE_TYPE=="LINE")
        return psdb.set_line_capacity(cap);
    if(DEVICE_TYPE=="TRANSFORMER")
        return psdb.set_transformer_capacity(cap);
    if(DEVICE_TYPE=="HVDC")
        return psdb.set_hvdc_capacity(cap);
    if(DEVICE_TYPE=="VSC HVDC")
        return psdb.set_vsc_hvdc_capacity(cap);
    if(DEVICE_TYPE=="EQUIVALENT DEVICE")
        return psdb.set_equivalent_device_capacity(cap);
    if(DEVICE_TYPE=="ENERGY STORAGE")
        return psdb.set_energy_storage_capacity(cap);

    show_parameter_not_supported_with_api(DEVICE_TYPE, __FUNCTION__);
}

void api_set_area_capacity(unsigned int cap, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    return psdb.set_area_capacity(cap);
}

void api_set_zone_capacity(unsigned int cap, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    return psdb.set_zone_capacity(cap);
}

void api_set_owner_capacity(unsigned int cap, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    return psdb.set_owner_capacity(cap);
}



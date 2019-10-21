#include "header/apis/steps_api.h"
#include "header/apis/steps_api_common.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

void api_set_default_toolkit_log_file(char* log_fie)
{
    string log_file_name = "";
    if(log_fie!=NULL)
        log_file_name = log_fie;
    default_toolkit.open_log_file(log_file_name);
}

size_t api_generate_new_toolkit(char* log_fie)
{
    string log_file_name = "";
    if(log_fie!=NULL)
        log_file_name = log_fie;
    size_t index = generate_new_toolkit(log_file_name);
    return index;
}

void api_delete_toolkit(size_t toolkit_index)
{
    delete_toolkit(toolkit_index);
}


void api_initialize_toolkit(size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    toolkit.clear();
}

void api_clear_toolkit(size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    toolkit.get_power_system_database().clear();
    toolkit.get_dynamic_simulator().clear();
}

void api_set_toolkit_parallel_thread_number(size_t n, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    toolkit.set_thread_number(n);
}

size_t api_get_toolkit_parallel_thread_number(size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    return toolkit.get_thread_number();
}

double api_get_toolkit_float_data(char* parameter_name, size_t toolkit_index)
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

void api_set_toolkit_float_data(char* parameter_name, double value, size_t toolkit_index)
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


const char* api_get_toolkit_string_data(char* parameter_name, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    snprintf(toolkit.steps_char_buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s","");
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    string PARAMETER_NAME = string2upper(parameter_name);
    if(PARAMETER_NAME=="TOOLKIT NAME")
    {
        snprintf(toolkit.steps_char_buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (toolkit.get_toolkit_name()).c_str());
        return toolkit.steps_char_buffer;
    }
    if(PARAMETER_NAME=="CASE INFORMATION")
    {
        snprintf(toolkit.steps_char_buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (psdb.get_case_information()).c_str());
        return toolkit.steps_char_buffer;
    }
    if(PARAMETER_NAME=="CASE ADDITIONAL INFORMATION")
    {
        snprintf(toolkit.steps_char_buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (psdb.get_case_additional_information()).c_str());
        return toolkit.steps_char_buffer;
    }

    show_parameter_not_supported_with_api(PARAMETER_NAME, __FUNCTION__);
    return toolkit.steps_char_buffer;
}

void api_set_toolkit_string_data(char* parameter_name, char* value, size_t toolkit_index)
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

bool api_get_toolkit_bool_data(char* parameter_name, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    snprintf(toolkit.steps_char_buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s","");
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    string PARAMETER_NAME = string2upper(parameter_name);
    if(PARAMETER_NAME=="DETAILED LOG LOGIC")
    {
        return toolkit.is_detailed_log_enabled();
    }
    if(PARAMETER_NAME=="OPTIMIZE NETWORK LOGIC")
    {
        return toolkit.is_optimize_network_enabled();
    }

    show_parameter_not_supported_with_api(PARAMETER_NAME, __FUNCTION__);
    return toolkit.steps_char_buffer;
}

void api_set_toolkit_bool_data(char* parameter_name, bool value, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

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

    show_parameter_not_supported_with_api(PARAMETER_NAME, __FUNCTION__);
    return;
}

size_t api_get_allowed_maximum_bus_number(size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    return psdb.get_allowed_max_bus_number();
}

void api_set_allowed_maximum_bus_number(size_t max_bus, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    psdb.set_allowed_max_bus_number(max_bus);
}


size_t api_get_device_capacity(const char* device_type, size_t toolkit_index)
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
    if(DEVICE_TYPE=="EQUIVALENT DEVICE")
        return psdb.get_equivalent_device_capacity();
    if(DEVICE_TYPE=="ENERGY STORAGE")
        return psdb.get_energy_storage_capacity();

    show_parameter_not_supported_with_api(DEVICE_TYPE, __FUNCTION__);
    return 0;
}


size_t api_get_area_capacity(size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    return psdb.get_area_capacity();
}

size_t api_get_zone_capacity(size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    return psdb.get_zone_capacity();
}

size_t api_get_owner_capacity(size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    return psdb.get_owner_capacity();
}


void api_set_device_capacity(const char* device_type, size_t cap, size_t toolkit_index)
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
    if(DEVICE_TYPE=="EQUIVALENT DEVICE")
        return psdb.set_equivalent_device_capacity(cap);
    if(DEVICE_TYPE=="ENERGY STORAGE")
        return psdb.set_energy_storage_capacity(cap);

    show_parameter_not_supported_with_api(DEVICE_TYPE, __FUNCTION__);
}

void api_set_area_capacity(size_t cap, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    return psdb.set_area_capacity(cap);
}

void api_set_zone_capacity(size_t cap, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    return psdb.set_zone_capacity(cap);
}

void api_set_owner_capacity(size_t cap, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    return psdb.set_owner_capacity(cap);
}



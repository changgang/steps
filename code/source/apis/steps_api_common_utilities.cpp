#include "header/apis/steps_api.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

void show_side_not_supported_for_device_with_api(string side, DEVICE_ID did, string api_func)
{
    char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Side '%s' is not supported for %s with api %s.\n"
             "Nothing will be chaged.\n"
             "If return value is expected, 0/0.0/False/EMPTY STRING will be returned",
             side.c_str(), (did.get_device_name()).c_str(), api_func.c_str());
    show_information_with_leading_time_stamp(buffer);
}

void show_parameter_not_supported_for_device_with_api(string par_name, DEVICE_ID did, string api_func)
{
    char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Parameter '%s' is not supported for %s with api %s.\n"
             "Nothing will be chaged.\n"
             "If return value is expected, 0/0.0/False/EMPTY STRING will be returned",
             par_name.c_str(), (did.get_device_name()).c_str(), api_func.c_str());
    show_information_with_leading_time_stamp(buffer);
}

void show_parameter_not_supported_with_api(string par_name, string api_func)
{
    char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Parameter '%s' is not supported with api %s.\n"
             "Nothing will be chaged.\n"
             "If return value is expected, 0/0.0/False/EMPTY STRING will be returned",
             par_name.c_str(), api_func.c_str());
    show_information_with_leading_time_stamp(buffer);
}

void show_device_not_exist_with_api(DEVICE_ID did, string api_func)
{
    char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Device %s does not exist in database with api %s.\n"
             "Nothing will be chaged.\n"
             "If return value is expected, 0/0.0/False/EMPTY STRING will be returned",
             (did.get_device_name()).c_str(), api_func.c_str());
    show_information_with_leading_time_stamp(buffer);
}


void show_parameter_not_supported_for_area_zone_owner_with_api(string par_name, size_t no, string api_func)
{
    char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Parameter '%s' is not supported for Area/Zone/Owner %lu with api %s.\n"
             "Nothing will be chaged.\n"
             "If return value is expected, 0/0.0/False/EMPTY STRING will be returned",
             par_name.c_str(), no, api_func.c_str());
    show_information_with_leading_time_stamp(buffer);
}

void show_area_zone_owner_not_exist_with_api(size_t no, string api_func)
{
    char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Area/Zone/Owner %lu does not exist in database with api %s.\n"
             "Nothing will be chaged.\n"
             "If return value is expected, 0/0.0/False/EMPTY STRING will be returned",
             no, api_func.c_str());
    show_information_with_leading_time_stamp(buffer);
}

size_t get_owner_of_device(DEVICE* device, string parameter_name)
{
    string PARAMETER_NAME = string2upper(parameter_name);
    if(PARAMETER_NAME=="OWNER1")
    {
        return device->get_owner_of_index(0);
    }

    if(PARAMETER_NAME=="OWNER2")
    {
        return device->get_owner_of_index(1);
    }

    if(PARAMETER_NAME=="OWNER3")
    {
        return device->get_owner_of_index(2);
    }

    if(PARAMETER_NAME=="OWNER4")
    {
        return device->get_owner_of_index(3);
    }
    show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, device->get_device_id(), __FUNCTION__);
    return 0;
}

void set_owner_of_device(DEVICE* device, string parameter_name, int value)
{
    string PARAMETER_NAME = string2upper(parameter_name);
    char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
    if(PARAMETER_NAME=="OWNER1")
    {
        size_t n = device->get_owner_count();
        if(n>=1)
        {
            snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Owner 1 already set and will not be set again for %s.",
                     (device->get_device_name()).c_str());
            show_information_with_leading_time_stamp(buffer);
            return;
        }
        OWNERSHIP os;
        os.append_owner_and_its_fraction(value, 0.0);
        device->set_ownership(os);
        return;
    }

    if(PARAMETER_NAME=="OWNER2")
    {
        size_t n = device->get_owner_count();
        if(n>=2)
        {
            snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Owner 2 already set and will not be set again for %s.",
                     (device->get_device_name()).c_str());
            show_information_with_leading_time_stamp(buffer);
            return;
        }
        OWNERSHIP os = device->get_ownership();
        os.append_owner_and_its_fraction(value, 0.0);
        device->set_ownership(os);
        return;
    }

    if(PARAMETER_NAME=="OWNER3")
    {
        size_t n = device->get_owner_count();
        if(n>=3)
        {
            snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Owner 3 already set and will not be set again for %s.",
                     (device->get_device_name()).c_str());
            show_information_with_leading_time_stamp(buffer);
            return;
        }
        OWNERSHIP os = device->get_ownership();
        os.append_owner_and_its_fraction(value, 0.0);
        device->set_ownership(os);
        return;
    }

    if(PARAMETER_NAME=="OWNER4")
    {
        size_t n = device->get_owner_count();
        if(n>=4)
        {
            snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Owner 4 already set and will not be set again for %s.",
                     (device->get_device_name()).c_str());
            show_information_with_leading_time_stamp(buffer);
            return;
        }
        OWNERSHIP os = device->get_ownership();
        os.append_owner_and_its_fraction(value, 0.0);
        device->set_ownership(os);
        return;
    }
    show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, device->get_device_id(), __FUNCTION__);
    return;
}

double get_owner_fraction_of_device(DEVICE* device, string parameter_name)
{
    string PARAMETER_NAME = string2upper(parameter_name);
    if(PARAMETER_NAME=="FRAC1")
    {
        return device->get_fraction_of_owner_of_index(0);
    }

    if(PARAMETER_NAME=="FRAC2")
    {
        return device->get_fraction_of_owner_of_index(1);
    }

    if(PARAMETER_NAME=="FRAC3")
    {
        return device->get_fraction_of_owner_of_index(2);
    }

    if(PARAMETER_NAME=="FRAC4")
    {
        return device->get_fraction_of_owner_of_index(3);
    }
    show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, device->get_device_id(), __FUNCTION__);
    return 0;
}

void set_owner_fraction_of_device(DEVICE* device, string parameter_name, double value)
{
    string PARAMETER_NAME = string2upper(parameter_name);
    char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
    if(PARAMETER_NAME=="FRAC1")
    {
        size_t n = device->get_owner_count();
        if(n<1)
        {
            snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Owner 1 does not exist and owner fraction will not be set for %s.",
                     (device->get_device_name()).c_str());
            show_information_with_leading_time_stamp(buffer);
            return;
        }
        OWNERSHIP os = device->get_ownership();
        size_t owner = device->get_owner_of_index(0);
        double frac = device->get_fraction_of_owner_of_index(0);
        os.append_owner_and_its_fraction(owner, -frac+value);
        device->set_ownership(os);
        return;
    }

    if(PARAMETER_NAME=="FRAC2")
    {
        size_t n = device->get_owner_count();
        if(n<2)
        {
            snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Owner 2 does not exist and owner fraction will not be set for %s.",
                     (device->get_device_name()).c_str());
            show_information_with_leading_time_stamp(buffer);
            return;
        }
        OWNERSHIP os = device->get_ownership();
        size_t owner = device->get_owner_of_index(1);
        double frac = device->get_fraction_of_owner_of_index(1);
        os.append_owner_and_its_fraction(owner, -frac+value);
        device->set_ownership(os);
        return;
    }

    if(PARAMETER_NAME=="FRAC3")
    {
        size_t n = device->get_owner_count();
        if(n<3)
        {
            snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Owner 3 does not exist and owner fraction will not be set for %s.",
                     (device->get_device_name()).c_str());
            show_information_with_leading_time_stamp(buffer);
            return;
        }
        OWNERSHIP os = device->get_ownership();
        size_t owner = device->get_owner_of_index(2);
        double frac = device->get_fraction_of_owner_of_index(2);
        os.append_owner_and_its_fraction(owner, -frac+value);
        device->set_ownership(os);
        return;
    }

    if(PARAMETER_NAME=="FRAC4")
    {
        size_t n = device->get_owner_count();
        if(n<4)
        {
            snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Owner 4 does not exist and owner fraction will not be set for %s.",
                     (device->get_device_name()).c_str());
            show_information_with_leading_time_stamp(buffer);
            return;
        }
        OWNERSHIP os = device->get_ownership();
        size_t owner = device->get_owner_of_index(3);
        double frac = device->get_fraction_of_owner_of_index(3);
        os.append_owner_and_its_fraction(owner, -frac+value);
        device->set_ownership(os);
        return;
    }
    show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, device->get_device_id(), __FUNCTION__);
}


void api_initialize_package()
{
    initialize_simulator();
}

void api_initialize_powerflow_solver()
{
    POWERFLOW_SOLVER& solver = get_default_powerflow_solver();
    solver.initialize_powerflow_solver();
}

size_t api_get_allowed_maximum_bus_number()
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    return psdb.get_allowed_max_bus_number();
}

void api_set_allowed_maximum_bus_number(size_t max_bus)
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    psdb.set_allowed_max_bus_number(max_bus);
}


size_t api_get_device_capacity(const char* device_type)
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
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


size_t api_get_area_capacity()
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    return psdb.get_area_capacity();
}

size_t api_get_zone_capacity()
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    return psdb.get_zone_capacity();
}

size_t api_get_owner_capacity()
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    return psdb.get_owner_capacity();
}


void api_set_device_capacity(const char* device_type, size_t cap)
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
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

void api_set_area_capacity(size_t cap)
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    return psdb.set_area_capacity(cap);
}

void api_set_zone_capacity(size_t cap)
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    return psdb.set_zone_capacity(cap);
}

void api_set_owner_capacity(size_t cap)
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    return psdb.set_owner_capacity(cap);
}

void api_clear_package()
{
    get_default_power_system_database().clear_database();
    get_default_dynamic_simulator().clear();

}

void api_terminate_package()
{
    terminate_simulator();
}

double api_get_package_float_data(char* parameter_name)
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    string PARAMETER_NAME = string2upper(parameter_name);
    if(PARAMETER_NAME=="SBASE")
        return psdb.get_system_base_power_in_MVA();

    show_parameter_not_supported_with_api(PARAMETER_NAME, __FUNCTION__);
    return 0.0;
}

void api_set_package_float_data(char* parameter_name, double value)
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    string PARAMETER_NAME = string2upper(parameter_name);
    if(PARAMETER_NAME=="SBASE")
        return psdb.set_system_base_power_in_MVA(value);

    show_parameter_not_supported_with_api(PARAMETER_NAME, __FUNCTION__);
}


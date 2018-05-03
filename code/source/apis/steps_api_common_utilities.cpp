#include "header/apis/steps_api.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

void show_side_not_supported_for_device_with_api(string side, DEVICE_ID did, string api_func)
{
    ostringstream sstream;
    sstream<<"Side '"<<side<<"' is not supported for "<<did.get_device_name()<<" with api "<<api_func<<endl
           <<"Nothing will be changed."<<endl
           <<"If return value is expected, 0/0.0/False/EMPTY STRING will be returned"<<endl;
    show_information_with_leading_time_stamp(sstream);
}

void show_parameter_not_supported_for_device_with_api(string par_name, DEVICE_ID did, string api_func)
{
    ostringstream sstream;
    sstream<<"Parameter '"<<par_name<<"' is not supported for "<<did.get_device_name()<<" with api "<<api_func<<endl
           <<"Nothing will be changed."<<endl
           <<"If return value is expected, 0/0.0/False/EMPTY STRING will be returned"<<endl;
    show_information_with_leading_time_stamp(sstream);
}

void show_parameter_not_supported_with_api(string par_name, string api_func)
{
    ostringstream sstream;
    sstream<<"Parameter '"<<par_name<<"' is not supported with api "<<api_func<<endl
           <<"Nothing will be changed."<<endl
           <<"If return value is expected, 0/0.0/False/EMPTY STRING will be returned"<<endl;
    show_information_with_leading_time_stamp(sstream);
}

void show_device_not_exist_with_api(DEVICE_ID did, string api_func)
{
    ostringstream sstream;
    sstream<<did.get_device_name()<<" does not exist in database with api "<<api_func<<endl
           <<"Nothing will be changed."<<endl
           <<"If return value is expected, 0/0.0/False/EMPTY STRING will be returned"<<endl;
    show_information_with_leading_time_stamp(sstream);
}


void show_parameter_not_supported_for_area_zone_owner_with_api(string par_name, size_t no, string api_func)
{
    ostringstream sstream;
    sstream<<"Parameter '"<<par_name<<"' is not supported for Area/Zone/Owner "<<no<<" with api "<<api_func<<endl
           <<"Nothing will be changed."<<endl
           <<"If return value is expected, 0/0.0/False/EMPTY STRING will be returned"<<endl;
    show_information_with_leading_time_stamp(sstream);
}

void show_area_zone_owner_not_exist_with_api(size_t no, string api_func)
{
    ostringstream sstream;
    sstream<<"Area/Zone/Owner "<<no<<" does not exist in database with api "<<api_func<<endl
           <<"Nothing will be changed."<<endl
           <<"If return value is expected, 0/0.0/False/EMPTY STRING will be returned"<<endl;
    show_information_with_leading_time_stamp(sstream);
}

void api_initialize_package()
{
    initialize_simulator();

    api_initialize_powerflow_solver();
}

void api_initialize_powerflow_solver()
{
    POWERFLOW_SOLVER* solver = api_get_default_powerflow_solver();
    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();
    solver->set_power_system_database(psdb);
}

size_t api_get_allowed_maximum_bus_number()
{
    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();
    return psdb->get_allowed_max_bus_number();
}

void api_set_allowed_maximum_bus_number(size_t max_bus)
{
    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();
    psdb->set_allowed_max_bus_number(max_bus);
}


size_t api_get_device_capacity(const char* device_type)
{
    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();
    string DEVICE_TYPE = string2upper(device_type);

    if(DEVICE_TYPE=="BUS")
        return psdb->get_bus_capacity();
    if(DEVICE_TYPE=="GENERATOR")
        return psdb->get_generator_capacity();
    if(DEVICE_TYPE=="WT GENERATOR")
        return psdb->get_wt_generator_capacity();
    if(DEVICE_TYPE=="LOAD")
        return psdb->get_load_capacity();
    if(DEVICE_TYPE=="FIXED SHUNT")
        return psdb->get_fixed_shunt_capacity();
    if(DEVICE_TYPE=="LINE")
        return psdb->get_line_capacity();
    if(DEVICE_TYPE=="TRANSFORMER")
        return psdb->get_transformer_capacity();
    if(DEVICE_TYPE=="HVDC")
        return psdb->get_hvdc_capacity();
    if(DEVICE_TYPE=="EQUIVALENT DEVICE")
        return psdb->get_equivalent_device_capacity();

    show_parameter_not_supported_with_api(DEVICE_TYPE, __FUNCTION__);
    return 0;
}


size_t api_get_area_capacity()
{
    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();
    return psdb->get_area_capacity();
}

size_t api_get_zone_capacity()
{
    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();
    return psdb->get_zone_capacity();
}

size_t api_get_owner_capacity()
{
    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();
    return psdb->get_owner_capacity();
}


void api_set_device_capacity(const char* device_type, size_t cap)
{
    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();
    string DEVICE_TYPE = string2upper(device_type);

    if(DEVICE_TYPE=="BUS")
        return psdb->set_bus_capacity(cap);
    if(DEVICE_TYPE=="GENERATOR")
        return psdb->set_generator_capacity(cap);
    if(DEVICE_TYPE=="WT GENERATOR")
        return psdb->set_wt_generator_capacity(cap);
    if(DEVICE_TYPE=="LOAD")
        return psdb->set_load_capacity(cap);
    if(DEVICE_TYPE=="FIXED SHUNT")
        return psdb->set_fixed_shunt_capacity(cap);
    if(DEVICE_TYPE=="LINE")
        return psdb->set_line_capacity(cap);
    if(DEVICE_TYPE=="TRANSFORMER")
        return psdb->set_transformer_capacity(cap);
    if(DEVICE_TYPE=="HVDC")
        return psdb->set_hvdc_capacity(cap);
    if(DEVICE_TYPE=="EQUIVALENT DEVICE")
        return psdb->set_equivalent_device_capacity(cap);

    show_parameter_not_supported_with_api(DEVICE_TYPE, __FUNCTION__);
}

void api_set_area_capacity(size_t cap)
{
    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();
    return psdb->set_area_capacity(cap);
}

void api_set_zone_capacity(size_t cap)
{
    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();
    return psdb->set_zone_capacity(cap);
}

void api_set_owner_capacity(size_t cap)
{
    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();
    return psdb->set_owner_capacity(cap);
}

void api_clear_package()
{
    STEPS::power_system_db[0].clear_database();
}

void api_terminate_package()
{
    terminate_simulator();
}

double api_get_package_float_data(char* parameter_name)
{
    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();

    string PARAMETER_NAME = string2upper(parameter_name);
    if(PARAMETER_NAME=="FBASE")
        return psdb->get_system_base_frequency_in_Hz();
    if(PARAMETER_NAME=="SBASE")
        return psdb->get_system_base_power_in_MVA();

    show_parameter_not_supported_with_api(PARAMETER_NAME, __FUNCTION__);
    return 0.0;
}

void api_set_package_float_data(char* parameter_name, double value)
{
    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();

    string PARAMETER_NAME = string2upper(parameter_name);
    if(PARAMETER_NAME=="FBASE")
        return psdb->set_system_base_frequency_in_Hz(value);
    if(PARAMETER_NAME=="SBASE")
        return psdb->set_system_base_power_in_MVA(value);

    show_parameter_not_supported_with_api(PARAMETER_NAME, __FUNCTION__);
}


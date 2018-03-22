#include "header/apis/steps_api.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/data_imexporter/psse_imexporter.h"
#include "header/data_imexporter/bpa_imexporter.h"

size_t api_get_device_capacity(const char* device_type)
{
    POWER_SYSTEM_DATABASE* psdb = api_get_default_power_system_database();
    string DEVICE_TYPE = string2upper(device_type);

    if(DEVICE_TYPE=="BUS")
        return psdb->get_bus_capacity();
    if(DEVICE_TYPE=="GENERATOR")
        return psdb->get_generator_capacity();
    if(DEVICE_TYPE=="PE SOURCE")
        return psdb->get_pe_source_capacity();
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
    if(DEVICE_TYPE=="AREA")
        return psdb->get_area_capacity();
    if(DEVICE_TYPE=="ZONE")
        return psdb->get_zone_capacity();
    if(DEVICE_TYPE=="OWNER")
        return psdb->get_owner_capacity();

    show_parameter_not_supported_with_api(DEVICE_TYPE, __FUNCTION__);
    return 0;
}


void api_set_device_capacity(const char* device_type, size_t cap)
{
    POWER_SYSTEM_DATABASE* psdb = api_get_default_power_system_database();
    string DEVICE_TYPE = string2upper(device_type);

    if(DEVICE_TYPE=="BUS")
        return psdb->set_bus_capacity(cap);
    if(DEVICE_TYPE=="GENERATOR")
        return psdb->set_generator_capacity(cap);
    if(DEVICE_TYPE=="PE SOURCE")
        return psdb->set_pe_source_capacity(cap);
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
    if(DEVICE_TYPE=="AREA")
        return psdb->set_area_capacity(cap);
    if(DEVICE_TYPE=="ZONE")
        return psdb->set_zone_capacity(cap);
    if(DEVICE_TYPE=="OWNER")
        return psdb->set_owner_capacity(cap);

    show_parameter_not_supported_with_api(DEVICE_TYPE, __FUNCTION__);
}


size_t api_get_device_count(const char* device_type)
{
    POWER_SYSTEM_DATABASE* psdb = api_get_default_power_system_database();
    string DEVICE_TYPE = string2upper(device_type);

    if(DEVICE_TYPE=="BUS")
        return psdb->get_bus_count();
    if(DEVICE_TYPE=="GENERATOR")
        return psdb->get_generator_count();
    if(DEVICE_TYPE=="PE SOURCE")
        return psdb->get_pe_source_count();
    if(DEVICE_TYPE=="LOAD")
        return psdb->get_load_count();
    if(DEVICE_TYPE=="FIXED SHUNT")
        return psdb->get_fixed_shunt_count();
    if(DEVICE_TYPE=="LINE")
        return psdb->get_line_count();
    if(DEVICE_TYPE=="TRANSFORMER")
        return psdb->get_transformer_count();
    if(DEVICE_TYPE=="HVDC")
        return psdb->get_hvdc_count();
    if(DEVICE_TYPE=="EQUIVALENT DEVICE")
        return psdb->get_equivalent_device_count();
    if(DEVICE_TYPE=="AREA")
        return psdb->get_area_count();
    if(DEVICE_TYPE=="ZONE")
        return psdb->get_zone_count();
    if(DEVICE_TYPE=="OWNER")
        return psdb->get_owner_count();

    show_parameter_not_supported_with_api(DEVICE_TYPE, __FUNCTION__);
    return 0;
}

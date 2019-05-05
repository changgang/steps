#include "header/apis/steps_api.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/data_imexporter/psse_imexporter.h"
#include "header/data_imexporter/bpa_imexporter.h"

void api_remove_bus(size_t bus_number, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    if(not psdb.is_bus_exist(bus_number))
        psdb.clear_bus(bus_number);
}

void api_remove_generator(size_t bus_number, char* identifier, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DEVICE_ID did = get_generator_device_id(bus_number, identifier);
    if(not psdb.is_generator_exist(did))
        psdb.clear_generator(did);
}

void api_remove_wt_generator(size_t bus_number, char* identifier, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DEVICE_ID did = get_wt_generator_device_id(bus_number, identifier);
    if(not psdb.is_wt_generator_exist(did))
        psdb.clear_wt_generator(did);
}

void api_remove_pv_unit(size_t bus_number, char* identifier, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DEVICE_ID did = get_pv_unit_device_id(bus_number, identifier);
    if(not psdb.is_pv_unit_exist(did))
        psdb.clear_pv_unit(did);
}

void api_remove_load(size_t bus_number, char* identifier, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DEVICE_ID did = get_load_device_id(bus_number, identifier);
    if(not psdb.is_load_exist(did))
        psdb.clear_load(did);
}

void api_remove_fixed_shunt(size_t bus_number, char* identifier, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DEVICE_ID did = get_fixed_shunt_device_id(bus_number, identifier);
    if(not psdb.is_fixed_shunt_exist(did))
        psdb.clear_fixed_shunt(did);
}

void api_remove_line(size_t sending_side_bus_number, size_t receiving_side_bus_number, char* identifier, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DEVICE_ID did = get_line_device_id(sending_side_bus_number, receiving_side_bus_number, identifier);
    if(not psdb.is_line_exist(did))
        psdb.clear_line(did);
}

void api_remove_hvdc(size_t rectifier_bus_number, size_t inverter_bus_number, char* identifier, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DEVICE_ID did = get_hvdc_device_id(rectifier_bus_number, inverter_bus_number, identifier);
    if(not psdb.is_hvdc_exist(did))
        psdb.clear_hvdc(did);
}

void api_remove_transformer(size_t primary_side_bus_number, size_t secondary_side_bus_number, size_t tertiary_side_bus_number, char* identifier, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DEVICE_ID did = get_transformer_device_id(primary_side_bus_number, secondary_side_bus_number, tertiary_side_bus_number, identifier);
    if(not psdb.is_transformer_exist(did))
        psdb.clear_transformer(did);
}

void api_remove_equivalent_device(size_t bus_number, char* identifier, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DEVICE_ID did = get_equivalent_device_id(bus_number, identifier);
    if(not psdb.is_equivalent_device_exist(did))
        psdb.clear_equivalent_device(did);
}

void api_remove_energy_storage(size_t bus_number, char* identifier, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DEVICE_ID did = get_energy_storage_device_id(bus_number, identifier);
    if(not psdb.is_energy_storage_exist(did))
        psdb.clear_energy_storage(did);
}

void api_remove_area(size_t area_number, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    if(not psdb.is_area_exist(area_number))
        psdb.clear_area(area_number);
}

void api_remove_zone(size_t zone_number, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    if(not psdb.is_zone_exist(zone_number))
        psdb.clear_zone(zone_number);
}

void api_remove_owner(size_t owner_number, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    if(not psdb.is_owner_exist(owner_number))
        psdb.clear_owner(owner_number);
}

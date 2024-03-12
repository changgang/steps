#include "header/apis/steps_api.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/data_imexporter/psse_imexporter.h"
#include "header/data_imexporter/bpa_imexporter.h"

void api_remove_bus(unsigned int bus_number, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    if(psdb.is_bus_exist(bus_number))
        psdb.clear_bus(bus_number);
}

void api_remove_generator(unsigned int bus_number, char* identifier, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DEVICE_ID did = get_generator_device_id(bus_number, identifier);
    if(psdb.is_generator_exist(did))
        psdb.clear_generator(did);
}

void api_remove_wt_generator(unsigned int bus_number, char* identifier, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DEVICE_ID did = get_wt_generator_device_id(bus_number, identifier);
    if(psdb.is_wt_generator_exist(did))
        psdb.clear_wt_generator(did);
}

void api_remove_pv_unit(unsigned int bus_number, char* identifier, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DEVICE_ID did = get_pv_unit_device_id(bus_number, identifier);
    if(psdb.is_pv_unit_exist(did))
        psdb.clear_pv_unit(did);
}

void api_remove_load(unsigned int bus_number, char* identifier, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DEVICE_ID did = get_load_device_id(bus_number, identifier);
    if(psdb.is_load_exist(did))
        psdb.clear_load(did);
}

void api_remove_fixed_shunt(unsigned int bus_number, char* identifier, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DEVICE_ID did = get_fixed_shunt_device_id(bus_number, identifier);
    if(psdb.is_fixed_shunt_exist(did))
        psdb.clear_fixed_shunt(did);
}

void api_remove_line(unsigned int sending_side_bus_number, unsigned int receiving_side_bus_number, char* identifier, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DEVICE_ID did = get_ac_line_device_id(sending_side_bus_number, receiving_side_bus_number, identifier);
    if(psdb.is_ac_line_exist(did))
        psdb.clear_ac_line(did);
}

void api_remove_hvdc(unsigned int rectifier_bus_number, unsigned int inverter_bus_number, char* identifier, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DEVICE_ID did = get_2t_lcc_hvdc_device_id(rectifier_bus_number, inverter_bus_number, identifier);
    if(psdb.is_2t_lcc_hvdc_exist(did))
        psdb.clear_2t_lcc_hvdc(did);
}

void api_remove_vsc_hvdc(char* vsc_name, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    if(psdb.is_vsc_hvdc_exist(vsc_name))
        psdb.clear_vsc_hvdc(vsc_name);
}

void api_remove_transformer(unsigned int primary_side_bus_number, unsigned int secondary_side_bus_number, unsigned int tertiary_side_bus_number, char* identifier, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DEVICE_ID did = get_transformer_device_id(primary_side_bus_number, secondary_side_bus_number, tertiary_side_bus_number, identifier);
    if(psdb.is_transformer_exist(did))
        psdb.clear_transformer(did);
}

void api_remove_equivalent_device(unsigned int bus_number, char* identifier, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DEVICE_ID did = get_equivalent_device_id(bus_number, identifier);
    if(psdb.is_equivalent_device_exist(did))
        psdb.clear_equivalent_device(did);
}

void api_remove_energy_storage(unsigned int bus_number, char* identifier, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DEVICE_ID did = get_energy_storage_device_id(bus_number, identifier);
    if(psdb.is_energy_storage_exist(did))
        psdb.clear_energy_storage(did);
}

void api_remove_area(unsigned int area_number, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    if(psdb.is_area_exist(area_number))
        psdb.clear_area(area_number);
}

void api_remove_zone(unsigned int zone_number, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    if(psdb.is_zone_exist(zone_number))
        psdb.clear_zone(zone_number);
}

void api_remove_owner(unsigned int owner_number, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    if(psdb.is_owner_exist(owner_number))
        psdb.clear_owner(owner_number);
}

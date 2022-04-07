#include <istream>
#include "header/apis/steps_api.h"
#include "header/apis/steps_api_common.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/data_imexporter/psse_imexporter.h"
#include "header/data_imexporter/bpa_imexporter.h"
#include <iostream>
using namespace std;

unsigned int api_bus_name2bus_number(const char* bus_name, double vbase, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    return psdb.bus_name2bus_number(bus_name, vbase);
}

const char* api_bus_number2bus_name(unsigned int bus_number, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    string name = psdb.bus_number2bus_name(bus_number);

    snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", name.c_str());
    return toolkit.steps_char_buffer;
}

void api_initialize_bus_search(double vbase_kV_min, double vbase_kV_max, double v_pu_min, double v_pu_max, unsigned int area, unsigned int zone, unsigned int owner, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    toolkit.api_search_buffer.buses = psdb.get_buses_with_constraints(vbase_kV_min, vbase_kV_max, v_pu_min, v_pu_max, area, zone, owner);
    toolkit.api_search_buffer.bus_pointer = 0;
}

void api_initialize_all_bus_search(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    toolkit.api_search_buffer.buses = psdb.get_all_buses();
    toolkit.api_search_buffer.bus_pointer = 0;
}

unsigned int api_get_current_bus_number(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    unsigned int index = toolkit.api_search_buffer.bus_pointer;
    unsigned int n = toolkit.api_search_buffer.buses.size();
    if(index<n)
        return toolkit.api_search_buffer.buses[index]->get_bus_number();
    else
        return 0;
}

void api_goto_next_bus(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    unsigned int index = toolkit.api_search_buffer.bus_pointer;
    unsigned int n = toolkit.api_search_buffer.buses.size();
    if(index<n)
        ++(toolkit.api_search_buffer.bus_pointer);
}

void api_initialize_device_search(const char* device_type, unsigned int bus, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    string DEVICE_TYPE = string2upper(device_type);
    if(DEVICE_TYPE=="GENERATOR")
    {
        if(bus==0)
            toolkit.api_search_buffer.generators = psdb.get_all_generators();
        else
            toolkit.api_search_buffer.generators = psdb.get_generators_connecting_to_bus(bus);
        toolkit.api_search_buffer.generator_pointer = 0;
    }

    if(DEVICE_TYPE=="WT GENERATOR")
    {
        if(bus==0)
            toolkit.api_search_buffer.wt_generators = psdb.get_all_wt_generators();
        else
            toolkit.api_search_buffer.wt_generators = psdb.get_wt_generators_connecting_to_bus(bus);
        toolkit.api_search_buffer.wt_generator_pointer = 0;
    }

    if(DEVICE_TYPE=="PV UNIT")
    {
        if(bus==0)
            toolkit.api_search_buffer.pv_units = psdb.get_all_pv_units();
        else
            toolkit.api_search_buffer.pv_units = psdb.get_pv_units_connecting_to_bus(bus);
        toolkit.api_search_buffer.pv_unit_pointer = 0;
    }

    if(DEVICE_TYPE=="LOAD")
    {
        if(bus==0)
            toolkit.api_search_buffer.loads = psdb.get_all_loads();
        else
            toolkit.api_search_buffer.loads = psdb.get_loads_connecting_to_bus(bus);
        toolkit.api_search_buffer.load_pointer = 0;
    }

    if(DEVICE_TYPE=="FIXED SHUNT")
    {
        if(bus==0)
            toolkit.api_search_buffer.fixed_shunts = psdb.get_all_fixed_shunts();
        else
            toolkit.api_search_buffer.fixed_shunts = psdb.get_fixed_shunts_connecting_to_bus(bus);
        toolkit.api_search_buffer.fixed_shunt_pointer = 0;
    }

    if(DEVICE_TYPE=="LINE")
    {
        if(bus==0)
            toolkit.api_search_buffer.lines = psdb.get_all_lines();
        else
            toolkit.api_search_buffer.lines = psdb.get_lines_connecting_to_bus(bus);
        toolkit.api_search_buffer.line_pointer = 0;
    }

    if(DEVICE_TYPE=="TRANSFORMER")
    {
        if(bus==0)
            toolkit.api_search_buffer.transformers = psdb.get_all_transformers();
        else
            toolkit.api_search_buffer.transformers = psdb.get_transformers_connecting_to_bus(bus);
        toolkit.api_search_buffer.transformer_pointer = 0;
    }

    if(DEVICE_TYPE=="HVDC")
    {
        if(bus==0)
            toolkit.api_search_buffer.hvdcs = psdb.get_all_hvdcs();
        else
            toolkit.api_search_buffer.hvdcs = psdb.get_hvdcs_connecting_to_bus(bus);
        toolkit.api_search_buffer.hvdc_pointer = 0;
    }

    if(DEVICE_TYPE=="VSC HVDC")
    {
        if(bus==0)
            toolkit.api_search_buffer.vscs = psdb.get_all_vsc_hvdcs();
        else
            toolkit.api_search_buffer.vscs = psdb.get_vsc_hvdcs_connecting_to_bus(bus);
        toolkit.api_search_buffer.vsc_pointer = 0;
    }

    if(DEVICE_TYPE=="EQUIVALENT DEVICE")
    {
        if(bus==0)
            toolkit.api_search_buffer.equivalent_devices = psdb.get_all_equivalent_devices();
        else
            toolkit.api_search_buffer.equivalent_devices = psdb.get_equivalent_devices_connecting_to_bus(bus);
        toolkit.api_search_buffer.equivalent_device_pointer = 0;
    }

    if(DEVICE_TYPE=="ENERGY STORAGE")
    {
        if(bus==0)
            toolkit.api_search_buffer.energy_storages = psdb.get_all_energy_storages();
        else
            toolkit.api_search_buffer.energy_storages = psdb.get_energy_storages_connecting_to_bus(bus);
        toolkit.api_search_buffer.energy_storage_pointer = 0;
    }
}

unsigned int api_get_current_device_bus_number(const char* device_type, const char* side, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    string DEVICE_TYPE = string2upper(device_type);
    string SIDE = string2upper(side);
    if(DEVICE_TYPE=="GENERATOR")
    {
        unsigned int index = toolkit.api_search_buffer.generator_pointer;
        unsigned int n = toolkit.api_search_buffer.generators.size();
        if(index<n)
            return toolkit.api_search_buffer.generators[index]->get_generator_bus();
        else
            return 0;
    }

    if(DEVICE_TYPE=="WT GENERATOR")
    {
        unsigned int index = toolkit.api_search_buffer.wt_generator_pointer;
        unsigned int n = toolkit.api_search_buffer.wt_generators.size();
        if(index<n)
            return toolkit.api_search_buffer.wt_generators[index]->get_source_bus();
        else
            return 0;
    }

    if(DEVICE_TYPE=="PV UNIT")
    {
        unsigned int index = toolkit.api_search_buffer.pv_unit_pointer;
        unsigned int n = toolkit.api_search_buffer.pv_units.size();
        if(index<n)
            return toolkit.api_search_buffer.pv_units[index]->get_unit_bus();
        else
            return 0;
    }

    if(DEVICE_TYPE=="LOAD")
    {
        unsigned int index = toolkit.api_search_buffer.load_pointer;
        unsigned int n = toolkit.api_search_buffer.loads.size();
        if(index<n)
            return toolkit.api_search_buffer.loads[index]->get_load_bus();
        else
            return 0;
    }

    if(DEVICE_TYPE=="FIXED SHUNT")
    {
        unsigned int index = toolkit.api_search_buffer.fixed_shunt_pointer;
        unsigned int n = toolkit.api_search_buffer.fixed_shunts.size();
        if(index<n)
            return toolkit.api_search_buffer.fixed_shunts[index]->get_shunt_bus();
        else
            return 0;
    }

    if(DEVICE_TYPE=="LINE")
    {
        unsigned int index = toolkit.api_search_buffer.line_pointer;
        unsigned int n = toolkit.api_search_buffer.lines.size();
        if(index<n)
        {
            if(SIDE=="SENDING" or SIDE=="SEND")
                return toolkit.api_search_buffer.lines[index]->get_sending_side_bus();
            else
                return toolkit.api_search_buffer.lines[index]->get_receiving_side_bus();
        }
        else
            return 0;
    }

    if(DEVICE_TYPE=="TRANSFORMER")
    {
        unsigned int index = toolkit.api_search_buffer.transformer_pointer;
        unsigned int n = toolkit.api_search_buffer.transformers.size();
        if(index<n)
        {
            if(SIDE=="PRIMARY")
                return toolkit.api_search_buffer.transformers[index]->get_winding_bus(PRIMARY_SIDE);
            else
            {
                if(SIDE=="SECONDARY")
                    return toolkit.api_search_buffer.transformers[index]->get_winding_bus(SECONDARY_SIDE);
                else
                    return toolkit.api_search_buffer.transformers[index]->get_winding_bus(TERTIARY_SIDE);
            }
        }
        else
            return 0;
    }

    if(DEVICE_TYPE=="HVDC")
    {
        unsigned int index = toolkit.api_search_buffer.hvdc_pointer;
        unsigned int n = toolkit.api_search_buffer.hvdcs.size();
        if(index<n)
        {
            if(SIDE=="RECTIFIER" or SIDE=="REC")
                return toolkit.api_search_buffer.hvdcs[index]->get_converter_bus(RECTIFIER);
            else
                return toolkit.api_search_buffer.hvdcs[index]->get_converter_bus(INVERTER);
        }
        else
            return 0;
    }

    if(DEVICE_TYPE=="VSC HVDC")
    {
        unsigned int index = toolkit.api_search_buffer.vsc_pointer;
        unsigned int n = toolkit.api_search_buffer.vscs.size();
        if(index<n)
            return toolkit.api_search_buffer.vscs[index]->get_converter_count();
        else
            return 0;
    }

    if(DEVICE_TYPE=="EQUIVALENT DEVICE")
    {
        unsigned int index = toolkit.api_search_buffer.equivalent_device_pointer;
        unsigned int n = toolkit.api_search_buffer.equivalent_devices.size();
        if(index<n)
            return toolkit.api_search_buffer.equivalent_devices[index]->get_equivalent_device_bus();
        else
            return 0;
    }

    if(DEVICE_TYPE=="ENERGY STORAGE")
    {
        unsigned int index = toolkit.api_search_buffer.energy_storage_pointer;
        unsigned int n = toolkit.api_search_buffer.energy_storages.size();
        if(index<n)
            return toolkit.api_search_buffer.energy_storages[index]->get_energy_storage_bus();
        else
            return 0;
    }

    show_parameter_not_supported_with_api(DEVICE_TYPE, __FUNCTION__);
    return 0;
}

 const char* api_get_current_device_identifier(const char* device_type, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
	snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s","");
    string DEVICE_TYPE = string2upper(device_type);
    if(DEVICE_TYPE=="GENERATOR")
    {
        unsigned int index = toolkit.api_search_buffer.generator_pointer;
        unsigned int n = toolkit.api_search_buffer.generators.size();
		if (index < n)
		{
			snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (toolkit.api_search_buffer.generators[index]->get_identifier()).c_str());
			return toolkit.steps_char_buffer;
		}
        else
            return toolkit.steps_char_buffer;
    }

    if(DEVICE_TYPE=="WT GENERATOR")
    {
        unsigned int index = toolkit.api_search_buffer.wt_generator_pointer;
        unsigned int n = toolkit.api_search_buffer.wt_generators.size();
        if(index<n)
		{
			snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (toolkit.api_search_buffer.wt_generators[index]->get_identifier()).c_str());
			return toolkit.steps_char_buffer;
		}
        else
            return toolkit.steps_char_buffer;
    }

    if(DEVICE_TYPE=="PV UNIT")
    {
        unsigned int index = toolkit.api_search_buffer.pv_unit_pointer;
        unsigned int n = toolkit.api_search_buffer.pv_units.size();
        if(index<n)
		{
			snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (toolkit.api_search_buffer.pv_units[index]->get_identifier()).c_str());
			return toolkit.steps_char_buffer;
		}
        else
            return toolkit.steps_char_buffer;
    }

    if(DEVICE_TYPE=="LOAD")
    {
        unsigned int index = toolkit.api_search_buffer.load_pointer;
        unsigned int n = toolkit.api_search_buffer.loads.size();
        if(index<n)
		{
			snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (toolkit.api_search_buffer.loads[index]->get_identifier()).c_str());
			return toolkit.steps_char_buffer;
		}
        else
            return toolkit.steps_char_buffer;
    }

    if(DEVICE_TYPE=="FIXED SHUNT")
    {
        unsigned int index = toolkit.api_search_buffer.fixed_shunt_pointer;
        unsigned int n = toolkit.api_search_buffer.fixed_shunts.size();
        if(index<n)
		{
			snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (toolkit.api_search_buffer.fixed_shunts[index]->get_identifier()).c_str());
			return toolkit.steps_char_buffer;
		}
        else
            return toolkit.steps_char_buffer;
    }

    if(DEVICE_TYPE=="LINE")
    {
        unsigned int index = toolkit.api_search_buffer.line_pointer;
        unsigned int n = toolkit.api_search_buffer.lines.size();
        if(index<n)
		{
			snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (toolkit.api_search_buffer.lines[index]->get_identifier()).c_str());
			return toolkit.steps_char_buffer;
		}
        else
            return toolkit.steps_char_buffer;
    }

    if(DEVICE_TYPE=="TRANSFORMER")
    {
        unsigned int index = toolkit.api_search_buffer.transformer_pointer;
        unsigned int n = toolkit.api_search_buffer.transformers.size();
        if(index<n)
		{
			snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (toolkit.api_search_buffer.transformers[index]->get_identifier()).c_str());
			return toolkit.steps_char_buffer;
		}
        else
            return toolkit.steps_char_buffer;
    }

    if(DEVICE_TYPE=="HVDC")
    {
        unsigned int index = toolkit.api_search_buffer.hvdc_pointer;
        unsigned int n = toolkit.api_search_buffer.hvdcs.size();
        if(index<n)
		{
			snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (toolkit.api_search_buffer.hvdcs[index]->get_identifier()).c_str());
			return toolkit.steps_char_buffer;
		}
        else
            return toolkit.steps_char_buffer;
    }

    if(DEVICE_TYPE=="VSC HVDC")
    {
        unsigned int index = toolkit.api_search_buffer.vsc_pointer;
        unsigned int n = toolkit.api_search_buffer.vscs.size();
        if(index<n)
		{
			snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (toolkit.api_search_buffer.vscs[index]->get_identifier()).c_str());
			return toolkit.steps_char_buffer;
		}
        else
            return toolkit.steps_char_buffer;
    }

    if(DEVICE_TYPE=="EQUIVALENT DEVICE")
    {
        unsigned int index = toolkit.api_search_buffer.equivalent_device_pointer;
        unsigned int n = toolkit.api_search_buffer.equivalent_devices.size();
        if(index<n)
		{
			snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (toolkit.api_search_buffer.equivalent_devices[index]->get_identifier()).c_str());
			return toolkit.steps_char_buffer;
		}
        else
            return toolkit.steps_char_buffer;
    }

    if(DEVICE_TYPE=="ENERGY STORAGE")
    {
        unsigned int index = toolkit.api_search_buffer.energy_storage_pointer;
        unsigned int n = toolkit.api_search_buffer.energy_storages.size();
        if(index<n)
		{
			snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (toolkit.api_search_buffer.energy_storages[index]->get_identifier()).c_str());
			return toolkit.steps_char_buffer;
		}
        else
            return toolkit.steps_char_buffer;
    }

    show_parameter_not_supported_with_api(DEVICE_TYPE, __FUNCTION__);
    return toolkit.steps_char_buffer;
}

void api_goto_next_device(const char* device_type, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    string DEVICE_TYPE = string2upper(device_type);
    if(DEVICE_TYPE=="GENERATOR")
    {
        unsigned int index = toolkit.api_search_buffer.generator_pointer;
        unsigned int n = toolkit.api_search_buffer.generators.size();
        if(index<n)
            ++(toolkit.api_search_buffer.generator_pointer);
        return;
    }

    if(DEVICE_TYPE=="WT GENERATOR")
    {
        unsigned int index = toolkit.api_search_buffer.wt_generator_pointer;
        unsigned int n = toolkit.api_search_buffer.wt_generators.size();
        if(index<n)
            ++(toolkit.api_search_buffer.wt_generator_pointer);
        return;
    }

    if(DEVICE_TYPE=="PV UNIT")
    {
        unsigned int index = toolkit.api_search_buffer.pv_unit_pointer;
        unsigned int n = toolkit.api_search_buffer.pv_units.size();
        if(index<n)
            ++(toolkit.api_search_buffer.pv_unit_pointer);
        return;
    }

    if(DEVICE_TYPE=="LOAD")
    {
        unsigned int index = toolkit.api_search_buffer.load_pointer;
        unsigned int n = toolkit.api_search_buffer.loads.size();
        if(index<n)
            ++(toolkit.api_search_buffer.load_pointer);
        return;
    }

    if(DEVICE_TYPE=="FIXED SHUNT")
    {
        unsigned int index = toolkit.api_search_buffer.fixed_shunt_pointer;
        unsigned int n = toolkit.api_search_buffer.fixed_shunts.size();
        if(index<n)
            ++(toolkit.api_search_buffer.fixed_shunt_pointer);
        return;
    }

    if(DEVICE_TYPE=="LINE")
    {
        unsigned int index = toolkit.api_search_buffer.line_pointer;
        unsigned int n = toolkit.api_search_buffer.lines.size();
        if(index<n)
            ++(toolkit.api_search_buffer.line_pointer);
        return;
    }

    if(DEVICE_TYPE=="TRANSFORMER")
    {
        unsigned int index = toolkit.api_search_buffer.transformer_pointer;
        unsigned int n = toolkit.api_search_buffer.transformers.size();
        if(index<n)
            ++(toolkit.api_search_buffer.transformer_pointer);
        return;
    }

    if(DEVICE_TYPE=="HVDC")
    {
        unsigned int index = toolkit.api_search_buffer.hvdc_pointer;
        unsigned int n = toolkit.api_search_buffer.hvdcs.size();
        if(index<n)
            ++(toolkit.api_search_buffer.hvdc_pointer);
        return;
    }

    if(DEVICE_TYPE=="VSC HVDC")
    {
        unsigned int index = toolkit.api_search_buffer.vsc_pointer;
        unsigned int n = toolkit.api_search_buffer.vscs.size();
        if(index<n)
            ++(toolkit.api_search_buffer.vsc_pointer);
        return;
    }

    if(DEVICE_TYPE=="EQUIVALENT DEVICE")
    {
        unsigned int index = toolkit.api_search_buffer.equivalent_device_pointer;
        unsigned int n = toolkit.api_search_buffer.equivalent_devices.size();
        if(index<n)
            ++(toolkit.api_search_buffer.equivalent_device_pointer);
        return;
    }

    if(DEVICE_TYPE=="ENERGY STORAGE")
    {
        unsigned int index = toolkit.api_search_buffer.energy_storage_pointer;
        unsigned int n = toolkit.api_search_buffer.energy_storages.size();
        if(index<n)
            ++(toolkit.api_search_buffer.energy_storage_pointer);
        return;
    }

    show_parameter_not_supported_with_api(DEVICE_TYPE, __FUNCTION__);
}

void api_initialize_area_search(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    toolkit.api_search_buffer.areas = psdb.get_all_areas();
    toolkit.api_search_buffer.area_pointer = 0;
}

unsigned int api_get_current_area_number(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    unsigned int index = toolkit.api_search_buffer.area_pointer;
    unsigned int n = toolkit.api_search_buffer.areas.size();
    if(index<n)
        return toolkit.api_search_buffer.areas[index]->get_area_number();
    else
        return 0;
}

void api_goto_next_area(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    unsigned int index = toolkit.api_search_buffer.area_pointer;
    unsigned int n = toolkit.api_search_buffer.areas.size();
    if(index<n)
		++(toolkit.api_search_buffer.area_pointer);
}

void api_initialize_zone_search(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    toolkit.api_search_buffer.zones = psdb.get_all_zones();
    toolkit.api_search_buffer.zone_pointer = 0;
}

unsigned int api_get_current_zone_number(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    unsigned int index = toolkit.api_search_buffer.zone_pointer;
    unsigned int n = toolkit.api_search_buffer.zones.size();
    if(index<n)
        return toolkit.api_search_buffer.zones[index]->get_zone_number();
    else
        return 0;
}

void api_goto_next_zone(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    unsigned int index = toolkit.api_search_buffer.zone_pointer;
    unsigned int n = toolkit.api_search_buffer.zones.size();
    if(index<n)
		++(toolkit.api_search_buffer.zone_pointer);
}

void api_initialize_owner_search(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    toolkit.api_search_buffer.owners = psdb.get_all_owners();
    toolkit.api_search_buffer.owner_pointer = 0;
}

unsigned int api_get_current_owner_number(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    unsigned int index = toolkit.api_search_buffer.owner_pointer;
    unsigned int n = toolkit.api_search_buffer.owners.size();
    if(index<n)
        return toolkit.api_search_buffer.owners[index]->get_owner_number();
    else
        return 0;
}

void api_goto_next_owner(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    unsigned int index = toolkit.api_search_buffer.owner_pointer;
    unsigned int n = toolkit.api_search_buffer.owners.size();
    if(index<n)
		++(toolkit.api_search_buffer.owner_pointer);
}

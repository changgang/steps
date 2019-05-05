#include <istream>
#include "header/apis/steps_api.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/data_imexporter/psse_imexporter.h"
#include "header/data_imexporter/bpa_imexporter.h"
#include <iostream>
using namespace std;

size_t api_bus_name2bus_number(const char* bus_name, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    return psdb.bus_name2bus_number(bus_name);
}

const char* api_bus_number2bus_name(size_t bus_number, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    string name = psdb.bus_number2bus_name(bus_number);

    snprintf(toolkit.steps_char_buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s", name.c_str());
    return toolkit.steps_char_buffer;
}

void api_initialize_bus_search(double vbase_kV_min, double vbase_kV_max, double v_pu_min, double v_pu_max, size_t area, size_t zone, size_t owner, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    toolkit.api_search_buffer.buses = psdb.get_buses_with_constraints(vbase_kV_min, vbase_kV_max, v_pu_min, v_pu_max, area, zone, owner);
    toolkit.api_search_buffer.bus_pointer = 0;
}

void api_initialize_all_bus_search(size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    toolkit.api_search_buffer.buses = psdb.get_all_buses();
    toolkit.api_search_buffer.bus_pointer = 0;
}

size_t api_get_current_bus_number(size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
    size_t index = toolkit.api_search_buffer.bus_pointer;
    size_t n = toolkit.api_search_buffer.buses.size();
    if(index<n)
        return toolkit.api_search_buffer.buses[index]->get_bus_number();
    else
        return 0;
}

void api_goto_next_bus(size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
    size_t index = toolkit.api_search_buffer.bus_pointer;
    size_t n = toolkit.api_search_buffer.buses.size();
    if(index<n)
        ++toolkit.api_search_buffer.bus_pointer;
}

void api_initialize_device_search(const char* device_type, size_t bus, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
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

size_t api_get_current_device_bus_number(const char* device_type, const char* side, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
    string DEVICE_TYPE = string2upper(device_type);
    string SIDE = string2upper(side);
    if(DEVICE_TYPE=="GENERATOR")
    {
        size_t index = toolkit.api_search_buffer.generator_pointer;
        size_t n = toolkit.api_search_buffer.generators.size();
        if(index<n)
            return toolkit.api_search_buffer.generators[index]->get_generator_bus();
        else
            return 0;
    }

    if(DEVICE_TYPE=="WT GENERATOR")
    {
        size_t index = toolkit.api_search_buffer.wt_generator_pointer;
        size_t n = toolkit.api_search_buffer.wt_generators.size();
        if(index<n)
            return toolkit.api_search_buffer.wt_generators[index]->get_source_bus();
        else
            return 0;
    }

    if(DEVICE_TYPE=="PV UNIT")
    {
        size_t index = toolkit.api_search_buffer.pv_unit_pointer;
        size_t n = toolkit.api_search_buffer.pv_units.size();
        if(index<n)
            return toolkit.api_search_buffer.pv_units[index]->get_unit_bus();
        else
            return 0;
    }

    if(DEVICE_TYPE=="LOAD")
    {
        size_t index = toolkit.api_search_buffer.load_pointer;
        size_t n = toolkit.api_search_buffer.loads.size();
        if(index<n)
            return toolkit.api_search_buffer.loads[index]->get_load_bus();
        else
            return 0;
    }

    if(DEVICE_TYPE=="FIXED SHUNT")
    {
        size_t index = toolkit.api_search_buffer.fixed_shunt_pointer;
        size_t n = toolkit.api_search_buffer.fixed_shunts.size();
        if(index<n)
            return toolkit.api_search_buffer.fixed_shunts[index]->get_shunt_bus();
        else
            return 0;
    }

    if(DEVICE_TYPE=="LINE")
    {
        size_t index = toolkit.api_search_buffer.line_pointer;
        size_t n = toolkit.api_search_buffer.lines.size();
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
        size_t index = toolkit.api_search_buffer.transformer_pointer;
        size_t n = toolkit.api_search_buffer.transformers.size();
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
        size_t index = toolkit.api_search_buffer.hvdc_pointer;
        size_t n = toolkit.api_search_buffer.hvdcs.size();
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

    if(DEVICE_TYPE=="EQUIVALENT DEVICE")
    {
        size_t index = toolkit.api_search_buffer.equivalent_device_pointer;
        size_t n = toolkit.api_search_buffer.equivalent_devices.size();
        if(index<n)
            return toolkit.api_search_buffer.equivalent_devices[index]->get_equivalent_device_bus();
        else
            return 0;
    }

    if(DEVICE_TYPE=="ENERGY STORAGE")
    {
        size_t index = toolkit.api_search_buffer.energy_storage_pointer;
        size_t n = toolkit.api_search_buffer.energy_storages.size();
        if(index<n)
            return toolkit.api_search_buffer.energy_storages[index]->get_energy_storage_bus();
        else
            return 0;
    }

    show_parameter_not_supported_with_api(DEVICE_TYPE, __FUNCTION__);
    return 0;
}

 const char* api_get_current_device_identifier(const char* device_type, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
	snprintf(toolkit.steps_char_buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s","");
    string DEVICE_TYPE = string2upper(device_type);
    if(DEVICE_TYPE=="GENERATOR")
    {
        size_t index = toolkit.api_search_buffer.generator_pointer;
        size_t n = toolkit.api_search_buffer.generators.size();
		if (index < n)
		{
			snprintf(toolkit.steps_char_buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (toolkit.api_search_buffer.generators[index]->get_identifier()).c_str());
			return toolkit.steps_char_buffer;
		}
        else
            return toolkit.steps_char_buffer;
    }

    if(DEVICE_TYPE=="WT GENERATOR")
    {
        size_t index = toolkit.api_search_buffer.wt_generator_pointer;
        size_t n = toolkit.api_search_buffer.wt_generators.size();
        if(index<n)
		{
			snprintf(toolkit.steps_char_buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (toolkit.api_search_buffer.wt_generators[index]->get_identifier()).c_str());
			return toolkit.steps_char_buffer;
		}
        else
            return toolkit.steps_char_buffer;
    }

    if(DEVICE_TYPE=="PV UNIT")
    {
        size_t index = toolkit.api_search_buffer.pv_unit_pointer;
        size_t n = toolkit.api_search_buffer.pv_units.size();
        if(index<n)
		{
			snprintf(toolkit.steps_char_buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (toolkit.api_search_buffer.pv_units[index]->get_identifier()).c_str());
			return toolkit.steps_char_buffer;
		}
        else
            return toolkit.steps_char_buffer;
    }

    if(DEVICE_TYPE=="LOAD")
    {
        size_t index = toolkit.api_search_buffer.load_pointer;
        size_t n = toolkit.api_search_buffer.loads.size();
        if(index<n)
		{
			snprintf(toolkit.steps_char_buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (toolkit.api_search_buffer.loads[index]->get_identifier()).c_str());
			return toolkit.steps_char_buffer;
		}
        else
            return toolkit.steps_char_buffer;
    }

    if(DEVICE_TYPE=="FIXED SHUNT")
    {
        size_t index = toolkit.api_search_buffer.fixed_shunt_pointer;
        size_t n = toolkit.api_search_buffer.fixed_shunts.size();
        if(index<n)
		{
			snprintf(toolkit.steps_char_buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (toolkit.api_search_buffer.fixed_shunts[index]->get_identifier()).c_str());
			return toolkit.steps_char_buffer;
		}
        else
            return toolkit.steps_char_buffer;
    }

    if(DEVICE_TYPE=="LINE")
    {
        size_t index = toolkit.api_search_buffer.line_pointer;
        size_t n = toolkit.api_search_buffer.lines.size();
        if(index<n)
		{
			snprintf(toolkit.steps_char_buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (toolkit.api_search_buffer.lines[index]->get_identifier()).c_str());
			return toolkit.steps_char_buffer;
		}
        else
            return toolkit.steps_char_buffer;
    }

    if(DEVICE_TYPE=="TRANSFORMER")
    {
        size_t index = toolkit.api_search_buffer.transformer_pointer;
        size_t n = toolkit.api_search_buffer.transformers.size();
        if(index<n)
		{
			snprintf(toolkit.steps_char_buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (toolkit.api_search_buffer.transformers[index]->get_identifier()).c_str());
			return toolkit.steps_char_buffer;
		}
        else
            return toolkit.steps_char_buffer;
    }

    if(DEVICE_TYPE=="HVDC")
    {
        size_t index = toolkit.api_search_buffer.hvdc_pointer;
        size_t n = toolkit.api_search_buffer.hvdcs.size();
        if(index<n)
		{
			snprintf(toolkit.steps_char_buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (toolkit.api_search_buffer.hvdcs[index]->get_identifier()).c_str());
			return toolkit.steps_char_buffer;
		}
        else
            return toolkit.steps_char_buffer;
    }

    if(DEVICE_TYPE=="EQUIVALENT DEVICE")
    {
        size_t index = toolkit.api_search_buffer.equivalent_device_pointer;
        size_t n = toolkit.api_search_buffer.equivalent_devices.size();
        if(index<n)
		{
			snprintf(toolkit.steps_char_buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (toolkit.api_search_buffer.equivalent_devices[index]->get_identifier()).c_str());
			return toolkit.steps_char_buffer;
		}
        else
            return toolkit.steps_char_buffer;
    }

    if(DEVICE_TYPE=="ENERGY STORAGE")
    {
        size_t index = toolkit.api_search_buffer.energy_storage_pointer;
        size_t n = toolkit.api_search_buffer.energy_storages.size();
        if(index<n)
		{
			snprintf(toolkit.steps_char_buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (toolkit.api_search_buffer.energy_storages[index]->get_identifier()).c_str());
			return toolkit.steps_char_buffer;
		}
        else
            return toolkit.steps_char_buffer;
    }

    show_parameter_not_supported_with_api(DEVICE_TYPE, __FUNCTION__);
    return toolkit.steps_char_buffer;
}

void api_goto_next_device(const char* device_type, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
    string DEVICE_TYPE = string2upper(device_type);
    if(DEVICE_TYPE=="GENERATOR")
    {
        size_t index = toolkit.api_search_buffer.generator_pointer;
        size_t n = toolkit.api_search_buffer.generators.size();
        if(index<n)
            ++toolkit.api_search_buffer.generator_pointer;
        return;
    }

    if(DEVICE_TYPE=="WT GENERATOR")
    {
        size_t index = toolkit.api_search_buffer.wt_generator_pointer;
        size_t n = toolkit.api_search_buffer.wt_generators.size();
        if(index<n)
            ++toolkit.api_search_buffer.wt_generator_pointer;
        return;
    }

    if(DEVICE_TYPE=="PV UNIT")
    {
        size_t index = toolkit.api_search_buffer.pv_unit_pointer;
        size_t n = toolkit.api_search_buffer.pv_units.size();
        if(index<n)
            ++toolkit.api_search_buffer.pv_unit_pointer;
        return;
    }

    if(DEVICE_TYPE=="LOAD")
    {
        size_t index = toolkit.api_search_buffer.load_pointer;
        size_t n = toolkit.api_search_buffer.loads.size();
        if(index<n)
            ++toolkit.api_search_buffer.load_pointer;
        return;
    }

    if(DEVICE_TYPE=="FIXED SHUNT")
    {
        size_t index = toolkit.api_search_buffer.fixed_shunt_pointer;
        size_t n = toolkit.api_search_buffer.fixed_shunts.size();
        if(index<n)
            ++toolkit.api_search_buffer.fixed_shunt_pointer;
        return;
    }

    if(DEVICE_TYPE=="LINE")
    {
        size_t index = toolkit.api_search_buffer.line_pointer;
        size_t n = toolkit.api_search_buffer.lines.size();
        if(index<n)
            ++toolkit.api_search_buffer.line_pointer;
        return;
    }

    if(DEVICE_TYPE=="TRANSFORMER")
    {
        size_t index = toolkit.api_search_buffer.transformer_pointer;
        size_t n = toolkit.api_search_buffer.transformers.size();
        if(index<n)
            ++toolkit.api_search_buffer.transformer_pointer;
        return;
    }

    if(DEVICE_TYPE=="HVDC")
    {
        size_t index = toolkit.api_search_buffer.hvdc_pointer;
        size_t n = toolkit.api_search_buffer.hvdcs.size();
        if(index<n)
            ++toolkit.api_search_buffer.hvdc_pointer;
        return;
    }

    if(DEVICE_TYPE=="EQUIVALENT DEVICE")
    {
        size_t index = toolkit.api_search_buffer.equivalent_device_pointer;
        size_t n = toolkit.api_search_buffer.equivalent_devices.size();
        if(index<n)
            ++toolkit.api_search_buffer.equivalent_device_pointer;
        return;
    }

    if(DEVICE_TYPE=="ENERGY STORAGE")
    {
        size_t index = toolkit.api_search_buffer.energy_storage_pointer;
        size_t n = toolkit.api_search_buffer.energy_storages.size();
        if(index<n)
            ++toolkit.api_search_buffer.energy_storage_pointer;
        return;
    }

    show_parameter_not_supported_with_api(DEVICE_TYPE, __FUNCTION__);
}

void api_initialize_area_search(size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    toolkit.api_search_buffer.areas = psdb.get_all_areas();
    toolkit.api_search_buffer.area_pointer = 0;
}

size_t api_get_current_area_number(size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
    size_t index = toolkit.api_search_buffer.area_pointer;
    size_t n = toolkit.api_search_buffer.areas.size();
    if(index<n)
        return toolkit.api_search_buffer.areas[index]->get_area_number();
    else
        return 0;
}

void api_goto_next_area(size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
    size_t index = toolkit.api_search_buffer.area_pointer;
    size_t n = toolkit.api_search_buffer.areas.size();
    if(index<n)
		++toolkit.api_search_buffer.area_pointer;
}

void api_initialize_zone_search(size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    toolkit.api_search_buffer.zones = psdb.get_all_zones();
    toolkit.api_search_buffer.zone_pointer = 0;
}

size_t api_get_current_zone_number(size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
    size_t index = toolkit.api_search_buffer.zone_pointer;
    size_t n = toolkit.api_search_buffer.zones.size();
    if(index<n)
        return toolkit.api_search_buffer.zones[index]->get_zone_number();
    else
        return 0;
}

void api_goto_next_zone(size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
    size_t index = toolkit.api_search_buffer.zone_pointer;
    size_t n = toolkit.api_search_buffer.zones.size();
    if(index<n)
		++toolkit.api_search_buffer.zone_pointer;
}

void api_initialize_owner_search(size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    toolkit.api_search_buffer.owners = psdb.get_all_owners();
    toolkit.api_search_buffer.owner_pointer = 0;
}

size_t api_get_current_owner_number(size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
    size_t index = toolkit.api_search_buffer.owner_pointer;
    size_t n = toolkit.api_search_buffer.owners.size();
    if(index<n)
        return toolkit.api_search_buffer.owners[index]->get_owner_number();
    else
        return 0;
}

void api_goto_next_owner(size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
    size_t index = toolkit.api_search_buffer.owner_pointer;
    size_t n = toolkit.api_search_buffer.owners.size();
    if(index<n)
		++toolkit.api_search_buffer.owner_pointer;
}

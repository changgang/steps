#include "header/apis/steps_api.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/data_imexporter/psse_imexporter.h"
#include "header/data_imexporter/bpa_imexporter.h"

void api_add_bus(unsigned int bus_number, char* bus_name, double base_voltage_in_kV, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    if(not psdb.is_bus_exist(bus_number))
    {
        BUS newbus(toolkit);
        newbus.set_bus_number(bus_number);
        newbus.set_bus_name(bus_name);
        newbus.set_base_voltage_in_kV(base_voltage_in_kV);
        psdb.append_bus(newbus);
    }
}

void api_add_generator(unsigned int bus_number, char* identifier, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    DEVICE_ID did = get_generator_device_id(bus_number, identifier);

    if(not psdb.is_generator_exist(did))
    {
        GENERATOR newgen(toolkit);
        newgen.set_generator_bus(bus_number);
        newgen.set_identifier(identifier);
        psdb.append_generator(newgen);
    }
}

void api_add_wt_generator(unsigned int bus_number, char* identifier, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    DEVICE_ID did = get_wt_generator_device_id(bus_number, identifier);

    if(not psdb.is_wt_generator_exist(did))
    {
        WT_GENERATOR newwt_generator(toolkit);
        newwt_generator.set_source_bus(bus_number);
        newwt_generator.set_identifier(identifier);
        psdb.append_wt_generator(newwt_generator);
    }
}


void api_add_pv_unit(unsigned int bus_number, char* identifier, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    DEVICE_ID did = get_pv_unit_device_id(bus_number, identifier);

    if(not psdb.is_pv_unit_exist(did))
    {
        PV_UNIT newpv_unit(toolkit);
        newpv_unit.set_unit_bus(bus_number);
        newpv_unit.set_identifier(identifier);
        psdb.append_pv_unit(newpv_unit);
    }
}

void api_add_load(unsigned int bus_number, char* identifier, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    DEVICE_ID did = get_load_device_id(bus_number, identifier);

    if(not psdb.is_load_exist(did))
    {
        LOAD newload(toolkit);
        newload.set_load_bus(bus_number);
        newload.set_identifier(identifier);
        psdb.append_load(newload);
    }
}

void api_add_fixed_shunt(unsigned int bus_number, char* identifier, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    DEVICE_ID did = get_fixed_shunt_device_id(bus_number, identifier);

    if(not psdb.is_fixed_shunt_exist(did))
    {
        FIXED_SHUNT newshunt(toolkit);
        newshunt.set_shunt_bus(bus_number);
        newshunt.set_identifier(identifier);
        psdb.append_fixed_shunt(newshunt);
    }
}

void api_add_line(unsigned int sending_side_bus_number, unsigned int receiving_side_bus_number, char* identifier, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    DEVICE_ID did = get_line_device_id(sending_side_bus_number, receiving_side_bus_number, identifier);

    if(not psdb.is_line_exist(did))
    {
        LINE newline(toolkit);
        newline.set_sending_side_bus(sending_side_bus_number);
        newline.set_receiving_side_bus(receiving_side_bus_number);
        newline.set_identifier(identifier);
        psdb.append_line(newline);
    }
}

void api_add_hvdc(unsigned int rectifier_bus_number, unsigned int inverter_bus_number, char* identifier, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    DEVICE_ID did = get_2t_lcc_hvdc_device_id(rectifier_bus_number, inverter_bus_number, identifier);

    if(not psdb.is_2t_lcc_hvdc_exist(did))
    {
        LCC_HVDC2T newhvdc(toolkit);
        newhvdc.set_converter_bus(RECTIFIER, rectifier_bus_number);
        newhvdc.set_converter_bus(INVERTER, inverter_bus_number);
        newhvdc.set_identifier(identifier);
        psdb.append_2t_lcc_hvdc(newhvdc);
    }
}

void api_add_vsc_hvdc(char* vsc_name, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    if(not psdb.is_vsc_hvdc_exist(vsc_name))
    {
        VSC_HVDC newvsc(toolkit);
        psdb.append_vsc_hvdc(newvsc);
    }

    ostringstream osstream;
    osstream<<"function::"<<__FUNCTION__<<" is not implementated now! ";
    toolkit.show_information_with_leading_time_stamp(osstream);

}

void api_add_transformer(unsigned int primary_side_bus_number, unsigned int secondary_side_bus_number, unsigned int tertiary_side_bus_number, char* identifier, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    DEVICE_ID did = get_transformer_device_id(primary_side_bus_number, secondary_side_bus_number, tertiary_side_bus_number, identifier);

    if(not psdb.is_transformer_exist(did))
    {
        TRANSFORMER newtrans(toolkit);
        newtrans.set_winding_bus(PRIMARY_SIDE, primary_side_bus_number);
        newtrans.set_winding_bus(SECONDARY_SIDE, secondary_side_bus_number);
        newtrans.set_winding_bus(TERTIARY_SIDE, tertiary_side_bus_number);
        newtrans.set_identifier(identifier);
        psdb.append_transformer(newtrans);
    }
}

void api_add_equivalent_device(unsigned int bus_number, char* identifier, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    DEVICE_ID did = get_equivalent_device_id(bus_number, identifier);

    if(not psdb.is_equivalent_device_exist(did))
    {
        EQUIVALENT_DEVICE newed(toolkit);
        newed.set_equivalent_device_bus(bus_number);
        newed.set_identifier(identifier);
        psdb.append_equivalent_device(newed);
    }
}

void api_add_energy_storage(unsigned int bus_number, char* identifier, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    DEVICE_ID did = get_energy_storage_device_id(bus_number, identifier);

    if(not psdb.is_energy_storage_exist(did))
    {
        ENERGY_STORAGE newes(toolkit);
        newes.set_energy_storage_bus(bus_number);
        newes.set_identifier(identifier);
        psdb.append_energy_storage(newes);
    }
}

void api_add_area(unsigned int area_number, char* area_name, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    if(not psdb.is_area_exist(area_number))
    {
        AREA newarea(toolkit);
        newarea.set_area_number(area_number);
        newarea.set_area_name(area_name);
        psdb.append_area(newarea);
    }
}

void api_add_zone(unsigned int zone_number, char* zone_name, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    if(not psdb.is_zone_exist(zone_number))
    {
        ZONE newzone(toolkit);
        newzone.set_zone_number(zone_number);
        newzone.set_zone_name(zone_name);
        psdb.append_zone(newzone);
    }
}

void api_add_owner(unsigned int owner_number, char* owner_name, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    if(not psdb.is_owner_exist(owner_number))
    {
        OWNER newowner(toolkit);
        newowner.set_owner_number(owner_number);
        newowner.set_owner_name(owner_name);
        psdb.append_owner(newowner);
    }
}

void api_change_bus_number(unsigned int original_bus_number, unsigned int new_bus_number, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    psdb.change_bus_number(original_bus_number, new_bus_number);
}

void api_change_bus_number_with_file(char* file, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    psdb.change_bus_number_with_file(file);
}

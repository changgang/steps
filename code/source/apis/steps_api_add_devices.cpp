#include "header/apis/steps_api.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/data_imexporter/psse_imexporter.h"
#include "header/data_imexporter/bpa_imexporter.h"

void api_add_bus(size_t bus_number, char* bus_name, double base_voltage_in_kV)
{
    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();

    if(psdb!=NULL)
    {
        if(not psdb->is_bus_exist(bus_number))
        {
            BUS newbus(psdb);
            newbus.set_bus_number(bus_number);
            newbus.set_bus_name(bus_name);
            newbus.set_base_voltage_in_kV(base_voltage_in_kV);
            psdb->append_bus(newbus);
        }
        else
            return;
    }
}

void api_add_generator(size_t bus_number, char* identifier)
{
    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();

    if(psdb!=NULL)
    {
        DEVICE_ID did;
        did.set_device_type("GENERATOR");
        TERMINAL terminal;
        terminal.append_bus(bus_number);
        did.set_device_terminal(terminal);
        did.set_device_identifier(identifier);

        if(not psdb->is_generator_exist(did))
        {
            GENERATOR newgen(psdb);
            newgen.set_generator_bus(bus_number);
            newgen.set_identifier(identifier);
            psdb->append_generator(newgen);
        }
        else
            return;
    }
}

void api_add_pe_source(size_t bus_number, char* identifier)
{
    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();

    if(psdb!=NULL)
    {
        DEVICE_ID did;
        did.set_device_type("PE SOURCE");
        TERMINAL terminal;
        terminal.append_bus(bus_number);
        did.set_device_terminal(terminal);
        did.set_device_identifier(identifier);

        if(not psdb->is_pe_source_exist(did))
        {
            PE_SOURCE newpesource(psdb);
            newpesource.set_source_bus(bus_number);
            newpesource.set_identifier(identifier);
            psdb->append_pe_source(newpesource);
        }
        else
            return;
    }
}

void api_add_load(size_t bus_number, char* identifier)
{
    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();

    if(psdb!=NULL)
    {
        DEVICE_ID did;
        did.set_device_type("LOAD");
        TERMINAL terminal;
        terminal.append_bus(bus_number);
        did.set_device_terminal(terminal);
        did.set_device_identifier(identifier);

        if(not psdb->is_load_exist(did))
        {
            LOAD newload(psdb);
            newload.set_load_bus(bus_number);
            newload.set_identifier(identifier);
            psdb->append_load(newload);
        }
        else
            return;
    }
}

void api_add_fixed_shunt(size_t bus_number, char* identifier)
{
    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();

    if(psdb!=NULL)
    {
        DEVICE_ID did;
        did.set_device_type("FIXED SHUNT");
        TERMINAL terminal;
        terminal.append_bus(bus_number);
        did.set_device_terminal(terminal);
        did.set_device_identifier(identifier);

        if(not psdb->is_fixed_shunt_exist(did))
        {
            FIXED_SHUNT newshunt(psdb);
            newshunt.set_shunt_bus(bus_number);
            newshunt.set_identifier(identifier);
            psdb->append_fixed_shunt(newshunt);
        }
        else
            return;
    }
}

void api_add_line(size_t sending_side_bus_number, size_t receiving_side_bus_number, char* identifier)
{
    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();

    if(psdb!=NULL)
    {
        DEVICE_ID did;
        did.set_device_type("LINE");
        TERMINAL terminal;
        terminal.append_bus(sending_side_bus_number);
        terminal.append_bus(receiving_side_bus_number);
        did.set_device_terminal(terminal);
        did.set_device_identifier(identifier);

        if(not psdb->is_line_exist(did))
        {
            LINE newline(psdb);
            newline.set_sending_side_bus(sending_side_bus_number);
            newline.set_receiving_side_bus(receiving_side_bus_number);
            newline.set_identifier(identifier);
            psdb->append_line(newline);
        }
        else
            return;
    }
}

void api_add_hvdc(size_t rectifier_bus_number, size_t inverter_bus_number, char* identifier)
{
    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();

    if(psdb!=NULL)
    {
        DEVICE_ID did;
        did.set_device_type("HVDC");
        TERMINAL terminal;
        terminal.append_bus(rectifier_bus_number);
        terminal.append_bus(inverter_bus_number);
        did.set_device_terminal(terminal);
        did.set_device_identifier(identifier);

        if(not psdb->is_hvdc_exist(did))
        {
            HVDC newhvdc(psdb);
            newhvdc.set_converter_bus(RECTIFIER, rectifier_bus_number);
            newhvdc.set_converter_bus(INVERTER, inverter_bus_number);;
            newhvdc.set_identifier(identifier);
            psdb->append_hvdc(newhvdc);
        }
        else
            return;
    }
}

void api_add_transformer(size_t primary_side_bus_number, size_t secondary_side_bus_number, size_t tertiary_side_bus_number, char* identifier)
{
    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();

    if(psdb!=NULL)
    {
        DEVICE_ID did;
        did.set_device_type("TRANSFORMER");
        TERMINAL terminal;
        terminal.append_bus(primary_side_bus_number);
        terminal.append_bus(secondary_side_bus_number);
        terminal.append_bus(tertiary_side_bus_number);
        did.set_device_terminal(terminal);
        did.set_device_identifier(identifier);

        if(not psdb->is_transformer_exist(did))
        {
            TRANSFORMER newtrans(psdb);
            newtrans.set_winding_bus(PRIMARY_SIDE, primary_side_bus_number);
            newtrans.set_winding_bus(SECONDARY_SIDE, secondary_side_bus_number);
            newtrans.set_winding_bus(TERTIARY_SIDE, tertiary_side_bus_number);
            newtrans.set_identifier(identifier);
            psdb->append_transformer(newtrans);
        }
        else
            return;
    }
}

void api_add_equivalent_device(size_t bus_number, char* identifier)
{
    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();

    if(psdb!=NULL)
    {
        DEVICE_ID did;
        did.set_device_type("EQUIVALENT DEVICE");
        TERMINAL terminal;
        terminal.append_bus(bus_number);
        did.set_device_terminal(terminal);
        did.set_device_identifier(identifier);

        if(not psdb->is_equivalent_device_exist(did))
        {
            EQUIVALENT_DEVICE newed(psdb);
            newed.set_equivalent_device_bus(bus_number);
            newed.set_identifier(identifier);
            psdb->append_equivalent_device(newed);
        }
        else
            return;
    }
}

void api_add_area(size_t area_number, char* area_name)
{
    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();

    if(psdb!=NULL)
    {
        if(not psdb->is_area_exist(area_number))
        {
            AREA newarea(psdb);
            newarea.set_area_number(area_number);
            newarea.set_area_name(area_name);
            psdb->append_area(newarea);
        }
        else
            return;
    }
}

void api_add_zone(size_t zone_number, char* zone_name)
{
    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();

    if(psdb!=NULL)
    {
        if(not psdb->is_zone_exist(zone_number))
        {
            ZONE newzone(psdb);
            newzone.set_zone_number(zone_number);
            newzone.set_zone_name(zone_name);
            psdb->append_zone(newzone);
        }
        else
            return;
    }
}

void api_add_owner(size_t owner_number, char* owner_name)
{
    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();

    if(psdb!=NULL)
    {
        if(not psdb->is_owner_exist(owner_number))
        {
            OWNER newowner(psdb);
            newowner.set_owner_number(owner_number);
            newowner.set_owner_name(owner_name);
            psdb->append_owner(newowner);
        }
        else
            return;
    }
}

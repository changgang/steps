#include "header/power_system_database.h"
#include "header/basic/utility.h"
#include "header/toolkit/dynamic_simulator/dynamic_simulator.h"
#include "thirdparty/rapidjson/document.h"
#include "thirdparty/rapidjson/prettywriter.h"
#include "thirdparty/rapidjson/stringbuffer.h"
#include <istream>
#include <iostream>
#include <fstream>

using namespace std;
using namespace rapidjson;

POWER_SYSTEM_DATABASE::POWER_SYSTEM_DATABASE()
{
    set_system_name("");
    set_system_base_power_in_MVA(100.0);

    set_allowed_max_bus_number(1000);

    set_database_capacity();

    set_zero_impedance_threshold_in_pu(0.0001);

    clear_database();

    update_in_service_bus_count();
}


POWER_SYSTEM_DATABASE::~POWER_SYSTEM_DATABASE()
{
    clear_database();
}

void POWER_SYSTEM_DATABASE::set_zero_impedance_threshold_in_pu(double z)
{
    zero_impedance_threshold = z;
}

double POWER_SYSTEM_DATABASE::get_zero_impedance_threshold_in_pu() const
{
    return zero_impedance_threshold;
}

void POWER_SYSTEM_DATABASE::set_database_capacity()
{
    size_t bus_capacity = 1000;
    size_t generator_capacity = size_t(round(bus_capacity*0.5));
    size_t wt_generator_capacity = size_t(round(bus_capacity*0.5));
    size_t pv_unit_capacity = size_t(round(bus_capacity*0.5));
    size_t load_capacity = size_t(round(bus_capacity*0.5));
    size_t fixedshunt_capacity = size_t(round(bus_capacity*0.5));
    size_t line_capacity = size_t(round(bus_capacity*3.0));
    size_t transformer_capacity = bus_capacity;
    size_t hvdc_capacity = size_t(round(bus_capacity*0.1));
    size_t equivalentdevice_capacity = size_t(round(bus_capacity*0.1));
    size_t energy_storage_capacity = size_t(round(bus_capacity*0.1));
    size_t area_capacity = size_t(round(bus_capacity*0.1));
    size_t zone_capacity = size_t(round(bus_capacity*0.1));
    size_t owner_capacity = size_t(round(bus_capacity*0.1));

    ifstream instream("steps_config.json");
    if(instream.is_open())
    {
        ostringstream tmp;
        tmp<<instream.rdbuf();
        string str = tmp.str();

        Document document;
        document.Parse(str.c_str());


        if(document.HasMember("bus") and document["bus"].IsInt())
            bus_capacity = size_t(document["bus"].GetInt());

        if(document.HasMember("generator") and document["generator"].IsInt())
            generator_capacity = size_t(document["generator"].GetInt());

        if(document.HasMember("wtGenerator") and document["wtGenerator"].IsInt())
            wt_generator_capacity = size_t(document["wtGenerator"].GetInt());

        if(document.HasMember("pvUnit") and document["pvUnit"].IsInt())
            pv_unit_capacity = size_t(document["pvUnit"].GetInt());

        if(document.HasMember("load") and document["load"].IsInt())
            load_capacity = size_t(document["load"].GetInt());

        if(document.HasMember("fixedShunt") and document["fixedShunt"].IsInt())
            fixedshunt_capacity = size_t(document["fixedShunt"].GetInt());

        if(document.HasMember("line") and document["line"].IsInt())
            line_capacity = size_t(document["line"].GetInt());

        if(document.HasMember("transformer") and document["transformer"].IsInt())
            transformer_capacity = size_t(document["transformer"].GetInt());

        if(document.HasMember("hvdc") and document["hvdc"].IsInt())
            hvdc_capacity = size_t(document["hvdc"].GetInt());

        if(document.HasMember("equivalentDevice") and document["equivalentDevice"].IsInt())
            equivalentdevice_capacity = size_t(document["equivalentDevice"].GetInt());

        if(document.HasMember("energyStorage") and document["energyStorage"].IsInt())
            energy_storage_capacity = size_t(document["energyStorage"].GetInt());

        if(document.HasMember("area") and document["area"].IsInt())
            area_capacity = size_t(document["area"].GetInt());

        if(document.HasMember("zone") and document["zone"].IsInt())
            zone_capacity = size_t(document["zone"].GetInt());

        if(document.HasMember("owner") and document["owner"].IsInt())
            owner_capacity = size_t(document["owner"].GetInt());
    }
    else
    {
        ostringstream osstream;
        osstream<<"No valid steps_config.json found. Use default power system database capacity.";
        show_information_with_leading_time_stamp(osstream);
    }

    set_bus_capacity(bus_capacity);
    set_generator_capacity(generator_capacity);
    set_wt_generator_capacity(wt_generator_capacity);
    set_pv_unit_capacity(pv_unit_capacity);
    set_load_capacity(load_capacity);
    set_fixed_shunt_capacity(fixedshunt_capacity);
    set_line_capacity(line_capacity);
    set_transformer_capacity(transformer_capacity);
    set_hvdc_capacity(hvdc_capacity);
    set_equivalent_device_capacity(equivalentdevice_capacity);
    set_energy_storage_capacity(energy_storage_capacity);
    set_area_capacity(area_capacity);
    set_zone_capacity(zone_capacity);
    set_owner_capacity(owner_capacity);
}

size_t POWER_SYSTEM_DATABASE::get_bus_capacity() const
{
    return Bus.capacity();
}

size_t POWER_SYSTEM_DATABASE::get_generator_capacity() const
{
    return Generator.capacity();
}

size_t POWER_SYSTEM_DATABASE::get_wt_generator_capacity() const
{
    return WT_Generator.capacity();
}

size_t POWER_SYSTEM_DATABASE::get_pv_unit_capacity() const
{
    return PV_Unit.capacity();
}

size_t POWER_SYSTEM_DATABASE::get_load_capacity() const
{
    return Load.capacity();
}

size_t POWER_SYSTEM_DATABASE::get_fixed_shunt_capacity() const
{
    return Fixed_shunt.capacity();
}

size_t POWER_SYSTEM_DATABASE::get_line_capacity() const
{
    return Line.capacity();
}

size_t POWER_SYSTEM_DATABASE::get_transformer_capacity() const
{
    return Transformer.capacity();
}

size_t POWER_SYSTEM_DATABASE::get_hvdc_capacity() const
{
    return Hvdc.capacity();
}

size_t POWER_SYSTEM_DATABASE::get_equivalent_device_capacity() const
{
    return Equivalent_device.capacity();
}

size_t POWER_SYSTEM_DATABASE::get_energy_storage_capacity() const
{
    return Energy_storage.capacity();
}

size_t POWER_SYSTEM_DATABASE::get_area_capacity() const
{
    return Area.capacity();
}

size_t POWER_SYSTEM_DATABASE::get_zone_capacity() const
{
    return Zone.capacity();
}

size_t POWER_SYSTEM_DATABASE::get_owner_capacity() const
{
    return Owner.capacity();
}

void POWER_SYSTEM_DATABASE::set_bus_capacity(size_t n)
{
    Bus.reserve(n);
}

void POWER_SYSTEM_DATABASE::set_generator_capacity(size_t n)
{
    Generator.reserve(n);
}

void POWER_SYSTEM_DATABASE::set_wt_generator_capacity(size_t n)
{
    WT_Generator.reserve(n);
}

void POWER_SYSTEM_DATABASE::set_pv_unit_capacity(size_t n)
{
    PV_Unit.reserve(n);
}

void POWER_SYSTEM_DATABASE::set_load_capacity(size_t n)
{
    Load.reserve(n);
}

void POWER_SYSTEM_DATABASE::set_fixed_shunt_capacity(size_t n)
{
    Fixed_shunt.reserve(n);
}

void POWER_SYSTEM_DATABASE::set_line_capacity(size_t n)
{
    Line.reserve(n);
}

void POWER_SYSTEM_DATABASE::set_transformer_capacity(size_t n)
{
    Transformer.reserve(n);
}

void POWER_SYSTEM_DATABASE::set_hvdc_capacity(size_t n)
{
    Hvdc.reserve(n);
}

void POWER_SYSTEM_DATABASE::set_equivalent_device_capacity(size_t n)
{
    Equivalent_device.reserve(n);
}

void POWER_SYSTEM_DATABASE::set_energy_storage_capacity(size_t n)
{
    Energy_storage.reserve(n);
}

void POWER_SYSTEM_DATABASE::set_area_capacity(size_t n)
{
    Area.reserve(n);
}

void POWER_SYSTEM_DATABASE::set_zone_capacity(size_t n)
{
    Zone.reserve(n);
}

void POWER_SYSTEM_DATABASE::set_owner_capacity(size_t n)
{
    Owner.reserve(n);
}

void POWER_SYSTEM_DATABASE::clear_database()
{
    set_case_information("");
    set_case_additional_information("");
    clear_all_buses();
    clear_all_sources();
    clear_all_loads();
    clear_all_lines();
    clear_all_transformers();
    clear_all_fixed_shunts();
    clear_all_hvdcs();
    clear_all_equivalent_devices();
    clear_all_areas();
    clear_all_zones();
    clear_all_owners();
    update_in_service_bus_count();
}

void POWER_SYSTEM_DATABASE::set_system_name(const string& name)
{
    this->system_name = name;
}

string POWER_SYSTEM_DATABASE::get_system_name() const
{
    return this->system_name;
}

void POWER_SYSTEM_DATABASE::set_allowed_max_bus_number(size_t max_bus_num)
{
    bus_index.set_max_bus_number(max_bus_num);
}

size_t POWER_SYSTEM_DATABASE::get_allowed_max_bus_number() const
{
    return bus_index.get_max_bus_number();
}

bool POWER_SYSTEM_DATABASE::is_bus_in_allowed_range(size_t bus) const
{
    return (bus>0 and bus<=this->get_allowed_max_bus_number());
}

void POWER_SYSTEM_DATABASE::set_system_base_power_in_MVA(const double s)
{
    if(s>0.0)
        system_base_power_in_MVA = s;
    else
    {
        ostringstream osstream;
        osstream<<"Warning. Non positive power ("<<s<<" MVA) is not allowed for setting system base power."<<endl
          <<"Nothing will be changed.";
        show_information_with_leading_time_stamp(osstream);
    }
}

double POWER_SYSTEM_DATABASE::get_system_base_power_in_MVA() const
{
    return system_base_power_in_MVA;
}

void POWER_SYSTEM_DATABASE::set_case_information(const string& title)
{
    case_information = title;
}

void POWER_SYSTEM_DATABASE::set_case_additional_information(const string& title)
{
    case_additional_information = title;
}

string POWER_SYSTEM_DATABASE::get_case_information() const
{
    return case_information;
}

string POWER_SYSTEM_DATABASE::get_case_additional_information() const
{
    return case_additional_information;
}

void POWER_SYSTEM_DATABASE::append_bus(const BUS& bus)
{
    ostringstream osstream;

    if(not bus.is_valid())
    {
        osstream<<"Warning. Failed to append invalid bus "<<bus.get_bus_number()<<" due to either 0 bus number of 0 base voltage.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    size_t bus_number = bus.get_bus_number();

    if(not this->is_bus_in_allowed_range(bus_number))
    {
        osstream<<"Warning. Bus number "<<bus_number<<" is not in the allowed range [1, "<<get_allowed_max_bus_number()<<"] when appending "<<bus.get_device_name()<<" to power system database '"<<get_system_name()<<"'."<<endl
          <<"Bus will not be appended into the database.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(this->is_bus_exist(bus_number))
    {
        osstream<<"Warning. Bus "<<bus_number<<" already exists in power system database '"<<get_system_name()<<"': Bus."<<endl
          <<"Duplicate copy is not allowed.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(Bus.capacity()==Bus.size())
    {
        osstream<<"Warning. Capacity limit ("<<Bus.capacity()<<") reached when appending bus to power system database "<<get_system_name()<<"."<<endl
          <<"Increase capacity by modified steps_config.json.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }
    size_t bus_count = get_bus_count();
    Bus.push_back(bus);
    bus_index.set_bus_with_index(bus_number, bus_count);

    update_in_service_bus_count();
}

void POWER_SYSTEM_DATABASE::append_generator(const GENERATOR& generator)
{
    ostringstream osstream;

    //if(generator.get_default_power_system_database()==NULL)
    //    generator.set_power_system_database(this);

    if(not generator.is_valid())
    {
        osstream<<"Warning. Failed to append invalid generator to power system database '"<<get_system_name()<<"'.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    size_t generator_bus = generator.get_generator_bus();

    if(not this->is_bus_in_allowed_range(generator_bus))
    {
        osstream<<"Warning. Bus number "<<generator_bus<<" is not in the allowed range [1, "<<get_allowed_max_bus_number()<<"] when appending "<<generator.get_device_name()<<" to power system database '"<<get_system_name()<<"'."<<endl
          <<"Generator will not be appended into the database.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    string identifier = generator.get_identifier();
    DEVICE_ID device_id;
    device_id.set_device_type("GENERATOR");
    TERMINAL terminal;
    terminal.append_bus(generator_bus);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier(identifier);

    if(this->is_generator_exist(device_id))
    {
        osstream<<"Warning. "<<generator.get_device_name()<<" already exists in power system database '"<<get_system_name()<<"': Generator."<<endl
          <<"Duplicate copy is not allowed.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(Generator.capacity()==Generator.size())
    {
        osstream<<"Warning. Capacity limit ("<<Generator.capacity()<<") reached when appending generator to power system database "<<get_system_name()<<"."<<endl
          <<"Increase capacity by modified steps_config.json.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    size_t generator_count = get_generator_count();
    Generator.push_back(generator);
    generator_index.set_device_index(device_id, generator_count);
}

void POWER_SYSTEM_DATABASE::append_wt_generator(const WT_GENERATOR& wt_generator)
{
    ostringstream osstream;

    //if(wt_generator.get_default_power_system_database()==NULL)
    //    wt_generator.set_power_system_database(this);

    if(not wt_generator.is_valid())
    {
        osstream<<"Warning. Failed to append invalid wt generator to power system database '"<<get_system_name()<<"'.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }


    size_t wt_generator_bus = wt_generator.get_source_bus();

    if(not this->is_bus_in_allowed_range(wt_generator_bus))
    {
        osstream<<"Warning. Bus "<<wt_generator_bus<<" is not in the allowed range [1, "<<get_allowed_max_bus_number()<<"] when appending "<<wt_generator.get_device_name()<<" to power system database '"<<get_system_name()<<"'."<<endl
          <<"WT generator will not be appended into the database.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    string identifier = wt_generator.get_identifier();
    DEVICE_ID device_id;
    device_id.set_device_type("WT GENERATOR");
    TERMINAL terminal;
    terminal.append_bus(wt_generator_bus);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier(identifier);

    if(this->is_wt_generator_exist(device_id))
    {
        osstream<<"Warning. "<<wt_generator.get_device_name()<<" already exists in power system database '"<<get_system_name()<<"': WT_Generator."<<endl
          <<"Duplicate copy is not allowed.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(WT_Generator.capacity()==WT_Generator.size())
    {
        osstream<<"Warning. Capacity limit ("<<WT_Generator.capacity()<<") reached when appending WT generator to power system database "<<get_system_name()<<"."<<endl
          <<"Increase capacity by modified steps_config.json.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    size_t wt_generator_count = get_wt_generator_count();
    WT_Generator.push_back(wt_generator);
    wt_generator_index.set_device_index(device_id, wt_generator_count);
}

void POWER_SYSTEM_DATABASE::append_pv_unit(const PV_UNIT& pv_unit)
{
    ostringstream osstream;

    //if(pv_unit.get_default_power_system_database()==NULL)
    //    pv_unit.set_power_system_database(this);

    if(not pv_unit.is_valid())
    {
        osstream<<"Warning. Failed to append invalid pv unit to power system database '"<<get_system_name()<<"'.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }


    size_t pv_unit_bus = pv_unit.get_unit_bus();

    if(not this->is_bus_in_allowed_range(pv_unit_bus))
    {
        osstream<<"Warning. Bus "<<pv_unit_bus<<" is not in the allowed range [1, "<<get_allowed_max_bus_number()<<"] when appending "<<pv_unit.get_device_name()<<" to power system database '"<<get_system_name()<<"'."<<endl
          <<"PV unit will not be appended into the database.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    string identifier = pv_unit.get_identifier();
    DEVICE_ID device_id;
    device_id.set_device_type("PV UNIT");
    TERMINAL terminal;
    terminal.append_bus(pv_unit_bus);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier(identifier);

    if(this->is_pv_unit_exist(device_id))
    {
        osstream<<"Warning. "<<pv_unit.get_device_name()<<" already exists in power system database '"<<get_system_name()<<"': PV_Unit."<<endl
          <<"Duplicate copy is not allowed.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(PV_Unit.capacity()==PV_Unit.size())
    {
        osstream<<"Warning. Capacity limit ("<<PV_Unit.capacity()<<") reached when appending PV Unit to power system database "<<get_system_name()<<"."<<endl
          <<"Increase capacity by modified steps_config.json.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    size_t pv_unit_count = get_pv_unit_count();
    PV_Unit.push_back(pv_unit);
    pv_unit_index.set_device_index(device_id, pv_unit_count);
}

void POWER_SYSTEM_DATABASE::append_energy_storage(const ENERGY_STORAGE& estorage)
{
    ostringstream osstream;

    //if(estorage.get_default_power_system_database()==NULL)
    //    estorage.set_power_system_database(this);

    if(not estorage.is_valid())
    {
        osstream<<"Warning. Failed to append invalid energy storage to power system database '"<<get_system_name()<<"'.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    size_t bus = estorage.get_energy_storage_bus();

    if(not this->is_bus_in_allowed_range(bus))
    {
        osstream<<"Warning. Bus "<<bus<<" is not in the allowed range [1, "<<get_allowed_max_bus_number()<<"] when appending "<<estorage.get_device_name()<<" to power system database '"<<get_system_name()<<"'."<<endl
          <<"Energy storage will not be appended into the database.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    string identifier = estorage.get_identifier();
    DEVICE_ID device_id;
    device_id.set_device_type("ENERGY STORAGE");
    TERMINAL terminal;
    terminal.append_bus(bus);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier(identifier);

    if(this->is_energy_storage_exist(device_id))
    {
        osstream<<"Warning. "<<estorage.get_device_name()<<" already exists in power system database '"<<get_system_name()<<"': Energy_storage.\n"
          <<"Duplicate copy is not allowed.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(Energy_storage.capacity()==Energy_storage.size())
    {
        osstream<<"Warning. Capacity limit ("<<Energy_storage.capacity()<<") reached when appending energy storage to power system database '"<<get_system_name()<<"'."<<endl
          <<"Increase capacity by modified steps_config.json.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    size_t energy_storage_count = get_energy_storage_count();
    Energy_storage.push_back(estorage);
    energy_storage_index.set_device_index(device_id, energy_storage_count);
 }

void POWER_SYSTEM_DATABASE::append_load(const LOAD& load)
{
    ostringstream osstream;

    //if(load.get_default_power_system_database()==NULL)
    //    load.set_power_system_database(this);

    if(not load.is_valid())
    {
        osstream<<"Warning. Failed to append invalid load to power system database '"<<get_system_name()<<"'.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    size_t load_bus = load.get_load_bus();

    if(not this->is_bus_in_allowed_range(load_bus))
    {
        osstream<<"Warning. Bus "<<load_bus<<" is not in the allowed range [1, "<<get_allowed_max_bus_number()<<"] when appending "<<load.get_device_name()<<" to power system database '"<<get_system_name()<<"'."<<endl
          <<"Load will not be appended into the database.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    string identifier = load.get_identifier();
    DEVICE_ID device_id;
    device_id.set_device_type("LOAD");
    TERMINAL terminal;
    terminal.append_bus(load_bus);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier(identifier);

    if(is_load_exist(device_id))
    {
        osstream<<"Warning. "<<load.get_device_name()<<" already exists in power system database '"<<get_system_name()<<"': Load."<<endl
          <<"Duplicate copy is not allowed.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(Load.capacity()==Load.size())
    {
        osstream<<"Warning. Capacity limit ("<<Load.capacity()<<") reached when appending load to power system database "<<get_system_name()<<"."<<endl
          <<"Increase capacity by modified steps_config.json.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    size_t load_count = get_load_count();
    Load.push_back(load);
    load_index.set_device_index(device_id, load_count);
}

void POWER_SYSTEM_DATABASE::append_line(const LINE& line)
{
    ostringstream osstream;

    //if(line.get_default_power_system_database()==NULL)
    //    line.set_power_system_database(this);

    if(not line.is_valid())
    {
        osstream<<"Warning. Failed to append invalid line to power system database '"<<get_system_name()<<"'.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    size_t sending_side_bus = line.get_sending_side_bus();
    size_t receiving_side_bus = line.get_receiving_side_bus();

    if(not this->is_bus_in_allowed_range(sending_side_bus))
    {
        osstream<<"Warning. Bus "<<sending_side_bus<<" is not in the allowed range [1, "<<get_allowed_max_bus_number()<<"] when appending "<<line.get_device_name()<<" to power system database '"<<get_system_name()<<"'."<<endl
          <<"Line will not be appended into the database.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }
    if(not this->is_bus_in_allowed_range(receiving_side_bus))
    {
        osstream<<"Warning. Bus "<<receiving_side_bus<<" is not in the allowed range [1, "<<get_allowed_max_bus_number()<<"] when appending "<<line.get_device_name()<<" to power system database '"<<get_system_name()<<"'."<<endl
          <<"Line will not be appended into the database.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    string identifier = line.get_identifier();
    DEVICE_ID device_id;
    device_id.set_device_type("LINE");
    TERMINAL terminal;
    terminal.append_bus(sending_side_bus);
    terminal.append_bus(receiving_side_bus);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier(identifier);

    if(this->is_line_exist(device_id))
    {
        osstream<<"Warning. "<<line.get_device_name()<<" already exists in power system database '"<<get_system_name()<<"': Line."<<endl
          <<"Duplicate copy is not allowed.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(Line.capacity()==Line.size())
    {
        osstream<<"Warning. Capacity limit ("<<Line.capacity()<<") reached when appending line to power system database '"<<get_system_name()<<"'."<<endl
          <<"Increase capacity by modified steps_config.json.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    size_t line_count = get_line_count();
    Line.push_back(line);
    line_index.set_device_index(device_id, line_count);
}

void POWER_SYSTEM_DATABASE::append_transformer(const TRANSFORMER& transformer)
{
    ostringstream osstream;

    //if(transformer.get_default_power_system_database()==NULL)
    //    transformer.set_power_system_database(this);

    if(not transformer.is_valid())
    {
        osstream<<"Warning. Failed to append invalid transformer to power system database '"<<get_system_name()<<"'.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    size_t primary_winding_bus = transformer.get_winding_bus(PRIMARY_SIDE);
    size_t secondary_winding_bus = transformer.get_winding_bus(SECONDARY_SIDE);
    size_t tertiary_winding_bus = transformer.get_winding_bus(TERTIARY_SIDE);

    if(not is_bus_in_allowed_range(primary_winding_bus))
    {
        osstream<<"Warning. Bus "<<primary_winding_bus<<" is not in the allowed range [1, "<<get_allowed_max_bus_number()<<"] when appending "<<transformer.get_device_name()<<" to power system database '"<<get_system_name()<<"'."<<endl
          <<"Transformer will not be appended into the database.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }
    if(not is_bus_in_allowed_range(secondary_winding_bus))
    {
        osstream<<"Warning. Bus "<<secondary_winding_bus<<" is not in the allowed range [1, "<<get_allowed_max_bus_number()<<"] when appending "<<transformer.get_device_name()<<" to power system database '"<<get_system_name()<<"'."<<endl
          <<"Transformer will not be appended into the database.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }
    if(not(is_bus_in_allowed_range(tertiary_winding_bus) or tertiary_winding_bus==0))
    {
        osstream<<"Warning. Bus "<<tertiary_winding_bus<<" is not in the allowed range [1, "<<get_allowed_max_bus_number()<<"] when appending "<<transformer.get_device_name()<<" to power system database '"<<get_system_name()<<"'."<<endl
          <<"Transformer will not be appended into the database.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    string identifier = transformer.get_identifier();
    DEVICE_ID device_id;
    device_id.set_device_type("TRANSFORMER");
    TERMINAL terminal;
    terminal.append_bus(primary_winding_bus);
    terminal.append_bus(secondary_winding_bus);
    terminal.append_bus(tertiary_winding_bus);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier(identifier);

    if(is_transformer_exist(device_id))
    {
        osstream<<"Warning. "<<transformer.get_device_name()<<" already exists in power system database '"<<get_system_name()<<"':Transformer.\n"
          <<"Duplicate copy is not allowed.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(Transformer.capacity()==Transformer.size())
    {
        osstream<<"Warning. Capacity limit ("<<Transformer.capacity()<<") reached when appending transformer to power system database '"<<get_system_name()<<"'."<<endl
          <<"Increase capacity by modified steps_config.json.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    size_t transformer_count = get_transformer_count();
    Transformer.push_back(transformer);
    transformer_index.set_device_index(device_id, transformer_count);
}

void POWER_SYSTEM_DATABASE::append_fixed_shunt(const FIXED_SHUNT& shunt)
{
    ostringstream osstream;

    //if(shunt.get_default_power_system_database()==NULL)
    //    shunt.set_power_system_database(this);

    if(not shunt.is_valid())
    {
        osstream<<"Warning. Failed to append invalid fixed shunt to power system database '"<<get_system_name()<<"'.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    size_t shunt_bus = shunt.get_shunt_bus();

    if(not this->is_bus_in_allowed_range(shunt_bus))
    {
        osstream<<"Warning. Bus "<<shunt_bus<<" is not in the allowed range [1, "<<get_allowed_max_bus_number()<<"] when appending "<<shunt.get_device_name()<<" to power system database '"<<get_system_name()<<"'."<<endl
          <<"Fixed shunt will not be appended into the database.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    string identifier = shunt.get_identifier();
    DEVICE_ID device_id;
    device_id.set_device_type("FIXED SHUNT");
    TERMINAL terminal;
    terminal.append_bus(shunt_bus);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier(identifier);

    if(this->is_fixed_shunt_exist(device_id))
    {
        osstream<<"Warning. "<<shunt.get_device_name()<<" already exists in power system database '"<<get_system_name()<<"': Fixed_shunt.\n"
          <<"Duplicate copy is not allowed.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(Fixed_shunt.capacity()==Fixed_shunt.size())
    {
        osstream<<"Warning. Capacity limit ("<<Fixed_shunt.capacity()<<") reached when appending fixed shunt to power system database '"<<get_system_name()<<"'."<<endl
          <<"Increase capacity by modified steps_config.json.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    size_t shunt_count = get_fixed_shunt_count();
    Fixed_shunt.push_back(shunt);
    fixed_shunt_index.set_device_index(device_id, shunt_count);
}

void POWER_SYSTEM_DATABASE::append_hvdc(const HVDC& hvdc)
{
    ostringstream osstream;

    //if(hvdc.get_default_power_system_database()==NULL)
    //    hvdc.set_power_system_database(this);

    if(not hvdc.is_valid())
    {
        osstream<<"Warning. Failed to append invalid hvdc to power system database '"<<get_system_name()<<"'.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    size_t rec_bus = hvdc.get_converter_bus(RECTIFIER);
    size_t inv_bus = hvdc.get_converter_bus(INVERTER);

    if(not this->is_bus_in_allowed_range(rec_bus))
    {
        osstream<<"Warning. Bus "<<rec_bus<<" is not in the allowed range [1, "<<get_allowed_max_bus_number()<<"] when appending "<<hvdc.get_device_name()<<" to power system database '"<<get_system_name()<<"'."<<endl
          <<"Hvdc will not be appended into the database.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }
    if(not this->is_bus_in_allowed_range(inv_bus))
    {
        osstream<<"Warning. Bus "<<inv_bus<<" is not in the allowed range [1, "<<get_allowed_max_bus_number()<<"] when appending "<<hvdc.get_device_name()<<" to power system database '"<<get_system_name()<<"'."<<endl
          <<"Hvdc will not be appended into the database.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    string identifier = hvdc.get_identifier();
    DEVICE_ID device_id;
    device_id.set_device_type("HVDC");
    TERMINAL terminal;
    terminal.append_bus(rec_bus);
    terminal.append_bus(inv_bus);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier(identifier);

    if(this->is_hvdc_exist(device_id))
    {
        osstream<<"Warning. "<<hvdc.get_device_name()<<" already exists in power system database '"<<get_system_name()<<"': Hvdc.\n"
          <<"Duplicate copy is not allowed.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(Hvdc.capacity()==Hvdc.size())
    {
        osstream<<"Warning. Capacity limit ("<<Hvdc.capacity()<<") reached when appending Hvdc to power system database '"<<get_system_name()<<"'."<<endl
          <<"Increase capacity by modified steps_config.json.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    size_t hvdc_count = get_hvdc_count();
    Hvdc.push_back(hvdc);
    hvdc_index.set_device_index(device_id, hvdc_count);
}

void POWER_SYSTEM_DATABASE::append_equivalent_device(const EQUIVALENT_DEVICE& edevice)
{
    ostringstream osstream;

    //if(edevice.get_default_power_system_database()==NULL)
    //    edevice.set_power_system_database(this);

    if(not edevice.is_valid())
    {
        osstream<<"Warning. Failed to append invalid equivalent device to power system database '"<<get_system_name()<<"'.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    size_t bus = edevice.get_equivalent_device_bus();

    if(not this->is_bus_in_allowed_range(bus))
    {
        osstream<<"Warning. Bus "<<bus<<" is not in the allowed range [1, "<<get_allowed_max_bus_number()<<"] when appending "<<edevice.get_device_name()<<" to power system database '"<<get_system_name()<<"'."<<endl
          <<"Equivalent device will not be appended into the database.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    string identifier = edevice.get_identifier();
    DEVICE_ID device_id;
    device_id.set_device_type("EQUIVALENT DEVICE");
    TERMINAL terminal;
    terminal.append_bus(bus);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier(identifier);

    if(this->is_equivalent_device_exist(device_id))
    {
        osstream<<"Warning. "<<edevice.get_device_name()<<" already exists in power system database '"<<get_system_name()<<"': Equivalent_device.\n"
          <<"Duplicate copy is not allowed.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(Equivalent_device.capacity()==Equivalent_device.size())
    {
        osstream<<"Warning. Capacity limit ("<<Equivalent_device.capacity()<<") reached when appending equivalent device to power system database '"<<get_system_name()<<"'."<<endl
          <<"Increase capacity by modified steps_config.json.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    size_t equivalent_device_count = get_equivalent_device_count();
    Equivalent_device.push_back(edevice);
    equivalent_device_index.set_device_index(device_id, equivalent_device_count);
 }

void POWER_SYSTEM_DATABASE::append_area(const AREA& area)
{
    ostringstream osstream;

    //if(area.get_default_power_system_database()==NULL)
    //    area.set_power_system_database(this);

    if(not area.is_valid())
    {
        osstream<<"Warning. Failed to append invalid area to power system database '"<<get_system_name()<<"'.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(this->is_area_exist(area.get_area_number()))
    {
        osstream<<"Warning. Area "<<area.get_area_number()<<" already exists in power system database '"<<get_system_name()<<"': Area."<<endl
          <<"Duplicate copy is not allowed.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(Area.capacity()==Area.size())
    {
        osstream<<"Warning. Capacity limit ("<<Area.capacity()<<") reached when appending area to power system database '"<<get_system_name()<<"'."<<endl
          <<"Increase capacity by modified steps_config.json.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    size_t area_count = get_area_count();
    Area.push_back(area);
    area_index[area.get_area_number()]= area_count;
}

void POWER_SYSTEM_DATABASE::append_zone(const ZONE& zone)
{
    ostringstream osstream;
    if(not zone.is_valid())
    {
        osstream<<"Warning. Failed to append invalid zone to power system database '"<<get_system_name()<<"'.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(this->is_zone_exist(zone.get_zone_number()))
    {
        osstream<<"Warning. Zone "<<zone.get_zone_number()<<" already exists in power system database '"<<get_system_name()<<"': Zone."<<endl
          <<"Duplicate copy is not allowed.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(Zone.capacity()==Zone.size())
    {
        osstream<<"Warning. Capacity limit ("<<Zone.capacity()<<") reached when appending zone to power system database '"<<get_system_name()<<"'."<<endl
          <<"Increase capacity by modified steps_config.json.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    size_t zone_count = get_zone_count();
    Zone.push_back(zone);
    zone_index[zone.get_zone_number()]= zone_count;
}

void POWER_SYSTEM_DATABASE::append_owner(const OWNER& owner)
{
    ostringstream osstream;

    if(not owner.is_valid())
    {
        osstream<<"Warning. Failed to append invalid owner to power system database '"<<get_system_name()<<"'.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(this->is_owner_exist(owner.get_owner_number()))
    {
        osstream<<"Warning. Owner "<<owner.get_owner_number()<<" already exists in power system database '"<<get_system_name()<<"': Owner."<<endl
          <<"Duplicate copy is not allowed.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(Owner.capacity()==Owner.size())
    {
        osstream<<"Warning. Capacity limit ("<<Owner.capacity()<<") reached when appending owner to power system database '"<<get_system_name()<<"'."<<endl
          <<"Increase capacity by modified steps_config.json.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    size_t owner_count = get_owner_count();
    Owner.push_back(owner);
    owner_index[owner.get_owner_number()]= owner_count;
}

void POWER_SYSTEM_DATABASE::append_dynamic_model(const DEVICE_ID& did, const MODEL* model)
{
    if(did.get_device_type() != model->get_allowed_device_type())
        return;
    if(did.get_device_type()=="LOAD")
    {
        append_load_related_model(did, model);
        return;
    }
    if(did.get_device_type()=="GENERATOR")
    {
        append_generator_related_model(did, model);
        return;
    }
    if(did.get_device_type()=="WT GENERATOR")
    {
        append_wt_generator_related_model(did, model);
        return;
    }
    if(did.get_device_type()=="PV UNIT")
    {
        append_pv_unit_related_model(did, model);
        return;
    }
    if(did.get_device_type()=="ENERGY STORAGE")
    {
        append_energy_storage_related_model(did, model);
        return;
    }
    if(did.get_device_type()=="HVDC")
    {
        append_hvdc_related_model(did, model);
        return;
    }
    return;
}


void POWER_SYSTEM_DATABASE::append_load_related_model(const DEVICE_ID& did, const MODEL* model)
{
    if(did.get_device_type() != model->get_allowed_device_type())
        return;
    if(did.get_device_type()=="LOAD")
    {
        LOAD* ptr = get_load(did);
        if(ptr!=NULL)
            ptr->set_model(model);
        return;
    }
}

void POWER_SYSTEM_DATABASE::append_generator_related_model(const DEVICE_ID& did, const MODEL* model)
{
    if(did.get_device_type() != model->get_allowed_device_type())
        return;
    if(did.get_device_type()=="GENERATOR")
    {
        GENERATOR* ptr = get_generator(did);
        if(ptr!=NULL)
            ptr->set_model(model);
        return;
    }
}

void POWER_SYSTEM_DATABASE::append_wt_generator_related_model(const DEVICE_ID& did, const MODEL* model)
{
    if(did.get_device_type() != model->get_allowed_device_type())
        return;
    if(did.get_device_type()=="WT GENERATOR")
    {
        WT_GENERATOR* ptr = get_wt_generator(did);
        if(ptr!=NULL)
            ptr->set_model(model);
        return;
    }
}

void POWER_SYSTEM_DATABASE::append_pv_unit_related_model(const DEVICE_ID& did, const MODEL* model)
{
    if(did.get_device_type() != model->get_allowed_device_type())
        return;
    if(did.get_device_type()=="PV UNIT")
    {
        PV_UNIT* ptr = get_pv_unit(did);
        if(ptr!=NULL)
            ptr->set_model(model);
        return;
    }
}

void POWER_SYSTEM_DATABASE::append_energy_storage_related_model(const DEVICE_ID& did, const MODEL* model)
{
    if(did.get_device_type() != model->get_allowed_device_type())
        return;
    if(did.get_device_type()=="ENERGY STORAGE")
    {
        ENERGY_STORAGE* ptr = get_energy_storage(did);
        if(ptr!=NULL)
            ptr->set_model(model);
        return;
    }
}
void POWER_SYSTEM_DATABASE::append_hvdc_related_model(const DEVICE_ID& did, const MODEL* model)
{
    if(did.get_device_type() != model->get_allowed_device_type())
        return;
    if(did.get_device_type()=="HVDC")
    {
        HVDC* ptr = get_hvdc(did);
        if(ptr!=NULL)
            ptr->set_model(model);
        return;
    }
}

void POWER_SYSTEM_DATABASE::update_all_bus_base_frequency(double fbase_Hz)
{
    size_t n = Bus.size();
    for(size_t i=0; i<n; ++i)
        Bus[i].set_base_frequency_in_Hz(fbase_Hz);
}

bool POWER_SYSTEM_DATABASE::is_bus_exist(size_t bus) const
{
    if(bus == 0) return false;

    size_t index = this->get_bus_index(bus);
    if(index != INDEX_NOT_EXIST)
        return true;
    else
        return false;
}

bool POWER_SYSTEM_DATABASE::is_generator_exist(const DEVICE_ID& device_id) const
{
    size_t index = get_generator_index(device_id);
    if(index==INDEX_NOT_EXIST) return false;
    else return true;
}

bool POWER_SYSTEM_DATABASE::is_wt_generator_exist(const DEVICE_ID& device_id) const
{
    size_t index = get_wt_generator_index(device_id);
    if(index==INDEX_NOT_EXIST) return false;
    else return true;
}

bool POWER_SYSTEM_DATABASE::is_pv_unit_exist(const DEVICE_ID& device_id) const
{
    size_t index = get_pv_unit_index(device_id);
    if(index==INDEX_NOT_EXIST) return false;
    else return true;
}

bool POWER_SYSTEM_DATABASE::is_load_exist(const DEVICE_ID& device_id) const
{
    size_t index = get_load_index(device_id);
    if(index==INDEX_NOT_EXIST) return false;
    else return true;
}

bool POWER_SYSTEM_DATABASE::is_line_exist(const DEVICE_ID& device_id) const
{
    size_t index = get_line_index(device_id);
    if(index==INDEX_NOT_EXIST) return false;
    else return true;
}

bool POWER_SYSTEM_DATABASE::is_transformer_exist(const DEVICE_ID& device_id) const
{
    size_t index = get_transformer_index(device_id);
    if(index==INDEX_NOT_EXIST) return false;
    else return true;
}

bool POWER_SYSTEM_DATABASE::is_fixed_shunt_exist(const DEVICE_ID& device_id) const
{
    size_t index = get_fixed_shunt_index(device_id);
    if(index==INDEX_NOT_EXIST) return false;
    else return true;
}

bool POWER_SYSTEM_DATABASE::is_hvdc_exist(const DEVICE_ID& device_id) const
{
    size_t index = get_hvdc_index(device_id);
    if(index==INDEX_NOT_EXIST) return false;
    else return true;
}

bool POWER_SYSTEM_DATABASE::is_equivalent_device_exist(const DEVICE_ID& device_id) const
{
    size_t index = get_equivalent_device_index(device_id);
    if(index==INDEX_NOT_EXIST) return false;
    else return true;
}

bool POWER_SYSTEM_DATABASE::is_energy_storage_exist(const DEVICE_ID& device_id) const
{
    size_t index = get_energy_storage_index(device_id);
    if(index==INDEX_NOT_EXIST) return false;
    else return true;
}

bool POWER_SYSTEM_DATABASE::is_area_exist(const size_t no) const
{
    size_t index = get_area_index(no);
    if(index==INDEX_NOT_EXIST) return false;
    else return true;
}

bool POWER_SYSTEM_DATABASE::is_zone_exist(const size_t no) const
{
    size_t index = get_zone_index(no);
    if(index==INDEX_NOT_EXIST) return false;
    else return true;
}

bool POWER_SYSTEM_DATABASE::is_owner_exist(const size_t no) const
{
    size_t index = get_owner_index(no);
    if(index==INDEX_NOT_EXIST) return false;
    else return true;
}

void POWER_SYSTEM_DATABASE::change_bus_number(size_t original_bus_number, size_t new_bus_number)
{
    ostringstream osstream;
    if(not is_bus_exist(original_bus_number))
    {
        osstream<<"Warning. The original bus number to change ("<<original_bus_number<<") does not exist in the power system database. No bus number will be changed.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }
    if(is_bus_exist(new_bus_number))
    {
        osstream<<"Warning. The new bus number to change ("<<new_bus_number<<") already exists in the power system database. No bus number will be changed.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }
    if(not is_bus_in_allowed_range(new_bus_number))
    {
        osstream<<"Warning. The new bus number to change ("<<new_bus_number<<") exceeds the allowed maximum bus number range [1, "<<get_allowed_max_bus_number()<<"] in the power system database. No bus number will be changed.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }


    BUS* bus = get_bus(original_bus_number);

    bus->set_bus_number(new_bus_number);

    bus_index.set_bus_with_index(new_bus_number, get_bus_index(original_bus_number));
    bus_index.set_bus_with_index(original_bus_number, INDEX_NOT_EXIST);

    vector<GENERATOR*> gens = get_generators_connecting_to_bus(original_bus_number);
    size_t ngens = gens.size();
    for(size_t i=0; i!=ngens; ++i)
    {
        GENERATOR* gen = gens[i];
        DEVICE_ID old_did = gen->get_device_id();
        size_t index = get_generator_index(old_did);
        generator_index.set_device_index(old_did, INDEX_NOT_EXIST);

        gen->set_generator_bus(new_bus_number);
        DEVICE_ID new_did = gen->get_device_id();
        generator_index.set_device_index(new_did, index);
    }

    vector<WT_GENERATOR*> wt_generators = get_wt_generators_connecting_to_bus(original_bus_number);
    size_t nwt_generator = wt_generators.size();
    for(size_t i=0; i!=nwt_generator; ++i)
    {
        WT_GENERATOR* wt_generator = wt_generators[i];
        DEVICE_ID old_did = wt_generator->get_device_id();
        size_t index = get_wt_generator_index(old_did);
        wt_generator_index.set_device_index(old_did, INDEX_NOT_EXIST);

        wt_generator->set_source_bus(new_bus_number);
        DEVICE_ID new_did = wt_generator->get_device_id();
        wt_generator_index.set_device_index(new_did, index);
    }

    vector<PV_UNIT*> pv_units = get_pv_units_connecting_to_bus(original_bus_number);
    size_t npv_unit = pv_units.size();
    for(size_t i=0; i!=npv_unit; ++i)
    {
        PV_UNIT* pv_unit = pv_units[i];
        DEVICE_ID old_did = pv_unit->get_device_id();
        size_t index = get_pv_unit_index(old_did);
        pv_unit_index.set_device_index(old_did, INDEX_NOT_EXIST);

        pv_unit->set_unit_bus(new_bus_number);
        DEVICE_ID new_did = pv_unit->get_device_id();
        pv_unit_index.set_device_index(new_did, index);
    }

    vector<ENERGY_STORAGE*> estorages = get_energy_storages_connecting_to_bus(original_bus_number);
    size_t nestorage = estorages.size();
    for(size_t i=0; i!=nestorage; ++i)
    {
        ENERGY_STORAGE* estorage = estorages[i];
        DEVICE_ID old_did = estorage->get_device_id();
        size_t index = get_energy_storage_index(old_did);
        energy_storage_index.set_device_index(old_did, INDEX_NOT_EXIST);

        estorage->set_energy_storage_bus(new_bus_number);
        DEVICE_ID new_did = estorage->get_device_id();
        energy_storage_index.set_device_index(new_did, index);
    }

    vector<LOAD*> loads = get_loads_connecting_to_bus(original_bus_number);
    size_t nload = loads.size();
    for(size_t i=0; i!=nload; ++i)
    {
        LOAD* load = loads[i];
        DEVICE_ID old_did = load->get_device_id();
        size_t index = get_load_index(old_did);
        load_index.set_device_index(old_did, INDEX_NOT_EXIST);

        load->set_load_bus(new_bus_number);
        DEVICE_ID new_did = load->get_device_id();
        load_index.set_device_index(new_did, index);
    }

    vector<FIXED_SHUNT*> fshunts = get_fixed_shunts_connecting_to_bus(original_bus_number);
    size_t nfshunt = fshunts.size();
    for(size_t i=0; i!=nfshunt; ++i)
    {
        FIXED_SHUNT* fshunt = fshunts[i];
        DEVICE_ID old_did = fshunt->get_device_id();
        size_t index = get_fixed_shunt_index(old_did);
        fixed_shunt_index.set_device_index(old_did, INDEX_NOT_EXIST);

        fshunt->set_shunt_bus(new_bus_number);
        DEVICE_ID new_did = fshunt->get_device_id();
        fixed_shunt_index.set_device_index(new_did, index);
    }

    vector<LINE*> lines = get_lines_connecting_to_bus(original_bus_number);
    size_t nline = lines.size();
    for(size_t i=0; i!=nline; ++i)
    {
        LINE* line = lines[i];
        DEVICE_ID old_did = line->get_device_id();
        size_t index = get_line_index(old_did);
        line_index.set_device_index(old_did, INDEX_NOT_EXIST);

        if(line->get_sending_side_bus()==original_bus_number)
            line->set_sending_side_bus(new_bus_number);
        else
            line->set_receiving_side_bus(new_bus_number);

        DEVICE_ID new_did = line->get_device_id();
        line_index.set_device_index(new_did, index);

    }

    vector<HVDC*> hvdcs = get_hvdcs_connecting_to_bus(original_bus_number);
    size_t nhvdc = hvdcs.size();
    for(size_t i=0; i!=nhvdc; ++i)
    {
        HVDC* hvdc = hvdcs[i];
        DEVICE_ID old_did = hvdc->get_device_id();
        size_t index = get_hvdc_index(old_did);
        hvdc_index.set_device_index(old_did, INDEX_NOT_EXIST);

        if(hvdc->get_converter_bus(RECTIFIER)==original_bus_number)
            hvdc->set_converter_bus(RECTIFIER, new_bus_number);
        else
            hvdc->set_converter_bus(INVERTER, new_bus_number);

        DEVICE_ID new_did = hvdc->get_device_id();
        hvdc_index.set_device_index(new_did, index);
    }

    vector<TRANSFORMER*> transformers = get_transformers_connecting_to_bus(original_bus_number);
    size_t ntrans = transformers.size();
    for(size_t i=0; i!=ntrans; ++i)
    {
        TRANSFORMER* trans = transformers[i];
        DEVICE_ID old_did = trans->get_device_id();
        size_t index = get_transformer_index(old_did);
        transformer_index.set_device_index(old_did, INDEX_NOT_EXIST);

        if(trans->get_winding_bus(PRIMARY_SIDE)==original_bus_number)
            trans->set_winding_bus(PRIMARY_SIDE, new_bus_number);
        else
        {
            if(trans->get_winding_bus(SECONDARY_SIDE)==original_bus_number)
                trans->set_winding_bus(SECONDARY_SIDE, new_bus_number);
            else
                trans->set_winding_bus(TERTIARY_SIDE, new_bus_number);
        }

        DEVICE_ID new_did = trans->get_device_id();
        transformer_index.set_device_index(new_did, index);
    }
}

DEVICE* POWER_SYSTEM_DATABASE::get_device(const DEVICE_ID& device_id)
{
    // this function is not tested
    if(not device_id.is_valid())
        return NULL;

    string dtype = device_id.get_device_type();
    if(dtype=="BUS")
        return (DEVICE*) get_bus(device_id);

    if(dtype=="GENERATOR")
        return (DEVICE*) get_generator(device_id);

    if(dtype=="WT GENERATOR")
        return (DEVICE*) get_wt_generator(device_id);

    if(dtype=="PV UNIT")
        return (DEVICE*) get_pv_unit(device_id);

    if(dtype=="LOAD")
        return (DEVICE*) get_load(device_id);

    if(dtype=="FIXED SHUNT")
        return (DEVICE*) get_fixed_shunt(device_id);

    if(dtype=="LINE")
        return (DEVICE*) get_line(device_id);

    if(dtype=="HVDC")
        return (DEVICE*) get_hvdc(device_id);

    if(dtype=="TRANSFORMER")
        return (DEVICE*) get_transformer(device_id);

    if(dtype=="EQUIVALENT DEVICE")
        return (DEVICE*) get_equivalent_device(device_id);

    return NULL;
}

BUS* POWER_SYSTEM_DATABASE::get_bus(size_t bus)
{
    size_t index = bus_index.get_index_of_bus(bus);
    if(index!=INDEX_NOT_EXIST)
        return &(Bus[index]);
    else
        return NULL;
}

BUS* POWER_SYSTEM_DATABASE::get_bus(const DEVICE_ID & device_id)
{
    if(device_id.is_valid())
    {
        if(device_id.get_device_type()=="BUS")
        {
            TERMINAL terminal = device_id.get_device_terminal();
            size_t bus = terminal[0];
            return get_bus(bus);
        }
        else
            return NULL;
    }
    else
        return NULL;
}

GENERATOR* POWER_SYSTEM_DATABASE::get_generator(const DEVICE_ID & device_id)
{
    size_t index = generator_index.get_index_of_device(device_id);
    if(index!=INDEX_NOT_EXIST)
        return &(Generator[index]);
    else
        return NULL;
}

WT_GENERATOR* POWER_SYSTEM_DATABASE::get_wt_generator(const DEVICE_ID & device_id)
{
    size_t index = wt_generator_index.get_index_of_device(device_id);
    if(index!=INDEX_NOT_EXIST)
        return &(WT_Generator[index]);
    else
        return NULL;
}

PV_UNIT* POWER_SYSTEM_DATABASE::get_pv_unit(const DEVICE_ID & device_id)
{
    size_t index = pv_unit_index.get_index_of_device(device_id);
    if(index!=INDEX_NOT_EXIST)
        return &(PV_Unit[index]);
    else
        return NULL;
}

SOURCE* POWER_SYSTEM_DATABASE::get_source(const DEVICE_ID & device_id)
{
    SOURCE* source = NULL;
    source = (SOURCE*) get_generator(device_id);
    if(source!=NULL)
        return source;
    else
    {
        source = (SOURCE*) get_wt_generator(device_id);
        if(source!=NULL)
            return source;
        else
        {
            source = (SOURCE*) get_pv_unit(device_id);
            if(source!=NULL)
                return source;
            else
            {
                source = (SOURCE*) get_energy_storage(device_id);
                return source;
            }
        }
    }
}

LOAD* POWER_SYSTEM_DATABASE::get_load(const DEVICE_ID & device_id)
{
    size_t index = load_index.get_index_of_device(device_id);
    if(index!=INDEX_NOT_EXIST)
        return &(Load[index]);
    else
        return NULL;
}

LINE* POWER_SYSTEM_DATABASE::get_line(const DEVICE_ID & device_id)
{
    size_t index = line_index.get_index_of_device(device_id);
    if(index!=INDEX_NOT_EXIST)
        return &(Line[index]);
    else
        return NULL;
}

TRANSFORMER* POWER_SYSTEM_DATABASE::get_transformer(const DEVICE_ID & device_id)
{
    size_t index = transformer_index.get_index_of_device(device_id);
    if(index!=INDEX_NOT_EXIST)
        return &(Transformer[index]);
    else
        return NULL;
}

FIXED_SHUNT* POWER_SYSTEM_DATABASE::get_fixed_shunt(const DEVICE_ID & device_id)
{
    size_t index = fixed_shunt_index.get_index_of_device(device_id);
    if(index!=INDEX_NOT_EXIST)
        return &(Fixed_shunt[index]);
    else
        return NULL;
}

HVDC* POWER_SYSTEM_DATABASE::get_hvdc(const DEVICE_ID & device_id)
{
    size_t index = hvdc_index.get_index_of_device(device_id);
    if(index!=INDEX_NOT_EXIST)
        return &(Hvdc[index]);
    else
        return NULL;
}

EQUIVALENT_DEVICE* POWER_SYSTEM_DATABASE::get_equivalent_device(const DEVICE_ID & device_id)
{
    size_t index = equivalent_device_index.get_index_of_device(device_id);
    if(index!=INDEX_NOT_EXIST)
        return &(Equivalent_device[index]);
    else
        return NULL;
}

ENERGY_STORAGE* POWER_SYSTEM_DATABASE::get_energy_storage(const DEVICE_ID & device_id)
{
    size_t index = energy_storage_index.get_index_of_device(device_id);
    if(index!=INDEX_NOT_EXIST)
        return &(Energy_storage[index]);
    else
        return NULL;
}

AREA* POWER_SYSTEM_DATABASE::get_area(const size_t no)
{
    size_t index = get_area_index(no);
    if(index!=INDEX_NOT_EXIST)
        return &(Area[index]);
    else
        return NULL;
}

ZONE* POWER_SYSTEM_DATABASE::get_zone(const size_t no)
{
    size_t index = get_zone_index(no);
    if(index!=INDEX_NOT_EXIST)
        return &(Zone[index]);
    else
        return NULL;
}

OWNER* POWER_SYSTEM_DATABASE::get_owner(const size_t no)
{
    size_t index = get_owner_index(no);
    if(index!=INDEX_NOT_EXIST)
        return &(Owner[index]);
    else
        return NULL;
}

vector<DEVICE*> POWER_SYSTEM_DATABASE::get_all_devices_connecting_to_bus(const size_t bus)
{
    vector<DEVICE*> devices;
    size_t n=0;

    vector<SOURCE*> sources = get_sources_connecting_to_bus(bus);
    n = sources.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(sources[i]);

    vector<LOAD*> loads = get_loads_connecting_to_bus(bus);
    n = loads.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(loads[i]);

    vector<LINE*> lines = get_lines_connecting_to_bus(bus);
    n = lines.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(lines[i]);

    vector<TRANSFORMER*> transes = get_transformers_connecting_to_bus(bus);
    n = transes.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(transes[i]);

    vector<FIXED_SHUNT*> fshunts = get_fixed_shunts_connecting_to_bus(bus);
    n = fshunts.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(fshunts[i]);

    vector<HVDC*> hvdcs = get_hvdcs_connecting_to_bus(bus);
    n = hvdcs.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(hvdcs[i]);

    vector<EQUIVALENT_DEVICE*> edevices = get_equivalent_devices_connecting_to_bus(bus);
    n = edevices.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(edevices[i]);

    return devices;
}

vector<GENERATOR*> POWER_SYSTEM_DATABASE::get_generators_connecting_to_bus(const size_t bus)
{
    vector<GENERATOR*> device;
    device.reserve(8);

    size_t n = get_generator_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Generator[i].is_connected_to_bus(bus))
            device.push_back(&(Generator[i]));
    }
    return device;
}

vector<WT_GENERATOR*> POWER_SYSTEM_DATABASE::get_wt_generators_connecting_to_bus(const size_t bus)
{
    vector<WT_GENERATOR*> device;
    device.reserve(8);

    size_t n = get_wt_generator_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(WT_Generator[i].is_connected_to_bus(bus))
            device.push_back(&(WT_Generator[i]));
    }
    return device;
}


vector<PV_UNIT*> POWER_SYSTEM_DATABASE::get_pv_units_connecting_to_bus(const size_t bus)
{
    vector<PV_UNIT*> device;
    device.reserve(8);

    size_t n = get_pv_unit_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(PV_Unit[i].is_connected_to_bus(bus))
            device.push_back(&(PV_Unit[i]));
    }
    return device;
}

vector<SOURCE*> POWER_SYSTEM_DATABASE::get_sources_connecting_to_bus(const size_t bus)
{
    vector<SOURCE*> device;
    vector<GENERATOR*> gen_device;
    vector<WT_GENERATOR*> wt_generator_device;
    vector<PV_UNIT*> pv_unit_device;
    vector<ENERGY_STORAGE*> estorage_device;
    device.reserve(8);

    gen_device = get_generators_connecting_to_bus(bus);
    size_t ngens = gen_device.size();
    for(size_t i=0; i!=ngens; ++i)
        device.push_back(gen_device[i]);

    wt_generator_device = get_wt_generators_connecting_to_bus(bus);
    size_t nwt_generator = wt_generator_device.size();
    for(size_t i=0; i!=nwt_generator; ++i)
        device.push_back(wt_generator_device[i]);

    pv_unit_device = get_pv_units_connecting_to_bus(bus);
    size_t npv_unit = pv_unit_device.size();
    for(size_t i=0; i!=npv_unit; ++i)
        device.push_back(pv_unit_device[i]);

    estorage_device = get_energy_storages_connecting_to_bus(bus);
    size_t nes = estorage_device.size();
    for(size_t i=0; i!=nes; ++i)
        device.push_back(estorage_device[i]);

    return device;
}

vector<LOAD*> POWER_SYSTEM_DATABASE::get_loads_connecting_to_bus(const size_t bus)
{
    vector<LOAD*> device;
    device.reserve(8);

    size_t n = get_load_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Load[i].is_connected_to_bus(bus))
            device.push_back(&(Load[i]));
    }
    return device;
}

vector<LINE*> POWER_SYSTEM_DATABASE::get_lines_connecting_to_bus(const size_t bus)
{
    vector<LINE*> device;
    device.reserve(8);

    size_t n = get_line_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Line[i].is_connected_to_bus(bus))
            device.push_back(&(Line[i]));
    }
    return device;
}

vector<TRANSFORMER*> POWER_SYSTEM_DATABASE::get_transformers_connecting_to_bus(const size_t bus)
{
    vector<TRANSFORMER*> device;
    device.reserve(8);

    size_t n = get_transformer_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Transformer[i].is_connected_to_bus(bus))
            device.push_back(&(Transformer[i]));
    }
    return device;
}

vector<FIXED_SHUNT*> POWER_SYSTEM_DATABASE::get_fixed_shunts_connecting_to_bus(const size_t bus)
{
    vector<FIXED_SHUNT*> device;
    device.reserve(8);

    size_t n = get_fixed_shunt_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Fixed_shunt[i].is_connected_to_bus(bus))
            device.push_back(&(Fixed_shunt[i]));
    }
    return device;
}

vector<HVDC*> POWER_SYSTEM_DATABASE::get_hvdcs_connecting_to_bus(const size_t bus)
{
    vector<HVDC*> device;
    device.reserve(8);

    size_t n = get_hvdc_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Hvdc[i].is_connected_to_bus(bus))
            device.push_back(&(Hvdc[i]));
    }
    return device;
}

vector<EQUIVALENT_DEVICE*> POWER_SYSTEM_DATABASE::get_equivalent_devices_connecting_to_bus(const size_t bus)
{
    vector<EQUIVALENT_DEVICE*> device;
    device.reserve(8);

    size_t n = get_equivalent_device_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Equivalent_device[i].is_connected_to_bus(bus))
            device.push_back(&(Equivalent_device[i]));
    }
    return device;
}

vector<ENERGY_STORAGE*> POWER_SYSTEM_DATABASE::get_energy_storages_connecting_to_bus(const size_t bus)
{
    vector<ENERGY_STORAGE*> device;
    device.reserve(8);

    size_t n = get_energy_storage_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Energy_storage[i].is_connected_to_bus(bus))
            device.push_back(&(Energy_storage[i]));
    }
    return device;
}


vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_all_devices_device_id_connecting_to_bus(const size_t bus)
{
    vector<DEVICE*> devices = get_all_devices_connecting_to_bus(bus);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_generators_device_id_connecting_to_bus(const size_t bus)
{
    vector<GENERATOR*> devices = get_generators_connecting_to_bus(bus);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_wt_generators_device_id_connecting_to_bus(const size_t bus)
{
    vector<WT_GENERATOR*> devices = get_wt_generators_connecting_to_bus(bus);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_pv_units_device_id_connecting_to_bus(const size_t bus)
{
    vector<PV_UNIT*> devices = get_pv_units_connecting_to_bus(bus);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_sources_device_id_connecting_to_bus(const size_t bus)
{
    vector<SOURCE*> devices = get_sources_connecting_to_bus(bus);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_loads_device_id_connecting_to_bus(const size_t bus)
{
    vector<LOAD*> devices = get_loads_connecting_to_bus(bus);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_lines_device_id_connecting_to_bus(const size_t bus)
{
    vector<LINE*> devices = get_lines_connecting_to_bus(bus);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_transformers_device_id_connecting_to_bus(const size_t bus)
{
    vector<TRANSFORMER*> devices = get_transformers_connecting_to_bus(bus);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_fixed_shunts_device_id_connecting_to_bus(const size_t bus)
{
    vector<FIXED_SHUNT*> devices = get_fixed_shunts_connecting_to_bus(bus);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_hvdcs_device_id_connecting_to_bus(const size_t bus)
{
    vector<HVDC*> devices = get_hvdcs_connecting_to_bus(bus);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_equivalent_devices_device_id_connecting_to_bus(const size_t bus)
{
    vector<EQUIVALENT_DEVICE*> devices = get_equivalent_devices_connecting_to_bus(bus);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_energy_storages_device_id_connecting_to_bus(const size_t bus)
{
    vector<ENERGY_STORAGE*> devices = get_energy_storages_connecting_to_bus(bus);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}


vector<DEVICE*> POWER_SYSTEM_DATABASE::get_all_devices_in_area(const size_t area)
{
    vector<DEVICE*> devices;
    size_t n=0;

    vector<BUS*> buses = get_buses_in_area(area);
    n = buses.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(buses[i]);

    vector<SOURCE*> sources = get_sources_in_area(area);
    n = sources.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(sources[i]);

    vector<LOAD*> loads = get_loads_in_area(area);
    n = loads.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(loads[i]);

    vector<LINE*> lines = get_lines_in_area(area);
    n = lines.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(lines[i]);

    vector<TRANSFORMER*> transes = get_transformers_in_area(area);
    n = transes.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(transes[i]);

    vector<FIXED_SHUNT*> fshunts = get_fixed_shunts_in_area(area);
    n = fshunts.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(fshunts[i]);

    vector<HVDC*> hvdcs = get_hvdcs_in_area(area);
    n = hvdcs.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(hvdcs[i]);

    vector<EQUIVALENT_DEVICE*> edevices = get_equivalent_devices_in_area(area);
    n = edevices.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(edevices[i]);

    return devices;
}

vector<BUS*> POWER_SYSTEM_DATABASE::get_buses_in_area(const size_t area)
{
    vector<BUS*> device;
    device.reserve(8);

    size_t n = get_bus_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Bus[i].is_in_area(area))
            device.push_back(&(Bus[i]));
    }
    return device;
}

vector<GENERATOR*> POWER_SYSTEM_DATABASE::get_generators_in_area(const size_t area)
{
    vector<GENERATOR*> device;
    device.reserve(8);

    size_t n = get_generator_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Generator[i].is_in_area(area))
            device.push_back(&(Generator[i]));
    }
    return device;
}

vector<WT_GENERATOR*> POWER_SYSTEM_DATABASE::get_wt_generators_in_area(const size_t area)
{
    vector<WT_GENERATOR*> device;
    device.reserve(8);

    size_t n = get_wt_generator_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(WT_Generator[i].is_in_area(area))
            device.push_back(&(WT_Generator[i]));
    }
    return device;
}


vector<PV_UNIT*> POWER_SYSTEM_DATABASE::get_pv_units_in_area(const size_t area)
{
    vector<PV_UNIT*> device;
    device.reserve(8);

    size_t n = get_pv_unit_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(PV_Unit[i].is_in_area(area))
            device.push_back(&(PV_Unit[i]));
    }
    return device;
}

vector<SOURCE*> POWER_SYSTEM_DATABASE::get_sources_in_area(const size_t area)
{
    vector<SOURCE*> device;
    vector<GENERATOR*> gen_device;
    vector<WT_GENERATOR*> wt_generator_device;
    vector<PV_UNIT*> pv_unit_device;
    vector<ENERGY_STORAGE*> estorage_device;
    device.reserve(8);

    gen_device = get_generators_in_area(area);
    size_t ngens = gen_device.size();
    for(size_t i=0; i!=ngens; ++i)
        device.push_back(gen_device[i]);

    wt_generator_device = get_wt_generators_in_area(area);
    size_t nwt_generator = wt_generator_device.size();
    for(size_t i=0; i!=nwt_generator; ++i)
        device.push_back(wt_generator_device[i]);

    pv_unit_device = get_pv_units_in_area(area);
    size_t npv_unit = pv_unit_device.size();
    for(size_t i=0; i!=npv_unit; ++i)
        device.push_back(pv_unit_device[i]);

    estorage_device = get_energy_storages_in_area(area);
    size_t nes = estorage_device.size();
    for(size_t i=0; i!=nes; ++i)
        device.push_back(estorage_device[i]);

    return device;
}

vector<LOAD*> POWER_SYSTEM_DATABASE::get_loads_in_area(const size_t area)
{
    vector<LOAD*> device;
    device.reserve(8);

    size_t n = get_load_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Load[i].is_in_area(area))
            device.push_back(&(Load[i]));
    }
    return device;
}

vector<LINE*> POWER_SYSTEM_DATABASE::get_lines_in_area(const size_t area)
{
    vector<LINE*> device;
    device.reserve(8);

    size_t n = get_line_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Line[i].is_in_area(area))
            device.push_back(&(Line[i]));
    }
    return device;
}

vector<TRANSFORMER*> POWER_SYSTEM_DATABASE::get_transformers_in_area(const size_t area)
{
    vector<TRANSFORMER*> device;
    device.reserve(8);

    size_t n = get_transformer_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Transformer[i].is_in_area(area))
            device.push_back(&(Transformer[i]));
    }
    return device;
}

vector<FIXED_SHUNT*> POWER_SYSTEM_DATABASE::get_fixed_shunts_in_area(const size_t area)
{
    vector<FIXED_SHUNT*> device;
    device.reserve(8);

    size_t n = get_fixed_shunt_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Fixed_shunt[i].is_in_area(area))
            device.push_back(&(Fixed_shunt[i]));
    }
    return device;
}

vector<HVDC*> POWER_SYSTEM_DATABASE::get_hvdcs_in_area(const size_t area)
{
    vector<HVDC*> device;
    device.reserve(8);

    size_t n = get_hvdc_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Hvdc[i].is_in_area(area))
            device.push_back(&(Hvdc[i]));
    }
    return device;
}

vector<EQUIVALENT_DEVICE*> POWER_SYSTEM_DATABASE::get_equivalent_devices_in_area(const size_t area)
{
    vector<EQUIVALENT_DEVICE*> device;
    device.reserve(8);

    size_t n = get_equivalent_device_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Equivalent_device[i].is_in_area(area))
            device.push_back(&(Equivalent_device[i]));
    }
    return device;
}

vector<ENERGY_STORAGE*> POWER_SYSTEM_DATABASE::get_energy_storages_in_area(const size_t area)
{
    vector<ENERGY_STORAGE*> device;
    device.reserve(8);

    size_t n = get_energy_storage_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Energy_storage[i].is_in_area(area))
            device.push_back(&(Energy_storage[i]));
    }
    return device;
}



vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_all_devices_device_id_in_area(const size_t area)
{
    vector<DEVICE*> devices = get_all_devices_in_area(area);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_buses_device_id_in_area(const size_t area)
{
    vector<BUS*> devices = get_buses_in_area(area);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_generators_device_id_in_area(const size_t area)
{
    vector<GENERATOR*> devices = get_generators_in_area(area);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_wt_generators_device_id_in_area(const size_t area)
{
    vector<WT_GENERATOR*> devices = get_wt_generators_in_area(area);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_pv_units_device_id_in_area(const size_t area)
{
    vector<PV_UNIT*> devices = get_pv_units_in_area(area);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_sources_device_id_in_area(const size_t area)
{
    vector<SOURCE*> devices = get_sources_in_area(area);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_loads_device_id_in_area(const size_t area)
{
    vector<LOAD*> devices = get_loads_in_area(area);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_lines_device_id_in_area(const size_t area)
{
    vector<LINE*> devices = get_lines_in_area(area);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_transformers_device_id_in_area(const size_t area)
{
    vector<TRANSFORMER*> devices = get_transformers_in_area(area);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_fixed_shunts_device_id_in_area(const size_t area)
{
    vector<FIXED_SHUNT*> devices = get_fixed_shunts_in_area(area);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_hvdcs_device_id_in_area(const size_t area)
{
    vector<HVDC*> devices = get_hvdcs_in_area(area);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_equivalent_devices_device_id_in_area(const size_t area)
{
    vector<EQUIVALENT_DEVICE*> devices = get_equivalent_devices_in_area(area);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_energy_storages_device_id_in_area(const size_t area)
{
    vector<ENERGY_STORAGE*> devices = get_energy_storages_in_area(area);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE*> POWER_SYSTEM_DATABASE::get_all_devices_in_zone(const size_t zone)
{
    vector<DEVICE*> devices;
    size_t n=0;

    vector<BUS*> buses = get_buses_in_zone(zone);
    n = buses.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(buses[i]);

    vector<SOURCE*> sources = get_sources_in_zone(zone);
    n = sources.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(sources[i]);

    vector<LOAD*> loads = get_loads_in_zone(zone);
    n = loads.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(loads[i]);

    vector<LINE*> lines = get_lines_in_zone(zone);
    n = lines.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(lines[i]);

    vector<TRANSFORMER*> transes = get_transformers_in_zone(zone);
    n = transes.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(transes[i]);

    vector<FIXED_SHUNT*> fshunts = get_fixed_shunts_in_zone(zone);
    n = fshunts.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(fshunts[i]);

    vector<HVDC*> hvdcs = get_hvdcs_in_zone(zone);
    n = hvdcs.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(hvdcs[i]);

    vector<EQUIVALENT_DEVICE*> edevices = get_equivalent_devices_in_zone(zone);
    n = edevices.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(edevices[i]);

    return devices;
}

vector<BUS*> POWER_SYSTEM_DATABASE::get_buses_in_zone(const size_t zone)
{
    vector<BUS*> device;
    device.reserve(8);

    size_t n = get_bus_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Bus[i].is_in_zone(zone))
            device.push_back(&(Bus[i]));
    }
    return device;
}

vector<GENERATOR*> POWER_SYSTEM_DATABASE::get_generators_in_zone(const size_t zone)
{
    vector<GENERATOR*> device;
    device.reserve(8);

    size_t n = get_generator_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Generator[i].is_in_zone(zone))
            device.push_back(&(Generator[i]));
    }
    return device;
}

vector<WT_GENERATOR*> POWER_SYSTEM_DATABASE::get_wt_generators_in_zone(const size_t zone)
{
    vector<WT_GENERATOR*> device;
    device.reserve(8);

    size_t n = get_wt_generator_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(WT_Generator[i].is_in_zone(zone))
            device.push_back(&(WT_Generator[i]));
    }
    return device;
}

vector<PV_UNIT*> POWER_SYSTEM_DATABASE::get_pv_units_in_zone(const size_t zone)
{
    vector<PV_UNIT*> device;
    device.reserve(8);

    size_t n = get_pv_unit_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(PV_Unit[i].is_in_zone(zone))
            device.push_back(&(PV_Unit[i]));
    }
    return device;
}

vector<SOURCE*> POWER_SYSTEM_DATABASE::get_sources_in_zone(const size_t zone)
{
    vector<SOURCE*> device;
    vector<GENERATOR*> gen_device;
    vector<WT_GENERATOR*> wt_generator_device;
    vector<PV_UNIT*> pv_unit_device;
    vector<ENERGY_STORAGE*> estorage_device;
    device.reserve(8);

    gen_device = get_generators_in_zone(zone);
    size_t ngens = gen_device.size();
    for(size_t i=0; i!=ngens; ++i)
        device.push_back(gen_device[i]);

    wt_generator_device = get_wt_generators_in_zone(zone);
    size_t nwt_generator = wt_generator_device.size();
    for(size_t i=0; i!=nwt_generator; ++i)
        device.push_back(wt_generator_device[i]);

    pv_unit_device = get_pv_units_in_zone(zone);
    size_t npv_unit = pv_unit_device.size();
    for(size_t i=0; i!=npv_unit; ++i)
        device.push_back(pv_unit_device[i]);

    estorage_device = get_energy_storages_in_zone(zone);
    size_t nes = estorage_device.size();
    for(size_t i=0; i!=nes; ++i)
        device.push_back(estorage_device[i]);

    return device;
}

vector<LOAD*> POWER_SYSTEM_DATABASE::get_loads_in_zone(const size_t zone)
{
    vector<LOAD*> device;
    device.reserve(8);

    size_t n = get_load_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Load[i].is_in_zone(zone))
            device.push_back(&(Load[i]));
    }
    return device;
}

vector<LINE*> POWER_SYSTEM_DATABASE::get_lines_in_zone(const size_t zone)
{
    vector<LINE*> device;
    device.reserve(8);

    size_t n = get_line_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Line[i].is_in_zone(zone))
            device.push_back(&(Line[i]));
    }
    return device;
}

vector<TRANSFORMER*> POWER_SYSTEM_DATABASE::get_transformers_in_zone(const size_t zone)
{
    vector<TRANSFORMER*> device;
    device.reserve(8);

    size_t n = get_transformer_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Transformer[i].is_in_zone(zone))
            device.push_back(&(Transformer[i]));
    }
    return device;
}

vector<FIXED_SHUNT*> POWER_SYSTEM_DATABASE::get_fixed_shunts_in_zone(const size_t zone)
{
    vector<FIXED_SHUNT*> device;
    device.reserve(8);

    size_t n = get_fixed_shunt_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Fixed_shunt[i].is_in_zone(zone))
            device.push_back(&(Fixed_shunt[i]));
    }
    return device;
}

vector<HVDC*> POWER_SYSTEM_DATABASE::get_hvdcs_in_zone(const size_t zone)
{
    vector<HVDC*> device;
    device.reserve(8);

    size_t n = get_hvdc_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Hvdc[i].is_in_zone(zone))
            device.push_back(&(Hvdc[i]));
    }
    return device;
}

vector<EQUIVALENT_DEVICE*> POWER_SYSTEM_DATABASE::get_equivalent_devices_in_zone(const size_t zone)
{
    vector<EQUIVALENT_DEVICE*> device;
    device.reserve(8);

    size_t n = get_equivalent_device_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Equivalent_device[i].is_in_zone(zone))
            device.push_back(&(Equivalent_device[i]));
    }
    return device;
}

vector<ENERGY_STORAGE*> POWER_SYSTEM_DATABASE::get_energy_storages_in_zone(const size_t zone)
{
    vector<ENERGY_STORAGE*> device;
    device.reserve(8);

    size_t n = get_energy_storage_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Energy_storage[i].is_in_zone(zone))
            device.push_back(&(Energy_storage[i]));
    }
    return device;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_all_devices_device_id_in_zone(const size_t zone)
{
    vector<DEVICE*> devices = get_all_devices_in_zone(zone);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_buses_device_id_in_zone(const size_t zone)
{
    vector<BUS*> devices = get_buses_in_zone(zone);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_generators_device_id_in_zone(const size_t zone)
{
    vector<GENERATOR*> devices = get_generators_in_zone(zone);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_wt_generators_device_id_in_zone(const size_t zone)
{
    vector<WT_GENERATOR*> devices = get_wt_generators_in_zone(zone);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_pv_units_device_id_in_zone(const size_t zone)
{
    vector<PV_UNIT*> devices = get_pv_units_in_zone(zone);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_sources_device_id_in_zone(const size_t zone)
{
    vector<SOURCE*> devices = get_sources_in_zone(zone);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_loads_device_id_in_zone(const size_t zone)
{
    vector<LOAD*> devices = get_loads_in_zone(zone);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_lines_device_id_in_zone(const size_t zone)
{
    vector<LINE*> devices = get_lines_in_zone(zone);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_transformers_device_id_in_zone(const size_t zone)
{
    vector<TRANSFORMER*> devices = get_transformers_in_zone(zone);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_fixed_shunts_device_id_in_zone(const size_t zone)
{
    vector<FIXED_SHUNT*> devices = get_fixed_shunts_in_zone(zone);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_hvdcs_device_id_in_zone(const size_t zone)
{
    vector<HVDC*> devices = get_hvdcs_in_zone(zone);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_equivalent_devices_device_id_in_zone(const size_t zone)
{
    vector<EQUIVALENT_DEVICE*> devices = get_equivalent_devices_in_zone(zone);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_energy_storages_device_id_in_zone(const size_t zone)
{
    vector<ENERGY_STORAGE*> devices = get_energy_storages_in_zone(zone);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}


vector<DEVICE*> POWER_SYSTEM_DATABASE::get_all_devices()
{
    vector<DEVICE*> devices;
    size_t n=0;

    vector<BUS*> buses = get_all_buses();
    n = buses.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(buses[i]);

    vector<SOURCE*> sources = get_all_sources();
    n = sources.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(sources[i]);

    vector<LOAD*> loads = get_all_loads();
    n = loads.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(loads[i]);

    vector<FIXED_SHUNT*> fshunts = get_all_fixed_shunts();
    n = fshunts.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(fshunts[i]);

    vector<LINE*> lines = get_all_lines();
    n = lines.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(lines[i]);

    vector<TRANSFORMER*> transes = get_all_transformers();
    n = transes.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(transes[i]);


    vector<HVDC*> hvdcs = get_all_hvdcs();
    n = hvdcs.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(hvdcs[i]);

    vector<EQUIVALENT_DEVICE*> edevices = get_all_equivalent_devices();
    n = edevices.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(edevices[i]);

    return devices;
}

vector<BUS*> POWER_SYSTEM_DATABASE::get_all_buses()
{
    vector<BUS*> device;
    size_t n = get_bus_count();
    device.reserve(n);
    for(size_t i=0; i!=n; ++i)
        device.push_back(&(Bus[i]));
    return device;
}

vector<BUS*> POWER_SYSTEM_DATABASE::get_buses_with_constraints(double vbase_kV_min, double vbase_kV_max, double v_pu_min, double v_pu_max, size_t area, size_t zone, size_t owner)
{
    vector<BUS*> device;
    size_t n = get_bus_count();
    device.reserve(n);
    double vbase=0.0, v=0.0;
    size_t this_area=0, this_zone=0, this_owner=0;
    for(size_t i=0; i!=n; ++i)
    {
        vbase = Bus[i].get_base_voltage_in_kV();
        v = Bus[i].get_voltage_in_pu();
        this_area = Bus[i].get_area_number();
        this_zone = Bus[i].get_zone_number();
        this_owner = Bus[i].get_owner_number();

        if(area!=0 and this_area!=area)
            continue;
        if(zone!=0 and this_zone!=zone)
            continue;
        if(owner!=0 and this_owner!=owner)
            continue;
        if(vbase>=vbase_kV_min and vbase<=vbase_kV_max and v>=v_pu_min and v<=v_pu_max)
            device.push_back(&(Bus[i]));
    }
    return device;
}

vector<BUS*> POWER_SYSTEM_DATABASE::get_all_in_service_buses()
{
    vector<BUS*> device;
    size_t n = get_bus_count();
    device.reserve(n);
    for(size_t i=0; i!=n; ++i)
    {
        if(Bus[i].get_bus_type()!=OUT_OF_SERVICE)
            device.push_back(&(Bus[i]));
    }
    return device;
}

vector<GENERATOR*> POWER_SYSTEM_DATABASE::get_all_generators()
{
    vector<GENERATOR*> device;
    size_t n = get_generator_count();
    device.reserve(n);

    for(size_t i=0; i!=n; ++i)
        device.push_back(&(Generator[i]));
    return device;
}

vector<WT_GENERATOR*> POWER_SYSTEM_DATABASE::get_all_wt_generators()
{
    vector<WT_GENERATOR*> device;
    size_t n = get_wt_generator_count();
    device.reserve(n);
    for(size_t i=0; i!=n; ++i)
        device.push_back(&(WT_Generator[i]));
    return device;
}

vector<PV_UNIT*> POWER_SYSTEM_DATABASE::get_all_pv_units()
{
    vector<PV_UNIT*> device;
    size_t n = get_pv_unit_count();
    device.reserve(n);
    for(size_t i=0; i!=n; ++i)
        device.push_back(&(PV_Unit[i]));
    return device;
}

vector<SOURCE*> POWER_SYSTEM_DATABASE::get_all_sources()
{
    vector<SOURCE*> device;
    vector<GENERATOR*> gen_device;
    vector<WT_GENERATOR*> wt_generator_device;
    vector<PV_UNIT*> pv_unit_device;
    vector<ENERGY_STORAGE*> estorage_device;
    device.reserve(get_source_count());

    gen_device = get_all_generators();
    size_t ngens = gen_device.size();
    for(size_t i=0; i!=ngens; ++i)
        device.push_back(gen_device[i]);

    wt_generator_device = get_all_wt_generators();
    size_t nwt_generator = wt_generator_device.size();
    for(size_t i=0; i!=nwt_generator; ++i)
        device.push_back(wt_generator_device[i]);

    pv_unit_device = get_all_pv_units();
    size_t npv_unit = pv_unit_device.size();
    for(size_t i=0; i!=npv_unit; ++i)
        device.push_back(pv_unit_device[i]);

    estorage_device = get_all_energy_storages();
    size_t nes = estorage_device.size();
    for(size_t i=0; i!=nes; ++i)
        device.push_back(estorage_device[i]);

    return device;
}

vector<LOAD*> POWER_SYSTEM_DATABASE::get_all_loads()
{
    vector<LOAD*> device;
    size_t n = get_load_count();
    device.reserve(n);
    for(size_t i=0; i!=n; ++i)
        device.push_back(&(Load[i]));
    return device;
}

vector<LINE*> POWER_SYSTEM_DATABASE::get_all_lines()
{
    vector<LINE*> device;
    size_t n = get_line_count();
    device.reserve(n);
    for(size_t i=0; i!=n; ++i)
        device.push_back(&(Line[i]));
    return device;
}

vector<TRANSFORMER*> POWER_SYSTEM_DATABASE::get_all_transformers()
{
    vector<TRANSFORMER*> device;
    size_t n = get_transformer_count();
    device.reserve(n);
    for(size_t i=0; i!=n; ++i)
        device.push_back(&(Transformer[i]));
    return device;
}

vector<FIXED_SHUNT*> POWER_SYSTEM_DATABASE::get_all_fixed_shunts()
{
    vector<FIXED_SHUNT*> device;
    size_t n = get_fixed_shunt_count();
    device.reserve(n);
    for(size_t i=0; i!=n; ++i)
        device.push_back(&(Fixed_shunt[i]));
    return device;
}

vector<HVDC*> POWER_SYSTEM_DATABASE::get_all_hvdcs()
{
    vector<HVDC*> device;
    size_t n = get_hvdc_count();
    device.reserve(n);
    for(size_t i=0; i!=n; ++i)
        device.push_back(&(Hvdc[i]));
    return device;
}

vector<EQUIVALENT_DEVICE*> POWER_SYSTEM_DATABASE::get_all_equivalent_devices()
{
    vector<EQUIVALENT_DEVICE*> device;
    size_t n = get_equivalent_device_count();
    device.reserve(n);
    for(size_t i=0; i!=n; ++i)
        device.push_back(&(Equivalent_device[i]));
    return device;
}

vector<ENERGY_STORAGE*> POWER_SYSTEM_DATABASE::get_all_energy_storages()
{
    vector<ENERGY_STORAGE*> device;
    size_t n = get_energy_storage_count();
    device.reserve(n);
    for(size_t i=0; i!=n; ++i)
        device.push_back(&(Energy_storage[i]));
    return device;
}

vector<AREA*> POWER_SYSTEM_DATABASE::get_all_areas()
{
    vector<AREA*> areas;
    areas.reserve(get_area_count());

    size_t n = get_area_count();
    for(size_t i=0; i!=n; ++i)
        areas.push_back(&(Area[i]));
    return areas;
}

vector<ZONE*> POWER_SYSTEM_DATABASE::get_all_zones()
{
    vector<ZONE*> zones;
    zones.reserve(get_zone_count());

    size_t n = get_zone_count();
    for(size_t i=0; i!=n; ++i)
        zones.push_back(&(Zone[i]));
    return zones;
}

vector<OWNER*> POWER_SYSTEM_DATABASE::get_all_owners()
{
    vector<OWNER*> owners;
    owners.reserve(get_owner_count());

    size_t n = get_owner_count();
    for(size_t i=0; i!=n; ++i)
        owners.push_back(&(Owner[i]));
    return owners;
}



vector<size_t> POWER_SYSTEM_DATABASE::get_all_buses_number()
{
    vector<size_t> buses_number;
    size_t n=get_bus_count();
    buses_number.reserve(n);
    for(size_t i=0; i!=n; ++i)
        buses_number.push_back(Bus[i].get_bus_number());
    return buses_number;
}

vector<size_t> POWER_SYSTEM_DATABASE::get_buses_number_with_constraints(double vbase_kV_min, double vbase_kV_max, double v_pu_min, double v_pu_max, size_t area, size_t zone, size_t owner)
{
    vector<size_t> buses_number;
    size_t n = get_bus_count();
    buses_number.reserve(n);
    double vbase=0.0, v=0.0;
    size_t this_area=0, this_zone=0, this_owner=0;
    for(size_t i=0; i!=n; ++i)
    {
        vbase = Bus[i].get_base_voltage_in_kV();
        v = Bus[i].get_voltage_in_pu();
        this_area = Bus[i].get_area_number();
        this_zone = Bus[i].get_zone_number();
        this_owner = Bus[i].get_owner_number();

        if(area!=0 and this_area!=area)
            continue;
        if(zone!=0 and this_zone!=zone)
            continue;
        if(owner!=0 and this_owner!=owner)
            continue;
        if(vbase>=vbase_kV_min and vbase<=vbase_kV_max and v>=v_pu_min and v<=v_pu_max)
            buses_number.push_back(Bus[i].get_bus_number());
    }
    return buses_number;
}

vector<size_t> POWER_SYSTEM_DATABASE::get_all_in_service_buses_number()
{
    vector<size_t> buses_number;
    size_t n=get_bus_count();
    buses_number.reserve(n);
    for(size_t i=0; i!=n; ++i)
    {
        if(Bus[i].get_bus_type()!=OUT_OF_SERVICE)
            buses_number.push_back(Bus[i].get_bus_number());
    }
    return buses_number;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_all_generators_device_id()
{
    vector<DEVICE_ID> dids;
    size_t n=get_generator_count();
    dids.reserve(n);
    for(size_t i=0; i!=n; ++i)
        dids.push_back(Generator[i].get_device_id());
    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_all_wt_generators_device_id()
{
    vector<DEVICE_ID> dids;
    size_t n=get_wt_generator_count();
    dids.reserve(n);
    for(size_t i=0; i!=n; ++i)
        dids.push_back(WT_Generator[i].get_device_id());
    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_all_pv_units_device_id()
{
    vector<DEVICE_ID> dids;
    size_t n=get_pv_unit_count();
    dids.reserve(n);
    for(size_t i=0; i!=n; ++i)
        dids.push_back(PV_Unit[i].get_device_id());
    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_all_sources_device_id()
{
    vector<DEVICE_ID> dids;
    size_t n=get_source_count();
    dids.reserve(n);
    size_t ngens = get_generator_count();
    for(size_t i=0; i!=ngens; ++i)
        dids.push_back(Generator[i].get_device_id());

    size_t nwt_generator = get_wt_generator_count();
    for(size_t i=0; i!=nwt_generator; ++i)
        dids.push_back(WT_Generator[i].get_device_id());

    size_t npv_unit = get_pv_unit_count();
    for(size_t i=0; i!=npv_unit; ++i)
        dids.push_back(PV_Unit[i].get_device_id());

    size_t nes = get_energy_storage_count();
    for(size_t i=0; i!=nes; ++i)
        dids.push_back(Energy_storage[i].get_device_id());
    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_all_loads_device_id()
{
    vector<DEVICE_ID> dids;
    size_t n=get_load_count();
    dids.reserve(n);
    for(size_t i=0; i!=n; ++i)
        dids.push_back(Load[i].get_device_id());
    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_all_lines_device_id()
{
    vector<DEVICE_ID> dids;
    size_t n=get_line_count();
    dids.reserve(n);
    for(size_t i=0; i!=n; ++i)
        dids.push_back(Line[i].get_device_id());
    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_all_transformers_device_id()
{
    vector<DEVICE_ID> dids;
    size_t n=get_transformer_count();
    dids.reserve(n);
    for(size_t i=0; i!=n; ++i)
        dids.push_back(Transformer[i].get_device_id());
    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_all_fixed_shunts_device_id()
{
    vector<DEVICE_ID> dids;
    size_t n=get_fixed_shunt_count();
    dids.reserve(n);
    for(size_t i=0; i!=n; ++i)
        dids.push_back(Fixed_shunt[i].get_device_id());
    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_all_hvdcs_device_id()
{
    vector<DEVICE_ID> dids;
    size_t n=get_hvdc_count();
    dids.reserve(n);
    for(size_t i=0; i!=n; ++i)
        dids.push_back(Hvdc[i].get_device_id());
    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_all_equivalent_devices_device_id()
{
    vector<DEVICE_ID> dids;
    size_t n=get_equivalent_device_count();
    dids.reserve(n);
    for(size_t i=0; i!=n; ++i)
        dids.push_back(Equivalent_device[i].get_device_id());
    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_all_energy_storages_device_id()
{
    vector<DEVICE_ID> dids;
    size_t n=get_energy_storage_count();
    dids.reserve(n);
    for(size_t i=0; i!=n; ++i)
        dids.push_back(Energy_storage[i].get_device_id());
    return dids;
}

vector<size_t> POWER_SYSTEM_DATABASE::get_all_areas_number()
{
    vector<size_t> areas_number;
    size_t n=get_area_count();
    areas_number.reserve(n);
    for(size_t i=0; i!=n; ++i)
        areas_number.push_back(Area[i].get_area_number());
    return areas_number;
}

vector<size_t> POWER_SYSTEM_DATABASE::get_all_zones_number()
{
    vector<size_t> zones_number;
    size_t n=get_zone_count();
    zones_number.reserve(n);
    for(size_t i=0; i!=n; ++i)
        zones_number.push_back(Zone[i].get_zone_number());
    return zones_number;
}

vector<size_t> POWER_SYSTEM_DATABASE::get_all_owners_number()
{
    vector<size_t> owners_number;
    size_t n=get_owner_count();
    owners_number.reserve(n);
    for(size_t i=0; i!=n; ++i)
        owners_number.push_back(Owner[i].get_owner_number());
    return owners_number;
}


size_t POWER_SYSTEM_DATABASE::get_bus_count() const
{
    return Bus.size();
}

size_t POWER_SYSTEM_DATABASE::get_in_service_bus_count() const
{
    return in_service_bus_count;
}

void POWER_SYSTEM_DATABASE::update_in_service_bus_count()
{
    size_t n = Bus.size();
    size_t n_out_of_service = 0;
    for(size_t i=0; i!=n; ++i)
    {
        if(Bus[i].get_bus_type()==OUT_OF_SERVICE)
			++n_out_of_service;
    }

    in_service_bus_count = n-n_out_of_service;
}

size_t POWER_SYSTEM_DATABASE::get_overshadowed_bus_count() const
{
    return overshadowed_bus_count;
}

void POWER_SYSTEM_DATABASE::set_all_buses_un_overshadowed()
{
    size_t n = Bus.size();
    for(size_t i=0; i!=n; ++i)
        Bus[i].set_equivalent_bus_number(0);
}

void POWER_SYSTEM_DATABASE::update_overshadowed_bus_count()
{
    set_all_buses_un_overshadowed();

    overshadowed_bus_count = 0;
    size_t n = Line.size();
    while(true)
    {
        bool new_bus_is_overshadowed = false;
        for(size_t i=0; i!=n; ++i)
        {
            if(Line[i].is_zero_impedance_line())
            {
                bool istatus = Line[i].get_sending_side_breaker_status();
                bool jstatus = Line[i].get_receiving_side_breaker_status();
                if(istatus==false or jstatus==false)
                    continue;
                else
                {
                    size_t ibus = Line[i].get_sending_side_bus();
                    size_t jbus = Line[i].get_receiving_side_bus();

                    size_t iequvilent_bus = get_equivalent_bus_of_bus(ibus);
                    size_t jequvilent_bus = get_equivalent_bus_of_bus(jbus);

                    if(iequvilent_bus==0)
                        iequvilent_bus = ibus;
                    if(jequvilent_bus==0)
                        jequvilent_bus = jbus;

                    if(iequvilent_bus==jequvilent_bus)
                        continue;
                    else
                    {
                        if(iequvilent_bus<jequvilent_bus)
                        {
                            BUS* busptr = get_bus(jbus);
                            busptr->set_equivalent_bus_number(iequvilent_bus);
                        }
                        else
                        {
                            BUS* busptr = get_bus(ibus);
                            busptr->set_equivalent_bus_number(jequvilent_bus);
                        }
                        if(new_bus_is_overshadowed==false)
                            new_bus_is_overshadowed = true;
                    }
                }
            }
        }
        if(new_bus_is_overshadowed==false)
            break;
    }

    n = Bus.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(Bus[i].is_bus_overshadowed())
            ++overshadowed_bus_count;
    }
}

size_t POWER_SYSTEM_DATABASE::get_equivalent_bus_of_bus(size_t bus)
{
    BUS* busptr = get_bus(bus);
    if(busptr!=NULL)
        return busptr->get_equivalent_bus_number();
    else
        return 0;
}

size_t POWER_SYSTEM_DATABASE::get_generator_count() const
{
    return Generator.size();
}

size_t POWER_SYSTEM_DATABASE::get_wt_generator_count() const
{
    return WT_Generator.size();
}

size_t POWER_SYSTEM_DATABASE::get_pv_unit_count() const
{
    return PV_Unit.size();
}

size_t POWER_SYSTEM_DATABASE::get_source_count() const
{
    size_t n = 0;
    n += get_generator_count();
    n += get_wt_generator_count();
    n += get_pv_unit_count();
    n += get_energy_storage_count();
    return n;
}

size_t POWER_SYSTEM_DATABASE::get_load_count() const
{
    return Load.size();
}

size_t POWER_SYSTEM_DATABASE::get_line_count() const
{
    return Line.size();
}

size_t POWER_SYSTEM_DATABASE::get_transformer_count() const
{
    return Transformer.size();
}

size_t POWER_SYSTEM_DATABASE::get_fixed_shunt_count() const
{
    return Fixed_shunt.size();
}

size_t POWER_SYSTEM_DATABASE::get_hvdc_count() const
{
    return Hvdc.size();
}

size_t POWER_SYSTEM_DATABASE::get_equivalent_device_count() const
{
    return Equivalent_device.size();
}

size_t POWER_SYSTEM_DATABASE::get_energy_storage_count() const
{
    return Energy_storage.size();
}

size_t POWER_SYSTEM_DATABASE::get_area_count() const
{
    return Area.size();
}

size_t POWER_SYSTEM_DATABASE::get_zone_count() const
{
    return Zone.size();
}

size_t POWER_SYSTEM_DATABASE::get_owner_count() const
{
    return Owner.size();
}

size_t POWER_SYSTEM_DATABASE::get_bus_index(size_t bus) const
{
    return bus_index.get_index_of_bus(bus);
}

size_t POWER_SYSTEM_DATABASE::get_generator_index(const DEVICE_ID & device_id) const
{
    return generator_index.get_index_of_device(device_id);
}

size_t POWER_SYSTEM_DATABASE::get_wt_generator_index(const DEVICE_ID & device_id) const
{
    return wt_generator_index.get_index_of_device(device_id);
}

size_t POWER_SYSTEM_DATABASE::get_pv_unit_index(const DEVICE_ID & device_id) const
{
    return pv_unit_index.get_index_of_device(device_id);
}

size_t POWER_SYSTEM_DATABASE::get_load_index(const DEVICE_ID & device_id) const
{
    return load_index.get_index_of_device(device_id);
}

size_t POWER_SYSTEM_DATABASE::get_line_index(const DEVICE_ID & device_id) const
{
    return line_index.get_index_of_device(device_id);
}

size_t POWER_SYSTEM_DATABASE::get_transformer_index(const DEVICE_ID & device_id) const
{
    return transformer_index.get_index_of_device(device_id);
}

size_t POWER_SYSTEM_DATABASE::get_fixed_shunt_index(const DEVICE_ID & device_id) const
{
    return fixed_shunt_index.get_index_of_device(device_id);
}

size_t POWER_SYSTEM_DATABASE::get_hvdc_index(const DEVICE_ID & device_id) const
{
    return hvdc_index.get_index_of_device(device_id);
}

size_t POWER_SYSTEM_DATABASE::get_equivalent_device_index(const DEVICE_ID & device_id) const
{
    return equivalent_device_index.get_index_of_device(device_id);
}

size_t POWER_SYSTEM_DATABASE::get_energy_storage_index(const DEVICE_ID & device_id) const
{
    return energy_storage_index.get_index_of_device(device_id);
}

size_t POWER_SYSTEM_DATABASE::get_area_index(const size_t no) const
{
    map<size_t, size_t>::const_iterator iter = area_index.begin();
    iter = area_index.find(no);
    if(iter==area_index.end()) return INDEX_NOT_EXIST;
    else                       return iter->second;
}

size_t POWER_SYSTEM_DATABASE::get_zone_index(const size_t no) const
{
    map<size_t, size_t>::const_iterator iter = zone_index.begin();
    iter = zone_index.find(no);
    if(iter==zone_index.end()) return INDEX_NOT_EXIST;
    else                       return iter->second;
}

size_t POWER_SYSTEM_DATABASE::get_owner_index(const size_t no) const
{
    map<size_t, size_t>::const_iterator iter = owner_index.begin();
    iter = owner_index.find(no);
    if(iter==owner_index.end()) return INDEX_NOT_EXIST;
    else                        return iter->second;
}

size_t POWER_SYSTEM_DATABASE::bus_name2bus_number(const string&name) const
{
    string trimmed_name = string2upper(trim_string(name));
    size_t n = get_bus_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Bus[i].get_bus_name()==trimmed_name)
            return Bus[i].get_bus_number();
    }
    return 0;
}

size_t POWER_SYSTEM_DATABASE::area_name2area_number(const string& name) const
{
    string trimmed_name = trim_string(name);
    size_t n = get_area_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Area[i].get_area_name()==trimmed_name)
            return Area[i].get_area_number();
    }
    return 0;
}

size_t POWER_SYSTEM_DATABASE::zone_name2zone_number(const string& name) const
{
    string trimmed_name = trim_string(name);
    size_t n = get_zone_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Zone[i].get_zone_name()==trimmed_name)
            return Zone[i].get_zone_number();
    }
    return 0;
}

size_t POWER_SYSTEM_DATABASE::owner_name2owner_number(const string& name) const
{
    string trimmed_name = trim_string(name);
    size_t n = get_owner_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Owner[i].get_owner_name()==trimmed_name)
            return Owner[i].get_owner_number();
    }
    return 0;
}

string POWER_SYSTEM_DATABASE::bus_number2bus_name(size_t number)
{
    if(not is_bus_in_allowed_range(number))
        return "";

    BUS* bus = get_bus(number);
    if(bus==NULL)
        return "";
    else
        return bus->get_bus_name();
}


string POWER_SYSTEM_DATABASE::area_number2area_name(size_t number)
{
    AREA* area = get_area(number);
    if(area==NULL)
        return "";
    else
        return area->get_area_name();
}

string POWER_SYSTEM_DATABASE::zone_number2zone_name(size_t number)
{
    ZONE* zone = get_zone(number);
    if(zone==NULL)
        return "";
    else
        return zone->get_zone_name();
}

string POWER_SYSTEM_DATABASE::owner_number2owner_name(size_t number)
{
    OWNER* owner = get_owner(number);
    if(owner==NULL)
        return "";
    else
        return owner->get_owner_name();
}

void POWER_SYSTEM_DATABASE::check_database()
{
    check_all_devices();
    check_all_areas();
    check_all_zones();
    check_all_owners();
}

void POWER_SYSTEM_DATABASE::check_all_devices()
{
    check_all_buses();
    check_all_sources();
    check_all_loads();
    check_all_fixed_shunts();
    check_all_lines();
    check_all_transformers();
    check_all_hvdcs();
    check_all_equivalent_devices();
    check_all_energy_storages();
}

void POWER_SYSTEM_DATABASE::check_all_buses()
{
    size_t n = get_bus_count();
    for(size_t i=0; i!=n; ++i)
        Bus[i].check();
}

void POWER_SYSTEM_DATABASE::check_all_sources()
{
    check_all_generators();
    check_all_wt_generators();
    check_all_pv_units();
    check_all_energy_storages();
}

void POWER_SYSTEM_DATABASE::check_all_generators()
{
    size_t n = get_generator_count();
    for(size_t i=0; i!=n; ++i)
        Generator[i].check();
}

void POWER_SYSTEM_DATABASE::check_all_wt_generators()
{
    size_t n = get_wt_generator_count();
    for(size_t i=0; i!=n; ++i)
        WT_Generator[i].check();
}

void POWER_SYSTEM_DATABASE::check_all_pv_units()
{
    size_t n = get_pv_unit_count();
    for(size_t i=0; i!=n; ++i)
        PV_Unit[i].check();
}

void POWER_SYSTEM_DATABASE::check_all_loads()
{
    size_t n = get_load_count();
    for(size_t i=0; i!=n; ++i)
        Load[i].check();
}

void POWER_SYSTEM_DATABASE::check_all_fixed_shunts()
{
    size_t n = get_fixed_shunt_count();
    for(size_t i=0; i!=n; ++i)
        Fixed_shunt[i].check();
}

void POWER_SYSTEM_DATABASE::check_all_lines()
{
    size_t n = get_line_count();
    for(size_t i=0; i!=n; ++i)
        Line[i].check();
}

void POWER_SYSTEM_DATABASE::check_all_transformers()
{
    size_t n = get_transformer_count();
    for(size_t i=0; i!=n; ++i)
        Transformer[i].check();
}

void POWER_SYSTEM_DATABASE::check_all_hvdcs()
{
    size_t n = get_hvdc_count();
    for(size_t i=0; i!=n; ++i)
        Hvdc[i].check();
}

void POWER_SYSTEM_DATABASE::check_all_equivalent_devices()
{
    size_t n = get_equivalent_device_count();
    for(size_t i=0; i!=n; ++i)
        Equivalent_device[i].check();
}

void POWER_SYSTEM_DATABASE::check_all_energy_storages()
{
    size_t n = get_energy_storage_count();
    for(size_t i=0; i!=n; ++i)
        Energy_storage[i].check();
}

void POWER_SYSTEM_DATABASE::check_all_areas()
{
    size_t n = get_area_count();
    for(size_t i=0; i!=n; ++i)
        Area[i].check();
}

void POWER_SYSTEM_DATABASE::check_all_zones()
{
    size_t n = get_zone_count();
    for(size_t i=0; i!=n; ++i)
        Zone[i].check();
}

void POWER_SYSTEM_DATABASE::check_all_owners()
{
    size_t n = get_owner_count();
    for(size_t i=0; i!=n; ++i)
        Owner[i].check();
}


void POWER_SYSTEM_DATABASE::check_dynamic_data()
{
    check_generator_related_dynamic_data();
    check_wt_generator_related_dynamic_data();
    check_pv_unit_related_dynamic_data();
    check_load_related_dynamic_data();
    check_hvdc_related_dynamic_data();
    check_energy_storage_related_dynamic_data();
    check_equivalent_device_related_dynamic_data();
}

void POWER_SYSTEM_DATABASE::check_generator_related_dynamic_data()
{
    vector<GENERATOR*> generators = get_all_generators();
    size_t n = generators.size();
    GENERATOR* generator;
    for(size_t i=0; i!=n; ++i)
    {
        generator = generators[i];

        SYNC_GENERATOR_MODEL* genmodel = generator->get_sync_generator_model();
        if(genmodel!=NULL)
            genmodel->check();

        EXCITER_MODEL* avrmodel = generator->get_exciter_model();
        if(avrmodel!=NULL)
            avrmodel->check();

        STABILIZER_MODEL* pssmodel = generator->get_stabilizer_model();
        if(pssmodel!=NULL)
            pssmodel->check();

        COMPENSATOR_MODEL* compmodel = generator->get_compensator_model();
        if(compmodel!=NULL)
            compmodel->check();

        TURBINE_GOVERNOR_MODEL* govmodel = generator->get_turbine_governor_model();
        if(govmodel!=NULL)
            govmodel->check();
    }
}

void POWER_SYSTEM_DATABASE::check_wt_generator_related_dynamic_data()
{
    vector<WT_GENERATOR*> generators = get_all_wt_generators();
    size_t n = generators.size();
    WT_GENERATOR* generator;
    for(size_t i=0; i!=n; ++i)
    {
        generator = generators[i];

        WT_GENERATOR_MODEL* genmodel = generator->get_wt_generator_model();
        if(genmodel!=NULL)
            genmodel->check();

        WT_ELECTRICAL_MODEL* elecmodel = generator->get_wt_electrical_model();
        if(elecmodel!=NULL)
            elecmodel->check();

        WT_AERODYNAMIC_MODEL* aerdmodel = generator->get_wt_aerodynamic_model();
        if(aerdmodel!=NULL)
            aerdmodel->check();

        WT_TURBINE_MODEL* turbmodel = generator->get_wt_turbine_model();
        if(turbmodel!=NULL)
            turbmodel->check();

        WT_PITCH_MODEL* pitchmodel = generator->get_wt_pitch_model();
        if(pitchmodel!=NULL)
            pitchmodel->check();

        WIND_SPEED_MODEL* windmodel = generator->get_wind_speed_model();
        if(windmodel!=NULL)
            windmodel->check();
    }
}

void POWER_SYSTEM_DATABASE::check_pv_unit_related_dynamic_data()
{
    ;
}

void POWER_SYSTEM_DATABASE::check_load_related_dynamic_data()
{
    vector<LOAD*> loads = get_all_loads();
    size_t n = loads.size();
    LOAD* load;
    for(size_t i=0; i!=n; ++i)
    {
        load = loads[i];

        LOAD_MODEL* loadmodel = load->get_load_model();
        if(loadmodel!=NULL)
            loadmodel->check();

        LOAD_VOLTAGE_RELAY_MODEL* uvlsmodel = load->get_load_voltage_relay_model();
        if(uvlsmodel!=NULL)
            uvlsmodel->check();

        LOAD_FREQUENCY_RELAY_MODEL* uflsmodel = load->get_load_frequency_relay_model();
        if(uflsmodel!=NULL)
            uflsmodel->check();
    }
}

void POWER_SYSTEM_DATABASE::check_hvdc_related_dynamic_data()
{
    vector<HVDC*> hvdcs = get_all_hvdcs();
    size_t n = hvdcs.size();
    HVDC* hvdc;
    for(size_t i=0; i!=n; ++i)
    {
        hvdc = hvdcs[i];

        HVDC_MODEL* hvdcmodel = hvdc->get_hvdc_model();
        if(hvdcmodel!=NULL)
            hvdcmodel->check();
    }
}

void POWER_SYSTEM_DATABASE::check_energy_storage_related_dynamic_data()
{
    vector<ENERGY_STORAGE*> estorages = get_all_energy_storages();
    size_t n = estorages.size();
    ENERGY_STORAGE* estorage;
    for(size_t i=0; i!=n; ++i)
    {
        estorage = estorages[i];

        ENERGY_STORAGE_MODEL* esmodel = estorage->get_energy_storage_model();
        if(esmodel!=NULL)
            esmodel->check();
    }
}

void POWER_SYSTEM_DATABASE::check_equivalent_device_related_dynamic_data()
{
    vector<EQUIVALENT_DEVICE*> edevices = get_all_equivalent_devices();
    size_t n = edevices.size();
    EQUIVALENT_DEVICE* edevice;
    for(size_t i=0; i!=n; ++i)
    {
        edevice = edevices[i];

        EQUIVALENT_MODEL* eqmodel = edevice->get_equivalent_model();
        if(eqmodel!=NULL)
            eqmodel->check();
    }
}

void POWER_SYSTEM_DATABASE::scale_load_power(const DEVICE_ID& did, double scale)
{
    LOAD* load = get_load(did);
    if(load!=NULL)
    {
        scale += 1.0;

        load->set_nominal_constant_power_load_in_MVA(load->get_nominal_constant_power_load_in_MVA()*scale);
        load->set_nominal_constant_current_load_in_MVA(load->get_nominal_constant_current_load_in_MVA()*scale);
        load->set_nominal_constant_impedance_load_in_MVA(load->get_nominal_constant_impedance_load_in_MVA()*scale);
    }
}


void POWER_SYSTEM_DATABASE::scale_all_loads_power(double scale)
{
    vector<DEVICE_ID> loads = get_all_loads_device_id();
    size_t n = loads.size();
    for(size_t i=0; i!=n; ++i)
        scale_load_power(loads[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_loads_power_at_bus(size_t bus, double scale)
{
    vector<DEVICE_ID> loads = get_loads_device_id_connecting_to_bus(bus);
    size_t n = loads.size();
    for(size_t i=0; i!=n; ++i)
        scale_load_power(loads[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_loads_power_in_area(size_t area_number, double scale)
{
    vector<DEVICE_ID> loads = get_loads_device_id_in_area(area_number);
    size_t n = loads.size();
    for(size_t i=0; i!=n; ++i)
        scale_load_power(loads[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_loads_power_in_zone(size_t zone_number, double scale)
{
    vector<DEVICE_ID> loads = get_loads_device_id_in_zone(zone_number);
    size_t n = loads.size();
    for(size_t i=0; i!=n; ++i)
        scale_load_power(loads[i], scale);
}



void POWER_SYSTEM_DATABASE::scale_source_power(const DEVICE_ID& did, double scale)
{
    SOURCE* source = get_source(did);
    if(source!=NULL)
    {
        scale += 1.0;

        double P = source->get_p_generation_in_MW();
        double Q = source->get_q_generation_in_MVar();

        P *= scale;
        double pmax = source->get_p_max_in_MW();
        double pmin = source->get_p_min_in_MW();
        if(P>pmax)
            source->set_p_generation_in_MW(pmax);
        else
        {
            if(P<pmin)
                source->set_p_generation_in_MW(pmin);
            else
                source->set_p_generation_in_MW(P);
        }

        Q *= scale;
        double qmax = source->get_q_max_in_MVar();
        double qmin = source->get_q_min_in_MVar();
        if(Q>qmax)
            source->set_q_generation_in_MVar(qmax);
        else
        {
            if(Q<qmin)
                source->set_q_generation_in_MVar(qmin);
            else
                source->set_q_generation_in_MVar(Q);
        }
    }
}

void POWER_SYSTEM_DATABASE::scale_all_sources_power(double scale)
{
    vector<DEVICE_ID> sources = get_all_sources_device_id();
    size_t n = sources.size();
    for(size_t i=0; i!=n; ++i)
        scale_source_power(sources[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_sources_power_at_bus(size_t bus, double scale)
{
    vector<DEVICE_ID> sources = get_sources_device_id_connecting_to_bus(bus);
    size_t n = sources.size();
    for(size_t i=0; i!=n; ++i)
        scale_source_power(sources[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_sources_power_in_area(size_t area_number, double scale)
{
    vector<DEVICE_ID> sources = get_sources_device_id_in_area(area_number);
    size_t n = sources.size();
    for(size_t i=0; i!=n; ++i)
        scale_source_power(sources[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_sources_power_in_zone(size_t zone_number, double scale)
{
    vector<DEVICE_ID> sources = get_sources_device_id_in_zone(zone_number);
    size_t n = sources.size();
    for(size_t i=0; i!=n; ++i)
        scale_source_power(sources[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_generator_power(const DEVICE_ID& did, double scale)
{
    scale_source_power(did, scale);
}

void POWER_SYSTEM_DATABASE::scale_all_generators_power(double scale)
{
    vector<DEVICE_ID> generators = get_all_generators_device_id();
    size_t n = generators.size();
    for(size_t i=0; i!=n; ++i)
        scale_generator_power(generators[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_generators_power_at_bus(size_t bus, double scale)
{
    vector<DEVICE_ID> generators = get_generators_device_id_connecting_to_bus(bus);
    size_t n = generators.size();
    for(size_t i=0; i!=n; ++i)
        scale_generator_power(generators[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_generators_power_in_area(size_t area_number, double scale)
{
    vector<DEVICE_ID> generators = get_generators_device_id_in_area(area_number);
    size_t n = generators.size();
    for(size_t i=0; i!=n; ++i)
        scale_generator_power(generators[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_generators_power_in_zone(size_t zone_number, double scale)
{
    vector<DEVICE_ID> generators = get_generators_device_id_in_zone(zone_number);
    size_t n = generators.size();
    for(size_t i=0; i!=n; ++i)
        scale_generator_power(generators[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_wt_generator_power(const DEVICE_ID& did, double scale)
{
    scale_source_power(did, scale);
}

void POWER_SYSTEM_DATABASE::scale_all_wt_generators_power(double scale)
{
    vector<DEVICE_ID> wt_generators = get_all_wt_generators_device_id();
    size_t n = wt_generators.size();
    for(size_t i=0; i!=n; ++i)
        scale_wt_generator_power(wt_generators[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_wt_generators_power_at_bus(size_t bus, double scale)
{
    vector<DEVICE_ID> wt_generators = get_wt_generators_device_id_connecting_to_bus(bus);
    size_t n = wt_generators.size();
    for(size_t i=0; i!=n; ++i)
        scale_wt_generator_power(wt_generators[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_wt_generators_power_in_area(size_t area_number, double scale)
{
    vector<DEVICE_ID> wt_generators = get_wt_generators_device_id_in_area(area_number);
    size_t n = wt_generators.size();
    for(size_t i=0; i!=n; ++i)
        scale_wt_generator_power(wt_generators[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_wt_generators_power_in_zone(size_t zone_number, double scale)
{
    vector<DEVICE_ID> wt_generators = get_wt_generators_device_id_in_zone(zone_number);
    size_t n = wt_generators.size();
    for(size_t i=0; i!=n; ++i)
        scale_wt_generator_power(wt_generators[i], scale);
}


void POWER_SYSTEM_DATABASE::scale_pv_unit_power(const DEVICE_ID& did, double scale)
{
    scale_source_power(did, scale);
}

void POWER_SYSTEM_DATABASE::scale_all_pv_units_power(double scale)
{
    vector<DEVICE_ID> pv_units = get_all_pv_units_device_id();
    size_t n = pv_units.size();
    for(size_t i=0; i!=n; ++i)
        scale_pv_unit_power(pv_units[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_pv_units_power_at_bus(size_t bus, double scale)
{
    vector<DEVICE_ID> pv_units = get_pv_units_device_id_connecting_to_bus(bus);
    size_t n = pv_units.size();
    for(size_t i=0; i!=n; ++i)
        scale_pv_unit_power(pv_units[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_pv_units_power_in_area(size_t area_number, double scale)
{
    vector<DEVICE_ID> pv_units = get_pv_units_device_id_in_area(area_number);
    size_t n = pv_units.size();
    for(size_t i=0; i!=n; ++i)
        scale_pv_unit_power(pv_units[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_pv_units_power_in_zone(size_t zone_number, double scale)
{
    vector<DEVICE_ID> pv_units = get_pv_units_device_id_in_zone(zone_number);
    size_t n = pv_units.size();
    for(size_t i=0; i!=n; ++i)
        scale_pv_unit_power(pv_units[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_energy_storage_power(const DEVICE_ID& did, double scale)
{
    scale_source_power(did, scale);
}

void POWER_SYSTEM_DATABASE::scale_all_energy_storages_power(double scale)
{
    vector<DEVICE_ID> ess = get_all_energy_storages_device_id();
    size_t n = ess.size();
    for(size_t i=0; i!=n; ++i)
        scale_energy_storage_power(ess[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_energy_storages_power_at_bus(size_t bus, double scale)
{
    vector<DEVICE_ID> ess = get_energy_storages_device_id_connecting_to_bus(bus);
    size_t n = ess.size();
    for(size_t i=0; i!=n; ++i)
        scale_energy_storage_power(ess[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_energy_storages_power_in_area(size_t area_number, double scale)
{
    vector<DEVICE_ID> ess = get_energy_storages_device_id_in_area(area_number);
    size_t n = ess.size();
    for(size_t i=0; i!=n; ++i)
        scale_energy_storage_power(ess[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_energy_storages_power_in_zone(size_t zone_number, double scale)
{
    vector<DEVICE_ID> ess = get_energy_storages_device_id_in_zone(zone_number);
    size_t n = ess.size();
    for(size_t i=0; i!=n; ++i)
        scale_energy_storage_power(ess[i], scale);
}

void POWER_SYSTEM_DATABASE::clear_bus(size_t bus)
{
    if(not is_bus_exist(bus)) return;

    clear_sources_connecting_to_bus(bus);
    clear_loads_connecting_to_bus(bus);
    clear_lines_connecting_to_bus(bus);
    clear_transformers_connecting_to_bus(bus);
    clear_fixed_shunts_connecting_to_bus(bus);
    clear_hvdcs_connecting_to_bus(bus);
    clear_equivalent_devices_connecting_to_bus(bus);

    size_t current_index = get_bus_index(bus);

    vector<BUS>::iterator iter = Bus.begin();
    std::advance(iter, current_index);
    Bus.erase(iter);
    bus_index.set_bus_with_index(bus, INDEX_NOT_EXIST);

    size_t max_bus_number = bus_index.get_max_bus_number();

    size_t max_bus_number_plus_1 = max_bus_number+1;
    for(size_t i=1; i!=max_bus_number_plus_1; ++i)
    {
        size_t index = bus_index.get_index_of_bus(i);
        if(index!= INDEX_NOT_EXIST and index>current_index)
            bus_index.set_bus_with_index(i, index-1);
    }
    update_in_service_bus_count();
}

void POWER_SYSTEM_DATABASE::clear_all_buses()
{
    Bus.clear();
    bus_index.clear();

    clear_all_sources();
    clear_all_loads();
    clear_all_lines();
    clear_all_transformers();
    clear_all_fixed_shunts();
    clear_all_hvdcs();
    clear_all_equivalent_devices();
    clear_all_areas();
    clear_all_zones();
    clear_all_owners();
    update_in_service_bus_count();
}


void POWER_SYSTEM_DATABASE::clear_generator(const DEVICE_ID& device_id)
{
    if(not is_generator_exist(device_id)) return;

    size_t current_index = get_generator_index(device_id);

    vector<GENERATOR>::iterator iter_generator = Generator.begin();

    std::advance(iter_generator, current_index);
    Generator.erase(iter_generator);
    generator_index.set_device_index(device_id, INDEX_NOT_EXIST);

    generator_index.decrease_index_by_1_for_device_with_index_greater_than(current_index);
}

void POWER_SYSTEM_DATABASE::clear_generators_connecting_to_bus(const size_t bus)
{
    DEVICE_ID device_id;
    TERMINAL terminal;
    device_id.set_device_type("GENERATOR");
    while(true)
    {
        vector<GENERATOR*> generator = get_generators_connecting_to_bus(bus);
        if(generator.size()==0)
            break;

        terminal.clear();
        terminal.append_bus(bus);
        device_id.set_device_terminal(terminal);
        device_id.set_device_identifier(generator[0]->get_identifier());
        clear_generator(device_id);
    }
}

void POWER_SYSTEM_DATABASE::clear_all_generators()
{
    Generator.clear();
    generator_index.clear();
}


void POWER_SYSTEM_DATABASE::clear_wt_generator(const DEVICE_ID& device_id)
{
    if(not is_wt_generator_exist(device_id)) return;

    size_t current_index = get_wt_generator_index(device_id);

    vector<WT_GENERATOR>::iterator iter_wt_generator = WT_Generator.begin();

    std::advance(iter_wt_generator, current_index);
    WT_Generator.erase(iter_wt_generator);
    wt_generator_index.set_device_index(device_id, INDEX_NOT_EXIST);

    wt_generator_index.decrease_index_by_1_for_device_with_index_greater_than(current_index);
}

void POWER_SYSTEM_DATABASE::clear_wt_generators_connecting_to_bus(const size_t bus)
{
    DEVICE_ID device_id;
    TERMINAL terminal;
    device_id.set_device_type("WT GENERATOR");
    while(true)
    {
        vector<WT_GENERATOR*> wt_generator = get_wt_generators_connecting_to_bus(bus);
        if(wt_generator.size()==0)
            break;
        terminal.clear();
        terminal.append_bus(bus);
        device_id.set_device_terminal(terminal);
        device_id.set_device_identifier(wt_generator[0]->get_identifier());
        clear_wt_generator(device_id);
    }
}

void POWER_SYSTEM_DATABASE::clear_all_wt_generators()
{
    WT_Generator.clear();
    wt_generator_index.clear();
}

void POWER_SYSTEM_DATABASE::clear_pv_unit(const DEVICE_ID& device_id)
{
    if(not is_pv_unit_exist(device_id)) return;

    size_t current_index = get_pv_unit_index(device_id);

    vector<PV_UNIT>::iterator iter_pv_unit = PV_Unit.begin();

    std::advance(iter_pv_unit, current_index);
    PV_Unit.erase(iter_pv_unit);
    pv_unit_index.set_device_index(device_id, INDEX_NOT_EXIST);

    pv_unit_index.decrease_index_by_1_for_device_with_index_greater_than(current_index);
}


void POWER_SYSTEM_DATABASE::clear_pv_units_connecting_to_bus(const size_t bus)
{
    DEVICE_ID device_id;
    TERMINAL terminal;
    device_id.set_device_type("PV UNIT");
    while(true)
    {
        vector<PV_UNIT*> pv_unit = get_pv_units_connecting_to_bus(bus);
        if(pv_unit.size()==0)
            break;
        terminal.clear();
        terminal.append_bus(bus);
        device_id.set_device_terminal(terminal);
        device_id.set_device_identifier(pv_unit[0]->get_identifier());
        clear_pv_unit(device_id);
    }
}

void POWER_SYSTEM_DATABASE::clear_all_pv_units()
{
    PV_Unit.clear();
    pv_unit_index.clear();
}

void POWER_SYSTEM_DATABASE::clear_sources_connecting_to_bus(const size_t bus)
{
    clear_generators_connecting_to_bus(bus);
    clear_wt_generators_connecting_to_bus(bus);
    clear_pv_units_connecting_to_bus(bus);
    clear_energy_storages_connecting_to_bus(bus);
}

void POWER_SYSTEM_DATABASE::clear_all_sources()
{
    clear_all_generators();
    clear_all_wt_generators();
    clear_all_pv_units();
    clear_all_energy_storages();
}

void POWER_SYSTEM_DATABASE::clear_load(const DEVICE_ID& device_id)
{
    if(not is_load_exist(device_id)) return;

    size_t current_index = get_load_index(device_id);

    vector<LOAD>::iterator iter_load = Load.begin();


    std::advance(iter_load, current_index);
    Load.erase(iter_load);
    load_index.set_device_index(device_id, INDEX_NOT_EXIST);

    load_index.decrease_index_by_1_for_device_with_index_greater_than(current_index);
}

void POWER_SYSTEM_DATABASE::clear_loads_connecting_to_bus(const size_t bus)
{
    DEVICE_ID device_id;
    TERMINAL terminal;
    device_id.set_device_type("LOAD");
    while(true)
    {
        vector<LOAD*> load = get_loads_connecting_to_bus(bus);
        if(load.size()==0)
            break;

        terminal.clear();
        terminal.append_bus(bus);
        device_id.set_device_terminal(terminal);
        device_id.set_device_identifier(load[0]->get_identifier());
        clear_load(device_id);
    }
}

void POWER_SYSTEM_DATABASE::clear_all_loads()
{
    Load.clear();
    load_index.clear();
}


void POWER_SYSTEM_DATABASE::clear_line(const DEVICE_ID& device_id)
{
    if(not is_line_exist(device_id)) return;

    size_t current_index = get_line_index(device_id);

    vector<LINE>::iterator iter_line = Line.begin();


    std::advance(iter_line, current_index);
    Line.erase(iter_line);
    line_index.set_device_index(device_id, INDEX_NOT_EXIST);

    line_index.decrease_index_by_1_for_device_with_index_greater_than(current_index);
}

void POWER_SYSTEM_DATABASE::clear_lines_connecting_to_bus(const size_t bus)
{
    DEVICE_ID device_id;
    TERMINAL terminal;
    device_id.set_device_type("LINE");
    while(true)
    {
        vector<LINE*> line = get_lines_connecting_to_bus(bus);
        if(line.size()==0)
            break;

        terminal.clear();
        terminal.append_bus(line[0]->get_sending_side_bus());
        terminal.append_bus(line[0]->get_receiving_side_bus());
        device_id.set_device_terminal(terminal);
        device_id.set_device_identifier(line[0]->get_identifier());
        clear_line(device_id);
    }
}

void POWER_SYSTEM_DATABASE::clear_all_lines()
{
    Line.clear();
    line_index.clear();
}

void POWER_SYSTEM_DATABASE::clear_transformer(const DEVICE_ID& device_id)
{
    if(not is_transformer_exist(device_id)) return;

    size_t current_index = get_transformer_index(device_id);

    vector<TRANSFORMER>::iterator iter_transformer = Transformer.begin();

    std::advance(iter_transformer, current_index);
    Transformer.erase(iter_transformer);
    transformer_index.set_device_index(device_id, INDEX_NOT_EXIST);

    transformer_index.decrease_index_by_1_for_device_with_index_greater_than(current_index);
}

void POWER_SYSTEM_DATABASE::clear_transformers_connecting_to_bus(const size_t bus)
{
    DEVICE_ID device_id;
    TERMINAL terminal;
    device_id.set_device_type("TRANSFORMER");
    while(true)
    {
        vector<TRANSFORMER*> transformer = get_transformers_connecting_to_bus(bus);

        if(transformer.size()==0)
            break;

        terminal.clear();
        terminal.append_bus(transformer[0]->get_winding_bus(PRIMARY_SIDE));
        terminal.append_bus(transformer[0]->get_winding_bus(SECONDARY_SIDE));
        terminal.append_bus(transformer[0]->get_winding_bus(TERTIARY_SIDE));
        device_id.set_device_terminal(terminal);
        device_id.set_device_identifier(transformer[0]->get_identifier());
        clear_transformer(device_id);
    }
}

void POWER_SYSTEM_DATABASE::clear_all_transformers()
{
    Transformer.clear();
    transformer_index.clear();
}


void POWER_SYSTEM_DATABASE::clear_fixed_shunt(const DEVICE_ID& device_id)
{
    if(not is_fixed_shunt_exist(device_id))
        return;

    size_t current_index = get_fixed_shunt_index(device_id);

    vector<FIXED_SHUNT> fs;
    size_t nfshunt = get_fixed_shunt_count();
    for(size_t i=0; i!=nfshunt; ++i)
        if(i!=current_index) fs.push_back(Fixed_shunt[i]);

    Fixed_shunt.clear();
    Fixed_shunt = fs;

    fixed_shunt_index.set_device_index(device_id, INDEX_NOT_EXIST);

    fixed_shunt_index.decrease_index_by_1_for_device_with_index_greater_than(current_index);
}


void POWER_SYSTEM_DATABASE::clear_fixed_shunts_connecting_to_bus(const size_t bus)
{
    DEVICE_ID device_id;
    device_id.set_device_type("FIXED SHUNT");
    TERMINAL terminal;
    terminal.append_bus(bus);
    device_id.set_device_terminal(terminal);
    while(true)
    {
        vector<FIXED_SHUNT*> fixed_shunt = get_fixed_shunts_connecting_to_bus(bus);
        if(fixed_shunt.size()==0)
            break;

        device_id.set_device_identifier(fixed_shunt[0]->get_identifier());
        clear_fixed_shunt(device_id);
        fixed_shunt.clear();
    }
}

void POWER_SYSTEM_DATABASE::clear_all_fixed_shunts()
{
    Fixed_shunt.clear();
    fixed_shunt_index.clear();
}

void POWER_SYSTEM_DATABASE::clear_hvdc(const DEVICE_ID& device_id)
{
    if(not is_hvdc_exist(device_id)) return;

    size_t current_index = get_hvdc_index(device_id);

    vector<HVDC>::iterator iter_hvdc = Hvdc.begin();


    std::advance(iter_hvdc, current_index);
    Hvdc.erase(iter_hvdc);
    hvdc_index.set_device_index(device_id, INDEX_NOT_EXIST);

    hvdc_index.decrease_index_by_1_for_device_with_index_greater_than(current_index);
}

void POWER_SYSTEM_DATABASE::clear_hvdcs_connecting_to_bus(const size_t bus)
{
    DEVICE_ID device_id;
    TERMINAL terminal;
    device_id.set_device_type("HVDC");
    while(true)
    {
        vector<HVDC*> hvdc = get_hvdcs_connecting_to_bus(bus);
        if(hvdc.size()==0)
            break;

        terminal.clear();
        terminal.append_bus(hvdc[0]->get_converter_bus(RECTIFIER));
        terminal.append_bus(hvdc[0]->get_converter_bus(INVERTER));
        device_id.set_device_terminal(terminal);
        device_id.set_device_identifier(hvdc[0]->get_identifier());
        clear_hvdc(device_id);
    }
}

void POWER_SYSTEM_DATABASE::clear_all_hvdcs()
{
    Hvdc.clear();
    hvdc_index.clear();
}

void POWER_SYSTEM_DATABASE::clear_equivalent_device(const DEVICE_ID& device_id)
{
    if(not is_equivalent_device_exist(device_id)) return;

    size_t current_index = get_equivalent_device_index(device_id);

    vector<EQUIVALENT_DEVICE>::iterator iter_edevice = Equivalent_device.begin();


    std::advance(iter_edevice, current_index);
    Equivalent_device.erase(iter_edevice);
    equivalent_device_index.set_device_index(device_id, INDEX_NOT_EXIST);

    equivalent_device_index.decrease_index_by_1_for_device_with_index_greater_than(current_index);
}

void POWER_SYSTEM_DATABASE::clear_equivalent_devices_connecting_to_bus(const size_t bus)
{
    DEVICE_ID device_id;
    TERMINAL terminal;
    device_id.set_device_type("EQUIVALENT DEVICE");
    while(true)
    {
        vector<EQUIVALENT_DEVICE*> edevice = get_equivalent_devices_connecting_to_bus(bus);
        if(edevice.size()==0)
            break;

        terminal.clear();
        terminal.append_bus(edevice[0]->get_equivalent_device_bus());
        device_id.set_device_terminal(terminal);
        device_id.set_device_identifier(edevice[0]->get_identifier());
        clear_equivalent_device(device_id);
    }
}

void POWER_SYSTEM_DATABASE::clear_all_equivalent_devices()
{
    Equivalent_device.clear();
    equivalent_device_index.clear();
}


void POWER_SYSTEM_DATABASE::clear_energy_storage(const DEVICE_ID& device_id)
{
    if(not is_energy_storage_exist(device_id)) return;

    size_t current_index = get_energy_storage_index(device_id);

    vector<ENERGY_STORAGE>::iterator iter_edevice = Energy_storage.begin();


    std::advance(iter_edevice, current_index);
    Energy_storage.erase(iter_edevice);
    energy_storage_index.set_device_index(device_id, INDEX_NOT_EXIST);

    energy_storage_index.decrease_index_by_1_for_device_with_index_greater_than(current_index);
}

void POWER_SYSTEM_DATABASE::clear_energy_storages_connecting_to_bus(const size_t bus)
{
    DEVICE_ID device_id;
    TERMINAL terminal;
    device_id.set_device_type("ENERGY STORAGE");
    while(true)
    {
        vector<ENERGY_STORAGE*> estorage = get_energy_storages_connecting_to_bus(bus);
        if(estorage.size()==0)
            break;

        terminal.clear();
        terminal.append_bus(estorage[0]->get_energy_storage_bus());
        device_id.set_device_terminal(terminal);
        device_id.set_device_identifier(estorage[0]->get_identifier());
        clear_energy_storage(device_id);
    }
}

void POWER_SYSTEM_DATABASE::clear_all_energy_storages()
{
    Energy_storage.clear();
    energy_storage_index.clear();
}

void POWER_SYSTEM_DATABASE::clear_area(size_t area)
{
    ;
}

void POWER_SYSTEM_DATABASE::clear_all_areas()
{
    Area.clear();
    area_index.clear();
}

void POWER_SYSTEM_DATABASE::clear_zone(size_t zone)
{
    ;
}

void POWER_SYSTEM_DATABASE::clear_all_zones()
{
    Zone.clear();
    zone_index.clear();
}

void POWER_SYSTEM_DATABASE::clear_owner(size_t owner)
{
    ;
}

void POWER_SYSTEM_DATABASE::clear_all_owners()
{
    Owner.clear();
    owner_index.clear();
}


void POWER_SYSTEM_DATABASE::trip_bus(size_t bus)
{
    if(not is_bus_exist(bus)) return;

    size_t index = get_bus_index(bus);
    if(Bus[index].get_bus_type()==OUT_OF_SERVICE)
        return;

    ostringstream osstream;

    osstream<<"The following devices are tripped before bus "<<bus<<" is tripped."<<endl;

    size_t n = 0;

    vector<GENERATOR*> gens = get_generators_connecting_to_bus(bus);
    n=gens.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(gens[i]->get_status()==true)
        {
            gens[i]->set_status(false);
            osstream<<gens[i]->get_device_name()<<endl;
        }
    }

    vector<WT_GENERATOR*> wts = get_wt_generators_connecting_to_bus(bus);
    n=wts.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(wts[i]->get_status()==true)
        {
            wts[i]->set_status(false);
            osstream<<wts[i]->get_device_name()<<endl;
        }
    }

    vector<PV_UNIT*> pvs = get_pv_units_connecting_to_bus(bus);
    n=pvs.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(pvs[i]->get_status()==true)
        {
            pvs[i]->set_status(false);
            osstream<<pvs[i]->get_device_name()<<endl;
        }
    }

    vector<ENERGY_STORAGE*> ess = get_energy_storages_connecting_to_bus(bus);
    n=ess.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(ess[i]->get_status()==true)
        {
            ess[i]->set_status(false);
            osstream<<ess[i]->get_device_name()<<endl;
        }
    }

    vector<LOAD*> loads = get_loads_connecting_to_bus(bus);
    n=loads.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(loads[i]->get_status()==true)
        {
            loads[i]->set_status(false);
            osstream<<loads[i]->get_device_name()<<endl;
        }
    }

    vector<FIXED_SHUNT*> fshunts = get_fixed_shunts_connecting_to_bus(bus);
    n=fshunts.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(fshunts[i]->get_status()==true)
        {
            fshunts[i]->set_status(false);
            osstream<<fshunts[i]->get_device_name()<<endl;
        }
    }

    vector<LINE*> lines = get_lines_connecting_to_bus(bus);
    n=lines.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(lines[i]->get_sending_side_breaker_status()==true or lines[i]->get_receiving_side_breaker_status()==true)
        {
            lines[i]->set_sending_side_breaker_status(false);
            lines[i]->set_receiving_side_breaker_status(false);
            osstream<<lines[i]->get_device_name()<<endl;
        }
    }

    vector<TRANSFORMER*> transes = get_transformers_connecting_to_bus(bus);
    n=transes.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(transes[i]->get_winding_breaker_status(PRIMARY_SIDE)==true or transes[i]->get_winding_breaker_status(SECONDARY_SIDE)==true or
           (transes[i]->is_three_winding_transformer() and transes[i]->get_winding_breaker_status(TERTIARY_SIDE)==true) )
        {
            transes[i]->set_winding_breaker_status(PRIMARY_SIDE, false);
            transes[i]->set_winding_breaker_status(SECONDARY_SIDE, false);
            if(transes[i]->is_three_winding_transformer())
                transes[i]->set_winding_breaker_status(TERTIARY_SIDE, false);

            osstream<<transes[i]->get_device_name()<<endl;
        }
    }

    vector<HVDC*> hvdcs = get_hvdcs_connecting_to_bus(bus);
    n=hvdcs.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(hvdcs[i]->get_status()==true)
        {
            hvdcs[i]->set_status(false);
            osstream<<hvdcs[i]->get_device_name()<<endl;
        }
    }

    vector<EQUIVALENT_DEVICE*> edevices = get_equivalent_devices_connecting_to_bus(bus);
    n=edevices.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(edevices[i]->get_status()==true)
        {
            edevices[i]->set_status(false);
            osstream<<edevices[i]->get_device_name()<<endl;
        }
    }

    Bus[index].set_bus_type(OUT_OF_SERVICE);
    osstream<<"Bus "<<bus<<" is tripped.";
    show_information_with_leading_time_stamp(osstream);

    update_in_service_bus_count();
}

void POWER_SYSTEM_DATABASE::check_device_status_for_out_of_service_buses()
{
    vector<BUS*> buses = get_all_buses();
    BUS* bus;
    size_t nbus = buses.size();
    for(size_t i=0; i!=nbus; ++i)
    {
        bus = buses[i];
        if(bus->get_bus_type()==OUT_OF_SERVICE)
        {
            size_t bus_number = bus->get_bus_number();
            check_source_status_for_out_of_service_bus(bus_number);
            check_load_status_for_out_of_service_bus(bus_number);
            check_fixed_shunt_status_for_out_of_service_bus(bus_number);
            check_line_status_for_out_of_service_bus(bus_number);
            check_transformer_status_for_out_of_service_bus(bus_number);
            check_hvdc_status_for_out_of_service_bus(bus_number);
        }
    }
}

void POWER_SYSTEM_DATABASE::check_source_status_for_out_of_service_bus(size_t bus)
{
    vector<SOURCE*> sources = get_sources_connecting_to_bus(bus);
    SOURCE* source;
    size_t nsource = sources.size();
    for(size_t i=0; i!=nsource; ++i)
    {
        source = sources[i];
        if(source->get_status()==true)
            source->set_status(false);
    }
}

void POWER_SYSTEM_DATABASE::check_load_status_for_out_of_service_bus(size_t bus)
{
    vector<LOAD*> loads = get_loads_connecting_to_bus(bus);
    LOAD* load;
    size_t nload = loads.size();
    for(size_t i=0; i!=nload; ++i)
    {
        load = loads[i];
        if(load->get_status()==true)
            load->set_status(false);
    }
}

void POWER_SYSTEM_DATABASE::check_fixed_shunt_status_for_out_of_service_bus(size_t bus)
{
    vector<FIXED_SHUNT*> shunts = get_fixed_shunts_connecting_to_bus(bus);
    FIXED_SHUNT* shunt;
    size_t nshunt = shunts.size();
    for(size_t i=0; i!=nshunt; ++i)
    {
        shunt = shunts[i];
        if(shunt->get_status()==true)
            shunt->set_status(false);
    }
}

void POWER_SYSTEM_DATABASE::check_line_status_for_out_of_service_bus(size_t bus)
{
    vector<LINE*> lines = get_lines_connecting_to_bus(bus);
    LINE* line;
    size_t nline = lines.size();
    for(size_t i=0; i!=nline; ++i)
    {
        line = lines[i];
        if(line->get_sending_side_bus()==bus)
        {
            if(line->get_sending_side_breaker_status()==true)
                line->set_sending_side_breaker_status(false);
        }
        else
        {
            if(line->get_receiving_side_breaker_status()==true)
                line->set_receiving_side_breaker_status(false);
        }
    }
}

void POWER_SYSTEM_DATABASE::check_transformer_status_for_out_of_service_bus(size_t bus)
{
    vector<TRANSFORMER*> transformers = get_transformers_connecting_to_bus(bus);
    TRANSFORMER* transformer;
    size_t ntrans = transformers.size();
    for(size_t i=0; i!=ntrans; ++i)
    {
        transformer = transformers[i];
        if(transformer->get_winding_bus(PRIMARY_SIDE)==bus)
        {
            if(transformer->get_winding_breaker_status(PRIMARY_SIDE)==true)
                transformer->set_winding_breaker_status(PRIMARY_SIDE, false);
        }
        else
        {
            if(transformer->get_winding_bus(SECONDARY_SIDE)==bus)
            {
                if(transformer->get_winding_breaker_status(SECONDARY_SIDE)==true)
                    transformer->set_winding_breaker_status(SECONDARY_SIDE, false);
            }
            else
            {
                if(transformer->get_winding_breaker_status(TERTIARY_SIDE)==true)
                    transformer->set_winding_breaker_status(TERTIARY_SIDE, false);
            }
        }
    }
}

void POWER_SYSTEM_DATABASE::check_hvdc_status_for_out_of_service_bus(size_t bus)
{
    vector<HVDC*> hvdcs = get_hvdcs_connecting_to_bus(bus);
    HVDC* hvdc;
    size_t nhvdc = hvdcs.size();
    for(size_t i=0; i!=nhvdc; ++i)
    {
        hvdc = hvdcs[i];
        if(hvdc->get_status()==true)
            hvdc->set_status(false);
    }
}


void POWER_SYSTEM_DATABASE::calibrate_sources_pmax_and_pmin_with_dynamic_model()
{
    calibrate_generators_pmax_and_pmin_with_turbine_governor_model();
}

void POWER_SYSTEM_DATABASE::calibrate_generators_pmax_and_pmin_with_turbine_governor_model()
{
    vector<GENERATOR*> generators = get_all_generators();
    size_t n = generators.size();
    GENERATOR* generator;
    for(size_t i=0; i!=n; ++i)
    {
        generator = generators[i];
        double mbase = generator->get_mbase_in_MVA();
        TURBINE_GOVERNOR_MODEL* model = generator->get_turbine_governor_model();
        if(model!=NULL)
        {
            double pmax = model->get_mechanical_power_upper_limit_in_pu_based_on_mbase()*mbase;
            double pmin = model->get_mechanical_power_lower_limit_in_pu_based_on_mbase()*mbase;
            generator->set_p_max_in_MW(pmax);
            generator->set_p_min_in_MW(pmin);
        }
    }
}

complex<double> POWER_SYSTEM_DATABASE::get_bus_complex_voltage_in_pu(size_t bus)
{
    BUS* busptr = get_bus(bus);
    if(busptr!=NULL)
        return busptr->get_complex_voltage_in_pu();
    else
        return 0.0;

}

complex<double> POWER_SYSTEM_DATABASE::get_bus_complex_voltage_in_kV(size_t bus)
{
    BUS* busptr = get_bus(bus);
    if(busptr!=NULL)
        return busptr->get_complex_voltage_in_kV();
    else
        return 0.0;
}

double POWER_SYSTEM_DATABASE::get_bus_base_frequency_in_Hz(size_t bus)
{
    BUS* busptr = get_bus(bus);
    if(busptr!=NULL)
        return busptr->get_base_frequency_in_Hz();
    else
        return 0.0;
}

double POWER_SYSTEM_DATABASE::get_bus_base_voltage_in_kV(size_t bus)
{
    BUS* busptr = get_bus(bus);
    if(busptr!=NULL)
        return busptr->get_base_voltage_in_kV();
    else
        return 0.0;
}

double POWER_SYSTEM_DATABASE::get_bus_voltage_in_pu(size_t bus)
{
    BUS* busptr = get_bus(bus);
    if(busptr!=NULL)
        return busptr->get_voltage_in_pu();
    else
        return 0.0;
}

double POWER_SYSTEM_DATABASE::get_bus_voltage_in_kV(size_t bus)
{
    BUS* busptr = get_bus(bus);
    if(busptr!=NULL)
        return busptr->get_voltage_in_kV();
    else
        return 0.0;
}

double POWER_SYSTEM_DATABASE::get_bus_angle_in_deg(size_t bus)
{
    return rad2deg(get_bus_angle_in_rad(bus));
}

double POWER_SYSTEM_DATABASE::get_bus_angle_in_rad(size_t bus)
{
    BUS* busptr = get_bus(bus);
    if(busptr!=NULL)
        return busptr->get_angle_in_rad();
    else
        return 0.0;
}


double POWER_SYSTEM_DATABASE::get_bus_frequency_deviation_in_pu(size_t bus)
{
    BUS* busptr = get_bus(bus);
    if(busptr!=NULL)
    {
        BUS_FREQUENCY_MODEL* fmodel = busptr->get_bus_frequency_model();

        return fmodel->get_frequency_deviation_in_pu();
    }
    else
        return 0.0;
}

double POWER_SYSTEM_DATABASE::get_bus_frequency_deviation_in_Hz(size_t bus)
{
    BUS* busptr = get_bus(bus);
    if(busptr!=NULL)
    {
        BUS_FREQUENCY_MODEL* fmodel = busptr->get_bus_frequency_model();

        return fmodel->get_frequency_deviation_in_Hz();
    }
    else
        return 0.0;
}

double POWER_SYSTEM_DATABASE::get_bus_frequency_in_Hz(size_t bus)
{
    BUS* busptr = get_bus(bus);
    if(busptr!=NULL)
    {
        BUS_FREQUENCY_MODEL* fmodel = busptr->get_bus_frequency_model();

        return fmodel->get_frequency_in_Hz();
    }
    else
        return 0.0;
}

double POWER_SYSTEM_DATABASE::get_bus_frequency_in_pu(size_t bus)
{
    BUS* busptr = get_bus(bus);
    if(busptr!=NULL)
    {
        BUS_FREQUENCY_MODEL* fmodel = busptr->get_bus_frequency_model();

        return fmodel->get_frequency_in_pu();
    }
    else
        return 0.0;
}

double POWER_SYSTEM_DATABASE::get_voltage_to_regulate_of_physical_bus_in_pu(size_t bus)
{
    BUS* busptr = get_bus(bus);
    if(busptr!=NULL)
        return busptr->get_voltage_to_regulate_in_pu();
    else
        return 0.0;
}

double POWER_SYSTEM_DATABASE::get_regulatable_p_max_at_physical_bus_in_MW(size_t bus)
{
    if(not is_bus_exist(bus))
        return 0.0;

    BUS* busptr = get_bus(bus);
    BUS_TYPE btype = busptr->get_bus_type();
    if(btype!=SLACK_TYPE)
        return 0.0;
    else
    {
        double total_p_max_in_MW = 0.0;

        vector<SOURCE*> sources = get_sources_connecting_to_bus(bus);

        size_t n = sources.size();
        for(size_t i=0; i!=n; ++i)
        {
            if(sources[i]->get_status() == false)
                continue;
            total_p_max_in_MW += sources[i]->get_p_max_in_MW();
        }
        return total_p_max_in_MW;
    }
}

double POWER_SYSTEM_DATABASE::get_regulatable_p_min_at_physical_bus_in_MW(size_t bus)
{
    if(not is_bus_exist(bus))
        return 0.0;

    BUS* busptr = get_bus(bus);
    BUS_TYPE btype = busptr->get_bus_type();
    if(btype!=SLACK_TYPE)
        return 0.0;
    else
    {
        double total_p_min_in_MW = 0.0;

        vector<SOURCE*> sources = get_sources_connecting_to_bus(bus);

        size_t n = sources.size();
        for(size_t i=0; i!=n; ++i)
        {
            if(sources[i]->get_status() == false)
                continue;
            total_p_min_in_MW += sources[i]->get_p_min_in_MW();
        }
        return total_p_min_in_MW;
    }
}

double POWER_SYSTEM_DATABASE::get_regulatable_q_max_at_physical_bus_in_MVar(size_t bus)
{
    if(not is_bus_exist(bus))
        return 0.0;

    BUS* busptr = get_bus(bus);
    BUS_TYPE btype = busptr->get_bus_type();
    if(btype!=SLACK_TYPE and btype!=PV_TYPE
       and btype!=PV_TO_PQ_TYPE_1 and btype!=PV_TO_PQ_TYPE_2
       and btype!=PV_TO_PQ_TYPE_3 and btype!=PV_TO_PQ_TYPE_4)
        return 0.0;
    else
    {
        double total_q_max_in_MVar = 0.0;

        vector<SOURCE*> sources = get_sources_connecting_to_bus(bus);

        size_t n = sources.size();
        for(size_t i=0; i!=n; ++i)
        {
            if(sources[i]->get_status() == false)
                continue;
            total_q_max_in_MVar += sources[i]->get_q_max_in_MVar();
        }
        return total_q_max_in_MVar;
    }
}

double POWER_SYSTEM_DATABASE::get_regulatable_q_min_at_physical_bus_in_MVar(size_t bus)
{
    if(not is_bus_exist(bus))
        return 0.0;

    BUS* busptr = get_bus(bus);
    BUS_TYPE btype = busptr->get_bus_type();
    if(btype!=SLACK_TYPE and btype!=PV_TYPE
       and btype!=PV_TO_PQ_TYPE_1 and btype!=PV_TO_PQ_TYPE_2
       and btype!=PV_TO_PQ_TYPE_3 and btype!=PV_TO_PQ_TYPE_4)
        return 0.0;
    else
    {
        double total_q_min_in_MVar = 0.0;

        vector<SOURCE*> sources = get_sources_connecting_to_bus(bus);

        size_t n = sources.size();
        for(size_t i=0; i!=n; ++i)
        {
            if(sources[i]->get_status() == false)
                continue;
            total_q_min_in_MVar += sources[i]->get_q_min_in_MVar();
        }
        return total_q_min_in_MVar;
    }
}

double POWER_SYSTEM_DATABASE::get_total_regulating_p_generation_at_physical_bus_in_MW(size_t bus)
{
    if(not is_bus_exist(bus))
        return 0.0;

    BUS* busptr = get_bus(bus);
    BUS_TYPE btype = busptr->get_bus_type();
    if(btype!=SLACK_TYPE)
        return 0.0;
    else
    {
        double total_p_in_MW = 0.0;

        vector<SOURCE*> sources = get_sources_connecting_to_bus(bus);

        size_t n = sources.size();
        for(size_t i=0; i!=n; ++i)
        {
            if(sources[i]->get_status() == false)
                continue;
            total_p_in_MW += sources[i]->get_p_generation_in_MW();
        }
        return total_p_in_MW;
    }
}

double POWER_SYSTEM_DATABASE::get_total_regulating_q_generation_at_physical_bus_in_MVar(size_t bus)
{

    if(not is_bus_exist(bus))
        return 0.0;

    BUS* busptr = get_bus(bus);
    BUS_TYPE btype = busptr->get_bus_type();
    if(btype!=SLACK_TYPE and btype!=PV_TYPE
       and btype!=PV_TO_PQ_TYPE_1 and btype!=PV_TO_PQ_TYPE_2
       and btype!=PV_TO_PQ_TYPE_3 and btype!=PV_TO_PQ_TYPE_4)
        return 0.0;
    else
    {
        double total_q_in_MVar = 0.0;

        vector<SOURCE*> sources = get_sources_connecting_to_bus(bus);

        size_t n = sources.size();
        for(size_t i=0; i!=n; ++i)
        {
            if(sources[i]->get_status() == false)
                continue;
            total_q_in_MVar += sources[i]->get_q_generation_in_MVar();
        }
        return total_q_in_MVar;
    }
}

double POWER_SYSTEM_DATABASE::get_total_p_generation_at_physical_bus_in_MW(size_t bus)
{
    double total_p_in_MW = 0.0;

    vector<SOURCE*> sources = get_sources_connecting_to_bus(bus);

    size_t n = sources.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(sources[i]->get_status() == false)
            continue;

        total_p_in_MW += sources[i]->get_p_generation_in_MW();
    }
    return total_p_in_MW;
}

double POWER_SYSTEM_DATABASE::get_total_q_generation_at_physical_bus_in_MVar(size_t bus)
{
    double total_q_in_MVar = 0.0;

    vector<SOURCE*> sources = get_sources_connecting_to_bus(bus);

    size_t n = sources.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(sources[i]->get_status() == false)
            continue;

        total_q_in_MVar += sources[i]->get_q_generation_in_MVar();
    }
    return total_q_in_MVar;
}


complex<double> POWER_SYSTEM_DATABASE::get_total_load_power_in_MVA()
{
    size_t n = get_load_count();
    complex<double> S=0.0;
    for(size_t i=0; i!=n; ++i)
    {
        if(Load[i].get_status()==true)
            S += Load[i].get_actual_total_load_in_MVA();
    }
    return S;
}

complex<double> POWER_SYSTEM_DATABASE::get_total_generation_power_in_MVA()
{
    size_t n = get_generator_count();
    complex<double> S=0.0;
    for(size_t i=0; i!=n; ++i)
    {
        if(Generator[i].get_status()==true)
            S += Generator[i].get_complex_generation_in_MVA();
    }
    n = get_wt_generator_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(WT_Generator[i].get_status()==true)
            S += WT_Generator[i].get_complex_generation_in_MVA();
    }
    return S;
}

complex<double> POWER_SYSTEM_DATABASE::get_total_loss_power_in_MVA()
{
    return get_total_generation_power_in_MVA()-get_total_load_power_in_MVA();
}

